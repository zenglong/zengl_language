/*
   Copyright 2012 zenglong (made in china)

   For more information, please see www.zengl.com

   This file is part of zengl language.

   zengl language is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   zengl language is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with zengl language,the copy is in the file licence.txt.  If not,
   see <http://www.gnu.org/licenses/>.
*/

/**
	���ļ�Ϊ�����������صĺ�����
*/

#include "zengl_global.h"

/*�������ı��벿��*/
ZL_INT zenglDebug_Compile(ZL_VOID * VM_ARG,ZL_CHAR * script_file,ZENGL_EXPORT_VM_MAIN_ARGS * vm_main_args)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_COMPILE_TYPE * compile = &VM->compile;
	ZENGL_RUN_TYPE * run = &VM->run;
	ZENGL_TOKENTYPE token;
	ZL_INT retcode;
	ZL_INT isNeedDebugInfo = ZL_EXP_CP_AF_IN_DEBUG_MODE | ZL_EXP_CP_AF_OUTPUT_DEBUG_INFO;

	compile->init(VM_ARG,script_file,vm_main_args);
	if((retcode = ZENGL_SYS_JMP_SETJMP(compile->jumpBuffer))==0)
	{
		while(ZL_TRUE)
		{
			token = compile->getToken(VM_ARG);
			if(token == ZL_TK_RESERVE || token == ZL_TK_ERROR)
			{
				compile->exit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_TOKEN,
					compile->line_no,compile->col_no,compile->source.filename,compile->tokenInfo.str);
			}
			else if(token != ZL_TK_ENDFILE)
			{
				compile->ASTAddNode(VM_ARG,token);
				compile->freeTokenStr(VM_ARG);
				continue;
			}
			else
				break;
		}
		compile->buildAST(VM_ARG);		//�齨AST�����﷨��
		compile->buildAsmCode(VM_ARG);	//�齨������
		compile->LDAddrListReplaceAll(VM_ARG); //�����е�α��ַ�滻Ϊ��ʵ�Ļ�����λ�ã��Ӷ�������ӹ���	
		if((VM->vm_main_args->flags & isNeedDebugInfo) == isNeedDebugInfo) //�û��Զ���ĵ���ģʽ��
		{
			compile->printASTnodes(VM_ARG); //��ӡAST�����﷨���Ľڵ���Ϣ
			run->printInstList(VM_ARG,"\n[zenglrun assemble code]:\n"); //���ý������ĺ����������ʾָ������ڷ�������
		}
		compile->exit(VM_ARG,ZL_NO_ERR_SUCCESS);
	}
	if(retcode == 1)
		return 0;
	else
		return -1;
	return 0;
}

/*�������Ľ���ִ�в���*/
ZL_INT zenglDebug_Run(ZL_VOID * VM_ARG)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_VM_TYPE * PVM = VM->debug.DeubugPVM;
	ZENGL_RUN_TYPE * run = &VM->run;
	ZL_INT retcode;
	ZENGL_RUN_MEM_POOL_TYPE orig_mempool = run->mempool;
	ZENGL_RUN_MEM_FREE_POOL_TYPE orig_memfreepool = run->memfreepool; //���������ڴ��ͷų�
	ZENGL_RUN_VIRTUAL_MEM_LIST orig_vmem_list = run->vmem_list;
	ZENGL_RUN_VIRTUAL_MEM_LIST orig_vstack_list = run->vstack_list;
	ZENGL_RUN_MOD_FUN_TABLE orig_ModFunTable = run->ModFunTable;
	ZL_INT pvm_orig_vstack_list_count = PVM->run.vstack_list.count;
	VM->debug.orig_run_totalsize = run->totalsize;
	VM->debug.orig_vm_totalsize = VM->totalsize;
	ZENGL_SYS_MEM_COPY(&VM->run.HashTable,&PVM->run.HashTable,ZL_R_HASH_TOTAL_SIZE * sizeof(ZL_INT));
	run->mempool = PVM->run.mempool;
	run->memfreepool = PVM->run.memfreepool;
	run->vmem_list = PVM->run.vmem_list;
	run->vstack_list = PVM->run.vstack_list;
	run->ModFunTable = PVM->run.ModFunTable;
	run->totalsize = PVM->run.totalsize;
	run->isinRunning = ZL_TRUE;
	run->start_time = ZENGL_SYS_TIME_CLOCK();
	if((retcode = ZENGL_SYS_JMP_SETJMP(run->jumpBuffer))==0)
	{
		run->RunInsts(VM_ARG);
		if(VM->isUseApiSetErrThenStop == ZL_TRUE) //���ͨ��zenglApi_SetErrThenStop�ӿ���ֹͣ������ģ���ͨ��exit_forApiSetErrThenStop���˳�
			run->exit_forApiSetErrThenStop(VM_ARG);
		else
			run->exit(VM_ARG,ZL_NO_ERR_SUCCESS);
	}
	if(PVM->run.reg_list[ZL_R_RT_DEBUG].val.str != ZL_NULL)
		run->memFreeIndex(VM_ARG,PVM->run.reg_list[ZL_R_RT_DEBUG].val.str,&PVM->run.reg_list[ZL_R_RT_DEBUG].str_Index);
	if(ZENGL_RUN_REGVAL(ZL_R_RT_BX).str != ZL_NULL)
		run->memFreeIndex(VM_ARG,ZENGL_RUN_REGVAL(ZL_R_RT_BX).str,&ZENGL_RUN_REG(ZL_R_RT_BX).str_Index);
	PVM->run.reg_list[ZL_R_RT_DEBUG] = run->reg_list[ZL_R_RT_AX];
	PVM->totalsize += run->totalsize - PVM->run.totalsize;
	PVM->run.totalsize = run->totalsize;
	PVM->run.mempool = run->mempool;
	PVM->run.memfreepool = run->memfreepool;
	PVM->run.vmem_list = run->vmem_list;
	PVM->run.vstack_list = run->vstack_list;
	PVM->run.vstack_list.count = pvm_orig_vstack_list_count;
	run->mempool = orig_mempool;
	run->memfreepool = orig_memfreepool; //���������ڴ��ͷų�
	run->vmem_list = orig_vmem_list;
	run->vstack_list = orig_vstack_list;
	run->ModFunTable = orig_ModFunTable;
	run->totalsize = VM->debug.orig_run_totalsize;
	VM->totalsize = VM->debug.orig_vm_totalsize;
	if(retcode == 1)
		return 0;
	else
		return -1;
	return 0;
}

