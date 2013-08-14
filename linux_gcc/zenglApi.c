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
	该文件为虚拟机对外提供的函数接口文件。
*/

#include "zengl_global.h"

ZENGL_VM_TYPE ZL_Api_Const_VM = 
	{
		{
			{0}, //tokenInfo
			{0}, //source成员
			{0}, //mempool
			{0}, //jumpBuffer成员
			1, //line_no
			0, //col_no
			ZL_FALSE, //is_inConvChr
			0, //basesize
			0, //totalsize
			0, //start_time
			0, //end_time
			0, //total_time
			{0}, //infoFullString
			{0}, //errorFullString
			ZL_Reserves_String, //reserveString
			ZL_Token_Operate_String, //TokenOperateString
			0, //TokenOperateStringCount
			{0}, //def_StringPool
			{0}, //def_table
			ZL_FALSE, //isinCompiling
			ZL_FALSE, //isDestroyed
			/*zengl_symbol.c符号表相关的成员*/
			{0}, //HashTable
			{0}, //LineCols
			{0}, //PathFileName
			{0}, //FileStackList
			{0}, //SymGlobalTable
			{0}, //SymLocalTable
			{0}, //SymClassTable
			{0}, //SymClassMemberTable
			{0}, //SymFunTable
			/*和zengl_assemble.c汇编代码生成相关的成员*/
			{0}, //gencode_struct
			{0}, //AsmGCStackList
			0,	 //AsmGCAddrNum
			ZL_FALSE, //AsmGCIsInClass
			{0}, //memDataForDot
			/*和zengl_ld.c链接程序相关的成员*/
			{0}, //LDAddrList
			/*zengl_parser.c语法解析相关的成员*/
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
			/*定义在zengl_symbol.c中的相关函数*/
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
			/*定义在zengl_assemble.c中的相关函数*/
			zengl_buildAsmCode,
			zengl_AsmGenCodes,
			zengl_AsmGCStackPush,
			zengl_AsmGCStackPop,
			zengl_AsmGCStackInit,
			zengl_AsmGCElif,
			zengl_AsmGCBreak_Codes,
			zengl_AsmGCContinue_Codes,
			zengl_AsmScanCaseMinMax,
			zengl_GetNodeInt,
			zengl_AsmAddCaseJmpTable,
			zengl_AsmInitCaseJmpTable,
			zengl_AsmSortCaseJmpTable,
			/*定义在zengl_ld.c中的相关函数*/
			zengl_LDAddrListSet,
			zengl_LDAddrListInit,
			zengl_LDAddrListReplaceAll,
			/*定义在zengl_parser.c中的相关函数*/
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
			/*下面是用户自定义的函数*/
			ZL_NULL,
			ZL_NULL 
		}, //初始化虚拟机的编译器

		{
			0, //basesize
				0, //totalsize
				0, //start_time
				0, //end_time
				0, //total_time
				ZL_FALSE, //isinRunning
				ZL_FALSE, //isDestroyed
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
			/*定义在zenglrun_func.c中的相关函数*/
			zenglrun_init,
			zenglrun_memAlloc,
			zenglrun_memReAlloc,
			zenglrun_memReUsePtr,
			zenglrun_exit,
			zenglrun_ExitPrintSourceInfo,
			zenglrun_memFreeAll,
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
			/*定义在zenglrun_main.c中的相关函数*/
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
			zenglrun_op_relation,
			zenglrun_op_logic,
			zenglrun_op_addminisget,
			zenglrun_op_getaddminis,
			zenglrun_op_addminisone,
			zenglrun_op_addr,
			zenglrun_op_set_array,
			zenglrun_alloc_memblock,
			zenglrun_memblock_getindex,
			zenglrun_realloc_memblock,
			zenglrun_assign_memblock,
			zenglrun_memblock_setval,
			zenglrun_VMemBlockOps,
			zenglrun_memblock_free,
			zenglrun_op_get_array,
			zenglrun_op_get_array_addr,
			zenglrun_op_addminis_one_array,
			zenglrun_memblock_freeall_local,
			zenglrun_op_switch,
			zenglrun_getRegInt,
			zenglrun_main,
			/*下面是用户自定义的函数*/
			ZL_NULL, //userdef_run_info
			ZL_NULL, //userdef_run_print
			ZL_NULL  //userdef_run_error
			}, //初始化虚拟机的解释器

			ZL_NO_ERR_SUCCESS,
			ZL_Error_String,
			ZL_FALSE, //isCompileError
			ZL_FALSE, //isRunError
			0, //basesize
			0, //totalsize
			0, //start_time
			0, //end_time
			0, //total_time
			ZL_NULL, //vm_main_args
			ZL_FALSE,//isinApiRun
			/*虚拟机相关的函数*/
			zenglVM_init
	}; //虚拟机初始化模板

