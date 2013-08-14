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
	该文件为虚拟机解释器的辅助函数库文件。
*/

#include "zengl_global.h"

/*
	解释器初始化
*/
ZL_VOID zenglrun_init(ZL_VOID * VM_ARG)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run = &VM->run;
	ZL_INT increase = 0;
	run->mempool.size = ZL_R_MEM_POOL_SIZE;
	run->mempool.count = 1; //设为1，这样就只有大于0的索引才是有效的内存池索引。
	run->mempool.points = ZENGL_SYS_MEM_ALLOC(run->mempool.size * sizeof(ZENGL_RUN_MEM_POOL_POINT_TYPE)); //为内存池分配内存空间
	if(run->mempool.points == ZL_NULL)
	{
		run->exit(VM_ARG,ZL_ERR_RUN_MEM_POOL_MALLOC_FAILED);
	}
	else
		ZENGL_SYS_MEM_SET(run->mempool.points,0,run->mempool.size * sizeof(ZENGL_RUN_MEM_POOL_POINT_TYPE));
	increase = run->mempool.size * sizeof(ZENGL_RUN_MEM_POOL_POINT_TYPE);
	run->mempool.totalsize += increase; //增加内存池内存占用量统计数
	run->totalsize += increase;
	VM->totalsize += increase;
	run->mempool.isInit = ZL_TRUE;
	run->memfreepool.size = ZL_R_MEM_FREE_POOL_SIZE;//初始化内存释放池的动态数组。
	run->memfreepool.count = 0;
	run->memfreepool.frees = run->memAlloc(VM_ARG,run->memfreepool.size * sizeof(ZL_INT) , &run->memfreepool.mempool_index);
	if(run->memfreepool.frees == ZL_NULL)
		run->exit(VM_ARG,ZL_ERR_RUN_MEM_FREE_POOL_MALLOC_FAILED);
	else
		ZENGL_SYS_MEM_SET(run->memfreepool.frees,0,run->memfreepool.size * sizeof(ZL_INT));
	run->userdef_run_info = VM->vm_main_args->userdef_run_info;
	run->userdef_run_print = VM->vm_main_args->userdef_run_print;
	run->userdef_run_error = VM->vm_main_args->userdef_run_error;
}

