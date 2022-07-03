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
	���ļ�Ϊ���ű�Ĵ����ļ���
*/

#include "zengl_global.h"

/**
 * ��ʼ��self�ڵ�����Ϣ��̬����
 */
static ZL_VOID zengl_static_SymInitSelfClassTable(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	if(compile->SymSelfClassTable.isInit)
		return;
	compile->SymSelfClassTable.count = 0;
	compile->SymSelfClassTable.size = ZL_SYM_SELF_CLASS_TABLE_SIZE;
	compile->SymSelfClassTable.members = compile->memAlloc(VM_ARG,compile->SymSelfClassTable.size * sizeof(ZENGL_SYM_SELF_CLASS_TABLE_MEMBER));
	if(compile->SymSelfClassTable.members == ZL_NULL)
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_SELF_CLASS_TABLE_MALLOC_FAILED);
	ZENGL_SYS_MEM_SET(compile->SymSelfClassTable.members,0,compile->SymSelfClassTable.size * sizeof(ZENGL_SYM_SELF_CLASS_TABLE_MEMBER));
	compile->SymSelfClassTable.isInit = ZL_TRUE;
}

/**
 * ����class��AST�ڵ�ţ��õ�������������������������ҳ����������ID��������ID���õ�self�ڵ�����Ϣ��̬������
 * �Ӷ���������ṹ��ʹ��self����ʾ������������ˣ���Ϊ��̬�����д洢��self�ڵ��Ӧ����ID��Ϣ����Щ��ID��ֱ��ʹ����������������������ID����ͬ��
 */
static ZL_VOID zengl_static_SymSelfClassTableSetClassID(ZL_VOID * VM_ARG, ZL_INT class_nodenum)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZL_INT classname_nodenum = nodes[class_nodenum].childs.childnum[0];
	ZL_INT classid = compile->SymLookupClass(VM_ARG,classname_nodenum);
	ZL_INT i = 0;
	if(compile->SymSelfClassTable.count > 0)
	{
		for(; i < compile->SymSelfClassTable.count; i++)
		{
			if(compile->SymSelfClassTable.members[i].isvalid == ZL_TRUE &&
				compile->SymSelfClassTable.members[i].class_nodenum == class_nodenum)
			{
				compile->SymSelfClassTable.members[i].classid = classid;
			}
		}
	}
}

/**
 * ����self�ڵ��AST�ڵ�ţ���self�ڵ�����Ϣ��̬�����У����ҳ���self�ڵ�����Ӧ����ID
 */
static ZL_INT zengl_static_SymSelfClassTableFindClassID(ZL_VOID * VM_ARG, ZL_INT self_nodenum)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZL_INT classid = 0;
	ZL_INT i = 0;
	if(compile->SymSelfClassTable.count > 0)
	{
		for(; i < compile->SymSelfClassTable.count; i++)
		{
			if(compile->SymSelfClassTable.members[i].isvalid == ZL_TRUE &&
				compile->SymSelfClassTable.members[i].self_nodenum == self_nodenum)
			{
				classid = compile->SymSelfClassTable.members[i].classid;
			}
		}
	}
	return classid;
}

/**
	hash��ϣɢ���㷨��ͨ���ڵ�token�ı�ʾ��������������ñ�ʾ���ڷ���hash���������ֵ��
*/
ZL_INT zengl_Sym_Hash (ZL_VOID * VM_ARG , ZL_CHAR * key )
{
	ZL_INT temp = 0;
	ZL_INT i = 0;
	while (key[i] != ZL_STRNULL)
	{ 
		temp = ((temp << ZL_SYM_HASH_SHIFT) + key[i]) % ZL_SYM_HASH_SIZE;
		++i;
	}
	return temp;
}

/*
	��line,col���кţ��Լ����к����ڵ��ļ������뵽LineCols���кŶ�̬�����С�
*/
ZL_INT zengl_insert_LineCol(ZL_VOID * VM_ARG , ZL_INT line , ZL_INT col,ZL_CHAR * filename)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZL_INT tmpindex;
	if(!compile->LineCols.isInit)
		compile->initLineColTable(VM_ARG);
	if(compile->LineCols.count == compile->LineCols.size)
	{
		compile->LineCols.size += ZL_LINECOL_SIZE;
		compile->LineCols.lines = compile->memReAlloc(VM_ARG,compile->LineCols.lines,compile->LineCols.size * sizeof(ZENGL_LINECOL));
		ZENGL_SYS_MEM_SET(compile->LineCols.lines + (compile->LineCols.size - ZL_LINECOL_SIZE),0,
						ZL_LINECOL_SIZE * sizeof(ZENGL_LINECOL));
	}
	tmpindex = compile->LineCols.count;
	compile->LineCols.lines[tmpindex].lineno = line;
	compile->LineCols.lines[tmpindex].colno = col;
	compile->LineCols.lines[tmpindex].filename = filename;
	compile->LineCols.lines[tmpindex].next = 0;
	compile->LineCols.count++;
	return tmpindex;
}

/*
	��ʼ��LineCols���кŶ�̬���飬���ڼ�¼���ű���ÿ��ȫ�ַ��ų��ֹ������к���Ϣ��
*/
ZL_VOID zengl_initLineColTable(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	if(compile->LineCols.isInit)
		return;
	compile->LineCols.count = 1;
	compile->LineCols.size = ZL_LINECOL_SIZE;
	compile->LineCols.lines = compile->memAlloc(VM_ARG,compile->LineCols.size * sizeof(ZENGL_LINECOL));
	if(compile->LineCols.lines == ZL_NULL)
		compile->exit(VM_ARG,ZL_ERR_CP_LINECOL_MALLOC_FAILED);
	ZENGL_SYS_MEM_SET(compile->LineCols.lines , 0 , compile->LineCols.size * sizeof(ZENGL_LINECOL));
	compile->LineCols.isInit = ZL_TRUE;
}

/**
	�齨���ű������ʽ��
	ѭ��ɨ���﷨����ÿ���ڵ㣬�����ID��ʾ���ͽ��������ű�������FUN�������ͼ��뵽FunTable�е�
*/
ZL_VOID zengl_buildSymTable(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZL_INT tmpNodeNum = compile->AST_nodes.rootnode;
	ZL_INT count = 0;
	ZL_INT i = 0;

	compile->AST_TreeScanStackList.count = 0;
	if(compile->AST_nodes.isInit == ZL_FALSE || compile->AST_nodes.count <= 0) //����﷨��û��ʼ����Ϊ�գ����ʾ����û���κ�token (������һ���յ��ַ����ű���յĽű��ļ�) ��ʲô��������ֱ�ӷ���
		return;
	while(tmpNodeNum !=0 || count == 0)
	{
		switch(nodes[tmpNodeNum].reserve)
		{
		case ZL_RSV_USE:
			break;
		case ZL_RSV_CLASS:
			compile->SymInsertHashTableForClass(VM_ARG,nodes[tmpNodeNum].childs.childnum[0]);
			zengl_static_SymSelfClassTableSetClassID(VM_ARG, tmpNodeNum);
			break;
		case ZL_RSV_FUN:
			compile->SymInsertHashTableForFun(VM_ARG,nodes[tmpNodeNum].childs.childnum[0],0);
			break;
		default:
			compile->ScanInsertGlobalSym(VM_ARG,tmpNodeNum);
			break;
		}
		tmpNodeNum = nodes[tmpNodeNum].nextnode;
		count++;
	}
	for(i=1;i<compile->SymClassTable.count;i++)
	{
		if(compile->SymClassTable.classTable[i].isvalid)
		{
			compile->ScanInsertClassMemberSym(VM_ARG,compile->SymClassTable.classTable[i].nodenum,
								compile->SymClassTable.classTable[i].classid);
		}
	}
}

/*
	ɨ�貢����ȫ�ֱ������ű�
*/
ZL_VOID zengl_ScanInsertGlobalSym(ZL_VOID * VM_ARG,ZL_INT arg_nodenum)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_SCAN_STACK_TYPE tmpstack;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZL_INT nodenum = arg_nodenum;
	ZL_INT parentnum = -1;
	compile->push_AST_TreeScanStack(VM_ARG,nodenum); //��nodenum�ڵ�ѹ��ջ����ʾ���ýڵ��µ������ӽڵ����ɨ�衣
	do{
		tmpstack = compile->pop_AST_TreeScanStack(VM_ARG,ZL_FALSE); //����ǰ��ѹ��ջ�Ľڵ���Ϣ������ZL_FALSE��ʾֻ������Ϣ���ݲ����ڵ�Ӷ�ջ��ɾ����
		if(tmpstack.nodenum < 0) //��-1֮��Ŀսڵ�������
		{
			compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE);
			continue;
		}
		if(tmpstack.childcnt > 0 &&
		   tmpstack.curchild < tmpstack.childcnt) //������صĽڵ�������ӽڵ㣬��curchildС��childcnt�ӽڵ���ʱ���ͼ���ɨ���ӽڵ㡣
		{
			if(tmpstack.curchild < ZL_AST_CHILD_NODE_SIZE) //���С��ZL_AST_CHILD_NODE_SIZE,�Ͷ�childnum��Ļ����ӽڵ����ɨ�裬����ͽ�extchilds��չ�ӽڵ���Ľڵ����ɨ�衣
				nodenum = nodes[tmpstack.nodenum].childs.childnum[tmpstack.curchild];
			else
				nodenum = nodes[tmpstack.nodenum].childs.extchilds[tmpstack.curchild -
																   ZL_AST_CHILD_NODE_SIZE];
			compile->AST_TreeScanStackList.stacks[compile->AST_TreeScanStackList.count - 1].curchild++; //����ǰɨ��Ľڵ�������һ�������´ξͿ���ɨ����һ���ӽڵ㡣
			compile->push_AST_TreeScanStack(VM_ARG,nodenum); //����ǰɨ����ӽڵ�ѹ��ջ�������ͺ�֮ǰѹ��ջ�Ľڵ��ڶ�ջ�й�����һ���ڵ�·����
			continue; //continue�������do...while��ͷ��Ȼ��pop_AST_TreeScanStack�ͻὫ��ѹ��ջ���ӽڵ㵯������
		}
		else if(nodes[tmpstack.nodenum].toktype == ZL_TK_ID ||
				nodes[tmpstack.nodenum].toktype == ZL_TK_ARRAY_ITEM)
		{
			parentnum = nodes[tmpstack.nodenum].parentnode;
			/*�����parentnumһ��Ҫ���д��ڵ���0�Ĳ��ԣ�
			 * ���С��0������android�»����terminated by signal (11)�Ĵ���
			 * �˴�����ndk-stack���߷���*/
			if(parentnum >=0 && nodes[parentnum].toktype == ZL_TK_DOT) //���ID�ĸ��ڵ��ǵ������������SymInsertDotID����
				compile->SymInsertDotID(VM_ARG,tmpstack.nodenum,parentnum);
			else
				compile->SymInsertHashTableForGlobal(VM_ARG,tmpstack.nodenum);
		}

		if(tmpstack.next != 0 && compile->AST_TreeScanStackList.count > 1) //�����ǰ�ڵ���next�ֵܽڵ��Ҳ��Ǹ��ڵ������£��ͽ���ǰ�ڵ㵯��������next�ڵ�ѹ��ջ
		{
			compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE);
			compile->push_AST_TreeScanStack(VM_ARG,tmpstack.next);
			continue; //continue�������do...while��ͷ��Ȼ��pop_AST_TreeScanStack�ͻὫ��ѹ��ջ���ӽڵ㵯������
		}
		else
			compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE); //���ӽڵ��next�ڵ㶼�������ˣ��ͽ��ڵ�Ӷ�ջ�е�����
	}while(compile->AST_TreeScanStackList.count > 0); //�����ջ�л���Ԫ�أ�˵�����нڵ�û�����ֻ꣬�е���ջ���Ԫ�ظ���Ϊ0ʱ���ʾ����AST��Ľڵ㶼�������ˣ��Ϳ�������ѭ�������ˡ�
}

/*
	�жϵ��������Ľڵ��ǳ�Ա�����ຯ�����û���ȫ�ֱ���������ȫ�ֱ�������ȫ�ַ��ű��С�
*/
ZL_VOID zengl_SymInsertDotID(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT parentNum)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZL_INT first_num,second_num;
	first_num = nodes[parentNum].childs.childnum[0];
	second_num = nodes[parentNum].childs.childnum[1];
	if(first_num != nodenum ||
	   nodes[second_num].toktype == ZL_TK_FUNCALL)
	{
		return;
	}
	compile->SymInsertHashTableForGlobal(VM_ARG,nodenum);
}