/*通过zenglApi_Load可以加载并执行script_file脚本*/
ZL_EXPORT ZL_EXP_INT zenglApi_Load(ZL_EXP_CHAR * script_file,ZENGL_EXPORT_VM_MAIN_ARGS * vm_main_args)
{
	ZENGL_VM_TYPE VM = ZL_Api_Const_VM;
	ZENGL_SYS_ARG_LIST arg = ZL_NULL;
	ZL_INT retcode;

	VM.init((ZL_VOID * )&VM,vm_main_args);
	VM.run.init((ZL_VOID * )&VM); //编译器中需要对解释器输出汇编指令，所以在此初始化解释器
	if(script_file == ZL_NULL)
	{
		VM.errorno = ZL_ERR_VM_API_INVALID_SCRIPT_NAME;
		VM.run.makeInfoString((ZL_VOID * )&VM,&VM.run.errorFullString , VM.errorString[VM.errorno] , arg);
		if(VM.run.userdef_run_error != ZL_NULL)
			VM.run.userdef_run_error(VM.run.errorFullString.str,VM.run.errorFullString.count);
		VM.isRunError = ZL_TRUE;
		return -1;
	}
	retcode = VM.compile.main((ZL_VOID * )&VM,script_file,vm_main_args);
	if(retcode == 0) //如果编译成功，则进入解释器
	{
		if(VM.vm_main_args->userdef_module_init != ZL_NULL)
			VM.vm_main_args->userdef_module_init((ZL_VOID * )&VM); //调用用户自定义的模块初始化函数
		retcode = VM.run.main((ZL_VOID * )&VM);
	}
	else
	{
		if(VM.compile.isDestroyed == ZL_FALSE) //当发生编译错误时，在调试模式下，就需要在此释放资源
			VM.compile.memFreeAll((ZL_VOID * )&VM);
		VM.run.memFreeAll((ZL_VOID * )&VM);
	}
	VM.end_time = ZENGL_SYS_TIME_CLOCK();
	VM.total_time = VM.end_time - VM.start_time; //得到虚拟机总的执行时间
	return retcode;
}

/*创建并返回新的虚拟机指针，该指针必须通过zenglApi_Close进行释放，否则会发生内存泄漏*/
ZL_EXPORT ZL_EXP_VOID * zenglApi_Open()
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)ZENGL_SYS_MEM_ALLOC(sizeof(ZENGL_VM_TYPE));
	ZENGL_EXPORT_VM_MAIN_ARGS * vm_main_args = (ZENGL_EXPORT_VM_MAIN_ARGS *)ZENGL_SYS_MEM_ALLOC(sizeof(ZENGL_EXPORT_VM_MAIN_ARGS));
	(*VM) = ZL_Api_Const_VM;
	ZENGL_SYS_MEM_SET(vm_main_args,0,sizeof(ZENGL_EXPORT_VM_MAIN_ARGS));
	VM->init((ZL_VOID * )VM,vm_main_args);
	return (ZL_EXP_VOID *)VM;
}

/*使用zenglApi_Open打开的虚拟机来运行script_file对应的脚本*/
ZL_EXPORT ZL_EXP_INT zenglApi_Run(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * script_file)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_SYS_ARG_LIST arg = ZL_NULL;
	ZL_INT retcode;
	if(VM->isinApiRun == ZL_TRUE)
	{
		VM->errorno = ZL_ERR_VM_API_VM_MUST_RESET;
		VM->run.makeInfoString(VM_ARG,&VM->run.errorFullString , VM->errorString[VM->errorno] , arg);
		VM->isRunError = ZL_TRUE;
		return -1;
	}
	VM->isinApiRun = ZL_TRUE;
	if(VM->run.mempool.isInit == ZL_FALSE)
		VM->run.init(VM_ARG); //编译器中需要对解释器输出汇编指令，所以在此初始化解释器
	if(script_file == ZL_NULL)
	{
		VM->errorno = ZL_ERR_VM_API_INVALID_SCRIPT_NAME;
		VM->run.makeInfoString(VM_ARG,&VM->run.errorFullString , VM->errorString[VM->errorno] , arg);
		VM->isRunError = ZL_TRUE;
		return -1;
	}
	retcode = VM->compile.main(VM_ARG,script_file,VM->vm_main_args);
	if(retcode == 0) //如果编译成功，则进入解释器
	{
		if(VM->vm_main_args->userdef_module_init != ZL_NULL)
			VM->vm_main_args->userdef_module_init(VM_ARG); //调用用户自定义的模块初始化函数
		retcode = VM->run.main(VM_ARG);
	}
	VM->end_time = ZENGL_SYS_TIME_CLOCK();
	VM->total_time = VM->end_time - VM->start_time; //得到虚拟机总的执行时间
	return retcode;
}

