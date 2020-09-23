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
	���ļ�Ϊ����������ṩ�ĺ����ӿ��ļ���
*/

#include "zengl_global.h"

ZENGL_VM_TYPE ZL_Api_Const_VM = 
	{
		{
			{0}, //tokenInfo
			{0}, //source��Ա
			{0}, //mempool
			{0}, //jumpBuffer��Ա
			1, //line_no
			0, //col_no
			ZL_FALSE, //is_inConvChr
			ZL_FALSE, //is_inDefRsv
			0, //basesize
			0, //totalsize
			0, //start_time
			0, //end_time
			0, //total_time
			0, //total_print_time
			{0}, //infoFullString
			{0}, //errorFullString
			ZL_Reserves_String, //reserveString
			ZL_Token_Operate_String, //TokenOperateString
			0, //TokenOperateStringCount
			{0}, //def_StringPool
			{0}, //def_table
			ZL_FALSE, //isinCompiling
			ZL_FALSE, //isDestroyed
			ZL_FALSE, //isReUse
			/*zengl_symbol.c���ű���صĳ�Ա*/
			{0}, //HashTable
			{0}, //LineCols
			{0}, //PathFileName
			{0}, //FileStackList
			{0}, //SymGlobalTable
			{0}, //SymLocalTable
			{0}, //SymClassTable
			{0}, //SymClassMemberTable
			{0}, //SymFunTable
			/*��zengl_assemble.c������������صĳ�Ա*/
			{0}, //gencode_struct
			{0}, //AsmGCStackList
			{0}, //AsmGCLoopStackList
			0,	 //AsmGCAddrNum
			ZL_FALSE, //AsmGCIsInClass
			{0}, //memDataForDot
			/*��zengl_ld.c���ӳ�����صĳ�Ա*/
			{0}, //LDAddrList
			/*zengl_parser.c�﷨������صĳ�Ա*/
			{0}, //AST_nodes
			{0}, //Token_StringPool
			-1, //parser_curnode
			{0}, //exp_struct
			{0}, //parser_opLevel_stackList
			{0}, //AST_TreeScanStackList
			zengl_exit,
			zengl_freeall,
			zengl_getNextchar,
			zengl_getToken,
			zengl_makeTokenInfoLineCol,
			zengl_makeTokenStr,
			zengl_malloc,
			zengl_realloc,
			zengl_freeonce,
			zengl_ungetchar,
			zengl_convertStrToDec,
			zengl_freeTokenStr,
			zengl_convertOctalToDec,
			zengl_convertHexToDec,
			zengl_makeTokenStrForString,
			zengl_lookupReserve,
			zengl_AddDefConst,
			zengl_DefPoolAddString,
			zengl_initDefStringPool,
			zengl_insert_HashTableForDef,
			zengl_DefPoolGetPtr,
			zengl_insert_DefTable,
			zengl_initDefTable,
			zengl_include_file,
			zengl_makePathFileName,
			zengl_push_FileStack,
			zengl_pop_FileStack,
			zengl_initFileStack,
			zengl_ReplaceDefConst,
			zengl_lookupDefTable,
			zengl_makeInfoString,
			zengl_freeInfoString,
			zengl_info,
			zengl_init,
			zengl_main,
			/*������zengl_symbol.c�е���غ���*/
			zengl_Sym_Hash,
			zengl_insert_LineCol,
			zengl_initLineColTable,
			zengl_buildSymTable,
			zengl_ScanInsertGlobalSym,
			zengl_SymInsertDotID,
			zengl_SymInsertHashTableForGlobal,
			zengl_SymInsertGlobalTable,
			zengl_SymInitGlobalTable,
			zengl_SymInsertHashTableForClass,
			zengl_SymInsertClassTable,
			zengl_SymInitClassTable,
			zengl_SymInsertHashTableForFun,
			zengl_SymInsertFunTable,
			zengl_SymInitFunTable,
			zengl_ScanInsertClassMemberSym,
			zengl_SymLookupClass,
			zengl_SymLookupClassByName,
			zengl_SymLookupClassMember,
			zengl_SymInsertHashTableForClassMember,
			zengl_SymInsertClassMemberTable,
			zengl_SymInitClassMemberTable,
			zengl_SymLookupID,
			zengl_SymInsertClassID,
			zengl_SymLookupID_ForDot,
			zengl_SymLookupID_ByName,
			zengl_SymLookupFun,
			zengl_SymLookupFunByName,
			zengl_SymScanFunArg,
			zengl_SymScanFunGlobal,
			zengl_SymScanClassStatement,
			zengl_SymScanDotForClass,
			zengl_SymScanUseRsv,
			zengl_SymScanFunLocal,
			zengl_SymInsertHashTableForLocal,
			zengl_SymInsertLocalTable,
			zengl_SymInitLocalTable,
			zengl_SymPrintTables,
			/*������zengl_assemble.c�е���غ���*/
			zengl_buildAsmCode,
			zengl_AsmGenCodes,
			zengl_AsmGCStackPush,
			zengl_AsmGCStackPop,
			zengl_AsmGCStackInit,
			//zengl_AsmGCElif,
			//zengl_AsmGCBreak_Codes,
			//zengl_AsmGCContinue_Codes,
			zengl_AsmScanCaseMinMax,
			zengl_GetNodeInt,
			zengl_AsmAddCaseJmpTable,
			zengl_AsmInitCaseJmpTable,
			zengl_AsmSortCaseJmpTable,
			/*������zengl_ld.c�е���غ���*/
			zengl_LDAddrListSet,
			zengl_LDAddrListInit,
			zengl_LDAddrListReplaceAll,
			/*������zengl_parser.c�е���غ���*/
			zengl_ASTAddNode,
			zengl_initAST,
			zengl_TokenStringPoolAddStr,
			zengl_initTokenStringPool,
			zengl_printNode,
			zengl_getTokenStr,
			zengl_TokenStringPoolGetPtr,
			zengl_printASTnodes,
			zengl_push_AST_TreeScanStack,
			zengl_pop_AST_TreeScanStack,
			zengl_init_AST_TreeScanStack,
			zengl_buildAST,
			zengl_statement,
			zengl_KeywordCompleteDetect,
			zengl_TokenSyntaxDetect,
			zengl_if_stmt,
			zengl_if_stmt_sequence,
			zengl_for_stmt,
			zengl_for_stmt_sequence,
			zengl_fun_stmt,
			zengl_fun_stmt_sequence,
			zengl_switch_stmt,
			zengl_switch_stmt_sequence,
			zengl_class_stmt,
			zengl_class_stmt_sequence,
			zengl_while_stmt,
			zengl_while_stmt_sequence,
			zengl_do_stmt,
			zengl_do_stmt_sequence,
			zengl_detectSetRBracketToSemi,
			zengl_express,
			zengl_parser_addcurnode,
			zengl_parser_errorExit,
			zengl_opLevel_push_stack,
			zengl_opLevel_pop_stack,
			zengl_initOpLevelStackList,
			zengl_detectCurnodeSyntax,
			zengl_OpLevelForTwo,
			zengl_OpLevelForAssign,
			zengl_OpLevelForSEMI,
			zengl_OpLevelForPPMM,
			zengl_PPMM_GET_POS,
			zengl_OpLevelForRBRACKET,
			zengl_OpLevelForRMBRACKET,
			zengl_OpLevelForQuestion,
			zengl_OpLevelForColon,
			zengl_CheckQstColonValid,
			zengl_ASTAddNodeChild,
			zengl_CheckIsNegative,
			zengl_CheckIsBitAnd,
			/*�������û��Զ���ĺ���*/
			ZL_NULL, //userdef_info
			ZL_NULL  //userdef_compile_error
		}, //��ʼ��������ı�����

		{
			0, //basesize
				0, //totalsize
				0, //start_time
				0, //end_time
				0, //total_time
				ZL_FALSE, //isinRunning
				ZL_FALSE, //isDestroyed
				ZL_FALSE, //isUserWantStop
				ZL_FALSE, //is_inMemBlkSetVal
			{0}, //jumpBuffer
			{0}, //mempool
			{0}, //memfreepool
			{0}, //inst_list
			{0}, //vmem_list
			{0}, //vstack_list
			{0}, //reg_list
			{0}, //infoFullString
			{0}, //printFullString
			{0}, //errorFullString
			ZL_Run_Inst_Type_String, //instTypeString
			ZL_Run_Register_Type_String, //registerTypeString
			{0}, //InstData_StringPool
			{0}, //HashTable
			{0}, //moduleTable
			{0}, //ModFunTable
			{0}, //ExtraDataTable
			0,	 //CurRunModFunIndex
			/*������zenglrun_func.c�е���غ���*/
			zenglrun_init,
			zenglrun_memAlloc,
			zenglrun_memReAlloc,
			zenglrun_memFindPtrIndex,
			zenglrun_memReUsePtr,
			zenglrun_exit,
			zenglrun_exit_forApiSetErrThenStop,
			zenglrun_ExitPrintSourceInfo,
			zenglrun_memFreeAll,
			zenglrun_SetApiErrorEx,
			zenglrun_SetApiError,
			zenglrun_makeInfoString,
			zenglrun_freeInfoString,
			zenglrun_info,
			zenglrun_print,
			zenglrun_InstDataStringPoolAdd,
			zenglrun_initInstDataStringPool,
			zenglrun_InstDataStringPoolGetPtr,
			zenglrun_strcat,
			zenglrun_memFreeIndex,
			zenglrun_memFreeOnce,
			zenglrun_strcat2,
			zenglrun_Hash,
			zenglrun_InsertModuleTable,
			zenglrun_initModuleTable,
			zenglrun_InsertModFunTable,
			zenglrun_initModFunTable,
			zenglrun_LookupModuleTable,
			zenglrun_LookupModFunTable,
			zenglrun_initExtraDataTable,
			zenglrun_InsertExtraDataTable,
			/*������zenglrun_main.c�е���غ���*/
			zenglrun_AddInst,
			zenglrun_initInstList,
			zenglrun_VMemListOps,
			zenglrun_initVMemList,
			zenglrun_VStackListOps,
			zenglrun_initVStackList,
			zenglrun_printInstList,
			zenglrun_RunInsts,
			zenglrun_RegAssignStr,
			zenglrun_op_minis,
			zenglrun_op_je,
			zenglrun_op_jne,
			zenglrun_op_bits,
			zenglrun_op_relation,
			zenglrun_op_logic,
			zenglrun_op_addminisget,
			zenglrun_op_getaddminis,
			zenglrun_op_addminisone,
			zenglrun_op_addr,
			zenglrun_op_set_array,
			zenglrun_alloc_memblock,
			//zenglrun_memblock_getindex,
			zenglrun_memblock_getindex_ext,
			zenglrun_realloc_memblock,
			zenglrun_assign_memblock,
			zenglrun_memblock_setval,
			zenglrun_VMemBlockOps,
			zenglrun_memblock_free,
			zenglrun_op_get_array,
			zenglrun_op_get_array_addr,
			zenglrun_op_addminis_one_array,
			zenglrun_memblock_freeall_local,
			zenglrun_FreeAllForReUse,
			zenglrun_op_switch,
			zenglrun_getRegInt,
			zenglrun_main,
			/*�������û��Զ���ĺ���*/
			ZL_NULL, //userdef_run_info
			ZL_NULL, //userdef_run_print
			ZL_NULL  //userdef_run_error
			}, //��ʼ��������Ľ�����

			{
				ZL_NULL, //DeubugVM
				ZL_NULL, //DeubugPVM
				0,		 //orig_run_totalsize
				0,		 //orig_vm_totalsize
				0,		 //api_call_pc
				0,		 //api_call_arg
				ZL_FALSE,//break_start
				ZL_FALSE,//output_debug_info
				0,		 //flag
				{0},	 //BreakPoint
				{0},	 //singleBreak
				/*������zenglDebug.c�е���غ���*/
				zenglDebug_Compile,
				zenglDebug_Run,
				zenglDebug_ReplaceDefConst,
				zenglDebug_lookupDefTable,
				zenglDebug_SymLookupID,
				zenglDebug_SymLookupID_ForDot,
				zenglDebug_SymLookupClass,
				zenglDebug_SymLookupClassMember,
				zenglDebug_LookupModFunTable,
				zenglDebug_LookupFunID,
				zenglDebug_SetFunInfo,
				zenglDebug_GetTraceInfo,
				zenglDebug_InitBreak,
				zenglDebug_SetBreak,
				zenglDebug_BreakStart,
				zenglDebug_DelBreak,
				zenglDebug_RestoreBreaks,
				zenglDebug_CheckCondition,
				zenglDebug_RestoreSingleBreak,
				zenglDebug_ChkAndSetSingleBreak,
				/*������Ҫ���õ��û��Զ��庯��*/
				ZL_NULL, //userdef_debug_break
				ZL_NULL  //userdef_debug_conditionError
			}, //��ʼ��������ĵ�����

			ZL_NO_ERR_SUCCESS,
			ZL_Error_String,
			ZL_FALSE, //isCompileError
			ZL_FALSE, //isRunError
			ZL_VM_SIGNER, //signer
			0, //basesize
			0, //totalsize
			0, //start_time
			0, //end_time
			0, //total_time
			ZL_NULL, //vm_main_args
			ZL_FALSE,//isinApiRun
			ZL_FALSE,//isUseApiSetErrThenStop
			{0}, //initEncrypt
			ZL_API_ST_NONE, //ApiState
			ZL_NULL, //ApiError
			/*�������صĺ���*/
			zenglVM_rc4InitState,
			zenglVM_init
	}; //�������ʼ��ģ��

/*ͨ��zenglApi_Load���Լ��ز�ִ��script_file�ű�*/
ZL_EXPORT ZL_EXP_INT zenglApi_Load(ZL_EXP_CHAR * script_file,ZENGL_EXPORT_VM_MAIN_ARGS * vm_main_args)
{
	ZENGL_VM_TYPE VM = ZL_Api_Const_VM;
	ZENGL_SYS_ARG_LIST arg;
	ZL_INT retcode;

	ZENGL_SYS_ARG_END(arg); //��va_end����arg��Ϊ0������64λgcc�±���
	VM.init((ZL_VOID * )&VM,vm_main_args);
	VM.run.init((ZL_VOID * )&VM); //����������Ҫ�Խ�����������ָ������ڴ˳�ʼ��������
	if(script_file == ZL_NULL)
	{
		VM.errorno = ZL_ERR_VM_API_INVALID_SCRIPT_NAME;
		VM.run.makeInfoString((ZL_VOID * )&VM,&VM.run.errorFullString , VM.errorString[VM.errorno] , arg);
		if(VM.run.userdef_run_error != ZL_NULL)
			VM.run.userdef_run_error(VM.run.errorFullString.str,VM.run.errorFullString.count,(ZL_VOID * )&VM);
		VM.isRunError = ZL_TRUE;
		return -1;
	}
	retcode = VM.compile.main((ZL_VOID * )&VM,script_file,vm_main_args);
	if(retcode == 0) //�������ɹ�������������
	{
		retcode = VM.run.main((ZL_VOID * )&VM);
	}
	else
	{
		if(VM.compile.isDestroyed == ZL_FALSE) //�������������ʱ���ڵ���ģʽ�£�����Ҫ�ڴ��ͷ���Դ
			VM.compile.memFreeAll((ZL_VOID * )&VM);
		VM.run.memFreeAll((ZL_VOID * )&VM);
	}
	VM.end_time = ZENGL_SYS_TIME_CLOCK();
	VM.total_time = VM.end_time - VM.start_time; //�õ�������ܵ�ִ��ʱ��
	return retcode;
}

/*�����������µ������ָ�룬��ָ�����ͨ��zenglApi_Close�����ͷţ�����ᷢ���ڴ�й©*/
ZL_EXPORT ZL_EXP_VOID * zenglApi_Open()
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)ZENGL_SYS_MEM_ALLOC(sizeof(ZENGL_VM_TYPE));
	ZENGL_EXPORT_VM_MAIN_ARGS * vm_main_args = (ZENGL_EXPORT_VM_MAIN_ARGS *)ZENGL_SYS_MEM_ALLOC(sizeof(ZENGL_EXPORT_VM_MAIN_ARGS));
	(*VM) = ZL_Api_Const_VM;
	ZENGL_SYS_MEM_SET(vm_main_args,0,sizeof(ZENGL_EXPORT_VM_MAIN_ARGS));
	VM->init((ZL_VOID * )VM,vm_main_args);
	VM->ApiState = ZL_API_ST_OPEN;
	return (ZL_EXP_VOID *)VM;
}

/*ʹ��zenglApi_Open�򿪵������������script_file��Ӧ�Ľű�*/
ZL_EXPORT ZL_EXP_INT zenglApi_Run(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * script_file)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_SYS_ARG_LIST arg;
	ZL_INT retcode;
	ZL_CHAR * ApiName = "zenglApi_Run";
	ZENGL_SYS_ARG_END(arg); //��va_end����arg��Ϊ0������64λgcc�±���
	if(VM->signer != ZL_VM_SIGNER)
		return -1;
	switch(VM->ApiState)
	{
	case ZL_API_ST_OPEN:
	case ZL_API_ST_RESET:
	case ZL_API_ST_REUSE:
		break;
	default:
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_CALL_POSITION, ApiName , ApiName);
		return -1;
		break;
	}
	if(VM->isinApiRun == ZL_TRUE && VM->compile.isReUse == ZL_FALSE)
	{
		VM->run.SetApiError(VM_ARG,ZL_ERR_VM_API_VM_MUST_RESET,arg);
		return -1;
	}
	VM->isinApiRun = ZL_TRUE;
	VM->ApiState = ZL_API_ST_RUN; //����ΪRUN״̬
	if(VM->run.mempool.isInit == ZL_FALSE)
		VM->run.init(VM_ARG); //����������Ҫ�Խ�����������ָ������ڴ˳�ʼ��������
	if(script_file == ZL_NULL)
	{
		VM->run.SetApiError(VM_ARG,ZL_ERR_VM_API_INVALID_SCRIPT_NAME,arg);
		return -1;
	}
	retcode = VM->compile.main(VM_ARG,script_file,VM->vm_main_args);
	if(retcode == 0) //�������ɹ�������������
	{
		retcode = VM->run.main(VM_ARG);
	}
	VM->end_time = ZENGL_SYS_TIME_CLOCK();
	VM->total_time = VM->end_time - VM->start_time; //�õ�������ܵ�ִ��ʱ��
	VM->ApiState = ZL_API_ST_AFTER_RUN; //����ΪAFTER_RUN״̬
	if(VM->compile.isReUse == ZL_TRUE)
		VM->compile.isReUse = ZL_FALSE;
	return retcode;
}

/*
ʹ��zenglApi_Open�򿪵������������script_strָ����ַ����ű���lenΪ�ַ����ű�����Ч���ȣ�script_nameΪ���ַ����ű����Զ������֣�
script_name�������ڵ�����Ϣ�����ͬʱ�����Ը�script_name����һ��·��ǰ׺������"script/runstr"�������ַ����ű��ڲ�inc���������ű�ʱ�ͻᰴ��"script/"·��������
*/
ZL_EXPORT ZL_EXP_INT zenglApi_RunStr(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * script_str,ZL_EXP_INT len,ZL_EXP_CHAR * script_name)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_SYS_ARG_LIST arg;
	ZL_INT retcode;
	ZL_CHAR * ApiName = "zenglApi_RunStr";
	ZENGL_SYS_ARG_END(arg); //��va_end����arg��Ϊ0������64λgcc�±���
	if(VM->signer != ZL_VM_SIGNER)
		return -1;
	switch(VM->ApiState)
	{
	case ZL_API_ST_OPEN:
	case ZL_API_ST_RESET:
	case ZL_API_ST_REUSE:
		break;
	default:
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_CALL_POSITION, ApiName , ApiName);
		return -1;
		break;
	}
	if(VM->isinApiRun == ZL_TRUE && VM->compile.isReUse == ZL_FALSE)
	{
		VM->run.SetApiError(VM_ARG,ZL_ERR_VM_API_VM_MUST_RESET,arg);
		return -1;
	}
	VM->isinApiRun = ZL_TRUE;
	VM->ApiState = ZL_API_ST_RUN; //����ΪRUN״̬
	if(VM->run.mempool.isInit == ZL_FALSE)
		VM->run.init(VM_ARG); //����������Ҫ�Խ�����������ָ������ڴ˳�ʼ��������
	if(script_str == ZL_NULL)
	{
		VM->run.SetApiError(VM_ARG,ZL_ERR_VM_API_INVALID_SCRIPT_STR_WHEN_RUNSTR,arg);
		return -1;
	}
	VM->compile.source.run_str = script_str;
	VM->compile.source.run_str_len = len;
	retcode = VM->compile.main(VM_ARG,script_name,VM->vm_main_args);
	if(retcode == 0) //�������ɹ�������������
	{
		retcode = VM->run.main(VM_ARG);
	}
	VM->end_time = ZENGL_SYS_TIME_CLOCK();
	VM->total_time = VM->end_time - VM->start_time; //�õ�������ܵ�ִ��ʱ��
	VM->ApiState = ZL_API_ST_AFTER_RUN; //����ΪAFTER_RUN״̬
	if(VM->compile.isReUse == ZL_TRUE)
		VM->compile.isReUse = ZL_FALSE;
	return retcode;
}