/*
	��nodenum��Ӧ�Ľڵ�ķ�����Ϣ��ͨ��SymInsertGlobalTable���뵽SymGlobalTable���ű��Ӧ�Ķ�̬�����У��ú������صĶ�̬���������ֵ�ٱ��浽HashTable�С�
	�´β���ʱ�Ϳ����ɽڵ�����hash�����㷨�����ҵ�HashTable�������ֵ����������ֵ�Ϳ����ҵ�SymGlobalTable�е�ĳ��Ԫ���ˡ�
*/
ZL_INT zengl_SymInsertHashTableForGlobal(ZL_VOID * VM_ARG,ZL_INT nodenum)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_AST_NODE_TYPE * node = nodes + nodenum;
	ZL_CHAR * name = compile->TokenStringPoolGetPtr(VM_ARG,nodes[nodenum].strindex);
	ZL_INT h = compile->hash(VM_ARG,name) + ZL_SYM_HASH_SIZE * ZL_HASH_TYPE_SYM_TABLE;
	ZL_INT tmpindex = compile->HashTable[h];

	while(tmpindex != 0 && compile->SymGlobalTable.sym[tmpindex].isvalid == ZL_TRUE &&
		  ZENGL_SYS_STRCMP(name,compile->TokenStringPoolGetPtr(VM_ARG,compile->SymGlobalTable.sym[tmpindex].nameIndex)) != 0)
		tmpindex = compile->SymGlobalTable.sym[tmpindex].next;
	if(tmpindex == 0)
	{
		tmpindex = compile->HashTable[h];
		compile->HashTable[h] = compile->SymInsertGlobalTable(VM_ARG,nodes[nodenum].strindex,
															node->line_no,node->col_no,node->filename);
		compile->SymGlobalTable.sym[compile->HashTable[h]].next = tmpindex;
		return compile->HashTable[h];
	}
	else if(compile->SymGlobalTable.sym[tmpindex].isvalid == ZL_FALSE)
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_GLOBAL_TABLE_FIND_NOT_VALID_INDEX);
	else
	{
		tmpindex = compile->SymGlobalTable.sym[tmpindex].linecols;
		if(tmpindex == 0)
			compile->exit(VM_ARG,ZL_ERR_CP_SYM_GLOBAL_TABLE_INDEX_LINECOL_NOT_INIT);
		else
		{
			while(compile->LineCols.lines[tmpindex].next)
				tmpindex = compile->LineCols.lines[tmpindex].next;
			compile->LineCols.lines[tmpindex].next = compile->insert_LineCol(VM_ARG,node->line_no,node->col_no,node->filename);
		}
	}
	return -1;
}

/*
	��ĳ��������ʾ�����뵽ȫ�ֱ������ű����ȷ���ñ�����Ӧ���ڴ��ַ
*/
ZL_INT zengl_SymInsertGlobalTable(ZL_VOID * VM_ARG,ZL_INT nameIndex,ZL_INT line,ZL_INT col,ZL_CHAR * filename)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZL_INT tmpindex;

	if(!compile->SymGlobalTable.isInit)
		compile->SymInitGlobalTable(VM_ARG);
	if(compile->SymGlobalTable.count == compile->SymGlobalTable.size)
	{
		compile->SymGlobalTable.size += ZL_SYM_GLOBAL_TABLE_SIZE;
		compile->SymGlobalTable.sym = compile->memReAlloc(VM_ARG,compile->SymGlobalTable.sym,
														compile->SymGlobalTable.size * sizeof(ZENGL_SYM_GLOBAL_TABLE_MEMBER));
		ZENGL_SYS_MEM_SET(compile->SymGlobalTable.sym + (compile->SymGlobalTable.size - ZL_SYM_GLOBAL_TABLE_SIZE),0,
						ZL_SYM_GLOBAL_TABLE_SIZE * sizeof(ZENGL_SYM_GLOBAL_TABLE_MEMBER));
	}
	tmpindex = compile->SymGlobalTable.count;
	if(compile->SymGlobalTable.sym[tmpindex].isvalid == ZL_FALSE)
	{
		compile->SymGlobalTable.sym[tmpindex].nameIndex = nameIndex;
		compile->SymGlobalTable.sym[tmpindex].memloc = compile->SymGlobalTable.global_memloc++;
		compile->SymGlobalTable.sym[tmpindex].linecols = compile->insert_LineCol(VM_ARG,line,col,filename);
		compile->SymGlobalTable.sym[tmpindex].next = 0;
		compile->SymGlobalTable.sym[tmpindex].isvalid = ZL_TRUE;
		compile->SymGlobalTable.count++;
		return tmpindex;
	}
	else
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_GLOBAL_TABLE_NO_NOT_VALID_INDEX);
	return -1;
}

/*
	��ʼ��ȫ�ֱ������ű�
*/
ZL_VOID zengl_SymInitGlobalTable(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	if(compile->SymGlobalTable.isInit)
		return;
	compile->SymGlobalTable.count = 1; //��1������ʼ������Ч��������С�ڵ���0��������Ч����������������жϡ�
	compile->SymGlobalTable.size = ZL_SYM_GLOBAL_TABLE_SIZE;
	compile->SymGlobalTable.global_memloc = 0;
	compile->SymGlobalTable.sym = compile->memAlloc(VM_ARG,compile->SymGlobalTable.size * sizeof(ZENGL_SYM_GLOBAL_TABLE_MEMBER));
	if(compile->SymGlobalTable.sym == ZL_NULL)
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_GLOBAL_TABLE_MALLOC_FAILED);
	ZENGL_SYS_MEM_SET(compile->SymGlobalTable.sym,0,compile->SymGlobalTable.size * sizeof(ZENGL_SYM_GLOBAL_TABLE_MEMBER));
	compile->SymGlobalTable.isInit = ZL_TRUE;
}

/*
	��nodenum�������ڵĽڵ���뵽��ϣ�������ű����ڵĶ�̬������
*/
ZL_INT zengl_SymInsertHashTableForClass(ZL_VOID * VM_ARG,ZL_INT nodenum)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_AST_NODE_TYPE * node = nodes + nodenum;
	ZL_CHAR * name = compile->TokenStringPoolGetPtr(VM_ARG,nodes[nodenum].strindex);
	ZL_INT h = compile->hash(VM_ARG,name) + ZL_SYM_HASH_SIZE * ZL_HASH_TYPE_CLASS_TABLE;
	ZL_INT tmpindex = compile->HashTable[h];

	while(tmpindex != 0 && compile->SymClassTable.classTable[tmpindex].isvalid == ZL_TRUE &&
		  ZENGL_SYS_STRCMP(name,compile->TokenStringPoolGetPtr(VM_ARG,compile->SymClassTable.classTable[tmpindex].nameIndex)) != 0)
		tmpindex = compile->SymClassTable.classTable[tmpindex].next;
	if(tmpindex == 0)
	{
		tmpindex = compile->HashTable[h];
		compile->HashTable[h] = compile->SymInsertClassTable(VM_ARG,nodes[nodenum].strindex,nodes[nodenum].parentnode,
															node->line_no,node->col_no,node->filename);
		compile->SymClassTable.classTable[compile->HashTable[h]].next = tmpindex;
		return compile->HashTable[h];
	}
	else if(compile->SymClassTable.classTable[tmpindex].isvalid == ZL_FALSE)
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_CLASS_TABLE_FIND_NOT_VALID_INDEX);
	else
	{
		compile->parser_curnode = nodenum;
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYM_CLASS_HAS_BEEN_DEFINED,
			name,compile->LineCols.lines[compile->SymClassTable.classTable[tmpindex].linecols].lineno,
			compile->LineCols.lines[compile->SymClassTable.classTable[tmpindex].linecols].colno,
			compile->LineCols.lines[compile->SymClassTable.classTable[tmpindex].linecols].filename,
			name);
	}
	return -1;
}

/*
	�������͸��ඨ�����ڵ����к�,�ļ���,�ڵ����Ϣ���뵽����ű��Ӧ�Ķ�̬������
	���õ������Ψһ��id
*/
ZL_INT zengl_SymInsertClassTable(ZL_VOID * VM_ARG,ZL_INT nameIndex,ZL_INT nodenum,ZL_INT line,ZL_INT col,ZL_CHAR * filename)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZL_INT tmpindex;

	if(!compile->SymClassTable.isInit)
		compile->SymInitClassTable(VM_ARG);
	if(compile->SymClassTable.count == compile->SymClassTable.size)
	{
		compile->SymClassTable.size += ZL_SYM_CLASS_TABLE_SIZE;
		compile->SymClassTable.classTable = compile->memReAlloc(VM_ARG,compile->SymClassTable.classTable,
														compile->SymClassTable.size * sizeof(ZENGL_SYM_CLASS_TABLE_MEMBER));
		ZENGL_SYS_MEM_SET(compile->SymClassTable.classTable + (compile->SymClassTable.size - ZL_SYM_CLASS_TABLE_SIZE),0,
						ZL_SYM_CLASS_TABLE_SIZE * sizeof(ZENGL_SYM_CLASS_TABLE_MEMBER));
	}
	tmpindex = compile->SymClassTable.count;
	if(compile->SymClassTable.classTable[tmpindex].isvalid == ZL_FALSE)
	{
		compile->SymClassTable.classTable[tmpindex].nameIndex = nameIndex;
		compile->SymClassTable.classTable[tmpindex].nodenum = nodenum;
		compile->SymClassTable.classTable[tmpindex].classid = compile->SymClassTable.global_classid++;
		compile->SymClassTable.classTable[tmpindex].linecols = compile->insert_LineCol(VM_ARG,line,col,filename);
		compile->SymClassTable.classTable[tmpindex].next = 0;
		compile->SymClassTable.classTable[tmpindex].isvalid = ZL_TRUE;
		compile->SymClassTable.count++;
		return tmpindex;
	}
	else
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_CLASS_TABLE_NO_NOT_VALID_INDEX);
	return -1;
}

/*
	��ʼ������ű��Ӧ�Ķ�̬����
*/
ZL_VOID zengl_SymInitClassTable(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	if(compile->SymClassTable.isInit)
		return;
	compile->SymClassTable.count = 1; //��1������ʼ������Ч��������С�ڵ���0��������Ч����������������жϡ�
	compile->SymClassTable.size = ZL_SYM_CLASS_TABLE_SIZE;
	compile->SymClassTable.global_classid = 1; //��id��1��ʼ��0����Ч����id
	compile->SymClassTable.classTable = compile->memAlloc(VM_ARG,compile->SymClassTable.size * sizeof(ZENGL_SYM_CLASS_TABLE_MEMBER));
	if(compile->SymClassTable.classTable == ZL_NULL)
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_CLASS_TABLE_MALLOC_FAILED);
	ZENGL_SYS_MEM_SET(compile->SymClassTable.classTable,0,compile->SymClassTable.size * sizeof(ZENGL_SYM_CLASS_TABLE_MEMBER));
	compile->SymClassTable.isInit = ZL_TRUE;
}

/*
	�������ڵ���Ϣ���뵽������̬�����У�������̬������������浽HashTable��
*/
ZL_INT zengl_SymInsertHashTableForFun(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT classid)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_AST_NODE_TYPE * node = nodes + nodenum;
	ZL_CHAR * name = compile->TokenStringPoolGetPtr(VM_ARG,nodes[nodenum].strindex);
	ZL_INT h = compile->hash(VM_ARG,name) + ZL_SYM_HASH_SIZE * ZL_HASH_TYPE_FUN_TABLE;
	ZL_INT tmpindex = compile->HashTable[h];

	while(tmpindex != 0 && compile->SymFunTable.funs[tmpindex].isvalid == ZL_TRUE &&
		 (classid != compile->SymFunTable.funs[tmpindex].classid ||
		  ZENGL_SYS_STRCMP(name,compile->TokenStringPoolGetPtr(VM_ARG,compile->SymFunTable.funs[tmpindex].nameIndex)) != 0)) //ѭ������ͬ���������ຯ�������ͬ��������﷨����
		tmpindex = compile->SymFunTable.funs[tmpindex].next;
	if(tmpindex == 0)
	{
		tmpindex = compile->HashTable[h];
		compile->HashTable[h] = compile->SymInsertFunTable(VM_ARG,nodes[nodenum].strindex,classid,
															node->line_no,node->col_no,node->filename);
		compile->SymFunTable.funs[compile->HashTable[h]].next = tmpindex;
		return compile->HashTable[h];
	}
	else if(compile->SymFunTable.funs[tmpindex].isvalid == ZL_FALSE)
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_FUN_TABLE_FIND_NOT_VALID_INDEX);
	else
	{
		compile->parser_curnode = nodenum;
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYM_FUN_HAS_BEEN_DEFINED,
			name,compile->LineCols.lines[compile->SymFunTable.funs[tmpindex].linecols].lineno,
			compile->LineCols.lines[compile->SymFunTable.funs[tmpindex].linecols].colno,
			compile->LineCols.lines[compile->SymFunTable.funs[tmpindex].linecols].filename,
			name);
	}
	return -1;
}