/*关闭zenglApi_Open打开的虚拟机指针，并释放虚拟机占用的系统资源*/
ZL_EXPORT ZL_EXP_VOID zenglApi_Close(ZL_EXP_VOID * VM_ARG)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	if(VM->compile.isDestroyed == ZL_FALSE)
		VM->compile.memFreeAll(VM_ARG);
	if(VM->run.isDestroyed == ZL_FALSE)
		VM->run.memFreeAll(VM_ARG);
	ZENGL_SYS_MEM_FREE((ZL_VOID *)VM->vm_main_args);
	ZENGL_SYS_MEM_FREE(VM_ARG);
}

/*API接口，重置参数对应的虚拟机*/
ZL_EXPORT ZL_EXP_VOID * zenglApi_Reset(ZL_EXP_VOID * VM_ARG)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_EXPORT_VM_MAIN_ARGS * vm_main_args = VM->vm_main_args;
	if(VM->compile.isDestroyed == ZL_FALSE)
		VM->compile.memFreeAll(VM_ARG);
	if(VM->run.isDestroyed == ZL_FALSE)
		VM->run.memFreeAll(VM_ARG);
	(*VM) = ZL_Api_Const_VM;
	VM->vm_main_args = vm_main_args;
	return (ZL_EXP_VOID *)VM;
}

/*API接口，返回编译运行时的出错信息*/
ZL_EXPORT ZL_EXP_CHAR * zenglApi_GetErrorString(ZL_EXP_VOID * VM_ARG)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	if(VM->errorno == ZL_NO_ERR_SUCCESS)
		return (ZL_EXP_CHAR *)VM->errorString[VM->errorno];
	else if(VM->isCompileError == ZL_TRUE)
		return VM->compile.errorFullString.str;
	else if(VM->isRunError == ZL_TRUE)
		return VM->run.errorFullString.str;
	return ZL_NULL;
}

/*API接口，设置虚拟机运行的一些选项标记*/
ZL_EXPORT ZL_EXP_VOID zenglApi_SetFlags(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_VM_MAIN_ARG_FLAGS flag)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	VM->vm_main_args->flags = flag;
	return;
}

/*API接口，设置虚拟机一些可选的操作句柄，例如PRINT指令对应的用户自定义输出函数等*/
ZL_EXPORT ZL_EXP_VOID zenglApi_SetHandle(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_VM_FLAGS_HANDLE_TYPE handleType,ZL_EXP_VOID * handle)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
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
	return;
}

/*API接口，将用户自定义的参数压入虚拟栈中*/
ZL_EXPORT ZL_EXP_INT zenglApi_Push(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_MOD_FUN_ARG_TYPE type,ZL_EXP_CHAR * arg_str,ZL_EXP_INT arg_integer,ZL_EXP_DOUBLE arg_floatnum)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_RUN_VIRTUAL_MEM_STRUCT tmpmem;  //临时的虚拟内存变量。
	if(run->mempool.isInit == ZL_FALSE)
	{
		run->init(VM_ARG);
	}
	if(run->vstack_list.count == 0)
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
		return -1;
		break;
	}
	return 0;
}

/*API接口，调用脚本中的某函数*/
ZL_EXPORT ZL_EXP_INT zenglApi_Call(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * script_file,ZL_EXP_CHAR * function_name,ZL_EXP_CHAR * class_name)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_RUN_VIRTUAL_MEM_STRUCT tmpmem;  //临时的虚拟内存变量。
	ZL_INT retcode,tmpFunID,tmpClassID;
	ZENGL_SYS_ARG_LIST arg = ZL_NULL;
	if(VM->isinApiRun == ZL_TRUE)
	{
		VM->errorno = ZL_ERR_VM_API_VM_MUST_RESET;
		VM->run.makeInfoString(VM_ARG,&VM->run.errorFullString , VM->errorString[VM->errorno] , arg);
		VM->isRunError = ZL_TRUE;
		return -1;
	}
	VM->isinApiRun = ZL_TRUE;
	if(run->mempool.isInit == ZL_FALSE)
		VM->run.init(VM_ARG); //编译器中需要对解释器输出汇编指令，所以在此初始化解释器
	if(script_file == ZL_NULL)
	{
		VM->errorno = ZL_ERR_VM_API_INVALID_SCRIPT_NAME;
		VM->run.makeInfoString(VM_ARG,&VM->run.errorFullString , VM->errorString[VM->errorno] , arg);
		VM->isRunError = ZL_TRUE;
		return -1;
	}

	retcode = VM->compile.main(VM_ARG,script_file,VM->vm_main_args);
	if(retcode == 0) //如果编译成功，则进入解释器
	{
		if(VM->vm_main_args->userdef_module_init != ZL_NULL)
			VM->vm_main_args->userdef_module_init(VM_ARG); //调用用户自定义的模块初始化函数
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
	VM->total_time = VM->end_time - VM->start_time; //得到虚拟机总的执行时间
	return retcode;
}

