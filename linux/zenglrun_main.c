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
	该文件为虚拟机解释器的主处理文件。
*/

#include "zengl_global.h"

/*
	添加汇编指令，使用ZL_DOUBLE传值，double占8个字节，可以代表8字节以内的值，再强制转换类型即可
*/
ZL_VOID zenglrun_AddInst(ZL_VOID * VM_ARG,ZL_INT pc,ZL_INT nodenum,ZENGL_RUN_INST_TYPE type, 
						 ZENGL_RUN_INST_OP_DATA_TYPE dest_type , ZL_DOUBLE dest_val ,
						 ZENGL_RUN_INST_OP_DATA_TYPE src_type , ZL_DOUBLE src_val)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZL_INT tmpint;
	if(!run->inst_list.isInit)
		run->initInstList(VM_ARG);
	if(run->inst_list.count == run->inst_list.size)
	{
		run->inst_list.size += ZL_R_INST_LIST_SIZE;
		run->inst_list.insts = run->memReAlloc(VM_ARG,run->inst_list.insts,run->inst_list.size * sizeof(ZENGL_RUN_INST_LIST_MEMBER),
												&run->inst_list.mempool_index);
		ZENGL_SYS_MEM_SET((run->inst_list.insts + (run->inst_list.size - ZL_R_INST_LIST_SIZE)),0,
					ZL_R_INST_LIST_SIZE * sizeof(ZENGL_RUN_INST_LIST_MEMBER));
	}
	if(run->inst_list.insts[run->inst_list.count].isvalid == ZL_FALSE)
	{
		run->inst_list.insts[run->inst_list.count].pc = pc;
		run->inst_list.insts[run->inst_list.count].nodenum = nodenum;
		run->inst_list.insts[run->inst_list.count].type = type;
		run->inst_list.insts[run->inst_list.count].dest.type = dest_type;
		switch(dest_type)
		{
		case ZL_R_DT_REG:
			run->inst_list.insts[run->inst_list.count].dest.val.reg = (ZENGL_RUN_REG_TYPE)dest_val;
			break;
		case ZL_R_DT_ARGMEM:
		case ZL_R_DT_LOCMEM:
		case ZL_R_DT_MEM:
			run->inst_list.insts[run->inst_list.count].dest.val.mem = (ZL_INT)dest_val;
			break;
		case ZL_R_DT_NUM:
			run->inst_list.insts[run->inst_list.count].dest.val.num = (ZL_INT)dest_val;
			break;
		case ZL_R_DT_FLOAT:
			run->inst_list.insts[run->inst_list.count].dest.val.floatnum = dest_val;
			break;
		case ZL_R_DT_STR:
			tmpint = (ZL_INT)dest_val;
			run->inst_list.insts[run->inst_list.count].dest.val.str_Index = run->InstDataStringPoolAdd(VM_ARG,(ZL_CHAR * )((ZL_LONG)tmpint)); //64位下需先转为long，再转为指针
			break;
		default:
			run->inst_list.insts[run->inst_list.count].dest.val.num = (ZL_INT)dest_val;
			break;
		}
		run->inst_list.insts[run->inst_list.count].src.type = src_type;
		switch(src_type)
		{
		case ZL_R_DT_REG:
			run->inst_list.insts[run->inst_list.count].src.val.reg = (ZENGL_RUN_REG_TYPE)src_val;
			break;
		case ZL_R_DT_ARGMEM:
		case ZL_R_DT_LOCMEM:
		case ZL_R_DT_MEM:
			run->inst_list.insts[run->inst_list.count].src.val.mem = (ZL_INT)src_val;
			break;
		case ZL_R_DT_NUM:
			run->inst_list.insts[run->inst_list.count].src.val.num = (ZL_INT)src_val;
			break;
		case ZL_R_DT_FLOAT:
			run->inst_list.insts[run->inst_list.count].src.val.floatnum = src_val;
			break;
		case ZL_R_DT_STR:
			tmpint = (ZL_INT)src_val;
			run->inst_list.insts[run->inst_list.count].src.val.str_Index = run->InstDataStringPoolAdd(VM_ARG,(ZL_CHAR * )((ZL_LONG)tmpint)); //64位下需先转为long，再转为指针
			break;
		default:
			run->inst_list.insts[run->inst_list.count].src.val.num = (ZL_INT)src_val;
			break;
		}
		run->inst_list.insts[run->inst_list.count].isvalid = ZL_TRUE;
		run->inst_list.count++;
	}
}

/*
	初始化解释器的汇编指令集
*/
ZL_VOID zenglrun_initInstList(ZL_VOID * VM_ARG)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	if(run->inst_list.isInit)
		return;
	run->inst_list.size = ZL_R_INST_LIST_SIZE;
	run->inst_list.count = 0;
	run->inst_list.insts = run->memAlloc(VM_ARG,run->inst_list.size * sizeof(ZENGL_RUN_INST_LIST_MEMBER),
											&run->inst_list.mempool_index);
	if(run->inst_list.insts == ZL_NULL)
		run->exit(VM_ARG,ZL_ERR_RUN_INST_LIST_MALLOC_FAILED);
	ZENGL_SYS_MEM_SET(run->inst_list.insts,0,run->inst_list.size * sizeof(ZENGL_RUN_INST_LIST_MEMBER));
	run->inst_list.isInit = ZL_TRUE;
}

/**
	虚拟机的虚拟内存操作函数
	第一个参数ZENGL_RUN_VMEM_OP_TYPE opType为内存操作类型
	如 ZL_R_VMOPT_GETMEM 为从某虚拟内存读取值。
*/
ZENGL_RUN_VIRTUAL_MEM_STRUCT zenglrun_VMemListOps(ZL_VOID * VM_ARG,ZENGL_RUN_VMEM_OP_TYPE opType,ZL_INT memloc,
												  ZENGL_RUN_VIRTUAL_MEM_STRUCT argval)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_RUN_VIRTUAL_MEM_STRUCT retmem = argval;
	ZL_VOID ** tmptr;
	ZL_INT len;
	if(!run->vmem_list.isInit)
		run->initVMemList(VM_ARG);
	if(run->vmem_list.count == run->vmem_list.size) //虚拟内存动态数组空间满了，就扩容内存。
	{
realloc:
		run->vmem_list.size += ZL_R_VMEM_LIST_SIZE;
		run->vmem_list.mem_array = run->memReAlloc(VM_ARG,run->vmem_list.mem_array,run->vmem_list.size * sizeof(ZENGL_RUN_VIRTUAL_MEM_STRUCT),
												&run->vmem_list.mempool_index);
		ZENGL_SYS_MEM_SET(run->vmem_list.mem_array + (run->vmem_list.size - ZL_R_VMEM_LIST_SIZE),0,
			ZL_R_VMEM_LIST_SIZE * sizeof(ZENGL_RUN_VIRTUAL_MEM_STRUCT));
	}
	if(memloc >= run->vmem_list.size) //如果内存寻址超过内存动态数组的大小，就扩容内存。
		goto realloc;

	if(opType != ZL_R_VMOPT_ADDMEM_ADDR && opType != ZL_R_VMOPT_SETMEM_ADDR) //如果不是设置某内存为引用的话，就进行下面的处理
	{
		switch(run->vmem_list.mem_array[memloc].runType)
		{
		case ZL_R_RDT_ADDR: //如果该数组元素是全局变量引用则调用VMemListOps
			return run->VMemListOps(VM_ARG,opType,run->vmem_list.mem_array[memloc].val.dword,argval);
			break;
		case ZL_R_RDT_ADDR_LOC: //局部变量引用则调用VStackListOps
			return run->VStackListOps(VM_ARG,opType,run->vmem_list.mem_array[memloc].val.dword,argval,ZL_TRUE);
			break;
		case ZL_R_RDT_ADDR_MEMBLK: //如果是另一个数组元素的引用，则调用VMemBlockOps
			return run->VMemBlockOps(VM_ARG,opType,run->vmem_list.mem_array[memloc].val.memblock,run->vmem_list.mem_array[memloc].val.dword,&argval);
			break;
		}
	}

	if(opType != ZL_R_VMOPT_GETMEM)
	{
		//如果所访问的内存原先是个数组之类的内存块的话，就将该内存块的内存空间释放掉
		if(run->vmem_list.mem_array[memloc].runType == ZL_R_RDT_MEM_BLOCK && run->vmem_list.mem_array[memloc].val.memblock != ZL_NULL)
		{
			run->memblock_free(VM_ARG,run->vmem_list.mem_array[memloc].val.memblock,
				&run->vmem_list.mem_array[memloc].memblk_Index);
			run->vmem_list.mem_array[memloc].val.memblock = ZL_NULL;
		}
		else if(run->vmem_list.mem_array[memloc].runType == ZL_R_RDT_ADDR_MEMBLK) //如果是内存块引用，则直接将memblock内存块指针设为ZL_NULL
			run->vmem_list.mem_array[memloc].val.memblock = ZL_NULL;
	}

	switch(opType) //执行不同的内存操作
	{
	case ZL_R_VMOPT_SETMEM_INT: //SETMEM的操作侧重于设置，而非添加
	case ZL_R_VMOPT_ADDMEM_INT: //ADDMEM的操作侧重于添加，设置memloc位置的内存值为整数
		run->vmem_list.mem_array[memloc].runType = ZL_R_RDT_INT;
		run->vmem_list.mem_array[memloc].val.dword = argval.val.dword;
		if(run->vmem_list.mem_array[memloc].isvalid == ZL_FALSE) //如果memloc所在虚拟内存没有被占用，则将vmem_list里的占用数加一
		{
			run->vmem_list.count++;
			run->vmem_list.mem_array[memloc].isvalid = ZL_TRUE;
		}
		break;
	case ZL_R_VMOPT_SETMEM_DOUBLE:
	case ZL_R_VMOPT_ADDMEM_DOUBLE: //设置memloc位置的内存值为浮点数
		run->vmem_list.mem_array[memloc].runType = ZL_R_RDT_FLOAT;
		run->vmem_list.mem_array[memloc].val.qword = argval.val.qword;
		if(run->vmem_list.mem_array[memloc].isvalid == ZL_FALSE)
		{
			run->vmem_list.count++;
			run->vmem_list.mem_array[memloc].isvalid = ZL_TRUE;
		}
		break;
	case ZL_R_VMOPT_SETMEM_STR:
	case ZL_R_VMOPT_ADDMEM_STR: //设置memloc位置的内存值为字符串
		run->vmem_list.mem_array[memloc].runType = ZL_R_RDT_STR;
		tmptr = &run->vmem_list.mem_array[memloc].val.str;
		len = ZENGL_SYS_STRLEN(argval.val.str);
		(*tmptr) = run->memReUsePtr(VM_ARG,(*tmptr),(len+1) * sizeof(ZL_CHAR),
									&run->vmem_list.mem_array[memloc].str_Index);
		ZENGL_SYS_STRNCPY(*tmptr,argval.val.str,len);
		(*((ZL_CHAR **)tmptr))[len] = ZL_STRNULL;
		if(run->vmem_list.mem_array[memloc].isvalid == ZL_FALSE)
		{
			run->vmem_list.count++;
			run->vmem_list.mem_array[memloc].isvalid = ZL_TRUE;
		}
		break;
	case ZL_R_VMOPT_SETMEM_MEMBLOCK:
	case ZL_R_VMOPT_ADDMEM_MEMBLOCK: //设置memloc位置的内存为内存块
		run->vmem_list.mem_array[memloc].runType = ZL_R_RDT_MEM_BLOCK; //将运行时类型设为ZL_R_RDT_MEM_BLOCK即数组之类的内存块
		run->assign_memblock(VM_ARG,&(run->vmem_list.mem_array[memloc].val.memblock),argval.val.memblock); //通过assign_memblock为memloc内存设为argval.val.memblock数组内存块
		run->vmem_list.mem_array[memloc].memblk_Index = argval.memblk_Index;
		if(run->vmem_list.mem_array[memloc].isvalid == ZL_FALSE)
		{
			run->vmem_list.count++;
			run->vmem_list.mem_array[memloc].isvalid = ZL_TRUE;
		}
		break;
	case ZL_R_VMOPT_SETMEM_ADDR:
	case ZL_R_VMOPT_ADDMEM_ADDR: //将memloc内存设置为另一个内存的引用
		run->vmem_list.mem_array[memloc].runType = argval.runType;
		run->vmem_list.mem_array[memloc].val.dword = argval.val.dword;
		if(argval.runType == ZL_R_RDT_ADDR_MEMBLK) //如果是内存块的引用，则将memblock字段设置为该内存块的指针memblock
		{
			run->vmem_list.mem_array[memloc].val.memblock = argval.val.memblock;
			run->vmem_list.mem_array[memloc].memblk_Index = argval.memblk_Index;
		}
		if(run->vmem_list.mem_array[memloc].isvalid == ZL_FALSE)
		{
			run->vmem_list.count++;
			run->vmem_list.mem_array[memloc].isvalid = ZL_TRUE;
		}
		break;
	case ZL_R_VMOPT_SETMEM_NONE:
	case ZL_R_VMOPT_ADDMEM_NONE: //将memloc内存设置为未初始化状态
		run->vmem_list.mem_array[memloc].runType = ZL_R_RDT_NONE;
		if(run->vmem_list.mem_array[memloc].isvalid == ZL_FALSE)
		{
			run->vmem_list.count++;
			run->vmem_list.mem_array[memloc].isvalid = ZL_TRUE;
		}
		break;
	case ZL_R_VMOPT_GETMEM: //获取memloc位置的内存值。
		retmem = run->vmem_list.mem_array[memloc];
		return retmem;
		break;
	}
	return retmem; //返回设置的内存值。
}

/**
	初始化全局虚拟内存动态数组
*/
ZL_VOID zenglrun_initVMemList(ZL_VOID * VM_ARG)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	if(run->vmem_list.isInit)
		return;
	run->vmem_list.size = ZL_R_VMEM_LIST_SIZE;
	run->vmem_list.count = 0;
	run->vmem_list.mem_array = (ZENGL_RUN_VIRTUAL_MEM_STRUCT *)run->memAlloc(VM_ARG,run->vmem_list.size * sizeof(ZENGL_RUN_VIRTUAL_MEM_STRUCT),
											&run->vmem_list.mempool_index); //根据size成员的大小和ZENGL_RUN_VIRTUAL_MEM_STRUCT(每个虚拟内存单元的结构体)的大小，申请得到mem_array虚拟内存数组的空间。
	if(run->vmem_list.mem_array == ZL_NULL)
		run->exit(VM_ARG,ZL_ERR_RUN_VMEM_LIST_MALLOC_FAILED);
	ZENGL_SYS_MEM_SET(run->vmem_list.mem_array,0,run->vmem_list.size * sizeof(ZENGL_RUN_VIRTUAL_MEM_STRUCT)); //将内存动态数组初始化为0
	run->vmem_list.isInit = ZL_TRUE;
}