/*
	因为memfreepool中存放的是之前在内存池中释放过的指针信息，所以如果在memfreepool中存在已释放过的指针，
	就找到该指针在内存池中的索引，这样就可以直接利用原来分配过的内存作为新的指针空间，避免了内存开销，
	只有在没找到释放过的内存时，才分配新的内存。
	第一个参数size是要需求的内存大小。
	第二个参数index是分配的内存池索引。
*/
ZL_VOID * zenglrun_memAlloc(ZL_VOID * VM_ARG,ZL_INT size,ZL_INT * index)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run = &VM->run;
	ZL_INT i = 0;
	ZL_INT increase = 0;
	ZL_VOID * point;
	if(run->mempool.count == run->mempool.size) //如果内存池超过范围则进行动态扩容。
	{
		run->mempool.size += ZL_R_MEM_POOL_SIZE;
		increase += ZL_R_MEM_POOL_SIZE * sizeof(ZENGL_RUN_MEM_POOL_POINT_TYPE);
		run->mempool.points = ZENGL_SYS_MEM_RE_ALLOC(run->mempool.points,run->mempool.size * sizeof(ZENGL_RUN_MEM_POOL_POINT_TYPE));
		ZENGL_SYS_MEM_SET((ZL_VOID *)(run->mempool.points + (run->mempool.size - ZL_R_MEM_POOL_SIZE)),0,
						  (ZL_SIZE_T)increase);
	}
	if(run->memfreepool.count == 0 && run->mempool.points[run->mempool.count].isvalid == ZL_FALSE &&
		run->mempool.points[run->mempool.count].point == ZL_NULL) //如果没找到释放过的内存，则使用内存池中最后一个元素的后面作为索引即run->mempool.count
	{
		i = run->mempool.count;
		goto alloc_point;
	}
	else if(run->memfreepool.count > 0 && 
		   (i = run->memfreepool.frees[run->memfreepool.count - 1]) > 0) //找到释放过的内存，则直接回收利用。
	{
		run->memfreepool.frees[run->memfreepool.count - 1] = 0;
		run->memfreepool.count--;
		if(run->mempool.points[i].point == ZL_NULL && run->mempool.points[i].isvalid == ZL_FALSE) //如果该元素因某些未知原因不可用，就只有重新分配。
			goto alloc_point;
		else if(run->mempool.points[i].point != ZL_NULL && run->mempool.points[i].isvalid == ZL_FALSE)
		{
			if(run->mempool.points[i].size < size) //如果原来释放过的内存大小不能满足要求，则只有重新分配
			{
				ZENGL_SYS_MEM_FREE(run->mempool.points[i].point);
				increase -= run->mempool.points[i].size;
				goto alloc_point;
			}
			else if(run->mempool.points[i].size >= size &&
					run->mempool.points[i].size <= (size*3/2)) //如果原来释放过的内存大小介于函数需要的大小的1到1.5倍的话，就直接拿来用。
			{
				(*index) = i;
				run->mempool.points[i].isvalid = ZL_TRUE;
				run->mempool.count++;
				return run->mempool.points[i].point;
			}
			else //如果原内存大小超过需求的1.5倍，即过大时，就使用realloc调整内存分配的大小。
			{
				(*index) = i;
				run->mempool.points[i].isvalid = ZL_TRUE;
				run->mempool.points[i].point = ZENGL_SYS_MEM_RE_ALLOC(run->mempool.points[i].point,size);
				increase += size - run->mempool.points[i].size;
				run->mempool.points[i].size = size;
				run->mempool.count++;
				run->mempool.totalsize += increase;
				run->totalsize += increase;
				VM->totalsize += increase;
				return run->mempool.points[i].point;
			}
		}
	}
	for(i=0;i<run->mempool.size;i++) //只有在上面的方法都找不到时，才循环查找可利用的内存池索引。
	{
		if(run->mempool.points[i].point == ZL_NULL && run->mempool.points[i].isvalid == ZL_FALSE)
		{
alloc_point:
			point = ZENGL_SYS_MEM_ALLOC(size);
			if(point != ZL_NULL)
			{
				run->mempool.points[i].point = point;
				run->mempool.points[i].size = size;
				run->mempool.points[i].isvalid = ZL_TRUE;
				(*index) = i;
				run->mempool.count++;
				increase += size;
				run->mempool.totalsize += increase;
				run->totalsize += increase;
				VM->totalsize += increase;
				return point;
			}
			else
				run->exit(VM_ARG,ZL_ERR_RUN_MEM_ALLOC_FUNC_FAILED);
		}
	}
	run->exit(VM_ARG,ZL_ERR_RUN_MEM_ALLOC_FUNC_CAN_NOT_FIND_NULL_POINT);
	return ZL_NULL;
}

/*
	调整point指针的大小为size，同时设置index为该指针在内存池中的索引。
*/
ZL_VOID * zenglrun_memReAlloc(ZL_VOID * VM_ARG , ZL_VOID * point , ZL_INT size, ZL_INT * index)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run = &VM->run;
	ZL_INT i;
	ZL_INT increase = 0;
	if(point == ZL_NULL || (i = (*index)) <= 0) //当point为NULL或索引小于等于0，则新分配一个指针和内存。
	{
		point = run->memAlloc(VM_ARG,size,index);
		return point;
	}

	if(i > 0 && run->mempool.points[i].point == point && run->mempool.points[i].isvalid == ZL_TRUE) //如果index索引指向的内存池指针等于point，则无需循环遍历，直接进行realloc操作。
		goto realloc_point;
	for(i=0;i<run->mempool.size;i++) //否则，就需要循环遍历内存池
	{
		if(run->mempool.points[i].point == point)
		{
			if(run->mempool.points[i].isvalid == ZL_FALSE)
				run->exit(VM_ARG,ZL_ERR_RUN_MEM_RE_ALLOC_INVALID_POINT);
realloc_point:
			point = ZENGL_SYS_MEM_RE_ALLOC(point,size);
			if(point != ZL_NULL)
			{
				run->mempool.points[i].point = point;
				increase += size - run->mempool.points[i].size;
				run->mempool.points[i].size = size;
				(*index) = i;
				run->mempool.totalsize += increase;
				run->totalsize += increase;
				VM->totalsize += increase;
				return point;
			}
			else
				run->exit(VM_ARG,ZL_ERR_RUN_MEM_RE_ALLOC_FUNC_FAILED);
		}
	}
	point = run->memAlloc(VM_ARG,size,index); //如果没找到，则重新分配。
	return point;
}

