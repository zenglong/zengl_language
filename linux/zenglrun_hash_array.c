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
 * 根据字符串key计算哈希值
 */
static ZL_INT zenglrun_getHashCode(ZL_CHAR * key)
{
	ZL_INT temp = 0;
	ZL_INT i = 0;
	while (key[i] != ZL_STRNULL)
	{
		temp = ((temp << ZL_R_HASH_CODE_SHIFT) | key[i]);
		++i;
	}
	return temp;
}

/**
 * 检测字符串是否是数字
 */
static ZL_BOOL zenglrun_detectIsDigit(ZL_CHAR * key)
{
	ZL_INT i = 0;
	while (key[i] != ZL_STRNULL)
	{
		if(!ZENGL_SYS_CTYPE_IS_DIGIT(key[i]))
		{
			return ZL_FALSE;
		}
	}
	return ZL_TRUE;
}

/**
 * 初始化哈希字符串池
 */
static ZL_VOID zenglrun_initHashStrPool(ZL_VOID * VM_ARG, ZENGL_RUN_HASH_STR_POOL * str_pool)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	if(str_pool->ptr != ZL_NULL)
		return;
	str_pool->count = 2;
	str_pool->size = ZL_R_HASH_STR_POOL_SIZE;
	str_pool->ptr = (ZL_CHAR *)run->memAlloc(VM_ARG, str_pool->size * sizeof(ZL_CHAR), &str_pool->mempool_index);
	if(str_pool->ptr == ZL_NULL)
		run->exit(VM_ARG,ZL_ERR_RUN_HASH_ARRAY_MEM_ALLOC_FAILED);
	ZENGL_SYS_MEM_SET(str_pool->ptr, 0, str_pool->size * sizeof(ZL_CHAR));
}

/**
 * 添加字符串到哈希字符串池中
 */
static ZL_INT zenglrun_HashStrPoolAddString(ZL_VOID * VM_ARG, ZENGL_RUN_HASH_STR_POOL * str_pool, ZL_CHAR * str)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZL_INT len;
	ZL_INT i,j;
	if(str_pool->ptr == ZL_NULL)
		zenglrun_initHashStrPool(VM_ARG, str_pool);
	if(str == ZL_NULL)
		return -1;
	len = ZENGL_SYS_STRLEN(str);
	if(str_pool->count == str_pool->size ||
		str_pool->count + len + 1 > str_pool->size)
	{
		str_pool->size += ZL_R_HASH_STR_POOL_SIZE;
		str_pool->ptr = (ZL_CHAR *)run->memReAlloc(VM_ARG, str_pool->ptr, str_pool->size * sizeof(ZL_CHAR), &str_pool->mempool_index);
		ZENGL_SYS_MEM_SET(str_pool->ptr + (str_pool->size - ZL_R_HASH_STR_POOL_SIZE), 0, ZL_R_HASH_STR_POOL_SIZE * sizeof(ZL_CHAR));
	}
	for(i=str_pool->count,j=0;i < str_pool->size && j < len;i++,j++)
	{
		str_pool->ptr[i] = str[j];
	}
	str_pool->ptr[i] = ZL_STRNULL;
	i = str_pool->count;
	str_pool->count += len + 1;
	return i;
}

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

/**
 * 将字符串key添加到
 */