/*�ر�zenglApi_Open�򿪵������ָ�룬���ͷ������ռ�õ�ϵͳ��Դ*/
ZL_EXPORT ZL_EXP_INT zenglApi_Close(ZL_EXP_VOID * VM_ARG)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZL_CHAR * ApiName = "zenglApi_Close";
	if(VM->signer != ZL_VM_SIGNER)
		return -1;
	switch(VM->ApiState)
	{
	case ZL_API_ST_OPEN:
	case ZL_API_ST_RESET:
	case ZL_API_ST_AFTER_RUN:
	case ZL_API_ST_REUSE:
		break;
	default:
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_CALL_POSITION, ApiName , ApiName);
		return -1;
		break;
	}
	VM->signer = ZL_NULL; //��ǩ������Ϊ��Ч��0ֵ
	if(VM->compile.isDestroyed == ZL_FALSE)
		VM->compile.memFreeAll(VM_ARG);
	if(VM->run.isDestroyed == ZL_FALSE)
		VM->run.memFreeAll(VM_ARG);
	ZENGL_SYS_MEM_FREE((ZL_VOID *)VM->vm_main_args);
	ZENGL_SYS_MEM_FREE(VM_ARG);
	return 0;
}

/*API�ӿڣ����ò�����Ӧ�������*/
ZL_EXPORT ZL_EXP_VOID * zenglApi_Reset(ZL_EXP_VOID * VM_ARG)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_EXPORT_VM_MAIN_ARGS * vm_main_args;
	ZL_CHAR * ApiName = "zenglApi_Reset";
	if(VM->signer != ZL_VM_SIGNER)
		return ZL_NULL;
	switch(VM->ApiState)
	{
	case ZL_API_ST_OPEN:
	case ZL_API_ST_AFTER_RUN:
	case ZL_API_ST_REUSE:
		break;
	default:
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_CALL_POSITION, ApiName , ApiName);
		return ZL_NULL;
		break;
	}
	vm_main_args = VM->vm_main_args;
	if(VM->compile.isDestroyed == ZL_FALSE)
		VM->compile.memFreeAll(VM_ARG);
	if(VM->run.isDestroyed == ZL_FALSE)
		VM->run.memFreeAll(VM_ARG);
	(*VM) = ZL_Api_Const_VM;
	VM->vm_main_args = vm_main_args;
	VM->ApiState = ZL_API_ST_RESET;
	return (ZL_EXP_VOID *)VM;
}

/*API�ӿڣ������������֮ǰ�ı�����Դ�������Ϳ���ֱ��ִ��֮ǰ�Ѿ�����õ�ָ�����
 ����isClear��Ϊ0��ִ�������ڴ��������������-1��ʾ��������0��ʾʲô��û��������1��ʾ����ִ��*/
ZL_EXPORT ZL_EXP_INT zenglApi_ReUse(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT isClear)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	if(VM->signer != ZL_VM_SIGNER)
		return -1;
	switch(VM->ApiState)
	{
	case ZL_API_ST_AFTER_RUN:
		break;
	default: //����API״̬��ֱ�ӷ���
		return 0;
		break;
	}
	VM->compile.isReUse = ZL_TRUE;
	if(isClear != 0) //isClear��Ϊ0��ִ�������ڴ��������
	{
		VM->run.FreeAllForReUse(VM_ARG);
	}
	VM->run.reg_list[ZL_R_RT_PC].val.dword = 0; //��PC�Ĵ�������Ϊ0
	VM->run.reg_list[ZL_R_RT_ARGTMP].val.dword = 0; // ��ARGTMP�Ĵ�������Ϊ0
	VM->run.reg_list[ZL_R_RT_LOC].val.dword = 0; // ��LOC�Ĵ�������Ϊ0
	VM->run.reg_list[ZL_R_RT_ARG].val.dword = 0; // ��ARG�Ĵ�������Ϊ0
	VM->run.memblock_freeall_local(VM); // ��֮ǰ��ջ�ռ��е��ڴ���ͷŵ�
	VM->run.vstack_list.count = 0;      // ��ջ��λ������Ϊ0
	VM->run.isUserWantStop = ZL_FALSE; //����������ֹͣ��־����Ϊ ZL_FALSE
	VM->isUseApiSetErrThenStop = ZL_FALSE; //����API�����õĴ���ֹͣ��־
	VM->ApiState = ZL_API_ST_REUSE;
	return 1;
}

/*API�ӿڣ����ر�������ʱ�ĳ�����Ϣ*/
ZL_EXPORT ZL_EXP_CHAR * zenglApi_GetErrorString(ZL_EXP_VOID * VM_ARG)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	if(VM_ARG == ZL_NULL || VM->signer != ZL_VM_SIGNER)
		return (ZL_EXP_CHAR *)ZL_Error_String[ZL_ERR_VM_API_INVALID_ZLVM];

	if(VM->errorno == ZL_NO_ERR_SUCCESS)
		return (ZL_EXP_CHAR *)VM->errorString[VM->errorno];
	else if(VM->isCompileError == ZL_TRUE)
		return VM->compile.errorFullString.str;
	else if(VM->isRunError == ZL_TRUE)
		return VM->run.errorFullString.str;
	else if(VM->ApiError != ZL_NULL)
		return VM->ApiError;
	return ZL_NULL;
}

/*API�ӿڣ�������������е�һЩѡ����*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetFlags(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_VM_MAIN_ARG_FLAGS flag)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	if(VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return -1;
	VM->vm_main_args->flags = flag;
	return 0;
}

/*API�ӿڣ����������һЩ��ѡ�Ĳ������������PRINTָ���Ӧ���û��Զ������������*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetHandle(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_VM_FLAGS_HANDLE_TYPE handleType,ZL_EXP_VOID * handle)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	if(VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return -1;
	switch(handleType)
	{
	case ZL_EXP_VFLAG_HANDLE_COMPILE_INFO:
		VM->vm_main_args->userdef_info = handle;
		break;
	case ZL_EXP_VFLAG_HANDLE_COMPILE_ERROR:
		VM->vm_main_args->userdef_compile_error = handle;
		break;
	case ZL_EXP_VFLAG_HANDLE_RUN_INFO:
		VM->vm_main_args->userdef_run_info = handle;
		break;
	case ZL_EXP_VFLAG_HANDLE_RUN_PRINT:
		VM->vm_main_args->userdef_run_print = handle;
		break;
	case ZL_EXP_VFLAG_HANDLE_RUN_ERROR:
		VM->vm_main_args->userdef_run_error = handle;
		break;
	case ZL_EXP_VFLAG_HANDLE_MODULE_INIT:
		VM->vm_main_args->userdef_module_init = handle;
		break;
	}
	return 0;
}

/*API�ӿڣ����û��Զ���Ĳ���ѹ������ջ��*/
ZL_EXPORT ZL_EXP_INT zenglApi_Push(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_MOD_FUN_ARG_TYPE type,ZL_EXP_CHAR * arg_str,ZL_EXP_LONG arg_integer,ZL_EXP_DOUBLE arg_floatnum)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run;
	ZENGL_RUN_VIRTUAL_MEM_STRUCT tmpmem;  //��ʱ�������ڴ������
	ZL_CHAR * ApiName = "zenglApi_Push";
	ZENGL_SYS_ARG_LIST arg;
	ZENGL_SYS_ARG_END(arg); //��va_end����arg��Ϊ0������64λgcc�±���
	if(VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return -1;
	switch(VM->ApiState)
	{
	case ZL_API_ST_OPEN:
	case ZL_API_ST_RESET:
	case ZL_API_ST_REUSE:
		break;
	default:
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_CALL_POSITION, ApiName , ApiName);
		return -1;
		break;
	}
	run = &VM->run;
	if(run->mempool.isInit == ZL_FALSE)
	{
		run->init(VM_ARG);
	}
	if(run->vstack_list.count == 0) //��Ҫģ�⹹��һ���ű���������ʱ��ջ����������call������ű�������RETָ���ʱ�Ų��ᷢ��ջ�ķ������
	{
		tmpmem.val.dword = ZENGL_RUN_REGVAL(ZL_R_RT_ARG).dword;
		run->VStackListOps(VM_ARG,ZL_R_VMOPT_ADDMEM_INT,-1,tmpmem,ZL_TRUE);
		tmpmem.val.dword = ZENGL_RUN_REGVAL(ZL_R_RT_LOC).dword;
		run->VStackListOps(VM_ARG,ZL_R_VMOPT_ADDMEM_INT,-1,tmpmem,ZL_TRUE);
		tmpmem.val.dword = ZENGL_RUN_REGVAL(ZL_R_RT_ARGTMP).dword;
		run->VStackListOps(VM_ARG,ZL_R_VMOPT_ADDMEM_INT,-1,tmpmem,ZL_TRUE);
		ZENGL_RUN_REGVAL(ZL_R_RT_ARG).dword = run->vstack_list.count;
	}
	switch(type)
	{
	case ZL_EXP_FAT_INT:
		tmpmem.val.dword = arg_integer;
		run->VStackListOps(VM_ARG,ZL_R_VMOPT_ADDMEM_INT,-1,tmpmem,ZL_TRUE);
		break;
	case ZL_EXP_FAT_FLOAT:
		tmpmem.val.qword = arg_floatnum;
		run->VStackListOps(VM_ARG,ZL_R_VMOPT_ADDMEM_DOUBLE,-1,tmpmem,ZL_TRUE);
		break;
	case ZL_EXP_FAT_STR:
		tmpmem.val.str = arg_str;
		run->VStackListOps(VM_ARG,ZL_R_VMOPT_ADDMEM_STR,-1,tmpmem,ZL_TRUE);
		break;
	default:
		run->SetApiError(VM_ARG,ZL_ERR_VM_API_INVALID_ARG_TYPE_WHEN_PUSH,arg);
		return -1;
		break;
	}
	return 0;
}

/*API�ӿڣ����ýű��е�ĳ����*/
ZL_EXPORT ZL_EXP_INT zenglApi_Call(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * script_file,ZL_EXP_CHAR * function_name,ZL_EXP_CHAR * class_name)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run;
	ZENGL_RUN_VIRTUAL_MEM_STRUCT tmpmem;  //��ʱ�������ڴ������
	ZL_INT retcode,tmpFunID,tmpClassID;
	ZENGL_SYS_ARG_LIST arg;
	ZL_CHAR * ApiName = "zenglApi_Call";
	ZENGL_SYS_ARG_END(arg); //��va_end����arg��Ϊ0������64λgcc�±���
	if(VM->signer != ZL_VM_SIGNER)
		return -1;
	switch(VM->ApiState)
	{
	case ZL_API_ST_OPEN:
	case ZL_API_ST_RESET:
	case ZL_API_ST_REUSE:
		break;
	default:
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_CALL_POSITION, ApiName , ApiName);
		return -1;
		break;
	}
	run = &VM->run;
	if(VM->isinApiRun == ZL_TRUE && VM->compile.isReUse == ZL_FALSE)
	{
		VM->run.SetApiError(VM_ARG,ZL_ERR_VM_API_VM_MUST_RESET,arg);
		return -1;
	}
	VM->isinApiRun = ZL_TRUE;
	VM->ApiState = ZL_API_ST_RUN; //����ΪRUN״̬
	if(run->mempool.isInit == ZL_FALSE)
		VM->run.init(VM_ARG); //����������Ҫ�Խ�����������ָ������ڴ˳�ʼ��������
	if(run->vstack_list.count == 0) //��Ҫģ�⹹��һ���ű���������ʱ��ջ����������call������ű�������RETָ���ʱ�Ų��ᷢ��ջ�ķ������
	{
		tmpmem.val.dword = ZENGL_RUN_REGVAL(ZL_R_RT_ARG).dword;
		run->VStackListOps(VM_ARG,ZL_R_VMOPT_ADDMEM_INT,-1,tmpmem,ZL_TRUE);
		tmpmem.val.dword = ZENGL_RUN_REGVAL(ZL_R_RT_LOC).dword;
		run->VStackListOps(VM_ARG,ZL_R_VMOPT_ADDMEM_INT,-1,tmpmem,ZL_TRUE);
		tmpmem.val.dword = ZENGL_RUN_REGVAL(ZL_R_RT_ARGTMP).dword;
		run->VStackListOps(VM_ARG,ZL_R_VMOPT_ADDMEM_INT,-1,tmpmem,ZL_TRUE);
		ZENGL_RUN_REGVAL(ZL_R_RT_ARG).dword = run->vstack_list.count;
	}
	if(script_file == ZL_NULL)
	{
		VM->run.SetApiError(VM_ARG,ZL_ERR_VM_API_INVALID_SCRIPT_NAME,arg);
		return -1;
	}

	retcode = VM->compile.main(VM_ARG,script_file,VM->vm_main_args);
	if(retcode == 0) //�������ɹ�������������
	{
		tmpmem.val.dword = VM->run.inst_list.count - 1;
		VM->run.VStackListOps(VM_ARG,ZL_R_VMOPT_ADDMEM_INT,-1,tmpmem,ZL_TRUE);
		ZENGL_RUN_REGVAL(ZL_R_RT_LOC).dword = VM->run.vstack_list.count;
		if(class_name != ZL_NULL)
		{
			tmpClassID = VM->compile.SymLookupClassByName(VM_ARG,class_name);
			if(tmpClassID == 0)
			{
				VM->errorno = ZL_ERR_VM_API_INVALID_CLASS_NAME_WHEN_CALL;
				VM->run.makeInfoString(VM_ARG,&VM->run.errorFullString , VM->errorString[VM->errorno] , arg);
				VM->isRunError = ZL_TRUE;
				return -1;
			}
			else if(tmpClassID == -1)
			{
				VM->errorno = ZL_ERR_VM_API_INVALID_CLASS_TABLE_INDEX_WHEN_CALL;
				VM->run.makeInfoString(VM_ARG,&VM->run.errorFullString , VM->errorString[VM->errorno] , arg);
				VM->isRunError = ZL_TRUE;
				return -1;
			}
		}
		else
			tmpClassID = 0;
		tmpFunID = VM->compile.SymLookupFunByName(VM_ARG,function_name,tmpClassID);
		if(tmpFunID == 0)
		{
			VM->errorno = ZL_ERR_VM_API_INVALID_FUNCTION_NAME;
			VM->run.makeInfoString(VM_ARG,&VM->run.errorFullString , VM->errorString[VM->errorno] , arg);
			VM->isRunError = ZL_TRUE;
			retcode = -1;
		}
		else if(tmpFunID == -1)
		{
			VM->errorno = ZL_ERR_CP_SYM_FUN_TABLE_FIND_NOT_VALID_INDEX;
			VM->run.makeInfoString(VM_ARG,&VM->run.errorFullString , VM->errorString[VM->errorno] , arg);
			VM->isRunError = ZL_TRUE;
			retcode = -1;
		}
		else
		{
			ZL_R_REG_PC = VM->compile.LDAddrList.addr[VM->compile.SymFunTable.funs[tmpFunID].LDAdr].loc;
			retcode = VM->run.main(VM_ARG);
		}
	}
	VM->end_time = ZENGL_SYS_TIME_CLOCK();
	VM->total_time = VM->end_time - VM->start_time; //�õ�������ܵ�ִ��ʱ��
	VM->ApiState = ZL_API_ST_AFTER_RUN; //����ΪAFTER_RUN״̬
	if(VM->compile.isReUse == ZL_TRUE)
		VM->compile.isReUse = ZL_FALSE;
	return retcode;
}

/*API�ӿڣ�ͨ���˽ӿڻ�ȡĳ������ֵ���ַ�����ʽ*/
ZL_EXPORT ZL_EXP_CHAR * zenglApi_GetValueAsString(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * valueName)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run;
	ZENGL_COMPILE_TYPE * compile;
	ZL_INT memloc = -1;
	ZENGL_RUN_VIRTUAL_MEM_STRUCT tmpmem;  //��ʱ�������ڴ������
	ZL_CHAR tmpstr[80];
	ZENGL_SYS_ARG_LIST arg;
	ZL_CHAR * ApiName = "zenglApi_GetValueAsString";
	ZENGL_SYS_ARG_END(arg); //��va_end����arg��Ϊ0������64λgcc�±���
	if(VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return ZL_NULL;
	switch(VM->ApiState)
	{
	case ZL_API_ST_AFTER_RUN:
	case ZL_API_ST_REUSE:
	case ZL_API_ST_MOD_FUN_HANDLE:
		break;
	default:
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_CALL_POSITION, ApiName , ApiName);
		return ZL_NULL;
		break;
	}
	run = &VM->run;
	compile = &VM->compile;
	VM->run.freeInfoString(VM_ARG,&VM->run.errorFullString);
	if((VM->vm_main_args->flags & ZL_EXP_CP_AF_IN_DEBUG_MODE) == 0)
	{
		VM->run.SetApiError(VM_ARG,ZL_ERR_VM_API_GET_VALUE_AS_MUST_IN_DEBUG_MODE,arg);
		return ZL_NULL;
	}
	memloc = compile->SymLookupID_ByName(VM_ARG,valueName);
	if(memloc == -1)
	{
		VM->run.SetApiError(VM_ARG,ZL_ERR_VM_API_GET_VALUE_AS_VALUE_NAME_INVALID,arg);
		return ZL_NULL;
	}
	else if(memloc == -2)
	{
		VM->run.SetApiError(VM_ARG,ZL_ERR_VM_API_SYM_GLOBAL_TABLE_INVALID_INDEX,arg);
		return ZL_NULL;
	}
	switch(run->vmem_list.mem_array[memloc].runType)
	{
	case ZL_R_RDT_INT:
		ZENGL_SYS_SPRINTF(tmpstr,"%ld",run->vmem_list.mem_array[memloc].val.dword);
		tmpmem.val.str = (ZL_VOID *)tmpstr;
		run->VMemListOps(VM_ARG,ZL_R_VMOPT_SETMEM_STR,memloc,tmpmem);
		run->vmem_list.mem_array[memloc].runType = ZL_R_RDT_INT;
		return (ZL_EXP_CHAR *)run->vmem_list.mem_array[memloc].val.str;
		break;
	case ZL_R_RDT_FLOAT:
		ZENGL_SYS_SPRINTF(tmpstr,"%.16g",run->vmem_list.mem_array[memloc].val.qword);
		tmpmem.val.str = (ZL_VOID *)tmpstr;
		run->VMemListOps(VM_ARG,ZL_R_VMOPT_SETMEM_STR,memloc,tmpmem);
		run->vmem_list.mem_array[memloc].runType = ZL_R_RDT_FLOAT;
		return (ZL_EXP_CHAR *)run->vmem_list.mem_array[memloc].val.str;
		break;
	case ZL_R_RDT_STR:
		return (ZL_EXP_CHAR *)run->vmem_list.mem_array[memloc].val.str;
		break;
	}
	VM->run.SetApiError(VM_ARG,ZL_ERR_VM_API_GET_VALUE_AS_VALUE_RUNTYPE_INVALID,arg);
	return ZL_NULL;
}

/*API�ӿڣ�ͨ���˽ӿڻ�ȡĳ������ֵ��������ʽ*/
ZL_EXPORT ZL_EXP_INT zenglApi_GetValueAsInt(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * valueName,ZL_EXP_LONG * retValue)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run;
	ZENGL_COMPILE_TYPE * compile;
	ZL_INT memloc = -1;
	ZENGL_SYS_ARG_LIST arg;
	ZL_CHAR * ApiName = "zenglApi_GetValueAsInt";
	ZENGL_SYS_ARG_END(arg); //��va_end����arg��Ϊ0������64λgcc�±���
	if(VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return -1;
	switch(VM->ApiState)
	{
	case ZL_API_ST_AFTER_RUN:
	case ZL_API_ST_REUSE:
	case ZL_API_ST_MOD_FUN_HANDLE:
		break;
	default:
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_CALL_POSITION, ApiName , ApiName);
		return -1;
		break;
	}
	run = &VM->run;
	compile = &VM->compile;
	VM->run.freeInfoString(VM_ARG,&VM->run.errorFullString);
	if((VM->vm_main_args->flags & ZL_EXP_CP_AF_IN_DEBUG_MODE) == 0)
	{
		VM->run.SetApiError(VM_ARG,ZL_ERR_VM_API_GET_VALUE_AS_MUST_IN_DEBUG_MODE,arg);
		return -1;
	}
	memloc = compile->SymLookupID_ByName(VM_ARG,valueName);
	if(memloc == -1)
	{
		VM->run.SetApiError(VM_ARG,ZL_ERR_VM_API_GET_VALUE_AS_VALUE_NAME_INVALID,arg);
		return -1;
	}
	else if(memloc == -2)
	{
		VM->run.SetApiError(VM_ARG,ZL_ERR_VM_API_SYM_GLOBAL_TABLE_INVALID_INDEX,arg);
		return -1;
	}
	switch(run->vmem_list.mem_array[memloc].runType)
	{
	case ZL_R_RDT_INT:
		(*retValue) = run->vmem_list.mem_array[memloc].val.dword;
		return 0;
		break;
	case ZL_R_RDT_FLOAT:
		(*retValue) = (ZL_LONG)run->vmem_list.mem_array[memloc].val.qword;
		return 0;
		break;
	case ZL_R_RDT_STR:
		(*retValue) = ZENGL_SYS_STR_TO_LONG_NUM((ZL_CONST ZL_EXP_CHAR *)run->vmem_list.mem_array[memloc].val.str);
		return 0;
		break;
	}
	VM->run.SetApiError(VM_ARG,ZL_ERR_VM_API_GET_VALUE_AS_VALUE_RUNTYPE_INVALID,arg);
	return -1;
}