/*
	根据需求的size大小，对内存池的指针进行调整大小等重利用操作。与memReAlloc区别在于，本函数只调整大小，没有内存copy的开销
*/
ZL_VOID * zenglrun_memReUsePtr(ZL_VOID * VM_ARG,ZL_VOID * point,ZL_INT size,ZL_INT * index)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run = &VM->run;
	ZL_INT i;
	ZL_INT increase = 0;
	if(point == ZL_NULL || (i = (*index)) <= 0) //当point为NULL或索引小于等于0，则新分配一个指针和内存。
	{
		point = run->memAlloc(VM_ARG,size,index);
		return point;
	}

	if(i > 0 && run->mempool.points[i].point == point && run->mempool.points[i].isvalid == ZL_TRUE) //如果index索引指向的内存池指针等于point，则直接调整
	{
		if(run->mempool.points[i].size < size) //原始大小小于需求则重新分配
		{
			ZENGL_SYS_MEM_FREE(run->mempool.points[i].point);
			point = ZENGL_SYS_MEM_ALLOC(size);
			if(point != ZL_NULL)
			{
				run->mempool.points[i].point = point;
				increase += size - run->mempool.points[i].size;
				run->mempool.points[i].size = size;
				run->mempool.totalsize += increase;
				run->totalsize += increase;
				VM->totalsize += increase;
				return run->mempool.points[i].point;
			}
			else
				run->exit(VM_ARG,ZL_ERR_RUN_MEM_RE_USE_PTR_MALLOC_FAILED);
		}
		else if(run->mempool.points[i].size >= size && 
				run->mempool.points[i].size <= (size*3/2)) //介于需求的1到1.5倍，则不需要调整直接返回。
		{
			return run->mempool.points[i].point;
		}
		else //过大则realloc调整内存大小。realloc在从大改到小时，不会进行真正的分配，只会修改内存的大小参数。一般内存指针不会发生变化，也不会有copy开销。
		{
			run->mempool.points[i].point = ZENGL_SYS_MEM_RE_ALLOC(run->mempool.points[i].point , size);
			increase += size - run->mempool.points[i].size;
			run->mempool.points[i].size = size;
			run->mempool.totalsize += increase;
			run->totalsize += increase;
			VM->totalsize += increase;
			return run->mempool.points[i].point;
		}
	}
	point = run->memAlloc(VM_ARG,size,index);
	return point;
}