/*���������滻�곣���ĺ���*/
ZENGL_TOKENTYPE zenglDebug_ReplaceDefConst(ZL_VOID * VM_ARG, ZENGL_TOKENTYPE token)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_VM_TYPE * PVM = VM->debug.DeubugPVM;
	ZENGL_COMPILE_TYPE * compile = &VM->compile;
	ZL_INT index = compile->lookupDefTable(VM_ARG,compile->tokenInfo.str);
	ZL_INT len;
	ZL_CHAR * valstr;
	if(index == 0)
		return token;
	token = compile->def_table.defs[index].tokentype;
	valstr = compile->DefPoolGetPtr(PVM,compile->def_table.defs[index].valIndex);
	len = ZENGL_SYS_STRLEN(valstr);
	if(compile->tokenInfo.size < len + 1)
	{
		while(compile->tokenInfo.size < len + 1)
			compile->tokenInfo.size += ZL_TOKENSTR_SIZE;
		compile->tokenInfo.str = compile->memReAlloc(VM_ARG,compile->tokenInfo.str,compile->tokenInfo.size);
		if(compile->tokenInfo.str == ZL_NULL)
			compile->exit(VM_ARG,ZL_ERR_CP_DEF_TABLE_TK_STR_REALLOC_FAILED_WHEN_REPLACE);
	}
	ZENGL_SYS_STRNCPY(compile->tokenInfo.str,valstr,len);
	compile->tokenInfo.str[len] = ZL_STRNULL;
	compile->tokenInfo.count = len;
	compile->tokenInfo.cur = len;
	return token;
}

/*���������Һ궨����滻����*/
ZL_INT zenglDebug_lookupDefTable(ZL_VOID * VM_ARG, ZL_CHAR * name)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_VM_TYPE * PVM = VM->debug.DeubugPVM;
	ZENGL_COMPILE_TYPE * compile = &VM->compile;
	ZL_INT h;
	ZL_INT tmpindex;
	if(!compile->def_table.isInit)
		compile->initDefTable(VM_ARG);
	h = compile->hash(VM_ARG,name) +  + ZL_SYM_HASH_SIZE * ZL_HASH_TYPE_DEF_TABLE;
	tmpindex = compile->HashTable[h];
	while(tmpindex != 0 && compile->def_table.defs[tmpindex].isvalid == ZL_TRUE &&
		ZENGL_SYS_STRCMP(name,compile->DefPoolGetPtr(PVM,compile->def_table.defs[tmpindex].nameIndex)) != 0)
		tmpindex = compile->def_table.defs[tmpindex].next;
	if(tmpindex == 0)
		return 0;
	else if(compile->def_table.defs[tmpindex].isvalid == ZL_FALSE)
		compile->exit(VM_ARG,ZL_ERR_CP_DEF_TABLE_INVALID_INDEX_WHEN_LOOKUP);
	else
		return tmpindex;
	return 0;
}

/**
	������ͨ���ڵ����������Ҹýڵ�ı�����ʾ�����Զ���������ڴ��е��ڴ��ַ���������ڴ��ַ��ָ���������ʽ	
*/
ZENGL_RUN_INST_OP_DATA zenglDebug_SymLookupID(ZL_VOID * VM_ARG,ZL_INT nodenum)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_VM_TYPE * PVM = VM->debug.DeubugPVM;
	ZENGL_COMPILE_TYPE * compile = &VM->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_AST_NODE_TYPE * node = nodes + nodenum;
	ZL_CHAR * name = compile->TokenStringPoolGetPtr(VM_ARG,nodes[nodenum].strindex);
	ZL_INT h;
	ZL_INT tmpindex;
	ZENGL_RUN_INST_OP_DATA ret_data;
	
	if(!compile->gencode_struct.is_inFun)
	{
FindGlobalSymLoc:
		h = compile->hash(VM_ARG,name) + ZL_SYM_HASH_SIZE * ZL_HASH_TYPE_SYM_TABLE;
		tmpindex = compile->HashTable[h];
		while(tmpindex != 0 && compile->SymGlobalTable.sym[tmpindex].isvalid == ZL_TRUE &&
			  ZENGL_SYS_STRCMP(name,compile->TokenStringPoolGetPtr(PVM,compile->SymGlobalTable.sym[tmpindex].nameIndex)) != 0)
			tmpindex = compile->SymGlobalTable.sym[tmpindex].next;
		if(tmpindex == 0)
		{
			compile->parser_curnode = nodenum;
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYM_CAN_NOT_FIND_ID_MEMLOC,name,name);
		}
		else if(compile->SymGlobalTable.sym[tmpindex].isvalid == ZL_FALSE)
			compile->exit(VM_ARG,ZL_ERR_CP_SYM_GLOBAL_TABLE_FIND_NOT_VALID_INDEX_WHEN_LOOKUP);
		ret_data.type = ZL_R_DT_MEM;
		ret_data.val.mem = compile->SymGlobalTable.sym[tmpindex].memloc;
		return ret_data;
	}
	else
	{
		h = compile->hash(VM_ARG,name) + ZL_SYM_HASH_SIZE * ZL_HASH_TYPE_LOCAL_TABLE;
		tmpindex = compile->HashTable[h];
		while(tmpindex != 0 && compile->SymLocalTable.local[tmpindex].isvalid == ZL_TRUE && 
			(ZENGL_SYS_STRCMP(name,compile->TokenStringPoolGetPtr(PVM,compile->SymLocalTable.local[tmpindex].nameIndex)) != 0 || 
			compile->SymLocalTable.local[tmpindex].funid != compile->SymFunTable.global_funid))
		{
			tmpindex = compile->SymLocalTable.local[tmpindex].next;
		}
		if(tmpindex == 0)
		{
			//compile->parser_curnode = nodenum;
			//compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYM_CAN_NOT_FIND_LOCAL_ID_MEMLOC,name,name);
			goto FindGlobalSymLoc; //�ֲ����ű��Ҳ������͵�ȫ�ַ��ű�����в��ң�����debug�ͼȿ��Ե��Ժ�����ľֲ��������ֿ��Ե�����Щû�б�global���������ⲿȫ�ֱ���
		}
		else if(compile->SymLocalTable.local[tmpindex].isvalid == ZL_FALSE)
			compile->exit(VM_ARG,ZL_ERR_CP_SYM_LOCAL_TABLE_INVALID_INDEX);
		else if(compile->SymLocalTable.local[tmpindex].funid != compile->SymFunTable.global_funid)
			compile->exit(VM_ARG,ZL_ERR_CP_SYM_LOCAL_ID_INVALID_FUN_ID);
		switch(compile->SymLocalTable.local[tmpindex].type)
		{
		case ZL_SYM_ENUM_LOCAL_TYPE_ARG:
			ret_data.type = ZL_R_DT_ARGMEM;
			ret_data.val.mem = compile->SymLocalTable.local[tmpindex].localid;
			break;
		case ZL_SYM_ENUM_LOCAL_TYPE_LOCAL:
			ret_data.type = ZL_R_DT_LOCMEM;
			ret_data.val.mem = compile->SymLocalTable.local[tmpindex].localid;
			break;
		case ZL_SYM_ENUM_LOCAL_TYPE_GLOBAL:
			goto FindGlobalSymLoc;
			break;
		default:
			compile->exit(VM_ARG,ZL_ERR_CP_SYM_LOCAL_ID_INVALID_TYPE);
			break;
		}
		return ret_data;
	}
	return ret_data;
}