/*API接口，通过此接口获取某个变量值的字符串格式*/
ZL_EXPORT ZL_EXP_CHAR * zenglApi_GetValueAsString(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * valueName)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_COMPILE_TYPE * compile = &VM->compile;
	ZL_INT memloc = -1;
	ZENGL_RUN_VIRTUAL_MEM_STRUCT tmpmem;  //临时的虚拟内存变量。
	ZL_CHAR tmpstr[80];
	ZENGL_SYS_ARG_LIST arg = ZL_NULL;
	VM->run.freeInfoString(VM_ARG,&VM->run.errorFullString);
	if((VM->vm_main_args->flags & ZL_EXP_CP_AF_IN_DEBUG_MODE) == 0)
	{
		VM->errorno = ZL_ERR_VM_API_GET_VALUE_AS_MUST_IN_DEBUG_MODE;
		VM->run.makeInfoString(VM_ARG,&VM->run.errorFullString , VM->errorString[VM->errorno] , arg);
		VM->isRunError = ZL_TRUE;
		return ZL_NULL;
	}
	memloc = compile->SymLookupID_ByName(VM_ARG,valueName);
	if(memloc == -1)
	{
		VM->errorno = ZL_ERR_VM_API_GET_VALUE_AS_VALUE_NAME_INVALID;
		VM->run.makeInfoString(VM_ARG,&VM->run.errorFullString , VM->errorString[VM->errorno] , arg);
		VM->isRunError = ZL_TRUE;
		return ZL_NULL;
	}
	else if(memloc == -2)
	{
		VM->errorno = ZL_ERR_VM_API_SYM_GLOBAL_TABLE_INVALID_INDEX;
		VM->run.makeInfoString(VM_ARG,&VM->run.errorFullString , VM->errorString[VM->errorno] , arg);
		VM->isRunError = ZL_TRUE;
		return ZL_NULL;
	}
	switch(run->vmem_list.mem_array[memloc].runType)
	{
	case ZL_R_RDT_INT:
		ZENGL_SYS_SPRINTF(tmpstr,"%d",run->vmem_list.mem_array[memloc].val.dword);
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
	VM->errorno = ZL_ERR_VM_API_GET_VALUE_AS_VALUE_RUNTYPE_INVALID;
	VM->run.makeInfoString(VM_ARG,&VM->run.errorFullString , VM->errorString[VM->errorno] , arg);
	VM->isRunError = ZL_TRUE;
	return ZL_NULL;
}

/*API接口，通过此接口获取某个变量值的整数格式*/
ZL_EXPORT ZL_EXP_INT zenglApi_GetValueAsInt(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * valueName,ZL_EXP_INT * retValue)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_COMPILE_TYPE * compile = &VM->compile;
	ZL_INT memloc = -1;
	ZENGL_SYS_ARG_LIST arg = ZL_NULL;
	VM->run.freeInfoString(VM_ARG,&VM->run.errorFullString);
	if((VM->vm_main_args->flags & ZL_EXP_CP_AF_IN_DEBUG_MODE) == 0)
	{
		VM->errorno = ZL_ERR_VM_API_GET_VALUE_AS_MUST_IN_DEBUG_MODE;
		VM->run.makeInfoString(VM_ARG,&VM->run.errorFullString , VM->errorString[VM->errorno] , arg);
		VM->isRunError = ZL_TRUE;
		return -1;
	}
	memloc = compile->SymLookupID_ByName(VM_ARG,valueName);
	if(memloc == -1)
	{
		VM->errorno = ZL_ERR_VM_API_GET_VALUE_AS_VALUE_NAME_INVALID;
		VM->run.makeInfoString(VM_ARG,&VM->run.errorFullString , VM->errorString[VM->errorno] , arg);
		VM->isRunError = ZL_TRUE;
		return -1;
	}
	else if(memloc == -2)
	{
		VM->errorno = ZL_ERR_VM_API_SYM_GLOBAL_TABLE_INVALID_INDEX;
		VM->run.makeInfoString(VM_ARG,&VM->run.errorFullString , VM->errorString[VM->errorno] , arg);
		VM->isRunError = ZL_TRUE;
		return -1;
	}
	switch(run->vmem_list.mem_array[memloc].runType)
	{
	case ZL_R_RDT_INT:
		(*retValue) = run->vmem_list.mem_array[memloc].val.dword;
		return 0;
		break;
	case ZL_R_RDT_FLOAT:
		(*retValue) = (ZL_INT)run->vmem_list.mem_array[memloc].val.qword;
		return 0;
		break;
	case ZL_R_RDT_STR:
		(*retValue) = ZENGL_SYS_STR_TO_NUM((ZL_CONST ZL_EXP_CHAR *)run->vmem_list.mem_array[memloc].val.str);
		return 0;
		break;
	}
	VM->errorno = ZL_ERR_VM_API_GET_VALUE_AS_VALUE_RUNTYPE_INVALID;
	VM->run.makeInfoString(VM_ARG,&VM->run.errorFullString , VM->errorString[VM->errorno] , arg);
	VM->isRunError = ZL_TRUE;
	return -1;
}