/**
	虚拟堆栈空间的操作函数，虚拟机中堆栈的概念类似汇编语言里的堆栈概念。用于保存和恢复数据的地方。经常用于存放局部变量，函数的返回地址，寄存器的值等。
	参数valid用于判断是否需要将某栈内存设为占用状态，占用表示该栈内存已经被初始化，主要用于判断函数参数是否被初始化
*/
ZENGL_RUN_VIRTUAL_MEM_STRUCT zenglrun_VStackListOps(ZL_VOID * VM_ARG,ZENGL_RUN_VMEM_OP_TYPE opType,ZL_INT index,
												  ZENGL_RUN_VIRTUAL_MEM_STRUCT argval,ZL_BOOL valid)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_RUN_VIRTUAL_MEM_STRUCT retmem=argval;
	ZL_VOID ** tmptr;
	ZL_INT len;
	if(!run->vstack_list.isInit)
		run->initVStackList(VM_ARG);
	if(run->vstack_list.count == run->vstack_list.size) //动态数组数据装满了就扩容。
	{
		run->vstack_list.size += ZL_R_VSTACK_LIST_SIZE;
		run->vstack_list.mem_array = run->memReAlloc(VM_ARG,run->vstack_list.mem_array,
			run->vstack_list.size * sizeof(ZENGL_RUN_VIRTUAL_MEM_STRUCT),
			&run->vstack_list.mempool_index);
		ZENGL_SYS_MEM_SET(run->vstack_list.mem_array + (run->vstack_list.size - ZL_R_VSTACK_LIST_SIZE),0,
			ZL_R_VSTACK_LIST_SIZE * sizeof(ZENGL_RUN_VIRTUAL_MEM_STRUCT));
	}
	if(index < 0) //小于0属于压栈操作
	{
		switch(opType)
		{
		case ZL_R_VMOPT_ADDMEM_NONE:
			run->vstack_list.mem_array[run->vstack_list.count].runType = ZL_R_RDT_NONE;
			run->vstack_list.mem_array[run->vstack_list.count].isvalid = valid;
			run->vstack_list.count++;
			break;
		case ZL_R_VMOPT_ADDMEM_INT:
			run->vstack_list.mem_array[run->vstack_list.count].runType = ZL_R_RDT_INT;
			run->vstack_list.mem_array[run->vstack_list.count].val.dword = argval.val.dword;
			run->vstack_list.mem_array[run->vstack_list.count].isvalid = valid;
			run->vstack_list.count++;
			break;
		case ZL_R_VMOPT_ADDMEM_DOUBLE:
			run->vstack_list.mem_array[run->vstack_list.count].runType = ZL_R_RDT_FLOAT;
			run->vstack_list.mem_array[run->vstack_list.count].val.qword = argval.val.qword;
			run->vstack_list.mem_array[run->vstack_list.count].isvalid = valid;
			run->vstack_list.count++;
			break;
		case ZL_R_VMOPT_ADDMEM_STR:
			run->vstack_list.mem_array[run->vstack_list.count].runType = ZL_R_RDT_STR;
			tmptr = &run->vstack_list.mem_array[run->vstack_list.count].val.str;
			len = ZENGL_SYS_STRLEN(argval.val.str);
			(*tmptr) = run->memReUsePtr(VM_ARG,(*tmptr),(len+1) * sizeof(ZL_CHAR),
				&run->vstack_list.mem_array[run->vstack_list.count].str_Index);
			ZENGL_SYS_STRNCPY(*tmptr,argval.val.str,len);
			(*((ZL_CHAR **)tmptr))[len] = ZL_STRNULL;
			run->vstack_list.mem_array[run->vstack_list.count].isvalid = valid;
			run->vstack_list.count++;
			break;
		case ZL_R_VMOPT_ADDMEM_ADDR: //将另一内存的引用压入栈
			run->vstack_list.mem_array[run->vstack_list.count].runType = argval.runType;
			run->vstack_list.mem_array[run->vstack_list.count].val.dword = argval.val.dword;
			if(argval.runType == ZL_R_RDT_ADDR_MEMBLK)
			{
				run->vstack_list.mem_array[run->vstack_list.count].val.memblock = argval.val.memblock;
				run->vstack_list.mem_array[run->vstack_list.count].memblk_Index = argval.memblk_Index;
			}
			run->vstack_list.count++;
			break;
		case ZL_R_VMOPT_ADDMEM_MEMBLOCK: //通过assign_memblock将栈内存设为argval.val.memblock指向的内存块
			run->vstack_list.mem_array[run->vstack_list.count].runType = ZL_R_RDT_MEM_BLOCK;
			run->assign_memblock(VM_ARG,&(run->vstack_list.mem_array[run->vstack_list.count].val.memblock),
				argval.val.memblock);
			run->vstack_list.mem_array[run->vstack_list.count].memblk_Index = argval.memblk_Index;
			run->vstack_list.count++;
			break;
		case ZL_R_VMOPT_GETMEM: //获取堆栈的最后一个值，并弹出该值，有点像汇编的POP操作。
			if(run->vstack_list.count <= 0) //过分弹出数据或未知原因导致堆栈反向溢出。
				run->exit(VM_ARG,ZL_ERR_RUN_VSTACK_LIST_OVERFLOW,ZL_R_REG_PC,ZL_R_REG_PC);
			switch(run->vstack_list.mem_array[run->vstack_list.count - 1].runType)
			{
			case ZL_R_RDT_ADDR: //如果是全局变量内存的引用，就通过VMemListOps来访问
				retmem = run->VMemListOps(VM_ARG,opType,run->vstack_list.mem_array[run->vstack_list.count - 1].val.dword,argval);
				break;
			case ZL_R_RDT_ADDR_LOC: ////如果是局部变量栈内存的引用，就通过VStackListOps来访问
				retmem = run->VStackListOps(VM_ARG,opType,run->vstack_list.mem_array[run->vstack_list.count - 1].val.dword,argval,ZL_TRUE);
				break;
			default:
				retmem = run->vstack_list.mem_array[run->vstack_list.count - 1]; //如果不是引用，就将栈中最后一个元素返回
				break;
			}
			run->vstack_list.count--; //栈顶计数器减一。
			return retmem;
			break;
		}
	}
	else //如果index大于等于0，就对堆栈里的某个元素进行操作，主要是对函数的参数和局部变量进行操作时会用到。
	{
		if(run->is_inMemBlkSetVal == ZL_TRUE) //因为在设置数组元素的时候，值都是先放在栈中的，再从栈中提取出来，最后赋给数组元素的，当要赋值的是一个引用时，例如test[0] = &test，test的引用信息会先放在栈中，如果没有此处的代码做判断的话，就会在下面对引用进行递归，这样就得不到引用信息了，所以这里加了条判断 >>> 如果是设置数组元素时，就直接返回元素
			return run->vstack_list.mem_array[index];

		if(opType != ZL_R_VMOPT_ADDMEM_ADDR && opType != ZL_R_VMOPT_SETMEM_ADDR) //如果不是设置内存引用，就进行下面的处理
		{
			switch(run->vstack_list.mem_array[index].runType)
			{
			case ZL_R_RDT_ADDR: //如果是全局变量引用，就直接将操作交给VMemListOps
				return run->VMemListOps(VM_ARG,opType,run->vstack_list.mem_array[index].val.dword,argval);
				break;
			case ZL_R_RDT_ADDR_LOC: //如果是局部变量或参数的引用，就直接将操作交给VStackListOps，操作完后直接返回
				return run->VStackListOps(VM_ARG,opType,run->vstack_list.mem_array[index].val.dword,argval,ZL_TRUE);
				break;
			case ZL_R_RDT_ADDR_MEMBLK: //如果是内存块引用，则调用memop_array_addr
				return run->VMemBlockOps(VM_ARG,opType,run->vstack_list.mem_array[index].val.memblock,
					run->vstack_list.mem_array[index].val.dword,&argval);
				break;
			}
		}
		
		if(opType != ZL_R_VMOPT_GETMEM)
		{
			if(run->vstack_list.mem_array[index].runType == ZL_R_RDT_MEM_BLOCK && 
				run->vstack_list.mem_array[index].val.memblock != ZL_NULL) //如果栈中该元素之前是个数组之类的内存块，就将内存块进行释放
			{
				run->memblock_free(VM_ARG,run->vstack_list.mem_array[index].val.memblock,
					&run->vstack_list.mem_array[index].memblk_Index);
				run->vstack_list.mem_array[index].val.memblock = ZL_NULL;
			}
			else if(run->vstack_list.mem_array[index].runType == ZL_R_RDT_ADDR_MEMBLK) //如果是内存块引用，就直接将memblock内存块指针设为ZL_NULL
				run->vstack_list.mem_array[index].val.memblock = ZL_NULL;
		}

		switch(opType)
		{
		case ZL_R_VMOPT_SETMEM_NONE:
			run->vstack_list.mem_array[index].runType = ZL_R_RDT_NONE;
			break;
		case ZL_R_VMOPT_SETMEM_INT:
			run->vstack_list.mem_array[index].runType = ZL_R_RDT_INT;
			run->vstack_list.mem_array[index].val.dword = argval.val.dword;
			break;
		case ZL_R_VMOPT_SETMEM_DOUBLE:
			run->vstack_list.mem_array[index].runType = ZL_R_RDT_FLOAT;
			run->vstack_list.mem_array[index].val.qword = argval.val.qword;
			break;
		case ZL_R_VMOPT_SETMEM_STR:
			run->vstack_list.mem_array[index].runType = ZL_R_RDT_STR;
			tmptr = &run->vstack_list.mem_array[index].val.str;
			len = ZENGL_SYS_STRLEN(argval.val.str);
			(*tmptr) = run->memReUsePtr(VM_ARG,(*tmptr),(len+1) * sizeof(ZL_CHAR),
				&run->vstack_list.mem_array[index].str_Index);
			ZENGL_SYS_STRNCPY(*tmptr,argval.val.str,len);
			(*((ZL_CHAR **)tmptr))[len] = ZL_STRNULL;
			break;
		case ZL_R_VMOPT_SETMEM_ADDR: //设置为另一内存的引用
			run->vstack_list.mem_array[index].runType = argval.runType;
			run->vstack_list.mem_array[index].val.dword = argval.val.dword;
			if(argval.runType == ZL_R_RDT_ADDR_MEMBLK)
			{
				run->vstack_list.mem_array[index].val.memblock = argval.val.memblock;
				run->vstack_list.mem_array[index].memblk_Index = argval.memblk_Index;
			}
			break;
		case ZL_R_VMOPT_SETMEM_MEMBLOCK: //通过assign_memblock将栈内存设置为argval.val.memblock指向的内存块
			run->vstack_list.mem_array[index].runType = ZL_R_RDT_MEM_BLOCK;
			run->assign_memblock(VM_ARG,&(run->vstack_list.mem_array[index].val.memblock),argval.val.memblock);
			run->vstack_list.mem_array[index].memblk_Index = argval.memblk_Index;
			break;
		case ZL_R_VMOPT_GETMEM:
			retmem = run->vstack_list.mem_array[index];
			return retmem;
			break;
		}
	}
	return retmem; //返回设置的堆栈值。
}

/**
	初始化虚拟机的堆栈空间。
*/
ZL_VOID zenglrun_initVStackList(ZL_VOID * VM_ARG)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	if(run->vstack_list.isInit)
		return;
	run->vstack_list.size = ZL_R_VSTACK_LIST_SIZE;
	run->vstack_list.count = 0;
	run->vstack_list.mem_array = (ZENGL_RUN_VIRTUAL_MEM_STRUCT *)run->memAlloc(VM_ARG,run->vstack_list.size * sizeof(ZENGL_RUN_VIRTUAL_MEM_STRUCT),
		&run->vstack_list.mempool_index);
	if(run->vstack_list.mem_array == ZL_NULL)
		run->exit(VM_ARG,ZL_ERR_RUN_VSTACK_LIST_MALLOC_FAILED);
	ZENGL_SYS_MEM_SET(run->vstack_list.mem_array,0,run->vstack_list.size * sizeof(ZENGL_RUN_VIRTUAL_MEM_STRUCT)); //虚拟堆栈动态数组初始化为0
	run->vstack_list.isInit = ZL_TRUE;
}

/*
	打印输出指令集
*/
ZL_VOID zenglrun_printInstList(ZL_VOID * VM_ARG,ZL_CHAR * head_title)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_COMPILE_TYPE * compile = &VM->compile;
	ZENGL_RUN_TYPE * run = &VM->run;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZL_INT i,j,nodenum;
	ZENGL_RUN_INST_OP_DATA op_data;
	ZL_CLOCK_T start_time = ZENGL_SYS_TIME_CLOCK();
	ZL_CLOCK_T end_time;

	if(head_title != ZL_NULL) //打印调试标题
		run->info(VM_ARG,head_title);
	for(i=0;i<run->inst_list.count;i++)
	{
		run->info(VM_ARG,"%d %s ",run->inst_list.insts[i].pc , run->instTypeString[run->inst_list.insts[i].type]);
		for(j=1;j<=2;j++)
		{
			if(j==1)
				op_data = run->inst_list.insts[i].dest;
			else
				op_data = run->inst_list.insts[i].src;
			switch(op_data.type)
			{
			case ZL_R_DT_REG:
				run->info(VM_ARG,"%s ",run->registerTypeString[op_data.val.reg]);
				break;
			case ZL_R_DT_MEM:
				run->info(VM_ARG,"(%d) ",op_data.val.mem);
				break;
			case ZL_R_DT_ARGMEM:
				run->info(VM_ARG,"arg(%d) ",op_data.val.mem);
				break;
			case ZL_R_DT_LOCMEM:
				run->info(VM_ARG,"loc(%d) ",op_data.val.mem);
				break;
			case ZL_R_DT_NUM:
				run->info(VM_ARG,"%d ",op_data.val.num);
				break;
			case ZL_R_DT_FLOAT:
				run->info(VM_ARG,"%.16g ",op_data.val.floatnum);
				break;
			case ZL_R_DT_STR:
				run->info(VM_ARG,"\"%s\" ",run->InstData_StringPool.ptr + op_data.val.str_Index);
				break;
			case ZL_R_DT_LDADDR: //伪地址
				run->info(VM_ARG,"adr%d ",op_data.val.num);
				break;
			case ZL_R_DT_LDFUNID: //函数调用时，使用的函数ID，链接替换时会先转为伪地址
				run->info(VM_ARG,"funid%d ",op_data.val.num);
				break;
			}
		} //for(j=1;j<=2;j++)
		nodenum = run->inst_list.insts[i].nodenum;
		if(nodenum == -1)
			run->info(VM_ARG,"[%d]\n",nodenum);
		else
			run->info(VM_ARG,"[%d line:%d,col:%d,%s]\n",nodenum,nodes[nodenum].line_no,nodes[nodenum].col_no,nodes[nodenum].filename);
	} //for(i=0;i<run->inst_list.count;i++)
	end_time = ZENGL_SYS_TIME_CLOCK();
	compile->total_print_time += end_time - start_time;
}