/*
	解释器退出函数
*/
ZL_VOID zenglrun_exit(ZL_VOID * VM_ARG,ZENGL_ERRORNO errorno, ...)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_COMPILE_TYPE * compile = &VM->compile;
	ZENGL_RUN_TYPE * run = &VM->run;
	ZENGL_SYS_ARG_LIST arg;
	ZL_INT error_nodenum = 0;
	ZL_INT isNeedDebugInfo = ZL_EXP_CP_AF_IN_DEBUG_MODE | ZL_EXP_CP_AF_OUTPUT_DEBUG_INFO;
	if(compile->isinCompiling) //如果是在编译器编译中，比如编译器向解释器输出汇编指令时，此时就需要在释放解释器相关资源后还需要调用编译器的退出函数，以释放编译器的资源
	{
		VM->errorno = errorno;
		if(VM->errorno != ZL_NO_ERR_SUCCESS)
		{
			ZENGL_SYS_ARG_START(arg,errorno);
			compile->makeInfoString(VM_ARG,&compile->errorFullString, VM->errorString[VM->errorno] , arg);
			compile->makeInfoString(VM_ARG,&compile->errorFullString, ": " , arg);
			ZENGL_SYS_ARG_END(arg);
		}
		if(VM->isinApiRun == ZL_FALSE) //如果是zenglApi_Run运行的脚本，则不在此释放资源，而是通过zenglApi_Close来释放
		{
			run->memFreeAll(VM_ARG);
			run->isDestroyed = ZL_TRUE;
			compile->exit(VM_ARG,ZL_ERR_RUN_ERROR_EXIT_WHEN_IN_COMPILE);
		}
	}
	else
	{
		VM->errorno = errorno;
		if(VM->errorno != ZL_NO_ERR_SUCCESS)
		{
			ZENGL_SYS_ARG_START(arg,errorno);
			run->makeInfoString(VM_ARG,&run->errorFullString , VM->errorString[VM->errorno] , arg);
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
		}
		else
		{
			if(!compile->isDestroyed && VM->isinApiRun == ZL_FALSE)
			{
				compile->memFreeAll(VM_ARG);
				compile->infoFullString.str = compile->errorFullString.str = ZL_NULL;
				compile->isDestroyed = ZL_TRUE;
			}
		}
		run->end_time = ZENGL_SYS_TIME_CLOCK();
		run->total_time = run->end_time - run->start_time;
		if(VM->errorno != ZL_NO_ERR_SUCCESS)
			VM->isRunError = ZL_TRUE;
		if((VM->vm_main_args->flags & isNeedDebugInfo) == isNeedDebugInfo) //用户自定义的调试模式下
		{
			VM->end_time = ZENGL_SYS_TIME_CLOCK();
			VM->total_time = VM->end_time - VM->start_time; //得到虚拟机总的执行时间
			run->info(VM_ARG,"\n run time:%.16g s totalsize: %.16g Kbyte\n VM time:%.16g s totalsize: %.16g Kbyte\n",(ZL_DOUBLE)run->total_time / CLOCKS_PER_SEC,
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
}

/*
	当解释器出错退出时，打印出当前汇编代码对应的AST节点的行列号文件名信息
*/
ZL_VOID zenglrun_ExitPrintSourceInfo(ZL_VOID * VM_ARG,ZENGL_ERRORNO errorno, ...)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run = &VM->run;
	ZENGL_SYS_ARG_LIST arg;
	if(errorno != ZL_NO_ERR_SUCCESS)
	{
		ZENGL_SYS_ARG_START(arg,errorno);
		run->makeInfoString(VM_ARG,&run->errorFullString , VM->errorString[errorno] , arg);
		ZENGL_SYS_ARG_END(arg);
	}
}

/*解释器释放内存池资源*/
ZL_VOID zenglrun_memFreeAll(ZL_VOID * VM_ARG)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZL_INT i;
	if(run->mempool.isInit)
	{
		for(i=0;i<run->mempool.size;i++)
		{
			if(run->mempool.points[i].point != ZL_NULL)
				ZENGL_SYS_MEM_FREE(run->mempool.points[i].point);
		}
		ZENGL_SYS_MEM_FREE(run->mempool.points);
		run->mempool.isInit = ZL_FALSE;
	}
}

/*生成格式化信息字符串*/
ZL_CHAR * zenglrun_makeInfoString(ZL_VOID * VM_ARG,ZENGL_RUN_INFO_STRING_TYPE * infoStringPtr , ZL_CONST ZL_CHAR * format , ZENGL_SYS_ARG_LIST arglist)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZL_INT retcount = -1;
	if(infoStringPtr->str == ZL_NULL)
	{
		infoStringPtr->size = ZL_INFO_STRING_SIZE;
		infoStringPtr->str = run->memAlloc(VM_ARG,infoStringPtr->size * sizeof(ZL_CHAR),&infoStringPtr->mempool_index);
	}
	while(ZL_TRUE)
	{
		retcount = ZENGL_SYS_SPRINTF_ARG_NUM((infoStringPtr->str + infoStringPtr->cur),
											 (infoStringPtr->size - infoStringPtr->count),format,arglist);

		if(retcount >= 0 && retcount < (infoStringPtr->size - infoStringPtr->count))
		{
			infoStringPtr->count += retcount;
			infoStringPtr->cur = infoStringPtr->count;
			infoStringPtr->str[infoStringPtr->cur] = ZL_STRNULL;
			return infoStringPtr->str;
		}

		infoStringPtr->size += ZL_INFO_STRING_SIZE;
		infoStringPtr->str = run->memReAlloc(VM_ARG,infoStringPtr->str,infoStringPtr->size * sizeof(ZL_CHAR),&infoStringPtr->mempool_index);
	}
	return ZL_NULL;
}

/*释放格式化信息字符串的游标和大小信息*/
ZL_VOID zenglrun_freeInfoString(ZL_VOID * VM_ARG,ZENGL_RUN_INFO_STRING_TYPE * infoStringPtr)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	infoStringPtr->cur = infoStringPtr->count = 0;
	if(infoStringPtr->size > ZL_INFO_STRING_SIZE * 3) //如果超过基础大小3倍，那么调整到3倍，以免占用过多资源
	{
		infoStringPtr->size = ZL_INFO_STRING_SIZE * 3;
		infoStringPtr->str = run->memReAlloc(VM_ARG,infoStringPtr->str,infoStringPtr->size * sizeof(ZL_CHAR),&infoStringPtr->mempool_index);
	}
}