/*API接口，通过此接口获取某个变量值的浮点数格式*/
ZL_EXPORT ZL_EXP_INT zenglApi_GetValueAsDouble(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * valueName,ZL_EXP_DOUBLE * retValue)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_COMPILE_TYPE * compile = &VM->compile;
	ZL_INT memloc = -1;
	ZENGL_SYS_ARG_LIST arg = ZL_NULL;
	VM->run.freeInfoString(VM_ARG,&VM->run.errorFullString);
	if((VM->vm_main_args->flags & ZL_EXP_CP_AF_IN_DEBUG_MODE) == 0)
	{
		VM->errorno = ZL_ERR_VM_API_GET_VALUE_AS_MUST_IN_DEBUG_MODE;
		VM->run.makeInfoString(VM_ARG,&VM->run.errorFullString , VM->errorString[VM->errorno] , arg);
		VM->isRunError = ZL_TRUE;
		return -1;
	}
	memloc = compile->SymLookupID_ByName(VM_ARG,valueName);
	if(memloc == -1)
	{
		VM->errorno = ZL_ERR_VM_API_GET_VALUE_AS_VALUE_NAME_INVALID;
		VM->run.makeInfoString(VM_ARG,&VM->run.errorFullString , VM->errorString[VM->errorno] , arg);
		VM->isRunError = ZL_TRUE;
		return -1;
	}
	else if(memloc == -2)
	{
		VM->errorno = ZL_ERR_VM_API_SYM_GLOBAL_TABLE_INVALID_INDEX;
		VM->run.makeInfoString(VM_ARG,&VM->run.errorFullString , VM->errorString[VM->errorno] , arg);
		VM->isRunError = ZL_TRUE;
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
	VM->errorno = ZL_ERR_VM_API_GET_VALUE_AS_VALUE_RUNTYPE_INVALID;
	VM->run.makeInfoString(VM_ARG,&VM->run.errorFullString , VM->errorString[VM->errorno] , arg);
	VM->isRunError = ZL_TRUE;
	return -1;
}

/*
	API接口，用户可以通过此接口定义模块初始化函数
*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetModInitHandle(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * moduleName,ZL_VM_API_MOD_INIT_FUNC module_init_function)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZL_INT h = run->Hash(VM_ARG,moduleName) + ZL_R_HASH_SIZE * ZL_R_HASH_TYPE_MODULE_TABLE;
	ZL_INT tmpindex = run->HashTable[h];

	if(run->mempool.isInit == ZL_FALSE)
	{
		run->init(VM_ARG);
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
		run->exit(VM_ARG,ZL_ERR_RUN_MODULE_HAS_BEEN_SET_HANDLE_BEFORE,moduleName,moduleName);
	return -1;
}

/*
	API接口，用户通过此接口可以自定义某模块中的函数处理句柄
*/
ZL_EXPORT ZL_EXP_VOID zenglApi_SetModFunHandle(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT moduleID,ZL_EXP_CHAR * functionName,ZL_VM_API_MOD_FUN_HANDLE handle)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZL_INT h = run->Hash(VM_ARG,functionName) + ZL_R_HASH_SIZE * ZL_R_HASH_TYPE_MOD_FUN_TABLE;
	ZL_INT tmpindex = run->HashTable[h];

	if(run->mempool.isInit == ZL_FALSE)
	{
		run->init(VM_ARG);
	}
	while(tmpindex != 0 && run->ModFunTable.mod_funs[tmpindex].isvalid == ZL_TRUE && 
		ZENGL_SYS_STRCMP(functionName,run->InstDataStringPoolGetPtr(VM_ARG,run->ModFunTable.mod_funs[tmpindex].strIndex)) != 0)
		tmpindex = run->ModFunTable.mod_funs[tmpindex].next;
	if(tmpindex == 0)
	{
		tmpindex = run->HashTable[h];
		run->HashTable[h] = run->InsertModFunTable(VM_ARG,moduleID,functionName,handle);
		run->ModFunTable.mod_funs[run->HashTable[h]].next = tmpindex;
		return ;
	}
	else if(run->ModFunTable.mod_funs[tmpindex].isvalid == ZL_FALSE)
		run->exit(VM_ARG,ZL_ERR_RUN_MOD_FUN_TABLE_FIND_INVALID_INDEX);
	else
	{
		ZL_CHAR * moduleName = run->InstDataStringPoolGetPtr(VM_ARG,run->moduleTable.modules[run->ModFunTable.mod_funs[tmpindex].moduleID].strIndex);
		run->exit(VM_ARG,ZL_ERR_RUN_MOD_FUN_HAS_BEEN_SET_HANDLE_IN_OTHER_MOD_BEFORE,functionName,moduleName,functionName,moduleName);
	}
}