/*
    ����������nodenum��Ӧ�ڵ��classidֵ����Ҫ�������ɵ�������Ļ��ָ��ʱ
*/
ZL_INT zenglDebug_SymLookupID_ForDot(ZL_VOID * VM_ARG,ZL_INT nodenum)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_VM_TYPE * PVM = VM->debug.DeubugPVM;
	ZENGL_COMPILE_TYPE * compile = &VM->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_AST_NODE_TYPE * node = nodes + nodenum;
	ZL_CHAR * name = compile->TokenStringPoolGetPtr(VM_ARG,nodes[nodenum].strindex);
	ZL_INT h;
	ZL_INT tmpindex;
	
	if(!compile->gencode_struct.is_inFun)
	{
FindGlobalSymLoc:
		h = compile->hash(VM_ARG,name) + ZL_SYM_HASH_SIZE * ZL_HASH_TYPE_SYM_TABLE;
		tmpindex = compile->HashTable[h];
		while(tmpindex != 0 && compile->SymGlobalTable.sym[tmpindex].isvalid == ZL_TRUE &&
			  ZENGL_SYS_STRCMP(name,compile->TokenStringPoolGetPtr(PVM,compile->SymGlobalTable.sym[tmpindex].nameIndex)) != 0)
			tmpindex = compile->SymGlobalTable.sym[tmpindex].next;
		if(tmpindex == 0)
		{
			compile->parser_curnode = nodenum;
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYM_CAN_NOT_FIND_ID_MEMLOC,name,name);
		}
		else if(compile->SymGlobalTable.sym[tmpindex].isvalid == ZL_FALSE)
			compile->exit(VM_ARG,ZL_ERR_CP_SYM_GLOBAL_TABLE_FIND_NOT_VALID_INDEX_WHEN_LOOKUP);
		else
		{
			compile->memDataForDot.type = ZL_R_DT_MEM;
			compile->memDataForDot.val.mem = compile->SymGlobalTable.sym[tmpindex].memloc;
			return compile->SymGlobalTable.sym[tmpindex].classid;
		}
	}
	else
	{
		h = compile->hash(VM_ARG,name) + ZL_SYM_HASH_SIZE * ZL_HASH_TYPE_LOCAL_TABLE;
		tmpindex = compile->HashTable[h];
		while(tmpindex != 0 && compile->SymLocalTable.local[tmpindex].isvalid == ZL_TRUE && 
			(ZENGL_SYS_STRCMP(name,compile->TokenStringPoolGetPtr(PVM,compile->SymLocalTable.local[tmpindex].nameIndex)) != 0 || 
			compile->SymLocalTable.local[tmpindex].funid != compile->SymFunTable.global_funid))
		{
			tmpindex = compile->SymLocalTable.local[tmpindex].next;
		}
		if(tmpindex == 0)
		{
			//compile->parser_curnode = nodenum;
			//compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYM_CAN_NOT_FIND_LOCAL_ID_MEMLOC,name,name);
			goto FindGlobalSymLoc; //�ֲ����ű��Ҳ������͵�ȫ�ַ��ű�����в��ң�����debug�ͼȿ��Ե��Ժ�����ľֲ��������ֿ��Ե�����Щû�б�global���������ⲿȫ�ֱ���
		}
		else if(compile->SymLocalTable.local[tmpindex].isvalid == ZL_FALSE)
			compile->exit(VM_ARG,ZL_ERR_CP_SYM_LOCAL_TABLE_INVALID_INDEX);
		else if(compile->SymLocalTable.local[tmpindex].funid != compile->SymFunTable.global_funid)
			compile->exit(VM_ARG,ZL_ERR_CP_SYM_LOCAL_ID_INVALID_FUN_ID);
		switch(compile->SymLocalTable.local[tmpindex].type)
		{
		case ZL_SYM_ENUM_LOCAL_TYPE_ARG:
			compile->memDataForDot.type = ZL_R_DT_ARGMEM;
			compile->memDataForDot.val.mem = compile->SymLocalTable.local[tmpindex].localid;
			return compile->SymLocalTable.local[tmpindex].classid;
			break;
		case ZL_SYM_ENUM_LOCAL_TYPE_LOCAL:
			compile->memDataForDot.type = ZL_R_DT_LOCMEM;
			compile->memDataForDot.val.mem = compile->SymLocalTable.local[tmpindex].localid;
			return compile->SymLocalTable.local[tmpindex].classid;
			break;
		case ZL_SYM_ENUM_LOCAL_TYPE_GLOBAL:
			goto FindGlobalSymLoc;
			break;
		default:
			compile->exit(VM_ARG,ZL_ERR_CP_SYM_LOCAL_ID_INVALID_TYPE);
			break;
		}
		return 0;
	}
	return 0;
}