/*
	生成infoFullString或errorFullString的格式化字符串信息，并使用用户自定义函数进行输出显示
*/
ZL_VOID zenglrun_info(ZL_VOID * VM_ARG , ZL_CONST ZL_CHAR * format , ...)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_SYS_ARG_LIST arg;
	ZENGL_SYS_ARG_START(arg,format);
	if(run->errorFullString.count > 0 && ((ZENGL_VM_TYPE *)VM_ARG)->isRunError == ZL_FALSE) //在exit错误退出时，且正在输出错误信息时，使用errorFullString来输出。
	{
		run->makeInfoString(VM_ARG,&run->errorFullString,format,arg);
	}
	else //否则，使用普通方式进行输出
	{
		run->makeInfoString(VM_ARG,&run->infoFullString,format,arg);
		if(run->userdef_run_info != ZL_NULL)
			run->userdef_run_info(run->infoFullString.str , run->infoFullString.count);
		run->freeInfoString(VM_ARG,&run->infoFullString);
	}
	ZENGL_SYS_ARG_END(arg);
}

/*
	生成printFullString的格式化字符串信息，并使用用户自定义函数输出显示
*/
ZL_VOID zenglrun_print(ZL_VOID * VM_ARG , ZL_CONST ZL_CHAR * format , ...)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_SYS_ARG_LIST arg;
	ZENGL_SYS_ARG_START(arg,format);
	run->makeInfoString(VM_ARG,&run->printFullString,format,arg);
	if(run->userdef_run_print != ZL_NULL)
		run->userdef_run_print(run->printFullString.str , run->printFullString.count);
	run->freeInfoString(VM_ARG,&run->printFullString);
	ZENGL_SYS_ARG_END(arg);
}

/*
	指令操作数字符串池添加字符串
*/
ZL_INT zenglrun_InstDataStringPoolAdd(ZL_VOID * VM_ARG , ZL_CHAR * src)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZL_INT len;
	ZL_INT i,j;
	if(!run->InstData_StringPool.isInit)
		run->initInstDataStringPool(VM_ARG);
	if(src == ZL_NULL)
		return -1;
	len = ZENGL_SYS_STRLEN(src);
	if(run->InstData_StringPool.count == run->InstData_StringPool.size ||
		run->InstData_StringPool.count + len + 1 > run->InstData_StringPool.size)
	{
		run->InstData_StringPool.size += ZL_R_INST_DATA_STRING_POOL_SIZE;
		run->InstData_StringPool.ptr = run->memReAlloc(VM_ARG,run->InstData_StringPool.ptr,
												run->InstData_StringPool.size * sizeof(ZL_CHAR),
												&run->InstData_StringPool.mempool_index);
		ZENGL_SYS_MEM_SET(run->InstData_StringPool.ptr + (run->InstData_StringPool.size - ZL_R_INST_DATA_STRING_POOL_SIZE),0,
						ZL_R_INST_DATA_STRING_POOL_SIZE * sizeof(ZL_CHAR));
	}
	for(i=run->InstData_StringPool.count,j=0;
		i<run->InstData_StringPool.size && j<len;i++,j++)
	{
		run->InstData_StringPool.ptr[i] = src[j];
	}
	if(i >= run->InstData_StringPool.size)
		run->exit(VM_ARG,ZL_ERR_RUN_INST_DATA_STR_POOL_ADD_I_OUT_OF_BOUNDS);
	else
		run->InstData_StringPool.ptr[i] = ZL_STRNULL;
	i = run->InstData_StringPool.count;
	run->InstData_StringPool.count += len + 1;
	return i;
}

/*
	指令操作数字符串池的初始化
*/
ZL_VOID zenglrun_initInstDataStringPool(ZL_VOID * VM_ARG)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	if(run->InstData_StringPool.isInit)
		return;
	run->InstData_StringPool.count = 2;
	run->InstData_StringPool.size = ZL_R_INST_DATA_STRING_POOL_SIZE;
	run->InstData_StringPool.ptr = run->memAlloc(VM_ARG,run->InstData_StringPool.size * sizeof(ZL_CHAR),
													&run->InstData_StringPool.mempool_index);
	if(run->InstData_StringPool.ptr == ZL_NULL)
		run->exit(VM_ARG,ZL_ERR_RUN_INST_DATA_STRING_POOL_MALLOC_FAILED);
	ZENGL_SYS_MEM_SET(run->InstData_StringPool.ptr, 0 , run->InstData_StringPool.size * sizeof(ZL_CHAR));
	run->InstData_StringPool.isInit = ZL_TRUE;
}