/*
	API接口，获取脚本中模块函数调用时的参数信息
	argnum 为参数位置，从1开始，1代表第一个参数，2代表第二个参数，以此类推
*/
ZL_EXPORT ZL_EXP_VOID zenglApi_GetFunArg(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argnum,ZENGL_EXPORT_MOD_FUN_ARG * retval)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_RUN_VIRTUAL_MEM_STRUCT tmpmem = {0};  //临时的虚拟内存变量。
	ZL_INT argcount = ZENGL_RUN_REGVAL(ZL_R_RT_LOC).dword - ZENGL_RUN_REGVAL(ZL_R_RT_ARG).dword - 1;
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
	case ZL_R_RDT_MEM_BLOCK: //内存块
		retval->type = ZL_EXP_FAT_MEMBLOCK;
		retval->val.memblock.ptr = tmpmem.val.memblock;
		retval->val.memblock.index = tmpmem.memblk_Index;
		break;
	case ZL_R_RDT_ADDR: //全局变量引用，该函数理论上应该获取不到引用类型，只有下面的GetFunArgInfo才能获取到引用类型，因为run->VStackListOps函数会自动递归查找引用对应的变量的值，不过还是先把代码放在这里
		retval->type = ZL_EXP_FAT_ADDR;
		retval->val.addr.index = tmpmem.val.dword;
		break;
	case ZL_R_RDT_ADDR_LOC: //局部变量引用
		retval->type = ZL_EXP_FAT_ADDR_LOC;
		retval->val.addr.index = tmpmem.val.dword;
		break;
	case ZL_R_RDT_ADDR_MEMBLK: //内存块引用
		retval->type = ZL_EXP_FAT_ADDR_MEMBLK;
		retval->val.addr.index = tmpmem.val.dword;
		retval->val.addr.memblock.ptr = tmpmem.val.memblock;
		retval->val.addr.memblock.index = tmpmem.memblk_Index;
		break;
	}
}

/*
	API接口，退出当前脚本
*/
ZL_EXPORT ZL_EXP_VOID zenglApi_Exit(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * errorStr, ...)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_COMPILE_TYPE * compile = &VM->compile;
	ZENGL_RUN_TYPE * run = &VM->run;
	ZENGL_SYS_ARG_LIST arg;
	ZL_INT error_nodenum = 0;
	ZL_INT isNeedDebugInfo = ZL_EXP_CP_AF_IN_DEBUG_MODE | ZL_EXP_CP_AF_OUTPUT_DEBUG_INFO;
	VM->errorno = ZL_ERR_VM_API_USER_DEFINED_ERROR;

	ZENGL_SYS_ARG_START(arg,errorStr);
	run->makeInfoString(VM_ARG,&run->errorFullString , VM->errorString[VM->errorno] , arg);
	run->makeInfoString(VM_ARG,&run->errorFullString , errorStr , arg);
	if((VM->vm_main_args->flags & ZL_EXP_CP_AF_IN_DEBUG_MODE) != 0) //用户自定义的调试模式下，打印出节点和行列号信息
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
		run->userdef_run_error(run->errorFullString.str,run->errorFullString.count);
	if(VM->isinApiRun == ZL_FALSE)
		run->freeInfoString(VM_ARG,&run->errorFullString);
	ZENGL_SYS_ARG_END(arg);

	run->end_time = ZENGL_SYS_TIME_CLOCK();
	run->total_time = run->end_time - run->start_time;
	if(VM->errorno != ZL_NO_ERR_SUCCESS)
		VM->isRunError = ZL_TRUE;
	if((VM->vm_main_args->flags & isNeedDebugInfo) == isNeedDebugInfo) //用户自定义的调试模式下
	{
		VM->end_time = ZENGL_SYS_TIME_CLOCK();
		VM->total_time = VM->end_time - VM->start_time; //得到虚拟机总的执行时间
		run->info(VM_ARG,"\n run time:%.16g s totalsize: %.16g Kbyte\n VM time:%.16g s totalsize: %.16g Kbyte\n\n",(ZL_DOUBLE)run->total_time / CLOCKS_PER_SEC,
		(ZL_FLOAT)run->totalsize / 1024,
		(ZL_DOUBLE)VM->total_time / CLOCKS_PER_SEC,
		(ZL_FLOAT)VM->totalsize / 1024); //for debug
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
}