/*
	���������ͺ������ڵ����кŵ���Ϣ���뵽�������ű��Ӧ�Ķ�̬�����С�
*/
ZL_INT zengl_SymInsertFunTable(ZL_VOID * VM_ARG,ZL_INT nameIndex,ZL_INT classid,ZL_INT line,ZL_INT col,ZL_CHAR * filename)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZL_INT tmpindex;

	if(!compile->SymFunTable.isInit)
		compile->SymInitFunTable(VM_ARG);
	if(compile->SymFunTable.count == compile->SymFunTable.size)
	{
		compile->SymFunTable.size += ZL_SYM_FUN_TABLE_SIZE;
		compile->SymFunTable.funs = compile->memReAlloc(VM_ARG,compile->SymFunTable.funs,
														compile->SymFunTable.size * sizeof(ZENGL_SYM_FUN_TABLE_MEMBER));
		ZENGL_SYS_MEM_SET(compile->SymFunTable.funs + (compile->SymFunTable.size - ZL_SYM_FUN_TABLE_SIZE),0,
						ZL_SYM_FUN_TABLE_SIZE * sizeof(ZENGL_SYM_FUN_TABLE_MEMBER));
	}
	tmpindex = compile->SymFunTable.count;
	if(compile->SymFunTable.funs[tmpindex].isvalid == ZL_FALSE)
	{
		compile->SymFunTable.funs[tmpindex].nameIndex = nameIndex;
		compile->SymFunTable.funs[tmpindex].funid = compile->SymFunTable.global_funid++;
		compile->SymFunTable.funs[tmpindex].classid = classid;
		compile->SymFunTable.funs[tmpindex].linecols = compile->insert_LineCol(VM_ARG,line,col,filename);
		compile->SymFunTable.funs[tmpindex].next = 0;
		compile->SymFunTable.funs[tmpindex].isvalid = ZL_TRUE;
		compile->SymFunTable.count++;
		return tmpindex;
	}
	else
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_FUN_TABLE_NO_NOT_VALID_INDEX);
	return -1;
}

/*
	��ʼ���������ű��Ӧ�Ķ�̬����
*/
ZL_VOID zengl_SymInitFunTable(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	if(compile->SymFunTable.isInit)
		return;
	compile->SymFunTable.count = 1; //��1������ʼ������Ч��������С�ڵ���0��������Ч����������������жϡ�
	compile->SymFunTable.size = ZL_SYM_FUN_TABLE_SIZE;
	compile->SymFunTable.global_funid = 1; //����id��1��ʼ��0����Ч�ĺ���id
	compile->SymFunTable.funs = compile->memAlloc(VM_ARG,compile->SymFunTable.size * sizeof(ZENGL_SYM_FUN_TABLE_MEMBER));
	if(compile->SymFunTable.funs == ZL_NULL)
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_FUN_TABLE_MALLOC_FAILED);
	ZENGL_SYS_MEM_SET(compile->SymFunTable.funs,0,compile->SymFunTable.size * sizeof(ZENGL_SYM_FUN_TABLE_MEMBER));
	compile->SymFunTable.isInit = ZL_TRUE;
}

/*
	ɨ�����Ա���������Ա�������Ա���ű��Լ����ຯ�����뺯�����ű��С�
*/
ZL_VOID zengl_ScanInsertClassMemberSym(ZL_VOID * VM_ARG,ZL_INT arg_nodenum,ZL_INT classid)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_SCAN_STACK_TYPE tmpstack;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZL_INT nodenum = arg_nodenum;
	ZL_INT cls_stmt_classid = 0;
	ZL_INT classmember_index = 0;
	compile->push_AST_TreeScanStack(VM_ARG,nodenum); //��nodenum�ڵ�ѹ��ջ����ʾ���ýڵ��µ������ӽڵ����ɨ�衣
	compile->AST_TreeScanStackList.stacks[compile->AST_TreeScanStackList.count - 1].curchild = 1; //���������ڵ�
	do{
		tmpstack = compile->pop_AST_TreeScanStack(VM_ARG,ZL_FALSE); //����ǰ��ѹ��ջ�Ľڵ���Ϣ������ZL_FALSE��ʾֻ������Ϣ���ݲ����ڵ�Ӷ�ջ��ɾ����
		if(tmpstack.nodenum < 0) //��-1֮��Ŀսڵ�������
		{
			compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE);
			continue;
		}
		if(tmpstack.childcnt > 0 &&
		   tmpstack.curchild < tmpstack.childcnt) //������صĽڵ�������ӽڵ㣬��curchildС��childcnt�ӽڵ���ʱ���ͼ���ɨ���ӽڵ㡣
		{
			if(nodes[tmpstack.nodenum].toktype == ZL_TK_CLASS_STATEMENT)
				cls_stmt_classid = compile->SymLookupClass(VM_ARG,tmpstack.nodenum);
			else if(nodes[tmpstack.nodenum].toktype == ZL_TK_RESERVE && nodes[tmpstack.nodenum].reserve == ZL_RSV_FUN)
			{
				compile->SymInsertHashTableForFun(VM_ARG,nodes[tmpstack.nodenum].childs.childnum[0],classid);
				compile->AST_TreeScanStackList.stacks[compile->AST_TreeScanStackList.count - 1].curchild = tmpstack.childcnt; //����class���fun����Ϊ�ຯ����ı�����Ϣ��������ɨ�躯���д���
				continue;
			}

			if(tmpstack.curchild < ZL_AST_CHILD_NODE_SIZE) //���С��ZL_AST_CHILD_NODE_SIZE,�Ͷ�childnum��Ļ����ӽڵ����ɨ�裬����ͽ�extchilds��չ�ӽڵ���Ľڵ����ɨ�衣
				nodenum = nodes[tmpstack.nodenum].childs.childnum[tmpstack.curchild];
			else
				nodenum = nodes[tmpstack.nodenum].childs.extchilds[tmpstack.curchild -
																   ZL_AST_CHILD_NODE_SIZE];
			compile->AST_TreeScanStackList.stacks[compile->AST_TreeScanStackList.count - 1].curchild++; //����ǰɨ��Ľڵ�������һ�������´ξͿ���ɨ����һ���ӽڵ㡣
			compile->push_AST_TreeScanStack(VM_ARG,nodenum); //����ǰɨ����ӽڵ�ѹ��ջ�������ͺ�֮ǰѹ��ջ�Ľڵ��ڶ�ջ�й�����һ���ڵ�·����
			continue; //continue�������do...while��ͷ��Ȼ��pop_AST_TreeScanStack�ͻὫ��ѹ��ջ���ӽڵ㵯������
		}
		else if(nodes[tmpstack.nodenum].toktype == ZL_TK_ID)
		{
			compile->SymInsertHashTableForClassMember(VM_ARG,tmpstack.nodenum,classmember_index++,classid,cls_stmt_classid);
		}
		else if(nodes[tmpstack.nodenum].toktype == ZL_TK_CLASS_STATEMENT)
		{
			cls_stmt_classid = 0;
		}

		if(tmpstack.next != 0 && compile->AST_TreeScanStackList.count > 1) //�����ǰ�ڵ���next�ֵܽڵ��Ҳ��Ǹ��ڵ������£��ͽ���ǰ�ڵ㵯��������next�ڵ�ѹ��ջ
		{
			compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE);
			compile->push_AST_TreeScanStack(VM_ARG,tmpstack.next);
			continue; //continue�������do...while��ͷ��Ȼ��pop_AST_TreeScanStack�ͻὫ��ѹ��ջ���ӽڵ㵯������
		}
		else
			compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE); //���ӽڵ��next�ڵ㶼�������ˣ��ͽ��ڵ�Ӷ�ջ�е�����
	}while(compile->AST_TreeScanStackList.count > 0); //�����ջ�л���Ԫ�أ�˵�����нڵ�û�����ֻ꣬�е���ջ���Ԫ�ظ���Ϊ0ʱ���ʾ����AST��Ľڵ㶼�������ˣ��Ϳ�������ѭ�������ˡ�
}

/*
	���ݽڵ�Ų�����ID��Ϣ
*/
ZL_INT zengl_SymLookupClass(ZL_VOID * VM_ARG,ZL_INT nodenum)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZL_CHAR * name = compile->TokenStringPoolGetPtr(VM_ARG,nodes[nodenum].strindex);
	ZL_INT h = compile->hash(VM_ARG,name) + ZL_SYM_HASH_SIZE * ZL_HASH_TYPE_CLASS_TABLE;
	ZL_INT tmpindex = compile->HashTable[h];
	ZL_INT self_classid = 0;
	// �����self�ڵ㣬��ֱ�Ӵ�self�ڵ�����Ϣ��̬�����в�����ID
	if(compile->SymIsSelfToken(VM_ARG, name))
	{
		self_classid = zengl_static_SymSelfClassTableFindClassID(VM_ARG, nodenum);
		if(!self_classid)
		{
			compile->parser_curnode = nodenum;
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_SELF_MUST_BE_USE_IN_CLASS);
		}
		return self_classid;
	}
	while(tmpindex != 0 && compile->SymClassTable.classTable[tmpindex].isvalid == ZL_TRUE &&
		  ZENGL_SYS_STRCMP(name,compile->TokenStringPoolGetPtr(VM_ARG,compile->SymClassTable.classTable[tmpindex].nameIndex)) != 0)
		tmpindex = compile->SymClassTable.classTable[tmpindex].next;
	if(tmpindex == 0)
	{
		compile->parser_curnode = nodenum;
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_CLASS_NAME_OF_CLS_STMT_NOT_EXISTS,name,name);
	}
	else if(compile->SymClassTable.classTable[tmpindex].isvalid == ZL_FALSE)
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_CLASS_TABLE_FIND_NOT_VALID_INDEX_WHEN_LOOKUP);
	return compile->SymClassTable.classTable[tmpindex].classid;
}

/*
	��������������ID��Ϣ
*/
ZL_INT zengl_SymLookupClassByName(ZL_VOID * VM_ARG,ZL_CHAR * name)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZL_INT h = compile->hash(VM_ARG,name) + ZL_SYM_HASH_SIZE * ZL_HASH_TYPE_CLASS_TABLE;
	ZL_INT tmpindex = compile->HashTable[h];
	while(tmpindex != 0 && compile->SymClassTable.classTable[tmpindex].isvalid == ZL_TRUE &&
		  ZENGL_SYS_STRCMP(name,compile->TokenStringPoolGetPtr(VM_ARG,compile->SymClassTable.classTable[tmpindex].nameIndex)) != 0)
		tmpindex = compile->SymClassTable.classTable[tmpindex].next;
	if(tmpindex == 0)
		return 0;
	else if(compile->SymClassTable.classTable[tmpindex].isvalid == ZL_FALSE)
		return -1;
	return compile->SymClassTable.classTable[tmpindex].classid;
}

/*
	��SymClassMemberTable�в���parent_classid��Ӧ����ĳ�Աnodenum����Ϣ
*/
ZL_INT zengl_SymLookupClassMember(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT parent_classid)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_AST_NODE_TYPE * node = nodes + nodenum;
	ZL_CHAR * name = compile->TokenStringPoolGetPtr(VM_ARG,nodes[nodenum].strindex);
	ZL_INT h = compile->hash(VM_ARG,name) + ZL_SYM_HASH_SIZE * ZL_HASH_TYPE_CLASSMEMBER_TABLE;
	ZL_INT tmpindex = compile->HashTable[h];

	while(tmpindex != 0 && compile->SymClassMemberTable.members[tmpindex].isvalid == ZL_TRUE &&
		 (compile->SymClassMemberTable.members[tmpindex].classid != parent_classid || 
		  ZENGL_SYS_STRCMP(name,compile->TokenStringPoolGetPtr(VM_ARG,compile->SymClassMemberTable.members[tmpindex].nameIndex)) != 0))
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

/*
	��nodenum���Ա���ڵĽڵ���뵽��ϣ������Ա���ű����ڵĶ�̬������
*/
ZL_INT zengl_SymInsertHashTableForClassMember(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT index,ZL_INT classid,ZL_INT cls_stmt_classid)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_AST_NODE_TYPE * node = nodes + nodenum;
	ZL_CHAR * name = compile->TokenStringPoolGetPtr(VM_ARG,nodes[nodenum].strindex);
	ZL_INT h = compile->hash(VM_ARG,name) + ZL_SYM_HASH_SIZE * ZL_HASH_TYPE_CLASSMEMBER_TABLE;
	ZL_INT tmpindex = compile->HashTable[h];

	while(tmpindex != 0 && compile->SymClassMemberTable.members[tmpindex].isvalid == ZL_TRUE &&
		 (compile->SymClassMemberTable.members[tmpindex].classid != classid || 
		  ZENGL_SYS_STRCMP(name,compile->TokenStringPoolGetPtr(VM_ARG,compile->SymClassMemberTable.members[tmpindex].nameIndex)) != 0))
		tmpindex = compile->SymClassMemberTable.members[tmpindex].next;
	if(tmpindex == 0)
	{
		tmpindex = compile->HashTable[h];
		compile->HashTable[h] = compile->SymInsertClassMemberTable(VM_ARG,nodes[nodenum].strindex,index,classid,cls_stmt_classid,nodenum);
		compile->SymClassMemberTable.members[compile->HashTable[h]].next = tmpindex;
		return compile->HashTable[h];
	}
	else if(compile->SymClassMemberTable.members[tmpindex].isvalid == ZL_FALSE)
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_CLASSMEMBER_TABLE_FIND_NOT_VALID_INDEX);
	else
	{
		compile->parser_curnode = nodenum;
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_CLASSMEMBER_HAS_BEEN_DEFINED,
			name,compile->TokenStringPoolGetPtr(VM_ARG,compile->SymClassTable.classTable[classid].nameIndex),
			nodes[compile->SymClassMemberTable.members[tmpindex].nodenum].line_no,
			nodes[compile->SymClassMemberTable.members[tmpindex].nodenum].col_no,
			nodes[compile->SymClassMemberTable.members[tmpindex].nodenum].filename,
			name);
	}
	return -1;
}

