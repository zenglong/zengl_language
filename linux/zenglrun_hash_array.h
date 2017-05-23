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

#ifndef _ZENGL_HASH_ARRAY_H_
#define _ZENGL_HASH_ARRAY_H_

#include "zengl_global_header.h"

#define ZL_R_HASH_CODE_TABLE_SIZE 10 // 哈希数组的初始化及动态扩容的大小
#define ZL_R_HASH_STR_POOL_SIZE 120  // 哈希字符串池的初始化及动态扩容的大小
#define ZL_R_HASH_CODE_SHIFT 4  // 哈希值的计算因子

typedef struct _ZENGL_RUN_HASH_CODE_TABLE_MEMBER{
	ZL_INT hash_code; // 字符串的hash code
	ZL_INT str_len;   // 字符串的长度
	ZL_INT str_offset; // 字符串在字符串池中的字节偏移值
	ZL_INT memblock_index; // 内存块中，字符串对应的真实的索引值
	ZL_INT hits; //命中次数，命中次数越多，在哈希表中越靠前
}ZENGL_RUN_HASH_CODE_TABLE_MEMBER; // 哈希数组中单个成员的定义

typedef struct _ZENGL_RUN_HASH_CODE_TABLE{
	ZL_INT size; // 哈希表对应的动态数组的尺寸
	ZL_INT count; // 哈希表中存储的元素个数
	ZL_INT last_index; // 记录最后一个哈希表成员所对应的索引值
	ZL_INT mempool_index; //下面的members指针在内存池中的索引
	ZENGL_RUN_HASH_CODE_TABLE_MEMBER * members; // 存放具体的哈希数组成员
}ZENGL_RUN_HASH_CODE_TABLE; // 哈希数组的结构定义

typedef struct _ZENGL_RUN_HASH_STR_POOL{
	ZL_INT size; // 字符串池的尺寸
	ZL_INT count; // 字符串池中实际包含的字符数
	ZL_INT mempool_index; //下面的ptr指针在内存池中的索引
	ZL_CHAR * ptr; // 存放所有字符串
}ZENGL_RUN_HASH_STR_POOL; // 哈希数组中的字符串池

typedef struct _ZENGL_RUN_HASH_ARRAY{
	ZENGL_RUN_HASH_CODE_TABLE hash_code_table; // 哈希数组中的哈希表
	ZENGL_RUN_HASH_STR_POOL str_pool; // 哈希数组中的字符串都存储在str_pool中
}ZENGL_RUN_HASH_ARRAY;

#endif/* _ZENGL_HASH_ARRAY_H_ */