/*API�ӿڣ�ͨ���˽ӿڻ�ȡĳ������ֵ�ĸ�������ʽ*/
ZL_EXPORT ZL_EXP_INT zenglApi_GetValueAsDouble(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * valueName,ZL_EXP_DOUBLE * retValue)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run;
	ZENGL_COMPILE_TYPE * compile;
	ZL_INT memloc = -1;
	ZENGL_SYS_ARG_LIST arg;
	ZL_CHAR * ApiName = "zenglApi_GetValueAsDouble";
	ZENGL_SYS_ARG_END(arg); //��va_end����arg��Ϊ0������64λgcc�±���
	if(VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return -1;
	switch(VM->ApiState)
	{
	case ZL_API_ST_AFTER_RUN:
	case ZL_API_ST_REUSE:
	case ZL_API_ST_MOD_FUN_HANDLE:
		break;
	default:
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_CALL_POSITION, ApiName , ApiName);
		return -1;
		break;
	}
	run = &VM->run;
	compile = &VM->compile;
	VM->run.freeInfoString(VM_ARG,&VM->run.errorFullString);
	if((VM->vm_main_args->flags & ZL_EXP_CP_AF_IN_DEBUG_MODE) == 0)
	{
		VM->run.SetApiError(VM_ARG,ZL_ERR_VM_API_GET_VALUE_AS_MUST_IN_DEBUG_MODE,arg);
		return -1;
	}
	memloc = compile->SymLookupID_ByName(VM_ARG,valueName);
	if(memloc == -1)
	{
		VM->run.SetApiError(VM_ARG,ZL_ERR_VM_API_GET_VALUE_AS_VALUE_NAME_INVALID,arg);
		return -1;
	}
	else if(memloc == -2)
	{
		VM->run.SetApiError(VM_ARG,ZL_ERR_VM_API_SYM_GLOBAL_TABLE_INVALID_INDEX,arg);
		return -1;
	}
	switch(run->vmem_list.mem_array[memloc].runType)
	{
	case ZL_R_RDT_INT:
		(*retValue) = (ZL_DOUBLE)run->vmem_list.mem_array[memloc].val.dword;
		return 0;
		break;
	case ZL_R_RDT_FLOAT:
		(*retValue) = run->vmem_list.mem_array[memloc].val.qword;
		return 0;
		break;
	case ZL_R_RDT_STR:
		(*retValue) = ZENGL_SYS_STR_TO_FLOAT((ZL_CONST ZL_EXP_CHAR *)run->vmem_list.mem_array[memloc].val.str);
		return 0;
		break;
	}
	VM->run.SetApiError(VM_ARG,ZL_ERR_VM_API_GET_VALUE_AS_VALUE_RUNTYPE_INVALID,arg);
	return -1;
}

/*
	API�ӿڣ��û�����ͨ���˽ӿڶ���ģ���ʼ������
*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetModInitHandle(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * moduleName,ZL_EXP_VOID * modInitFun)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run;
	ZL_INT h;
	ZL_INT tmpindex;
	ZL_VM_API_MOD_INIT_FUNC module_init_function = (ZL_VM_API_MOD_INIT_FUNC)modInitFun;
	ZL_CHAR * ApiName = "zenglApi_SetModInitHandle";
	if(VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return -1;
	switch(VM->ApiState)
	{
	case ZL_API_ST_AFTER_RUN:
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_CALL_POSITION, ApiName , ApiName);
		return -1;
		break;
	}
	run = &VM->run;
	h = run->Hash(VM_ARG,moduleName) + ZL_R_HASH_SIZE * ZL_R_HASH_TYPE_MODULE_TABLE;
	tmpindex = run->HashTable[h];
	if(run->mempool.isInit == ZL_FALSE)
	{
		run->init(VM_ARG);
	}
	if(run->isinRunning == ZL_FALSE)
	{
		ZL_INT retcode;
		if((retcode = ZENGL_SYS_JMP_SETJMP(run->jumpBuffer)) == 0)
			;
		else if(retcode == 1)
		{
			ZENGL_SYS_MEM_SET(run->jumpBuffer,0,sizeof(run->jumpBuffer));
			return 0;
		}
		else if(retcode == -1)
		{
			VM->isRunError = ZL_TRUE;
			ZENGL_SYS_MEM_SET(run->jumpBuffer,0,sizeof(run->jumpBuffer));
			return -1;
		}
	}
	while(tmpindex != 0 && run->moduleTable.modules[tmpindex].isvalid == ZL_TRUE && 
		ZENGL_SYS_STRCMP(moduleName,run->InstDataStringPoolGetPtr(VM_ARG,run->moduleTable.modules[tmpindex].strIndex)) != 0)
		tmpindex = run->moduleTable.modules[tmpindex].next;
	if(tmpindex == 0)
	{
		tmpindex = run->HashTable[h];
		run->HashTable[h] = run->InsertModuleTable(VM_ARG,moduleName,module_init_function);
		run->moduleTable.modules[run->HashTable[h]].next = tmpindex;
		return run->HashTable[h];
	}
	else if(run->moduleTable.modules[tmpindex].isvalid == ZL_FALSE)
		run->exit(VM_ARG,ZL_ERR_RUN_MODULE_TABLE_FIND_INVALID_INDEX);
	else
	{
		if(VM->compile.isReUse == ZL_TRUE) //����������£�ֱ�ӷ�����ǰ�����������ֵ
			return tmpindex;
		else
			run->exit(VM_ARG,ZL_ERR_RUN_MODULE_HAS_BEEN_SET_HANDLE_BEFORE,moduleName,moduleName);
	}
	return -1;
}

/*
	API�ӿڣ��û�ͨ���˽ӿڿ����Զ���ĳģ���еĺ���������
*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetModFunHandle(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT moduleID,ZL_EXP_CHAR * functionName,ZL_EXP_VOID * funHandle)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run;
	ZL_INT h;
	ZL_INT tmpindex;
	ZL_VM_API_MOD_FUN_HANDLE handle = (ZL_VM_API_MOD_FUN_HANDLE)funHandle;
	ZL_CHAR * ApiName = "zenglApi_SetModFunHandle";
	if(VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return -1;
	switch(VM->ApiState)
	{
	case ZL_API_ST_AFTER_RUN:
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_CALL_POSITION, ApiName , ApiName);
		return -1;
		break;
	}
	run = &VM->run;
	h = run->Hash(VM_ARG,functionName) + ZL_R_HASH_SIZE * ZL_R_HASH_TYPE_MOD_FUN_TABLE;
	tmpindex = run->HashTable[h];
	if(run->mempool.isInit == ZL_FALSE)
	{
		run->init(VM_ARG);
	}
	if(run->isinRunning == ZL_FALSE)
	{
		ZL_INT retcode;
		if((retcode = ZENGL_SYS_JMP_SETJMP(run->jumpBuffer)) == 0)
			;
		else if(retcode == 1)
		{
			ZENGL_SYS_MEM_SET(run->jumpBuffer,0,sizeof(run->jumpBuffer));
			return 0;
		}
		else if(retcode == -1)
		{
			VM->isRunError = ZL_TRUE;
			ZENGL_SYS_MEM_SET(run->jumpBuffer,0,sizeof(run->jumpBuffer));
			return -1;
		}
	}
	while(tmpindex != 0 && run->ModFunTable.mod_funs[tmpindex].isvalid == ZL_TRUE && 
		ZENGL_SYS_STRCMP(functionName,run->InstDataStringPoolGetPtr(VM_ARG,run->ModFunTable.mod_funs[tmpindex].strIndex)) != 0)
		tmpindex = run->ModFunTable.mod_funs[tmpindex].next;
	if(tmpindex == 0)
	{
		tmpindex = run->HashTable[h];
		run->HashTable[h] = run->InsertModFunTable(VM_ARG,moduleID,functionName,handle);
		run->ModFunTable.mod_funs[run->HashTable[h]].next = tmpindex;
		return 0;
	}
	else if(run->ModFunTable.mod_funs[tmpindex].isvalid == ZL_FALSE)
		run->exit(VM_ARG,ZL_ERR_RUN_MOD_FUN_TABLE_FIND_INVALID_INDEX);
	else
	{
		ZL_CHAR * moduleName;
		if(VM->compile.isReUse == ZL_TRUE) //����������£�ֱ�ӷ���
			return 0;
		else
		{
			moduleName = run->InstDataStringPoolGetPtr(VM_ARG,run->moduleTable.modules[run->ModFunTable.mod_funs[tmpindex].moduleID].strIndex);
			run->exit(VM_ARG,ZL_ERR_RUN_MOD_FUN_HAS_BEEN_SET_HANDLE_IN_OTHER_MOD_BEFORE,functionName,moduleName,functionName,moduleName);
		}
	}
	return -1;
}

/*
	API�ӿڣ���ȡ�ű���ģ�麯������ʱ�Ĳ�����Ϣ
	argnum Ϊ����λ�ã���1��ʼ��1�����һ��������2����ڶ����������Դ�����
*/
ZL_EXPORT ZL_EXP_INT zenglApi_GetFunArg(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argnum,ZENGL_EXPORT_MOD_FUN_ARG * retval)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run;
	ZENGL_RUN_VIRTUAL_MEM_STRUCT tmpmem = {0};  //��ʱ�������ڴ������
	ZL_INT argcount;
	ZL_CHAR * ApiName = "zenglApi_GetFunArg";
	if(VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return -1;
	switch(VM->ApiState)
	{
	case ZL_API_ST_MOD_FUN_HANDLE:
		break;
	default:
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_CALL_POSITION, ApiName , ApiName);
		return -1;
		break;
	}
	if(retval == ZL_NULL)
		return 0;
	run = &VM->run;
	argcount = ZENGL_RUN_REGVAL(ZL_R_RT_LOC).dword - ZENGL_RUN_REGVAL(ZL_R_RT_ARG).dword - 1;
	if(argnum < 1 || argnum > argcount)
		run->exit(VM_ARG,ZL_ERR_VM_API_GET_FUN_ARG_ARGNUM_IS_INVALID,ZL_R_REG_PC);
	tmpmem = run->VStackListOps(VM_ARG,ZL_R_VMOPT_GETMEM,ZENGL_RUN_REGVAL(ZL_R_RT_ARG).dword + argnum - 1,tmpmem,ZL_TRUE);
	switch(tmpmem.runType)
	{
	case ZL_R_RDT_NONE:
		retval->type = ZL_EXP_FAT_NONE;
		retval->val.integer = tmpmem.val.dword;
		break;
	case ZL_R_RDT_INT:
		retval->type = ZL_EXP_FAT_INT;
		retval->val.integer = tmpmem.val.dword;
		break;
	case ZL_R_RDT_FLOAT:
		retval->type = ZL_EXP_FAT_FLOAT;
		retval->val.floatnum = tmpmem.val.qword;
		break;
	case ZL_R_RDT_STR:
		retval->type = ZL_EXP_FAT_STR;
		retval->val.str = (ZL_CHAR *)tmpmem.val.str;
		break;
	case ZL_R_RDT_MEM_BLOCK: //�ڴ��
		retval->type = ZL_EXP_FAT_MEMBLOCK;
		retval->val.memblock.ptr = tmpmem.val.memblock;
		retval->val.memblock.index = tmpmem.memblk_Index;
		break;
	case ZL_R_RDT_ADDR: //ȫ�ֱ������ã��ú���������Ӧ�û�ȡ�����������ͣ�ֻ�������GetFunArgInfo���ܻ�ȡ���������ͣ���Ϊrun->VStackListOps�������Զ��ݹ�������ö�Ӧ�ı�����ֵ�����������ȰѴ����������
		retval->type = ZL_EXP_FAT_ADDR;
		retval->val.addr.index = tmpmem.val.dword;
		break;
	case ZL_R_RDT_ADDR_LOC: //�ֲ���������
		retval->type = ZL_EXP_FAT_ADDR_LOC;
		retval->val.addr.index = tmpmem.val.dword;
		break;
	case ZL_R_RDT_ADDR_MEMBLK: //�ڴ������
		retval->type = ZL_EXP_FAT_ADDR_MEMBLK;
		retval->val.addr.index = tmpmem.val.dword;
		retval->val.addr.memblock.ptr = tmpmem.val.memblock;
		retval->val.addr.memblock.index = tmpmem.memblk_Index;
		break;
	}
	return 0;
}

/*
	API�ӿڣ��˳���ǰ�ű�
*/
ZL_EXPORT ZL_EXP_INT zenglApi_Exit(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * errorStr, ...)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_COMPILE_TYPE * compile;
	ZENGL_RUN_TYPE * run;
	ZENGL_SYS_ARG_LIST arg;
	ZL_INT error_nodenum = 0;
	ZL_INT isNeedDebugInfo = ZL_EXP_CP_AF_IN_DEBUG_MODE | ZL_EXP_CP_AF_OUTPUT_DEBUG_INFO;
	ZL_CHAR * ApiName = "zenglApi_Exit";
	if(VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return -1;
	switch(VM->ApiState)
	{
	case ZL_API_ST_MODULES_INIT:
	case ZL_API_ST_MOD_INIT_HANDLE:
	case ZL_API_ST_MOD_FUN_HANDLE:
		break;
	default:
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_CALL_POSITION, ApiName , ApiName);
		return -1;
		break;
	}
	compile = &VM->compile;
	run = &VM->run;
	VM->errorno = ZL_ERR_VM_API_USER_DEFINED_ERROR;

	ZENGL_SYS_ARG_START(arg,errorStr);
	run->makeInfoString(VM_ARG,&run->errorFullString , VM->errorString[VM->errorno] , arg);
	run->makeInfoString(VM_ARG,&run->errorFullString , errorStr , arg);
	if((VM->vm_main_args->flags & ZL_EXP_CP_AF_IN_DEBUG_MODE) != 0) //�û��Զ���ĵ���ģʽ�£���ӡ���ڵ�����к���Ϣ
	{
		error_nodenum = run->inst_list.insts[ZL_R_REG_PC].nodenum;
		run->ExitPrintSourceInfo(VM_ARG,ZL_ERR_RUN_SOURCE_CODE_INFO,
			compile->getTokenStr(VM_ARG,compile->AST_nodes.nodes,error_nodenum),
			compile->AST_nodes.nodes[error_nodenum].line_no,
			compile->AST_nodes.nodes[error_nodenum].col_no,
			compile->AST_nodes.nodes[error_nodenum].filename);
		if(!compile->isDestroyed && VM->isinApiRun == ZL_FALSE)
		{
			compile->memFreeAll(VM_ARG);
			compile->infoFullString.str = compile->errorFullString.str = ZL_NULL;
			compile->isDestroyed = ZL_TRUE;
		}
	}
	if(run->userdef_run_error != ZL_NULL)
		run->userdef_run_error(run->errorFullString.str,run->errorFullString.count,VM_ARG);
	if(VM->isinApiRun == ZL_FALSE)
		run->freeInfoString(VM_ARG,&run->errorFullString);
	ZENGL_SYS_ARG_END(arg);

	run->end_time = ZENGL_SYS_TIME_CLOCK();
	run->total_time = run->end_time - run->start_time;
	if(VM->errorno != ZL_NO_ERR_SUCCESS)
		VM->isRunError = ZL_TRUE;
	if((VM->vm_main_args->flags & isNeedDebugInfo) == isNeedDebugInfo) //�û��Զ���ĵ���ģʽ��
	{
		ZL_INT run_totalsize;
		ZL_INT vm_totalsize;
		run_totalsize = VM->debug.orig_run_totalsize != 0 ? VM->debug.orig_run_totalsize : run->totalsize;
		vm_totalsize = VM->debug.orig_vm_totalsize != 0 ? VM->debug.orig_vm_totalsize : VM->totalsize;
		VM->end_time = ZENGL_SYS_TIME_CLOCK();
		VM->total_time = VM->end_time - VM->start_time; //�õ�������ܵ�ִ��ʱ��
		run->info(VM_ARG,"\n run time:%.16g s totalsize: %.16g Kbyte\n VM time:%.16g s totalsize: %.16g Kbyte\n\n",(ZL_DOUBLE)run->total_time / CLOCKS_PER_SEC,
		(ZL_FLOAT)run_totalsize / 1024,
		(ZL_DOUBLE)VM->total_time / CLOCKS_PER_SEC,
		(ZL_FLOAT)vm_totalsize / 1024); //for debug
	}
	if(VM->isinApiRun == ZL_FALSE)
	{
		run->memFreeAll(VM_ARG);
		run->isDestroyed = ZL_TRUE;
		run->infoFullString.str = run->errorFullString.str = run->printFullString.str = ZL_NULL;
	}
	run->isinRunning = ZL_FALSE;
	if(VM->errorno == ZL_NO_ERR_SUCCESS)
		ZENGL_SYS_JMP_LONGJMP_TO(run->jumpBuffer,1);
	else
		ZENGL_SYS_JMP_LONGJMP_TO(run->jumpBuffer,-1);
	return 0;
}

/*API�ӿڣ��������û��Զ����ģ�麯�������ó�����Ϣ��Ȼ�����������ִֹͣ�У���zenglApi_Exit�õĵط����ڣ����᳤��תֱ�ӽ��������Ƿ������û������˳���ʱ������Ч��ֹ�ⲿC++���ó����ڴ�й©������쳣*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetErrThenStop(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * errorStr, ...)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_COMPILE_TYPE * compile;
	ZENGL_RUN_TYPE * run;
	ZENGL_SYS_ARG_LIST arg;
	ZL_INT error_nodenum = 0;
	ZL_CHAR * ApiName = "zenglApi_SetErrThenStop";
	if(VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return -1;
	switch(VM->ApiState)
	{
	case ZL_API_ST_MODULES_INIT:
	case ZL_API_ST_MOD_INIT_HANDLE:
	case ZL_API_ST_MOD_FUN_HANDLE:
		break;
	default:
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_CALL_POSITION, ApiName , ApiName);
		return -1;
		break;
	}
	compile = &VM->compile;
	run = &VM->run;
	VM->errorno = ZL_ERR_VM_API_USER_DEFINED_ERROR;

	/*�����úʹ�ӡ������Ϣ*/
	ZENGL_SYS_ARG_START(arg,errorStr);
	run->makeInfoString(VM_ARG,&run->errorFullString , VM->errorString[VM->errorno] , arg);
	run->makeInfoString(VM_ARG,&run->errorFullString , errorStr , arg);
	if((VM->vm_main_args->flags & ZL_EXP_CP_AF_IN_DEBUG_MODE) != 0) //�û��Զ���ĵ���ģʽ�£���ӡ���ڵ�����к���Ϣ
	{
		error_nodenum = run->inst_list.insts[ZL_R_REG_PC].nodenum;
		run->ExitPrintSourceInfo(VM_ARG,ZL_ERR_RUN_SOURCE_CODE_INFO,
			compile->getTokenStr(VM_ARG,compile->AST_nodes.nodes,error_nodenum),
			compile->AST_nodes.nodes[error_nodenum].line_no,
			compile->AST_nodes.nodes[error_nodenum].col_no,
			compile->AST_nodes.nodes[error_nodenum].filename);
	}
	if(run->userdef_run_error != ZL_NULL)
		run->userdef_run_error(run->errorFullString.str,run->errorFullString.count,VM_ARG);
	if(VM->isinApiRun == ZL_FALSE)
		run->freeInfoString(VM_ARG,&run->errorFullString);
	ZENGL_SYS_ARG_END(arg);

	/*���������ֹͣ��־*/
	zenglApi_Stop(VM_ARG);

	/*����isUseApiSetErrThenStop��־����ʾͨ����API������ֹͣ���˳��������*/
	VM->isUseApiSetErrThenStop = ZL_TRUE;

	return 0;
}