/**
	虚拟机解释器执行汇编指令的主程式。
*/
ZL_VOID zenglrun_RunInsts(ZL_VOID * VM_ARG)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run = &VM->run;
	ZENGL_RUN_RUNTIME_OP_DATA src; //临时变量，用于存放源操作数等。
	ZENGL_RUN_VIRTUAL_MEM_STRUCT tmpmem;  //临时的虚拟内存变量。
	ZL_CHAR tmpchar[30]; //临时字符串数组
	ZENGL_API_STATES origState;
	while(ZL_R_CUR_INST.type != ZL_R_IT_END && run->isUserWantStop == ZL_FALSE) //根据PC寄存器的值来读取对应的汇编指令，如果当前指令不是END指令且用户没有要求停止脚本，就继续运行。
	{
		if(ZL_R_CUR_INST.isvalid == ZL_FALSE || 
			ZL_R_CUR_INST.pc != ZL_R_REG_PC)
			run->exit(VM_ARG,ZL_ERR_RUN_INST_INVALID_PC,ZL_R_CUR_INST.pc,ZL_R_REG_PC);

		src.runType = ZL_R_RDT_INT; //先将源操作数类型初始化为整形。
		switch(ZL_R_CUR_INST.type) //根据当前的指令类型执行不同的操作
		{
		case ZL_R_IT_MOV: //MOV指令根据原操作数和目标操作数来将数据从内存转到寄存器，或从寄存器转到内存等。
			switch(ZL_R_CUR_INST.src.type)
			{
			case ZL_R_DT_REG: //如果是寄存器，就将寄存器里的整数或浮点数或字符串赋值给src
				switch(ZENGL_RUN_REG(ZL_R_CUR_INST.src.val.reg).runType)
				{
				case ZL_R_RDT_INT:
					src.val.dword = ZENGL_RUN_REGVAL(ZL_R_CUR_INST.src.val.reg).dword;
					break;
				case ZL_R_RDT_FLOAT:
					src.runType = ZL_R_RDT_FLOAT;
					src.val.qword = ZENGL_RUN_REGVAL(ZL_R_CUR_INST.src.val.reg).qword;
					break;
				case ZL_R_RDT_STR:
					src.runType = ZL_R_RDT_STR;
					src.val.str = ZENGL_RUN_REGVAL(ZL_R_CUR_INST.src.val.reg).str;
					src.str_Index = ZENGL_RUN_REG(ZL_R_CUR_INST.src.val.reg).str_Index;
					break;
				case ZL_R_RDT_ADDR:
				case ZL_R_RDT_ADDR_LOC: //当原操作数为引用时
					src.runType = ZENGL_RUN_REG(ZL_R_CUR_INST.src.val.reg).runType;
					src.val.dword = ZENGL_RUN_REGVAL(ZL_R_CUR_INST.src.val.reg).dword;
					break;
				case ZL_R_RDT_MEM_BLOCK: //内存块
					src.runType = ZL_R_RDT_MEM_BLOCK;
					src.val.memblock = ZENGL_RUN_REGVAL(ZL_R_CUR_INST.src.val.reg).memblock;
					src.memblk_Index = ZENGL_RUN_REG(ZL_R_CUR_INST.src.val.reg).memblk_Index;
					break;
				case ZL_R_RDT_ADDR_MEMBLK: //内存块引用
					src.runType = ZL_R_RDT_ADDR_MEMBLK;
					src.val.dword = ZENGL_RUN_REGVAL(ZL_R_CUR_INST.src.val.reg).dword;
					src.val.memblock = ZENGL_RUN_REGVAL(ZL_R_CUR_INST.src.val.reg).memblock;
					src.memblk_Index = ZENGL_RUN_REG(ZL_R_CUR_INST.src.val.reg).memblk_Index;
					break;
				} //switch(ZENGL_RUN_REG(ZL_R_CUR_INST.src.val.reg).runType)
				break; //case ZL_R_DT_REG: //如果是寄存器......
			case ZL_R_DT_ARGMEM:
			case ZL_R_DT_LOCMEM:
			case ZL_R_DT_MEM: //如果是全局变量的内存
				ZENGL_RUN_VMEM_OP_GET(ZL_R_CUR_INST.src.type,tmpmem,src,ZL_ERR_RUN_INVALID_VMEM_TYPE)
				switch(tmpmem.runType)
				{
				case ZL_R_RDT_INT:
					src.val.dword = tmpmem.val.dword;
					break;
				case ZL_R_RDT_FLOAT:
					src.runType = ZL_R_RDT_FLOAT;
					src.val.qword = tmpmem.val.qword;
					break;
				case ZL_R_RDT_STR:
					src.runType = ZL_R_RDT_STR;
					src.val.str = tmpmem.val.str;
					src.str_Index = tmpmem.str_Index;
					break;
				case ZL_R_RDT_NONE: //该内存没有被赋值的情况
					src.val.dword = 0;
					break;
				case ZL_R_RDT_MEM_BLOCK: //内存块
					src.runType = ZL_R_RDT_MEM_BLOCK;
					src.val.memblock = tmpmem.val.memblock;
					src.memblk_Index = tmpmem.memblk_Index;
					break;
				}
				break; //case ZL_R_DT_MEM: //如果是全局变量的内存
			case ZL_R_DT_NUM: //MOV指令的源操作数为整数
				src.val.dword = ZL_R_CUR_INST.src.val.num;
				break;
			case ZL_R_DT_FLOAT:
				src.runType = ZL_R_RDT_FLOAT;
				src.val.qword = ZL_R_CUR_INST.src.val.floatnum;
				break;
			case ZL_R_DT_STR: //MOV指令的源操作数为字符串
				src.runType = ZL_R_RDT_STR;
				src.val.str = run->InstData_StringPool.ptr + ZL_R_CUR_INST.src.val.str_Index;
				break;
			default:
				run->exit(VM_ARG,ZL_ERR_RUN_INVALID_INST_SRC_TYPE,ZL_R_REG_PC);
				break;
			} //switch(ZL_R_CUR_INST.src.type)
			switch(ZL_R_CUR_INST.dest.type) //当前MOV指令的目标操作数
			{
			case ZL_R_DT_REG: //当前MOV指令的目标操作数为寄存器类型
				switch(src.runType)
				{
				case ZL_R_RDT_INT:
					ZENGL_RUN_REG(ZL_R_CUR_INST.dest.val.reg).runType = ZL_R_RDT_INT;
					ZENGL_RUN_REGVAL(ZL_R_CUR_INST.dest.val.reg).dword = src.val.dword;
					break;
				case ZL_R_RDT_FLOAT:
					ZENGL_RUN_REG(ZL_R_CUR_INST.dest.val.reg).runType = ZL_R_RDT_FLOAT;
					ZENGL_RUN_REGVAL(ZL_R_CUR_INST.dest.val.reg).qword = src.val.qword;
					break;
				case ZL_R_RDT_STR:
					ZENGL_RUN_REG(ZL_R_CUR_INST.dest.val.reg).runType = ZL_R_RDT_STR;
					run->RegAssignStr(VM_ARG,ZL_R_CUR_INST.dest.val.reg,src.val.str);
					break;
				case ZL_R_RDT_ADDR:
				case ZL_R_RDT_ADDR_LOC:
				case ZL_R_RDT_ADDR_MEMBLK: //如果源操作数是引用，就将引用的内存类型和内存索引赋值给目标寄存器
					ZENGL_RUN_REG(ZL_R_CUR_INST.dest.val.reg).runType = src.runType;
					ZENGL_RUN_REGVAL(ZL_R_CUR_INST.dest.val.reg).dword = src.val.dword;
					if(src.runType == ZL_R_RDT_ADDR_MEMBLK)
					{
						ZENGL_RUN_REGVAL(ZL_R_CUR_INST.dest.val.reg).memblock = src.val.memblock;
						ZENGL_RUN_REG(ZL_R_CUR_INST.dest.val.reg).memblk_Index = src.memblk_Index;
					}
					break;
				case ZL_R_RDT_MEM_BLOCK: //内存块
					ZENGL_RUN_REG(ZL_R_CUR_INST.dest.val.reg).runType = ZL_R_RDT_MEM_BLOCK;
					ZENGL_RUN_REGVAL(ZL_R_CUR_INST.dest.val.reg).memblock = src.val.memblock;  //内存块指针
					ZENGL_RUN_REG(ZL_R_CUR_INST.dest.val.reg).memblk_Index = src.memblk_Index; //内存块指针在内存池中的索引
					break;
				}
				break; //case ZL_R_DT_REG:
			case ZL_R_DT_ARGMEM:
			case ZL_R_DT_LOCMEM:
			case ZL_R_DT_MEM: //当前MOV指令的目标操作数为内存寻址类型
				switch(src.runType)
				{
				case ZL_R_RDT_INT:
					tmpmem.val.dword = src.val.dword;
					ZENGL_RUN_VMEM_OP(ZL_R_CUR_INST.dest.type,ZL_R_VMOPT_ADDMEM_INT,ZL_R_VMOPT_SETMEM_INT,tmpmem,dest,ZL_ERR_RUN_INVALID_VMEM_TYPE)
					break;
				case ZL_R_RDT_FLOAT:
					tmpmem.val.qword = src.val.qword;
					ZENGL_RUN_VMEM_OP(ZL_R_CUR_INST.dest.type,ZL_R_VMOPT_ADDMEM_DOUBLE,ZL_R_VMOPT_SETMEM_DOUBLE,tmpmem,dest,ZL_ERR_RUN_INVALID_VMEM_TYPE)
					break;
				case ZL_R_RDT_STR:
					tmpmem.val.str = src.val.str;
					ZENGL_RUN_VMEM_OP(ZL_R_CUR_INST.dest.type,ZL_R_VMOPT_ADDMEM_STR,ZL_R_VMOPT_SETMEM_STR,tmpmem,dest,ZL_ERR_RUN_INVALID_VMEM_TYPE)
					break;
				case ZL_R_RDT_ADDR:
				case ZL_R_RDT_ADDR_LOC:
				case ZL_R_RDT_ADDR_MEMBLK: //如果源操作数是引用，目标操作数是内存，则通过ZENGL_RUN_VMEM_OP宏进行操作，将目标内存设为引用
					tmpmem.runType = src.runType;
					tmpmem.val.dword = src.val.dword;
					if(src.runType == ZL_R_RDT_ADDR_MEMBLK)
					{
						tmpmem.val.memblock = src.val.memblock;
						tmpmem.memblk_Index = src.memblk_Index;
					}
					ZENGL_RUN_VMEM_OP(ZL_R_CUR_INST.dest.type,ZL_R_VMOPT_ADDMEM_ADDR,ZL_R_VMOPT_SETMEM_ADDR,tmpmem,dest,ZL_ERR_RUN_INVALID_VMEM_TYPE)
					break;
				case ZL_R_RDT_MEM_BLOCK:
					tmpmem.val.memblock = src.val.memblock;
					tmpmem.memblk_Index = src.memblk_Index;
					ZENGL_RUN_VMEM_OP(ZL_R_CUR_INST.dest.type,ZL_R_VMOPT_ADDMEM_MEMBLOCK,ZL_R_VMOPT_SETMEM_MEMBLOCK,tmpmem,dest,ZL_ERR_RUN_INVALID_VMEM_TYPE)
					break;
				} //switch(src.runType)
				break;
			default:
				run->exit(VM_ARG,ZL_ERR_RUN_INVALID_INST_DEST_TYPE,ZL_R_REG_PC);
				break;
			} //switch(ZL_R_CUR_INST.dest.type) //当前MOV指令的目标操作数
			break; //case ZL_R_IT_MOV //MOV指令....
		case ZL_R_IT_PRINT: //PRINT指令
			switch(ZL_R_CUR_INST.src.type) //得到PRINT的源操作数
			{
			case ZL_R_DT_REG: //print 指令源操作数类型为寄存器
				switch(ZENGL_RUN_REG(ZL_R_CUR_INST.src.val.reg).runType)
				{
				case ZL_R_RDT_INT:
					src.val.dword = ZENGL_RUN_REGVAL(ZL_R_CUR_INST.src.val.reg).dword;
					break;
				case ZL_R_RDT_FLOAT:
					src.runType = ZL_R_RDT_FLOAT;
					src.val.qword = ZENGL_RUN_REGVAL(ZL_R_CUR_INST.src.val.reg).qword;
					break;
				case ZL_R_RDT_STR:
					src.runType = ZL_R_RDT_STR;
					src.val.str = ZENGL_RUN_REGVAL(ZL_R_CUR_INST.src.val.reg).str;
					break;
				case ZL_R_RDT_MEM_BLOCK:
				case ZL_R_RDT_ADDR:
				case ZL_R_RDT_ADDR_LOC:
				case ZL_R_RDT_ADDR_MEMBLK:
					src.runType = ZENGL_RUN_REG(ZL_R_CUR_INST.src.val.reg).runType;
					break;
				}
				break;
			case ZL_R_DT_ARGMEM:
			case ZL_R_DT_LOCMEM:
			case ZL_R_DT_MEM: //print 指令源操作数类型为内存寻址
				ZENGL_RUN_VMEM_OP_GET(ZL_R_CUR_INST.src.type,tmpmem,src,ZL_ERR_RUN_INVALID_VMEM_TYPE)
				switch(tmpmem.runType)
				{
				case ZL_R_RDT_INT:
					src.val.dword = tmpmem.val.dword;
					break;
				case ZL_R_RDT_FLOAT:
					src.runType = ZL_R_RDT_FLOAT;
					src.val.qword = tmpmem.val.qword;
					break;
				case ZL_R_RDT_STR:
					src.runType = ZL_R_RDT_STR;
					src.val.str = tmpmem.val.str;
					break;
				case ZL_R_RDT_NONE: //没有被赋值初始化时按整数0对待
					src.val.dword = 0;
					break;
				case ZL_R_RDT_MEM_BLOCK:
					src.runType = ZL_R_RDT_MEM_BLOCK;
					break;
				}
				break;
			case ZL_R_DT_NUM: //print 指令源操作数类型为整数
				src.val.dword = ZL_R_CUR_INST.src.val.num;
				break;
			case ZL_R_DT_FLOAT: //print 指令源操作数类型为浮点数
				src.runType = ZL_R_RDT_FLOAT;
				src.val.qword = ZL_R_CUR_INST.src.val.floatnum;
				break;
			case ZL_R_DT_STR: //print 指令源操作数类型为字符串
				src.runType = ZL_R_RDT_STR;
				src.val.str = run->InstData_StringPool.ptr + ZL_R_CUR_INST.src.val.str_Index;
				break;
			default:
				run->exit(VM_ARG,ZL_ERR_RUN_INVALID_INST_SRC_TYPE,ZL_R_REG_PC);
				break;
			} //switch(ZL_R_CUR_INST.src.type) //得到PRINT的源操作数
			switch(src.runType)
			{
			case ZL_R_RDT_INT: //打印整数
				run->print(VM_ARG,"%d",src.val.dword);
				break;
			case ZL_R_RDT_FLOAT: //打印浮点数
				run->print(VM_ARG,"%.16g",src.val.qword);
				break;
			case ZL_R_RDT_STR: //打印字符串
				run->print(VM_ARG,"%s",(char *)src.val.str);
				break;
			case ZL_R_RDT_MEM_BLOCK:
				run->print(VM_ARG,"[array or class obj type]");
				break;
			case ZL_R_RDT_ADDR:
			case ZL_R_RDT_ADDR_LOC:
			case ZL_R_RDT_ADDR_MEMBLK:
				run->print(VM_ARG,"[addr type]");
				break;
			}
			break; //case ZL_R_IT_PRINT: //PRINT指令
		case ZL_R_IT_PLUS: //PLUS加法指令
			if(ZENGL_RUN_REG(ZL_R_RT_AX).runType == ZL_R_RDT_INT && 
			   ZENGL_RUN_REG(ZL_R_RT_BX).runType == ZL_R_RDT_INT) //加法指令AX BX都是整数时，相加
				ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword += ZENGL_RUN_REGVAL(ZL_R_RT_BX).dword;
			else if(ZENGL_RUN_REG(ZL_R_RT_AX).runType == ZL_R_RDT_STR)
			{
				switch(ZENGL_RUN_REG(ZL_R_RT_BX).runType)
				{
				case ZL_R_RDT_INT: //加法指令，AX 为字符串，BX为整数，将整数转为字符串，再添加到AX字符串的末尾
					ZENGL_SYS_SPRINTF(tmpchar,"%d",ZENGL_RUN_REGVAL(ZL_R_RT_BX).dword);
					ZENGL_RUN_REGVAL(ZL_R_RT_AX).str = (run->strcat)(VM_ARG,ZENGL_RUN_REGVAL(ZL_R_RT_AX).str,&ZENGL_RUN_REG(ZL_R_RT_AX).str_Index,tmpchar);
					break;
				case ZL_R_RDT_FLOAT: //加法指令，AX 为字符串，BX为浮点数，将浮点数转为字符串，再添加到AX字符串的末尾
					ZENGL_SYS_SPRINTF(tmpchar,"%.16g",ZENGL_RUN_REGVAL(ZL_R_RT_BX).qword);
					ZENGL_RUN_REGVAL(ZL_R_RT_AX).str = (run->strcat)(VM_ARG,ZENGL_RUN_REGVAL(ZL_R_RT_AX).str,&ZENGL_RUN_REG(ZL_R_RT_AX).str_Index,tmpchar);
					break;
				case ZL_R_RDT_STR: //加法指令，AX 为字符串，BX也为字符串，将BX字符串添加到AX字符串的末尾
					ZENGL_RUN_REGVAL(ZL_R_RT_AX).str = (run->strcat)(VM_ARG,ZENGL_RUN_REGVAL(ZL_R_RT_AX).str,
																&ZENGL_RUN_REG(ZL_R_RT_AX).str_Index,ZENGL_RUN_REGVAL(ZL_R_RT_BX).str);
					break;
				}
			}
			else if(ZENGL_RUN_REG(ZL_R_RT_BX).runType == ZL_R_RDT_STR)
			{
				switch(ZENGL_RUN_REG(ZL_R_RT_AX).runType)
				{
				case ZL_R_RDT_INT: //加法指令，BX为字符串，AX为整数，将AX转为字符串，再和BX字符串相连接。
					ZENGL_SYS_SPRINTF(tmpchar,"%d",ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword);
					run->memFreeIndex(VM_ARG,ZENGL_RUN_REGVAL(ZL_R_RT_AX).str,&ZENGL_RUN_REG(ZL_R_RT_AX).str_Index); //释放AX里原来的字符串指针
					ZENGL_RUN_REGVAL(ZL_R_RT_AX).str = run->strcat2(VM_ARG,ZENGL_RUN_REGVAL(ZL_R_RT_AX).str,
															&ZENGL_RUN_REG(ZL_R_RT_AX).str_Index,tmpchar,ZENGL_RUN_REGVAL(ZL_R_RT_BX).str);
					break;
				case ZL_R_RDT_FLOAT: //加法指令，BX为字符串，AX为浮点数，将AX转为字符串，再和BX字符串相连接。
					ZENGL_SYS_SPRINTF(tmpchar,"%.16g",ZENGL_RUN_REGVAL(ZL_R_RT_AX).qword);
					run->memFreeIndex(VM_ARG,ZENGL_RUN_REGVAL(ZL_R_RT_AX).str,&ZENGL_RUN_REG(ZL_R_RT_AX).str_Index); //释放AX里原来的字符串指针
					ZENGL_RUN_REGVAL(ZL_R_RT_AX).str = run->strcat2(VM_ARG,ZENGL_RUN_REGVAL(ZL_R_RT_AX).str,
															&ZENGL_RUN_REG(ZL_R_RT_AX).str_Index,tmpchar,ZENGL_RUN_REGVAL(ZL_R_RT_BX).str);
					break;
				}
				ZENGL_RUN_REG(ZL_R_RT_AX).runType = ZL_R_RDT_STR;
			}
			else if(ZENGL_RUN_REG(ZL_R_RT_AX).runType == ZL_R_RDT_FLOAT)
			{
				switch(ZENGL_RUN_REG(ZL_R_RT_BX).runType)
				{
				case ZL_R_RDT_FLOAT: //加法指令，AX为浮点数，BX也为浮点数，AX BX浮点数相加，结果存放在AX的qword成员里。
					ZENGL_RUN_REGVAL(ZL_R_RT_AX).qword += ZENGL_RUN_REGVAL(ZL_R_RT_BX).qword;
					break;
				case ZL_R_RDT_INT: //加法指令，AX为浮点数，BX为整数，AX BX相加，结果存放在AX的qword成员里。
					ZENGL_RUN_REGVAL(ZL_R_RT_AX).qword += ZENGL_RUN_REGVAL(ZL_R_RT_BX).dword;
					break;
				}
			}
			else if(ZENGL_RUN_REG(ZL_R_RT_BX).runType == ZL_R_RDT_FLOAT &&
					ZENGL_RUN_REG(ZL_R_RT_AX).runType == ZL_R_RDT_INT) //加法指令，AX为整数，BX为浮点数，AX BX相加，结果存放在AX的qword成员里，将AX类型设为浮点数。
			{
				ZENGL_RUN_REGVAL(ZL_R_RT_AX).qword = ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword + ZENGL_RUN_REGVAL(ZL_R_RT_BX).qword;
				ZENGL_RUN_REG(ZL_R_RT_AX).runType = ZL_R_RDT_FLOAT;
			}
			break; //case ZL_R_IT_PLUS: //PLUS加法指令
		case ZL_R_IT_MINIS: //MINIS减法指令
			run->op_minis(VM_ARG); //当为MINIS减法指令时，调用op_minis函数来处理。
			break;
		case ZL_R_IT_MOD: //取余指令。
			switch(ZENGL_RUN_REG(ZL_R_RT_AX).runType)
			{
			case ZL_R_RDT_INT:
				switch(ZENGL_RUN_REG(ZL_R_RT_BX).runType)
				{
				case ZL_R_RDT_INT:
					if(ZENGL_RUN_REGVAL(ZL_R_RT_BX).dword == 0)
						run->exit(VM_ARG,ZL_ERR_RUN_MOD_BY_ZERO,ZL_R_REG_PC);
					ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword %= ZENGL_RUN_REGVAL(ZL_R_RT_BX).dword;
					break;
				case ZL_R_RDT_FLOAT:
					if(ZENGL_RUN_REGVAL(ZL_R_RT_BX).qword == 0)
						run->exit(VM_ARG,ZL_ERR_RUN_MOD_BY_ZERO,ZL_R_REG_PC);
					ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword %= (ZL_INT)ZENGL_RUN_REGVAL(ZL_R_RT_BX).qword;
					break;
				}
				break;
			case ZL_R_RDT_FLOAT:
				switch(ZENGL_RUN_REG(ZL_R_RT_BX).runType)
				{
				case ZL_R_RDT_FLOAT:
					if(ZENGL_RUN_REGVAL(ZL_R_RT_BX).qword == 0)
						run->exit(VM_ARG,ZL_ERR_RUN_MOD_BY_ZERO,ZL_R_REG_PC);
					ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword = (ZL_INT)ZENGL_RUN_REGVAL(ZL_R_RT_AX).qword % (ZL_INT)ZENGL_RUN_REGVAL(ZL_R_RT_BX).qword;
					break;
				case ZL_R_RDT_INT:
					if(ZENGL_RUN_REGVAL(ZL_R_RT_BX).dword == 0)
						run->exit(VM_ARG,ZL_ERR_RUN_MOD_BY_ZERO,ZL_R_REG_PC);
					ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword = (ZL_INT)ZENGL_RUN_REGVAL(ZL_R_RT_AX).qword % ZENGL_RUN_REGVAL(ZL_R_RT_BX).dword;
					break;
				}
				ZENGL_RUN_REG(ZL_R_RT_AX).runType = ZL_R_RDT_INT;
				break;
			} //switch(ZENGL_RUN_REG(ZL_R_RT_AX).runType)
			break; //case ZL_R_IT_MOD: //取余指令。
		case ZL_R_IT_TIMES:
			if(ZENGL_RUN_REG(ZL_R_RT_AX).runType == ZL_R_RDT_INT && 
			   ZENGL_RUN_REG(ZL_R_RT_BX).runType == ZL_R_RDT_INT) //乘法指令，AX,BX为整数，两者相乘，结果存放在AX的dword成员里。
				ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword *= ZENGL_RUN_REGVAL(ZL_R_RT_BX).dword;
			else if(ZENGL_RUN_REG(ZL_R_RT_AX).runType == ZL_R_RDT_FLOAT)
			{
				switch(ZENGL_RUN_REG(ZL_R_RT_BX).runType)
				{
				case ZL_R_RDT_FLOAT: //乘法指令，AX浮点数,BX浮点数，两者相乘，结果存放在AX的qword成员里。
					ZENGL_RUN_REGVAL(ZL_R_RT_AX).qword *= ZENGL_RUN_REGVAL(ZL_R_RT_BX).qword;
					break;
				case ZL_R_RDT_INT: //乘法指令，AX浮点数,BX整数，两者相乘，结果存放在AX的qword成员里。
					ZENGL_RUN_REGVAL(ZL_R_RT_AX).qword *= ZENGL_RUN_REGVAL(ZL_R_RT_BX).dword;
					break;
				}
			}
			else if(ZENGL_RUN_REG(ZL_R_RT_BX).runType == ZL_R_RDT_FLOAT && 
				    ZENGL_RUN_REG(ZL_R_RT_AX).runType == ZL_R_RDT_INT) //乘法指令，AX整数,BX浮点数，两者相乘，结果存放在AX的qword成员里，AX设为浮点型。
			{
				ZENGL_RUN_REGVAL(ZL_R_RT_AX).qword = ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword * ZENGL_RUN_REGVAL(ZL_R_RT_BX).qword;
				ZENGL_RUN_REG(ZL_R_RT_AX).runType = ZL_R_RDT_FLOAT;
			}
			break;
		case ZL_R_IT_DIVIDE:
			if(ZENGL_RUN_REG(ZL_R_RT_AX).runType == ZL_R_RDT_INT && 
			   ZENGL_RUN_REG(ZL_R_RT_BX).runType == ZL_R_RDT_INT) //除法指令，AX，BX整数，转成double浮点数，再相除，结果以浮点数的形式存放在AX的qword里。这样1/2结果就是0.5，比C语言里的除法更符合人们的习惯。
			{
				if(ZENGL_RUN_REGVAL(ZL_R_RT_BX).dword == 0)
					run->exit(VM_ARG,ZL_ERR_RUN_DIVIDE_BY_ZERO,ZL_R_REG_PC);
				ZENGL_RUN_REGVAL(ZL_R_RT_AX).qword = (ZL_DOUBLE)ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword / (ZL_DOUBLE)ZENGL_RUN_REGVAL(ZL_R_RT_BX).dword; //将dword里的整数转为double类型，以浮点方式相除，结果存放在浮点成员qword中。
				ZENGL_RUN_REG(ZL_R_RT_AX).runType = ZL_R_RDT_FLOAT; //将AX类型设为浮点数类型
			}
			else if(ZENGL_RUN_REG(ZL_R_RT_AX).runType == ZL_R_RDT_FLOAT)
			{
				switch(ZENGL_RUN_REG(ZL_R_RT_BX).runType)
				{
				case ZL_R_RDT_FLOAT: //除法指令，AX浮点数，BX浮点数，相除，结果存放在AX的qword里。
					if(ZENGL_RUN_REGVAL(ZL_R_RT_BX).qword == 0)
						run->exit(VM_ARG,ZL_ERR_RUN_DIVIDE_BY_ZERO,ZL_R_REG_PC);
					ZENGL_RUN_REGVAL(ZL_R_RT_AX).qword /= ZENGL_RUN_REGVAL(ZL_R_RT_BX).qword;
					break;
				case ZL_R_RDT_INT: //除法指令，AX浮点数，BX整数，相除，结果存放在AX的qword里。
					if(ZENGL_RUN_REGVAL(ZL_R_RT_BX).dword == 0)
						run->exit(VM_ARG,ZL_ERR_RUN_DIVIDE_BY_ZERO,ZL_R_REG_PC);
					ZENGL_RUN_REGVAL(ZL_R_RT_AX).qword /= ZENGL_RUN_REGVAL(ZL_R_RT_BX).dword;
					break;
				}
			}
			else if(ZENGL_RUN_REG(ZL_R_RT_BX).runType == ZL_R_RDT_FLOAT && 
				    ZENGL_RUN_REG(ZL_R_RT_AX).runType == ZL_R_RDT_INT) //除法指令，AX整数，BX浮点数，相除，结果存放在AX的qword里，AX设为浮点型。
			{
				if(ZENGL_RUN_REGVAL(ZL_R_RT_BX).qword == 0)
					run->exit(VM_ARG,ZL_ERR_RUN_DIVIDE_BY_ZERO,ZL_R_REG_PC);
				ZENGL_RUN_REGVAL(ZL_R_RT_AX).qword = ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword / ZENGL_RUN_REGVAL(ZL_R_RT_BX).qword;
				ZENGL_RUN_REG(ZL_R_RT_AX).runType = ZL_R_RDT_FLOAT;
			}
			break; //case ZL_R_IT_DIVIDE:
		case ZL_R_IT_PUSH: //堆栈的压栈操作。将数据存入虚拟堆栈空间。
			if(ZL_R_CUR_INST.src.type == ZL_R_DT_REG) //压栈源操作数为寄存器类型
			{
				switch(ZENGL_RUN_REG(ZL_R_CUR_INST.src.val.reg).runType)
				{
				case ZL_R_RDT_NONE: //寄存器未初始化则按整数0对待
					tmpmem.val.dword = 0;
					run->VStackListOps(VM_ARG,ZL_R_VMOPT_ADDMEM_INT,-1,tmpmem,ZL_TRUE);
					break;
				case ZL_R_RDT_INT:
					tmpmem.val.dword = ZENGL_RUN_REGVAL(ZL_R_CUR_INST.src.val.reg).dword;
					run->VStackListOps(VM_ARG,ZL_R_VMOPT_ADDMEM_INT,-1,tmpmem,ZL_TRUE); //利用虚拟堆栈操作函数来操作虚拟堆栈空间的数据。
					break;
				case ZL_R_RDT_FLOAT:
					tmpmem.val.qword = ZENGL_RUN_REGVAL(ZL_R_CUR_INST.src.val.reg).qword;
					run->VStackListOps(VM_ARG,ZL_R_VMOPT_ADDMEM_DOUBLE,-1,tmpmem,ZL_TRUE);
					break;
				case ZL_R_RDT_STR:
					tmpmem.val.str = ZENGL_RUN_REGVAL(ZL_R_CUR_INST.src.val.reg).str;
					run->VStackListOps(VM_ARG,ZL_R_VMOPT_ADDMEM_STR,-1,tmpmem,ZL_TRUE);
					break;
				case ZL_R_RDT_ADDR:
				case ZL_R_RDT_ADDR_LOC:
				case ZL_R_RDT_ADDR_MEMBLK: //将引用压入栈
					tmpmem.runType = ZENGL_RUN_REG(ZL_R_CUR_INST.src.val.reg).runType;
					tmpmem.val.dword = ZENGL_RUN_REGVAL(ZL_R_CUR_INST.src.val.reg).dword;
					if(tmpmem.runType == ZL_R_RDT_ADDR_MEMBLK)
					{
						tmpmem.val.memblock = ZENGL_RUN_REGVAL(ZL_R_CUR_INST.src.val.reg).memblock;
						tmpmem.memblk_Index = ZENGL_RUN_REG(ZL_R_CUR_INST.src.val.reg).memblk_Index;
					}
					run->VStackListOps(VM_ARG,ZL_R_VMOPT_ADDMEM_ADDR,-1,tmpmem,ZL_TRUE);
					break;
				case ZL_R_RDT_MEM_BLOCK:
					tmpmem.val.memblock = ZENGL_RUN_REGVAL(ZL_R_CUR_INST.src.val.reg).memblock;
					tmpmem.memblk_Index = ZENGL_RUN_REG(ZL_R_CUR_INST.src.val.reg).memblk_Index;
					run->VStackListOps(VM_ARG,ZL_R_VMOPT_ADDMEM_MEMBLOCK,-1,tmpmem,ZL_TRUE);
					break;
				} //switch(ZENGL_RUN_REG(ZL_R_CUR_INST.src.val.reg).runType)
			}
			else if(ZL_R_CUR_INST.src.type == ZL_R_DT_NUM) //如果PUSH的操作数是数字，就将数字压入栈中。
			{
				tmpmem.val.dword = ZL_R_CUR_INST.src.val.num;
				run->VStackListOps(VM_ARG,ZL_R_VMOPT_ADDMEM_INT,-1,tmpmem,ZL_TRUE);
			}
			break; //case ZL_R_IT_PUSH: //堆栈的压栈操作。将数据存入虚拟堆栈空间。
		case ZL_R_IT_POP: //堆栈的弹出栈操作，将数据从虚拟堆栈中弹出去。
			if(ZL_R_CUR_INST.src.type == ZL_R_DT_REG)
			{
				tmpmem = run->VStackListOps(VM_ARG,ZL_R_VMOPT_GETMEM,-1,tmpmem,ZL_TRUE); //获取堆栈最后一个数据。
				switch(tmpmem.runType)
				{
				case ZL_R_RDT_INT: //堆栈数据为整数时。
					ZENGL_RUN_REGVAL(ZL_R_CUR_INST.src.val.reg).dword = tmpmem.val.dword; //将堆栈的整数设置到目标寄存器里,pop只有一个操作数，所以这里用src表示目标。
					ZENGL_RUN_REG(ZL_R_CUR_INST.src.val.reg).runType = ZL_R_RDT_INT;
					break;
				case ZL_R_RDT_FLOAT: //堆栈数据为浮点数时。
					ZENGL_RUN_REGVAL(ZL_R_CUR_INST.src.val.reg).qword = tmpmem.val.qword;
					ZENGL_RUN_REG(ZL_R_CUR_INST.src.val.reg).runType = ZL_R_RDT_FLOAT;
					break;
				case ZL_R_RDT_STR: //堆栈数据为字符串时。
					run->RegAssignStr(VM_ARG,ZL_R_CUR_INST.src.val.reg,tmpmem.val.str);
					ZENGL_RUN_REG(ZL_R_CUR_INST.src.val.reg).runType = ZL_R_RDT_STR;
					break;
				case ZL_R_RDT_ADDR:
				case ZL_R_RDT_ADDR_LOC:
				case ZL_R_RDT_ADDR_MEMBLK: //将引用弹出栈
					ZENGL_RUN_REG(ZL_R_CUR_INST.src.val.reg).runType = tmpmem.runType;
					ZENGL_RUN_REGVAL(ZL_R_CUR_INST.src.val.reg).dword = tmpmem.val.dword;
					if(tmpmem.runType == ZL_R_RDT_ADDR_MEMBLK)
					{
						ZENGL_RUN_REGVAL(ZL_R_CUR_INST.src.val.reg).memblock = tmpmem.val.memblock;
						ZENGL_RUN_REG(ZL_R_CUR_INST.src.val.reg).memblk_Index = tmpmem.memblk_Index;
					}
					break;
				case ZL_R_RDT_MEM_BLOCK: //将内存块弹出栈
					ZENGL_RUN_REG(ZL_R_CUR_INST.src.val.reg).runType = tmpmem.runType;
					ZENGL_RUN_REGVAL(ZL_R_CUR_INST.src.val.reg).memblock = tmpmem.val.memblock;
					ZENGL_RUN_REG(ZL_R_CUR_INST.src.val.reg).memblk_Index = tmpmem.memblk_Index;
					break;
				} //switch(tmpmem.runType)
			}
			break; //case ZL_R_IT_POP: //堆栈的弹出栈操作，将数据从虚拟堆栈中弹出去。
		case ZL_R_IT_JE:
			if(ZL_R_CUR_INST.src.type == ZL_R_DT_NUM)
			{
				src.val.dword = ZL_R_CUR_INST.src.val.num;
				run->op_je(VM_ARG,&src);
			}
			break;
		case ZL_R_IT_JNE:
			if(ZL_R_CUR_INST.src.type == ZL_R_DT_NUM)
			{
				src.val.dword = ZL_R_CUR_INST.src.val.num;
				run->op_jne(VM_ARG,&src);
			}
			break;
		case ZL_R_IT_JMP: //JMP无条件跳转指令，直接修改PC寄存器值，使脚本发生跳转。
			if(ZL_R_CUR_INST.src.type == ZL_R_DT_NUM)
			{
				ZL_R_REG_PC = ZL_R_CUR_INST.src.val.num;
				ZL_R_REG_PC--;
			}
			break;
		case ZL_R_IT_GREAT:
		case ZL_R_IT_LESS:
		case ZL_R_IT_EQUAL:
		case ZL_R_IT_NOT_EQ:
		case ZL_R_IT_GREAT_EQ:
		case ZL_R_IT_LESS_EQ:
			run->op_relation(VM_ARG); //当为大于等于之类的比较运算符指令时，调用op_relation函数来处理。
			break;
		case ZL_R_IT_AND:
		case ZL_R_IT_OR:
		case ZL_R_IT_REVERSE:
			run->op_logic(VM_ARG); //当为且或非的指令时，调用op_logic函数来处理。
			break;
		case ZL_R_IT_ADDGET:
		case ZL_R_IT_MINIS_GET:
			switch(ZL_R_CUR_INST.src.type)
			{
			case ZL_R_DT_MEM:
			case ZL_R_DT_ARGMEM:
			case ZL_R_DT_LOCMEM:
				run->op_addminisget(VM_ARG,&tmpmem,ZL_R_CUR_INST.type,ZL_R_CUR_INST.src.type);
				break;
			}
			break;
		case ZL_R_IT_GETADD:
		case ZL_R_IT_GET_MINIS:
			switch(ZL_R_CUR_INST.src.type)
			{
			case ZL_R_DT_MEM:
			case ZL_R_DT_ARGMEM:
			case ZL_R_DT_LOCMEM:
				run->op_getaddminis(VM_ARG,&tmpmem,ZL_R_CUR_INST.type,ZL_R_CUR_INST.src.type);
				break;
			}
			break;
		case ZL_R_IT_ADDONE:
		case ZL_R_IT_MINIS_ONE:
			run->op_addminisone(VM_ARG,ZL_R_CUR_INST.type);
			break;
		case ZL_R_IT_FUNARG: //FUNARG指令，为函数的参数分配足够的栈空间。有时候函数定义了3个参数，但在调用时只给出了1个参数那么通过FUNARG指令，系统会再在栈中分配两个内存，从而使得函数能有三个参数，当然第二和第三个参数的值因为没提供，所以默认是NONE未初始化状态。
			if(ZL_R_CUR_INST.src.type == ZL_R_DT_NUM)
			{
				ZL_INT argnum,i,count,retadr;
				argnum = ZENGL_RUN_REGVAL(ZL_R_RT_LOC).dword - ZENGL_RUN_REGVAL(ZL_R_RT_ARG).dword - 1;
				count = ZL_R_CUR_INST.src.val.num - argnum;
				if(count > 0)
				{
					tmpmem = run->VStackListOps(VM_ARG,ZL_R_VMOPT_GETMEM,-1,tmpmem,ZL_TRUE);
					if(tmpmem.runType == ZL_R_RDT_INT)
						retadr = tmpmem.val.dword;
					else
						run->exit(VM_ARG,ZL_ERR_RUN_MAIN_INVALID_FUN_RET_ADDRESS,ZL_R_REG_PC);
					for(i=0;i<count;i++)
					{
						run->VStackListOps(VM_ARG,ZL_R_VMOPT_ADDMEM_NONE,-1,tmpmem,ZL_TRUE);
					}
					tmpmem.val.dword = retadr;
					run->VStackListOps(VM_ARG,ZL_R_VMOPT_ADDMEM_INT,-1,tmpmem,ZL_TRUE);
					ZENGL_RUN_REGVAL(ZL_R_RT_LOC).dword = run->vstack_list.count;
				}
			}
			break; //case ZL_R_IT_FUNARG: //FUNARG指令
		case ZL_R_IT_ARG_SET: //ARG_SET指令，如果某参数没有被赋值，则设置一个默认值，例如test(a=3)如果a没有赋值，则默认值为3
			if(ZL_R_CUR_INST.dest.type == ZL_R_DT_ARGMEM)
			{
				ZL_INT tmpStackIndex;
				tmpStackIndex = ZENGL_RUN_REGVAL(ZL_R_RT_ARG).dword + ZL_R_CUR_INST.dest.val.mem;
				tmpmem = run->VStackListOps(VM_ARG,ZL_R_VMOPT_GETMEM,tmpStackIndex,tmpmem,ZL_TRUE);
				if(tmpmem.runType == ZL_R_RDT_NONE)
				{
					switch(ZL_R_CUR_INST.src.type)
					{
					case ZL_R_DT_NUM:
						tmpmem.val.dword = ZL_R_CUR_INST.src.val.num;
						run->VStackListOps(VM_ARG,ZL_R_VMOPT_SETMEM_INT,tmpStackIndex,tmpmem,ZL_TRUE);
						break;
					case ZL_R_DT_FLOAT:
						tmpmem.val.qword = ZL_R_CUR_INST.src.val.floatnum;
						run->VStackListOps(VM_ARG,ZL_R_VMOPT_SETMEM_DOUBLE,tmpStackIndex,tmpmem,ZL_TRUE);
						break;
					case ZL_R_DT_STR:
						tmpmem.val.str = (ZL_VOID *)run->InstDataStringPoolGetPtr(VM_ARG,ZL_R_CUR_INST.src.val.str_Index);
						run->VStackListOps(VM_ARG,ZL_R_VMOPT_SETMEM_STR,tmpStackIndex,tmpmem,ZL_TRUE);
						break;
					}
				}
			}
			break; //case ZL_R_IT_ARG_SET: //ARG_SET指令
		case ZL_R_IT_PUSH_LOC: //PUSH_LOC指令，为脚本函数局部变量分配栈空间
			run->VStackListOps(VM_ARG,ZL_R_VMOPT_ADDMEM_NONE,-1,tmpmem,ZL_TRUE);
			break;
		case ZL_R_IT_RET: //RET指令，从脚本函数返回
run_ret:
			run->memblock_freeall_local(VM_ARG);
			run->vstack_list.count = ZENGL_RUN_REGVAL(ZL_R_RT_LOC).dword; //将栈顶设为LOC寄存器的值，这样栈顶就指向脚本函数的返回地址
			tmpmem = run->VStackListOps(VM_ARG,ZL_R_VMOPT_GETMEM,-1,tmpmem,ZL_TRUE);
			if(tmpmem.runType == ZL_R_RDT_INT)
			{
				ZL_R_REG_PC = tmpmem.val.dword;
				ZL_R_REG_PC--;
			}
			else
				run->exit(VM_ARG,ZL_ERR_RUN_MAIN_FUN_VSTACK_STRUCT_EXCEPTION,ZL_R_REG_PC);
			run->vstack_list.count = ZENGL_RUN_REGVAL(ZL_R_RT_ARG).dword;
			tmpmem = run->VStackListOps(VM_ARG,ZL_R_VMOPT_GETMEM,-1,tmpmem,ZL_TRUE); //再弹出ARGTMP
			if(tmpmem.runType == ZL_R_RDT_INT)
				ZENGL_RUN_REGVAL(ZL_R_RT_ARGTMP).dword = tmpmem.val.dword;
			else
				run->exit(VM_ARG,ZL_ERR_RUN_MAIN_FUN_VSTACK_STRUCT_EXCEPTION,ZL_R_REG_PC);
			tmpmem = run->VStackListOps(VM_ARG,ZL_R_VMOPT_GETMEM,-1,tmpmem,ZL_TRUE); //接着是LOC寄存器
			if(tmpmem.runType == ZL_R_RDT_INT)
				ZENGL_RUN_REGVAL(ZL_R_RT_LOC).dword = tmpmem.val.dword;
			else
				run->exit(VM_ARG,ZL_ERR_RUN_MAIN_FUN_VSTACK_STRUCT_EXCEPTION,ZL_R_REG_PC);
			tmpmem = run->VStackListOps(VM_ARG,ZL_R_VMOPT_GETMEM,-1,tmpmem,ZL_TRUE); //最后是ARG寄存器，弹出顺序刚好和压栈顺序相反。
			if(tmpmem.runType == ZL_R_RDT_INT)
				ZENGL_RUN_REGVAL(ZL_R_RT_ARG).dword = tmpmem.val.dword;
			else
				run->exit(VM_ARG,ZL_ERR_RUN_MAIN_FUN_VSTACK_STRUCT_EXCEPTION,ZL_R_REG_PC);
			break; //case ZL_R_IT_RET: //RET指令，从脚本函数返回
		case ZL_R_IT_RESET: //RESET指令 将当前虚拟栈顶的计数器值赋值给目标操作寄存器。
			if(ZL_R_CUR_INST.src.type == ZL_R_DT_REG)
			{
				ZENGL_RUN_REG(ZL_R_CUR_INST.src.val.reg).runType = ZL_R_RDT_INT;
				ZENGL_RUN_REGVAL(ZL_R_CUR_INST.src.val.reg).dword = run->vstack_list.count;
			}
			break;
		case ZL_R_IT_USE: //USE指令 调用用户自定义的模块初始化函数
			if(ZL_R_CUR_INST.src.type == ZL_R_DT_STR)
			{
				ZL_CHAR * moduleName = run->InstDataStringPoolGetPtr(VM_ARG,ZL_R_CUR_INST.src.val.str_Index);
				ZL_INT moduleIndex = run->LookupModuleTable(VM_ARG,moduleName);
				if(run->moduleTable.modules[moduleIndex].init_func != ZL_NULL)
				{
					origState = VM->ApiState;
					VM->ApiState = ZL_API_ST_MOD_INIT_HANDLE;
					run->moduleTable.modules[moduleIndex].init_func(VM_ARG,run->moduleTable.modules[moduleIndex].ID);
					VM->ApiState = origState;
				}
			}
			break; //case ZL_R_IT_USE: //USE指令 调用用户自定义的模块初始化函数
		case ZL_R_IT_CALL: //CALL指令 调用用户自定义的模块函数
			switch(ZL_R_CUR_INST.src.type)
			{
			case ZL_R_DT_STR:
				{
					ZL_CHAR * functionName = run->InstDataStringPoolGetPtr(VM_ARG,ZL_R_CUR_INST.src.val.str_Index);
					ZL_INT ModFunIndex = run->LookupModFunTable(VM_ARG,functionName);
					ZL_R_CUR_INST.src.type = ZL_R_DT_NUM;
					ZL_R_CUR_INST.src.val.num = ModFunIndex;
					if(run->ModFunTable.mod_funs[ModFunIndex].handle != ZL_NULL)
					{
						ZL_INT argcount = ZENGL_RUN_REGVAL(ZL_R_RT_LOC).dword - ZENGL_RUN_REGVAL(ZL_R_RT_ARG).dword - 1;
						run->CurRunModFunIndex = ModFunIndex; //设置当前运行的模块函数在模块函数动态数组中的索引值
						origState = VM->ApiState;
						VM->ApiState = ZL_API_ST_MOD_FUN_HANDLE; //设置API调用状态处于模块函数中，如果不设置则大部分API接口都会直接返回-1，这样程序执行就会乱了套
						run->ModFunTable.mod_funs[ModFunIndex].handle(VM_ARG,argcount);
						VM->ApiState = origState;
					}
					else
						run->exit(VM_ARG,ZL_ERR_RUN_FUNCTION_IS_INVALID,functionName,ZL_R_REG_PC,functionName);
				}
				goto run_ret;
				break;
			case ZL_R_DT_NUM:
				{
					ZL_INT ModFunIndex = ZL_R_CUR_INST.src.val.num;
					if(run->ModFunTable.mod_funs[ModFunIndex].handle != ZL_NULL)
					{
						ZL_INT argcount = ZENGL_RUN_REGVAL(ZL_R_RT_LOC).dword - ZENGL_RUN_REGVAL(ZL_R_RT_ARG).dword - 1;
						run->CurRunModFunIndex = ModFunIndex; //设置当前运行的模块函数在模块函数动态数组中的索引值
						origState = VM->ApiState;
						VM->ApiState = ZL_API_ST_MOD_FUN_HANDLE; //设置API调用状态处于模块函数中，如果不设置则大部分API接口都会直接返回-1，这样程序执行就会乱了套
						run->ModFunTable.mod_funs[ModFunIndex].handle(VM_ARG,argcount);
						VM->ApiState = origState;
					}
				}
				goto run_ret;
				break;
			}
			break; //case ZL_R_IT_CALL: //CALL指令 调用用户自定义的模块函数
		case ZL_R_IT_ADDR: //如果是ADDR引用汇编指令就调用op_addr函数来处理。
			run->op_addr(VM_ARG,&tmpmem);
			break; //case ZL_R_IT_ADDR: //如果是ADDR引用汇编指令就调用op_addr函数来处理。
		case ZL_R_IT_SET_ARRAY: //设置数组元素
			run->op_set_array(VM_ARG,&tmpmem);
			break;
		case ZL_R_IT_GET_ARRAY: //获取数组中的某元素
			run->op_get_array(VM_ARG,&tmpmem);
			break;
		case ZL_R_IT_GET_ARRAY_ADDR: //获取数组元素引用
			run->op_get_array_addr(VM_ARG,&tmpmem);
			break;
		case ZL_R_IT_ADDGET_ARRAY: //例如++test[0]
		case ZL_R_IT_MINISGET_ARRAY: //例如 --test[0]
		case ZL_R_IT_GETADD_ARRAY: //例如 test[0]++
		case ZL_R_IT_GETMINIS_ARRAY: //例如test[0]--
			run->op_addminis_one_array(VM_ARG,&tmpmem,ZL_R_CUR_INST.type);
			break;
		case ZL_R_IT_SWITCH: //SWITCH指令 则调用op_switch进行处理
			run->op_switch(VM_ARG);
			break;
		case ZL_R_IT_BIT_AND:
		case ZL_R_IT_BIT_OR:
		case ZL_R_IT_BIT_XOR:
		case ZL_R_IT_BIT_RIGHT:
		case ZL_R_IT_BIT_LEFT:
		case ZL_R_IT_BIT_REVERSE:
			run->op_bits(VM_ARG); //按位与，或，异或等位运算指令的处理程式
			break;
		default:
			run->exit(VM_ARG,ZL_ERR_RUN_INVALID_INST_TYPE,ZL_R_REG_PC);
			break;
		} //switch(ZL_R_CUR_INST.type) //根据当前的指令类型执行不同的操作
		ZL_R_REG_PC++; //增加PC寄存器的值。
	} //while(ZL_R_CUR_INST.type != ZL_R_IT_END)
}