static ZL_VOID zenglrun_addKeyToHashCodeTable(ZL_VOID * VM_ARG, ZENGL_RUN_HASH_CODE_TABLE * hash_code_table, ZENGL_RUN_HASH_STR_POOL * str_pool,
												ZL_CHAR * key, ZL_INT str_len,
												ZL_INT hash_code, ZL_INT memblock_index)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	if(hash_code_table->members == ZL_NULL)
			zenglrun_initHashCodeTable(VM_ARG, hash_code_table);
	if(hash_code_table->count == hash_code_table->size)
	{
		hash_code_table->size += ZL_R_HASH_CODE_TABLE_SIZE;
		hash_code_table->members = (ZENGL_RUN_HASH_CODE_TABLE_MEMBER *)run->memReAlloc(VM_ARG,
							hash_code_table->members,
							hash_code_table->size * sizeof(ZENGL_RUN_HASH_CODE_TABLE_MEMBER),
							&hash_code_table->mempool_index);
		ZENGL_SYS_MEM_SET((hash_code_table->members + (hash_code_table->size - ZL_R_HASH_CODE_TABLE_SIZE)), 0,
				ZL_R_HASH_CODE_TABLE_SIZE * sizeof(ZENGL_RUN_HASH_CODE_TABLE_MEMBER));
	}
	hash_code_table->members[hash_code_table->count].hash_code = hash_code;
	hash_code_table->members[hash_code_table->count].str_len = str_len;
	hash_code_table->members[hash_code_table->count].str_offset = zenglrun_HashStrPoolAddString(VM_ARG, str_pool, key);
	hash_code_table->members[hash_code_table->count].memblock_index = memblock_index;
	hash_code_table->members[hash_code_table->count].hits = 0;
	hash_code_table->count++;
}

/**
 * 从哈希数组的哈希表中，根据字符串key获取对应的索引值
 */
ZL_INT zenglrun_getIndexFromHashCodeTable(ZL_VOID * VM_ARG, ZENGL_RUN_VIRTUAL_MEM_LIST * memblock, ZL_CHAR * key)
{
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_RUN_HASH_CODE_TABLE * hash_code_table = &(memblock->hash_array.hash_code_table);
	ZENGL_RUN_HASH_STR_POOL * str_pool = &(memblock->hash_array.str_pool);
	ZENGL_RUN_HASH_CODE_TABLE_MEMBER tmp_member; // 根据hits交换成员时，需要用到的临时变量
	ZL_INT hash_code,i,str_len,min_hits,min_hits_index,ret_memblock_index;
	ZL_CHAR * str_pool_ptr;
	if(zenglrun_detectIsDigit(key)) // 如果key只包含了数字，则直接将其转为对应的整数返回
		return ZENGL_SYS_STR_TO_NUM(key);
	if(str_pool->ptr == ZL_NULL)
		zenglrun_initHashStrPool(VM_ARG, str_pool);
	hash_code = zenglrun_getHashCode(key);
	str_len = ZENGL_SYS_STRLEN(key);
	min_hits_index = 0;
	// 对min_hits进行初始化
	if(hash_code_table->count > 0)
		min_hits = hash_code_table->members[0].hits;
	else
		min_hits = 0;
	for(i=0;i < hash_code_table->count;i++)
	{
		// 检测最小的hits及对应的索引
		if(min_hits > hash_code_table->members[i].hits) {
			min_hits = hash_code_table->members[i].hits;
			min_hits_index = i;
		}
		if(hash_code_table->members[i].hash_code != hash_code)
			continue;
		if(hash_code_table->members[i].str_len != str_len)
			continue;
		str_pool_ptr = str_pool->ptr + hash_code_table->members[i].str_offset;
		if(ZENGL_SYS_STRCMP(key, str_pool_ptr) != 0)
			continue;
		hash_code_table->members[i].hits += 1;
		ret_memblock_index = hash_code_table->members[i].memblock_index; // 将要返回的内存块索引赋值给ret_memblock_index，防止受到下面数据交换的影响
		// 如果当前选中的哈希表成员的命中次数比前面的最少命中成员的命中次数要大的话，就交换这两个成员，以提高当前成员下次响应的速度
		if(hash_code_table->members[i].hits > min_hits && i > min_hits_index) {
			tmp_member = hash_code_table->members[min_hits_index];
			hash_code_table->members[min_hits_index] = hash_code_table->members[i];
			hash_code_table->members[i] = tmp_member;
		}
		return ret_memblock_index;
	}
	zenglrun_addKeyToHashCodeTable(VM_ARG, hash_code_table, str_pool, key, str_len, hash_code, memblock->count);
	return memblock->count;
}