/*API�ӿڣ�����ģ�麯���ķ���ֵ*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetRetVal(ZL_EXP_VOID * VM_ARG,
										 ZENGL_EXPORT_MOD_FUN_ARG_TYPE type,ZL_EXP_CHAR * arg_str,ZL_EXP_LONG arg_integer,ZL_EXP_DOUBLE arg_floatnum)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run;
	ZL_CHAR * ApiName = "zenglApi_SetRetVal";
	if(VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return -1;
	switch(VM->ApiState)
	{
	case ZL_API_ST_MOD_FUN_HANDLE:
		break;
	default:
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_CALL_POSITION, ApiName , ApiName);
		return -1;
		break;
	}
	run = &VM->run;
	switch(type)
	{
	case ZL_EXP_FAT_INT:
		ZENGL_RUN_REG(ZL_R_RT_AX).runType = ZL_R_RDT_INT;
		ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword = arg_integer;
		break;
	case ZL_EXP_FAT_FLOAT:
		ZENGL_RUN_REG(ZL_R_RT_AX).runType = ZL_R_RDT_FLOAT;
		ZENGL_RUN_REGVAL(ZL_R_RT_AX).qword = arg_floatnum;
		break;
	case ZL_EXP_FAT_STR:
		ZENGL_RUN_REG(ZL_R_RT_AX).runType = ZL_R_RDT_STR;
		run->RegAssignStr(VM_ARG,ZL_R_RT_AX,arg_str);
		break;
	default:
		run->exit(VM_ARG,ZL_ERR_VM_API_SET_RET_VAL_INVALID_TYPE);
		return -1;
		break;
	}
	return 0;
}

/*API�ӿڣ�������ֵ��Ϊ�ڴ��*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetRetValAsMemBlock(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_MEMBLOCK * memblock)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run;
	ZL_CHAR * ApiName = "zenglApi_SetRetValAsMemBlock";
	if(VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return -1;
	switch(VM->ApiState)
	{
	case ZL_API_ST_MOD_FUN_HANDLE:
		break;
	default:
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_CALL_POSITION, ApiName , ApiName);
		return -1;
		break;
	}
	run = &VM->run;
	if(memblock == ZL_NULL)
	{
		run->exit(VM_ARG,ZL_ERR_VM_API_INVALID_MEMBLOCK_ARG_IN_SET_RETVAL);
		return -1;
	}
	ZENGL_RUN_REG(ZL_R_RT_AX).runType = ZL_R_RDT_MEM_BLOCK;
	ZENGL_RUN_REGVAL(ZL_R_RT_AX).memblock = memblock->ptr;
	ZENGL_RUN_REG(ZL_R_RT_AX).memblk_Index = memblock->index;
	return 0;
}

/*API�ӿڣ�����size���������㹻����size��Ԫ�ص��ڴ��(�ڴ���С��һ������size��ֻ�Ǵ��ڵ���size)�����飬������������ڴ��*/
ZL_EXPORT ZL_EXP_INT zenglApi_CreateMemBlock(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_MEMBLOCK * memblock,ZL_EXP_INT size)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run;
	ZL_CHAR * ApiName = "zenglApi_CreateMemBlock";
	if(VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return -1;
	switch(VM->ApiState)
	{
	case ZL_API_ST_MOD_FUN_HANDLE:
		break;
	default:
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_CALL_POSITION, ApiName , ApiName);
		return -1;
		break;
	}
	run = &VM->run;
	if(memblock == ZL_NULL)
	{
		run->exit(VM_ARG,ZL_ERR_VM_API_INVALID_MEMBLOCK_ARG_IN_CREATE_MEM_BLOCK);
		return -1;
	}
	else if(size < 0)
	{
		run->exit(VM_ARG,ZL_ERR_VM_API_INVALID_SIZE_ARG_IN_CREATE_MEM_BLOCK);
		return -1;
	}
	else if(size == 0)
	{
		memblock->ptr = (ZL_VOID *)run->alloc_memblock(VM_ARG,&memblock->index);
		return 0;
	}
	memblock->ptr = (ZL_VOID *)run->alloc_memblock(VM_ARG,&memblock->index);
	memblock->ptr = (ZL_VOID *)run->realloc_memblock(VM_ARG,(ZENGL_RUN_VIRTUAL_MEM_LIST *)memblock->ptr,size);
	return 0;
}

/*API�ӿڣ�����������ڴ������ü���ֵ(���add_refcount�Ǹ��������Ǽ������ü���ֵ)������0��ʾ�ɹ�������-1��ʾʧ��*/
ZL_EXPORT ZL_EXP_INT zenglApi_AddMemBlockRefCount(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_MEMBLOCK * memblock,ZL_EXP_INT add_refcount)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_VIRTUAL_MEM_LIST * mem_list;
	ZL_CHAR * ApiName = "zenglApi_AddMemBlockRefCount";
	if(VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return -1;
	switch(VM->ApiState)
	{
	case ZL_API_ST_MOD_FUN_HANDLE:
		break;
	default:
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_CALL_POSITION, ApiName , ApiName);
		return -1;
		break;
	}
	if(memblock == ZL_NULL)
		return -1;
	mem_list = (ZENGL_RUN_VIRTUAL_MEM_LIST *)memblock->ptr;
	mem_list->refcount += add_refcount;
	return 0;
}

/*�����ڴ����index - 1������Ӧ��ֵ��index����Ϊ1��ʾ������ڴ��ĵ�һ��Ԫ�أ��Դ����ƣ�����0��ʾ�ɹ�������-1��ʾretval���ʹ���*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetMemBlock(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_MEMBLOCK * memblock,ZL_EXP_INT index,ZENGL_EXPORT_MOD_FUN_ARG * retval)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run;
	ZENGL_RUN_VIRTUAL_MEM_STRUCT tmpmem = {0};
	ZL_CHAR * ApiName = "zenglApi_SetMemBlock";
	if(VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return -1;
	switch(VM->ApiState)
	{
	case ZL_API_ST_MOD_FUN_HANDLE:
		break;
	default:
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_CALL_POSITION, ApiName , ApiName);
		return -1;
		break;
	}
	if(retval == ZL_NULL)
		return 0;
	run = &VM->run;
	run->realloc_memblock(VM_ARG,(ZENGL_RUN_VIRTUAL_MEM_LIST *)memblock->ptr,index-1);
	switch(retval->type)
	{
	case ZL_EXP_FAT_NONE:
		tmpmem.runType = ZL_R_RDT_NONE;
		run->VMemBlockOps(VM_ARG,ZL_R_VMOPT_SETMEM_NONE,(ZENGL_RUN_VIRTUAL_MEM_LIST *)memblock->ptr,(index - 1),&tmpmem);
		break;
	case ZL_EXP_FAT_INT:
		tmpmem.val.dword = retval->val.integer;
		run->VMemBlockOps(VM_ARG,ZL_R_VMOPT_SETMEM_INT,(ZENGL_RUN_VIRTUAL_MEM_LIST *)memblock->ptr,(index - 1),&tmpmem);
		break;
	case ZL_EXP_FAT_FLOAT:
		tmpmem.val.qword = retval->val.floatnum;
		run->VMemBlockOps(VM_ARG,ZL_R_VMOPT_SETMEM_DOUBLE,(ZENGL_RUN_VIRTUAL_MEM_LIST *)memblock->ptr,(index - 1),&tmpmem);
		break;
	case ZL_EXP_FAT_STR:
		tmpmem.val.str = (ZL_VOID *)retval->val.str;
		run->VMemBlockOps(VM_ARG,ZL_R_VMOPT_SETMEM_STR,(ZENGL_RUN_VIRTUAL_MEM_LIST *)memblock->ptr,(index - 1),&tmpmem);
		break;
	case ZL_EXP_FAT_MEMBLOCK:
		tmpmem.val.memblock = retval->val.memblock.ptr;
		tmpmem.memblk_Index = retval->val.memblock.index;
		run->VMemBlockOps(VM_ARG,ZL_R_VMOPT_SETMEM_MEMBLOCK,(ZENGL_RUN_VIRTUAL_MEM_LIST *)memblock->ptr,(index - 1),&tmpmem);
		break;
	case ZL_EXP_FAT_ADDR:
		tmpmem.runType = ZL_R_RDT_ADDR;
		tmpmem.val.dword = retval->val.addr.index;
		run->VMemBlockOps(VM_ARG,ZL_R_VMOPT_SETMEM_ADDR,(ZENGL_RUN_VIRTUAL_MEM_LIST *)memblock->ptr,(index - 1),&tmpmem);
		break;
	case ZL_EXP_FAT_ADDR_LOC:
		tmpmem.runType = ZL_R_RDT_ADDR_LOC;
		tmpmem.val.dword = retval->val.addr.index;
		run->VMemBlockOps(VM_ARG,ZL_R_VMOPT_SETMEM_ADDR,(ZENGL_RUN_VIRTUAL_MEM_LIST *)memblock->ptr,(index - 1),&tmpmem);
		break;
	case ZL_EXP_FAT_ADDR_MEMBLK:
		tmpmem.runType = ZL_R_RDT_ADDR_MEMBLK;
		tmpmem.val.dword = retval->val.addr.index;
		tmpmem.val.memblock = retval->val.addr.memblock.ptr;
		tmpmem.memblk_Index = retval->val.addr.memblock.index;
		run->VMemBlockOps(VM_ARG,ZL_R_VMOPT_SETMEM_ADDR,(ZENGL_RUN_VIRTUAL_MEM_LIST *)memblock->ptr,(index - 1),&tmpmem);
		break;
	default:
		run->exit(VM_ARG,ZL_ERR_VM_API_INVALID_RETVAL_TYPE_WHEN_SETMEMBLOCK);
		return -1;
		break;
	}
	return 0;
}

/*ͨ���ַ���key������������ڴ��ĳ�Ա���ýӿڻ��Ƚ��ַ���תΪ��Ӧ������ֵ���ٵ���SetMemBlock�ӿ�ȥִ�о�������ù���*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetMemBlockByHashKey(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_MEMBLOCK * memblock,ZL_EXP_CHAR * key,ZENGL_EXPORT_MOD_FUN_ARG * retval)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZL_INT index;
	ZL_CHAR * ApiName = "zenglApi_SetMemBlockByHashKey";
	if(VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return -1;
	switch(VM->ApiState)
	{
	case ZL_API_ST_MOD_FUN_HANDLE:
		break;
	default:
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_CALL_POSITION, ApiName , ApiName);
		return -1;
		break;
	}
	if(retval == ZL_NULL)
		return 0;
	index = zenglrun_getIndexFromHashCodeTable(VM_ARG, (ZENGL_RUN_VIRTUAL_MEM_LIST *)memblock->ptr, key);
	return zenglApi_SetMemBlock(VM_ARG, memblock, (index + 1), retval);
}

/*��ȡ������ڴ���е�index - 1��������Ԫ�أ�index����Ϊ1��ʾ������ڴ��ĵ�һ��Ԫ�أ��Դ�����*/
ZL_EXPORT ZENGL_EXPORT_MOD_FUN_ARG zenglApi_GetMemBlock(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_MEMBLOCK * memblock,ZL_EXP_INT index)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run;
	ZENGL_RUN_VIRTUAL_MEM_STRUCT tmpmem = {0};
	ZENGL_EXPORT_MOD_FUN_ARG retval = {0};
	ZL_CHAR * ApiName = "zenglApi_GetMemBlock";
	if(VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
	{
		retval.type = ZL_EXP_FAT_INVALID;
		return retval;
	}
	switch(VM->ApiState)
	{
	case ZL_API_ST_MOD_FUN_HANDLE:
	case ZL_API_ST_DEBUG_HANDLE:
		break;
	default:
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_CALL_POSITION, ApiName , ApiName);
		retval.type = ZL_EXP_FAT_INVALID;
		return retval;
		break;
	}
	run = &VM->run;
	run->realloc_memblock(VM_ARG,(ZENGL_RUN_VIRTUAL_MEM_LIST *)memblock->ptr,index-1);
	tmpmem = run->VMemBlockOps(VM_ARG,ZL_R_VMOPT_GETMEM,(ZENGL_RUN_VIRTUAL_MEM_LIST *)memblock->ptr,index - 1,&tmpmem);
	switch(tmpmem.runType)
	{
	case ZL_R_RDT_NONE:
		retval.type = ZL_EXP_FAT_NONE;
		retval.val.integer = tmpmem.val.dword;
		break;
	case ZL_R_RDT_INT:
		retval.type = ZL_EXP_FAT_INT;
		retval.val.integer = tmpmem.val.dword;
		break;
	case ZL_R_RDT_FLOAT:
		retval.type = ZL_EXP_FAT_FLOAT;
		retval.val.floatnum = tmpmem.val.qword;
		break;
	case ZL_R_RDT_STR:
		retval.type = ZL_EXP_FAT_STR;
		retval.val.str = (ZL_CHAR *)tmpmem.val.str;
		break;
	case ZL_R_RDT_MEM_BLOCK:
		retval.type = ZL_EXP_FAT_MEMBLOCK;
		retval.val.memblock.ptr = tmpmem.val.memblock;
		retval.val.memblock.index = tmpmem.memblk_Index;
		break;
	case ZL_R_RDT_ADDR:
		retval.type = ZL_EXP_FAT_ADDR;
		retval.val.addr.index = tmpmem.val.dword;
		break;
	case ZL_R_RDT_ADDR_LOC:
		retval.type = ZL_EXP_FAT_ADDR_LOC;
		retval.val.addr.index = tmpmem.val.dword;
		break;
	case ZL_R_RDT_ADDR_MEMBLK:
		retval.type = ZL_EXP_FAT_ADDR_MEMBLK;
		retval.val.addr.index = tmpmem.val.dword;
		retval.val.addr.memblock.ptr = tmpmem.val.memblock;
		retval.val.addr.memblock.index = tmpmem.memblk_Index;
		break;
	}
	return retval;
}

/*����key����ȡ������ڴ���еĳ�Ա���˽ӿڻ��Ƚ�keyתΪ����ֵ���ٸ�������ֵ�������zenglApi_GetMemBlock�ӿ������ض�Ӧ�ĳ�Ա*/
ZL_EXPORT ZENGL_EXPORT_MOD_FUN_ARG zenglApi_GetMemBlockByHashKey(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_MEMBLOCK * memblock,ZL_EXP_CHAR * key)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_EXPORT_MOD_FUN_ARG retval = {0};
	ZL_INT index;
	ZL_CHAR * ApiName = "zenglApi_GetMemBlockByHashKey";
	if(VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
	{
		retval.type = ZL_EXP_FAT_INVALID;
		return retval;
	}
	switch(VM->ApiState)
	{
	case ZL_API_ST_MOD_FUN_HANDLE:
		break;
	default:
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_CALL_POSITION, ApiName , ApiName);
		retval.type = ZL_EXP_FAT_INVALID;
		return retval;
		break;
	}
	index = zenglrun_getIndexFromHashCodeTable(VM_ARG, (ZENGL_RUN_VIRTUAL_MEM_LIST *)memblock->ptr, key);
	return zenglApi_GetMemBlock(VM_ARG, memblock, index + 1);
}

/*��ȡ��argnum�����������͵���Ϣ��argnum��1��ʼ��ʾ��һ��������֮ǰ��GetFunArg����ֻ�ܻ�ȡ������ֵ��������������ã���ֱ�ӵݹ��ȡ���õı�����ֵ��
�����޷�֪��������������Ϣ�������޷�֪��ĳ�������Ƿ������õ����ͣ��ú�������Ի�ȡ����Щ��Ϣ��������������������ͣ��������������ȼ�*/
ZL_EXPORT ZL_EXP_INT zenglApi_GetFunArgInfo(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argnum,ZENGL_EXPORT_MOD_FUN_ARG * retval)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run;
	ZENGL_RUN_VIRTUAL_MEM_STRUCT tmpmem = {0};  //��ʱ�������ڴ������
	ZL_INT argcount;
	ZL_INT index;
	ZL_CHAR * ApiName = "zenglApi_GetFunArgInfo";
	if(VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return -1;
	switch(VM->ApiState)
	{
	case ZL_API_ST_MOD_FUN_HANDLE:
		break;
	default:
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_CALL_POSITION, ApiName , ApiName);
		return -1;
		break;
	}
	if(retval == ZL_NULL)
		return 0;
	run = &VM->run;
	argcount = ZENGL_RUN_REGVAL(ZL_R_RT_LOC).dword - ZENGL_RUN_REGVAL(ZL_R_RT_ARG).dword - 1;
	if(argnum < 1 || argnum > argcount)
		run->exit(VM_ARG,ZL_ERR_VM_API_GET_FUN_ARG_ARGNUM_IS_INVALID,ZL_R_REG_PC);
	index = ZENGL_RUN_REGVAL(ZL_R_RT_ARG).dword + argnum - 1;
	tmpmem = run->vstack_list.mem_array[index];
	switch(tmpmem.runType)
	{
	case ZL_R_RDT_NONE:
		retval->type = ZL_EXP_FAT_NONE;
		retval->val.integer = tmpmem.val.dword;
		break;
	case ZL_R_RDT_INT:
		retval->type = ZL_EXP_FAT_INT;
		retval->val.integer = tmpmem.val.dword;
		break;
	case ZL_R_RDT_FLOAT:
		retval->type = ZL_EXP_FAT_FLOAT;
		retval->val.floatnum = tmpmem.val.qword;
		break;
	case ZL_R_RDT_STR:
		retval->type = ZL_EXP_FAT_STR;
		retval->val.str = (ZL_CHAR *)tmpmem.val.str;
		break;
	case ZL_R_RDT_MEM_BLOCK: //�ڴ��
		retval->type = ZL_EXP_FAT_MEMBLOCK;
		retval->val.memblock.ptr = tmpmem.val.memblock;
		retval->val.memblock.index = tmpmem.memblk_Index;
		break;
	case ZL_R_RDT_ADDR:
		retval->type = ZL_EXP_FAT_ADDR;
		retval->val.addr.index = tmpmem.val.dword;
		break;
	case ZL_R_RDT_ADDR_LOC: //�ֲ���������
		retval->type = ZL_EXP_FAT_ADDR_LOC;
		retval->val.addr.index = tmpmem.val.dword;
		break;
	case ZL_R_RDT_ADDR_MEMBLK: //�ڴ������
		retval->type = ZL_EXP_FAT_ADDR_MEMBLK;
		retval->val.addr.index = tmpmem.val.dword;
		retval->val.addr.memblock.ptr = tmpmem.val.memblock;
		retval->val.addr.memblock.index = tmpmem.memblk_Index;
		break;
	}
	return 0;
}