/*
	���������ݽڵ�Ų�����ID��Ϣ
	����Ŀǰ���Ա��ʽ�в���ʹ����������䣬Ҳ���ܵ������еķ�������ˣ����溯����ֱ�ӱ������ڵ��Ա��ʽ��ʹ���������﷨����
*/
ZL_INT zenglDebug_SymLookupClass(ZL_VOID * VM_ARG,ZL_INT nodenum)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_COMPILE_TYPE * compile = &VM->compile;
	compile->parser_curnode = nodenum;
	compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_CLASS_NAME_CAN_NOT_USE_IN_DEBUG);
	return -1;
}

/*
	��������SymClassMemberTable�в���parent_classid��Ӧ����ĳ�Աnodenum����Ϣ
*/
ZL_INT zenglDebug_SymLookupClassMember(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT parent_classid)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_VM_TYPE * PVM = VM->debug.DeubugPVM;
	ZENGL_COMPILE_TYPE * compile = &VM->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_AST_NODE_TYPE * node = nodes + nodenum;
	ZL_CHAR * name = compile->TokenStringPoolGetPtr(VM_ARG,nodes[nodenum].strindex);
	ZL_INT h = compile->hash(VM_ARG,name) + ZL_SYM_HASH_SIZE * ZL_HASH_TYPE_CLASSMEMBER_TABLE;
	ZL_INT tmpindex = compile->HashTable[h];

	while(tmpindex != 0 && compile->SymClassMemberTable.members[tmpindex].isvalid == ZL_TRUE &&
		 (compile->SymClassMemberTable.members[tmpindex].classid != parent_classid || 
		  ZENGL_SYS_STRCMP(name,compile->TokenStringPoolGetPtr(PVM,compile->SymClassMemberTable.members[tmpindex].nameIndex)) != 0))
		tmpindex = compile->SymClassMemberTable.members[tmpindex].next;
	if(tmpindex == 0)
	{
		compile->parser_curnode = nodenum;
		compile->parser_errorExit(VM_ARG,ZL_ERR_SYNTAX_INVALID_CLASS_MEMBER);
	}
	else if(compile->SymClassMemberTable.members[tmpindex].isvalid == ZL_FALSE)
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_CLASSMEMBER_TABLE_FIND_NOT_VALID_INDEX);
	else
	{
		return tmpindex;
	}
	return -1;
}

/**
 * �������и��ݽڵ�Ų��Һ���ID��Ϣ
 * ����Ŀǰ���Ա��ʽ��ֻ�ܵ���ģ�麯�������ܵ����û��Զ���Ľű���������ˣ����溯���ڲ��ҵ��Զ���Ľű�����ʱ�ͻᱨ��Ӧ�Ĵ���
 */
ZL_INT zenglDebug_SymLookupFun(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT classid)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_VM_TYPE * PVM = VM->debug.DeubugPVM;
	ZENGL_COMPILE_TYPE * compile = &VM->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_AST_NODE_TYPE * node = nodes + nodenum;
	ZL_CHAR * name = compile->TokenStringPoolGetPtr(VM_ARG,nodes[nodenum].strindex);
	ZL_INT h = compile->hash(VM_ARG,name) + ZL_SYM_HASH_SIZE * ZL_HASH_TYPE_FUN_TABLE;
	ZL_INT tmpindex = compile->HashTable[h];

	while(tmpindex != 0 && compile->SymFunTable.funs[tmpindex].isvalid == ZL_TRUE &&
		 (classid != compile->SymFunTable.funs[tmpindex].classid ||
		  ZENGL_SYS_STRCMP(name,compile->TokenStringPoolGetPtr(PVM,compile->SymFunTable.funs[tmpindex].nameIndex)) != 0)) //ѭ������ͬ���������ຯ��
		tmpindex = compile->SymFunTable.funs[tmpindex].next;
	if(tmpindex == 0)
		return 0; //���û�ҵ���˵�������û��Զ���ĺ�������ô�п�����ģ����ĺ�����
	else if(compile->SymFunTable.funs[tmpindex].isvalid == ZL_FALSE)
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_FUN_TABLE_FIND_NOT_VALID_INDEX);
	compile->parser_curnode = nodenum;
	compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_SCRIPT_FUN_CALL_CAN_NOT_USE_IN_DEBUG);
	return -1;
}

/*
	�������в���ĳģ�麯������Ϣ�����ظ�ģ�麯���ڶ�̬�����е�����
*/
ZL_INT zenglDebug_LookupModFunTable(ZL_VOID * VM_ARG,ZL_CHAR * functionName)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_VM_TYPE * PVM = VM->debug.DeubugPVM;
	ZENGL_RUN_TYPE * run = &VM->run;
	ZL_INT h = run->Hash(VM_ARG,functionName) + ZL_R_HASH_SIZE * ZL_R_HASH_TYPE_MOD_FUN_TABLE;
	ZL_INT tmpindex = run->HashTable[h];

	while(tmpindex != 0 && run->ModFunTable.mod_funs[tmpindex].isvalid == ZL_TRUE && 
		ZENGL_SYS_STRCMP(functionName,run->InstDataStringPoolGetPtr(PVM,run->ModFunTable.mod_funs[tmpindex].strIndex)) != 0)
		tmpindex = run->ModFunTable.mod_funs[tmpindex].next;
	if(tmpindex == 0)
		run->exit(VM_ARG,ZL_ERR_RUN_FUNCTION_IS_INVALID,functionName,ZL_R_REG_PC,functionName);
	else if(run->ModFunTable.mod_funs[tmpindex].isvalid == ZL_FALSE)
		run->exit(VM_ARG,ZL_ERR_RUN_MOD_FUN_TABLE_FIND_INVALID_INDEX);
	else
		return tmpindex;
	return -1;
}

