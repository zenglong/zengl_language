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

#ifndef _ZENGL_GLOBAL_HEADER_H_
#define _ZENGL_GLOBAL_HEADER_H_

#ifdef ZL_EXP_OS_IN_ZENGLOX
	#include <stdlib.h>
	#include "setjmp.h"
	#include "zengl_locals.h"
	#include "zengl_exportfuns.h"
#else
	#include <stdio.h>
	#include <stdlib.h>
	#include <memory.h>
	#include <stdarg.h>
	#include <ctype.h>
	#include <string.h>
	#include <time.h>
	#include "setjmp.h"
	#include "zengl_locals.h"
	#include "zengl_exportfuns.h"
#endif

/*一些宏定义*/
#define ZL_VM_SIGNER 0x4D564C5A //判断虚拟机结构体是否是有效结构体的signer标志，即ZLVM四个英文字母的ASCII码，由低字节向高字节排列
#define ZL_FALSE 0 //逻辑假
#define ZL_TRUE 1 //逻辑真
#define ZL_FILE_BUF_SIZE 1024 //读取文件的缓存大小
#define ZL_FILE_MAX_PATH 350 //最大路径值
#define ZL_FILE_EOF EOF //判断是否读到文件结尾
#define ZL_SYM_HASH_SIZE 211 //符号hash表数组的大小
#define ZL_SYM_HASH_TOTAL_SIZE 1477 //HASH表的总大小。 目前有7个动态数组，每个动态数组使用哈希表的211项，所以一共1477项
#define ZL_SYM_HASH_SHIFT 4  //符号名转为符号hash表索引时的计算因子
#define ZL_RC4_STATE_SIZE 256 //RC4加密运算state状态盒子里的元素个数
#define ZL_STRNULL '\0' //字符串结束字符
#define ZL_NULL 0 //指针为0的宏定义
#define ZL_VOID void //采用自定义的宏来代替void , char之类的C标准类型，方便以后的统一调整，这几个类型宏也可以用typedef来处理。
#ifdef ZL_EXP_OS_IN_ARM_GCC
	#define ZL_CHAR signed char //使用signed表示有符号的意思，因为ARM GCC下char默认是unsigned的(嵌入式上面会引发很多问题！)，所以有必要在这里指明是signed
#else
	#define ZL_CHAR char
#endif
#define ZL_UCHAR unsigned char
#define ZL_BYTE unsigned char
#define ZL_INT int
#define ZL_LONG long
#define ZL_WCHAR_T wchar_t
#define ZL_FLOAT float
#define ZL_DOUBLE double
#define ZL_CONST const
#define ZL_EXTERN extern
typedef unsigned char ZL_BOOL; //定义bool类型
typedef size_t ZL_SIZE_T;
typedef unsigned int ZL_UINT;
typedef clock_t ZL_CLOCK_T;
#define ZL_TOKENSTR_SIZE 40 //compile中的tokenInfo的str初始化和动态扩容的大小
#define ZL_MEM_POOL_SIZE 40 //内存池初始化和动态扩容的大小
#define ZL_DEF_STR_POOL_SIZE 1024 //常量宏对应的字符串池初始化和动态扩容的大小
#define ZL_DEF_TABLE_SIZE 20 //def定义的常量表对应动态数组初始化和动态扩容的大小
#define ZL_LINECOL_SIZE 211 //compile中的LineCols行列号动态数组初始化和动态扩容的大小
#define ZL_FILE_STACK_SIZE 10 //用于inc加载文件的堆栈的初始化和扩容的大小。
#define ZL_INFO_STRING_SIZE 1024 //用于信息字符串的初始化和动态扩容的大小。
#define ZENGL_SYS_PRINTF printf //打印语句的宏定义，可以使用printf，也可以用自定义的函数，比如输出到日志文件的函数等。
#define ZENGL_SYS_VPRINTF vprintf //可以通过函数的arglist来打印可选参数列表的函数
#define ZENGL_SYS_FILE_OPEN fopen //打开文件函数
#define ZENGL_SYS_FILE_GETS fgets //读取文件内容到缓存中的函数
#define ZENGL_SYS_FILE_READ fread //读取文件内容到缓存中，主要用于读取二进制文件
#define ZENGL_SYS_FILE_EOF feof //读取文件内容到缓存中的函数
#define ZENGL_SYS_FILE_CLOSE fclose //关闭文件句柄
#define ZENGL_SYS_MEM_ALLOC malloc //内存分配函数
#define ZENGL_SYS_MEM_RE_ALLOC realloc //内存重新调整大小的函数
#define ZENGL_SYS_MEM_SET memset //设置连续内存中的值
#define ZENGL_SYS_MEM_COPY memcpy //连续内存拷贝函数
#define ZENGL_SYS_MEM_FREE free //释放内存资源
#define ZENGL_SYS_CTYPE_IS_ALPHA isalpha //ctype.h中声明的标准库函数，判断字符是否为英文字母
#define ZENGL_SYS_CTYPE_IS_DIGIT isdigit //ctype.h中声明的标准库函数，判断字符是否为数字
#define ZENGL_SYS_CTYPE_IS_HEXDIGIT isxdigit //ctype.h中声明的标准库函数，判断字符是否为十六进制数
#define ZENGL_SYS_CTYPE_IS_ALNUM isalnum //ctype.h中声明的标准库函数，判断字符是否为字母或数字
#define ZENGL_SYS_SPRINTF sprintf //将字符串格式化输出到缓存中
#ifdef ZL_LANG_IN_VC6
	#define ZENGL_SYS_SPRINTF_ARG_NUM _vsnprintf //使用va_list和限制了字符数的格式化输出函数，另外VC6编译器下的vsnprintf需要在前面加下划线！
#else
	#define ZENGL_SYS_SPRINTF_ARG_NUM vsnprintf //使用va_list和限制了字符数的格式化输出函数
#endif
#define ZENGL_SYS_STRLEN strlen //得到字符串长度信息
#define ZENGL_SYS_STRCMP strcmp //字符串比较函数
#define ZENGL_SYS_STRNCMP strncmp //字符串比较函数
#define ZENGL_SYS_STRNCPY strncpy //字符串拷贝函数
#define ZENGL_SYS_STRCHR strchr //在字符串中查找某字符
#define ZENGL_SYS_STR_TO_NUM atoi //将字符串转为整数
#define ZENGL_SYS_STR_TO_LONG_NUM atol //将字符串转为长整数
#define ZENGL_SYS_STR_TO_FLOAT atof //将字符串转为浮点数
#define ZENGL_SYS_TIME_CLOCK clock //获取CPU执行的tick滴答值，用于统计执行时间
#define ZENGL_SYS_JMP_LONGJMP_TO longjmp //长跳转，用于出错时跳转
#define ZENGL_SYS_JMP_SETJMP setjmp //设置长跳转的地址
#define ZENGL_SYS_ARG_LIST va_list //可变参数列表
#define ZENGL_SYS_ARG_START va_start //可变参数起始地址
#define ZENGL_SYS_ARG_GET  va_arg  //获取可变参数值
#define ZENGL_SYS_ARG_END va_end //结束可变参数列表定义
#ifndef va_copy
	#define va_copy(dst, src)   memcpy(&(dst), &(src), sizeof(va_list))
#endif
#define ZENGL_SYS_ARG_COPY va_copy // 拷贝可变参数列表，主要用于64位系统中的GCC和clang
/*宏定义结束*/

#endif/* _ZENGL_GLOBAL_HEADER_H_ */