/*
	将某字符串赋值给reg寄存器,通过memReUsePtr重分配寄存器里的str字符串指针对应的内存大小，以便有足够空间存放字符串，再通过strncpy将字符串信息拷贝到寄存器的str字段
*/
ZL_VOID zenglrun_RegAssignStr(ZL_VOID * VM_ARG,ZENGL_RUN_REG_TYPE reg,ZL_VOID * str)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZL_VOID ** tmptr;
	ZL_INT len;
	tmptr = &ZENGL_RUN_REGVAL(reg).str;
	if(str == ZL_NULL) //如果str为ZL_NULL，则将目标寄存器分配为最短字符串即2个字节的内存，第一个字节设为STRNULL即'\0'字符串结尾符。
	{
		(*tmptr) = run->memReUsePtr(VM_ARG,(*tmptr),2 * sizeof(ZL_CHAR),&ZENGL_RUN_REG(reg).str_Index); //使用memReUsePtr提高内存的重利用率。
		(*((ZL_CHAR **)tmptr))[0] = ZL_STRNULL;
		return;
	}
	len = ZENGL_SYS_STRLEN(str);
	(*tmptr) = run->memReUsePtr(VM_ARG,(*tmptr),(len+1) * sizeof(ZL_CHAR),&ZENGL_RUN_REG(reg).str_Index);
	ZENGL_SYS_STRNCPY(*tmptr,str,len);
	(*((ZL_CHAR **)tmptr))[len] = ZL_STRNULL;
}