/*������ͨ�����������ڵĽڵ�����ֵ�����Һ�����IDֵ*/
ZL_INT zenglDebug_LookupFunID(ZL_VOID * VM_ARG,ZL_INT nodenum)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_AST_NODE_TYPE * node = nodes + nodenum;
	ZL_CHAR * name = compile->TokenStringPoolGetPtr(VM_ARG,nodes[nodenum].strindex);
	ZL_INT h = compile->hash(VM_ARG,name) + ZL_SYM_HASH_SIZE * ZL_HASH_TYPE_FUN_TABLE;
	ZL_INT tmpindex = compile->HashTable[h];

	while(tmpindex != 0 && compile->SymFunTable.funs[tmpindex].isvalid == ZL_TRUE &&
		 (node->line_no != compile->LineCols.lines[compile->SymFunTable.funs[tmpindex].linecols].lineno ||
		  node->col_no != compile->LineCols.lines[compile->SymFunTable.funs[tmpindex].linecols].colno ||
		  ZENGL_SYS_STRCMP(name,compile->TokenStringPoolGetPtr(VM_ARG,compile->SymFunTable.funs[tmpindex].nameIndex)) != 0)) //ѭ������ͬ���������ຯ��
		tmpindex = compile->SymFunTable.funs[tmpindex].next;
	if(tmpindex == 0)
		return 0; //���û�ҵ���˵�������û��Զ���ĺ�������ô�п�����ģ����ĺ�����
	else if(compile->SymFunTable.funs[tmpindex].isvalid == ZL_FALSE)
		return 0;
	return compile->SymFunTable.funs[tmpindex].funid; //����ID��1��ʼ���ҵ���funs�����Ӧ������ֵ
}

/*���õ��������ڵĽű���������*/
ZL_INT zenglDebug_SetFunInfo(ZL_VOID * VM_ARG)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_VM_TYPE * PVM = VM->debug.DeubugPVM;
	ZENGL_RUN_TYPE * run = &VM->run;
	ZENGL_RUN_TYPE * PRun = &PVM->run;
	ZENGL_DEBUG_TYPE * PDebug = &PVM->debug;
	ZL_INT arg = PRun->reg_list[ZL_R_RT_ARG].val.dword;
	ZL_INT loc = PRun->reg_list[ZL_R_RT_LOC].val.dword;
	ZL_INT tmp_arg = 0;
	ZL_INT PJmp = 0;
	ZL_INT PFun = 0;
	ZL_INT PFunID = 0;
	ZL_INT * chnum = ZL_NULL;

	for(;arg > 0 && loc > 0;)
	{
		PJmp = PRun->vstack_list.mem_array[loc - 1].val.dword - 1;
		if(PDebug->api_call_pc > 0 && PDebug->api_call_arg == arg) //�����zenglApi_Call���õ������ʱ
		{
			PFun = PDebug->api_call_pc;
			chnum = PVM->compile.AST_nodes.nodes[PRun->inst_list.insts[PFun].nodenum].childs.childnum;
			PFunID = PVM->debug.LookupFunID(PVM,chnum[0]);
			if(PFunID == 0)
				break;
			VM->compile.gencode_struct.is_inFun = ZL_TRUE;
			VM->compile.AsmGCIsInClass = ZL_TRUE;
			VM->compile.SymClassTable.global_classid = PVM->compile.SymFunTable.funs[PFunID].classid;
			VM->compile.SymFunTable.global_funid = PVM->compile.SymFunTable.funs[PFunID].funid;
			run->reg_list[ZL_R_RT_ARG].val.dword = arg;
			// ���������ZL_R_RDT_INT����Ĭ����ZL_R_RDT_NONE���ͣ�ZL_R_RDT_NONE���͵ļĴ�����PUSH��ʱ��ᱻǿ�Ƶ���0���������õ�dwordֵ�ͻ�ʧЧ��!
			run->reg_list[ZL_R_RT_ARG].runType = ZL_R_RDT_INT;
			run->reg_list[ZL_R_RT_LOC].val.dword = loc;
			// ������ͬ��
			run->reg_list[ZL_R_RT_LOC].runType = ZL_R_RDT_INT;
			return 0;
		}
		else if(PRun->inst_list.insts[PJmp].type == ZL_R_IT_CALL)
		{
			tmp_arg = arg;
			arg = PRun->vstack_list.mem_array[tmp_arg - 3].val.dword;
			loc = PRun->vstack_list.mem_array[tmp_arg - 2].val.dword;
			continue;
		}
		else if(PRun->inst_list.insts[PJmp].type == ZL_R_IT_JMP)
		{
			PFun = PRun->inst_list.insts[PJmp].src.val.num;
			chnum = PVM->compile.AST_nodes.nodes[PRun->inst_list.insts[PFun].nodenum].childs.childnum;
			PFunID = PVM->debug.LookupFunID(PVM,chnum[0]);
			if(PFunID == 0)
				break;
			VM->compile.gencode_struct.is_inFun = ZL_TRUE;
			VM->compile.AsmGCIsInClass = ZL_TRUE;
			VM->compile.SymClassTable.global_classid = PVM->compile.SymFunTable.funs[PFunID].classid;
			VM->compile.SymFunTable.global_funid = PVM->compile.SymFunTable.funs[PFunID].funid;
			run->reg_list[ZL_R_RT_ARG].val.dword = arg;
			// ���������ZL_R_RDT_INT����Ĭ����ZL_R_RDT_NONE���ͣ�ZL_R_RDT_NONE���͵ļĴ�����PUSH��ʱ��ᱻǿ�Ƶ���0���������õ�dwordֵ�ͻ�ʧЧ��!
			run->reg_list[ZL_R_RT_ARG].runType = ZL_R_RDT_INT;
			run->reg_list[ZL_R_RT_LOC].val.dword = loc;
			// ������ͬ��
			run->reg_list[ZL_R_RT_LOC].runType = ZL_R_RDT_INT;
			return 0;
		}
		else
			break;
	}
	VM->compile.gencode_struct.is_inFun = ZL_FALSE;
	VM->compile.AsmGCIsInClass = ZL_FALSE;
	VM->compile.SymClassTable.global_classid = 0;
	VM->compile.SymFunTable.global_funid = 0;
	return 0;
}