/*
	根据索引值从字符串池中查找指令操作数字符串的指针信息
*/
ZL_CHAR * zenglrun_InstDataStringPoolGetPtr(ZL_VOID * VM_ARG , ZL_INT index)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	if(index < 2 || index > run->InstData_StringPool.count -1)
		return ZL_NULL;
	return run->InstData_StringPool.ptr + index;
}

/*
	使用C库的strcat会破坏内存池，所以就自建一个zenglrun_strcat，连接字符串用的，同时在必要时会更新内存池指针从而不破坏内存池。
*/
ZL_VOID * zenglrun_strcat(ZL_VOID * VM_ARG,ZL_CHAR * dest,ZL_INT * destIndex,ZL_CHAR * src)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZL_INT lensrc;
	ZL_INT lendest;
	ZL_INT index,i;

	if(dest == ZL_NULL)
		lendest = 0;
	else
		lendest = ZENGL_SYS_STRLEN(dest);
	if(src == ZL_NULL)
		lensrc = 0;
	else
		lensrc = ZENGL_SYS_STRLEN(src);
	dest = run->memReAlloc(VM_ARG,dest,(lendest+lensrc+1) * sizeof(ZL_CHAR),destIndex);
	for(index = lendest,i=0;index< (lendest + lensrc) && i<lensrc ;index++,i++)
	{
		dest[index] = src[i];
	}
	dest[index] = ZL_STRNULL;
	return dest;
}

/*
	释放内存池中指定索引对应的指针。
	将释放过的索引存放到memfreepool的frees动态数组中(以便将来的重利用)。
*/
ZL_VOID zenglrun_memFreeIndex(ZL_VOID * VM_ARG,ZL_VOID * point,ZL_INT * srcindex)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZL_INT index = (*srcindex);
	if(run->mempool.points[index].isvalid == ZL_TRUE && run->mempool.points[index].point == point)
	{
		run->mempool.points[index].isvalid = ZL_FALSE; //将内存池该索引的有效位设为ZL_FALSE
		run->mempool.count--;
		run->mempool.count = run->mempool.count < 1 ? 1 : run->mempool.count; //大于0才是有效的解释器内存池索引，即最小为1
		if(run->memfreepool.count == run->memfreepool.size)
		{
			run->memfreepool.size += ZL_R_MEM_FREE_POOL_SIZE;
			run->memfreepool.frees = run->memReAlloc(VM_ARG,run->memfreepool.frees,run->memfreepool.size * sizeof(ZL_INT),
													&run->memfreepool.mempool_index);
			ZENGL_SYS_MEM_SET((ZL_VOID *)(run->memfreepool.frees + (run->memfreepool.size - ZL_R_MEM_FREE_POOL_SIZE)),0,
					(ZL_SIZE_T)(ZL_R_MEM_FREE_POOL_SIZE * sizeof(ZL_INT)));
		}
		run->memfreepool.frees[run->memfreepool.count++] = index;
		(*srcindex) = 0;
		return;
	}
	else //如果不能通过index直接定位到内存池的具体位置，则只有通过memFreeOnce函数进行循环遍历内存池。
	{
		run->memFreeOnce(VM_ARG,point);
		(*srcindex) = 0;
	}
}

/*
	释放内存池中的某个指针，并将该指针的索引存放到memfreepool的frees动态数组中(以便将来的重利用)。
*/
ZL_VOID zenglrun_memFreeOnce(ZL_VOID * VM_ARG,ZL_VOID * point)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZL_INT i = 0;
	for(;i < run->mempool.size;i++)
	{
		if(run->mempool.points[i].isvalid == ZL_TRUE && run->mempool.points[i].point == point)
		{
			run->mempool.points[i].isvalid = ZL_FALSE;
			run->mempool.count--;
			run->mempool.count = run->mempool.count < 1 ? 1 : run->mempool.count; //大于0才是有效的解释器内存池索引，即最小为1
			if(run->memfreepool.count == run->memfreepool.size)
			{
				run->memfreepool.size += ZL_R_MEM_FREE_POOL_SIZE;
				run->memfreepool.frees = run->memReAlloc(VM_ARG,run->memfreepool.frees,run->memfreepool.size * sizeof(ZL_INT),
														&run->memfreepool.mempool_index);
				ZENGL_SYS_MEM_SET((ZL_VOID *)(run->memfreepool.frees + (run->memfreepool.size - ZL_R_MEM_FREE_POOL_SIZE)),0,
						(ZL_SIZE_T)(ZL_R_MEM_FREE_POOL_SIZE * sizeof(ZL_INT)));
			}
			run->memfreepool.frees[run->memfreepool.count++] = i;
			break;
		}
	}
}