/*���ýű�ģ�麯���е�argnum��������ֵ��argnum��1��ʼ��ʾ��һ������*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetFunArg(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argnum,ZENGL_EXPORT_MOD_FUN_ARG * setval)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run;
	ZENGL_RUN_VIRTUAL_MEM_STRUCT tmpmem = {0};  //��ʱ�������ڴ������
	ZL_INT argcount;
	ZL_INT index;
	ZL_CHAR * ApiName = "zenglApi_SetFunArg";
	if(VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return -1;
	switch(VM->ApiState)
	{
	case ZL_API_ST_MOD_FUN_HANDLE:
		break;
	default:
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_CALL_POSITION, ApiName , ApiName);
		return -1;
		break;
	}
	if(setval == ZL_NULL)
		return 0;
	run = &VM->run;
	argcount = ZENGL_RUN_REGVAL(ZL_R_RT_LOC).dword - ZENGL_RUN_REGVAL(ZL_R_RT_ARG).dword - 1;
	if(argnum < 1 || argnum > argcount)
		run->exit(VM_ARG,ZL_ERR_VM_API_INVALID_ARGNUM_ARG_IN_SET_FUN_ARG);
	index = ZENGL_RUN_REGVAL(ZL_R_RT_ARG).dword + argnum - 1;
	switch(setval->type)
	{
	case ZL_EXP_FAT_NONE:
		tmpmem.runType = ZL_R_RDT_NONE;
		tmpmem.val.dword = setval->val.integer;
		run->VStackListOps(VM_ARG,ZL_R_VMOPT_SETMEM_NONE,index,tmpmem,ZL_TRUE);
		break;
	case ZL_EXP_FAT_INT:
		tmpmem.runType = ZL_R_RDT_INT;
		tmpmem.val.dword = setval->val.integer;
		run->VStackListOps(VM_ARG,ZL_R_VMOPT_SETMEM_INT,index,tmpmem,ZL_TRUE);
		break;
	case ZL_EXP_FAT_FLOAT:
		tmpmem.runType = ZL_R_RDT_FLOAT;
		tmpmem.val.qword = setval->val.floatnum;
		run->VStackListOps(VM_ARG,ZL_R_VMOPT_SETMEM_DOUBLE,index,tmpmem,ZL_TRUE);
		break;
	case ZL_EXP_FAT_STR:
		tmpmem.runType = ZL_R_RDT_STR;
		tmpmem.val.str = (ZL_VOID *)setval->val.str;
		run->VStackListOps(VM_ARG,ZL_R_VMOPT_SETMEM_STR,index,tmpmem,ZL_TRUE);
		break;
	case ZL_EXP_FAT_MEMBLOCK:
		tmpmem.runType = ZL_R_RDT_MEM_BLOCK;
		tmpmem.val.memblock = setval->val.memblock.ptr;
		tmpmem.memblk_Index = setval->val.memblock.index;
		run->VStackListOps(VM_ARG,ZL_R_VMOPT_SETMEM_MEMBLOCK,index,tmpmem,ZL_TRUE);
		break;
	case ZL_EXP_FAT_ADDR:
		tmpmem.runType = ZL_R_RDT_ADDR;
		tmpmem.val.dword = setval->val.addr.index;
		run->VStackListOps(VM_ARG,ZL_R_VMOPT_SETMEM_ADDR,index,tmpmem,ZL_TRUE);
		break;
	case ZL_EXP_FAT_ADDR_LOC:
		tmpmem.runType = ZL_R_RDT_ADDR_LOC;
		tmpmem.val.dword = setval->val.addr.index;
		run->VStackListOps(VM_ARG,ZL_R_VMOPT_SETMEM_ADDR,index,tmpmem,ZL_TRUE);
		break;
	case ZL_EXP_FAT_ADDR_MEMBLK:
		tmpmem.runType = ZL_R_RDT_ADDR_MEMBLK;
		tmpmem.val.dword = setval->val.addr.index;
		tmpmem.val.memblock = setval->val.addr.memblock.ptr;
		tmpmem.memblk_Index = setval->val.addr.memblock.index;
		run->VStackListOps(VM_ARG,ZL_R_VMOPT_SETMEM_ADDR,index,tmpmem,ZL_TRUE);
		break;
	}
	return 0;
}

/*�ýӿ�ΪzenglApi_SetFunArg����չ������������addr_levelС��0ʱ��zenglApi_SetFunArgEx�ȼ���zenglApi_SetFunArg��
  ��addr_level���ڵ���0ʱ���Ϳ��ԶԲ�ͬ��������ñ���������ò���*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetFunArgEx(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argnum,ZENGL_EXPORT_MOD_FUN_ARG * setval,ZL_EXP_INT addr_level)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run;
	ZENGL_RUN_VIRTUAL_MEM_STRUCT tmpmem = {0};  //��ʱ�������ڴ������
	ZENGL_RUN_VIRTUAL_MEM_LIST * tmp_mblk, * tmp_mblk_new; //��ʱ���ڴ��ָ��
	ZL_INT argcount;
	ZL_INT index,tmpindex,tmpindex_new;
	ZL_CHAR * ApiName = "zenglApi_SetFunArgEx";
	if(VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return -1;
	switch(VM->ApiState)
	{
	case ZL_API_ST_MOD_FUN_HANDLE:
		break;
	default:
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_CALL_POSITION, ApiName , ApiName);
		return -1;
		break;
	}
	if(setval == ZL_NULL)
		return 0;
	run = &VM->run;
	argcount = ZENGL_RUN_REGVAL(ZL_R_RT_LOC).dword - ZENGL_RUN_REGVAL(ZL_R_RT_ARG).dword - 1;
	if(argnum < 1 || argnum > argcount)
		run->exit(VM_ARG,ZL_ERR_VM_API_INVALID_ARGNUM_ARG_IN_SET_FUN_ARG);
	tmpindex = index = ZENGL_RUN_REGVAL(ZL_R_RT_ARG).dword + argnum - 1;
	tmp_mblk = &run->vstack_list;
	/*
	�����forѭ�����������ü���addr_level����Ҫ������������ΪZL_R_RDT_NONEδ��ʼ��״̬��
	���������VStackListOps֮��ĺ����ڵݹ�����ʱ�Ϳ��Զ��ض���������ý������ò�����
	���� a=&b; c=&a; ��c��Ϊ����ͨ��ģ�麯�����ݸ�zenglApi_SetFunArgEx�ӿڣ�
	��ô��addr_levelΪ0ʱ�ͻὫc��������Ϊδ��ʼ��״̬��addr_levelΪ1ʱ�ͽ�c���õ�a����Ϊδ��ʼ��״̬��
	addr_levelΪ2ʱ�����a���õ�b�����õĻ������ã����b�Ƿ��������;�����������ò���
	*/
	for(;addr_level >= 0;addr_level--)
	{
		if(addr_level == 0)
		{
			switch(tmp_mblk->mem_array[tmpindex].runType)
			{
			case ZL_R_RDT_ADDR:
			case ZL_R_RDT_ADDR_LOC:
			case ZL_R_RDT_ADDR_MEMBLK:
				tmp_mblk->mem_array[tmpindex].runType = ZL_R_RDT_NONE;
				tmp_mblk->mem_array[tmpindex].val.dword = 0;
				tmp_mblk->mem_array[tmpindex].val.memblock = ZL_NULL;
				tmp_mblk->mem_array[tmpindex].memblk_Index = 0;
				break;
			}
		}
		else //addr_level����0ʱ��������һ����������Ϣ
		{
			switch(tmp_mblk->mem_array[tmpindex].runType)
			{
			case ZL_R_RDT_ADDR:
				tmpindex = tmp_mblk->mem_array[tmpindex].val.dword;
				tmp_mblk = &run->vmem_list;
				break;
			case ZL_R_RDT_ADDR_LOC:
				tmpindex = tmp_mblk->mem_array[tmpindex].val.dword;
				tmp_mblk = &run->vstack_list;
				break;
			case ZL_R_RDT_ADDR_MEMBLK:
				tmpindex_new = tmp_mblk->mem_array[tmpindex].val.dword;
				tmp_mblk_new = (ZENGL_RUN_VIRTUAL_MEM_LIST *)(tmp_mblk->mem_array[tmpindex].val.memblock);
				tmpindex = tmpindex_new;
				tmp_mblk = tmp_mblk_new;
				break;
			default:
				addr_level = 0;
				break;
			}
		}
	}
	switch(setval->type)
	{
	case ZL_EXP_FAT_NONE:
		tmpmem.runType = ZL_R_RDT_NONE;
		tmpmem.val.dword = setval->val.integer;
		run->VStackListOps(VM_ARG,ZL_R_VMOPT_SETMEM_NONE,index,tmpmem,ZL_TRUE);
		break;
	case ZL_EXP_FAT_INT:
		tmpmem.runType = ZL_R_RDT_INT;
		tmpmem.val.dword = setval->val.integer;
		run->VStackListOps(VM_ARG,ZL_R_VMOPT_SETMEM_INT,index,tmpmem,ZL_TRUE);
		break;
	case ZL_EXP_FAT_FLOAT:
		tmpmem.runType = ZL_R_RDT_FLOAT;
		tmpmem.val.qword = setval->val.floatnum;
		run->VStackListOps(VM_ARG,ZL_R_VMOPT_SETMEM_DOUBLE,index,tmpmem,ZL_TRUE);
		break;
	case ZL_EXP_FAT_STR:
		tmpmem.runType = ZL_R_RDT_STR;
		tmpmem.val.str = (ZL_VOID *)setval->val.str;
		run->VStackListOps(VM_ARG,ZL_R_VMOPT_SETMEM_STR,index,tmpmem,ZL_TRUE);
		break;
	case ZL_EXP_FAT_MEMBLOCK:
		tmpmem.runType = ZL_R_RDT_MEM_BLOCK;
		tmpmem.val.memblock = setval->val.memblock.ptr;
		tmpmem.memblk_Index = setval->val.memblock.index;
		run->VStackListOps(VM_ARG,ZL_R_VMOPT_SETMEM_MEMBLOCK,index,tmpmem,ZL_TRUE);
		break;
	case ZL_EXP_FAT_ADDR:
		tmpmem.runType = ZL_R_RDT_ADDR;
		tmpmem.val.dword = setval->val.addr.index;
		run->VStackListOps(VM_ARG,ZL_R_VMOPT_SETMEM_ADDR,index,tmpmem,ZL_TRUE);
		break;
	case ZL_EXP_FAT_ADDR_LOC:
		tmpmem.runType = ZL_R_RDT_ADDR_LOC;
		tmpmem.val.dword = setval->val.addr.index;
		run->VStackListOps(VM_ARG,ZL_R_VMOPT_SETMEM_ADDR,index,tmpmem,ZL_TRUE);
		break;
	case ZL_EXP_FAT_ADDR_MEMBLK:
		tmpmem.runType = ZL_R_RDT_ADDR_MEMBLK;
		tmpmem.val.dword = setval->val.addr.index;
		tmpmem.val.memblock = setval->val.addr.memblock.ptr;
		tmpmem.memblk_Index = setval->val.addr.memblock.index;
		run->VStackListOps(VM_ARG,ZL_R_VMOPT_SETMEM_ADDR,index,tmpmem,ZL_TRUE);
		break;
	}
	return 0;
}

/*��ȡ�ڴ���size��������Ϣ*/
ZL_EXPORT ZL_EXP_INT zenglApi_GetMemBlockInfo(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_MEMBLOCK * memblock,ZL_EXP_INT * mblk_size,ZL_EXP_INT * mblk_count)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run;
	ZENGL_RUN_VIRTUAL_MEM_LIST * tmpMemBlock;
	ZL_CHAR * ApiName = "zenglApi_GetMemBlockInfo";
	if(VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return -1;
	switch(VM->ApiState)
	{
	case ZL_API_ST_MOD_FUN_HANDLE:
	case ZL_API_ST_DEBUG_HANDLE:
		break;
	default:
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_CALL_POSITION, ApiName , ApiName);
		return -1;
		break;
	}
	run = &VM->run;
	if(memblock == ZL_NULL)
		run->exit(VM_ARG,ZL_ERR_VM_API_INVALID_MEMBLOCK_ARG_IN_GET_MEM_BLOCK_SIZE);
	tmpMemBlock = (ZENGL_RUN_VIRTUAL_MEM_LIST *)memblock->ptr;
	if(mblk_size != ZL_NULL)
		(*mblk_size) = tmpMemBlock->size;
	if(mblk_count != ZL_NULL)
		(*mblk_count) = tmpMemBlock->count;
	return 0;
}

/*��ȡ�ڴ���з�NONE��Ա������*/
ZL_EXPORT ZL_EXP_INT zenglApi_GetMemBlockNNCount(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_MEMBLOCK * memblock)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run;
	ZENGL_RUN_VIRTUAL_MEM_LIST * tmpMemBlock;
	ZL_CHAR * ApiName = "zenglApi_GetMemBlockNNCount";
	if(VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return -1;
	switch(VM->ApiState)
	{
	case ZL_API_ST_MOD_FUN_HANDLE:
	case ZL_API_ST_DEBUG_HANDLE:
		break;
	default:
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_CALL_POSITION, ApiName , ApiName);
		return -1;
		break;
	}
	run = &VM->run;
	if(memblock == ZL_NULL)
		run->exit(VM_ARG,ZL_ERR_VM_API_INVALID_MEMBLOCK_ARG_IN_GET_MEM_BLOCK_NNCOUNT);
	tmpMemBlock = (ZENGL_RUN_VIRTUAL_MEM_LIST *)memblock->ptr;
	return tmpMemBlock->nncount;
}

/*��������ֵ���ӹ�ϣ���л�ȡ��Ӧ���ַ���key*/
ZL_EXPORT ZL_EXP_INT zenglApi_GetMemBlockHashKey(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_MEMBLOCK * memblock, ZL_EXP_INT memblock_index, ZL_EXP_CHAR ** key)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZL_CHAR * ApiName = "zenglApi_GetMemBlockHashKey";
	if(VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return -1;
	switch(VM->ApiState)
	{
	case ZL_API_ST_MOD_FUN_HANDLE:
	case ZL_API_ST_DEBUG_HANDLE:
		break;
	default:
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_CALL_POSITION, ApiName , ApiName);
		return -1;
		break;
	}
	if(memblock == ZL_NULL || memblock_index < 0 || key == ZL_NULL)
	{
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_ARG, ApiName , ApiName);
		return -1;
	}
	(*key) = zenglrun_getKeyFromHashCodeTable(VM_ARG, (ZENGL_RUN_VIRTUAL_MEM_LIST *)memblock->ptr, memblock_index);
	return 0;
}

/*API�ӿڣ��û�����ͨ���˽ӿ�����;ֹͣ�ű�*/
ZL_EXPORT ZL_EXP_INT zenglApi_Stop(ZL_EXP_VOID * VM_ARG)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run;
	ZL_CHAR * ApiName = "zenglApi_Stop";
	if(VM_ARG == ZL_NULL || VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return -1;
	switch(VM->ApiState)
	{
	case ZL_API_ST_AFTER_RUN:
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_CALL_POSITION, ApiName , ApiName);
		return -1;
		break;
	}
	run = &VM->run;
	if(run->isDestroyed)
		return 0;
	if(run->isinRunning)
	{
		run->isUserWantStop = ZL_TRUE;
	}
	return 0;
}

/*API�ӿڣ��û�����ͨ���˽ӿ�����һЩ�������ݵ�ָ��*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetExtraData(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * extraDataName,ZL_EXP_VOID * point)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run;
	ZL_INT h;
	ZL_INT tmpindex;
	ZL_CHAR * ApiName = "zenglApi_SetExtraData";
	if(VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return -1;
	switch(VM->ApiState)
	{
	case ZL_API_ST_AFTER_RUN:
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_CALL_POSITION, ApiName , ApiName);
		return -1;
		break;
	}
	run = &VM->run;
	h = run->Hash(VM_ARG,extraDataName) + ZL_R_HASH_SIZE * ZL_R_HASH_TYPE_EXTRA_DATA_TABLE;
	tmpindex = run->HashTable[h];
	if(run->mempool.isInit == ZL_FALSE)
	{
		run->init(VM_ARG);
	}
	if(run->isinRunning == ZL_FALSE)
	{
		ZL_INT retcode;
		if((retcode = ZENGL_SYS_JMP_SETJMP(run->jumpBuffer)) == 0)
			;
		else if(retcode == 1)
		{
			ZENGL_SYS_MEM_SET(run->jumpBuffer,0,sizeof(run->jumpBuffer));
			return 0;
		}
		else if(retcode == -1)
		{
			VM->isRunError = ZL_TRUE;
			ZENGL_SYS_MEM_SET(run->jumpBuffer,0,sizeof(run->jumpBuffer));
			return -1;
		}
	}
	while(tmpindex != 0 && run->ExtraDataTable.extras[tmpindex].isvalid == ZL_TRUE && 
		ZENGL_SYS_STRCMP(extraDataName,run->InstDataStringPoolGetPtr(VM_ARG,run->ExtraDataTable.extras[tmpindex].strIndex)) != 0)
		tmpindex = run->ExtraDataTable.extras[tmpindex].next;
	if(tmpindex == 0)
	{
		tmpindex = run->HashTable[h];
		run->HashTable[h] = run->InsertExtraDataTable(VM_ARG,extraDataName,point);
		run->ExtraDataTable.extras[run->HashTable[h]].next = tmpindex;
		return 0;
	}
	else if(run->ExtraDataTable.extras[tmpindex].isvalid == ZL_FALSE)
		run->exit(VM_ARG,ZL_ERR_VM_API_EXTRA_DATA_TABLE_INVALID_INDEX);
	else
	{
		if(VM->compile.isReUse == ZL_TRUE) //����������£�ֱ�ӷ���
			return 0;
		else
			run->exit(VM_ARG,ZL_ERR_VM_API_EXTRA_DATA_NAME_EXIST,extraDataName,extraDataName);
	}
	return 0;
}

/*API�ӿڣ��û�����ͨ���˽ӿڵõ���������*/
ZL_EXPORT ZL_EXP_VOID * zenglApi_GetExtraData(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * extraDataName)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run;
	ZL_INT h;
	ZL_INT tmpindex;
	ZL_CHAR * ApiName = "zenglApi_GetExtraData";
	if(VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return ZL_NULL;
	switch(VM->ApiState)
	{
	case ZL_API_ST_OPEN:
	case ZL_API_ST_RESET:
	case ZL_API_ST_AFTER_RUN:
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_CALL_POSITION, ApiName , ApiName);
		return ZL_NULL;
		break;
	}
	run = &VM->run;
	h = run->Hash(VM_ARG,extraDataName) + ZL_R_HASH_SIZE * ZL_R_HASH_TYPE_EXTRA_DATA_TABLE;
	tmpindex = run->HashTable[h];
	while(tmpindex != 0 && run->ExtraDataTable.extras[tmpindex].isvalid == ZL_TRUE && 
		ZENGL_SYS_STRCMP(extraDataName,run->InstDataStringPoolGetPtr(VM_ARG,run->ExtraDataTable.extras[tmpindex].strIndex)) != 0)
		tmpindex = run->ExtraDataTable.extras[tmpindex].next;
	if(tmpindex == 0)
		run->exit(VM_ARG,ZL_ERR_VM_API_EXTRA_DATA_NAME_INVALID,extraDataName,extraDataName);
	else if(run->ExtraDataTable.extras[tmpindex].isvalid == ZL_FALSE)
		run->exit(VM_ARG,ZL_ERR_VM_API_EXTRA_DATA_TABLE_INVALID_INDEX);
	else
		return run->ExtraDataTable.extras[tmpindex].point;
	return ZL_NULL;
}

/*API�ӿڣ��û�����ͨ���˽ӿڵõ�����������ݣ���Ϊ�����GetExtraData����չ��������������extraDataName��������ʱ������NULL�������ǲ���������Ϣ�˳��������
  ����Ҳ����ͬʱ�����������Ƿ����*/
ZL_EXPORT ZL_EXP_VOID * zenglApi_GetExtraDataEx(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * extraDataName)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run;
	ZL_INT h;
	ZL_INT tmpindex;
	ZL_CHAR * ApiName = "zenglApi_GetExtraDataEx";
	if(VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return ZL_NULL;
	switch(VM->ApiState)
	{
	case ZL_API_ST_OPEN:
	case ZL_API_ST_RESET:
	//case ZL_API_ST_AFTER_RUN: //�˽ӿ�ע�͵�������˵��������zenglApi_Run֮��ĺ�����������������ٻ�ȡ��صĶ������ݡ�
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_CALL_POSITION, ApiName , ApiName);
		return ZL_NULL;
		break;
	}
	run = &VM->run;
	h = run->Hash(VM_ARG,extraDataName) + ZL_R_HASH_SIZE * ZL_R_HASH_TYPE_EXTRA_DATA_TABLE;
	tmpindex = run->HashTable[h];
	while(tmpindex != 0 && run->ExtraDataTable.extras[tmpindex].isvalid == ZL_TRUE && 
		ZENGL_SYS_STRCMP(extraDataName,run->InstDataStringPoolGetPtr(VM_ARG,run->ExtraDataTable.extras[tmpindex].strIndex)) != 0)
		tmpindex = run->ExtraDataTable.extras[tmpindex].next;
	if(tmpindex == 0) //��������ڸö������ݣ��ͷ���NULL�������������󣬿������ڼ����������Ƿ����
		return ZL_NULL;
	else if(run->ExtraDataTable.extras[tmpindex].isvalid == ZL_FALSE)
		return ZL_NULL;
	else
		return run->ExtraDataTable.extras[tmpindex].point;
	return ZL_NULL;
}

/*API�ӿڣ��û�ͨ���˽ӿ����ýű�Դ�����XOR��ͨ������������Կ*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetSourceXorKey(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * xor_key_str)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZL_CHAR * ApiName = "zenglApi_SetSourceXorKey";
	if(VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return -1;
	switch(VM->ApiState)
	{
	case ZL_API_ST_OPEN:
	case ZL_API_ST_RESET:
		break;
	default:
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_CALL_POSITION, ApiName , ApiName);
		return -1;
		break;
	}
	VM->initEncrypt.xor.xor_key_str =  xor_key_str;
	VM->initEncrypt.xor.xor_key_len = ZENGL_SYS_STRLEN(xor_key_str);
	VM->initEncrypt.xor.xor_key_cur = 0;
	VM->initEncrypt.type = ZL_ENC_TYPE_XOR;
	return 0;
}

/*API�ӿڣ��û�ͨ���˽ӿ����ýű�Դ�����RC4������Կ*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetSourceRC4Key(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * rc4_key_str,ZL_EXP_INT rc4_key_len)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZL_CHAR * ApiName = "zenglApi_SetSourceRC4Key";
	if(VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return -1;
	switch(VM->ApiState)
	{
	case ZL_API_ST_OPEN:
	case ZL_API_ST_RESET:
		break;
	default:
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_CALL_POSITION, ApiName , ApiName);
		return -1;
		break;
	}
	VM->rc4InitState(VM_ARG,rc4_key_str,rc4_key_len);
	VM->initEncrypt.type = ZL_ENC_TYPE_RC4;
	return 0;
}

/*API�ӿڣ��û�ͨ���˽ӿڽ��ַ���������������У�������C++�оͿ�����ǰ��Դ�ַ�����Դ���ֶ��ͷŵ�����������������е��·������Դ����ڽ���ʱ�Զ��ͷŵ���
  ��ֹ�ڴ�й©*/