/*
	MINIS减法指令的相关处理程式。op是operate操作的缩写
*/
ZENGL_RUN_RUNTIME_OP_DATA_TYPE zenglrun_op_minis(ZL_VOID * VM_ARG)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	if(ZENGL_RUN_REG(ZL_R_RT_AX).runType == ZL_R_RDT_INT &&
	   ZENGL_RUN_REG(ZL_R_RT_BX).runType == ZL_R_RDT_INT) //AX,BX都是整数时，AX和BX的dword成员相减，结果存放在AX中。
		ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword -= ZENGL_RUN_REGVAL(ZL_R_RT_BX).dword;
	else if(ZENGL_RUN_REG(ZL_R_RT_AX).runType == ZL_R_RDT_STR) //当AX是字符串时
	{
		switch(ZENGL_RUN_REG(ZL_R_RT_BX).runType)
		{
		case ZL_R_RDT_INT: //AX字符串，BX为整数，将AX通过atoi的C库函数转为整数，再和BX相减，结果存放于AX中，同时将AX类型设为ZL_R_RDT_INT整形数类型。
			ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword = ZENGL_SYS_STR_TO_NUM(ZENGL_RUN_REGVAL(ZL_R_RT_AX).str);
			ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword -= ZENGL_RUN_REGVAL(ZL_R_RT_BX).dword;
			ZENGL_RUN_REG(ZL_R_RT_AX).runType = ZL_R_RDT_INT;
			break;
		case ZL_R_RDT_FLOAT:
			ZENGL_RUN_REGVAL(ZL_R_RT_AX).qword = ZENGL_SYS_STR_TO_FLOAT(ZENGL_RUN_REGVAL(ZL_R_RT_AX).str); //将AX转为浮点数，AX,BX的qword成员相减。
			ZENGL_RUN_REGVAL(ZL_R_RT_AX).qword -= ZENGL_RUN_REGVAL(ZL_R_RT_BX).qword;
			ZENGL_RUN_REG(ZL_R_RT_AX).runType = ZL_R_RDT_FLOAT;
			break;
		case ZL_R_RDT_STR: //AX,BX都是字符串时，通过strcmp对两个字符串进行比较，并将比较的结果存放于AX的dword中。
			ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword = ZENGL_SYS_STRCMP(ZENGL_RUN_REGVAL(ZL_R_RT_AX).str,ZENGL_RUN_REGVAL(ZL_R_RT_BX).str);
			ZENGL_RUN_REG(ZL_R_RT_AX).runType = ZL_R_RDT_INT;
			break;
		}
	}
	else if(ZENGL_RUN_REG(ZL_R_RT_BX).runType == ZL_R_RDT_STR) //BX为字符串时的情况
	{
		switch(ZENGL_RUN_REG(ZL_R_RT_AX).runType)
		{
		case ZL_R_RDT_INT:
			ZENGL_RUN_REGVAL(ZL_R_RT_BX).dword = ZENGL_SYS_STR_TO_NUM(ZENGL_RUN_REGVAL(ZL_R_RT_BX).str);
			ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword -= ZENGL_RUN_REGVAL(ZL_R_RT_BX).dword;
			ZENGL_RUN_REG(ZL_R_RT_AX).runType = ZL_R_RDT_INT;
			break;
		case ZL_R_RDT_FLOAT:
			ZENGL_RUN_REGVAL(ZL_R_RT_BX).qword = ZENGL_SYS_STR_TO_FLOAT(ZENGL_RUN_REGVAL(ZL_R_RT_BX).str);
			ZENGL_RUN_REGVAL(ZL_R_RT_AX).qword -= ZENGL_RUN_REGVAL(ZL_R_RT_BX).qword;
			ZENGL_RUN_REG(ZL_R_RT_AX).runType = ZL_R_RDT_FLOAT;
			break;
		}
	}
	else if(ZENGL_RUN_REG(ZL_R_RT_AX).runType == ZL_R_RDT_FLOAT)
	{
		switch(ZENGL_RUN_REG(ZL_R_RT_BX).runType)
		{
		case ZL_R_RDT_FLOAT:
			ZENGL_RUN_REGVAL(ZL_R_RT_AX).qword -= ZENGL_RUN_REGVAL(ZL_R_RT_BX).qword;
			break;
		case ZL_R_RDT_INT:
			ZENGL_RUN_REGVAL(ZL_R_RT_AX).qword -= ZENGL_RUN_REGVAL(ZL_R_RT_BX).dword;
			break;
		}
	}
	else if(ZENGL_RUN_REG(ZL_R_RT_BX).runType == ZL_R_RDT_FLOAT &&
			ZENGL_RUN_REG(ZL_R_RT_AX).runType == ZL_R_RDT_INT)
	{
		ZENGL_RUN_REGVAL(ZL_R_RT_AX).qword = ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword - ZENGL_RUN_REGVAL(ZL_R_RT_BX).qword;
		ZENGL_RUN_REG(ZL_R_RT_AX).runType = ZL_R_RDT_FLOAT;
	}
	return ZENGL_RUN_REG(ZL_R_RT_AX).runType; //返回AX(计算结果)的类型
}

/*
	JE指令执行的操作，AX寄存器里存放着之前表达式的结果，当AX为0或空时，将修改PC寄存器的值，使脚本发生跳转
*/
ZL_VOID zenglrun_op_je(ZL_VOID * VM_ARG,ZENGL_RUN_RUNTIME_OP_DATA * src)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	if((ZENGL_RUN_REG(ZL_R_RT_AX).runType == ZL_R_RDT_INT && ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword == 0) || 
		(ZENGL_RUN_REG(ZL_R_RT_AX).runType == ZL_R_RDT_FLOAT && ZENGL_RUN_REGVAL(ZL_R_RT_AX).qword == 0) || 
		(ZENGL_RUN_REG(ZL_R_RT_AX).runType == ZL_R_RDT_STR && !ZENGL_SYS_STRCMP(ZENGL_RUN_REGVAL(ZL_R_RT_AX).str,"")))
	{
		if(src->runType == ZL_R_RDT_INT)
		{
			ZL_R_REG_PC = src->val.dword;
			ZL_R_REG_PC--;
		}
	}
}

/*
	JNE指令，和JE指令刚好相反。AX寄存器里存放着之前表达式的结果，当AX不为0，即为TRUE时，将修改PC寄存器的值，使脚本发生跳转
*/
ZL_VOID zenglrun_op_jne(ZL_VOID * VM_ARG,ZENGL_RUN_RUNTIME_OP_DATA * src)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	if((ZENGL_RUN_REG(ZL_R_RT_AX).runType == ZL_R_RDT_INT && ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword != 0) || 
		(ZENGL_RUN_REG(ZL_R_RT_AX).runType == ZL_R_RDT_FLOAT && ZENGL_RUN_REGVAL(ZL_R_RT_AX).qword != 0) || 
		(ZENGL_RUN_REG(ZL_R_RT_AX).runType == ZL_R_RDT_STR && ZENGL_SYS_STRCMP(ZENGL_RUN_REGVAL(ZL_R_RT_AX).str,"")))
	{
		if(src->runType == ZL_R_RDT_INT)
		{
			ZL_R_REG_PC = src->val.dword;
			ZL_R_REG_PC--;
		}
	}
}

/*按位与，或，异或等位运算指令的处理程式*/
ZL_VOID zenglrun_op_bits(ZL_VOID * VM_ARG)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	switch(ZENGL_RUN_REG(ZL_R_RT_AX).runType)
	{
	case ZL_R_RDT_FLOAT:
		ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword = (ZL_INT)ZENGL_RUN_REGVAL(ZL_R_RT_AX).qword;
		ZENGL_RUN_REG(ZL_R_RT_AX).runType = ZL_R_RDT_INT;
		break;
	case ZL_R_RDT_STR:
		ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword = ZENGL_SYS_STR_TO_NUM(ZENGL_RUN_REGVAL(ZL_R_RT_AX).str);
		ZENGL_RUN_REG(ZL_R_RT_AX).runType = ZL_R_RDT_INT;
		break;
	case ZL_R_RDT_INT:
		break;
	default: //其他类型使用0
		ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword = 0;
		ZENGL_RUN_REG(ZL_R_RT_AX).runType = ZL_R_RDT_INT;
		break;
	}
	if(ZL_R_CUR_INST.type != ZL_R_IT_BIT_REVERSE)
	{
		switch(ZENGL_RUN_REG(ZL_R_RT_BX).runType)
		{
		case ZL_R_RDT_FLOAT:
			ZENGL_RUN_REGVAL(ZL_R_RT_BX).dword = (ZL_INT)ZENGL_RUN_REGVAL(ZL_R_RT_BX).qword;
			ZENGL_RUN_REG(ZL_R_RT_BX).runType = ZL_R_RDT_INT;
			break;
		case ZL_R_RDT_STR:
			ZENGL_RUN_REGVAL(ZL_R_RT_BX).dword = ZENGL_SYS_STR_TO_NUM(ZENGL_RUN_REGVAL(ZL_R_RT_BX).str);
			ZENGL_RUN_REG(ZL_R_RT_BX).runType = ZL_R_RDT_INT;
			break;
		case ZL_R_RDT_INT:
			break;
		default:
			ZENGL_RUN_REGVAL(ZL_R_RT_BX).dword = 0;
			ZENGL_RUN_REG(ZL_R_RT_BX).runType = ZL_R_RDT_INT;
			break;
		}
	}
	switch(ZL_R_CUR_INST.type)
	{
	case ZL_R_IT_BIT_AND:
		ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword = ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword & ZENGL_RUN_REGVAL(ZL_R_RT_BX).dword;
		break;
	case ZL_R_IT_BIT_OR:
		ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword = ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword | ZENGL_RUN_REGVAL(ZL_R_RT_BX).dword;
		break;
	case ZL_R_IT_BIT_XOR:
		ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword = ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword ^ ZENGL_RUN_REGVAL(ZL_R_RT_BX).dword;
		break;
	case ZL_R_IT_BIT_RIGHT:
		ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword = ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword >> ZENGL_RUN_REGVAL(ZL_R_RT_BX).dword;
		break;
	case ZL_R_IT_BIT_LEFT:
		ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword = ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword << ZENGL_RUN_REGVAL(ZL_R_RT_BX).dword;
		break;
	case ZL_R_IT_BIT_REVERSE:
		ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword = ~ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword;
		break;
	}
}

/*
	大于小于等于之类的比较运算符指令的处理程式。
*/
ZL_VOID zenglrun_op_relation(ZL_VOID * VM_ARG)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_RUN_RUNTIME_OP_DATA_TYPE retval;
	ZL_DOUBLE tmpret;
	retval = run->op_minis(VM_ARG);
	if(retval == ZL_R_RDT_FLOAT) //先将AX,BX进行减法处理，再根据处理的结果和对应的比较运算符，产生相应的结果。
	{
		tmpret = ZENGL_RUN_REGVAL(ZL_R_RT_AX).qword;
		if(tmpret > 0)
			ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword = 1;
		else if(tmpret < 0)
			ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword = -1;
		else
			ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword = 0;
		//ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword = (ZL_INT)ZENGL_RUN_REGVAL(ZL_R_RT_AX).qword; //不可以直接将浮点成员qword强制转为整形，因为这样的话0.5这样的小数就会变为0，就会影响下面的比较结果。
		ZENGL_RUN_REG(ZL_R_RT_AX).runType = ZL_R_RDT_INT;
	}
	switch(ZL_R_CUR_INST.type)
	{
	case ZL_R_IT_GREAT: //如果是大于指令，当AX,BX相减，结果大于0时，说明AX大于BX，则将1作为结果存放于AX中。类似C语言，这里用1代表PHP里的true布尔值。
		ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword = ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword > 0 ? 1 : 0;
		break;
	case ZL_R_IT_LESS:
		ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword = ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword < 0 ? 1 : 0;
		break;
	case ZL_R_IT_EQUAL:
		ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword = ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword == 0 ? 1 : 0;
		break;
	case ZL_R_IT_NOT_EQ:
		ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword = ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword != 0 ? 1 : 0;
		break;
	case ZL_R_IT_GREAT_EQ:
		ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword = ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword >= 0 ? 1 : 0;
		break;
	case ZL_R_IT_LESS_EQ:
		ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword = ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword <= 0 ? 1 : 0;
		break;
	}
}

/*
	AND(且)，OR（或），REVERSE（取反）逻辑运算符的处理程式。先得到AX的布尔值，如果为0或为空字符串则AX为FALSE布尔值（这里用0表示PHP里的FALSE），反之则为1 ，
	同理得到BX的布尔值，最后将AX,BX进行&&，||,!的运算。
*/
ZL_VOID zenglrun_op_logic(ZL_VOID * VM_ARG)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	switch(ZENGL_RUN_REG(ZL_R_RT_AX).runType)
	{
	case ZL_R_RDT_FLOAT:
		ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword = ZENGL_RUN_REGVAL(ZL_R_RT_AX).qword == 0 ? 0 : 1;
		ZENGL_RUN_REG(ZL_R_RT_AX).runType = ZL_R_RDT_INT;
		break;
	case ZL_R_RDT_STR:
		ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword = ZENGL_SYS_STRCMP(ZENGL_RUN_REGVAL(ZL_R_RT_AX).str,"") == 0 ? 0 : 1;
		ZENGL_RUN_REG(ZL_R_RT_AX).runType = ZL_R_RDT_INT;
		break;
	default:
		ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword = ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword != 0 ? 1 : 0;
		ZENGL_RUN_REG(ZL_R_RT_AX).runType = ZL_R_RDT_INT;
		break;
	}
	if(ZL_R_CUR_INST.type != ZL_R_IT_REVERSE)
	{
		switch(ZENGL_RUN_REG(ZL_R_RT_BX).runType)
		{
		case ZL_R_RDT_FLOAT:
			ZENGL_RUN_REGVAL(ZL_R_RT_BX).dword = ZENGL_RUN_REGVAL(ZL_R_RT_BX).qword == 0 ? 0 : 1;
			ZENGL_RUN_REG(ZL_R_RT_BX).runType = ZL_R_RDT_INT;
			break;
		case ZL_R_RDT_STR:
			ZENGL_RUN_REGVAL(ZL_R_RT_BX).dword = ZENGL_SYS_STRCMP(ZENGL_RUN_REGVAL(ZL_R_RT_BX).str,"") == 0 ? 0 : 1;
			ZENGL_RUN_REG(ZL_R_RT_BX).runType = ZL_R_RDT_INT;
			break;
		default:
			ZENGL_RUN_REGVAL(ZL_R_RT_BX).dword = ZENGL_RUN_REGVAL(ZL_R_RT_BX).dword != 0 ? 1 : 0;
			ZENGL_RUN_REG(ZL_R_RT_BX).runType = ZL_R_RDT_INT;
			break;
		}
	}
	switch(ZL_R_CUR_INST.type)
	{
	case ZL_R_IT_AND:
		ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword = ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword && ZENGL_RUN_REGVAL(ZL_R_RT_BX).dword;
		break;
	case ZL_R_IT_OR:
		ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword = ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword || ZENGL_RUN_REGVAL(ZL_R_RT_BX).dword;
		break;
	case ZL_R_IT_REVERSE:
		ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword = !ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword;
		break;
	}
}

/*当加加或减减运算符在变量标示符的前面时，先将变量值加一或减一，再返回结果，memtype是内存类型，主要是全局变量的虚拟内存和参数及局部变量所在的堆栈。*/
ZL_VOID zenglrun_op_addminisget(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem,ZENGL_RUN_INST_TYPE type,ZENGL_RUN_INST_OP_DATA_TYPE memtype)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_RUN_VMEM_OP_GET(memtype,(*tmpmem),src,ZL_ERR_RUN_INVALID_VMEM_TYPE)
	switch(tmpmem->runType)
	{
	case ZL_R_RDT_NONE: //如果没被初始化，按整数0对待
		tmpmem->runType = ZL_R_RDT_INT;
		tmpmem->val.dword = 0;
	case ZL_R_RDT_INT:
		switch(type)
		{
		case ZL_R_IT_ADDGET:
			++tmpmem->val.dword;
			break;
		case ZL_R_IT_MINIS_GET:
			--tmpmem->val.dword;
			break;
		}
		ZENGL_RUN_VMEM_OP(memtype,ZL_R_VMOPT_ADDMEM_INT,ZL_R_VMOPT_SETMEM_INT,*tmpmem,src,ZL_ERR_RUN_INVALID_VMEM_TYPE)
		ZENGL_RUN_REG(ZL_R_RT_AX).runType = tmpmem->runType;
		ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword = tmpmem->val.dword;
		break;
	case ZL_R_RDT_FLOAT:
		switch(type)
		{
		case ZL_R_IT_ADDGET:
			++tmpmem->val.qword;
			break;
		case ZL_R_IT_MINIS_GET:
			--tmpmem->val.qword;
			break;
		}
		ZENGL_RUN_VMEM_OP(memtype,ZL_R_VMOPT_ADDMEM_DOUBLE,ZL_R_VMOPT_SETMEM_DOUBLE,*tmpmem,src,ZL_ERR_RUN_INVALID_VMEM_TYPE)
		ZENGL_RUN_REG(ZL_R_RT_AX).runType = tmpmem->runType;
		ZENGL_RUN_REGVAL(ZL_R_RT_AX).qword = tmpmem->val.qword;
		break;
	case ZL_R_RDT_STR:
		{
			ZL_CHAR tmpstr[40];
			ZL_INT tmpint;
			ZL_DOUBLE tmpfloat;
			ZL_BOOL	isfloat;
			if(ZENGL_SYS_STRCHR(tmpmem->val.str,'.') == ZL_NULL)
				isfloat = ZL_FALSE;
			else
				isfloat = ZL_TRUE;
			if(isfloat)
			{
				tmpfloat = ZENGL_SYS_STR_TO_FLOAT(tmpmem->val.str);
				switch(type)
				{
				case ZL_R_IT_ADDGET:
					tmpfloat++;
					break;
				case ZL_R_IT_MINIS_GET:
					tmpfloat--;
					break;
				}
				ZENGL_SYS_SPRINTF(tmpstr,"%.16g",tmpfloat);
			}
			else
			{
				tmpint = ZENGL_SYS_STR_TO_NUM(tmpmem->val.str);
				switch(type)
				{
				case ZL_R_IT_ADDGET:
					tmpint++;
					break;
				case ZL_R_IT_MINIS_GET:
					tmpint--;
					break;
				}
				ZENGL_SYS_SPRINTF(tmpstr,"%d",tmpint);
			}
			tmpmem->val.str = tmpstr;
			ZENGL_RUN_VMEM_OP(memtype,ZL_R_VMOPT_ADDMEM_STR,ZL_R_VMOPT_SETMEM_STR,*tmpmem,src,ZL_ERR_RUN_INVALID_VMEM_TYPE)
			ZENGL_RUN_REG(ZL_R_RT_AX).runType = tmpmem->runType;
			run->RegAssignStr(VM_ARG,ZL_R_RT_AX,tmpstr);
		}
		break; //case ZL_R_RDT_STR:
	} //switch(tmpmem->runType)
}