/*
	将src1和src2两字符串连接在一起设置dest。
*/
ZL_VOID * zenglrun_strcat2(ZL_VOID * VM_ARG,ZL_CHAR * dest,ZL_INT * destIndex,ZL_CHAR * src1,ZL_CHAR * src2)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZL_INT lensrc1,lensrc2;
	ZL_CHAR * tmpdest;
	if(src1 == ZL_NULL)
		lensrc1 = 0;
	else
		lensrc1 = ZENGL_SYS_STRLEN(src1);
	if(src2 == ZL_NULL)
		lensrc2 = 0;
	else
		lensrc2 = ZENGL_SYS_STRLEN(src2);
	dest = run->memReUsePtr(VM_ARG,dest,(lensrc1+lensrc2+1) * sizeof(ZL_CHAR),destIndex);
	ZENGL_SYS_STRNCPY(dest,src1,lensrc1);
	tmpdest = dest + lensrc1;
	ZENGL_SYS_STRNCPY(tmpdest,src2,lensrc2);
	dest[lensrc1+lensrc2] = ZL_STRNULL;
	return dest;
}

/*
	用于计算解释器中的哈希表索引
*/
ZL_INT zenglrun_Hash(ZL_VOID * VM_ARG,ZL_CHAR * key)
{
	ZL_INT temp = 0;
	ZL_INT i = 0;
	while (key[i] != ZL_STRNULL)
	{
		temp = ((temp << ZL_R_HASH_SHIFT) + key[i]) % ZL_R_HASH_SIZE;
		++i;
	}
	return temp;
}

/*
	将某模块名插入到模块动态数组中
*/
ZL_INT zenglrun_InsertModuleTable(ZL_VOID * VM_ARG,ZL_CHAR * moduleName,ZL_VM_API_MOD_INIT_FUNC module_init_function)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZL_INT index;
	if(!run->moduleTable.isInit)
		run->initModuleTable(VM_ARG);
	index = run->moduleTable.count;
	if(run->moduleTable.modules[index].isvalid == ZL_FALSE)
	{
		run->moduleTable.modules[index].ID = index;
		run->moduleTable.modules[index].init_func = module_init_function;
		run->moduleTable.modules[index].strIndex = run->InstDataStringPoolAdd(VM_ARG,moduleName);
		run->moduleTable.modules[index].next = 0;
		run->moduleTable.modules[index].isvalid = ZL_TRUE;
		run->moduleTable.count++;
		return index;
	}
	else
		run->exit(VM_ARG,ZL_ERR_RUN_MODULE_TALBE_NO_NOT_VALID_INDEX);
	return -1;
}

/*
	模块动态数组初始化
*/
ZL_VOID zenglrun_initModuleTable(ZL_VOID * VM_ARG)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	if(run->moduleTable.isInit)
		return;
	run->moduleTable.size = ZL_R_MODULE_TABLE_SIZE;
	run->moduleTable.count = 1;
	run->moduleTable.modules = run->memAlloc(VM_ARG,run->moduleTable.size * sizeof(ZENGL_RUN_MODULE_TABLE_MEMBER),&run->moduleTable.mempool_index);
	if(run->moduleTable.modules == ZL_NULL)
		run->exit(VM_ARG,ZL_ERR_RUN_MODULE_TALBE_MALLOC_FAILED);
	ZENGL_SYS_MEM_SET(run->moduleTable.modules,0,run->moduleTable.size * sizeof(ZENGL_RUN_MODULE_TABLE_MEMBER));
	run->moduleTable.isInit = ZL_TRUE;
}