/*
	�����Ա�������Ա����ṹ�е�����ֵ��������ID����Ϣ�������Ա���ű���
*/
ZL_INT zengl_SymInsertClassMemberTable(ZL_VOID * VM_ARG,ZL_INT nameIndex,ZL_INT index,ZL_INT classid,ZL_INT cls_stmt_classid,ZL_INT nodenum)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZL_INT tmpindex;

	if(!compile->SymClassMemberTable.isInit)
		compile->SymInitClassMemberTable(VM_ARG);
	if(compile->SymClassMemberTable.count == compile->SymClassMemberTable.size)
	{
		compile->SymClassMemberTable.size += ZL_SYM_CLASSMEMBER_TABLE_SIZE;
		compile->SymClassMemberTable.members = compile->memReAlloc(VM_ARG,compile->SymClassMemberTable.members,
															compile->SymClassMemberTable.size * sizeof(ZENGL_SYM_CLASSMEMBER_TABLE_MEMBER));
		ZENGL_SYS_MEM_SET(compile->SymClassMemberTable.members + (compile->SymClassMemberTable.size - ZL_SYM_CLASSMEMBER_TABLE_SIZE),0,
						ZL_SYM_CLASSMEMBER_TABLE_SIZE * sizeof(ZENGL_SYM_CLASSMEMBER_TABLE_MEMBER));
	}
	tmpindex = compile->SymClassMemberTable.count;
	if(compile->SymClassMemberTable.members[tmpindex].isvalid == ZL_FALSE)
	{
		compile->SymClassMemberTable.members[tmpindex].nameIndex = nameIndex;
		compile->SymClassMemberTable.members[tmpindex].index = index;
		compile->SymClassMemberTable.members[tmpindex].nodenum = nodenum;
		compile->SymClassMemberTable.members[tmpindex].classid = classid;
		compile->SymClassMemberTable.members[tmpindex].cls_stmt_classid = cls_stmt_classid;
		compile->SymClassMemberTable.members[tmpindex].next = 0;
		compile->SymClassMemberTable.members[tmpindex].isvalid = ZL_TRUE;
		compile->SymClassMemberTable.count++;
		return tmpindex;
	}
	else
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_CLASSMEMBER_TABLE_NO_NOT_VALID_INDEX);
	return -1;
}

/*
	��ʼ�����Ա���ű��Ӧ�Ķ�̬����
*/
ZL_VOID zengl_SymInitClassMemberTable(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	if(compile->SymClassMemberTable.isInit)
		return;
	compile->SymClassMemberTable.count = 1; //��1������ʼ������Ч��������С�ڵ���0��������Ч����������������жϡ�
	compile->SymClassMemberTable.size = ZL_SYM_CLASSMEMBER_TABLE_SIZE;
	compile->SymClassMemberTable.members = compile->memAlloc(VM_ARG,compile->SymClassMemberTable.size * sizeof(ZENGL_SYM_CLASSMEMBER_TABLE_MEMBER));
	if(compile->SymClassMemberTable.members == ZL_NULL)
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_CLASSMEMBER_TABLE_MALLOC_FAILED);
	ZENGL_SYS_MEM_SET(compile->SymClassMemberTable.members,0,compile->SymClassMemberTable.size * sizeof(ZENGL_SYM_CLASSMEMBER_TABLE_MEMBER));
	compile->SymClassMemberTable.isInit = ZL_TRUE;
}

/**
	ͨ���ڵ����������Ҹýڵ�ı�����ʾ�����Զ���������ڴ��е��ڴ��ַ���������ڴ��ַ����ؽṹ	
*/
ZENGL_RUN_INST_OP_DATA zengl_SymLookupID(ZL_VOID * VM_ARG,ZL_INT nodenum)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_AST_NODE_TYPE * node = nodes + nodenum;
	ZL_CHAR * name = compile->TokenStringPoolGetPtr(VM_ARG,nodes[nodenum].strindex);
	ZL_INT h;
	ZL_INT tmpindex;
	ZL_BOOL is_local_global = ZL_FALSE;
	ZENGL_RUN_INST_OP_DATA ret_data;
	
	if(!compile->gencode_struct.is_inFun)
	{
FindGlobalSymLoc:
		h = compile->hash(VM_ARG,name) + ZL_SYM_HASH_SIZE * ZL_HASH_TYPE_SYM_TABLE;
		tmpindex = compile->HashTable[h];
		while(tmpindex != 0 && compile->SymGlobalTable.sym[tmpindex].isvalid == ZL_TRUE &&
			  ZENGL_SYS_STRCMP(name,compile->TokenStringPoolGetPtr(VM_ARG,compile->SymGlobalTable.sym[tmpindex].nameIndex)) != 0)
			tmpindex = compile->SymGlobalTable.sym[tmpindex].next;
		if(is_local_global == ZL_TRUE && tmpindex == 0)
		{
			tmpindex = compile->SymInsertHashTableForGlobal(VM_ARG,nodenum); //�����fun����������ͨ��global�������µ�ȫ�ֱ���������SymInsertHashTableForGlobal��ȫ�ֱ���HASH���в���һ���µ�ȫ�ֱ�����
			goto ReturnGlobalSymInfo;
		}
		else if(tmpindex == 0)
		{
			compile->parser_curnode = nodenum;
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYM_CAN_NOT_FIND_ID_MEMLOC,name,name);
		}
		else if(compile->SymGlobalTable.sym[tmpindex].isvalid == ZL_FALSE)
			compile->exit(VM_ARG,ZL_ERR_CP_SYM_GLOBAL_TABLE_FIND_NOT_VALID_INDEX_WHEN_LOOKUP);
ReturnGlobalSymInfo:
		ret_data.type = ZL_R_DT_MEM;
		ret_data.val.mem = compile->SymGlobalTable.sym[tmpindex].memloc;
		return ret_data;
	}
	else
	{
		h = compile->hash(VM_ARG,name) + ZL_SYM_HASH_SIZE * ZL_HASH_TYPE_LOCAL_TABLE;
		tmpindex = compile->HashTable[h];
		while(tmpindex != 0 && compile->SymLocalTable.local[tmpindex].isvalid == ZL_TRUE && 
			(ZENGL_SYS_STRCMP(name,compile->TokenStringPoolGetPtr(VM_ARG,compile->SymLocalTable.local[tmpindex].nameIndex)) != 0 || 
			compile->SymLocalTable.local[tmpindex].funid != compile->SymFunTable.global_funid))
		{
			tmpindex = compile->SymLocalTable.local[tmpindex].next;
		}
		if(tmpindex == 0)
		{
			compile->parser_curnode = nodenum;
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYM_CAN_NOT_FIND_LOCAL_ID_MEMLOC,name,name);
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
			is_local_global = ZL_TRUE;
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

/**
	Ϊnodenum�ڵ�����classid��id	
*/
ZL_VOID zengl_SymInsertClassID(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT classid)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_AST_NODE_TYPE * node = nodes + nodenum;
	ZL_CHAR * name = compile->TokenStringPoolGetPtr(VM_ARG,nodes[nodenum].strindex);
	ZL_INT h;
	ZL_INT tmpindex;
	ZL_BOOL is_local_global = ZL_FALSE;
	
	if(!compile->gencode_struct.is_inFun)
	{
FindGlobalSymLoc:
		h = compile->hash(VM_ARG,name) + ZL_SYM_HASH_SIZE * ZL_HASH_TYPE_SYM_TABLE;
		tmpindex = compile->HashTable[h];
		while(tmpindex != 0 && compile->SymGlobalTable.sym[tmpindex].isvalid == ZL_TRUE &&
			  ZENGL_SYS_STRCMP(name,compile->TokenStringPoolGetPtr(VM_ARG,compile->SymGlobalTable.sym[tmpindex].nameIndex)) != 0)
			tmpindex = compile->SymGlobalTable.sym[tmpindex].next;
		if(is_local_global == ZL_TRUE && tmpindex == 0)
		{
			tmpindex = compile->SymInsertHashTableForGlobal(VM_ARG,nodenum); //�����fun����������ͨ��global�������µ�ȫ�ֱ���������SymInsertHashTableForGlobal��ȫ�ֱ���HASH���в���һ���µ�ȫ�ֱ�����
			goto ReturnGlobalSymInfo;
		}
		else if(tmpindex == 0)
		{
			compile->parser_curnode = nodenum;
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYM_CAN_NOT_FIND_ID_MEMLOC,name,name);
		}
		else if(compile->SymGlobalTable.sym[tmpindex].isvalid == ZL_FALSE)
			compile->exit(VM_ARG,ZL_ERR_CP_SYM_GLOBAL_TABLE_FIND_NOT_VALID_INDEX_WHEN_LOOKUP);
ReturnGlobalSymInfo:
		compile->SymGlobalTable.sym[tmpindex].classid = classid;
		return ;
	}
	else
	{
		h = compile->hash(VM_ARG,name) + ZL_SYM_HASH_SIZE * ZL_HASH_TYPE_LOCAL_TABLE;
		tmpindex = compile->HashTable[h];
		while(tmpindex != 0 && compile->SymLocalTable.local[tmpindex].isvalid == ZL_TRUE && 
			(ZENGL_SYS_STRCMP(name,compile->TokenStringPoolGetPtr(VM_ARG,compile->SymLocalTable.local[tmpindex].nameIndex)) != 0 || 
			compile->SymLocalTable.local[tmpindex].funid != compile->SymFunTable.global_funid))
		{
			tmpindex = compile->SymLocalTable.local[tmpindex].next;
		}
		if(tmpindex == 0)
		{
			compile->parser_curnode = nodenum;
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYM_CAN_NOT_FIND_LOCAL_ID_MEMLOC,name,name);
		}
		else if(compile->SymLocalTable.local[tmpindex].isvalid == ZL_FALSE)
			compile->exit(VM_ARG,ZL_ERR_CP_SYM_LOCAL_TABLE_INVALID_INDEX);
		else if(compile->SymLocalTable.local[tmpindex].funid != compile->SymFunTable.global_funid)
			compile->exit(VM_ARG,ZL_ERR_CP_SYM_LOCAL_ID_INVALID_FUN_ID);
		switch(compile->SymLocalTable.local[tmpindex].type)
		{
		case ZL_SYM_ENUM_LOCAL_TYPE_ARG:
		case ZL_SYM_ENUM_LOCAL_TYPE_LOCAL:
			compile->SymLocalTable.local[tmpindex].classid = classid;
			break;
		case ZL_SYM_ENUM_LOCAL_TYPE_GLOBAL:
			is_local_global = ZL_TRUE;
			goto FindGlobalSymLoc;
			break;
		default:
			compile->exit(VM_ARG,ZL_ERR_CP_SYM_LOCAL_ID_INVALID_TYPE);
			break;
		}
		return ;
	}
	return ;
}

/*
    ����nodenum��Ӧ�ڵ��classidֵ����Ҫ�������ɵ�������Ļ��ָ��ʱ
*/
ZL_INT zengl_SymLookupID_ForDot(ZL_VOID * VM_ARG,ZL_INT nodenum)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
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
			  ZENGL_SYS_STRCMP(name,compile->TokenStringPoolGetPtr(VM_ARG,compile->SymGlobalTable.sym[tmpindex].nameIndex)) != 0)
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
			(ZENGL_SYS_STRCMP(name,compile->TokenStringPoolGetPtr(VM_ARG,compile->SymLocalTable.local[tmpindex].nameIndex)) != 0 || 
			compile->SymLocalTable.local[tmpindex].funid != compile->SymFunTable.global_funid))
		{
			tmpindex = compile->SymLocalTable.local[tmpindex].next;
		}
		if(tmpindex == 0)
		{
			compile->parser_curnode = nodenum;
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYM_CAN_NOT_FIND_LOCAL_ID_MEMLOC,name,name);
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

/**
	ͨ���������Ʋ��ұ�����ʾ�����Զ���������ڴ��е��ڴ��ַ����Ҫ���ڶ����API�ӿ���	
*/
ZL_INT zengl_SymLookupID_ByName(ZL_VOID * VM_ARG,ZL_CHAR * name)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZL_INT h;
	ZL_INT tmpindex;
	h = compile->hash(VM_ARG,name) + ZL_SYM_HASH_SIZE * ZL_HASH_TYPE_SYM_TABLE;
	tmpindex = compile->HashTable[h];
	while(tmpindex != 0 && compile->SymGlobalTable.sym[tmpindex].isvalid == ZL_TRUE &&
		  ZENGL_SYS_STRCMP(name,compile->TokenStringPoolGetPtr(VM_ARG,compile->SymGlobalTable.sym[tmpindex].nameIndex)) != 0)
		tmpindex = compile->SymGlobalTable.sym[tmpindex].next;
	if(tmpindex == 0)
		return -1;
	else if(compile->SymGlobalTable.sym[tmpindex].isvalid == ZL_FALSE)
		return -2;
	return compile->SymGlobalTable.sym[tmpindex].memloc;
}