/*加加减减运算符在变量标示符后面时，先取值后对其进行加一或减一操作*/
ZL_VOID zenglrun_op_getaddminis(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem,ZENGL_RUN_INST_TYPE type,ZENGL_RUN_INST_OP_DATA_TYPE memtype)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_RUN_VMEM_OP_GET(memtype,(*tmpmem),src,ZL_ERR_RUN_INVALID_VMEM_TYPE)
	switch(tmpmem->runType)
	{
	case ZL_R_RDT_NONE: //如果没被初始化，按整数0对待
		tmpmem->runType = ZL_R_RDT_INT;
		tmpmem->val.dword = 0;
	case ZL_R_RDT_INT:
		ZENGL_RUN_REG(ZL_R_RT_AX).runType = tmpmem->runType;
		ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword = tmpmem->val.dword;
		switch(type)
		{
		case ZL_R_IT_GETADD:
			++tmpmem->val.dword;
			break;
		case ZL_R_IT_GET_MINIS:
			--tmpmem->val.dword;
			break;
		}
		ZENGL_RUN_VMEM_OP(memtype,ZL_R_VMOPT_ADDMEM_INT,ZL_R_VMOPT_SETMEM_INT,*tmpmem,src,ZL_ERR_RUN_INVALID_VMEM_TYPE)
		break;
	case ZL_R_RDT_FLOAT:
		ZENGL_RUN_REG(ZL_R_RT_AX).runType = tmpmem->runType;
		ZENGL_RUN_REGVAL(ZL_R_RT_AX).qword = tmpmem->val.qword;
		switch(type)
		{
		case ZL_R_IT_GETADD:
			++tmpmem->val.qword;
			break;
		case ZL_R_IT_GET_MINIS:
			--tmpmem->val.qword;
			break;
		}
		ZENGL_RUN_VMEM_OP(memtype,ZL_R_VMOPT_ADDMEM_DOUBLE,ZL_R_VMOPT_SETMEM_DOUBLE,*tmpmem,src,ZL_ERR_RUN_INVALID_VMEM_TYPE)
		break;
	case ZL_R_RDT_STR:
		{
			ZL_CHAR tmpstr[40];
			ZL_INT tmpint;
			ZL_DOUBLE tmpfloat;
			ZL_BOOL	isfloat;
			ZENGL_RUN_REG(ZL_R_RT_AX).runType = tmpmem->runType;
			run->RegAssignStr(VM_ARG,ZL_R_RT_AX,tmpmem->val.str);
			if(ZENGL_SYS_STRCHR(tmpmem->val.str,'.') == ZL_NULL)
				isfloat = ZL_FALSE;
			else
				isfloat = ZL_TRUE;
			if(isfloat)
			{
				tmpfloat = ZENGL_SYS_STR_TO_FLOAT(tmpmem->val.str);
				switch(type)
				{
				case ZL_R_IT_GETADD:
					tmpfloat++;
					break;
				case ZL_R_IT_GET_MINIS:
					tmpfloat--;
					break;
				}
				ZENGL_SYS_SPRINTF(tmpstr,"%.16g",tmpfloat);
			}
			else
			{
				tmpint = ZENGL_SYS_STR_TO_NUM(tmpmem->val.str);
				switch(type)
				{
				case ZL_R_IT_GETADD:
					tmpint++;
					break;
				case ZL_R_IT_GET_MINIS:
					tmpint--;
					break;
				}
				ZENGL_SYS_SPRINTF(tmpstr,"%d",tmpint);
			}
			tmpmem->val.str = tmpstr;
			ZENGL_RUN_VMEM_OP(memtype,ZL_R_VMOPT_ADDMEM_STR,ZL_R_VMOPT_SETMEM_STR,*tmpmem,src,ZL_ERR_RUN_INVALID_VMEM_TYPE)
		}
		break;
	} //switch(tmpmem->runType)
}

/*当加加减减和表达式连用时，因为表达式不像变量有内存空间，可以修改内存空间的值，所以这种情况就直接将表达式的结果进行加一，减一，再返回结果。*/
ZL_VOID zenglrun_op_addminisone(ZL_VOID * VM_ARG,ZENGL_RUN_INST_TYPE type)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	switch(ZENGL_RUN_REG(ZL_R_RT_AX).runType)
	{
	case ZL_R_RDT_INT:
		switch(type)
		{
		case ZL_R_IT_ADDONE:
			ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword++;
			break;
		case ZL_R_IT_MINIS_ONE:
			ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword--;
			break;
		}
		break;
	case ZL_R_RDT_FLOAT:
		switch(type)
		{
		case ZL_R_IT_ADDONE:
			ZENGL_RUN_REGVAL(ZL_R_RT_AX).qword++;
			break;
		case ZL_R_IT_MINIS_ONE:
			ZENGL_RUN_REGVAL(ZL_R_RT_AX).qword--;
			break;
		}
		break;
	case ZL_R_RDT_STR: //如果是字符串，先转为整数或浮点数，再进行加加减减运算，最后再将结果以字符串的形式返回。
		{
			ZL_CHAR tmpstr[40];
			ZL_INT tmpint;
			ZL_DOUBLE tmpfloat;
			ZL_BOOL	isfloat;
			if(ZENGL_SYS_STRCHR(ZENGL_RUN_REGVAL(ZL_R_RT_AX).str,'.') == ZL_NULL)
				isfloat = ZL_FALSE;
			else
				isfloat = ZL_TRUE;
			if(isfloat)
			{
				tmpfloat = ZENGL_SYS_STR_TO_FLOAT(ZENGL_RUN_REGVAL(ZL_R_RT_AX).str);
				switch(type)
				{
				case ZL_R_IT_ADDONE:
					tmpfloat++;
					break;
				case ZL_R_IT_MINIS_ONE:
					tmpfloat--;
					break;
				}
				ZENGL_SYS_SPRINTF(tmpstr,"%.16g",tmpfloat);
			}
			else
			{
				tmpint = ZENGL_SYS_STR_TO_NUM(ZENGL_RUN_REGVAL(ZL_R_RT_AX).str);
				switch(type)
				{
				case ZL_R_IT_ADDONE:
					tmpint++;
					break;
				case ZL_R_IT_MINIS_ONE:
					tmpint--;
					break;
				}
				ZENGL_SYS_SPRINTF(tmpstr,"%d",tmpint);
			}
			run->RegAssignStr(VM_ARG,ZL_R_RT_AX,tmpstr);
		}
		break;
	} //switch(ZENGL_RUN_REG(ZL_R_RT_AX).runType)
}

/*ADDR引用汇编指令的操作*/
ZL_VOID zenglrun_op_addr(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_RUN_RUNTIME_OP_DATA_TYPE runtype;
	ZL_INT mem;
	switch(ZL_R_CUR_INST.src.type)
	{
	case ZL_R_DT_MEM: //如果当前ADDR指令的操作数是全局内存，就将runtype设为ZL_R_RDT_ADDR
		runtype = ZL_R_RDT_ADDR;
		mem = ZL_R_CUR_INST.src.val.mem; //mem为引用的全局内存的索引
		break;
	case ZL_R_DT_ARGMEM:
		runtype = ZL_R_RDT_ADDR_LOC;
		mem = ZENGL_RUN_REGVAL(ZL_R_RT_ARG).dword + ZL_R_CUR_INST.src.val.mem; //ZENGL_RUN_REGVAL(ZL_R_RT_ARG).dword 是所有参数的起始栈位置，加上ZL_R_CUR_INST.src.val.mem参数偏移值才能得到正确的参数栈位置。
		break;
	case ZL_R_DT_LOCMEM:
		runtype = ZL_R_RDT_ADDR_LOC;
		mem = ZENGL_RUN_REGVAL(ZL_R_RT_LOC).dword + ZL_R_CUR_INST.src.val.mem; //ZENGL_RUN_REGVAL(ZL_R_RT_LOC).dword 是所有局部变量的起始栈位置。
		break; 
	} //switch(ZL_R_CUR_INST.src.type)
	ZENGL_RUN_REG(ZL_R_RT_AX).runType = runtype; //将引用的内存类型赋值给AX的runType字段 。
	ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword = mem; //将引用内存的索引赋值给AX的dword字段。
}

/*设置数组元素*/
ZL_VOID zenglrun_op_set_array(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_RUN_VIRTUAL_MEM_LIST * ptr;
	ZL_INT argcount,index,ptrIndex,i;
	ZENGL_RUN_VMEM_OP_GET(ZL_R_CUR_INST.src.type,(*tmpmem),src,ZL_ERR_RUN_INVALID_VMEM_TYPE)
	if(tmpmem->val.memblock == ZL_NULL)
	{
		ptr = run->alloc_memblock(VM_ARG,&ptrIndex);
		tmpmem->val.memblock = (ZL_VOID *)ptr;
		tmpmem->memblk_Index = ptrIndex;
		ZENGL_RUN_VMEM_OP(ZL_R_CUR_INST.src.type,ZL_R_VMOPT_ADDMEM_MEMBLOCK,ZL_R_VMOPT_SETMEM_MEMBLOCK,(*tmpmem),src,ZL_ERR_RUN_INVALID_VMEM_TYPE)
	}
	else
		ptr = (ZENGL_RUN_VIRTUAL_MEM_LIST *)tmpmem->val.memblock;
	argcount = run->vstack_list.count - ZENGL_RUN_REGVAL(ZL_R_RT_ARRAY_ITEM).dword;
	if(argcount == 0)
		index = ptr->count;
	else if(argcount == 1)
		index = run->memblock_getindex(VM_ARG,0,tmpmem);
	else
	{
		for(i=0;i < argcount-1;i++)
		{
			index = run->memblock_getindex(VM_ARG,i,tmpmem);
			if(index < 0)
				run->exit(VM_ARG,ZL_ERR_RUN_MEM_BLOCK_INVALID_INDEX);
			ptr = run->realloc_memblock(VM_ARG,ptr,index);
			if(ptr->mem_array[index].val.memblock == ZL_NULL)
			{
				ptr->mem_array[index].runType = ZL_R_RDT_MEM_BLOCK;
				run->assign_memblock(VM_ARG,&(ptr->mem_array[index].val.memblock) ,
					run->alloc_memblock(VM_ARG,&ptr->mem_array[index].memblk_Index));
			}
			ptr = ptr->mem_array[index].val.memblock;
		}
		index = run->memblock_getindex(VM_ARG,i,tmpmem);
	}
	if(index < 0)
		run->exit(VM_ARG,ZL_ERR_RUN_MEM_BLOCK_INVALID_INDEX);
	ptr = run->realloc_memblock(VM_ARG,ptr,index);
	run->memblock_setval(VM_ARG,ptr,index,tmpmem); //使用memblock_setval设置数组元素。
	run->vstack_list.count = ZENGL_RUN_REGVAL(ZL_R_RT_ARRAY_ITEM).dword; //恢复栈空间。
	(*tmpmem) = run->VStackListOps(VM_ARG,ZL_R_VMOPT_GETMEM,-1,(*tmpmem),ZL_TRUE);
	if(tmpmem->runType == ZL_R_RDT_INT)
		ZENGL_RUN_REGVAL(ZL_R_RT_ARRAY_ITEM).dword = tmpmem->val.dword;
	else
		run->exit(VM_ARG,ZL_ERR_RUN_MEM_BLOCK_VSTACK_STRUCT_EXCEPTION,ZL_R_REG_PC);
	if(run->vstack_list.mem_array[run->vstack_list.count - 1].runType == ZL_R_RDT_MEM_BLOCK)
	{
		run->memblock_free(VM_ARG,run->vstack_list.mem_array[run->vstack_list.count - 1].val.memblock,
			&run->vstack_list.mem_array[run->vstack_list.count - 1].memblk_Index);
		run->vstack_list.mem_array[run->vstack_list.count - 1].val.memblock = ZL_NULL;
	}
	run->vstack_list.count--;
}

/*为内存块分配动态内存，通过ZENGL_RUN_VIRTUAL_MEM_LIST结构来管理内存块，ZENGL_RUN_VIRTUAL_MEM_LIST结构中的mem_array字段是新分配的内存块指针。*/
ZENGL_RUN_VIRTUAL_MEM_LIST * zenglrun_alloc_memblock(ZL_VOID * VM_ARG,ZL_INT * index)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_RUN_VIRTUAL_MEM_LIST * ptr;
	ptr = (ZENGL_RUN_VIRTUAL_MEM_LIST *)run->memAlloc(VM_ARG,sizeof(ZENGL_RUN_VIRTUAL_MEM_LIST),index);
	ptr->size = ZL_R_MEM_BLOCK_SIZE;
	ptr->count = 0;
	ptr->refcount = 0;
	ptr->mem_array = (ZENGL_RUN_VIRTUAL_MEM_STRUCT *)run->memAlloc(VM_ARG,ptr->size * sizeof(ZENGL_RUN_VIRTUAL_MEM_STRUCT),&ptr->mempool_index);
	if(ptr->mem_array == ZL_NULL)
		run->exit(VM_ARG,ZL_ERR_RUN_MEM_BLOCK_MALLOC_FAILED);
	ZENGL_SYS_MEM_SET(ptr->mem_array,0,ptr->size * sizeof(ZENGL_RUN_VIRTUAL_MEM_STRUCT));
	return ptr;
}

/*
	获取数组内存块的索引，例如test[1] ，那么数组索引1会先存放在栈中，之所以放在栈中，是因为数组里的索引的值可以由表达式生成(例如test[a+b],a+b为表达式)，
	表达式的结果会先放在AX中，最后再压入栈中，
	对应的栈位置为ARRAY_ITEM寄存器的值加上偏移量此处为0(因为是一维数组)，在这个函数中参数i就是偏移量0，
	然后通过VStackListOps栈操作函数加上index(数组索引在栈中所在位置)作为参数，就得到tmpmem，即栈中的值，本例中就是整数1，这样就得到了数组的索引值。
*/
ZL_INT zenglrun_memblock_getindex(ZL_VOID * VM_ARG,ZL_INT i,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZL_INT index = ZENGL_RUN_REGVAL(ZL_R_RT_ARRAY_ITEM).dword + i;
	(*tmpmem) = run->VStackListOps(VM_ARG,ZL_R_VMOPT_GETMEM,index,(*tmpmem),ZL_TRUE);
	switch(tmpmem->runType)
	{
	case ZL_R_RDT_INT:
		return tmpmem->val.dword;
		break;
	case ZL_R_RDT_FLOAT:
		return (ZL_INT)tmpmem->val.qword;
		break;
	case ZL_R_RDT_STR:
		return ZENGL_SYS_STR_TO_NUM((ZL_CHAR *)tmpmem->val.str);
		break;
	}
	return 0;
}

/*为内存块动态调整大小。*/
ZENGL_RUN_VIRTUAL_MEM_LIST * zenglrun_realloc_memblock(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_LIST * ptr,ZL_INT index)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZL_INT oldsize = ptr->size;
	if(index < ptr->size)
		return ptr;
	while(index >= ptr->size)
	{
		ptr->size += ZL_R_MEM_BLOCK_SIZE;
	}
	ptr->mem_array = (ZENGL_RUN_VIRTUAL_MEM_STRUCT *)run->memReAlloc(VM_ARG,ptr->mem_array,ptr->size * sizeof(ZENGL_RUN_VIRTUAL_MEM_STRUCT),
		&ptr->mempool_index);
	ZENGL_SYS_MEM_SET(ptr->mem_array + oldsize,0,(ptr->size - oldsize) * sizeof(ZENGL_RUN_VIRTUAL_MEM_STRUCT));
	return ptr;
}

/*
	将dest_arg设为src_arg所指向的内存块，同时增加refcount内存块的引用计数。
	在释放内存块时会将refcount进行减一，直到refcount为0时才进行内存块的具体释放操作。
*/
ZL_VOID zenglrun_assign_memblock(ZL_VOID * VM_ARG,ZL_VOID ** dest_arg,ZL_VOID * src_arg)
{
	ZENGL_RUN_VIRTUAL_MEM_LIST ** dest;
	ZENGL_RUN_VIRTUAL_MEM_LIST * src;
	if(src_arg == ZL_NULL || dest_arg == ZL_NULL)
		return;
	dest = (ZENGL_RUN_VIRTUAL_MEM_LIST **)dest_arg;
	src = (ZENGL_RUN_VIRTUAL_MEM_LIST *)src_arg;
	src->refcount++;
	(*dest) = src;
}

/*设置数组对应内存块ptr的index索引位置的值*/
ZL_VOID zenglrun_memblock_setval(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_LIST * ptr,ZL_INT index,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZL_INT argindex = ZENGL_RUN_REGVAL(ZL_R_RT_ARRAY_ITEM).dword - 2; //之所以减2，是因为在要设置的值和索引之间还压入了一个原始环境下的ARRAY_ITEM寄存器的值在栈中，所以要减2
	run->is_inMemBlkSetVal = ZL_TRUE; //将is_inMemBlkSetVal设为TRUE，这样下面VStackListOps获取栈中要设置的值时，就会直接返回该值，如果该值是引用也会直接返回，不会再递归引用。
	(*tmpmem) = run->VStackListOps(VM_ARG,ZL_R_VMOPT_GETMEM,argindex,(*tmpmem),ZL_TRUE);
	run->is_inMemBlkSetVal = ZL_FALSE;
	switch(tmpmem->runType) //根据值类型设置数组元素，同时将值也同步设置到AX中，这样设置完数组后，返回值AX中也有此值，就可以继续使用该值参与其他的表达式的运算。
	{
	case ZL_R_RDT_INT:
		run->VMemBlockOps(VM_ARG,ZL_R_VMOPT_SETMEM_INT,ptr,index,tmpmem);
		ZENGL_RUN_REG(ZL_R_RT_AX).runType = ZL_R_RDT_INT;
		ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword = ptr->mem_array[index].val.dword;
		break;
	case ZL_R_RDT_FLOAT:
		run->VMemBlockOps(VM_ARG,ZL_R_VMOPT_SETMEM_DOUBLE,ptr,index,tmpmem);
		ZENGL_RUN_REG(ZL_R_RT_AX).runType = ZL_R_RDT_FLOAT;
		ZENGL_RUN_REGVAL(ZL_R_RT_AX).qword = ptr->mem_array[index].val.qword;
		break;
	case ZL_R_RDT_STR:
		run->VMemBlockOps(VM_ARG,ZL_R_VMOPT_SETMEM_STR,ptr,index,tmpmem);
		ZENGL_RUN_REG(ZL_R_RT_AX).runType = ZL_R_RDT_STR;
		run->RegAssignStr(VM_ARG,ZL_R_RT_AX,ptr->mem_array[index].val.str);
		break;
	case ZL_R_RDT_MEM_BLOCK:
		run->VMemBlockOps(VM_ARG,ZL_R_VMOPT_SETMEM_MEMBLOCK,ptr,index,tmpmem);
		ZENGL_RUN_REG(ZL_R_RT_AX).runType = ZL_R_RDT_MEM_BLOCK;
		ZENGL_RUN_REGVAL(ZL_R_RT_AX).memblock = tmpmem->val.memblock;
		ZENGL_RUN_REG(ZL_R_RT_AX).memblk_Index = tmpmem->memblk_Index;
		break;
	case ZL_R_RDT_ADDR:
	case ZL_R_RDT_ADDR_LOC:
	case ZL_R_RDT_ADDR_MEMBLK:
		run->VMemBlockOps(VM_ARG,ZL_R_VMOPT_SETMEM_ADDR,ptr,index,tmpmem);
		ZENGL_RUN_REG(ZL_R_RT_AX).runType = tmpmem->runType;
		ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword = tmpmem->val.dword;
		if(tmpmem->runType == ZL_R_RDT_ADDR_MEMBLK)
		{
			ZENGL_RUN_REGVAL(ZL_R_RT_AX).memblock = tmpmem->val.memblock;
			ZENGL_RUN_REG(ZL_R_RT_AX).memblk_Index = tmpmem->memblk_Index;
		}
		break;
	} //switch(tmpmem->runType)
}