/*API接口，设置模块函数的返回值*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetRetVal(ZL_EXP_VOID * VM_ARG,
										 ZENGL_EXPORT_MOD_FUN_ARG_TYPE type,ZL_EXP_CHAR * arg_str,ZL_EXP_INT arg_integer,ZL_EXP_DOUBLE arg_floatnum)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run = &VM->run;
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

/*API接口，将返回值设为内存块*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetRetValAsMemBlock(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_MEMBLOCK * memblock)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run = &VM->run;
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

/*API接口，根据size参数创建足够容纳size个元素的内存块(内存块大小不一定等于size，只是大于等于size)，数组，类变量都属于内存块*/
ZL_EXPORT ZL_EXP_INT zenglApi_CreateMemBlock(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_MEMBLOCK * memblock,ZL_EXP_INT size)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run = &VM->run;
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

/*设置内存块中index - 1索引对应的值，index参数为1表示数组等内存块的第一个元素，以此类推，返回0表示成功，返回-1表示retval类型错误*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetMemBlock(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_MEMBLOCK * memblock,ZL_EXP_INT index,ZENGL_EXPORT_MOD_FUN_ARG * retval)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_RUN_VIRTUAL_MEM_STRUCT tmpmem = {0};
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
		return -1;
		break;
	}
	return 0;
}

/*获取数组等内存块中的index - 1索引处的元素，index参数为1表示数组等内存块的第一个元素，以此类推*/
ZL_EXPORT ZENGL_EXPORT_MOD_FUN_ARG zenglApi_GetMemBlock(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_MEMBLOCK * memblock,ZL_EXP_INT index)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_RUN_VIRTUAL_MEM_STRUCT tmpmem = {0};
	ZENGL_EXPORT_MOD_FUN_ARG retval = {0};
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

/*获取第argnum个参数的类型等信息，argnum从1开始表示第一个参数，之前的GetFunArg函数只能获取参数的值，如果参数是引用，则直接递归获取引用的变量的值，
所以无法知道参数的类型信息，比如无法知道某个参数是否是引用等类型，该函数则可以获取到这些信息，如果参数不是引用类型，则这两个函数等价*/
ZL_EXPORT ZL_EXP_VOID zenglApi_GetFunArgInfo(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argnum,ZENGL_EXPORT_MOD_FUN_ARG * retval)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_RUN_VIRTUAL_MEM_STRUCT tmpmem = {0};  //临时的虚拟内存变量。
	ZL_INT argcount = ZENGL_RUN_REGVAL(ZL_R_RT_LOC).dword - ZENGL_RUN_REGVAL(ZL_R_RT_ARG).dword - 1;
	ZL_INT index;
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
	case ZL_R_RDT_MEM_BLOCK: //内存块
		retval->type = ZL_EXP_FAT_MEMBLOCK;
		retval->val.memblock.ptr = tmpmem.val.memblock;
		retval->val.memblock.index = tmpmem.memblk_Index;
		break;
	case ZL_R_RDT_ADDR: //全局变量引用，该函数理论上应该获取不到引用类型，只有下面的GetFunArgInfo才能获取到引用类型，因为run->VStackListOps函数会自动递归查找引用对应的变量的值，不过还是先把代码放在这里
		retval->type = ZL_EXP_FAT_ADDR;
		retval->val.addr.index = tmpmem.val.dword;
		break;
	case ZL_R_RDT_ADDR_LOC: //局部变量引用
		retval->type = ZL_EXP_FAT_ADDR_LOC;
		retval->val.addr.index = tmpmem.val.dword;
		break;
	case ZL_R_RDT_ADDR_MEMBLK: //内存块引用
		retval->type = ZL_EXP_FAT_ADDR_MEMBLK;
		retval->val.addr.index = tmpmem.val.dword;
		retval->val.addr.memblock.ptr = tmpmem.val.memblock;
		retval->val.addr.memblock.index = tmpmem.memblk_Index;
		break;
	}
}