/*
	将模块函数信息插入到模块函数动态数组中
*/
ZL_INT zenglrun_InsertModFunTable(ZL_VOID * VM_ARG,ZL_INT moduleID,ZL_CHAR * functionName,ZL_VM_API_MOD_FUN_HANDLE handle)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZL_INT index;
	if(!run->ModFunTable.isInit)
		run->initModFunTable(VM_ARG);
	index = run->ModFunTable.count;
	if(run->ModFunTable.mod_funs[index].isvalid == ZL_FALSE)
	{
		run->ModFunTable.mod_funs[index].moduleID = moduleID;
		run->ModFunTable.mod_funs[index].strIndex = run->InstDataStringPoolAdd(VM_ARG,functionName);
		run->ModFunTable.mod_funs[index].handle = handle;
		run->ModFunTable.mod_funs[index].next = 0;
		run->ModFunTable.mod_funs[index].isvalid = ZL_TRUE;
		run->ModFunTable.count++;
		return index;
	}
	else
		run->exit(VM_ARG,ZL_ERR_RUN_MOD_FUN_TABLE_NO_NOT_VALID_INDEX);
	return -1;
}

/*
	模块函数动态数组初始化
*/
ZL_VOID zenglrun_initModFunTable(ZL_VOID * VM_ARG)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	if(run->ModFunTable.isInit)
		return;
	run->ModFunTable.size = ZL_R_MOD_FUN_TABLE_SIZE;
	run->ModFunTable.count = 1;
	run->ModFunTable.mod_funs = run->memAlloc(VM_ARG,run->ModFunTable.size * sizeof(ZENGL_RUN_MOD_FUN_TABLE_MEMBER),&run->ModFunTable.mempool_index);
	if(run->ModFunTable.mod_funs == ZL_NULL)
		run->exit(VM_ARG,ZL_ERR_RUN_MOD_FUN_TABLE_MALLOC_FAILED);
	ZENGL_SYS_MEM_SET(run->ModFunTable.mod_funs,0,run->ModFunTable.size * sizeof(ZENGL_RUN_MOD_FUN_TABLE_MEMBER));
	run->ModFunTable.isInit = ZL_TRUE;
}

/*
	查找某模块的信息，返回该模块在模块动态数组中的索引
*/
ZL_INT zenglrun_LookupModuleTable(ZL_VOID * VM_ARG,ZL_CHAR * moduleName)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZL_INT h = run->Hash(VM_ARG,moduleName) + ZL_R_HASH_SIZE * ZL_R_HASH_TYPE_MODULE_TABLE;
	ZL_INT tmpindex = run->HashTable[h];

	while(tmpindex != 0 && run->moduleTable.modules[tmpindex].isvalid == ZL_TRUE && 
		ZENGL_SYS_STRCMP(moduleName,run->InstDataStringPoolGetPtr(VM_ARG,run->moduleTable.modules[tmpindex].strIndex)) != 0)
		tmpindex = run->moduleTable.modules[tmpindex].next;
	if(tmpindex == 0)
		run->exit(VM_ARG,ZL_ERR_RUN_MODULE_IS_INVALID,moduleName,ZL_R_REG_PC,moduleName);
	else if(run->moduleTable.modules[tmpindex].isvalid == ZL_FALSE)
		run->exit(VM_ARG,ZL_ERR_RUN_MODULE_TABLE_FIND_INVALID_INDEX);
	else
		return tmpindex;
	return -1;
}

/*
	查找某模块函数的信息，返回该模块函数在动态数组中的索引
*/
ZL_INT zenglrun_LookupModFunTable(ZL_VOID * VM_ARG,ZL_CHAR * functionName)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZL_INT h = run->Hash(VM_ARG,functionName) + ZL_R_HASH_SIZE * ZL_R_HASH_TYPE_MOD_FUN_TABLE;
	ZL_INT tmpindex = run->HashTable[h];

	while(tmpindex != 0 && run->ModFunTable.mod_funs[tmpindex].isvalid == ZL_TRUE && 
		ZENGL_SYS_STRCMP(functionName,run->InstDataStringPoolGetPtr(VM_ARG,run->ModFunTable.mod_funs[tmpindex].strIndex)) != 0)
		tmpindex = run->ModFunTable.mod_funs[tmpindex].next;
	if(tmpindex == 0)
		run->exit(VM_ARG,ZL_ERR_RUN_FUNCTION_IS_INVALID,functionName,ZL_R_REG_PC,functionName);
	else if(run->ModFunTable.mod_funs[tmpindex].isvalid == ZL_FALSE)
		run->exit(VM_ARG,ZL_ERR_RUN_MOD_FUN_TABLE_FIND_INVALID_INDEX);
	else
		return tmpindex;
	return -1;
}