/*对数组ptr里的index索引指向的内存进行操作*/
ZENGL_RUN_VIRTUAL_MEM_STRUCT zenglrun_VMemBlockOps(ZL_VOID * VM_ARG,ZENGL_RUN_VMEM_OP_TYPE op,ZENGL_RUN_VIRTUAL_MEM_LIST * ptr,ZL_INT index,
												   ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZL_INT len;
	ZL_VOID ** tmptr;
	ZENGL_RUN_VIRTUAL_MEM_STRUCT retmem = (*tmpmem);
	if(index >= ptr->size || index < 0)
		run->exit(VM_ARG,ZL_ERR_RUN_MEM_BLOCK_INVALID_INDEX);
	if(op != ZL_R_VMOPT_ADDMEM_ADDR && op != ZL_R_VMOPT_SETMEM_ADDR)
	{
		switch(ptr->mem_array[index].runType)
		{
		case ZL_R_RDT_ADDR: //如果该数组元素是全局变量引用则调用VMemListOps
			return run->VMemListOps(VM_ARG,op,ptr->mem_array[index].val.dword,(*tmpmem));
			break;
		case ZL_R_RDT_ADDR_LOC: //局部变量引用则调用VStackListOps
			return run->VStackListOps(VM_ARG,op,ptr->mem_array[index].val.dword,(*tmpmem),ZL_TRUE);
			break;
		case ZL_R_RDT_ADDR_MEMBLK: //如果是另一个数组元素的引用，则递归调用VMemBlockOps
			return run->VMemBlockOps(VM_ARG,op,ptr->mem_array[index].val.memblock,ptr->mem_array[index].val.dword,tmpmem);
			break;
		}
	}

	if(op != ZL_R_VMOPT_GETMEM)
	{
		if(tmpmem->runType != ZL_R_RDT_STR && ptr->mem_array[index].runType == ZL_R_RDT_STR && 
			ptr->mem_array[index].str_Index > 0) //在设置某数组元素时，先将数组元素之前存储的字符串内存空间释放掉。
		{
			run->memFreeIndex(VM_ARG,ptr->mem_array[index].val.str,&ptr->mem_array[index].str_Index);
			ptr->mem_array[index].val.str = ZL_NULL;
		}
		else if(ptr->mem_array[index].runType == ZL_R_RDT_MEM_BLOCK) //在设置某数组元素时，如果该数组元素本身又是一个数组，则将该数组元素分配的内存块释放掉。
		{
			run->memblock_free(VM_ARG,(ZENGL_RUN_VIRTUAL_MEM_LIST *)ptr->mem_array[index].val.memblock,&ptr->mem_array[index].memblk_Index);
			ptr->mem_array[index].val.memblock = ZL_NULL;
		}
	}
	switch(op)
	{
	case ZL_R_VMOPT_SETMEM_INT: //将数组元素设为整数
	case ZL_R_VMOPT_ADDMEM_INT:
		ptr->mem_array[index].runType = ZL_R_RDT_INT;
		ptr->mem_array[index].val.dword = tmpmem->val.dword;
		if(ptr->mem_array[index].isvalid == ZL_FALSE)
		{
			ptr->count++;
			ptr->mem_array[index].isvalid = ZL_TRUE;
		}
		break;
	case ZL_R_VMOPT_SETMEM_DOUBLE: //将数组元素设为浮点数
	case ZL_R_VMOPT_ADDMEM_DOUBLE:
		ptr->mem_array[index].runType = ZL_R_RDT_FLOAT;
		ptr->mem_array[index].val.qword = tmpmem->val.qword;
		if(ptr->mem_array[index].isvalid == ZL_FALSE)
		{
			ptr->count++;
			ptr->mem_array[index].isvalid = ZL_TRUE;
		}
		break;
	case ZL_R_VMOPT_SETMEM_STR: //将数组元素设为字符串
	case ZL_R_VMOPT_ADDMEM_STR:
		ptr->mem_array[index].runType = ZL_R_RDT_STR;
		tmptr = &ptr->mem_array[index].val.str;
		len = ZENGL_SYS_STRLEN(tmpmem->val.str);
		(*tmptr) = run->memReUsePtr(VM_ARG,(*tmptr),(len+1) * sizeof(ZL_CHAR),&ptr->mem_array[index].str_Index);
		ZENGL_SYS_STRNCPY(*tmptr,tmpmem->val.str,len);
		(*((ZL_CHAR **)tmptr))[len] = ZL_STRNULL;
		if(ptr->mem_array[index].isvalid == ZL_FALSE)
		{
			ptr->count++;
			ptr->mem_array[index].isvalid = ZL_TRUE;
		}
		break;
	case ZL_R_VMOPT_SETMEM_MEMBLOCK: //将数组元素设为一个新的数组
	case ZL_R_VMOPT_ADDMEM_MEMBLOCK:
		ptr->mem_array[index].runType = ZL_R_RDT_MEM_BLOCK;
		run->assign_memblock(VM_ARG,&(ptr->mem_array[index].val.memblock),tmpmem->val.memblock);
		ptr->mem_array[index].memblk_Index = tmpmem->memblk_Index; //同时设置内存块指针在内存池中的索引
		if(ptr->mem_array[index].isvalid == ZL_FALSE)
		{
			ptr->count++;
			ptr->mem_array[index].isvalid = ZL_TRUE;
		}
		break;
	case ZL_R_VMOPT_SETMEM_ADDR: //将数组元素设为另一个内存元素的引用。
	case ZL_R_VMOPT_ADDMEM_ADDR:
		ptr->mem_array[index].runType = tmpmem->runType;
		ptr->mem_array[index].val.dword = tmpmem->val.dword;
		if(tmpmem->runType == ZL_R_RDT_ADDR_MEMBLK)
		{
			ptr->mem_array[index].val.memblock = tmpmem->val.memblock;
			ptr->mem_array[index].memblk_Index = tmpmem->memblk_Index; //同时设置内存块指针在内存池中的索引
		}
		if(ptr->mem_array[index].isvalid == ZL_FALSE)
		{
			ptr->count++;
			ptr->mem_array[index].isvalid = ZL_TRUE;
		}
		break;
	case ZL_R_VMOPT_SETMEM_NONE: //将数组元素设为未初始化状态
	case ZL_R_VMOPT_ADDMEM_NONE:
		ptr->mem_array[index].runType = ZL_R_RDT_NONE;
		if(ptr->mem_array[index].isvalid == ZL_FALSE)
		{
			ptr->count++;
			ptr->mem_array[index].isvalid = ZL_TRUE;
		}
		break;
	case ZL_R_VMOPT_GETMEM: //获取数组元素的值。
		retmem = ptr->mem_array[index];
		return retmem;
		break;
	} //switch(op)
	return retmem;
}

/*释放memblock所管理的内存块，先将refcount减一，当refcount小于等于0时，就循环加递归将内存块包括内存块里的子内存块全部释放掉*/
ZL_VOID zenglrun_memblock_free(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_LIST * memblock,ZL_INT * index)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZL_INT i;
	if(memblock == ZL_NULL)
		return;
	memblock->refcount--;
	if(memblock->refcount <= 0)
	{
		for(i=0;i<memblock->size;i++)
		{
			if(memblock->mem_array[i].runType == ZL_R_RDT_STR && 
				memblock->mem_array[i].str_Index > 0)
			{
				run->memFreeIndex(VM_ARG,memblock->mem_array[i].val.str,&memblock->mem_array[i].str_Index);
				memblock->mem_array[i].val.str = ZL_NULL;
			}
			else if(memblock->mem_array[i].runType == ZL_R_RDT_MEM_BLOCK &&
				memblock->mem_array[i].val.memblock != ZL_NULL)
			{
				run->memblock_free(VM_ARG,(ZENGL_RUN_VIRTUAL_MEM_LIST *)memblock->mem_array[i].val.memblock,&memblock->mem_array[i].memblk_Index);
				memblock->mem_array[i].val.memblock = ZL_NULL;
			}
		}
		run->memFreeIndex(VM_ARG,memblock->mem_array,&memblock->mempool_index); //没有这条就会内存一直增大，导致内存泄漏。。
		run->memFreeIndex(VM_ARG,memblock,index);
	}
}

/*获取数组中的某元素*/
ZL_VOID zenglrun_op_get_array(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_RUN_VIRTUAL_MEM_LIST * ptr;
	ZL_INT argcount,index,ptrIndex,i;
	ZENGL_RUN_VMEM_OP_GET(ZL_R_CUR_INST.src.type,(*tmpmem),src,ZL_ERR_RUN_INVALID_VMEM_TYPE)
	if(tmpmem->val.memblock == ZL_NULL)
	{
		ptr = run->alloc_memblock(VM_ARG,&ptrIndex);
		tmpmem->val.memblock = (ZL_VOID *)ptr;
		tmpmem->memblk_Index = ptrIndex;
		ZENGL_RUN_VMEM_OP(ZL_R_CUR_INST.src.type,ZL_R_VMOPT_ADDMEM_MEMBLOCK,ZL_R_VMOPT_SETMEM_MEMBLOCK,(*tmpmem),src,ZL_ERR_RUN_INVALID_VMEM_TYPE)
	}
	else
		ptr = (ZENGL_RUN_VIRTUAL_MEM_LIST *)tmpmem->val.memblock;
	argcount = run->vstack_list.count - ZENGL_RUN_REGVAL(ZL_R_RT_ARRAY_ITEM).dword;
	if(argcount == 0)
		index = ptr->count;
	else if(argcount == 1)
		index = run->memblock_getindex(VM_ARG,0,tmpmem);
	else
	{
		for(i=0;i < argcount-1;i++)
		{
			index = run->memblock_getindex(VM_ARG,i,tmpmem);
			if(index < 0)
				run->exit(VM_ARG,ZL_ERR_RUN_MEM_BLOCK_INVALID_INDEX);
			ptr = run->realloc_memblock(VM_ARG,ptr,index);
			if(ptr->mem_array[index].val.memblock == ZL_NULL)
			{
				ptr->mem_array[index].runType = ZL_R_RDT_MEM_BLOCK;
				run->assign_memblock(VM_ARG,&(ptr->mem_array[index].val.memblock) ,
					run->alloc_memblock(VM_ARG,&ptr->mem_array[index].memblk_Index));
			}
			ptr = ptr->mem_array[index].val.memblock;
		}
		index = run->memblock_getindex(VM_ARG,i,tmpmem);
	}
	if(index < 0)
		run->exit(VM_ARG,ZL_ERR_RUN_MEM_BLOCK_INVALID_INDEX);
	ptr = run->realloc_memblock(VM_ARG,ptr,index);
	(*tmpmem) = run->VMemBlockOps(VM_ARG,ZL_R_VMOPT_GETMEM,ptr,index,tmpmem);
	switch(tmpmem->runType)
	{
	case ZL_R_RDT_NONE:
		ZENGL_RUN_REG(ZL_R_RT_AX).runType = ZL_R_RDT_INT;
		ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword = 0;
		break;
	case ZL_R_RDT_INT:
		ZENGL_RUN_REG(ZL_R_RT_AX).runType = ZL_R_RDT_INT;
		ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword = tmpmem->val.dword;
		break;
	case ZL_R_RDT_FLOAT:
		ZENGL_RUN_REG(ZL_R_RT_AX).runType = ZL_R_RDT_FLOAT;
		ZENGL_RUN_REGVAL(ZL_R_RT_AX).qword = tmpmem->val.qword;
		break;
	case ZL_R_RDT_STR:
		ZENGL_RUN_REG(ZL_R_RT_AX).runType = ZL_R_RDT_STR;
		run->RegAssignStr(VM_ARG,ZL_R_RT_AX,tmpmem->val.str);
		break;
	case ZL_R_RDT_MEM_BLOCK:
		ZENGL_RUN_REG(ZL_R_RT_AX).runType = ZL_R_RDT_MEM_BLOCK;
		ZENGL_RUN_REGVAL(ZL_R_RT_AX).memblock = tmpmem->val.memblock;
		ZENGL_RUN_REG(ZL_R_RT_AX).memblk_Index = tmpmem->memblk_Index;
		break;
	case ZL_R_RDT_ADDR:
	case ZL_R_RDT_ADDR_LOC:
	case ZL_R_RDT_ADDR_MEMBLK:
		ZENGL_RUN_REG(ZL_R_RT_AX).runType = tmpmem->runType;
		ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword = tmpmem->val.dword;
		if(tmpmem->runType == ZL_R_RDT_ADDR_MEMBLK)
		{
			ZENGL_RUN_REGVAL(ZL_R_RT_AX).memblock = tmpmem->val.memblock;
			ZENGL_RUN_REG(ZL_R_RT_AX).memblk_Index = tmpmem->memblk_Index;
		}
		break;
	} //switch(tmpmem->runType)
	run->vstack_list.count = ZENGL_RUN_REGVAL(ZL_R_RT_ARRAY_ITEM).dword; //恢复栈空间
	(*tmpmem) = run->VStackListOps(VM_ARG,ZL_R_VMOPT_GETMEM,-1,(*tmpmem),ZL_TRUE);
	if(tmpmem->runType == ZL_R_RDT_INT)
		ZENGL_RUN_REGVAL(ZL_R_RT_ARRAY_ITEM).dword = tmpmem->val.dword;
	else
		run->exit(VM_ARG,ZL_ERR_RUN_MEM_BLOCK_VSTACK_STRUCT_EXCEPTION,ZL_R_REG_PC);
}

/*获取内存块元素的引用，如test = &testarray[0];的语句*/
ZL_VOID zenglrun_op_get_array_addr(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_RUN_VIRTUAL_MEM_LIST * ptr;
	ZL_INT argcount,index,ptrIndex,i;
	ZENGL_RUN_VMEM_OP_GET(ZL_R_CUR_INST.src.type,(*tmpmem),src,ZL_ERR_RUN_INVALID_VMEM_TYPE)
	if(tmpmem->val.memblock == ZL_NULL)
	{
		ptr = run->alloc_memblock(VM_ARG,&ptrIndex);
		tmpmem->val.memblock = (ZL_VOID *)ptr;
		tmpmem->memblk_Index = ptrIndex;
		ZENGL_RUN_VMEM_OP(ZL_R_CUR_INST.src.type,ZL_R_VMOPT_ADDMEM_MEMBLOCK,ZL_R_VMOPT_SETMEM_MEMBLOCK,(*tmpmem),src,ZL_ERR_RUN_INVALID_VMEM_TYPE)
	}
	else
	{
		ptr = (ZENGL_RUN_VIRTUAL_MEM_LIST *)tmpmem->val.memblock;
		ptrIndex = tmpmem->memblk_Index;
	}
	argcount = run->vstack_list.count - ZENGL_RUN_REGVAL(ZL_R_RT_ARRAY_ITEM).dword;
	if(argcount == 0)
		index = ptr->count;
	else if(argcount == 1)
		index = run->memblock_getindex(VM_ARG,0,tmpmem);
	else
	{
		for(i=0;i < argcount-1;i++)
		{
			index = run->memblock_getindex(VM_ARG,i,tmpmem);
			if(index < 0)
				run->exit(VM_ARG,ZL_ERR_RUN_MEM_BLOCK_INVALID_INDEX);
			ptr = run->realloc_memblock(VM_ARG,ptr,index);
			if(ptr->mem_array[index].val.memblock == ZL_NULL)
			{
				ptr->mem_array[index].runType = ZL_R_RDT_MEM_BLOCK;
				run->assign_memblock(VM_ARG,&(ptr->mem_array[index].val.memblock) ,
					run->alloc_memblock(VM_ARG,&ptr->mem_array[index].memblk_Index));
			}
			ptr = ptr->mem_array[index].val.memblock;
			ptrIndex = ptr->mem_array[index].memblk_Index;
		}
		index = run->memblock_getindex(VM_ARG,i,tmpmem);
	}
	if(index < 0)
		run->exit(VM_ARG,ZL_ERR_RUN_MEM_BLOCK_INVALID_INDEX);
	ptr = run->realloc_memblock(VM_ARG,ptr,index);
	ZENGL_RUN_REG(ZL_R_RT_AX).runType = ZL_R_RDT_ADDR_MEMBLK; //设置AX返回值的运行时类型为 ZL_R_RDT_ADDR_MEMBLK 即内存块引用
	ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword = index; //dword存放引用内存块的索引
	ZENGL_RUN_REGVAL(ZL_R_RT_AX).memblock = ptr; //memblock存放引用内存块的指针
	ZENGL_RUN_REG(ZL_R_RT_AX).memblk_Index = ptrIndex; //memblk_Index存放的是memblock指针在内存池中的索引
	run->vstack_list.count = ZENGL_RUN_REGVAL(ZL_R_RT_ARRAY_ITEM).dword; //恢复栈空间
	(*tmpmem) = run->VStackListOps(VM_ARG,ZL_R_VMOPT_GETMEM,-1,(*tmpmem),ZL_TRUE);
	if(tmpmem->runType == ZL_R_RDT_INT)
		ZENGL_RUN_REGVAL(ZL_R_RT_ARRAY_ITEM).dword = tmpmem->val.dword;
	else
		run->exit(VM_ARG,ZL_ERR_RUN_MEM_BLOCK_VSTACK_STRUCT_EXCEPTION,ZL_R_REG_PC);
}