/*��������ջ�л�ȡ�ű������ĵ������*/
ZL_INT zenglDebug_GetTraceInfo(ZL_VOID * VM_ARG,ZL_CHAR * ApiName,ZL_INT * argArg,ZL_INT * argLOC,ZL_INT * argPC,
							   ZL_CHAR ** fileName,ZL_INT * line,ZL_CHAR ** className,ZL_CHAR ** funcName)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_COMPILE_TYPE * compile = &VM->compile;
	ZENGL_RUN_TYPE * run = &VM->run;
	ZENGL_DEBUG_TYPE * debug = &VM->debug;
	ZL_INT arg,loc,pc,nodenum;
	ZL_INT tmp_arg = 0;
	ZL_INT Jmp = 0;
	ZL_INT Fun = 0;
	ZL_INT FunID = 0;
	ZL_INT ClassID = 0;
	ZL_INT * chnum = ZL_NULL;
	
	if(argLOC == ZL_NULL || (*argLOC) == -1)
		loc = run->reg_list[ZL_R_RT_LOC].val.dword;
	else
		loc = (*argLOC);
	if(argArg == ZL_NULL || (*argArg) == -1)
		arg = run->reg_list[ZL_R_RT_ARG].val.dword;
	else
		arg = (*argArg);
	if(argPC == ZL_NULL || (*argPC) == -1)
		pc = run->reg_list[ZL_R_RT_PC].val.dword;
	else
		pc = (*argPC);

	if(pc >= 0 && pc < run->inst_list.count)
	{
		nodenum = run->inst_list.insts[pc].nodenum;
		if(fileName != ZL_NULL)
			(*fileName) = compile->AST_nodes.nodes[nodenum].filename;
		if(line != ZL_NULL)
			(*line) = compile->AST_nodes.nodes[nodenum].line_no;
	}
	else
	{
		run->SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_DEBUG_TRACE_INVALID_ARGS,ApiName,ApiName);
		return -1;
	}

	for(;arg > 0 && loc > 0;)
	{
		Jmp = run->vstack_list.mem_array[loc - 1].val.dword - 1;
		if(debug->api_call_pc > 0 && debug->api_call_arg == arg) //�����zenglApi_Call���õ������ʱ
		{
			Fun = debug->api_call_pc;
			chnum = compile->AST_nodes.nodes[run->inst_list.insts[Fun].nodenum].childs.childnum;
			FunID = VM->debug.LookupFunID(VM_ARG,chnum[0]);
			if(FunID == 0)
			{
				run->SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_DEBUG_TRACE_INVALID_ARGS,ApiName,ApiName);
				return -1;
			}
			ClassID = compile->SymFunTable.funs[FunID].classid;
			if(funcName != ZL_NULL)
				(* funcName) = compile->TokenStringPoolGetPtr(VM_ARG,compile->SymFunTable.funs[FunID].nameIndex);
			if(className != ZL_NULL)
				(* className) = compile->TokenStringPoolGetPtr(VM_ARG,compile->SymClassTable.classTable[ClassID].nameIndex);
			return 0;
		}
		else if(run->inst_list.insts[Jmp].type == ZL_R_IT_CALL)
		{
			tmp_arg = arg;
			arg = run->vstack_list.mem_array[tmp_arg - 3].val.dword;
			loc = run->vstack_list.mem_array[tmp_arg - 2].val.dword;
			continue;
		}
		else if(run->inst_list.insts[Jmp].type == ZL_R_IT_JMP)
		{
			Fun = run->inst_list.insts[Jmp].src.val.num;
			chnum = compile->AST_nodes.nodes[run->inst_list.insts[Fun].nodenum].childs.childnum;
			FunID = VM->debug.LookupFunID(VM_ARG,chnum[0]);
			if(FunID == 0)
			{
				run->SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_DEBUG_TRACE_INVALID_ARGS,ApiName,ApiName);
				return -1;
			}
			ClassID = compile->SymFunTable.funs[FunID].classid;
			if(funcName != ZL_NULL)
				(* funcName) = compile->TokenStringPoolGetPtr(VM_ARG,compile->SymFunTable.funs[FunID].nameIndex);
			if(className != ZL_NULL)
			{
				if(ClassID > 0)
					(* className) = compile->TokenStringPoolGetPtr(VM_ARG,compile->SymClassTable.classTable[ClassID].nameIndex);
				else
					(* className) = ZL_NULL;
			}
			if(argPC != ZL_NULL)
				(*argPC) = Jmp;
			if(argArg != ZL_NULL)
				(*argArg) = run->vstack_list.mem_array[arg - 3].val.dword;
			if(argLOC != ZL_NULL)
				(*argLOC) = run->vstack_list.mem_array[arg - 2].val.dword;
			return 1;
		}
		else
		{
			run->SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_DEBUG_TRACE_INVALID_ARGS,ApiName,ApiName);
			return -1;
		}
	}
	if(funcName != ZL_NULL)
		(* funcName) = ZL_NULL;
	if(className != ZL_NULL)
		(* className) = ZL_NULL;
	return 0;
}

/*��ʼ���������Ĵ�Ŷϵ�Ķ�̬����*/
ZL_INT zenglDebug_InitBreak(ZL_VOID * VM_ARG)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run = &VM->run;
	ZENGL_DEBUG_TYPE * debug = &VM->debug;
	if(debug->BreakPoint.isInit)
		return 0;
	debug->BreakPoint.size = ZL_DBG_BREAK_POINT_SIZE;
	debug->BreakPoint.count = 0;
	debug->BreakPoint.members = run->memAlloc(VM_ARG,debug->BreakPoint.size * sizeof(ZENGL_DEBUG_BREAK_POINT_MEMBER),&debug->BreakPoint.mempool_index);
	if(debug->BreakPoint.members == ZL_NULL)
	{
		run->SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INIT_DEBUG_BREAK_FAILED);
		return -1;
	}
	ZENGL_SYS_MEM_SET(debug->BreakPoint.members,0,debug->BreakPoint.size * sizeof(ZENGL_DEBUG_BREAK_POINT_MEMBER));
	debug->BreakPoint.isInit = ZL_TRUE;
	return 0;
}