/*设置脚本模块函数中第argnum个参数的值，argnum从1开始表示第一个参数*/
ZL_EXPORT ZL_EXP_VOID zenglApi_SetFunArg(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argnum,ZENGL_EXPORT_MOD_FUN_ARG * retval)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_RUN_VIRTUAL_MEM_STRUCT tmpmem = {0};  //临时的虚拟内存变量。
	ZL_INT argcount = ZENGL_RUN_REGVAL(ZL_R_RT_LOC).dword - ZENGL_RUN_REGVAL(ZL_R_RT_ARG).dword - 1;
	ZL_INT index;
	if(argnum < 1 || argnum > argcount)
		run->exit(VM_ARG,ZL_ERR_VM_API_INVALID_ARGNUM_ARG_IN_SET_FUN_ARG);
	index = ZENGL_RUN_REGVAL(ZL_R_RT_ARG).dword + argnum - 1;
	switch(retval->type)
	{
	case ZL_EXP_FAT_NONE:
		tmpmem.runType = ZL_R_RDT_NONE;
		tmpmem.val.dword = retval->val.integer;
		run->VStackListOps(VM_ARG,ZL_R_VMOPT_SETMEM_NONE,index,tmpmem,ZL_TRUE);
		break;
	case ZL_EXP_FAT_INT:
		tmpmem.runType = ZL_R_RDT_INT;
		tmpmem.val.dword = retval->val.integer;
		run->VStackListOps(VM_ARG,ZL_R_VMOPT_SETMEM_INT,index,tmpmem,ZL_TRUE);
		break;
	case ZL_EXP_FAT_FLOAT:
		tmpmem.runType = ZL_R_RDT_FLOAT;
		tmpmem.val.qword = retval->val.floatnum;
		run->VStackListOps(VM_ARG,ZL_R_VMOPT_SETMEM_DOUBLE,index,tmpmem,ZL_TRUE);
		break;
	case ZL_EXP_FAT_STR:
		tmpmem.runType = ZL_R_RDT_STR;
		tmpmem.val.str = (ZL_VOID *)retval->val.str;
		run->VStackListOps(VM_ARG,ZL_R_VMOPT_SETMEM_STR,index,tmpmem,ZL_TRUE);
		break;
	case ZL_EXP_FAT_MEMBLOCK:
		tmpmem.runType = ZL_R_RDT_MEM_BLOCK;
		tmpmem.val.memblock = retval->val.memblock.ptr;
		tmpmem.memblk_Index = retval->val.memblock.index;
		run->VStackListOps(VM_ARG,ZL_R_VMOPT_SETMEM_MEMBLOCK,index,tmpmem,ZL_TRUE);
		break;
	case ZL_EXP_FAT_ADDR:
		tmpmem.runType = ZL_R_RDT_ADDR;
		tmpmem.val.dword = retval->val.addr.index;
		run->VStackListOps(VM_ARG,ZL_R_VMOPT_SETMEM_ADDR,index,tmpmem,ZL_TRUE);
		break;
	case ZL_EXP_FAT_ADDR_LOC:
		tmpmem.runType = ZL_R_RDT_ADDR_LOC;
		tmpmem.val.dword = retval->val.addr.index;
		run->VStackListOps(VM_ARG,ZL_R_VMOPT_SETMEM_ADDR,index,tmpmem,ZL_TRUE);
		break;
	case ZL_EXP_FAT_ADDR_MEMBLK:
		tmpmem.runType = ZL_R_RDT_ADDR_MEMBLK;
		tmpmem.val.dword = retval->val.addr.index;
		tmpmem.val.memblock = retval->val.addr.memblock.ptr;
		tmpmem.memblk_Index = retval->val.addr.memblock.index;
		run->VStackListOps(VM_ARG,ZL_R_VMOPT_SETMEM_ADDR,index,tmpmem,ZL_TRUE);
		break;
	}
}

/*获取内存块的size容量等信息*/
ZL_EXPORT ZL_EXP_VOID zenglApi_GetMemBlockInfo(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_MEMBLOCK * memblock,ZL_EXP_INT * mblk_size,ZL_EXP_INT * mblk_count)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_RUN_VIRTUAL_MEM_LIST * tmpMemBlock;
	if(memblock == ZL_NULL)
		run->exit(VM_ARG,ZL_ERR_VM_API_INVALID_MEMBLOCK_ARG_IN_GET_MEM_BLOCK_SIZE);
	tmpMemBlock = (ZENGL_RUN_VIRTUAL_MEM_LIST *)memblock->ptr;
	if(mblk_size != ZL_NULL)
		(*mblk_size) = tmpMemBlock->size;
	if(mblk_count != ZL_NULL)
		(*mblk_count) = tmpMemBlock->count;
}