/*
	���ݽڵ�Ų��Һ���ID��Ϣ
*/
ZL_INT zengl_SymLookupFun(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT classid)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_AST_NODE_TYPE * node = nodes + nodenum;
	ZL_CHAR * name = compile->TokenStringPoolGetPtr(VM_ARG,nodes[nodenum].strindex);
	ZL_INT h = compile->hash(VM_ARG,name) + ZL_SYM_HASH_SIZE * ZL_HASH_TYPE_FUN_TABLE;
	ZL_INT tmpindex = compile->HashTable[h];

	while(tmpindex != 0 && compile->SymFunTable.funs[tmpindex].isvalid == ZL_TRUE &&
		 (classid != compile->SymFunTable.funs[tmpindex].classid ||
		  ZENGL_SYS_STRCMP(name,compile->TokenStringPoolGetPtr(VM_ARG,compile->SymFunTable.funs[tmpindex].nameIndex)) != 0)) //ѭ������ͬ���������ຯ��
		tmpindex = compile->SymFunTable.funs[tmpindex].next;
	if(tmpindex == 0)
		return 0; //���û�ҵ���˵�������û��Զ���ĺ�������ô�п�����ģ����ĺ�����
	else if(compile->SymFunTable.funs[tmpindex].isvalid == ZL_FALSE)
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_FUN_TABLE_FIND_NOT_VALID_INDEX);
	return compile->SymFunTable.funs[tmpindex].funid; //����ID��1��ʼ���ҵ���funs�����Ӧ������ֵ
}

/*
	���ݺ������Ʋ��Һ���ID��Ϣ
*/
ZL_INT zengl_SymLookupFunByName(ZL_VOID * VM_ARG,ZL_CHAR * name,ZL_INT classid)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZL_INT h;
	ZL_INT tmpindex;
	if(name == ZL_NULL)
		return 0;
	h = compile->hash(VM_ARG,name) + ZL_SYM_HASH_SIZE * ZL_HASH_TYPE_FUN_TABLE;
	tmpindex = compile->HashTable[h];
	while(tmpindex != 0 && compile->SymFunTable.funs[tmpindex].isvalid == ZL_TRUE &&
		 (classid != compile->SymFunTable.funs[tmpindex].classid ||
		  ZENGL_SYS_STRCMP(name,compile->TokenStringPoolGetPtr(VM_ARG,compile->SymFunTable.funs[tmpindex].nameIndex)) != 0)) //ѭ������ͬ���������ຯ�������ͬ��������﷨����
		tmpindex = compile->SymFunTable.funs[tmpindex].next;
	if(tmpindex == 0)
		return 0; //���û�ҵ���˵�������û��Զ���ĺ�������ô�п�����ģ����ĺ�����
	else if(compile->SymFunTable.funs[tmpindex].isvalid == ZL_FALSE)
		return -1;
	return compile->SymFunTable.funs[tmpindex].funid; //����ID��1��ʼ���ҵ���funs�����Ӧ������ֵ
}

/*
	ʹ��ASTɨ���ջ��ɨ���﷨���к����Ĳ���
*/
ZL_VOID zengl_SymScanFunArg(ZL_VOID * VM_ARG,ZL_INT nodenum)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_AST_SCAN_STACK_TYPE tmpstack;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZL_INT * chnum;
	ZL_INT chnum_0, chnum_1;
	ZL_BOOL is_negative;
	ZENGL_RUN_INST_OP_DATA_TYPE tmpInstType = ZL_R_DT_NONE;
	ZL_DOUBLE tmpInstData = 0;
	if(compile->AST_TreeScanStackList.count != 0)
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_AST_TREE_SCAN_STACK_NOT_EMPTY);
	if(nodenum == -1) //������������б��ǿյ��򷵻�
		return;
	compile->push_AST_TreeScanStack(VM_ARG,nodenum); //�Ƚ��ڵ�ѹ��ջ���γ�ɨ��ڵ�·������ʼ�㡣
	do{
		tmpstack = compile->pop_AST_TreeScanStack(VM_ARG,ZL_FALSE); //����ǰ��ѹ��ջ�Ľڵ���Ϣ������ZL_FALSE��ʾֻ������Ϣ���ݲ����ڵ�Ӷ�ջ��ɾ����
		if(tmpstack.nodenum < 0) //��-1֮��Ŀսڵ�������
		{
			compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE);
			continue;
		}
		if(tmpstack.curchild == 0) //curchildΪ0ʱ����ʾ��û��ʼɨ���ӽڵ㣬�ͶԵ�ǰ�ڵ���д���
		{
			nodenum = tmpstack.nodenum;
			switch(nodes[nodenum].toktype)
			{
			case ZL_TK_ID:
				if(compile->SymInsertHashTableForLocal(VM_ARG,nodenum,ZL_SYM_ENUM_LOCAL_TYPE_ARG) == ZL_TRUE) //����ǽű������Ĳ������ͽ�����뵽SymLocalTable�ֲ��������ű�̬�����У��������ʾΪZL_SYM_ENUM_LOCAL_TYPE_ARG
					compile->gencode_struct.localID++;
				break;
			case ZL_TK_COMMA:
				break;
			case ZL_TK_ASSIGN:
				chnum = nodes[nodenum].childs.childnum;
				is_negative = ZL_FALSE;
				chnum_0 = chnum[0];
				if(nodes[chnum[1]].toktype == ZL_TK_NEGATIVE && nodes[chnum[1]].childs.count == 1) {
					chnum_1 = nodes[chnum[1]].childs.childnum[0];
					is_negative = ZL_TRUE;
				}
				else {
					chnum_1 = chnum[1];
				}
				switch(nodes[chnum_1].toktype)
				{
				case ZL_TK_NUM:
					tmpInstType = ZL_R_DT_NUM;
					tmpInstData = (ZL_DOUBLE)ZENGL_SYS_STR_TO_LONG_NUM(compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum_1].strindex));
					break;
				case ZL_TK_FLOAT:
					tmpInstType = ZL_R_DT_FLOAT;
					tmpInstData = ZENGL_SYS_STR_TO_FLOAT(compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum_1].strindex));
					break;
				case ZL_TK_STR:
					tmpInstType = ZL_R_DT_STR;
					tmpInstData = (ZL_DOUBLE)((ZL_LONG)compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum_1].strindex));
					break;
				default:
					compile->parser_curnode = nodenum;
					compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_FUN_ARGLIST_INVALID_TOKEN);
					break;
				}
				if((tmpInstType == ZL_R_DT_NUM || tmpInstType == ZL_R_DT_FLOAT) && is_negative) {
					tmpInstData = -tmpInstData;
				}
				run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
					ZL_R_IT_ARG_SET , ZL_R_DT_ARGMEM , compile->gencode_struct.localID,
					tmpInstType , tmpInstData); //��Ӧ���ָ�� ���� "ARG_SET arg(%d) 123" ��ĳ������û�б���ֵʱ�����õ�Ĭ��ֵ
				if(compile->SymInsertHashTableForLocal(VM_ARG,chnum_0,ZL_SYM_ENUM_LOCAL_TYPE_ARG) == ZL_TRUE)
					compile->gencode_struct.localID++;
				break;
			default:
				//compile->parser_curnode = nodenum;
				//compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_FUN_ARGLIST_INVALID_TOKEN);
				break;
			}
		}
		if(tmpstack.curchild < tmpstack.childcnt) //���curchildС��childcnt�ӽڵ�������˵���ӽڵ�û�����꣬����������ӽڵ���Ϣ��
		{
			if(tmpstack.curchild < ZL_AST_CHILD_NODE_SIZE) //����curchild�����ж��ǻ����ӽڵ㻹����չ�ӽڵ㡣
				nodenum = nodes[tmpstack.nodenum].childs.childnum[tmpstack.curchild];
			else
				nodenum = nodes[tmpstack.nodenum].childs.extchilds[tmpstack.curchild -
																   ZL_AST_CHILD_NODE_SIZE];
			compile->AST_TreeScanStackList.stacks[compile->AST_TreeScanStackList.count - 1].curchild++; //��curchild��һ���´ξͻᴦ����һ���ӽڵ���Ϣ��
			compile->push_AST_TreeScanStack(VM_ARG,nodenum); //���ӽڵ�ѹ��ջ
			continue; //continue�������do...while��ͷ��Ȼ��pop_TreeStack���Ӷ�����ǰ��ѹ��ջ���ӽڵ���Ϣ���д���
		}
		else
			compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE); //���û���ӽڵ�����ӽڵ�Ҳ��ɨ���꣬�򵯳���ǰ�ڵ㡣
	}while(compile->AST_TreeScanStackList.count > 0); //�����ջ�л���Ԫ�أ�˵�����нڵ�û�����ֻ꣬�е���ջ���Ԫ�ظ���Ϊ0ʱ���ʾ����AST��Ľڵ㶼�������ˣ��Ϳ�������ѭ�������ˡ�
}

/*
	ʹ��ASTɨ���ջ��ɨ���﷨���к�����global�ؼ���������ȫ�ֱ���
*/
ZL_VOID zengl_SymScanFunGlobal(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT global_nodenum)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_AST_SCAN_STACK_TYPE tmpstack;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZL_INT globalnum;
	ZENGL_RUN_INST_OP_DATA_TYPE tmpInstType = ZL_R_DT_NONE;
	ZL_DOUBLE tmpInstData = 0;
	globalnum = compile->AST_TreeScanStackList.count; //��Ϊglobal�ؼ�����fun�ڲ�����ɨ��ֲ�������ʱ����ӵ�ɨ�赽global��ȫ�ֱ���������global��Ӧ����ʼ��ջ������һ��Ϊ0�����Դ���0����Ϊ֮ǰѹ��Ŀ����Ǿֲ�������ص�ɨ��ڵ���Ϣ��
	if(nodenum == -1)
	{
		compile->parser_curnode = global_nodenum;
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_SYM_GLOBAL_MUST_HAVE_CHILD_NODE);
	}
	compile->push_AST_TreeScanStack(VM_ARG,nodenum); //�Ƚ��ڵ�ѹ��ջ���γ�ɨ��ڵ�·������ʼ�㡣
	do{
		tmpstack = compile->pop_AST_TreeScanStack(VM_ARG,ZL_FALSE); //����ǰ��ѹ��ջ�Ľڵ���Ϣ������ZL_FALSE��ʾֻ������Ϣ���ݲ����ڵ�Ӷ�ջ��ɾ����
		if(tmpstack.nodenum < 0) //��-1֮��Ŀսڵ�������
		{
			compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE);
			continue;
		}
		if(tmpstack.curchild == 0) //curchildΪ0ʱ����ʾ��û��ʼɨ���ӽڵ㣬�ͶԵ�ǰ�ڵ���д���
		{
			nodenum = tmpstack.nodenum;
			switch(nodes[nodenum].toktype)
			{
			case ZL_TK_ID:
				compile->SymInsertHashTableForLocal(VM_ARG,nodenum,ZL_SYM_ENUM_LOCAL_TYPE_GLOBAL); //����ǽű�������global������ȫ�ֱ������ͽ�����뵽SymLocalTable�ֲ��������ű�̬�����У��������ʾΪZL_SYM_ENUM_LOCAL_TYPE_GLOBAL
				break;
			case ZL_TK_COMMA:
				break;
			default:
				compile->exit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_TOKEN,
					nodes[nodenum].line_no,
					nodes[nodenum].col_no,
					nodes[nodenum].filename,
					compile->getTokenStr(VM_ARG,nodes,nodenum));
				break;
			}
		}
		if(tmpstack.curchild < tmpstack.childcnt) //���curchildС��childcnt�ӽڵ�������˵���ӽڵ�û�����꣬����������ӽڵ���Ϣ��
		{
			if(tmpstack.curchild < ZL_AST_CHILD_NODE_SIZE) //����curchild�����ж��ǻ����ӽڵ㻹����չ�ӽڵ㡣
				nodenum = nodes[tmpstack.nodenum].childs.childnum[tmpstack.curchild];
			else
				nodenum = nodes[tmpstack.nodenum].childs.extchilds[tmpstack.curchild -
																   ZL_AST_CHILD_NODE_SIZE];
			compile->AST_TreeScanStackList.stacks[compile->AST_TreeScanStackList.count - 1].curchild++; //��curchild��һ���´ξͻᴦ����һ���ӽڵ���Ϣ��
			compile->push_AST_TreeScanStack(VM_ARG,nodenum); //���ӽڵ�ѹ��ջ
			continue; //continue�������do...while��ͷ��Ȼ��pop_TreeStack���Ӷ�����ǰ��ѹ��ջ���ӽڵ���Ϣ���д���
		}
		else
			compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE); //���û���ӽڵ�����ӽڵ�Ҳ��ɨ���꣬�򵯳���ǰ�ڵ㡣
	}while(compile->AST_TreeScanStackList.count > globalnum); //ɨ���ջ�ļ�����ֵ����globalnum��ʼ����ֵʱ����˵��ɨ�账����ϲ�����ѭ����
}