/*�����������öϵ�*/
ZL_INT zenglDebug_SetBreak(ZL_VOID * VM_ARG,ZL_INT pc,ZL_CHAR * filename,ZL_INT line,ZL_CHAR * condition,ZL_CHAR * log,ZL_INT count,ZL_BOOL disabled)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run = &VM->run;
	ZENGL_DEBUG_TYPE * debug = &VM->debug;
	ZL_INT index,tmpindex = -1;
	ZL_CHAR * tmpstr = ZL_NULL;

	if(!debug->BreakPoint.isInit)
	{
		if(debug->InitBreak(VM_ARG) == -1)
		{
			return -1;
		}
	}
	if(debug->BreakPoint.count == debug->BreakPoint.size)
	{
		debug->BreakPoint.size += ZL_DBG_BREAK_POINT_SIZE;
		debug->BreakPoint.members = run->memReAlloc(VM_ARG,debug->BreakPoint.members,
			debug->BreakPoint.size * sizeof(ZENGL_DEBUG_BREAK_POINT_MEMBER),&debug->BreakPoint.mempool_index);
		ZENGL_SYS_MEM_SET(debug->BreakPoint.members + (debug->BreakPoint.size - ZL_DBG_BREAK_POINT_SIZE),0,
			ZL_DBG_BREAK_POINT_SIZE * sizeof(ZENGL_DEBUG_BREAK_POINT_MEMBER));
	}
	for(index=0;index < debug->BreakPoint.size;index++)
	{
		if(debug->BreakPoint.members[index].isvalid == ZL_FALSE)
			tmpindex = (tmpindex == -1 ? index : tmpindex);
		else if(debug->BreakPoint.members[index].filename == filename &&
			debug->BreakPoint.members[index].line == line)
		{
			tmpindex = index;
			break;
		}
	}
	index = tmpindex;
	if(index != -1)
	{
		debug->BreakPoint.members[index].filename = filename;
		debug->BreakPoint.members[index].line = line;
		tmpstr = debug->BreakPoint.members[index].condition;
		if(condition != ZL_NULL)
		{
			debug->BreakPoint.members[index].condition = zenglApi_AllocMemForString(VM_ARG,condition);
			if(debug->BreakPoint.members[index].condition == ZL_NULL)
				return -1;
		}
		else
			debug->BreakPoint.members[index].condition = ZL_NULL;
		if(tmpstr != ZL_NULL)
			run->memFreeOnce(VM_ARG,tmpstr);
		tmpstr = debug->BreakPoint.members[index].log;
		if(log != ZL_NULL)
		{
			debug->BreakPoint.members[index].log = zenglApi_AllocMemForString(VM_ARG,log);
			if(debug->BreakPoint.members[index].log == ZL_NULL)
				return -1;
		}
		else
			debug->BreakPoint.members[index].log = ZL_NULL;
		if(tmpstr != ZL_NULL)
			run->memFreeOnce(VM_ARG,tmpstr);
		debug->BreakPoint.members[index].pc = pc;
		if(run->inst_list.insts[pc].type != ZL_R_IT_BREAK)
		{
			debug->BreakPoint.members[index].orig_inst_type = run->inst_list.insts[pc].type;
			debug->BreakPoint.members[index].orig_inst_src_num = run->inst_list.insts[pc].src.val.num;
		}
		debug->BreakPoint.members[index].count = count;
		debug->BreakPoint.members[index].disabled = disabled;
		if(debug->BreakPoint.members[index].isvalid == ZL_FALSE)
		{
			debug->BreakPoint.members[index].isvalid = ZL_TRUE;
			debug->BreakPoint.count++;
		}
		else
			debug->BreakPoint.members[index].isvalid = ZL_TRUE;
		run->inst_list.insts[pc].type = ZL_R_IT_BREAK;
		run->inst_list.insts[pc].src.val.num = index;
		return index;
	}
	run->SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_SET_DEBUG_BREAK_FAILED);
	return -1;
}

/*����������һ��ָ����Ϊ��ִ��һ�εĶϵ�*/
ZL_INT zenglDebug_BreakStart(ZL_VOID * VM_ARG)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_COMPILE_TYPE * compile = &VM->compile;
	ZENGL_RUN_TYPE * run = &VM->run;
	ZENGL_DEBUG_TYPE * debug = &VM->debug;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	if(debug->SetBreak(VM_ARG,ZL_R_REG_PC,nodes[run->inst_list.insts[ZL_R_REG_PC].nodenum].filename,
				nodes[run->inst_list.insts[ZL_R_REG_PC].nodenum].line_no,ZL_NULL,ZL_NULL,1,ZL_FALSE) == -1)
		return -1;
	return 0;
}

/*ɾ��index������Ӧ�ĵ��Զϵ�*/
ZL_INT zenglDebug_DelBreak(ZL_VOID * VM_ARG,ZL_INT index)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run = &VM->run;
	ZENGL_DEBUG_TYPE * debug = &VM->debug;
	if(debug->BreakPoint.members[index].isvalid == ZL_TRUE)
	{
		ZENGL_RUN_INST((debug->BreakPoint.members[index].pc)).type = debug->BreakPoint.members[index].orig_inst_type;
		ZENGL_RUN_INST((debug->BreakPoint.members[index].pc)).src.val.num = debug->BreakPoint.members[index].orig_inst_src_num;
		if(debug->BreakPoint.members[index].condition != ZL_NULL)
			run->memFreeOnce(VM_ARG,debug->BreakPoint.members[index].condition);
		if(debug->BreakPoint.members[index].log != ZL_NULL)
			run->memFreeOnce(VM_ARG,debug->BreakPoint.members[index].log);
		ZENGL_SYS_MEM_SET(&debug->BreakPoint.members[index],0,sizeof(ZENGL_DEBUG_BREAK_POINT_MEMBER));
		debug->BreakPoint.count--;
		return debug->BreakPoint.count;
	}
	return -1;
}