ZL_EXPORT ZL_EXP_CHAR * zenglApi_AllocMemForString(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * src_str)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run;
	ZL_CHAR * ret_str = ZL_NULL;
	ZL_INT tmpIndex = 0,src_len = 0;
	ZENGL_SYS_ARG_LIST arg;
	ZENGL_SYS_ARG_END(arg); //��va_end����arg��Ϊ0������64λgcc�±���
	if(VM_ARG == ZL_NULL || VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return ZL_NULL;
	run = &VM->run;
	if(src_str == ZL_NULL)
	{
		VM->run.SetApiError(VM_ARG,ZL_ERR_VM_API_INVALID_SRC_WHEN_ALLOCMEM_FOR_STRING,arg);
		return ZL_NULL;
	}
	src_len = ZENGL_SYS_STRLEN(src_str);
	if(src_len <= 0)
	{
		VM->run.SetApiError(VM_ARG,ZL_ERR_VM_API_INVALID_SRC_WHEN_ALLOCMEM_FOR_STRING,arg);
		return ZL_NULL;
	}
	ret_str = (ZL_CHAR *)run->memAlloc(VM_ARG,src_len + 1,&tmpIndex);
	ZENGL_SYS_STRNCPY(ret_str,src_str,src_len);
	ret_str[src_len] = ZL_STRNULL;
	return ret_str;
}

/*API�ӿڣ��û�ͨ���˽ӿ���������з���һ���ڴ�ռ�*/
ZL_EXPORT ZL_EXP_VOID * zenglApi_AllocMem(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT size)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run;
	ZL_VOID * retptr = ZL_NULL;
	ZL_INT tmpIndex = 0;
	ZENGL_SYS_ARG_LIST arg;
	ZENGL_SYS_ARG_END(arg); //��va_end����arg��Ϊ0������64λgcc�±���
	if(VM_ARG == ZL_NULL || VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return ZL_NULL;
	run = &VM->run;
	if(size <= 0)
	{
		VM->run.SetApiError(VM_ARG,ZL_ERR_VM_API_INVALID_SIZE_WHEN_ALLOCMEM,arg);
		return ZL_NULL;
	}
	retptr = run->memAlloc(VM_ARG,size,&tmpIndex);
	return retptr;
}

/*API�ӿڣ���AllocMem�������Դ�ֶ��ͷŵ�����ֹ��ԴԽ��Խ��*/
ZL_EXPORT ZL_EXP_INT zenglApi_FreeMem(ZL_EXP_VOID * VM_ARG,ZL_EXP_VOID * ptr)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run;
	ZENGL_SYS_ARG_LIST arg;
	ZENGL_SYS_ARG_END(arg); //��va_end����arg��Ϊ0������64λgcc�±���
	if(VM_ARG == ZL_NULL || VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return -1;
	run = &VM->run;
	if(ptr == ZL_NULL)
	{
		VM->run.SetApiError(VM_ARG,ZL_ERR_VM_API_INVALID_PTR_WHEN_FREEMEM,arg);
		return -1;
	}
	run->memFreeOnce(VM_ARG,ptr);
	return 0;
}

/*API�ӿڣ���ptrָ��Ĵ�С����Ϊsize�ߴ�*/
ZL_EXPORT ZL_EXP_VOID * zenglApi_ReAllocMem(ZL_EXP_VOID * VM_ARG,ZL_EXP_VOID * ptr,ZL_EXP_INT size)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run;
	ZL_INT index;

	if(VM_ARG == ZL_NULL || VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return ZL_NULL;
	run = &VM->run;
	if(ptr == ZL_NULL)
	{
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_PTR_WHEN_REALLOC);
		return ZL_NULL;
	}
	index = run->memFindPtrIndex(VM_ARG,ptr);
	if(index == -1)
	{
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_PTR_WHEN_REALLOC);
		return ZL_NULL;
	}
	return run->memReAlloc(VM_ARG,ptr,size,&index);
}

/*API�ӿڣ���fileName��������ڵ�ǰ�ű�������·����Ϣ�����ɵ�·����Ϣ��ŵ��û��ṩ��destPathBuffer��������*/
ZL_EXPORT ZL_EXP_INT zenglApi_makePathFileName(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * fileName,ZL_EXP_CHAR * destPathBuffer,ZL_EXP_INT bufferSize)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_COMPILE_TYPE * compile;
	ZENGL_RUN_TYPE * run;
	ZENGL_AST_NODE_TYPE * nodes;
	ZL_EXP_CHAR * sourceFileName;
	ZL_INT i,len;
	ZL_INT cpylen=-1;
	ZL_CHAR * ApiName = "zenglApi_makePathFileName";
	if(VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return -1;
	switch(VM->ApiState)
	{
	case ZL_API_ST_OPEN:
	case ZL_API_ST_RESET:
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_CALL_POSITION, ApiName , ApiName);
		return -1;
		break;
	}
	if(fileName == ZL_NULL || destPathBuffer == ZL_NULL || bufferSize <= 0)
	{
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_ARG, ApiName , ApiName);
		return -1;
	}
	compile = &VM->compile;
	run = &VM->run;
	nodes = compile->AST_nodes.nodes;
	sourceFileName = nodes[ZL_R_CUR_INST.nodenum].filename;
	len = ZENGL_SYS_STRLEN(sourceFileName);
	for(i=len-1;i>=0;i--)
	{
		if(sourceFileName[i] == '/')
		{
			cpylen = i+1;
			break;
		}
	}
	len = ZENGL_SYS_STRLEN(fileName);
	if(cpylen > 0)
	{
		if(cpylen >= bufferSize)
		{
			VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_FILENAME_TOO_LONG,sourceFileName);
			return -1;
		}
		ZENGL_SYS_STRNCPY(destPathBuffer,sourceFileName,cpylen);
		if(cpylen + len >= bufferSize)
		{
			VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_FILENAME_TOO_LONG_WHEN_MERGE,sourceFileName,fileName);
			return -1;
		}
		ZENGL_SYS_STRNCPY(destPathBuffer + cpylen,fileName,len);
		destPathBuffer[cpylen + len] = ZL_STRNULL;
	}
	else
	{
		if(len >= bufferSize)
		{
			VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_FILENAME_TOO_LONG,fileName);
			return -1;
		}
		ZENGL_SYS_STRNCPY(destPathBuffer,fileName,len);
		destPathBuffer[len] = ZL_STRNULL;
	}
	return 0;
}

/*API�ӿڣ���ȡ��ǰ����ģ�麯�����û��Զ�������*/
ZL_EXPORT ZL_EXP_INT zenglApi_GetModFunName(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR ** modfun_name)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run;
	ZL_CHAR * ApiName = "zenglApi_GetModFunName";
	if(VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return -1;
	switch(VM->ApiState)
	{
	case ZL_API_ST_MOD_FUN_HANDLE:
		break;
	default:
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_CALL_POSITION, ApiName , ApiName);
		return -1;
		break;
	}
	run = &VM->run;
	(*modfun_name) = run->InstDataStringPoolGetPtr(VM_ARG,run->ModFunTable.mod_funs[run->CurRunModFunIndex].strIndex);
	return 0;
}

/*API�ӿڣ����Խӿ�*/
ZL_EXPORT ZL_EXP_INT zenglApi_Debug(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * debug_str)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_VM_TYPE * DebugVM = ZL_NULL;
	ZL_INT retcode;
	ZL_CHAR * ApiName = "zenglApi_Debug";
	if(VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return -1;
	switch(VM->ApiState)
	{
	case ZL_API_ST_OPEN:
	case ZL_API_ST_RESET:
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_CALL_POSITION, ApiName , ApiName);
		return -1;
		break;
	}
	if(VM->debug.DeubugVM == ZL_NULL)
	{
		DebugVM = VM->debug.DeubugVM = (ZENGL_VM_TYPE *)zenglApi_Open();
		if(DebugVM == ZL_NULL)
		{
			VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_DEBUGVM_OPEN_FAILED, ApiName , ApiName);
			return -1;
		}
		//���õ���������ʱ��Ҫ�Ĵ����ڸ�������е���Դ
		ZENGL_SYS_MEM_COPY(&DebugVM->compile.HashTable,&VM->compile.HashTable,ZL_SYM_HASH_TOTAL_SIZE * sizeof(ZL_INT));
		DebugVM->compile.def_table = VM->compile.def_table;
		DebugVM->compile.SymGlobalTable = VM->compile.SymGlobalTable;
		DebugVM->compile.SymLocalTable = VM->compile.SymLocalTable;
		DebugVM->compile.SymClassTable = VM->compile.SymClassTable;
		DebugVM->compile.SymClassMemberTable = VM->compile.SymClassMemberTable;
		//����������еĲ�ѯ������Ϣ�ĺ����ض��򵽵������Զ���ĺ���
		DebugVM->compile.ReplaceDefConst = DebugVM->debug.ReplaceDefConst;
		DebugVM->compile.lookupDefTable = DebugVM->debug.lookupDefTable;
		DebugVM->compile.SymLookupID = DebugVM->debug.SymLookupID;
		DebugVM->compile.SymLookupID_ForDot = DebugVM->debug.SymLookupID_ForDot;
		DebugVM->compile.SymLookupClass = DebugVM->debug.SymLookupClass;
		DebugVM->compile.SymLookupClassMember = DebugVM->debug.SymLookupClassMember;
		DebugVM->run.LookupModFunTable = DebugVM->debug.LookupModFunTable;
	}
	if(debug_str == ZL_NULL)
	{
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_DEBUG_STR, ApiName , ApiName);
		return -1;
	}
	DebugVM->debug.DeubugPVM = VM;
	ZENGL_SYS_MEM_COPY(DebugVM->vm_main_args,VM->vm_main_args,sizeof(ZENGL_EXPORT_VM_MAIN_ARGS));
	if(VM->debug.output_debug_info == ZL_FALSE)
		DebugVM->vm_main_args->flags &= ~(ZL_EXP_CP_AF_OUTPUT_DEBUG_INFO);
	else
		DebugVM->vm_main_args->flags |= (ZL_EXP_CP_AF_IN_DEBUG_MODE | ZL_EXP_CP_AF_OUTPUT_DEBUG_INFO);
	DebugVM->isinApiRun = ZL_TRUE;
	if(DebugVM->run.mempool.isInit == ZL_FALSE)
		DebugVM->run.init(DebugVM); //����������Ҫ�Խ�����������ָ������ڴ˳�ʼ��������
	DebugVM->compile.source.run_str = debug_str;
	DebugVM->compile.source.run_str_len = ZENGL_SYS_STRLEN(debug_str);
	DebugVM->debug.SetFunInfo(DebugVM); //���õ������ڵĺ������ຯ������
	DebugVM->compile.AsmGCStackPush(DebugVM,DebugVM->compile.SymClassTable.global_classid,ZL_ASM_STACK_ENUM_FUN_CLASSID);
	retcode = DebugVM->debug.Compile(DebugVM,ApiName,DebugVM->vm_main_args);
	DebugVM->compile.AsmGCStackPop(DebugVM,ZL_ASM_STACK_ENUM_FUN_CLASSID,ZL_TRUE);
	if(retcode == 0) //�������ɹ�������������
	{
		DebugVM->ApiState = ZL_API_ST_RUN; //����ΪRUN״̬
		DebugVM->debug.Run(DebugVM);
		DebugVM->ApiState = ZL_API_ST_AFTER_RUN; //����ΪAFTER_RUN״̬
	}
	if(retcode == -1)
	{
		VM->run.SetApiErrorEx(VM_ARG, ZL_ERR_VM_API_DEBUG_ERR, zenglApi_GetErrorString(DebugVM));
		zenglApi_Close(DebugVM);
		VM->debug.DeubugVM = ZL_NULL;
		return -1;
	}
	zenglApi_Close(DebugVM);
	VM->debug.DeubugVM = ZL_NULL;
	return 0;
}

/*API�ӿڣ���ȡ���ԼĴ�����ĵ��Խ��*/
ZL_EXPORT ZL_EXP_INT zenglApi_GetDebug(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_MOD_FUN_ARG * retval)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run;
	ZL_CHAR * ApiName = "zenglApi_GetDebug";
	if(VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return -1;
	switch(VM->ApiState)
	{
	case ZL_API_ST_OPEN:
	case ZL_API_ST_RESET:
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_CALL_POSITION, ApiName , ApiName);
		return -1;
		break;
	}
	run = &VM->run;
	if(retval == ZL_NULL)
		return 0;
	switch(run->reg_list[ZL_R_RT_DEBUG].runType)
	{
	case ZL_R_RDT_NONE:
		retval->type = ZL_EXP_FAT_NONE;
		retval->val.integer = run->reg_list[ZL_R_RT_DEBUG].val.dword;
		break;
	case ZL_R_RDT_INT:
		retval->type = ZL_EXP_FAT_INT;
		retval->val.integer = run->reg_list[ZL_R_RT_DEBUG].val.dword;
		break;
	case ZL_R_RDT_FLOAT:
		retval->type = ZL_EXP_FAT_FLOAT;
		retval->val.floatnum = run->reg_list[ZL_R_RT_DEBUG].val.qword;
		break;
	case ZL_R_RDT_STR:
		retval->type = ZL_EXP_FAT_STR;
		retval->val.str = (ZL_CHAR *)run->reg_list[ZL_R_RT_DEBUG].val.str;
		break;
	case ZL_R_RDT_MEM_BLOCK: //�ڴ��
		retval->type = ZL_EXP_FAT_MEMBLOCK;
		retval->val.memblock.ptr = run->reg_list[ZL_R_RT_DEBUG].val.memblock;
		retval->val.memblock.index = run->reg_list[ZL_R_RT_DEBUG].memblk_Index;
		break;
	case ZL_R_RDT_ADDR: //ȫ�ֱ�������
		retval->type = ZL_EXP_FAT_ADDR;
		retval->val.addr.index = run->reg_list[ZL_R_RT_DEBUG].val.dword;
		break;
	case ZL_R_RDT_ADDR_LOC: //�ֲ���������
		retval->type = ZL_EXP_FAT_ADDR_LOC;
		retval->val.addr.index = run->reg_list[ZL_R_RT_DEBUG].val.dword;
		break;
	case ZL_R_RDT_ADDR_MEMBLK: //�ڴ������
		retval->type = ZL_EXP_FAT_ADDR_MEMBLK;
		retval->val.addr.index = run->reg_list[ZL_R_RT_DEBUG].val.dword;
		retval->val.addr.memblock.ptr = run->reg_list[ZL_R_RT_DEBUG].val.memblock;
		retval->val.addr.memblock.index = run->reg_list[ZL_R_RT_DEBUG].memblk_Index;
		break;
	}
	return 0;
}

/*API�ӿڣ����õ��Զϵ�*/
ZL_EXPORT ZL_EXP_INT zenglApi_DebugSetBreak(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * filename,ZL_EXP_INT line,
											ZL_EXP_CHAR * condition,ZL_EXP_CHAR * log,ZL_EXP_INT count,ZL_EXP_BOOL disabled)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_COMPILE_TYPE * compile;
	ZENGL_RUN_TYPE * run;
	ZENGL_DEBUG_TYPE * debug;
	ZENGL_AST_NODE_TYPE * nodes;
	ZL_INT i;
	ZL_INT ret;
	ZL_BOOL disable_Inst = ZL_FALSE;
	if(VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return -1;
	compile = &VM->compile;
	run = &VM->run;
	debug = &VM->debug;
	nodes = compile->AST_nodes.nodes;
	for(i=0;i < run->inst_list.count;i++)
	{
		if(line == nodes[run->inst_list.insts[i].nodenum].line_no &&
			ZENGL_SYS_STRLEN(filename) == ZENGL_SYS_STRLEN(nodes[run->inst_list.insts[i].nodenum].filename) &&
			filename[0] == nodes[run->inst_list.insts[i].nodenum].filename[0] &&
			ZENGL_SYS_STRCMP(filename,nodes[run->inst_list.insts[i].nodenum].filename) == 0)
		{
			switch(run->inst_list.insts[i].type)
			{
			case ZL_R_IT_PUSH:
			case ZL_R_IT_PUSH_LOC:
			case ZL_R_IT_RESET:
				disable_Inst = ZL_TRUE;
				break;
			}
			if(disable_Inst)
			{
				disable_Inst = ZL_FALSE;
				continue;
			}
			if((ret = debug->SetBreak(VM_ARG,i,nodes[run->inst_list.insts[i].nodenum].filename,line,condition,log,count,disabled)) == -1)
				return -1;
			return ret;
		}
	}
	run->SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_BREAK_LOCATION);
	return -1;
}

/*API�ӿڣ����õ��Զϵ����չ������ֱ�Ӹ���ָ��PCֵ����������*/
ZL_EXPORT ZL_EXP_INT zenglApi_DebugSetBreakEx(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT pc,ZL_EXP_CHAR * condition,ZL_EXP_CHAR * log,ZL_EXP_INT count,ZL_EXP_BOOL disabled)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_COMPILE_TYPE * compile;
	ZENGL_RUN_TYPE * run;
	ZENGL_DEBUG_TYPE * debug;
	ZENGL_AST_NODE_TYPE * nodes;
	ZL_INT ret;
	ZL_CHAR * ApiName = "zenglApi_DebugSetBreakEx";
	if(VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return -1;
	switch(VM->ApiState)
	{
	case ZL_API_ST_OPEN:
	case ZL_API_ST_RESET:
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_CALL_POSITION, ApiName , ApiName);
		return -1;
		break;
	}
	compile = &VM->compile;
	run = &VM->run;
	debug = &VM->debug;
	nodes = compile->AST_nodes.nodes;
	switch(run->inst_list.insts[pc].type)
	{
	case ZL_R_IT_END:
		return 0;
		break;
	}
	ret = debug->SetBreak(VM_ARG,pc,nodes[run->inst_list.insts[pc].nodenum].filename,nodes[run->inst_list.insts[pc].nodenum].line_no,condition,log,count,disabled);
	return ret;
}

/*API�ӿڣ���ȡindex������Ӧ�Ķϵ���Ϣ*/
ZL_EXPORT ZL_EXP_INT zenglApi_DebugGetBreak(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT index,ZL_EXP_CHAR ** filename,ZL_EXP_INT * line,
											ZL_EXP_CHAR ** condition,ZL_EXP_CHAR ** log,ZL_EXP_INT * count,ZL_EXP_BOOL * disabled,ZL_EXP_INT * pc)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_DEBUG_TYPE * debug;
	if(VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return -1;
	debug = &VM->debug;
	if(index == -1)
	{
		if(count != ZL_NULL)
			(*count) = debug->BreakPoint.count;
		return debug->BreakPoint.size;
	}

	if(debug->BreakPoint.count > 0 && index >= 0 && index < debug->BreakPoint.size && debug->BreakPoint.members[index].isvalid == ZL_TRUE)
	{
		if(filename != ZL_NULL)
			(*filename) = debug->BreakPoint.members[index].filename;
		if(line != ZL_NULL)
			(*line) = debug->BreakPoint.members[index].line;
		if(condition != ZL_NULL)
			(*condition) = debug->BreakPoint.members[index].condition;
		if(log != ZL_NULL)
			(*log) = debug->BreakPoint.members[index].log;
		if(count != ZL_NULL)
			(*count) = debug->BreakPoint.members[index].count;
		if(disabled != ZL_NULL)
			(*disabled) = debug->BreakPoint.members[index].disabled;
		if(pc != ZL_NULL)
			(*pc) = debug->BreakPoint.members[index].pc;
		return 0;
	}
	return -1;
}

/*API�ӿڣ�ɾ��index������Ӧ�Ķϵ�*/
ZL_EXPORT ZL_EXP_INT zenglApi_DebugDelBreak(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT index)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_DEBUG_TYPE * debug;
	if(VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return -1;
	debug = &VM->debug;
	if(debug->BreakPoint.count > 0 && index >= 0 && index < debug->BreakPoint.size && debug->BreakPoint.members[index].isvalid == ZL_TRUE)
	{
		debug->DelBreak(VM_ARG,index);
		return 0;
	}
	return -1;
}

/*API�ӿڣ����õ��Զϵ㴥��ʱ���õ��û��Զ��庯��*/
ZL_EXPORT ZL_EXP_INT zenglApi_DebugSetBreakHandle(ZL_EXP_VOID * VM_ARG,ZL_EXP_VOID * handle,ZL_EXP_VOID * conditionErrorHandle,ZL_EXP_BOOL break_start,ZL_EXP_BOOL OutputDebugInfo)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_DEBUG_TYPE * debug;
	if(VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return -1;
	debug = &VM->debug;
	if(handle != ZL_NULL)
		debug->userdef_debug_break = (ZL_VM_API_DEBUG_BREAK_HANDLE)handle;
	if(conditionErrorHandle != ZL_NULL)
		debug->userdef_debug_conditionError = (ZL_VM_API_DEBUG_CON_ERROR_HANDLE)conditionErrorHandle;
	if(break_start == ZL_EXP_FALSE)
		debug->break_start = ZL_FALSE;
	else
		debug->break_start = ZL_TRUE;
	if(OutputDebugInfo == ZL_EXP_FALSE)
		debug->output_debug_info = ZL_FALSE;
	else
		debug->output_debug_info = ZL_TRUE;
	return 0;
}