/*ɨ������������������ı���*/
ZL_VOID zengl_SymScanClassStatement(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT cls_statement_nodenum)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_SCAN_STACK_TYPE tmpstack;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZL_INT classid = compile->SymLookupClass(VM_ARG,cls_statement_nodenum);
	if(compile->AST_TreeScanStackList.count != 0)
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_AST_TREE_SCAN_STACK_NOT_EMPTY);
	if(nodenum == -1)
	{
		compile->parser_curnode = cls_statement_nodenum;
		compile->parser_errorExit(VM_ARG,ZL_ERR_SYNTAX_SYM_CLASS_STATEMENT_MUST_HAVE_CHILD_NODE);
	}
	compile->push_AST_TreeScanStack(VM_ARG,nodenum); //�Ƚ��ڵ�ѹ��ջ���γ�ɨ��ڵ�·������ʼ��
	do{
		tmpstack = compile->pop_AST_TreeScanStack(VM_ARG,ZL_FALSE); //����ǰ��ѹ��ջ�Ľڵ���Ϣ������ZL_FALSE��ʾֻ������Ϣ���ݲ����ڵ�Ӷ�ջ��ɾ��
		if(tmpstack.nodenum < 0) //��-1֮��Ŀսڵ�������
		{
			compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE);
			continue;
		}
		if(tmpstack.curchild == 0) //curchildΪ0ʱ����ʾ��û��ʼɨ���ӽڵ㣬�ͶԵ�ǰ�ڵ���д���
		{
			nodenum = tmpstack.nodenum;
			switch(nodes[nodenum].toktype)
			{
			case ZL_TK_ID:
				compile->SymInsertClassID(VM_ARG,nodenum,classid);
				break;
			case ZL_TK_COMMA:
				break;
			default:
				compile->exit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_TOKEN,
					nodes[nodenum].line_no,
					nodes[nodenum].col_no,
					nodes[nodenum].filename,
					compile->getTokenStr(VM_ARG,nodes,nodenum));
				break;
			}
		}
		if(tmpstack.curchild < tmpstack.childcnt) //���curchildС��childcnt�ӽڵ�������˵���ӽڵ�û�����꣬����������ӽڵ���Ϣ��
		{
			if(tmpstack.curchild < ZL_AST_CHILD_NODE_SIZE) //����curchild�����ж��ǻ����ӽڵ㻹����չ�ӽڵ㡣
				nodenum = nodes[tmpstack.nodenum].childs.childnum[tmpstack.curchild];
			else
				nodenum = nodes[tmpstack.nodenum].childs.extchilds[tmpstack.curchild -
																   ZL_AST_CHILD_NODE_SIZE];
			compile->AST_TreeScanStackList.stacks[compile->AST_TreeScanStackList.count - 1].curchild++; //��curchild��һ���´ξͻᴦ����һ���ӽڵ���Ϣ��
			compile->push_AST_TreeScanStack(VM_ARG,nodenum); //���ӽڵ�ѹ��ջ
			continue; //continue�������do...while��ͷ��Ȼ��pop_TreeStack���Ӷ�����ǰ��ѹ��ջ���ӽڵ���Ϣ���д���
		}
		else
			compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE); //���û���ӽڵ�����ӽڵ�Ҳ��ɨ���꣬�򵯳���ǰ�ڵ㡣
	}while(compile->AST_TreeScanStackList.count > 0); //�����ջ�л���Ԫ�أ�˵�����нڵ�û�����ֻ꣬�е���ջ���Ԫ�ظ���Ϊ0ʱ���ʾ����AST��Ľڵ㶼�������ˣ��Ϳ�������ѭ��������
}


typedef enum _ZENGL_ASMGC_SCAN_DOT_FOR_CLASS_STATUS{
	ZENGL_ASMGC_SCAN_DOT_FOR_CLASS_STATUS_START,
	ZENGL_ASMGC_SCAN_DOT_FOR_CLASS_STATUS_FIRST_PLACE,
	ZENGL_ASMGC_SCAN_DOT_FOR_CLASS_STATUS_SECOND_PLACE,
} ZENGL_ASMGC_SCAN_DOT_FOR_CLASS_STATUS;

typedef struct _ZENGL_ASMGC_SCAN_DOT_FOR_CLASS_STACK_VAL{
	ZL_INT nodenum;
	ZL_INT classid;
	ZL_INT start_scan_num;
	ZL_BOOL initScan;
	ZENGL_AST_SCAN_STACK_TYPE tmpstack;
	ZENGL_ASMGC_SCAN_DOT_FOR_CLASS_STATUS status;
} ZENGL_ASMGC_SCAN_DOT_FOR_CLASS_STACK_VAL;

/*
    ɨ��������ʱ�Ľڵ㣬�����ԱתΪ���������ѹ��ջ�С�
	��test.test2[3].name.val
	���test2��test���е�����Ϊ1��name��test2���������Ϊ2,val��name�������Ϊ3��
	���ʽ�൱��test[1,3,2,3]����һ��1����test2��Ա,�ڶ���3��test2[3]�������3
	������2����name�����ĸ�3����val
	Ȼ��Ͱ�����ķ�ʽ��1,3,2,3����ѹ��ջ��Ϊtest���������ֵ������test�౾���Ͼ���
	һ�����顣 
	
	�������ǲ��õ�����������ӵģ����Ըú��������˶�ջɨ�跨
	���ڵ�������ĵ�һ���ӽڵ��в�����id��Ȼ��ʹ�ø���id�ҵ��ڶ����ӽڵ㼴���Ա��
	���������Ž�����PUSHѹ��ջ��
*/
ZENGL_STATES zengl_SymScanDotForClass(ZL_VOID * VM_ARG,ZL_INT nodenum, ZENGL_ASM_LOOP_STACK_TYPE ** loopStackTopArg)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_ASM_LOOP_STACK_TYPE * loopStackTop = (*loopStackTopArg);
	ZENGL_ASMGC_SCAN_DOT_FOR_CLASS_STACK_VAL * stackVal;
	if(loopStackTop != ZL_NULL) // ��ΪNULL��˵���Ǵ�zengl_AsmGenCode_Dot�����ģ��ͽ��Լ�ѹ����ģ��ջ���´��ڻ�������ѭ��ʱ���ͻ�ֱ�ӽ��뵱ǰ����
	{
		ZL_INT orig_count;
		ZENGL_ASM_LOOP_STACK_TYPE * dot_loopStackTop;
		ZENGL_ASMGC_DOT_STACK_VAL * dot_loopStackTop_stackVal;
		orig_count = compile->AsmGCLoopStackList.count;
		zengl_AsmGCLoopStackPush(VM_ARG, nodenum, ZL_ST_ASM_CODE_INDOT_SYM_SCAN);
		dot_loopStackTop = & compile->AsmGCLoopStackList.stacks[orig_count - 1];
		loopStackTop = & compile->AsmGCLoopStackList.stacks[compile->AsmGCLoopStackList.count - 1];
		stackVal = (ZENGL_ASMGC_SCAN_DOT_FOR_CLASS_STACK_VAL *)zengl_AsmGCLoopStackValsPush(VM_ARG, loopStackTop, sizeof(ZENGL_ASMGC_SCAN_DOT_FOR_CLASS_STACK_VAL));
		stackVal->nodenum = nodenum;
		stackVal->classid = 0;
		stackVal->start_scan_num = 0;
		stackVal->initScan = ZL_TRUE;
		dot_loopStackTop_stackVal = (ZENGL_ASMGC_DOT_STACK_VAL *)(& compile->AsmGCLoopStackList.stackVals[dot_loopStackTop->stackValIndex]);
		dot_loopStackTop_stackVal->status = ZENGL_ASMGC_DOT_STATUS_FINISH_SCAN_DOT;
		stackVal->status = ZENGL_ASMGC_SCAN_DOT_FOR_CLASS_STATUS_START;
	}
	else // ������Ǵӻ���������ѭ��ֱ�ӽ�����
	{
		loopStackTop = & compile->AsmGCLoopStackList.stacks[compile->AsmGCLoopStackList.count - 1];
		stackVal = (ZENGL_ASMGC_SCAN_DOT_FOR_CLASS_STACK_VAL *)(& compile->AsmGCLoopStackList.stackVals[loopStackTop->stackValIndex]);
	}
	
	switch(stackVal->status)
	{
	case ZENGL_ASMGC_SCAN_DOT_FOR_CLASS_STATUS_START:
		break;
	case ZENGL_ASMGC_SCAN_DOT_FOR_CLASS_STATUS_FIRST_PLACE:
		goto first_place;
	case ZENGL_ASMGC_SCAN_DOT_FOR_CLASS_STATUS_SECOND_PLACE:
		goto second_place;
	}

	/*ZENGL_AST_SCAN_STACK_TYPE tmpstack;
	ZL_INT classid = 0;
	ZL_INT start_scan_num = 0;
	ZL_BOOL initScan = ZL_TRUE;*/

	if(compile->AST_TreeScanStackList.count < 0)
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_AST_TREE_SCAN_STACK_INVALID_COUNT);
	else
		stackVal->start_scan_num = compile->AST_TreeScanStackList.count; //��obj.key[i % obj.keylen]�����ı��ʽ����ɨ��obj.key����Ԫ��ʱ����ݹ��ȥɨ��obj.keylen������AST_TreeScanStackList.count��һ��Ϊ0�����Դ���0
	if(stackVal->nodenum == -1)
	{
		//return;
		goto end;
	}
	compile->push_AST_TreeScanStack(VM_ARG,stackVal->nodenum); //�Ƚ��ڵ�ѹ��ջ���γ�ɨ��ڵ�·������ʼ��
	do{
		stackVal->tmpstack = compile->pop_AST_TreeScanStack(VM_ARG,ZL_FALSE); //����ǰ��ѹ��ջ�Ľڵ���Ϣ������ZL_FALSE��ʾֻ������Ϣ���ݲ����ڵ�Ӷ�ջ��ɾ��
		if(stackVal->tmpstack.nodenum < 0) //��-1֮��Ŀսڵ�������
		{
			compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE);
			continue;
		}
		if(stackVal->tmpstack.curchild == 0) //curchildΪ0ʱ����ʾ��û��ʼɨ���ӽڵ㣬�ͶԵ�ǰ�ڵ���д���
		{
			stackVal->nodenum = stackVal->tmpstack.nodenum;
			switch(nodes[stackVal->nodenum].toktype)
			{
			case ZL_TK_ID:
			case ZL_TK_ARRAY_ITEM:
				if(stackVal->classid == 0) //��һ�β���ʱ����test.test2[3].name.val��ɨ��testʱclassid��Ϊ0
				{
					if(stackVal->initScan != ZL_TRUE)
					{
						compile->parser_curnode = stackVal->nodenum;
						compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYM_SCAN_DOT_FOR_CLASS_ID_ZERO_WHEN_NOT_INIT);
					}
					else
						stackVal->initScan = ZL_FALSE;
					stackVal->classid = compile->SymLookupID_ForDot(VM_ARG,stackVal->nodenum); //�ҵ��������id
					if(stackVal->classid == 0)
					{
						compile->parser_curnode = stackVal->nodenum;
						compile->parser_errorExit(VM_ARG,ZL_ERR_SYNTAX_SYM_CAN_NOT_FIND_CLASS_INFO_FOR_ID);
					}
					if(nodes[stackVal->nodenum].toktype == ZL_TK_ARRAY_ITEM)
					{
						ZL_INT * chnum = nodes[stackVal->nodenum].childs.childnum;
						if(nodes[chnum[0]].tokcategory == ZL_TKCG_OP_FACTOR || 
							ZENGL_AST_ISTOKEXPRESS(chnum[0]))
						{
							compile->AsmGCStackPush(VM_ARG,(ZL_INT)ZL_ASM_AI_OP_NONE,ZL_ASM_STACK_ENUM_ARRAY_ITEM_OP_TYPE);
							//compile->AsmGenCodes(VM_ARG,chnum[0]); // TODO
							zengl_AsmGCLoopStackPush(VM_ARG, chnum[0], ZL_ST_START); // ��chnum[0]ѹ��ջ�����غ���chnum[0]��Ӧ�Ľڵ�ִ�����ɻ��ָ��Ĳ���
							stackVal->status = ZENGL_ASMGC_SCAN_DOT_FOR_CLASS_STATUS_FIRST_PLACE; return ZL_ST_START;
first_place:
							compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_ARRAY_ITEM_OP_TYPE,ZL_TRUE);
						}
						else if(chnum[0] != -1)
						{
							compile->exit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_TOKEN,
								nodes[chnum[0]].line_no,
								nodes[chnum[0]].col_no,
								nodes[chnum[0]].filename,
								compile->getTokenStr(VM_ARG,nodes,chnum[0]));
						}
						compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE);
						continue;
					}
				} //if(classid == 0)
				else
				{
					ZL_INT classMemberIndex = compile->SymLookupClassMember(VM_ARG,stackVal->nodenum,stackVal->classid);
					run->AddInst(VM_ARG,compile->gencode_struct.pc++,stackVal->nodenum,
						ZL_R_IT_PUSH,ZL_R_DT_NONE,0,
						ZL_R_DT_NUM,compile->SymClassMemberTable.members[classMemberIndex].index); //��Ӧ���ָ�� ���� "PUSH %d" ȡ�����Ա����������ѹ��ջ
					//classid = compile->SymClassMemberTable.members[classMemberIndex].classid; //ʹ�ø����Ա����id��Ϊ��ȡ����ĳ�Ա��������׼��
					stackVal->classid = compile->SymClassMemberTable.members[classMemberIndex].cls_stmt_classid; //Ӧ��ʹ�������ó�Աʱ��ʹ�õ���ID��Ϣ�����Ǹó�Ա���ڵ���ṹ����Ϊ�ó�Ա�ĳ�Ա����������ĳ�Ա��
					if(nodes[stackVal->nodenum].toktype == ZL_TK_ARRAY_ITEM) //��������Ա��һ������Ԫ����������test.test2[3].name.val��test2����һ�����飬�������������3Ҳѹ��ջ���ò�����zengl_AsmGCLoopStackPush�����غ󣬻��ɻ�������ѭ���Զ����
					{
						ZL_INT * chnum = nodes[stackVal->nodenum].childs.childnum;
						if(nodes[chnum[0]].tokcategory == ZL_TKCG_OP_FACTOR || 
							ZENGL_AST_ISTOKEXPRESS(chnum[0]))
						{
							compile->AsmGCStackPush(VM_ARG,(ZL_INT)ZL_ASM_AI_OP_NONE,ZL_ASM_STACK_ENUM_ARRAY_ITEM_OP_TYPE);
							//compile->AsmGenCodes(VM_ARG,chnum[0]); // TODO
							zengl_AsmGCLoopStackPush(VM_ARG, chnum[0], ZL_ST_START); // ��chnum[0]ѹ��ջ�����غ���chnum[0]��Ӧ�Ľڵ�ִ�����ɻ��ָ��Ĳ���
							stackVal->status = ZENGL_ASMGC_SCAN_DOT_FOR_CLASS_STATUS_SECOND_PLACE; return ZL_ST_START;
second_place:
							compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_ARRAY_ITEM_OP_TYPE,ZL_TRUE);
						}
						else if(chnum[0] != -1)
						{
							compile->exit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_TOKEN,
								nodes[chnum[0]].line_no,
								nodes[chnum[0]].col_no,
								nodes[chnum[0]].filename,
								compile->getTokenStr(VM_ARG,nodes,chnum[0]));
						}
						compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE);
						continue;
					}
				}
				break; //case ZL_TK_ARRAY_ITEM: case ZL_TK_ID:
			case ZL_TK_DOT:
				break;
			default:
				compile->exit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_TOKEN,
							nodes[stackVal->nodenum].line_no,
							nodes[stackVal->nodenum].col_no,
							nodes[stackVal->nodenum].filename,
							compile->getTokenStr(VM_ARG,nodes,stackVal->nodenum));
				break;
			} //switch(nodes[nodenum].toktype)
		} //if(tmpstack.curchild == 0)
		if(stackVal->tmpstack.curchild < stackVal->tmpstack.childcnt) //���curchildС��childcnt�ӽڵ�������˵���ӽڵ�û�����꣬����������ӽڵ���Ϣ��
		{
			if(stackVal->tmpstack.curchild < ZL_AST_CHILD_NODE_SIZE) //����curchild�����ж��ǻ����ӽڵ㻹����չ�ӽڵ㡣
				stackVal->nodenum = nodes[stackVal->tmpstack.nodenum].childs.childnum[stackVal->tmpstack.curchild];
			else
				stackVal->nodenum = nodes[stackVal->tmpstack.nodenum].childs.extchilds[stackVal->tmpstack.curchild -
																   ZL_AST_CHILD_NODE_SIZE];
			compile->AST_TreeScanStackList.stacks[compile->AST_TreeScanStackList.count - 1].curchild++; //��curchild��һ���´ξͻᴦ����һ���ӽڵ���Ϣ��
			compile->push_AST_TreeScanStack(VM_ARG,stackVal->nodenum); //���ӽڵ�ѹ��ջ
			continue; //continue�������do...while��ͷ��Ȼ��pop_TreeStack���Ӷ�����ǰ��ѹ��ջ���ӽڵ���Ϣ���д���
		}
		else
			compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE); //���û���ӽڵ�����ӽڵ�Ҳ��ɨ���꣬�򵯳���ǰ�ڵ㡣
	}while(compile->AST_TreeScanStackList.count > stackVal->start_scan_num); //�����ջ�л���Ԫ�أ�˵�����нڵ�û�����ֻ꣬�е���ջ���Ԫ�ظ���Ϊstart_scan_numʱ���ʾ���е�ǰAST��Ľڵ㶼�������ˣ��Ϳ�������ѭ��������
end:
	zengl_AsmGCLoopStackValsPop(VM_ARG,loopStackTop);
	return zengl_AsmGCLoopStackFinishTopSimple(VM_ARG);
}

/*
	ʹ��ASTɨ���ջ��ɨ���﷨����use�ؼ���������ģ����
*/
ZL_VOID zengl_SymScanUseRsv(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT use_nodenum)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_AST_SCAN_STACK_TYPE tmpstack;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_RUN_INST_OP_DATA inst_op_data;
	if(compile->AST_TreeScanStackList.count != 0)
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_AST_TREE_SCAN_STACK_NOT_EMPTY);
	if(nodenum == -1)
	{
		compile->parser_curnode = use_nodenum;
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_SYM_USE_MUST_HAVE_CHILD_NODE);
	}
	compile->push_AST_TreeScanStack(VM_ARG,nodenum); //�Ƚ��ڵ�ѹ��ջ���γ�ɨ��ڵ�·������ʼ�㡣
	do{
		tmpstack = compile->pop_AST_TreeScanStack(VM_ARG,ZL_FALSE); //����ǰ��ѹ��ջ�Ľڵ���Ϣ������ZL_FALSE��ʾֻ������Ϣ���ݲ����ڵ�Ӷ�ջ��ɾ����
		if(tmpstack.curchild == 0) //curchildΪ0ʱ����ʾ��û��ʼɨ���ӽڵ㣬�ͶԵ�ǰ�ڵ���д���
		{
			nodenum = tmpstack.nodenum;
			switch(nodes[nodenum].toktype)
			{
			case ZL_TK_ID:
				inst_op_data.val.num = (ZL_LONG)compile->TokenStringPoolGetPtr(VM_ARG,nodes[nodenum].strindex);
				run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
					ZL_R_IT_USE,ZL_R_DT_NONE,0,
					ZL_R_DT_STR,inst_op_data.val.num); //��Ӧ���ָ�� "USE %s"
				break;
			case ZL_TK_COMMA:
				break;
			default:
				compile->exit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_TOKEN,
					nodes[nodenum].line_no,
					nodes[nodenum].col_no,
					nodes[nodenum].filename,
					compile->getTokenStr(VM_ARG,nodes,nodenum));
				break;
			}
		}
		if(tmpstack.curchild < tmpstack.childcnt) //���curchildС��childcnt�ӽڵ�������˵���ӽڵ�û�����꣬����������ӽڵ���Ϣ��
		{
			if(tmpstack.curchild < ZL_AST_CHILD_NODE_SIZE) //����curchild�����ж��ǻ����ӽڵ㻹����չ�ӽڵ㡣
				nodenum = nodes[tmpstack.nodenum].childs.childnum[tmpstack.curchild];
			else
				nodenum = nodes[tmpstack.nodenum].childs.extchilds[tmpstack.curchild -
																   ZL_AST_CHILD_NODE_SIZE];
			compile->AST_TreeScanStackList.stacks[compile->AST_TreeScanStackList.count - 1].curchild++; //��curchild��һ���´ξͻᴦ����һ���ӽڵ���Ϣ��
			compile->push_AST_TreeScanStack(VM_ARG,nodenum); //���ӽڵ�ѹ��ջ
			continue; //continue�������do...while��ͷ��Ȼ��pop_TreeStack���Ӷ�����ǰ��ѹ��ջ���ӽڵ���Ϣ���д���
		}
		else
			compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE); //���û���ӽڵ�����ӽڵ�Ҳ��ɨ���꣬�򵯳���ǰ�ڵ㡣
	}while(compile->AST_TreeScanStackList.count > 0); //ɨ���ջ�ļ�����ֵ����0��ʼ����ֵʱ����˵��ɨ�账����ϲ�����ѭ����
}

/*
	ʹ��ASTɨ���ջ��ɨ���﷨���к����ľֲ�����
*/
ZL_VOID zengl_SymScanFunLocal(ZL_VOID * VM_ARG,ZL_INT nodenum)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_AST_SCAN_STACK_TYPE tmpstack;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZL_INT * chnum,childcnt;
	if(compile->AST_TreeScanStackList.count != 0)
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_AST_TREE_SCAN_STACK_NOT_EMPTY);
	if(nodenum == -1) //����������ǿյ��򷵻�
		return;
	compile->push_AST_TreeScanStack(VM_ARG,nodenum); //�Ƚ��ڵ�ѹ��ջ���γ�ɨ��ڵ�·������ʼ�㡣
	do{
		tmpstack = compile->pop_AST_TreeScanStack(VM_ARG,ZL_FALSE); //����ǰ��ѹ��ջ�Ľڵ���Ϣ������ZL_FALSE��ʾֻ������Ϣ���ݲ����ڵ�Ӷ�ջ��ɾ����
		if(tmpstack.nodenum < 0) //��-1֮��Ŀսڵ�������
		{
			compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE);
			continue;
		}
		if(tmpstack.curchild == 0) //curchildΪ0ʱ����ʾ��û��ʼɨ���ӽڵ㣬�ͶԵ�ǰ�ڵ���д���
		{
			nodenum = tmpstack.nodenum;
			switch(nodes[nodenum].toktype)
			{
			case ZL_TK_ID:
			case ZL_TK_ARRAY_ITEM:
				if(nodes[nodenum].parentnode == -1 || nodes[nodes[nodenum].parentnode].toktype != ZL_TK_DOT) //����������Ԫ�أ��Ҳ��������Ա����ʽ���ֵ�ʱ�򣬾Ϳ��Լ���ֲ����ű�
				{
					if(compile->SymInsertHashTableForLocal(VM_ARG,nodenum,ZL_SYM_ENUM_LOCAL_TYPE_LOCAL) == ZL_TRUE) //����ǽű������ľֲ��������ͽ�����뵽SymLocalTable�ֲ��������ű�̬�����У��������ʾΪZL_SYM_ENUM_LOCAL_TYPE_LOCAL
					{
						run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
								ZL_R_IT_PUSH_LOC , ZL_R_DT_NONE , 0,
								ZL_R_DT_NONE , 0); //��Ӧ���ָ�� "PUSH_LOC" ɨ�赽�ľֲ����������һ��PUSH_LOCָ��Ӷ������������ʱ�������ջ�з���һ��ջ�ռ䡣��������ʱ������ΪNONEδ��ʼ��״̬
						compile->gencode_struct.localID++;
					}
				}
				break;
			case ZL_TK_RESERVE:
				if(nodes[nodenum].reserve == ZL_RSV_GLOBAL)
				{
					childcnt = nodes[nodenum].childs.count;
					chnum = nodes[nodenum].childs.childnum;
					if(childcnt == 1)
					{
						compile->SymScanFunGlobal(VM_ARG,chnum[0],nodenum); //�����global�ؼ��֣��ͽ�global�����ȫ�ֱ���ͨ��SymScanFunGlobalɨ�赽SymLocalTable�С�
						compile->AST_TreeScanStackList.stacks[compile->AST_TreeScanStackList.count - 1].curchild++;
					}
					else
					{
						compile->parser_curnode = nodenum;
						compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_ONE_CHILD);
					}
				}
				break;
			}
		}
		if(tmpstack.curchild < tmpstack.childcnt) //���curchildС��childcnt�ӽڵ�������˵���ӽڵ�û�����꣬����������ӽڵ���Ϣ��
		{
			if(tmpstack.curchild < ZL_AST_CHILD_NODE_SIZE) //����curchild�����ж��ǻ����ӽڵ㻹����չ�ӽڵ㡣
				nodenum = nodes[tmpstack.nodenum].childs.childnum[tmpstack.curchild];
			else
				nodenum = nodes[tmpstack.nodenum].childs.extchilds[tmpstack.curchild -
																   ZL_AST_CHILD_NODE_SIZE];
			compile->AST_TreeScanStackList.stacks[compile->AST_TreeScanStackList.count - 1].curchild++; //��curchild��һ���´ξͻᴦ����һ���ӽڵ���Ϣ��
			compile->push_AST_TreeScanStack(VM_ARG,nodenum); //���ӽڵ�ѹ��ջ
			continue; //continue�������do...while��ͷ��Ȼ��pop_TreeStack���Ӷ�����ǰ��ѹ��ջ���ӽڵ���Ϣ���д���
		}
		else if(tmpstack.next != 0) //��next�ֵܽڵ����ɨ��
		{
			compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE);
			compile->push_AST_TreeScanStack(VM_ARG,tmpstack.next);
			continue;
		}
		else
			compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE); //���û���ӽڵ�����ӽڵ�Ҳ��ɨ���꣬�򵯳���ǰ�ڵ㡣
	}while(compile->AST_TreeScanStackList.count > 0); //�����ջ�л���Ԫ�أ�˵�����нڵ�û�����ֻ꣬�е���ջ���Ԫ�ظ���Ϊ0ʱ���ʾ����AST��Ľڵ㶼�������ˣ��Ϳ�������ѭ�������ˡ�
}

