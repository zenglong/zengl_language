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
	该文件存放的是和链接器相关的程式。
*/

#include "zengl_global.h"

/*
	设置链接动态数组中某索引(伪地址)对应的实际汇编代码位置
*/
ZL_INT zengl_LDAddrListSet(ZL_VOID * VM_ARG,ZL_INT index,ZL_INT loc)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZL_INT old_size;
	if(!compile->LDAddrList.isInit)
		compile->LDAddrListInit(VM_ARG);
	if(index >= compile->LDAddrList.size)
	{
		old_size = compile->LDAddrList.size;
		while(index >= compile->LDAddrList.size)
			compile->LDAddrList.size += ZL_LD_ADDRLIST_SIZE;
		compile->LDAddrList.addr = compile->memReAlloc(VM_ARG,compile->LDAddrList.addr,compile->LDAddrList.size * sizeof(ZENGL_LD_ADDR_TYPE));
		ZENGL_SYS_MEM_SET(compile->LDAddrList.addr + old_size,0,
				(compile->LDAddrList.size - old_size) * sizeof(ZENGL_LD_ADDR_TYPE));
	}
	if(compile->LDAddrList.addr[index].isvalid == ZL_FALSE)
	{
		compile->LDAddrList.addr[index].loc = loc;
		compile->LDAddrList.addr[index].isvalid = ZL_TRUE;
		compile->LDAddrList.count++;
	}
	else if(compile->LDAddrList.addr[index].loc != loc)
	{
		compile->exit(VM_ARG,ZL_ERR_CP_LD_INDEX_ADDR_MUST_UNIQUE,loc);
	}
	return index;
}

/*
	链接动态数组初始化
*/
ZL_VOID zengl_LDAddrListInit(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	if(compile->LDAddrList.isInit)
		return;
	compile->LDAddrList.size = ZL_LD_ADDRLIST_SIZE;
	compile->LDAddrList.count = 0;
	compile->LDAddrList.addr = (ZENGL_LD_ADDR_TYPE *)compile->memAlloc(VM_ARG,compile->LDAddrList.size * sizeof(ZENGL_LD_ADDR_TYPE));
	if(compile->LDAddrList.addr == ZL_NULL)
		compile->exit(VM_ARG,ZL_ERR_CP_LD_ADDR_LIST_MALLOC_FAILED);
	ZENGL_SYS_MEM_SET(compile->LDAddrList.addr,0,compile->LDAddrList.size * sizeof(ZENGL_LD_ADDR_TYPE));
	compile->LDAddrList.isInit = ZL_TRUE;
}

/*
	将所有的伪地址替换为真实的汇编代码位置，从而完成链接工作
*/
ZL_VOID zengl_LDAddrListReplaceAll(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZL_INT i,j;
	ZENGL_RUN_INST_OP_DATA * op_data;
	for(i=0;i<run->inst_list.count;i++)
	{
		for(j=1;j<=2;j++)
		{
			if(j==1)
				op_data = &run->inst_list.insts[i].dest;
			else
				op_data = &run->inst_list.insts[i].src;
			
			if(op_data->type == ZL_R_DT_LDADDR)
			{
				if(compile->LDAddrList.addr[op_data->val.num].isvalid == ZL_FALSE)
					compile->exit(VM_ARG,ZL_ERR_CP_LD_INVALID_ADDR_INDEX,op_data->val.num,op_data->val.num);
				op_data->val.num = compile->LDAddrList.addr[op_data->val.num].loc;
				op_data->type = ZL_R_DT_NUM;
			}
		}
	}
}