/*API�ӿڣ����õ����жϣ�isStepIn������Ϊ0��Ϊ�������룬����Ϊ��������*/
ZL_EXPORT ZL_EXP_INT zenglApi_DebugSetSingleBreak(ZL_EXP_VOID * VM_ARG,ZL_EXP_BOOL isStepIn)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run;
	ZENGL_DEBUG_TYPE * debug;
	ZL_CHAR * ApiName = "zenglApi_DebugSetSingleBreak";
	if(VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return -1;
	switch(VM->ApiState)
	{
	case ZL_API_ST_OPEN:
	case ZL_API_ST_RESET:
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_CALL_POSITION, ApiName , ApiName);
		return -1;
		break;
	}
	run = &VM->run;
	debug = &VM->debug;
	if(isStepIn)
	{
		debug->flag &= ~ZL_DBG_FLAG_SET_SINGLE_STEP_OUT;
		debug->flag |= ZL_DBG_FLAG_SET_SINGLE_STEP_IN;
	}
	else
	{
		debug->flag &= ~ZL_DBG_FLAG_SET_SINGLE_STEP_IN;
		debug->flag |= ZL_DBG_FLAG_SET_SINGLE_STEP_OUT;
	}
	if(debug->singleBreak.isvalid)
		debug->RestoreSingleBreak(VM_ARG);
	debug->singleBreak.compare_pc = ZL_R_CUR_INST.pc;
	return 0;
}

/*API�ӿڣ���ȡ�ű������Ķ�ջ������Ϣ*/
ZL_EXPORT ZL_EXP_INT zenglApi_DebugGetTrace(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT * argArg,ZL_EXP_INT * argLOC,ZL_EXP_INT * argPC,
											ZL_EXP_CHAR ** fileName,ZL_EXP_INT * line,ZL_EXP_CHAR ** className,ZL_EXP_CHAR ** funcName)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_DEBUG_TYPE * debug;
	ZL_CHAR * ApiName = "zenglApi_DebugGetTrace";
	if(VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return -1;
	switch(VM->ApiState)
	{
	case ZL_API_ST_OPEN:
	case ZL_API_ST_RESET:
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_CALL_POSITION, ApiName , ApiName);
		return -1;
		break;
	}
	debug = &VM->debug;
	return debug->GetTraceInfo(VM_ARG,ApiName,argArg,argLOC,argPC,fileName,line,className,funcName);
}

typedef struct _API_MEM_POOL_POINT_TYPE{
	ZL_VOID * point; // �ڴ���е�ָ��ֵ
	ZL_INT size;     // �ڴ��ָ����ռ�õ��ڴ��С
	ZL_INT offset;   // ָ���Ӧ���ڻ����е�ƫ��ֵ
}API_MEM_POOL_POINT_TYPE; // �ýṹ�д洢��ָ��ͻ���ƫ��ֵ�Ķ�Ӧ��ϵ��ͨ���ö�Ӧ��ϵ�������ҵ�ĳ��ָ���Ӧ��ƫ��ֵ

/**
 * ���ڴ��ָ�����Ӧ��ƫ��ֵ�洢����Ӧ��ϵ�ṹ����
 */
static ZL_VOID zengl_api_make_mempool(API_MEM_POOL_POINT_TYPE * api_mempool,
		ZENGL_MEM_POOL_TYPE * mempool, ZL_BYTE * cachePtr)
{
	ZL_INT i, j, k, offset = 0;
	API_MEM_POOL_POINT_TYPE temp;
	for(i=0;i < mempool->count;i++) {
		ZENGL_SYS_MEM_COPY((cachePtr + offset), mempool->points[i].point, mempool->points[i].size);
		api_mempool[i].point = mempool->points[i].point;
		api_mempool[i].size = mempool->points[i].size;
		api_mempool[i].offset = offset;
		offset += mempool->points[i].size;
	}
	// ��api_mempool��̬�����е�ָ��ֵ���մ�С�����˳���������
	for(i=0;i < (mempool->count - 1);i++)
	{
		k = i;
		for(j=i+1;j < (mempool->count - 1);j++)
			if(api_mempool[k].point > api_mempool[j].point)
				k = j;
		if(k!=i)
		{
			temp = api_mempool[i];
			api_mempool[i] = api_mempool[k];
			api_mempool[k] = temp;
		}
	}
}

/**
 * ��api_mempool��̬�����У����������㷨������pointָ���Ӧ�Ļ���ƫ��ֵ������ƫ��ֵ����
 * ���ص�ֵ��ʵ�ʵ�ƫ��ֵ��һ���������Ժ�NULL��ָ�����ֿ�
 */
static ZL_INT zengl_api_search_api_mempool(API_MEM_POOL_POINT_TYPE * api_mempool, ZL_INT count, ZL_VOID * point)
{
	ZL_VOID * min, * max;
	min = api_mempool[0].point;
	max = api_mempool[count - 1].point;
	if(point == min) {
		return (api_mempool[0].offset + 1);
	}
	else if(point == max) {
		return (api_mempool[count - 1].offset + 1);
	}
	else {
		ZL_INT minIndex=0,maxIndex=count - 1,diff;
		ZL_VOID * diffpoint;
		do
		{
			diff = (maxIndex - minIndex)/2;
			if(diff == 0) {
				return -1;
			}
			else {
				diffpoint = api_mempool[minIndex + diff].point;
				if(point == diffpoint) {
					return (api_mempool[minIndex + diff].offset + 1);
				}
				else if(point > diffpoint) {
					minIndex += diff;
				}
				else
					maxIndex = minIndex + diff;
			}
		}while(ZL_TRUE);
	}
}

/**
 * �����ڴ�ص�ʵ���ܴ�С
 */
static ZL_INT zengl_api_compute_mempool_realsize(ZENGL_MEM_POOL_TYPE * mempool)
{
	ZL_INT i, realsize = 0;
	for(i=0;i < mempool->count;i++) {
		realsize += mempool->points[i].size;
	}
	return realsize;
}

/**
 * API�ӿڣ����������ͽ���������Ҫ���ڴ����ݻ���������������ڴ����ݿ��Դ洢���ļ����߱�ĵط���
 * ֮��������û����������ڴ���������������̣�ֱ��ִ��������ָ������������ڴ�����ֻ�������ڵ�ǰ����
 */
ZL_EXPORT ZL_EXP_INT zenglApi_CacheMemData(ZL_EXP_VOID * VM_ARG, ZL_EXP_VOID ** cachePoint, ZL_EXP_INT * cacheSize)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_COMPILE_TYPE * compile;
	ZENGL_RUN_TYPE * run;
	ZENGL_EXPORT_API_CACHE_TYPE * api_cache;
	ZL_BYTE * cachePtr, * api_mempool, * api_cache_mempool, * tmp_point, * tmp_point2, * tp;
	ZL_INT baseSize, totalSize, mempoolRealSize, instListSize, instDataStringPoolSize, tmpIndex, i;
	ZL_LONG offset, file_offset;
	ZL_CHAR * ApiName = "zenglApi_CacheMemData";
	if(VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return -1;
	switch(VM->ApiState)
	{
	case ZL_API_ST_AFTER_RUN:
	case ZL_API_ST_REUSE:
		break;
	default:
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_CALL_POSITION, ApiName , ApiName);
		return -1;
		break;
	}
	compile = &VM->compile;
	run = &VM->run;
	/* compile->mempool + compile->def_StringPool + compile->def_table + compile->HashTable +
	 * compile->LineCols + compile->FileStackList + compile->SymGlobalTable + compile->SymLocalTable +
	 * compile->SymClassTable + compile->SymClassMemberTable + compile->SymFunTable + compile->LDAddrList +
	 * compile->AST_nodes + compile->Token_StringPool */
	baseSize = sizeof(ZENGL_EXPORT_API_CACHE_TYPE) + sizeof(ZENGL_STRING_POOL_TYPE) + sizeof(ZENGL_DEF_TABLE) +
				sizeof(ZL_INT) * ZL_SYM_HASH_TOTAL_SIZE + sizeof(ZENGL_LINECOL_TABLE) +
				sizeof(ZENGL_FILE_STACKLIST_TYPE) + sizeof(ZENGL_SYM_GLOBAL_TABLE) +
				sizeof(ZENGL_SYM_LOCAL_TABLE) + sizeof(ZENGL_SYM_CLASS_TABLE) +
				sizeof(ZENGL_SYM_CLASSMEMBER_TABLE) + sizeof(ZENGL_SYM_FUN_TABLE) + sizeof(ZENGL_LD_ADDRLIST_TYPE) +
				sizeof(ZENGL_AST_TYPE) + sizeof(ZENGL_TOKEN_STRING_POOL);
	mempoolRealSize = zengl_api_compute_mempool_realsize(&compile->mempool);
	instListSize = sizeof(ZENGL_RUN_INST_LIST) + run->inst_list.count * sizeof(ZENGL_RUN_INST_LIST_MEMBER);
	instDataStringPoolSize = sizeof(ZENGL_RUN_INST_DATA_STRING_POOL) + (run->InstData_StringPool.count * sizeof(ZL_CHAR));
	totalSize = baseSize + mempoolRealSize + instListSize + instDataStringPoolSize;
	cachePtr = (ZL_BYTE *)run->memAlloc(VM_ARG,totalSize, &tmpIndex);
	ZENGL_SYS_MEM_SET(cachePtr, 0, totalSize);
	api_cache = (ZENGL_EXPORT_API_CACHE_TYPE *)cachePtr;
	api_cache->signer = ZL_EXP_API_CACHE_SIGNER;  // ���û���ǩ���������ж��Ƿ�����Ч�Ļ�������
	api_cache->mempoolRealSize = mempoolRealSize; // �ڴ�ص�ʵ�ʴ�С
	api_cache->mempoolOffset = baseSize; // �����������е��ڴ�ص�ƫ��ֵ
	api_cache->totalSize = totalSize; // �������ݵ��ܴ�С
	api_mempool = (ZL_BYTE *)run->memAlloc(VM_ARG, compile->mempool.count * sizeof(API_MEM_POOL_POINT_TYPE), &tmpIndex);
	api_cache_mempool = cachePtr + api_cache->mempoolOffset;
	zengl_api_make_mempool((API_MEM_POOL_POINT_TYPE *)api_mempool, &compile->mempool, api_cache_mempool);

	// ��def_StringPool�궨�峣�����ַ����ؿ�����������
	tmp_point = cachePtr + sizeof(ZENGL_EXPORT_API_CACHE_TYPE); // compile->def_StringPool
	ZENGL_SYS_MEM_COPY(tmp_point, &compile->def_StringPool, sizeof(ZENGL_STRING_POOL_TYPE));
	if(compile->def_StringPool.ptr != ZL_NULL) {
		// ��ָ��תΪoffset(����ƫ��ֵ)�����洢������������û���ʱ��ֻ�轫offset���ϻ����ַ�����ɵõ�ָ��ֵ
		offset = zengl_api_search_api_mempool((API_MEM_POOL_POINT_TYPE *)api_mempool, compile->mempool.count, compile->def_StringPool.ptr);
		if(offset < 0) {
			VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_CACHE_SEARCH_MEMPOOL_POINT_FAILED, ApiName , ApiName);
			return -1;
		}
		((ZENGL_STRING_POOL_TYPE *)tmp_point)->ptr = (ZL_CHAR *)offset;
	}

	// ��def_table�궨�嶯̬���鿽����������
	tmp_point += sizeof(ZENGL_STRING_POOL_TYPE);
	ZENGL_SYS_MEM_COPY(tmp_point, &compile->def_table, sizeof(ZENGL_DEF_TABLE));
	if(compile->def_table.defs != ZL_NULL) {
		// ��ָ��תΪoffset(�����е�ƫ��ֵ)
		offset = zengl_api_search_api_mempool((API_MEM_POOL_POINT_TYPE *)api_mempool, compile->mempool.count, compile->def_table.defs);
		if(offset < 0) {
			VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_CACHE_SEARCH_MEMPOOL_POINT_FAILED, ApiName , ApiName);
			return -1;
		}
		((ZENGL_DEF_TABLE *)tmp_point)->defs = (ZENGL_DEF_TABLE_MEMBER *)offset;
	}

	// ����ϣ������������
	tmp_point += sizeof(ZENGL_DEF_TABLE);
	ZENGL_SYS_MEM_COPY(tmp_point, compile->HashTable, sizeof(ZL_INT) * ZL_SYM_HASH_TOTAL_SIZE);

	// ���洢���кŵĶ�̬���鿽����������
	tmp_point += sizeof(ZL_INT) * ZL_SYM_HASH_TOTAL_SIZE;
	ZENGL_SYS_MEM_COPY(tmp_point, &compile->LineCols, sizeof(ZENGL_LINECOL_TABLE));
	if(compile->LineCols.lines != ZL_NULL) {
		// ��LineCols.linesָ��תΪoffset����ƫ��ֵ
		offset = zengl_api_search_api_mempool((API_MEM_POOL_POINT_TYPE *)api_mempool, compile->mempool.count, compile->LineCols.lines);
		if(offset < 0) {
			VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_CACHE_SEARCH_MEMPOOL_POINT_FAILED, ApiName , ApiName);
			return -1;
		}
		((ZENGL_LINECOL_TABLE *)tmp_point)->lines = (ZENGL_LINECOL *)offset;
		tmp_point2 = api_cache_mempool + (offset - 1);
		tp = ZL_NULL;
		offset = 0;
		for(i=0; i < compile->LineCols.count ;i++) {
			if(compile->LineCols.lines[i].filename != ZL_NULL) {
				// �����к���Ϣ�е�filename�ļ���ָ��תΪ����ƫ��ֵ
				if(tp != (ZL_BYTE *)compile->LineCols.lines[i].filename) {
					offset = zengl_api_search_api_mempool((API_MEM_POOL_POINT_TYPE *)api_mempool, compile->mempool.count, compile->LineCols.lines[i].filename);
					if(offset < 0) {
						VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_CACHE_SEARCH_MEMPOOL_POINT_FAILED, ApiName , ApiName);
						return -1;
					}
					tp = (ZL_BYTE *)compile->LineCols.lines[i].filename;
				}
				((ZENGL_LINECOL *)tmp_point2)[i].filename = (ZL_CHAR *)offset;
			}
		}
	}

	// ��inc���ص��ļ���Ϣ�Ķ�ջ������������
	tmp_point += sizeof(ZENGL_LINECOL_TABLE);
	ZENGL_SYS_MEM_COPY(tmp_point, &compile->FileStackList, sizeof(ZENGL_FILE_STACKLIST_TYPE));
	if(compile->FileStackList.stacks != ZL_NULL) {
		offset = zengl_api_search_api_mempool((API_MEM_POOL_POINT_TYPE *)api_mempool, compile->mempool.count, compile->FileStackList.stacks);
		if(offset < 0) {
			VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_CACHE_SEARCH_MEMPOOL_POINT_FAILED, ApiName , ApiName);
			return -1;
		}
		((ZENGL_FILE_STACKLIST_TYPE *)tmp_point)->stacks = (ZENGL_FILE_STACK_TYPE *)offset;
		tmp_point2 = api_cache_mempool + (offset - 1);
		tp = ZL_NULL;
		offset = 0;
		for(i=0; i < compile->FileStackList.count ;i++) {
			// ��source�е�filenameָ��תΪoffset����ƫ��ֵ
			if(compile->FileStackList.stacks[i].source.filename != ZL_NULL) {
				if(tp != (ZL_BYTE *)compile->FileStackList.stacks[i].source.filename) {
					offset = zengl_api_search_api_mempool((API_MEM_POOL_POINT_TYPE *)api_mempool, compile->mempool.count,
							compile->FileStackList.stacks[i].source.filename);
					if(offset < 0) {
						VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_CACHE_SEARCH_MEMPOOL_POINT_FAILED, ApiName , ApiName);
						return -1;
					}
					tp = (ZL_BYTE *)compile->FileStackList.stacks[i].source.filename;
				}
				((ZENGL_FILE_STACK_TYPE *)tmp_point2)[i].source.filename = (ZL_CHAR *)offset;
				((ZENGL_FILE_STACK_TYPE *)tmp_point2)[i].source.file = ZL_NULL;
			}
		}

		// FileStackList.filenames�д洢�����м��ع����ļ����ļ�����Ϣ������Щ�ļ���ָ�붼תΪoffset����ƫ��ֵ���洢��������
		offset = zengl_api_search_api_mempool((API_MEM_POOL_POINT_TYPE *)api_mempool, compile->mempool.count, compile->FileStackList.filenames);
		if(offset < 0) {
			VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_CACHE_SEARCH_MEMPOOL_POINT_FAILED, ApiName , ApiName);
			return -1;
		}
		((ZENGL_FILE_STACKLIST_TYPE *)tmp_point)->filenames = (ZL_CHAR **)offset;
		api_cache->filenames = (ZL_CHAR **)offset;
		api_cache->filenames_count = compile->FileStackList.filenames_count;
		tmp_point2 = api_cache_mempool + (offset - 1);
		tp = ZL_NULL;
		offset = 0;
		for(i=0; i < compile->FileStackList.filenames_count; i++) {
			if(compile->FileStackList.filenames[i] != ZL_NULL) {
				// �����ع����ļ���ָ�붼תΪoffset����ƫ��ֵ
				if(tp != (ZL_BYTE *)compile->FileStackList.filenames[i]) {
					offset = zengl_api_search_api_mempool((API_MEM_POOL_POINT_TYPE *)api_mempool, compile->mempool.count,
												compile->FileStackList.filenames[i]);
					if(offset < 0) {
						VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_CACHE_SEARCH_MEMPOOL_POINT_FAILED, ApiName , ApiName);
						return -1;
					}
					tp = (ZL_BYTE *)compile->FileStackList.filenames[i];
				}
				((ZL_CHAR **)tmp_point2)[i] = (ZL_CHAR *)offset;
			}
		}
	}

	// ��ȫ�ֱ������ű�����������
	tmp_point += sizeof(ZENGL_FILE_STACKLIST_TYPE);
	ZENGL_SYS_MEM_COPY(tmp_point, &compile->SymGlobalTable, sizeof(ZENGL_SYM_GLOBAL_TABLE));
	if(compile->SymGlobalTable.sym != ZL_NULL) {
		offset = zengl_api_search_api_mempool((API_MEM_POOL_POINT_TYPE *)api_mempool, compile->mempool.count, compile->SymGlobalTable.sym);
		if(offset < 0) {
			VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_CACHE_SEARCH_MEMPOOL_POINT_FAILED, ApiName , ApiName);
			return -1;
		}
		((ZENGL_SYM_GLOBAL_TABLE *)tmp_point)->sym = (ZENGL_SYM_GLOBAL_TABLE_MEMBER *)offset;
	}

	// ���ű����������;ֲ��������ű�����������
	tmp_point += sizeof(ZENGL_SYM_GLOBAL_TABLE);
	ZENGL_SYS_MEM_COPY(tmp_point, &compile->SymLocalTable, sizeof(ZENGL_SYM_LOCAL_TABLE));
	if(compile->SymLocalTable.local != ZL_NULL) {
		offset = zengl_api_search_api_mempool((API_MEM_POOL_POINT_TYPE *)api_mempool, compile->mempool.count, compile->SymLocalTable.local);
		if(offset < 0) {
			VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_CACHE_SEARCH_MEMPOOL_POINT_FAILED, ApiName , ApiName);
			return -1;
		}
		((ZENGL_SYM_LOCAL_TABLE *)tmp_point)->local = (ZENGL_SYM_LOCAL_TABLE_MEMBER *)offset;
	}

	// ������ű�����������
	tmp_point += sizeof(ZENGL_SYM_LOCAL_TABLE);
	ZENGL_SYS_MEM_COPY(tmp_point, &compile->SymClassTable, sizeof(ZENGL_SYM_CLASS_TABLE));
	if(compile->SymClassTable.classTable != ZL_NULL) {
		// ������ű��е�classTable��̬�����ָ��תΪoffset����ƫ��ֵ
		offset = zengl_api_search_api_mempool((API_MEM_POOL_POINT_TYPE *)api_mempool, compile->mempool.count, compile->SymClassTable.classTable);
		if(offset < 0) {
			VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_CACHE_SEARCH_MEMPOOL_POINT_FAILED, ApiName , ApiName);
			return -1;
		}
		((ZENGL_SYM_CLASS_TABLE *)tmp_point)->classTable = (ZENGL_SYM_CLASS_TABLE_MEMBER *)offset;
	}

	// �����Ա���ű�����������
	tmp_point += sizeof(ZENGL_SYM_CLASS_TABLE);
	ZENGL_SYS_MEM_COPY(tmp_point, &compile->SymClassMemberTable, sizeof(ZENGL_SYM_CLASSMEMBER_TABLE));
	if(compile->SymClassMemberTable.members != ZL_NULL) {
		// �����Ա���ű��members��̬�����ָ��תΪoffset����ƫ��ֵ
		offset = zengl_api_search_api_mempool((API_MEM_POOL_POINT_TYPE *)api_mempool, compile->mempool.count, compile->SymClassMemberTable.members);
		if(offset < 0) {
			VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_CACHE_SEARCH_MEMPOOL_POINT_FAILED, ApiName , ApiName);
			return -1;
		}
		((ZENGL_SYM_CLASSMEMBER_TABLE *)tmp_point)->members = (ZENGL_SYM_CLASSMEMBER_TABLE_MEMBER *)offset;
	}

	// ���������ű�����������
	tmp_point += sizeof(ZENGL_SYM_CLASSMEMBER_TABLE);
	ZENGL_SYS_MEM_COPY(tmp_point, &compile->SymFunTable, sizeof(ZENGL_SYM_FUN_TABLE));
	if(compile->SymFunTable.funs != ZL_NULL) {
		// ���������ű�Ķ�̬�����ָ��ֵתΪoffset����ƫ��ֵ�����洢��������
		offset = zengl_api_search_api_mempool((API_MEM_POOL_POINT_TYPE *)api_mempool, compile->mempool.count, compile->SymFunTable.funs);
		if(offset < 0) {
			VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_CACHE_SEARCH_MEMPOOL_POINT_FAILED, ApiName , ApiName);
			return -1;
		}
		((ZENGL_SYM_FUN_TABLE *)tmp_point)->funs = (ZENGL_SYM_FUN_TABLE_MEMBER *)offset;
	}

	// �����ӵ�ַ��̬���鿽����������
	tmp_point += sizeof(ZENGL_SYM_FUN_TABLE);
	ZENGL_SYS_MEM_COPY(tmp_point, &compile->LDAddrList, sizeof(ZENGL_LD_ADDRLIST_TYPE));
	if(compile->LDAddrList.addr != ZL_NULL) {
		// ����̬�����ָ��תΪ����ƫ��ֵ
		offset = zengl_api_search_api_mempool((API_MEM_POOL_POINT_TYPE *)api_mempool, compile->mempool.count, compile->LDAddrList.addr);
		if(offset < 0) {
			VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_CACHE_SEARCH_MEMPOOL_POINT_FAILED, ApiName , ApiName);
			return -1;
		}
		((ZENGL_LD_ADDRLIST_TYPE *)tmp_point)->addr = (ZENGL_LD_ADDR_TYPE *)offset;
	}

	// ��AST�����﷨��������������
	tmp_point += sizeof(ZENGL_LD_ADDRLIST_TYPE);
	ZENGL_SYS_MEM_COPY(tmp_point, &compile->AST_nodes, sizeof(ZENGL_AST_TYPE));
	if(compile->AST_nodes.nodes != ZL_NULL) {
		offset = zengl_api_search_api_mempool((API_MEM_POOL_POINT_TYPE *)api_mempool, compile->mempool.count, compile->AST_nodes.nodes);
		if(offset < 0) {
			VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_CACHE_SEARCH_MEMPOOL_POINT_FAILED, ApiName , ApiName);
			return -1;
		}
		((ZENGL_AST_TYPE *)tmp_point)->nodes = (ZENGL_AST_NODE_TYPE *)offset;
		tmp_point2 = api_cache_mempool + (offset - 1);
		tp = ZL_NULL;
		offset = 0;
		file_offset = 0;
		for(i=0; i < compile->AST_nodes.count ;i++) {
			// ���﷨����ÿ���ڵ��а������ļ���ָ��תΪƫ��ֵ
			if(compile->AST_nodes.nodes[i].filename != ZL_NULL) {
				if(tp != (ZL_BYTE *)compile->AST_nodes.nodes[i].filename) {
					file_offset = zengl_api_search_api_mempool((API_MEM_POOL_POINT_TYPE *)api_mempool, compile->mempool.count, compile->AST_nodes.nodes[i].filename);
					if(file_offset < 0) {
						VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_CACHE_SEARCH_MEMPOOL_POINT_FAILED, ApiName , ApiName);
						return -1;
					}
					tp = (ZL_BYTE *)compile->AST_nodes.nodes[i].filename;
				}
				((ZENGL_AST_NODE_TYPE *)tmp_point2)[i].filename = (ZL_CHAR *)file_offset;
			}
			// ���ڵ��extchilds��չ�ӽڵ㶯̬�����ָ��ֵתΪoffsetƫ��ֵ
			if(compile->AST_nodes.nodes[i].childs.extchilds != ZL_NULL) {
				offset = zengl_api_search_api_mempool((API_MEM_POOL_POINT_TYPE *)api_mempool, compile->mempool.count, compile->AST_nodes.nodes[i].childs.extchilds);
				if(offset < 0) {
					VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_CACHE_SEARCH_MEMPOOL_POINT_FAILED, ApiName , ApiName);
					return -1;
				}
				((ZENGL_AST_NODE_TYPE *)tmp_point2)[i].childs.extchilds = (ZL_INT *)offset;
			}
		}
	}

	// ��token�ַ����ؿ�����������
	tmp_point += sizeof(ZENGL_AST_TYPE);
	ZENGL_SYS_MEM_COPY(tmp_point, &compile->Token_StringPool, sizeof(ZENGL_TOKEN_STRING_POOL));
	if(compile->Token_StringPool.ptr != ZL_NULL) {
		// ��token�ַ�����ָ��תΪoffsetƫ��ֵ
		offset = zengl_api_search_api_mempool((API_MEM_POOL_POINT_TYPE *)api_mempool, compile->mempool.count, compile->Token_StringPool.ptr);
		if(offset < 0) {
			VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_CACHE_SEARCH_MEMPOOL_POINT_FAILED, ApiName , ApiName);
			return -1;
		}
		((ZENGL_TOKEN_STRING_POOL *)tmp_point)->ptr = (ZL_CHAR *)offset;
	}

	// ����������ָ���б���صĽṹ�忽����������
	tmp_point = cachePtr + (baseSize + mempoolRealSize);
	ZENGL_SYS_MEM_COPY(tmp_point, &run->inst_list, sizeof(ZENGL_RUN_INST_LIST));
	((ZENGL_RUN_INST_LIST *)tmp_point)->size = ((ZENGL_RUN_INST_LIST *)tmp_point)->count;
	((ZENGL_RUN_INST_LIST *)tmp_point)->mempool_index = 0;
	((ZENGL_RUN_INST_LIST *)tmp_point)->insts = ZL_NULL;

	// ��ָ���б������е�ָ�������������
	tmp_point += sizeof(ZENGL_RUN_INST_LIST);
	ZENGL_SYS_MEM_COPY(tmp_point, run->inst_list.insts, run->inst_list.count * sizeof(ZENGL_RUN_INST_LIST_MEMBER));
	for(i=0; i < run->inst_list.count ;i++) {
		if(run->inst_list.insts[i].type == ZL_R_IT_CALL) {
			// ���CALLָ���srcԴ��������ִ��ʱ�����滻Ϊ��ģ�麯������������ֵ�����仹ԭΪ�ַ�����
			// ��������л�ԭ��������ô���ͻ��˵�����ģ�麯��ʱ��ԭ����ֵ���ܻ��Ӧ�������ģ�麯��
			if(run->inst_list.insts[i].src.type == ZL_R_DT_NUM) {
				ZENGL_RUN_INST_LIST_MEMBER * tmp_member = (((ZENGL_RUN_INST_LIST_MEMBER *)tmp_point) + i);
				tmpIndex = run->inst_list.insts[i].src.val.num;
				tmp_member->src.type = ZL_R_DT_STR;
				tmp_member->src.val.str_Index = run->ModFunTable.mod_funs[tmpIndex].strIndex;
			}
		}
	}

	tmp_point = cachePtr + (baseSize + mempoolRealSize + instListSize);
	// ��ָ��������ַ�������صĽṹ�忽����������
	ZENGL_SYS_MEM_COPY(tmp_point, &run->InstData_StringPool, sizeof(ZENGL_RUN_INST_DATA_STRING_POOL));
	((ZENGL_RUN_INST_DATA_STRING_POOL *)tmp_point)->size = ((ZENGL_RUN_INST_DATA_STRING_POOL *)tmp_point)->count;
	((ZENGL_RUN_INST_DATA_STRING_POOL *)tmp_point)->mempool_index = 0;
	((ZENGL_RUN_INST_DATA_STRING_POOL *)tmp_point)->ptr = ZL_NULL;

	tmp_point += sizeof(ZENGL_RUN_INST_DATA_STRING_POOL);
	// ��ָ��������ַ������е������ַ���������������
	ZENGL_SYS_MEM_COPY(tmp_point, run->InstData_StringPool.ptr, run->InstData_StringPool.count * sizeof(ZL_CHAR));

	// ������ָ��ͻ����Сͨ��ָ��������ظ�������
	(*cachePoint) = cachePtr;
	(*cacheSize) = totalSize;
	return 0;
}

