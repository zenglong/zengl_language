/*
   Copyright 2017 zenglong (made in china)

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
 * 该文件中主要存放与内存块的哈希数组相关的函数
 */

#include "zengl_global.h"

/**
 * 初始化哈希数组的哈希表
 */
static ZL_VOID zenglrun_initHashCodeTable(ZL_VOID * VM_ARG, ZENGL_RUN_HASH_CODE_TABLE * hash_code_table)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	if(hash_code_table->members != ZL_NULL)
		return;
	hash_code_table->count = 0;
	hash_code_table->size = ZL_R_HASH_CODE_TABLE_SIZE;
	hash_code_table->members = (ZENGL_RUN_HASH_CODE_TABLE_MEMBER *)run->memAlloc(VM_ARG, hash_code_table->size * sizeof(ZENGL_RUN_HASH_CODE_TABLE_MEMBER),
										&hash_code_table->mempool_index);
	if(hash_code_table->members == ZL_NULL)
		run->exit(VM_ARG,ZL_ERR_RUN_HASH_ARRAY_MEM_ALLOC_FAILED);
	ZENGL_SYS_MEM_SET(hash_code_table->members, 0, hash_code_table->size * sizeof(ZENGL_RUN_HASH_CODE_TABLE_MEMBER));
}