/*�ϵ�ԭָ��ִ�����Ҫ�ָ���ָ���Ӧ�Ķϵ�*/
ZL_INT zenglDebug_RestoreBreaks(ZL_VOID * VM_ARG)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run = &VM->run;
	ZENGL_DEBUG_TYPE * debug = &VM->debug;
	ZL_INT index;
	for(index=0;index<debug->BreakPoint.size;index++)
	{
		if(debug->BreakPoint.members[index].needRestore == ZL_TRUE)
		{
			if(debug->BreakPoint.members[index].count > 0)
			{
				if(--debug->BreakPoint.members[index].count <= 0) //�ﵽ�ϵ�ִ�д�������ɾ���ϵ�
				{
					debug->BreakPoint.members[index].count = 0;
					debug->DelBreak(VM_ARG,index);
					continue;
				}
			}
			ZENGL_RUN_INST((debug->BreakPoint.members[index].pc)).type = ZL_R_IT_BREAK;
			ZENGL_RUN_INST((debug->BreakPoint.members[index].pc)).src.val.num = index;
		}
	}
	debug->flag = debug->flag & (~ZL_DBG_FLAG_RESTORE_BREAK);
	return 0;
}

/*���ϵ��Ƿ񵽴�ָ���жϵ�����*/
ZL_INT zenglDebug_CheckCondition(ZL_VOID * VM_ARG,ZL_CHAR * condition)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_EXPORT_MOD_FUN_ARG reg_debug;
	if(zenglApi_Debug(VM_ARG,condition) == -1)
		return -1;
	zenglApi_GetDebug(VM_ARG,&reg_debug);
	switch(reg_debug.type)
	{
	case ZL_EXP_FAT_NONE:
		return 0;
		break;
	case ZL_EXP_FAT_INT:
		if(reg_debug.val.integer == 0)
			return 0;
		else
			return 1;
		break;
	case ZL_EXP_FAT_FLOAT:
		if(reg_debug.val.floatnum == 0)
			return 0;
		else
			return 1;
		break;
	case ZL_EXP_FAT_STR:
		if(ZENGL_SYS_STRLEN(reg_debug.val.str) == 0)
			return 0;
		else
			return 1;
		break;
	default:
		return 0;
		break;
	}
	return 0;
}

/*�ָ������ж϶�Ӧ��ԭʼָ��*/
ZL_INT zenglDebug_RestoreSingleBreak(ZL_VOID * VM_ARG)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run = &VM->run;
	ZENGL_DEBUG_TYPE * debug = &VM->debug;
	if(!debug->singleBreak.isvalid)
		return 0;
	if(debug->singleBreak.orig_pc >= 0 && 
	   debug->singleBreak.orig_inst_type != ZL_R_IT_BREAK && 
	   debug->singleBreak.orig_inst_type != ZL_R_IT_SINGLE_BREAK)
	{
		run->inst_list.insts[debug->singleBreak.orig_pc].type = debug->singleBreak.orig_inst_type;
		debug->singleBreak.orig_pc = 0;
		debug->singleBreak.isvalid = ZL_FALSE;
		debug->singleBreak.orig_inst_type = ZL_R_IT_NONE;
		return 0;
	}
	return -1;
}

/*������Ҫ�ж��Ƿ���Ҫ����һ��ָ�����õ����ж�*/
ZL_INT zenglDebug_ChkAndSetSingleBreak(ZL_VOID * VM_ARG)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_COMPILE_TYPE * compile = &VM->compile;
	ZENGL_RUN_TYPE * run = &VM->run;
	ZENGL_DEBUG_TYPE * debug = &VM->debug;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZL_INT next_pc = ZL_R_REG_PC + 1;
	ZL_INT compare_nodenum,next_nodenum;
	ZL_INT tmp;
	ZL_BOOL isSameLine; 
	ZL_BOOL isStepIn;

	if((debug->flag & ZL_DBG_FLAG_SET_SINGLE_STEP_IN) == ZL_DBG_FLAG_SET_SINGLE_STEP_IN)
		isStepIn = ZL_TRUE;
	else if((debug->flag & ZL_DBG_FLAG_SET_SINGLE_STEP_OUT) == ZL_DBG_FLAG_SET_SINGLE_STEP_OUT)
		isStepIn = ZL_FALSE;
	else
		return 0;

	if((next_pc < 0) || (next_pc >= run->inst_list.count))
		return 0;
	switch(run->inst_list.insts[next_pc].type)
	{
	case ZL_R_IT_END:
	case ZL_R_IT_PUSH:
	case ZL_R_IT_PUSH_LOC:
	case ZL_R_IT_RESET:
		return 0;
		break;
	}

	compare_nodenum = run->inst_list.insts[debug->singleBreak.compare_pc].nodenum;
	next_nodenum = run->inst_list.insts[next_pc].nodenum;
	if(nodes[compare_nodenum].filename == nodes[next_nodenum].filename)
	{
		if(nodes[compare_nodenum].line_no == nodes[next_nodenum].line_no)
			isSameLine = ZL_TRUE;
		else
			isSameLine = ZL_FALSE;
	}
	else
		isSameLine = ZL_FALSE;

	if(isStepIn)
	{
		if(!isSameLine)
			goto setSBreak;
		else
			return 0;
	}
	else
	{
		if(!isSameLine)
			goto setSBreak;
		else
		{
			if(run->inst_list.insts[next_pc].type == ZL_R_IT_JMP)
			{
				tmp = run->inst_list.insts[next_pc].src.val.num;
				if(run->inst_list.insts[tmp].type == ZL_R_IT_FUNARG)
				{
					next_pc++;
					goto setSBreak;
				}
			}
			return 0;
		}
	}
setSBreak:
	debug->singleBreak.orig_pc = next_pc;
	debug->singleBreak.orig_inst_type = run->inst_list.insts[next_pc].type;
	run->inst_list.insts[next_pc].type = ZL_R_IT_SINGLE_BREAK;
	debug->singleBreak.isvalid = ZL_TRUE;
	debug->flag &= ~ZL_DBG_FLAG_SET_SINGLE_STEP_IN;
	debug->flag &= ~ZL_DBG_FLAG_SET_SINGLE_STEP_OUT;
	return 1;
}