/* API�ӿڣ������û������ݣ��Ϳ�������������� */
ZL_EXPORT ZL_EXP_INT zenglApi_ReUseCacheMemData(ZL_EXP_VOID * VM_ARG, ZL_EXP_VOID * cachePoint, ZL_EXP_INT cacheSize)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_COMPILE_TYPE * compile;
	ZENGL_RUN_TYPE * run;
	ZENGL_EXPORT_API_CACHE_TYPE * api_cache;
	ZL_BYTE * mempoolPtr, * tmp_point, * cachePtr;
	ZL_INT i, str_Index;
	ZL_LONG offset;
	ZL_CHAR * ApiName = "zenglApi_ReUseCacheMemData";
	ZL_CHAR * inst_data_string_ptr;
	//ZENGL_RUN_INST_DATA_STRING_POOL tmp_inst_data_str_pool;
	cachePtr = (ZL_BYTE *)cachePoint;
	if(VM->signer != ZL_VM_SIGNER) //ͨ�������ǩ���ж��Ƿ�����Ч�������
		return -1;
	switch(VM->ApiState)
	{
	case ZL_API_ST_OPEN:
	case ZL_API_ST_RESET:
		break;
	default:
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_INVALID_CALL_POSITION, ApiName , ApiName);
		return -1;
		break;
	}
	compile = &VM->compile;
	run = &VM->run;
	api_cache = (ZENGL_EXPORT_API_CACHE_TYPE *)cachePoint;
	// ͨ������ǩ�����ж��Ƿ�����Ч�Ļ�������
	if(api_cache->signer != ZL_EXP_API_CACHE_SIGNER) {
		VM->run.SetApiErrorEx(VM_ARG,ZL_ERR_VM_API_CACHE_INVALID_CACHE_DATA, ApiName , ApiName);
		return -1;
	}
	mempoolPtr = (ZL_BYTE *)compile->memAlloc(VM_ARG, api_cache->mempoolRealSize);
	// �������б�����ڴ�����ݣ���������������
	ZENGL_SYS_MEM_COPY(mempoolPtr, (cachePtr + api_cache->mempoolOffset), api_cache->mempoolRealSize);

	tmp_point = cachePtr + sizeof(ZENGL_EXPORT_API_CACHE_TYPE);
	// �������е�def�궨�峣�����ַ����ؿ�������������
	ZENGL_SYS_MEM_COPY(&compile->def_StringPool, tmp_point, sizeof(ZENGL_STRING_POOL_TYPE));
	offset = (ZL_LONG)compile->def_StringPool.ptr;
	if(offset > 0) // �������е�ƫ��ֵתΪ����ʹ�õ�ָ��ֵ
		compile->def_StringPool.ptr = (ZL_CHAR *)(mempoolPtr + offset - 1);

	tmp_point += sizeof(ZENGL_STRING_POOL_TYPE);
	// ��def_table�궨�嶯̬���鿽������������
	ZENGL_SYS_MEM_COPY(&compile->def_table, tmp_point, sizeof(ZENGL_DEF_TABLE));
	offset = (ZL_LONG)compile->def_table.defs;
	if(offset > 0) // ������ƫ��ֵתΪָ��ֵ
		compile->def_table.defs = (ZENGL_DEF_TABLE_MEMBER *)(mempoolPtr + offset - 1);

	tmp_point += sizeof(ZENGL_DEF_TABLE);
	// ������Ĺ�ϣ������������
	ZENGL_SYS_MEM_COPY(compile->HashTable, tmp_point, sizeof(ZL_INT) * ZL_SYM_HASH_TOTAL_SIZE);

	tmp_point += sizeof(ZL_INT) * ZL_SYM_HASH_TOTAL_SIZE;
	// ����������кŶ�̬���鿽����������
	ZENGL_SYS_MEM_COPY(&compile->LineCols, tmp_point, sizeof(ZENGL_LINECOL_TABLE));
	offset = (ZL_LONG)compile->LineCols.lines;
	if(offset > 0) {
		compile->LineCols.lines = (ZENGL_LINECOL *)(mempoolPtr + offset - 1);
		for(i=0; i < compile->LineCols.count ;i++) {
			// ������ƫ��ֵתΪָ��ֵ
			offset = (ZL_LONG)(compile->LineCols.lines[i].filename);
			if(offset > 0)
				compile->LineCols.lines[i].filename = (ZL_CHAR *)(mempoolPtr + offset - 1);
		}
	}

	tmp_point += sizeof(ZENGL_LINECOL_TABLE);
	// �������inc���ص��ļ���Ϣ��ջ����������������������ƫ��ֵתΪ����ʹ�õ�ָ��ֵ
	ZENGL_SYS_MEM_COPY(&compile->FileStackList, tmp_point, sizeof(ZENGL_FILE_STACKLIST_TYPE));
	offset = (ZL_LONG)compile->FileStackList.stacks;
	if(offset > 0) {
		compile->FileStackList.stacks = (ZENGL_FILE_STACK_TYPE *)(mempoolPtr + offset - 1);
		for(i=0; i < compile->FileStackList.count ;i++) {
			offset = (ZL_LONG)(compile->FileStackList.stacks[i].source.filename);
			if(offset > 0)
				compile->FileStackList.stacks[i].source.filename = (ZL_CHAR *)(mempoolPtr + offset - 1);
		}
	}

	tmp_point += sizeof(ZENGL_FILE_STACKLIST_TYPE);
	// �������ȫ�ַ��ű�����������
	ZENGL_SYS_MEM_COPY(&compile->SymGlobalTable, tmp_point, sizeof(ZENGL_SYM_GLOBAL_TABLE));
	offset = (ZL_LONG)compile->SymGlobalTable.sym;
	if(offset > 0) // ������ƫ��תΪָ��
		compile->SymGlobalTable.sym = (ZENGL_SYM_GLOBAL_TABLE_MEMBER *)(mempoolPtr + offset - 1);

	tmp_point += sizeof(ZENGL_SYM_GLOBAL_TABLE);
	// ������ľֲ�����(������������)���ű�����������
	ZENGL_SYS_MEM_COPY(&compile->SymLocalTable, tmp_point, sizeof(ZENGL_SYM_LOCAL_TABLE));
	offset = (ZL_LONG)compile->SymLocalTable.local;
	if(offset > 0) // ������ƫ��תΪָ��
		compile->SymLocalTable.local = (ZENGL_SYM_LOCAL_TABLE_MEMBER *)(mempoolPtr + offset - 1);

	tmp_point += sizeof(ZENGL_SYM_LOCAL_TABLE);
	// �����������ű�����������
	ZENGL_SYS_MEM_COPY(&compile->SymClassTable, tmp_point, sizeof(ZENGL_SYM_CLASS_TABLE));
	offset = (ZL_LONG)compile->SymClassTable.classTable;
	if(offset > 0) // ������ƫ��תΪָ��
		compile->SymClassTable.classTable = (ZENGL_SYM_CLASS_TABLE_MEMBER *)(mempoolPtr + offset - 1);

	tmp_point += sizeof(ZENGL_SYM_CLASS_TABLE);
	// ����������Ա���ű�����������
	ZENGL_SYS_MEM_COPY(&compile->SymClassMemberTable, tmp_point, sizeof(ZENGL_SYM_CLASSMEMBER_TABLE));
	offset = (ZL_LONG)compile->SymClassMemberTable.members;
	if(offset > 0) // ������ƫ��תΪָ��
		compile->SymClassMemberTable.members = (ZENGL_SYM_CLASSMEMBER_TABLE_MEMBER *)(mempoolPtr + offset - 1);

	tmp_point += sizeof(ZENGL_SYM_CLASSMEMBER_TABLE);
	// ������ĺ������ű�����������
	ZENGL_SYS_MEM_COPY(&compile->SymFunTable, tmp_point, sizeof(ZENGL_SYM_FUN_TABLE));
	offset = (ZL_LONG)compile->SymFunTable.funs;
	if(offset > 0) // ������ƫ��תΪָ��
		compile->SymFunTable.funs = (ZENGL_SYM_FUN_TABLE_MEMBER *)(mempoolPtr + offset - 1);

	tmp_point += sizeof(ZENGL_SYM_FUN_TABLE);
	// ����������ӵ�ַ��̬���鿽����������
	ZENGL_SYS_MEM_COPY(&compile->LDAddrList, tmp_point, sizeof(ZENGL_LD_ADDRLIST_TYPE));
	offset = (ZL_LONG)compile->LDAddrList.addr;
	if(offset > 0) // ������ƫ��תΪָ��
		compile->LDAddrList.addr = (ZENGL_LD_ADDR_TYPE *)(mempoolPtr + offset - 1);

	tmp_point += sizeof(ZENGL_LD_ADDRLIST_TYPE);
	// �������AST�����﷨��������������
	ZENGL_SYS_MEM_COPY(&compile->AST_nodes, tmp_point, sizeof(ZENGL_AST_TYPE));
	offset = (ZL_LONG)compile->AST_nodes.nodes;
	if(offset > 0) { // ���ڵ��еĸ�������ƫ��ֵתΪָ��ֵ
		compile->AST_nodes.nodes = (ZENGL_AST_NODE_TYPE *)(mempoolPtr + offset - 1);
		for(i=0; i < compile->AST_nodes.count ;i++) {
			offset = (ZL_LONG)(compile->AST_nodes.nodes[i].filename);
			if(offset > 0)
				compile->AST_nodes.nodes[i].filename = (ZL_CHAR *)(mempoolPtr + offset - 1);
			offset = (ZL_LONG)(compile->AST_nodes.nodes[i].childs.extchilds);
			if(offset > 0)
				compile->AST_nodes.nodes[i].childs.extchilds = (ZL_INT *)(mempoolPtr + offset - 1);
		}
	}

	tmp_point += sizeof(ZENGL_AST_TYPE);
	// �������token�ַ����ؿ�����������
	ZENGL_SYS_MEM_COPY(&compile->Token_StringPool, tmp_point, sizeof(ZENGL_TOKEN_STRING_POOL));
	offset = (ZL_LONG)compile->Token_StringPool.ptr;
	if(offset > 0) // ������ƫ��תΪָ��
		compile->Token_StringPool.ptr = (ZL_CHAR *)(mempoolPtr + offset - 1);

	tmp_point = cachePtr + (api_cache->mempoolOffset + api_cache->mempoolRealSize);
	// �������ָ���б���صĽṹ�忽����������
	ZENGL_SYS_MEM_COPY( &run->inst_list, tmp_point, sizeof(ZENGL_RUN_INST_LIST));
	run->inst_list.insts = (ZENGL_RUN_INST_LIST_MEMBER *)run->memAlloc(VM_ARG, run->inst_list.count * sizeof(ZENGL_RUN_INST_LIST_MEMBER),
			&run->inst_list.mempool_index);

	tmp_point += sizeof(ZENGL_RUN_INST_LIST);
	// �������ָ���б��е�����ָ�������������
	ZENGL_SYS_MEM_COPY(run->inst_list.insts, tmp_point, run->inst_list.count * sizeof(ZENGL_RUN_INST_LIST_MEMBER));

	tmp_point += run->inst_list.count * sizeof(ZENGL_RUN_INST_LIST_MEMBER);
	//ZENGL_SYS_MEM_COPY(&tmp_inst_data_str_pool, tmp_point, sizeof(ZENGL_RUN_INST_DATA_STRING_POOL));
	tmp_point += sizeof(ZENGL_RUN_INST_DATA_STRING_POOL);
	inst_data_string_ptr = (ZL_CHAR *)tmp_point;
	// �������ָ��������ַ���ͨ��InstDataStringPoolAdd������ӵ���ǰ�������У�����Ӧ�ĵ���ָ���е�str_Index��ֵ(�ַ������ַ������е�����)
	for(i=0; i < run->inst_list.count ;i++) {
		if(run->inst_list.insts[i].src.type == ZL_R_DT_STR) {
			str_Index = run->inst_list.insts[i].src.val.str_Index;
			if(str_Index > 0)
				run->inst_list.insts[i].src.val.str_Index = run->InstDataStringPoolAdd(VM_ARG, (inst_data_string_ptr + str_Index));
		}
		if(run->inst_list.insts[i].dest.type == ZL_R_DT_STR) {
			str_Index = run->inst_list.insts[i].dest.val.str_Index;
			if(str_Index > 0)
				run->inst_list.insts[i].dest.val.str_Index = run->InstDataStringPoolAdd(VM_ARG, (inst_data_string_ptr + str_Index));
		}
	}
	// �ڽ�����ı������ݶ�������������к󣬾Ϳ��Խ���������isReUse����ΪZL_TRUE�������ڽ���ִ��ʱ���Ϳ��������������
	VM->compile.isReUse = ZL_TRUE;
	return 0;
}