/*对数组元素进行加加，减减运算*/
ZL_VOID zenglrun_op_addminis_one_array(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem,ZENGL_RUN_INST_TYPE op)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_RUN_VIRTUAL_MEM_LIST * ptr;
	ZL_INT argcount,index,ptrIndex,i;
	ZENGL_RUN_VMEM_OP_GET(ZL_R_CUR_INST.src.type,(*tmpmem),src,ZL_ERR_RUN_INVALID_VMEM_TYPE)
	if(tmpmem->val.memblock == ZL_NULL) //如果没分配过内存块则通过alloc_memblock为其分配内存块
	{
		ptr = run->alloc_memblock(VM_ARG,&ptrIndex);
		tmpmem->val.memblock = (ZL_VOID *)ptr;
		tmpmem->memblk_Index = ptrIndex;
		ZENGL_RUN_VMEM_OP(ZL_R_CUR_INST.src.type,ZL_R_VMOPT_ADDMEM_MEMBLOCK,ZL_R_VMOPT_SETMEM_MEMBLOCK,(*tmpmem),src,ZL_ERR_RUN_INVALID_VMEM_TYPE)
	}
	else
		ptr = (ZENGL_RUN_VIRTUAL_MEM_LIST *)tmpmem->val.memblock;
	argcount = run->vstack_list.count - ZENGL_RUN_REGVAL(ZL_R_RT_ARRAY_ITEM).dword; //得到数组的索引总维数，如test[1,2,3]那么argcount就是3
	if(argcount == 0) //如果没有索引信息如test[]，则直接在数组末尾添加一个元素
		index = ptr->count;
	else if(argcount == 1) //如果为一维数组，则直接memblock_getindex得到索引即可
		index = run->memblock_getindex(VM_ARG,0,tmpmem);
	else
	{
		for(i=0;i < argcount-1;i++) //如果维度大于等于2，则先循环根据需要得到数组的各维度的内存块，例如test[1,2,3]则i为0时为索引1分配内存块这样test[1]也成为了一个数组，然后i为1时为索引2分配内存块这样test[1,2]也成为了一个数组，test[1,2,3]就是test[1,2]数组中的第4个元素(索引从0开始)
		{
			index = run->memblock_getindex(VM_ARG,i,tmpmem);
			if(index < 0)
				run->exit(VM_ARG,ZL_ERR_RUN_MEM_BLOCK_INVALID_INDEX);
			ptr = run->realloc_memblock(VM_ARG,ptr,index); //默认分配的内存块并不大，当index索引超过内存块大小时，就需要动态分配内存块以满足index索引的需求
			if(ptr->mem_array[index].val.memblock == ZL_NULL) //如果此维度没有分配过内存块则alloc_memblock为其分配内存块，同时通过assign_memblock来增加内存块的引用计数
			{
				ptr->mem_array[index].runType = ZL_R_RDT_MEM_BLOCK;
				run->assign_memblock(VM_ARG,&(ptr->mem_array[index].val.memblock) ,
					run->alloc_memblock(VM_ARG,&ptr->mem_array[index].memblk_Index));
			}
			ptr = ptr->mem_array[index].val.memblock;
		}
		index = run->memblock_getindex(VM_ARG,i,tmpmem); //得到最后一维度的索引
	}
	if(index < 0)
		run->exit(VM_ARG,ZL_ERR_RUN_MEM_BLOCK_INVALID_INDEX);
	ptr = run->realloc_memblock(VM_ARG,ptr,index); //ptr为该索引所在的内存块
	(*tmpmem) = run->VMemBlockOps(VM_ARG,ZL_R_VMOPT_GETMEM,ptr,index,tmpmem); //根据内存块和索引值得到具体的内存
	switch(tmpmem->runType) //根据内存里的值进行加加，减减运算，并同步设置AX返回值
	{
	case ZL_R_RDT_INT:
		switch(op)
		{
		case ZL_R_IT_ADDGET_ARRAY:
			tmpmem->val.dword++;
			run->VMemBlockOps(VM_ARG,ZL_R_VMOPT_SETMEM_INT,ptr,index,tmpmem);
			break;
		case ZL_R_IT_MINISGET_ARRAY:
			tmpmem->val.dword--;
			run->VMemBlockOps(VM_ARG,ZL_R_VMOPT_SETMEM_INT,ptr,index,tmpmem);
			break;
		}
		ZENGL_RUN_REG(ZL_R_RT_AX).runType = ZL_R_RDT_INT;
		ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword = tmpmem->val.dword;
		switch(op)
		{
		case ZL_R_IT_GETADD_ARRAY:
			tmpmem->val.dword++;
			run->VMemBlockOps(VM_ARG,ZL_R_VMOPT_SETMEM_INT,ptr,index,tmpmem);
			break;
		case ZL_R_IT_GETMINIS_ARRAY:
			tmpmem->val.dword--;
			run->VMemBlockOps(VM_ARG,ZL_R_VMOPT_SETMEM_INT,ptr,index,tmpmem);
			break;
		}
		break; //case ZL_R_RDT_INT:
	case ZL_R_RDT_FLOAT:
		switch(op)
		{
		case ZL_R_IT_ADDGET_ARRAY:
			tmpmem->val.qword++;
			run->VMemBlockOps(VM_ARG,ZL_R_VMOPT_SETMEM_DOUBLE,ptr,index,tmpmem);
			break;
		case ZL_R_IT_MINISGET_ARRAY:
			tmpmem->val.qword--;
			run->VMemBlockOps(VM_ARG,ZL_R_VMOPT_SETMEM_DOUBLE,ptr,index,tmpmem);
			break;
		}
		ZENGL_RUN_REG(ZL_R_RT_AX).runType = ZL_R_RDT_FLOAT;
		ZENGL_RUN_REGVAL(ZL_R_RT_AX).qword = tmpmem->val.qword;
		switch(op)
		{
		case ZL_R_IT_GETADD_ARRAY:
			tmpmem->val.qword++;
			run->VMemBlockOps(VM_ARG,ZL_R_VMOPT_SETMEM_DOUBLE,ptr,index,tmpmem);
			break;
		case ZL_R_IT_GETMINIS_ARRAY:
			tmpmem->val.qword--;
			run->VMemBlockOps(VM_ARG,ZL_R_VMOPT_SETMEM_DOUBLE,ptr,index,tmpmem);
			break;
		}
		break; //case ZL_R_RDT_FLOAT:
	case ZL_R_RDT_STR:
		{
			ZL_CHAR tmpstr[40];
			ZL_INT tmpint;
			ZL_DOUBLE tmpfloat;
			ZL_BOOL	isfloat;
			if(ZENGL_SYS_STRCHR(tmpmem->val.str,'.') == ZL_NULL)
			{
				tmpint = ZENGL_SYS_STR_TO_NUM(tmpmem->val.str);
				isfloat = ZL_FALSE;
			}
			else
			{
				tmpfloat = ZENGL_SYS_STR_TO_FLOAT(tmpmem->val.str);
				isfloat = ZL_TRUE;
			}
			switch(op)
			{
			case ZL_R_IT_ADDGET_ARRAY:
			case ZL_R_IT_MINISGET_ARRAY:
				if(isfloat == ZL_FALSE)
				{
					if(op == ZL_R_IT_ADDGET_ARRAY)
						tmpint++;
					else
						tmpint--;
					ZENGL_SYS_SPRINTF(tmpstr,"%d",tmpint);
				}
				else
				{
					if(op == ZL_R_IT_ADDGET_ARRAY)
						tmpfloat++;
					else
						tmpfloat--;
					ZENGL_SYS_SPRINTF(tmpstr,"%.16g",tmpfloat);
				}
				tmpmem->val.str = tmpstr;
				run->VMemBlockOps(VM_ARG,ZL_R_VMOPT_SETMEM_STR,ptr,index,tmpmem);
				break;
			default:
				if(isfloat == ZL_FALSE)
					ZENGL_SYS_SPRINTF(tmpstr,"%d",tmpint);
				else
					ZENGL_SYS_SPRINTF(tmpstr,"%.16g",tmpfloat);
				break;
			}
			ZENGL_RUN_REG(ZL_R_RT_AX).runType = tmpmem->runType;
			run->RegAssignStr(VM_ARG,ZL_R_RT_AX,tmpstr);
			switch(op)
			{
			case ZL_R_IT_GETADD_ARRAY:
			case ZL_R_IT_GETMINIS_ARRAY:
				if(isfloat == ZL_FALSE)
				{
					if(op == ZL_R_IT_ADDGET_ARRAY)
						tmpint++;
					else
						tmpint--;
					ZENGL_SYS_SPRINTF(tmpstr,"%d",tmpint);
				}
				else
				{
					if(op == ZL_R_IT_ADDGET_ARRAY)
						tmpfloat++;
					else
						tmpfloat--;
					ZENGL_SYS_SPRINTF(tmpstr,"%.16g",tmpfloat);
				}
				tmpmem->val.str = tmpstr;
				run->VMemBlockOps(VM_ARG,ZL_R_VMOPT_SETMEM_STR,ptr,index,tmpmem);
				break;
			}
		}
		break; //case ZL_R_RDT_STR:
	default:
		ZENGL_RUN_REG(ZL_R_RT_AX).runType = ZL_R_RDT_INT;
		ZENGL_RUN_REGVAL(ZL_R_RT_AX).dword = 0;
		break;
	} //switch(tmpmem->runType)
	run->vstack_list.count = ZENGL_RUN_REGVAL(ZL_R_RT_ARRAY_ITEM).dword; //在访问数组元素时会将索引压入栈，因为数组已经访问完了，所以这里就可以根据ARRAY_ITEM寄存器的值恢复栈顶，丢弃掉索引值
	(*tmpmem) = run->VStackListOps(VM_ARG,ZL_R_VMOPT_GETMEM,-1,(*tmpmem),ZL_TRUE); //将原始环境中的ARRAY_ITEM寄存器值弹出。VStackListOps第三个参数为-1表示弹出栈中最后一个元素
	if(tmpmem->runType == ZL_R_RDT_INT)
		ZENGL_RUN_REGVAL(ZL_R_RT_ARRAY_ITEM).dword = tmpmem->val.dword; //使用弹出的值恢复ARRAY_ITEM寄存器
	else
		run->exit(VM_ARG,ZL_ERR_RUN_MEM_BLOCK_VSTACK_STRUCT_EXCEPTION,ZL_R_REG_PC);
}

/*释放栈中参数部分和局部变量部分的所有内存块*/
ZL_VOID zenglrun_memblock_freeall_local(ZL_VOID * VM_ARG)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZL_INT locIndex = ZENGL_RUN_REGVAL(ZL_R_RT_ARG).dword;
	for(;locIndex < run->vstack_list.count;locIndex++)
	{
		switch(run->vstack_list.mem_array[locIndex].runType)
		{
		case ZL_R_RDT_MEM_BLOCK:
			if(run->vstack_list.mem_array[locIndex].val.memblock != ZL_NULL)
			{
				run->memblock_free(VM_ARG,run->vstack_list.mem_array[locIndex].val.memblock,
					&run->vstack_list.mem_array[locIndex].memblk_Index);
				run->vstack_list.mem_array[locIndex].runType = ZL_R_RDT_NONE;
				run->vstack_list.mem_array[locIndex].val.dword = 0;
				run->vstack_list.mem_array[locIndex].val.memblock = ZL_NULL;
			}
			break;
		case ZL_R_RDT_ADDR: //将局部变量和参数里的引用也释放掉
		case ZL_R_RDT_ADDR_LOC:
		case ZL_R_RDT_ADDR_MEMBLK:
			run->vstack_list.mem_array[locIndex].runType = ZL_R_RDT_NONE;
			run->vstack_list.mem_array[locIndex].val.dword = 0;
			run->vstack_list.mem_array[locIndex].val.memblock = ZL_NULL;
			run->vstack_list.mem_array[locIndex].memblk_Index = 0;
			break;
		}
	}
}

/*重利用虚拟机时，释放掉全局虚拟内存，栈内存等里面的内存块和引用*/
ZL_VOID zenglrun_FreeAllForReUse(ZL_VOID * VM_ARG)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZL_INT locIndex,globalIndex;
	for(locIndex = 0;locIndex < run->vstack_list.count;locIndex++) //释放掉栈内存中的内存块和引用
	{
		switch(run->vstack_list.mem_array[locIndex].runType)
		{
		case ZL_R_RDT_MEM_BLOCK:
			if(run->vstack_list.mem_array[locIndex].val.memblock != ZL_NULL)
			{
				run->memblock_free(VM_ARG,run->vstack_list.mem_array[locIndex].val.memblock,
					&run->vstack_list.mem_array[locIndex].memblk_Index);
				run->vstack_list.mem_array[locIndex].runType = ZL_R_RDT_NONE;
				run->vstack_list.mem_array[locIndex].val.dword = 0;
				run->vstack_list.mem_array[locIndex].val.memblock = ZL_NULL;
			}
			break;
		case ZL_R_RDT_ADDR: //将局部变量和参数里的引用也释放掉
		case ZL_R_RDT_ADDR_LOC:
		case ZL_R_RDT_ADDR_MEMBLK:
			run->vstack_list.mem_array[locIndex].runType = ZL_R_RDT_NONE;
			run->vstack_list.mem_array[locIndex].val.dword = 0;
			run->vstack_list.mem_array[locIndex].val.memblock = ZL_NULL;
			run->vstack_list.mem_array[locIndex].memblk_Index = 0;
			break;
		default:
			run->vstack_list.mem_array[locIndex].runType = ZL_R_RDT_NONE;
			run->vstack_list.mem_array[locIndex].val.dword = 0;
			break;
		}
	}
	for(globalIndex = 0;globalIndex < run->vmem_list.count;globalIndex++) //释放掉全局虚拟内存中的内存块和引用
	{
		switch(run->vmem_list.mem_array[globalIndex].runType)
		{
		case ZL_R_RDT_MEM_BLOCK:
			if(run->vmem_list.mem_array[globalIndex].val.memblock != ZL_NULL)
			{
				run->memblock_free(VM_ARG,run->vmem_list.mem_array[globalIndex].val.memblock,
					&run->vmem_list.mem_array[globalIndex].memblk_Index);
				run->vmem_list.mem_array[globalIndex].runType = ZL_R_RDT_NONE;
				run->vmem_list.mem_array[globalIndex].val.dword = 0;
				run->vmem_list.mem_array[globalIndex].val.memblock = ZL_NULL;
			}
			break;
		case ZL_R_RDT_ADDR: //将局部变量和参数里的引用也释放掉
		case ZL_R_RDT_ADDR_LOC:
		case ZL_R_RDT_ADDR_MEMBLK:
			run->vmem_list.mem_array[globalIndex].runType = ZL_R_RDT_NONE;
			run->vmem_list.mem_array[globalIndex].val.dword = 0;
			run->vmem_list.mem_array[globalIndex].val.memblock = ZL_NULL;
			run->vmem_list.mem_array[globalIndex].memblk_Index = 0;
			break;
		default:
			run->vmem_list.mem_array[globalIndex].runType = ZL_R_RDT_NONE;
			run->vmem_list.mem_array[globalIndex].val.dword = 0;
			break;
		}
	}
}

/*SWITCH指令的处理*/
ZL_VOID zenglrun_op_switch(ZL_VOID * VM_ARG)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZL_INT count=0,min=0,max=0,num,golast,table;
	golast = ZL_R_CUR_INST.src.val.num; //SWITCH指令的源操作数是当在switch跳转表中找不到对应的值时，需要跳转的默认位置或结束位置
	table = ZL_R_CUR_INST.dest.val.num; //目标操作数是当前switch...case结构的跳转地址表所在的汇编代码位置
	if(ZENGL_RUN_INST(ZL_R_REG_PC+1).type == ZL_R_IT_LONG)
		count = ZENGL_RUN_INST(ZL_R_REG_PC+1).src.val.num; //SWITCH后的LONG的操作数是跳转表里的总数
	else
		run->exit(VM_ARG,ZL_ERR_RUN_SWITCH_HAVE_NO_LONG_INST_BELOW);
	if(count == 0) //如果跳转表为空，那么就没什么可比较的，直接跳到golast
		ZL_R_REG_PC = golast - 1;  //减一是因为在下条指令执行前会自动对ZL_R_REG_PC加一
	else
	{
		if(ZENGL_RUN_INST(ZL_R_REG_PC+2).type == ZL_R_IT_LONG) //SWITCH后的第二个LONG里存放了跳转表里可供比较的最大值和最小值
		{
			min = ZENGL_RUN_INST(ZL_R_REG_PC+2).dest.val.num; //目标操作数为最小值
			max = ZENGL_RUN_INST(ZL_R_REG_PC+2).src.val.num;  //源操作数为最大值
			num = run->getRegInt(VM_ARG,ZL_R_RT_AX); //将要进行比较的AX里的值转为整数形式返回
			if(num < min || num > max) //如果num比min还小，或比max还要大，则这就跳到golast
			{
				ZL_R_REG_PC = golast - 1;
				return;
			}
			else
			{
				if(count == max - min + 1) //如果跳转表里要进行比较的数是递增形式如 1,2,3,4,5,6这样的，就可以直接从跳转地址表中计算出要进行跳转的目标地址
				{
					ZL_R_REG_PC = ZENGL_RUN_INST(table + num - min).dest.val.num - 1; //跳转地址存放在跳转表的每一项的目标操作数中
					return;
				}
				else
				{
					if(num == min)	 //如果等于最小值，则返回跳转表第一项的跳转地址
					{
						ZL_R_REG_PC = ZENGL_RUN_INST(table).dest.val.num - 1;
						return;
					}
					else if(num == max) //如果等于最大值，则返回跳转表最后一项的跳转地址
					{
						ZL_R_REG_PC = ZENGL_RUN_INST(table + count - 1).dest.val.num - 1;
						return;
					}
					else //否则，采用折中法，循环取中间项进行比较，找到匹配的项目，如果没找到，则跳到golast
					{
						ZL_INT minIndex=0,maxIndex=count - 1,diff,diffnum;
						do
						{
							diff = (maxIndex - minIndex)/2;
							if(diff == 0)
							{
								ZL_R_REG_PC = golast - 1;
								return;
							}
							else
							{
								diffnum = ZENGL_RUN_INST(table + minIndex + diff).src.val.num;
								if(num == diffnum)
								{
									ZL_R_REG_PC = ZENGL_RUN_INST(table + minIndex + diff).dest.val.num - 1;
									return;
								}
								else if(num > diffnum)
									minIndex += diff;
								else
									maxIndex = minIndex + diff;
							}
						}while(ZL_TRUE);
					}
				} //if(count == max - min + 1) ... else ...
			} //if(num < min || num > max) ... else ...
		} //if(ZENGL_RUN_INST(ZL_R_REG_PC+2).type == ZL_R_IT_LONG)
	} //if(ZENGL_RUN_INST(ZL_R_REG_PC+2).type == ZL_R_IT_LONG) ... else ...
}

/*返回寄存器值的整数形式*/
ZL_INT zenglrun_getRegInt(ZL_VOID * VM_ARG,ZENGL_RUN_REG_TYPE reg)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	switch(ZENGL_RUN_REG(reg).runType)
	{
	case ZL_R_RDT_INT:
		return ZENGL_RUN_REGVAL(reg).dword;
		break;
	case ZL_R_RDT_FLOAT:
		return (ZL_INT)(ZENGL_RUN_REGVAL(reg).qword);
		break;
	case ZL_R_RDT_STR:
		return ZENGL_SYS_STR_TO_NUM((ZL_CHAR *)ZENGL_RUN_REGVAL(reg).str);
		break;
	}
	return 0;
}

/*解释器的入口函数*/
ZL_INT zenglrun_main(ZL_VOID * VM_ARG)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZL_INT retcode;
	ZENGL_API_STATES origState;
	run->isinRunning = ZL_TRUE;
	run->start_time = ZENGL_SYS_TIME_CLOCK();
	if((retcode = ZENGL_SYS_JMP_SETJMP(run->jumpBuffer))==0)
	{
		if(VM->vm_main_args->userdef_module_init != ZL_NULL)
		{
			origState = VM->ApiState;
			VM->ApiState = ZL_API_ST_MODULES_INIT;
			((ZL_VM_API_MODS_INIT)VM->vm_main_args->userdef_module_init)(VM_ARG); //调用用户自定义的模块初始化函数
			VM->ApiState = origState;
		}
		run->RunInsts(VM_ARG);
		if(VM->isUseApiSetErrThenStop == ZL_TRUE) //如果通过zenglApi_SetErrThenStop接口来停止虚拟机的，就通过exit_forApiSetErrThenStop来退出
			run->exit_forApiSetErrThenStop(VM_ARG);
		else
			run->exit(VM_ARG,ZL_NO_ERR_SUCCESS);
	}
	if(retcode == 1)
		return 0;
	else
		return -1;
	return 0;
}