/*
	���ֲ������Ͳ������뵽�ֲ��������ű��У��������ű�̬������������뵽��ϣ����
*/
ZL_BOOL zengl_SymInsertHashTableForLocal(ZL_VOID * VM_ARG,ZL_INT nodenum,ZENGL_SYM_ENUM_LOCAL_TYPE type)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_AST_NODE_TYPE * node = nodes + nodenum;
	ZL_CHAR * name = compile->TokenStringPoolGetPtr(VM_ARG,nodes[nodenum].strindex);
	ZL_INT h = compile->hash(VM_ARG,name) + ZL_SYM_HASH_SIZE * ZL_HASH_TYPE_LOCAL_TABLE;
	ZL_INT tmpindex = compile->HashTable[h];

	while(tmpindex != 0 && compile->SymLocalTable.local[tmpindex].isvalid == ZL_TRUE && 
		(ZENGL_SYS_STRCMP(name,compile->TokenStringPoolGetPtr(VM_ARG,compile->SymLocalTable.local[tmpindex].nameIndex)) != 0 || 
		compile->SymLocalTable.local[tmpindex].funid != compile->SymFunTable.global_funid))
	{
		tmpindex = compile->SymLocalTable.local[tmpindex].next;
	}
	if(tmpindex == 0)
	{
		tmpindex = compile->HashTable[h];
		compile->HashTable[h] = compile->SymInsertLocalTable(VM_ARG,nodes[nodenum].strindex,type);
		compile->SymLocalTable.local[compile->HashTable[h]].next = tmpindex;
		tmpindex = compile->HashTable[h];
		return ZL_TRUE;
	}
	else if(compile->SymLocalTable.local[tmpindex].isvalid == ZL_FALSE)
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_LOCAL_TABLE_INVALID_INDEX);
	return ZL_FALSE;
}

/*
	���ֲ��������;ֲ����������Ͳ��뵽SymLocalTable��̬�����С�
*/
ZL_INT zengl_SymInsertLocalTable(ZL_VOID * VM_ARG,ZL_INT nameIndex,ZENGL_SYM_ENUM_LOCAL_TYPE type)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZL_INT tmpindex;
	if(!compile->SymLocalTable.isInit)
		compile->SymInitLocalTable(VM_ARG);
	if(compile->SymLocalTable.count == compile->SymLocalTable.size)
	{
		compile->SymLocalTable.size += ZL_SYM_LOCAL_TABLE_SIZE;
		compile->SymLocalTable.local = compile->memReAlloc(VM_ARG,compile->SymLocalTable.local,
			compile->SymLocalTable.size * sizeof(ZENGL_SYM_LOCAL_TABLE_MEMBER));
		ZENGL_SYS_MEM_SET(compile->SymLocalTable.local + (compile->SymLocalTable.size - ZL_SYM_LOCAL_TABLE_SIZE),0,
			ZL_SYM_LOCAL_TABLE_SIZE * sizeof(ZENGL_SYM_LOCAL_TABLE_MEMBER));
	}
	tmpindex = compile->SymLocalTable.count;
	if(compile->SymLocalTable.local[tmpindex].isvalid == ZL_FALSE)
	{
		compile->SymLocalTable.local[tmpindex].nameIndex = nameIndex;
		compile->SymLocalTable.local[tmpindex].funid = compile->SymFunTable.global_funid;
		compile->SymLocalTable.local[tmpindex].localid = compile->gencode_struct.localID;
		compile->SymLocalTable.local[tmpindex].type = type;
		compile->SymLocalTable.local[tmpindex].next = 0;
		compile->SymLocalTable.local[tmpindex].isvalid = ZL_TRUE;
		compile->SymLocalTable.count++;
		return tmpindex;
	}
	else
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_LOCAL_TABLE_CAN_NOT_FIND_NOT_VALID_INDEX_WHEN_INSERT);
	return -1;
}

/*
	��ʼ��SymLocalTable�ֲ����ű��Ӧ�Ķ�̬����
*/
ZL_VOID zengl_SymInitLocalTable(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	if(compile->SymLocalTable.isInit)
		return;
	compile->SymLocalTable.count = 1;
	compile->SymLocalTable.size = ZL_SYM_LOCAL_TABLE_SIZE;
	compile->SymLocalTable.local = compile->memAlloc(VM_ARG,compile->SymLocalTable.size * sizeof(ZENGL_SYM_LOCAL_TABLE_MEMBER));
	if(compile->SymLocalTable.local == ZL_NULL)
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_LOCAL_TABLE_MALLOC_FAILED);
	ZENGL_SYS_MEM_SET(compile->SymLocalTable.local,0,compile->SymLocalTable.size * sizeof(ZENGL_SYM_LOCAL_TABLE_MEMBER));
	compile->SymLocalTable.isInit = ZL_TRUE;
}

/**
 * ����token�����ж��Ƿ���self��token��self��token����ṹ�п������ڱ�ʾ��ǰ�����������
 */
ZL_BOOL zengl_SymIsSelfToken(ZL_VOID * VM_ARG, ZL_CHAR * token_name)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	if(token_name == ZL_NULL)
		token_name = compile->tokenInfo.str;
	if(ZENGL_SYS_STRCMP(token_name, ZL_SYM_SELF_TOKEN_STR) == 0)
	{
		return ZL_TRUE;
	}
	else
		return ZL_FALSE;
}

/**
 * ���ĳ����ʶ���ڵ���self�ڵ㣬�򽫸ýڵ��AST�ڵ�ż��뵽self�ڵ�����Ϣ��̬������
 * self�ڵ�����Ϣ��̬�����е�ÿ����Ա�У���������self�ڵ��AST�ڵ�ţ��Լ���self�ڵ����ڵ����class�ڵ��AST�ڵ�ţ��Ӷ���self�����ڵ��ཨ���˹�����
 * �Ժ�Ϳ��Ը������ֹ�����������Щself�ڵ��Ӧ����ID��Ϣ���Ӷ�ʵ����self����ʾ��������������﷨��
 */
ZL_BOOL zengl_SymAddNodeNumToSelfClassTable(ZL_VOID * VM_ARG, ZL_INT self_nodenum)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	if(compile->SymSelfClassTable.cur_class_nodenum < 0)
		return ZL_FALSE;
	if(!compile->SymSelfClassTable.isInit)
		zengl_static_SymInitSelfClassTable(VM_ARG);
	if(compile->SymSelfClassTable.count == compile->SymSelfClassTable.size)
	{
		compile->SymSelfClassTable.size += ZL_SYM_SELF_CLASS_TABLE_SIZE;
		compile->SymSelfClassTable.members = compile->memReAlloc(VM_ARG, compile->SymSelfClassTable.members,
				compile->SymSelfClassTable.size * sizeof(ZENGL_SYM_SELF_CLASS_TABLE_MEMBER));
		ZENGL_SYS_MEM_SET(compile->SymSelfClassTable.members + (compile->SymSelfClassTable.size - ZL_SYM_SELF_CLASS_TABLE_SIZE),0,
				ZL_SYM_SELF_CLASS_TABLE_SIZE * sizeof(ZENGL_SYM_SELF_CLASS_TABLE_MEMBER));
	}
	compile->SymSelfClassTable.members[compile->SymSelfClassTable.count].class_nodenum = compile->SymSelfClassTable.cur_class_nodenum;
	compile->SymSelfClassTable.members[compile->SymSelfClassTable.count].classid = 0;
	compile->SymSelfClassTable.members[compile->SymSelfClassTable.count].self_nodenum = self_nodenum;
	compile->SymSelfClassTable.members[compile->SymSelfClassTable.count].isvalid = ZL_TRUE;
	compile->SymSelfClassTable.count++;
	return ZL_TRUE;
}

/*
	��ӡ���ű������ű����ÿ���������ڴ��ַ�����������Լ���Щ������ʾ����Դ�ļ�����ֵ�λ�ô�ӡ��ʾ������
	�Լ����еĺ���������Ϣ��
*/
ZL_VOID zengl_SymPrintTables(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZL_INT i;
	ZL_INT linecol;
	ZL_CLOCK_T start_time = ZENGL_SYS_TIME_CLOCK();
	ZL_CLOCK_T end_time;

	for(i=1;i<compile->SymGlobalTable.count;i++)
	{
		if(compile->SymGlobalTable.sym[i].isvalid)
		{
			compile->info(VM_ARG,"id:%d %s",compile->SymGlobalTable.sym[i].memloc,
						compile->TokenStringPoolGetPtr(VM_ARG,compile->SymGlobalTable.sym[i].nameIndex));
			linecol = compile->SymGlobalTable.sym[i].linecols;
			if(linecol == 0)
				compile->exit(VM_ARG,ZL_ERR_CP_SYM_GLOBAL_TABLE_INDEX_HAVE_NO_LINECOL_WHEN_PRINT,i,i);
			while(linecol)
			{
				compile->info(VM_ARG," [%d %d %s]",compile->LineCols.lines[linecol].lineno,
							compile->LineCols.lines[linecol].colno,compile->LineCols.lines[linecol].filename);
				linecol = compile->LineCols.lines[linecol].next;
			}
			compile->info(VM_ARG,"\n");
		}
	}
	for(i=1;i<compile->SymClassTable.count;i++)
	{
		if(compile->SymClassTable.classTable[i].isvalid)
		{
			compile->info(VM_ARG,"class:%d %s <nodenum:%d>",compile->SymClassTable.classTable[i].classid,
						compile->TokenStringPoolGetPtr(VM_ARG,compile->SymClassTable.classTable[i].nameIndex),
						compile->SymClassTable.classTable[i].nodenum);
			linecol = compile->SymClassTable.classTable[i].linecols;
			if(linecol == 0)
				compile->exit(VM_ARG,ZL_ERR_CP_SYM_CLASS_TABLE_INDEX_HAVE_NO_LINECOL_WHEN_PRINT,i,i);
			while(linecol)
			{
				compile->info(VM_ARG," [%d %d %s]",compile->LineCols.lines[linecol].lineno,
							compile->LineCols.lines[linecol].colno,compile->LineCols.lines[linecol].filename);
				linecol = compile->LineCols.lines[linecol].next;
			}
			compile->info(VM_ARG,"\n");
		}
	}
	for(i=1;i<compile->SymClassMemberTable.count;i++)
	{
		if(compile->SymClassMemberTable.members[i].isvalid)
		{
			compile->info(VM_ARG,"classmember:%d %s",
				compile->SymClassMemberTable.members[i].index,
				compile->TokenStringPoolGetPtr(VM_ARG,compile->SymClassTable.classTable[compile->SymClassMemberTable.members[i].classid].nameIndex));
			if(compile->SymClassMemberTable.members[i].cls_stmt_classid > 0)
			{
				compile->info(VM_ARG," %s",
					compile->TokenStringPoolGetPtr(VM_ARG,compile->SymClassTable.classTable[compile->SymClassMemberTable.members[i].cls_stmt_classid].nameIndex)
					);
			}
			compile->info(VM_ARG," %s",compile->TokenStringPoolGetPtr(VM_ARG,compile->SymClassMemberTable.members[i].nameIndex));
			compile->info(VM_ARG," [%d %d %s]",
				compile->AST_nodes.nodes[compile->SymClassMemberTable.members[i].nodenum].line_no,
				compile->AST_nodes.nodes[compile->SymClassMemberTable.members[i].nodenum].col_no,
				compile->AST_nodes.nodes[compile->SymClassMemberTable.members[i].nodenum].filename
				);
			compile->info(VM_ARG,"\n");
		}
	}
	for(i=1;i<compile->SymFunTable.count;i++)
	{
		if(compile->SymFunTable.funs[i].isvalid)
		{
			if(compile->SymFunTable.funs[i].classid > 0)
				compile->info(VM_ARG,"classfun:<class:%d %s> ",compile->SymFunTable.funs[i].classid,
							compile->TokenStringPoolGetPtr(VM_ARG,compile->SymClassTable.classTable[compile->SymFunTable.funs[i].classid].nameIndex));
			compile->info(VM_ARG,"fun:%d %s",compile->SymFunTable.funs[i].funid,
							compile->TokenStringPoolGetPtr(VM_ARG,compile->SymFunTable.funs[i].nameIndex));
			linecol = compile->SymFunTable.funs[i].linecols;
			if(linecol == 0)
				compile->exit(VM_ARG,ZL_ERR_CP_SYM_FUN_TABLE_INDEX_HAVE_NO_LINECOL_WHEN_PRINT,i,i);
			while(linecol)
			{
				compile->info(VM_ARG," [%d %d %s]",compile->LineCols.lines[linecol].lineno,
							compile->LineCols.lines[linecol].colno,compile->LineCols.lines[linecol].filename);
				linecol = compile->LineCols.lines[linecol].next;
			}
			compile->info(VM_ARG,"\n");
		}
	}
	end_time = ZENGL_SYS_TIME_CLOCK();
	compile->total_print_time += end_time - start_time;
}
