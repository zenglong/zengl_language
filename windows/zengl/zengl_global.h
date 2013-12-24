/*
 * global.h
 *
 *  Created on: 2012-1-22
 *      Author: zenglong
 */

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

#ifndef _ZENGL_GLOBAL_H_
#define _ZENGL_GLOBAL_H_

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
#define ZENGL_SYS_STR_TO_FLOAT atof //将字符串转为浮点数
#define ZENGL_SYS_TIME_CLOCK clock //获取CPU执行的tick滴答值，用于统计执行时间
#define ZENGL_SYS_JMP_LONGJMP_TO longjmp //长跳转，用于出错时跳转
#define ZENGL_SYS_JMP_SETJMP setjmp //设置长跳转的地址
#define ZENGL_SYS_ARG_LIST va_list //可变参数列表
#define ZENGL_SYS_ARG_START va_start //可变参数起始地址
#define ZENGL_SYS_ARG_GET  va_arg  //获取可变参数值
#define ZENGL_SYS_ARG_END va_end //结束可变参数列表定义
/*宏定义结束*/

/*API接口的各种状态枚举定义*/
typedef enum _ZENGL_API_STATES{
	ZL_API_ST_NONE,
	ZL_API_ST_OPEN,
	ZL_API_ST_RESET,
	ZL_API_ST_REUSE,
	ZL_API_ST_RUN,
	ZL_API_ST_AFTER_RUN,
	ZL_API_ST_MODULES_INIT,
	ZL_API_ST_MOD_INIT_HANDLE,
	ZL_API_ST_MOD_FUN_HANDLE,
}ZENGL_API_STATES;
/*API接口的各种状态枚举定义结束*/

/*token节点类型定义*/
typedef enum _ZENGL_TOKENTYPE{

	ZL_TK_START_NONE,		//初始值，不对应任何token
	ZL_TK_ID,				//变量之类的标识符token
	ZL_TK_RESERVE,			//关键字token
	ZL_TK_NUM,				//如123之类的数字的token
	ZL_TK_FLOAT,			//如3.14159之类的浮点数
	ZL_TK_STR,				//字符串token
	ZL_TK_ASSIGN,			//赋值运算符token
	ZL_TK_EQUAL,			//"=="等于运算符token
	ZL_TK_GREAT_EQ,			//">="大于等于运算符token
	ZL_TK_GREAT,			//">"大于运算符token
	ZL_TK_LESS_EQ,			//"<="小于等于运算符token
	ZL_TK_LESS,				//"<"小于运算符token
	ZL_TK_NOT_EQ,			//"!="不等于运算符token
	ZL_TK_REVERSE,			//"!"取反运算符token
	ZL_TK_AND,				//"&&"逻辑且运算符token
	ZL_TK_ADDRESS,			//"&"引用运算符token
	ZL_TK_OR,				//"||"逻辑或运算符token
	ZL_TK_PLUS_PLUS,		//++运算符token
	ZL_TK_PLUS_ASSIGN,		//+=运算符token
	ZL_TK_PLUS,				//加法运算符token
	ZL_TK_MINIS_MINIS,		//--运算符token
	ZL_TK_MINIS_ASSIGN,		//-=运算符token
	ZL_TK_MINIS,			//减法运算符token
	ZL_TK_NEGATIVE,			//负号单目运算符token
	ZL_TK_TIMES_ASSIGN,		//*=运算符token
	ZL_TK_TIMES,			//乘法运算符token
	ZL_TK_DIVIDE_ASSIGN,	// "/=" 除赋值token
	ZL_TK_DIVIDE,			//除法运算符token
	ZL_TK_MOD_ASSIGN,		// "%=" 取余赋值运算符token
	ZL_TK_MOD,				//取余运算符token
	ZL_TK_LBRACKET,			//左括号token
	ZL_TK_RBRACKET,			//右括号token
	ZL_TK_LMBRACKET,		//左中括号token
	ZL_TK_RMBRACKET,		//右中括号token
	ZL_TK_ENDFILE,			//文件结束符token
	ZL_TK_SEMI,				//分号token
	ZL_TK_COMMA,			//逗号token
	ZL_TK_COLON,			//冒号token
	ZL_TK_QUESTION_MARK,	//问号token
	ZL_TK_DOT,				//点运算符
	ZL_TK_BIT_AND,			//"&"按位与双目运算符token
	ZL_TK_BIT_AND_ASSIGN,	//&=运算符token
	ZL_TK_BIT_OR,			//"|"按位或双目运算符token
	ZL_TK_BIT_OR_ASSIGN,	//|=运算符token
	ZL_TK_BIT_XOR,			//"^"按位异或运算符token
	ZL_TK_BIT_XOR_ASSIGN,	//^=运算符token
	ZL_TK_BIT_RIGHT,		//">>"右移运算符token
	ZL_TK_BIT_RIGHT_ASSIGN,	//">>="右移赋值运算符token
	ZL_TK_BIT_LEFT,			//"<<"左移运算符token
	ZL_TK_BIT_LEFT_ASSIGN,	//"<<="左移赋值运算符token
	ZL_TK_BIT_REVERSE,		//"~"按位取反运算符token
	ZL_TK_FUNCALL,			//函数调用token 例如max(a,b); 表示max函数 
	ZL_TK_ARRAY_ITEM,		//数组元素形式 例如test[2];表示test数组的第三个元素，从0开始
	ZL_TK_CLASS_STATEMENT,	//类声明语句，例如 Poker x,y;其中Poker为某个类 x,y为该类声明的变量
	ZL_TK_ERROR				//未定义的token

}ZENGL_TOKENTYPE;
/*token节点类型定义结束*/

/*在switch case里要用到的各种状态的枚举定义*/
typedef enum _ZENGL_STATES{
	ZL_ST_START,				//开始状态
	ZL_ST_DOWN,					//结束状态
	ZL_ST_INID,					//处于标识符扫描状态
	ZL_ST_INEIGHT_HEX,			//处于扫描八进制和十六进制的状态
	ZL_ST_INEIGHT,				//处于扫描八进制的状态
	ZL_ST_INHEX,				//处于扫描十六进制的状态
	ZL_ST_INNUM,				//处于数字扫描状态
	ZL_ST_INFLOAT,				//处于浮点数扫描状态
	ZL_ST_INSTR,				//处于字符串扫描状态
	ZL_ST_INPLUS,				//处于加号运算符的扫描状态
	ZL_ST_INMINIS,				//处于减号运算符的扫描状态
	ZL_ST_INTIMES,				//处于乘号运算符的扫描状态
	ZL_ST_INDIVIDE,				//处于除号运算符的扫描状态
	ZL_ST_INMOD,				//处于取余运算符的扫描状态
	ZL_ST_INASSIGN,				//处于赋值运算符的扫描状态
	ZL_ST_INGREAT,				//处于大于运算符的扫描状态
	ZL_ST_INLESS,				//处于小于运算符的扫描状态
	ZL_ST_INNOT_EQ,				//处于不等于运算符的扫描状态
	ZL_ST_INAND,				//处于逻辑且运算符的扫描状态
	ZL_ST_INOR,					//处于逻辑或运算符的扫描状态
	ZL_ST_INCOMMENT,			//单行注释扫描状态
	ZL_ST_INMULTI_COMMENT,		//多行注释扫描状态
	ZL_ST_INENDMULTI_COMMENT,	//多行注释扫描结束状态
	ZL_ST_INXOR,				//按位异或运算符的扫描状态
	ZL_ST_INBIT_RIGHT,			//右移运算符的扫描状态
	ZL_ST_INBIT_LEFT,			//左移运算符的扫描状态
	ZL_ST_PARSER_INSEMI,		//在zengl_parser.c中添加的分号token状态
	ZL_ST_PARSER_INEQUAL,		//在zengl_parser.c中添加的等于token状态
	ZL_ST_PARSER_INGREAT_EQ,	//在zengl_parser.c中添加的大于等于token状态
	ZL_ST_PARSER_INLESS_EQ,		//在zengl_parser.c中添加的小于等于token状态
	ZL_ST_PARSER_INREVERSE,		//在zengl_parser.c中添加的取反运算符token状态
	ZL_ST_PARSER_INADDRESS,		//在zengl_parser.c中添加的引用运算符token状态
	ZL_ST_PARSER_INPLUS_PLUS,	//在zengl_parser.c中添加的++运算符token状态
	ZL_ST_PARSER_INPLUS_ASSIGN,	//在zengl_parser.c中添加的+=运算符token状态
	ZL_ST_PARSER_INMINIS_MINIS,	//在zengl_parser.c中添加的--运算符token状态
	ZL_ST_PARSER_INMINIS_ASSIGN,//在zengl_parser.c中添加的-=运算符token状态
	ZL_ST_PARSER_INTIMES_ASSIGN,//在zengl_parser.c中添加的*=运算符token状态
	ZL_ST_PARSER_INDIVIDE_ASSIGN,//在zengl_parser.c中添加的/=运算符token状态
	ZL_ST_PARSER_INMOD_ASSIGN,	//在zengl_parser.c中添加的%=运算符token状态
	ZL_ST_PARSER_INLBRACKET,	//在zengl_parser.c中添加的左括号token状态
	ZL_ST_PARSER_INRBRACKET,	//在zengl_parser.c中添加的右括号token状态
	ZL_ST_PARSER_INLMBRACKET,	//在zengl_parser.c中添加的左中括号token状态
	ZL_ST_PARSER_INRMBRACKET,	//在zengl_parser.c中添加的右中括号token状态
	ZL_ST_PARSER_INCOMMA,		//在zengl_parser.c中添加的逗号运算符token状态
	ZL_ST_PARSER_INCOLON,		//在zengl_parser.c中添加的冒号运算符token状态
	ZL_ST_PARSER_INQUESTION_MARK,//在zengl_parser.c中添加的问号运算符token状态
	ZL_ST_PARSER_INDOT,			//在zengl_parser.c中添加的点运算符token状态
	ZL_ST_PARSER_INNEGATIVE,	//在zengl_parser.c中添加的负号单目运算符token状态
	ZL_ST_PARSER_INBIT_AND,		//在zengl_parser.c中添加的按位与运算符token状态
	ZL_ST_PARSER_INBIT_AND_ASSIGN,//在zengl_parser.c中添加的&=运算符token状态
	ZL_ST_PARSER_INBIT_OR,		//在zengl_parser.c中添加的按位或运算符token状态
	ZL_ST_PARSER_INBIT_OR_ASSIGN,//在zengl_parser.c中添加的|=运算符token状态
	ZL_ST_PARSER_INBIT_XOR,		//在zengl_parser.c中添加的按位异或运算符token状态
	ZL_ST_PARSER_INBIT_XOR_ASSIGN,//在zengl_parser.c中添加的^=运算符token状态
	ZL_ST_PARSER_INBIT_RIGHT,	//在zengl_parser.c中添加的右移运算符token状态
	ZL_ST_PARSER_INBIT_RIGHT_ASSIGN,//在zengl_parser.c中添加的>>=右移赋值运算符token状态
	ZL_ST_PARSER_INBIT_LEFT,	//在zengl_parser.c中添加的左移运算符token状态
	ZL_ST_PARSER_INBIT_LEFT_ASSIGN,//在zengl_parser.c中添加的<<=左移赋值运算符token状态
	ZL_ST_PARSER_INBIT_REVERSE,	//在zengl_parser.c中添加的~按位取反运算符token状态
	ZL_ST_PARSER_STMT_INIF,		//用于生成if关键字语句AST的状态机
	ZL_ST_PARSER_STMT_INFOR,	//用于生成for关键字语句AST的状态机
	ZL_ST_PARSER_STMT_INFUN,	//用于生成fun关键字语句AST的状态机
	ZL_ST_PARSER_STMT_INSWITCH,	//用于生成switch关键字语句AST的状态机
	ZL_ST_PARSER_STMT_INCLASS,	//用于生成class关键字语句AST的状态机
	ZL_ST_PARSER_STMT_INWHILE,	//用于生成while关键字语句AST的状态机
	ZL_ST_PARSER_STMT_INDOWHILE,//用于生成do...dowhile关键字语句AST的状态机
	ZL_ST_ASM_CODE_INPRINT,		//用于生成print关键字的汇编指令
	ZL_ST_ASM_CODE_INPLUS_MINIS,//用于生成+,-,+=,-=运算符的汇编指令
	ZL_ST_ASM_CODE_INTIME_DIVIDE,//用于生成%,*,/,%=,*=,/=运算符的汇编指令
	ZL_ST_ASM_CODE_INRELATION,	//用于生成大于，小于，等于，不等于，大于等于，小于等于运算符的汇编指令
	ZL_ST_ASM_CODE_INAND_OR,	//用于生成逻辑与，逻辑或运算符的汇编指令
	ZL_ST_ASM_CODE_INREVERSE,	//用于生成逻辑非运算符的汇编指令
	ZL_ST_ASM_CODE_INBIT_REVERSE,//用于生成按位取反运算符的汇编指令
	ZL_ST_ASM_CODE_INIF,		//用于生成if语句的汇编指令
	ZL_ST_ASM_CODE_IN_PP_MM,	//用于生成加加减减的汇编指令
	ZL_ST_ASM_CODE_INFOR,		//用于生成for语句的汇编指令
	ZL_ST_ASM_CODE_INFUN,		//用于生成fun语句的汇编指令
	ZL_ST_ASM_CODE_INFUNCALL,	//用于生成funcall函数调用的汇编指令
	ZL_ST_ASM_CODE_INCOMMA,		//用于生成逗号语句的汇编指令
	ZL_ST_ASM_CODE_INUSE,		//用于生成use语句的汇编指令
	ZL_ST_ASM_CODE_INBREAK,		//用于生成break语句的汇编指令
	ZL_ST_ASM_CODE_INCONTINUE,	//用于生成continue语句的汇编指令
	ZL_ST_ASM_CODE_INGLOBAL,	//用于生成global语句的汇编指令
	ZL_ST_ASM_CODE_INRETURN,	//用于生成return语句的汇编指令
	ZL_ST_ASM_CODE_INADDRESS,	//用于生成引用语句的汇编指令
	ZL_ST_ASM_CODE_INARRAY_ITEM,//用于生成数组元素的汇编指令
	ZL_ST_ASM_CODE_INCLASS,		//用于生成class结构的汇编指令
	ZL_ST_ASM_CODE_INCLASS_STATEMENT,//用于生成类声明语句的汇编指令
	ZL_ST_ASM_CODE_INDOT,		//用于生成类成员点运行符的汇编指令
	ZL_ST_ASM_CODE_INSWITCH,	//用于生成switch结构的汇编指令
	ZL_ST_ASM_CODE_INWHILE,		//用于生成while结构的汇编指令
	ZL_ST_ASM_CODE_INDOWHILE,	//用于生成do...dowhile循环结构的汇编指令
	ZL_ST_ASM_CODE_INCOLON,		//用于生成冒号运算符的汇编指令
	ZL_ST_ASM_CODE_INQUESTION,	//用于生成问号运算符的汇编指令
	ZL_ST_ASM_CODE_INNEGATIVE,	//用于生成负号单目运算符的汇编指令
	ZL_ST_ASM_CODE_INBITS,		//用于生成按位与，或，异或等双目位运算符的汇编指令
}ZENGL_STATES;
/*在switch case里要用到的各种状态的枚举定义结束*/

/*各种需要用到哈希表的类型枚举*/
typedef enum _ZENGL_HASH_TYPES{
	ZL_HASH_TYPE_SYM_TABLE = 0,
	ZL_HASH_TYPE_FUN_TABLE = 1,
	ZL_HASH_TYPE_LOCAL_TABLE = 2,
	ZL_HASH_TYPE_CLASS_TABLE = 4,
	ZL_HASH_TYPE_CLASSMEMBER_TABLE = 5,
	ZL_HASH_TYPE_DEF_TABLE = 6,
}ZENGL_HASH_TYPES;
/*各种需要用到哈希表的类型枚举定义结束*/

/*加密算法类型枚举*/
typedef enum _ZENGL_ENCRYPT_TYPES{
	ZL_ENC_TYPE_NONE,  //没有加密，默认初始值
	ZL_ENC_TYPE_XOR,   //普通异或加密
	ZL_ENC_TYPE_RC4,   //RC4加密
}ZENGL_ENCRYPT_TYPES;
/*加密算法类型枚举定义结束*/

typedef struct _ZENGL_TOKEN_STRING_TYPE{
	ZL_CHAR * str; //token的字符串信息
	ZL_INT size; //字符串的动态大小
	ZL_INT count; //存放的字符数
	ZL_INT cur; //当前游标
	ZL_INT start_line; //token所在行
	ZL_INT start_col; //token所在列
	ZL_CHAR * filename; //用于inc加载的文件名
	ZENGL_RESERVE_TYPE reserve; //如果token是关键字，则存放关键字的类型信息
}ZENGL_TOKEN_INFO_TYPE;  //存放扫描到的token信息。

typedef struct _ZENGL_INFO_STRING_TYPE{
	ZL_CHAR * str; //信息的字符串指针
	ZL_INT size;  //字符串的动态大小
	ZL_INT count; //存放的字符数
	ZL_INT cur;  //当前游标
}ZENGL_INFO_STRING_TYPE; //存放普通打印信息或错误信息的类型

typedef struct _ZENGL_XOR_ENCRYPT{
	ZL_CHAR * xor_key_str;
	ZL_INT xor_key_len;
	ZL_INT xor_key_cur;
}ZENGL_XOR_ENCRYPT; //XOR普通异或加密运算相关的结构体定义

typedef struct _ZENGL_RC4_ENCRYPT{
	ZL_BYTE state[ZL_RC4_STATE_SIZE];
	ZL_INT i;
	ZL_INT j;
}ZENGL_RC4_ENCRYPT; //RC4加密运算相关的结构体定义

typedef struct _ZENGL_ENCRYPT{
	ZENGL_ENCRYPT_TYPES type;
	ZENGL_XOR_ENCRYPT xor;
	ZENGL_RC4_ENCRYPT rc4;
}ZENGL_ENCRYPT; //加密运算结构体定义

typedef struct _ZENGL_SOURCE_TYPE{
	ZL_CHAR *filename;
	FILE *file;
	ZL_UCHAR buf[ZL_FILE_BUF_SIZE];
	ZL_INT buf_read_len;
	ZL_INT cur;
	ZL_BOOL needread;
	ZENGL_ENCRYPT encrypt; //加密运算结构体成员
	ZL_UCHAR * run_str; //zenglApi_RunStr设置的字符串脚本
	ZL_INT run_str_len; //字符串脚本的长度
}ZENGL_SOURCE_TYPE;  //脚本源文件类型定义，里面包含要操作的脚本文件的文件指针，文件名等成员。

typedef struct _ZENGL_MEM_POOL_POINT_TYPE{
	ZL_VOID * point;
	ZL_INT size;
}ZENGL_MEM_POOL_POINT_TYPE;

typedef struct _ZENGL_MEM_POOL_TYPE{
	ZL_INT size;
	ZL_INT totalsize; //内存池动态分配的总大小，包括内存池自身的大小
	ZL_INT count;
	ZENGL_MEM_POOL_POINT_TYPE * points;
}ZENGL_MEM_POOL_TYPE;  //内存池类型定义，points成员指向一个指针数组，里面存放着编译器分配过的指针。

typedef struct _ZENGL_STRING_POOL_TYPE{
	ZL_BOOL isInit;
	ZL_INT size;
	ZL_INT count;
	ZL_CHAR * ptr; //动态分配的缓存
}ZENGL_STRING_POOL_TYPE; //将所有常量宏的字符串信息都存放在字符串池里，通过索引在池缓存中查找常量的字符串信息。

typedef struct _ZENGL_DEF_TABLE_MEMBER{
	ZL_BOOL isvalid;
	ZL_INT nameIndex; //采用整数形式的索引，因为字符串池的量达到一定程度时，就会扩容，如果用指针，指针地址就会失效！
	ZENGL_TOKENTYPE tokentype;
	ZL_INT valIndex;  //采用整数形式的索引来代替原来的指针类型。
	ZL_INT linecols;
	ZL_INT next;
} ZENGL_DEF_TABLE_MEMBER; //def宏定义动态数组里的成员结构

typedef struct _ZENGL_DEF_TABLE{
	ZL_BOOL isInit;
	ZL_INT size;
	ZL_INT count;
	ZENGL_DEF_TABLE_MEMBER * defs;
} ZENGL_DEF_TABLE;  //def宏定义动态数组

typedef struct _ZENGL_LINECOL{
	ZL_INT lineno;
	ZL_INT colno;
	ZL_CHAR * filename; //因为引入了inc，可以加载文件，所以行列号信息里还需要有文件名信息。
	ZL_INT next; //下一个元素在LineColTable对应的lines动态数组中的索引。
} ZENGL_LINECOL;  //符号表里的标示符对应的行列号，一个链表存放着标示符出现的每个位置。

typedef struct _ZENGL_LINECOL_TABLE{
	ZL_BOOL isInit;
	ZL_INT size;
	ZL_INT count;
	ZENGL_LINECOL * lines;
} ZENGL_LINECOL_TABLE; //符号表行列号结构的动态数组。

typedef struct _ZENGL_FILE_STACK_TYPE{
	ZENGL_SOURCE_TYPE source;	 //source为ZENGL_SOURCE_TYPE类型，里面存放了要加载的文件名，文件指针和文件缓存等。
	ZL_INT line;
	ZL_INT col;
}ZENGL_FILE_STACK_TYPE; //文件堆栈中每个文件的结构			

typedef struct _ZENGL_FILE_STACKLIST_TYPE{
	ZL_BOOL isInit;
	ZL_INT size;
	ZL_INT count;
	ZENGL_FILE_STACK_TYPE * stacks;
}ZENGL_FILE_STACKLIST_TYPE; //用于inc加载的文件堆栈		

/********************************************************************************
		下面是和zengl_symbol.c符号表处理相关的结构体和枚举等定义
********************************************************************************/

#define ZL_SYM_GLOBAL_TABLE_SIZE 211 //全局变量符号表动态数组初始化和动态扩容的大小
#define ZL_SYM_LOCAL_TABLE_SIZE 211	 //局部变量(包括函数参数)符号表动态数组初始化和动态扩容的大小
#define ZL_SYM_CLASS_TABLE_SIZE 50 //存放类信息的动态数组初始化和动态扩容的大小
#define ZL_SYM_CLASSMEMBER_TABLE_SIZE 211 //类成员符号表动态数组初始化和动态扩容的大小
#define ZL_SYM_FUN_TABLE_SIZE 100 //函数符号表的动态数组初始化和动态扩容的大小

typedef enum _ZENGL_SYM_ENUM_LOCAL_TYPE{
	ZL_SYM_ENUM_LOCAL_TYPE_START,	//默认初始值，不对应任何类型
	ZL_SYM_ENUM_LOCAL_TYPE_ARG,		//脚本函数的参数类型
	ZL_SYM_ENUM_LOCAL_TYPE_LOCAL,	//函数里的局部变量类型
	ZL_SYM_ENUM_LOCAL_TYPE_GLOBAL,	//函数中global声明的全局变量
}ZENGL_SYM_ENUM_LOCAL_TYPE; //局部符号表中成员的类型，例如参数类型，局部变量类型，函数中global关键字声明的全局变量

typedef struct _ZENGL_SYM_GLOBAL_TABLE_MEMBER{
	ZL_BOOL isvalid; //判断符号表某项是否被占用。
	ZL_INT nameIndex; //使用字符串池索引来代替指针
	ZL_INT classid; //类声明中的类id
	ZL_INT linecols;
	ZL_INT memloc ; /* memory location for variable 变量的内存地址*/
	ZL_INT next;
}ZENGL_SYM_GLOBAL_TABLE_MEMBER; //全局变量符号表里的成员

typedef struct _ZENGL_SYM_GLOBAL_TABLE{
	ZL_BOOL isInit; //判断sym动态数组是否初始化
	ZL_INT size;
	ZL_INT count;
	ZENGL_SYM_GLOBAL_TABLE_MEMBER * sym;
	ZL_INT global_memloc; //全局变量的内存地址计数器
}ZENGL_SYM_GLOBAL_TABLE; //全局变量符号表

typedef struct _ZENGL_SYM_LOCAL_TABLE_MEMBER{
	ZL_BOOL isvalid; //判断符号表某项是否被占用。
	ZL_INT nameIndex; //使用字符串池索引来代替指针
	ZENGL_SYM_ENUM_LOCAL_TYPE type; //成员类型
	ZL_INT localid;
	ZL_INT funid; //局部变量所在函数的id值。
	ZL_INT classid;  //局部变量对应的类id
	ZL_INT next;
}ZENGL_SYM_LOCAL_TABLE_MEMBER; //参数,局部变量符号表里的成员

typedef struct _ZENGL_SYM_LOCAL_TABLE{
	ZL_BOOL isInit;
	ZL_INT size;
	ZL_INT count;
	ZENGL_SYM_LOCAL_TABLE_MEMBER * local;
}ZENGL_SYM_LOCAL_TABLE; //脚本函数参数和局部变量符号表

typedef struct _ZENGL_SYM_CLASS_TABLE_MEMBER{
	ZL_BOOL isvalid;
	ZL_INT nameIndex;
	ZL_INT nodenum; //类定义根节点对应的AST节点号
	ZL_INT linecols;
	ZL_INT classid; //类id，类的唯一标识
	ZL_INT next;
}ZENGL_SYM_CLASS_TABLE_MEMBER; //每个类定义都要在ZENGL_SYM_CLASS_TABLE中注册一个成员，此为ZENGL_SYM_CLASS_TABLE动态数组中的成员的结构定义。

typedef struct _ZENGL_SYM_CLASS_TABLE{
	ZL_BOOL isInit; //判断class动态数组是否初始化
	ZL_INT size;
	ZL_INT count;
	ZENGL_SYM_CLASS_TABLE_MEMBER * classTable;
	ZL_INT global_classid; //类id计数器
}ZENGL_SYM_CLASS_TABLE; //存放类信息的动态数组

typedef struct _ZENGL_SYM_CLASSMEMBER_TABLE_MEMBER{
	ZL_BOOL isvalid;
	ZL_INT nameIndex;
	ZL_INT index;
	ZL_INT nodenum; //类成员所在的AST节点号，可以通过该AST节点信息得到类成员的行列号信息等，也可以用linecols，不过linecols会有额外开销，linecos可以记录多个行列号，不过此处只需要一个行列号
	ZL_INT classid;	//类成员所在class结构对应的类id
	ZL_INT cls_stmt_classid;	//类声明语句对应的类ID
	ZL_INT next;
}ZENGL_SYM_CLASSMEMBER_TABLE_MEMBER; //类成员符号表动态数组中单个元素的定义

typedef struct _ZENGL_SYM_CLASSMEMBER_TABLE{
	ZL_BOOL isInit; //判断类成员符号表动态数组是否初始化
	ZL_INT size;
	ZL_INT count;
	ZENGL_SYM_CLASSMEMBER_TABLE_MEMBER * members;
}ZENGL_SYM_CLASSMEMBER_TABLE; //类成员符号表动态数组的定义

typedef struct _ZENGL_SYM_FUN_TABLE_MEMBER{
	ZL_BOOL isvalid;
	ZL_INT nameIndex; //使用字符串池索引来代替字符串指针
	ZL_INT linecols;
	ZL_INT funid;
	ZL_INT LDAdr;	//链接伪地址
	ZL_INT classid; //如果是类函数，则包含类ID
	ZL_INT next;
}ZENGL_SYM_FUN_TABLE_MEMBER; //函数表动态数组中成员的结构定义

typedef struct _ZENGL_SYM_FUN_TABLE{
	ZL_BOOL isInit; //判断函数表动态数组是否初始化
	ZL_INT size;
	ZL_INT count;
	ZENGL_SYM_FUN_TABLE_MEMBER * funs;
	ZL_INT global_funid; //函数id计数器
}ZENGL_SYM_FUN_TABLE; //函数表动态数组的结构定义

/********************************************************************************
		上面是和zengl_symbol.c符号表处理相关的结构体和枚举等定义
********************************************************************************/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************************
		下面是和zengl_assemble.c汇编代码生成相关的结构体和枚举等定义
********************************************************************************/

#define ZL_ASM_ADDR_TYPE_NUM 22 //为ZL_ASM_STACK_ENUM_IF_ADDR,ZL_ASM_STACK_ENUM_IF_END,ZL_ASM_STACK_ENUM_FOR_ADDR,ZL_ASM_STACK_ENUM_FOR_END...等元素，以后增加时数目要对应增加！
#define ZL_ASM_STACK_LIST_SIZE 20 //生成汇编代码时因为处理嵌入式if等结构需要引入堆栈结构，这里是堆栈动态数组的初始化和动态扩容大小。
#define ZL_ASM_CASE_JMP_TABLE_SIZE 15 //switch...case的跳转表的初始值和扩容大小

typedef enum _ZENGL_ASM_STACK_ENUM{
	ZL_ASM_STACK_ENUM_IF_ADDR,			 //if,elif表达式判断为false时，需要跳转的地址
	ZL_ASM_STACK_ENUM_IF_END,			 //if-elif-else语句的结束位置
	ZL_ASM_STACK_ENUM_FOR_ADDR,			 //例如for(i=0;i<10;i++) 中间的i<10判断语句的跳转位置
	ZL_ASM_STACK_ENUM_FOR_END,			 //for...endfor结构的结束位置
	ZL_ASM_STACK_ENUM_FOR_CONTINUE,		 //for结构中continue语句会跳转的位置
	ZL_ASM_STACK_ENUM_FUN_ADDR,			 //函数的起始汇编代码位置
	ZL_ASM_STACK_ENUM_FUN_END,			 //函数的结束位置
	ZL_ASM_STACK_ENUM_ARRAY_ITEM_OP_TYPE,//判断对数组元素的操作方式，不同的操作会生成不同的汇编代码，如test[a] = 3;语句会生成SET_ARRAY的指令，b = test[a];会生成GET_ARRAY的指令等
	ZL_ASM_STACK_ENUM_IS_IN_ARRAYITEM_OR_FUNCALL, //判断当前表达式是否位于数组索引表达式中或函数调用的参数列表中，如test[a+b]，a+b位于test数组索引表达式中，所以a+b生成汇编代码后，还会PUSH将值压入栈以供数组元素进行索引。函数test(a+b-c)，a+b-c生成汇编代码后，还会PUSH压入栈成为函数test的参数值。
	ZL_ASM_STACK_ENUM_AND_ADDR,			 //逻辑与运算中例如i==0 && j==1语句中如果i==0结果为false时的跳转位置，当i==0为false时就不需要再判断j==1了，所以就可以直接跳转到语句末尾

	ZL_ASM_STACK_ENUM_OR_ADDR,			 //逻辑或运算中例如i==0 || j==1语句中如果i==0结果为true时的跳转位置，当i==0为true时就不需要再判断j==1了，所以就可以直接跳转到语句末尾
	ZL_ASM_STACK_ENUM_SWITCH_TABLE,		 //switch...case的跳转表位置
	ZL_ASM_STACK_ENUM_SWITCH_DEFAULT,	 //switch...case中default的跳转位置
	ZL_ASM_STACK_ENUM_SWITCH_END,		 //switch...case结构的结束位置
	ZL_ASM_STACK_ENUM_WHILE_ADDR,		 //while条件判断的开头位置
	ZL_ASM_STACK_ENUM_WHILE_END,		 //while结构的结束位置
	ZL_ASM_STACK_ENUM_DO_ADDR,			 //do...dowhile结构开头汇编位置
	ZL_ASM_STACK_ENUM_DO_CONTINUE,		 //do...dowhile结构中continue需要跳转的位置
	ZL_ASM_STACK_ENUM_DO_END,			 //do...dowhile结构的结束位置
	ZL_ASM_STACK_ENUM_QUESTION_ADDR,	 //例如a==b ? 1 : 2;问号冒号结构中当a==b为false时会跳转到2的汇编代码位置

	ZL_ASM_STACK_ENUM_QUESTION_END,		 //问号冒号结构的结束位置
	ZL_ASM_STACK_ENUM_FUN_CLASSID,		 //保存类函数ID信息
}ZENGL_ASM_STACK_ENUM; //汇编栈中与if,for,fun,array数组,and,or等结构的嵌套相关的宏

typedef enum _ZENGL_ASM_ARRAY_ITEM_OP_TYPE{
	ZL_ASM_AI_OP_NONE,			//默认初始值
	ZL_ASM_AI_OP_IN_MOV,		//对数组元素进行赋值操作
	ZL_ASM_AI_OP_IN_ADDR,		//对数组元素进行引用操作
	ZL_ASM_AI_OP_IN_ADDGET,		//对数组元素进行加加(先加一后获取)的操作
	ZL_ASM_AI_OP_IN_MINISGET,	//对数组元素进行减减(先减一后获取)的操作
	ZL_ASM_AI_OP_IN_GETADD,		//对数组元素进行加加(先获取后加一)的操作
	ZL_ASM_AI_OP_IN_GETMINIS,	//对数组元素进行减减(先获取后减一)的操作
}ZENGL_ASM_ARRAY_ITEM_OP_TYPE;

typedef struct _ZENGL_ASM_GENCODE_STRUCT{
	//ZENGL_STATES state; //状态机 暂没使用
	//ZL_CHAR ID_memloc_str[30]; //变量标识符在内存中的地址对应的字符串格式
	ZL_INT pc;
	ZL_BOOL is_inFun; //生成汇编码时是否位于函数中
	ZL_INT localID;	//用于脚本函数的参数和局部变量的ID计数器
}ZENGL_ASM_GENCODE_STRUCT; //zengl_AsmGenCodes函数中会用到的一些变量，放在结构体中，方便其他函数访问

typedef struct _ZENGL_ASM_STACK_TYPE{
	ZL_INT addrnum; //对应的汇编指令位置。
	ZENGL_ASM_STACK_ENUM addrtype; //地址类型，是if地址还是for结构的地址等。
	ZL_INT before_index; //和下面的ZENGL_ASM_STACKLIST_TYPE结构体里的ends字段一起得到一个地址链表。
	ZL_BOOL isvalid;
}ZENGL_ASM_STACK_TYPE; //汇编堆栈中每个元素的结构定义

typedef struct _ZENGL_ASM_STACKLIST_TYPE{
	ZL_BOOL isInit;
	ZL_INT size;
	ZL_INT count;
	ZL_INT ends[ZL_ASM_ADDR_TYPE_NUM]; //汇编堆栈根据ends字段以及上面ASM_STACK_TYPE里的before_index又划分出if地址构成的链表和for等结构的地址构成的链表。
	ZENGL_ASM_STACK_TYPE * stacks;
}ZENGL_ASM_STACKLIST_TYPE; //为了解决if,for结构的嵌入式问题，需要在生成汇编代码时引入模拟堆栈。

typedef struct _ZENGL_ASM_CASE_JMP_TABLE_MEMBER{
	ZL_INT caseNum;
	ZL_INT caseAddr;
}ZENGL_ASM_CASE_JMP_TABLE_MEMBER; //switch...case结构跳转表的成员

typedef struct _ZENGL_ASM_CASE_JMP_TABLE{
	ZL_BOOL isInit;
	ZL_INT size;
	ZL_INT count;
	ZENGL_ASM_CASE_JMP_TABLE_MEMBER * member;
}ZENGL_ASM_CASE_JMP_TABLE; //switch...case结构的跳转表

/********************************************************************************
		上面是和zengl_assemble.c汇编代码生成相关的结构体和枚举等定义
********************************************************************************/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************************
		下面是和zengl_ld.c链接器相关的结构体和枚举等定义
********************************************************************************/

#define ZL_LD_ADDRLIST_SIZE 20 //链接地址动态数组初始化和扩容的大小。

typedef struct _ZENGL_LD_ADDR_TYPE{
	ZL_BOOL isvalid; //判断是否有效，是否已在使用
	ZL_INT loc; //该伪地址对应的汇编代码位置
}ZENGL_LD_ADDR_TYPE;

typedef struct _ZENGL_LD_ADDRLIST_TYPE{
	ZL_BOOL isInit; //判断动态数组是否初始化
	ZL_INT size; //动态数组的总大小
	ZL_INT count;//动态数组中存放的元素个数
	ZENGL_LD_ADDR_TYPE * addr; //指向动态数组内存空间的指针
}ZENGL_LD_ADDRLIST_TYPE; //链接地址动态数组的类型定义

/********************************************************************************
		上面是和zengl_ld.c链接器相关的结构体和枚举等定义
********************************************************************************/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************************
		下面是和zengl_parser.c语法解析文件相关的结构体和枚举等定义
********************************************************************************/

#define ZL_AST_CHILD_NODE_SIZE 3 //语法树节点的基础子节点数，语法树子节点由基础子节点和扩展子节点构成
#define ZL_AST_SIZE 200 //AST抽象语法树动态数组的初始化和动态扩容的大小。
#define ZL_TK_STRING_POOL_SIZE 1024 //token字符串池的初始化和动态扩容的大小。
#define ZL_OPLVL_STACKLIST_SIZE 40 //虚拟堆栈动态数组的初始化和动态扩容的大小，这里的堆栈主要是为parser.c解析生成语法树时，保留和比较节点优先级用的。
#define ZL_AST_SCAN_STACKLIST_SIZE 60 //在新的利用堆栈进行节点扫描中，所用到的堆栈动态数组的初始化和动态扩容的大小。
#define ZENGL_AST_ISTOKTYPEX(nodenum,tokname) (nodenum >=0 && nodes[nodenum].isvalid == ZL_TRUE  &&  \
							  nodes[nodenum].toktype == tokname)
#define ZENGL_AST_ISTOKTYPEXOR(nodenum,tokname1,tokname2) (nodenum >=0 && nodes[nodenum].isvalid == ZL_TRUE  &&  \
							  (nodes[nodenum].toktype == tokname1 || \
							   nodes[nodenum].toktype == tokname2))
#define ZENGL_AST_ISTOKCATEX(nodenum,tokname1) (nodenum >=0 && nodes[nodenum].isvalid == ZL_TRUE  &&  \
							  (nodes[nodenum].tokcategory == tokname1))
#define ZENGL_AST_ISTOK_VALIDX(nodenum) (nodenum >=0 && nodes[nodenum].isvalid == ZL_TRUE) //判断某节点是否有效
#define ZENGL_AST_ISTOKCATEX10(nodenum,tokname1,tokname2,tokname3,tokname4,tokname5,tokname6,tokname7,tokname8,tokname9,tokname10) (nodenum >=0 && nodes[nodenum].isvalid == ZL_TRUE  &&  \
							  (nodes[nodenum].tokcategory == tokname1 || \
							   nodes[nodenum].tokcategory == tokname2 || \
							   nodes[nodenum].tokcategory == tokname3 || \
							   nodes[nodenum].tokcategory == tokname4 || \
							   nodes[nodenum].tokcategory == tokname5 || \
							   nodes[nodenum].tokcategory == tokname6 || \
							   nodes[nodenum].tokcategory == tokname7 || \
							   nodes[nodenum].tokcategory == tokname8 || \
							   nodes[nodenum].tokcategory == tokname9 || \
							   nodes[nodenum].tokcategory == tokname10))
#define ZENGL_AST_ISTOKCATEXOP(nodenum) ZENGL_AST_ISTOKCATEX10(nodenum,ZL_TKCG_OP_COMMA, \
													ZL_TKCG_OP_ASSIGN,\
													ZL_TKCG_OP_PLUS_MINIS,\
													ZL_TKCG_OP_TIM_DIV,\
													ZL_TKCG_OP_BITS, \
													ZL_TKCG_OP_RELATION,\
													ZL_TKCG_OP_LOGIC,\
													ZL_TKCG_OP_PP_MM,\
													ZL_TKCG_OP_DOT,\
													ZL_TKCG_OP_QUESTION)  //判断nodenum节点是否是表达式运算符，如加减乘除，比较运算，逻辑运算符与或非等。
#define ZENGL_AST_ISTOKEXPRESS(nodenum) (ZENGL_AST_ISTOKCATEXOP(nodenum) || ZENGL_AST_ISTOKTYPEXOR(nodenum,ZL_TK_FUNCALL,ZL_TK_ARRAY_ITEM)) //判断token是否是表达式

typedef enum _ZENGL_TOKEN_CATEGORY{  //token分类的枚举值，如加减运算符都是OP_PLUS_MINIS类型等等。OP_FACTOR为操作因子的token类型，指操作运算符的操作对象如变量，数字，字符串等。
	ZL_TKCG_OP_START,		//默认初始值
	ZL_TKCG_OP_PLUS_MINIS,	//加减运算操作类型
	ZL_TKCG_OP_ASSIGN,		//赋值操作类型
	ZL_TKCG_OP_TIM_DIV,		//乘除运算操作类型
	ZL_TKCG_OP_BITS,		//按位与，或，异或之类的位运算符
	ZL_TKCG_OP_RELATION,	//大于，等于之类的关系比较运算操作类型
	ZL_TKCG_OP_LOGIC,		//逻辑或，逻辑且之类的逻辑运算操作类型
	ZL_TKCG_OP_FACTOR,		//变量标识符，数字，字符串之类的操作因子类型
	ZL_TKCG_OP_COMMA,		//逗号操作类型
	ZL_TKCG_OP_QUESTION,	//问号操作类型
	ZL_TKCG_OP_PP_MM,		//加加减减操作类型
	ZL_TKCG_OP_DOT			//点运算操作类型
}ZENGL_TOKEN_CATEGORY;

typedef enum _ZENGL_OP_LEVEL{  //操作运算符的优先级枚举值，如赋值，逻辑，关系比较，加减，乘除，取反等的优先级。默认情况下枚举值越大的优先级也越大。
	ZL_OP_LEVEL_START,		//默认初始值
	ZL_OP_LEVEL_COMMA,		//逗号优先级
	ZL_OP_LEVEL_ASSIGN,		//赋值优先级，当赋值类型的符号在左侧时优先级为当前优先级，当在右侧时优先级又比除了点运算符以外的优先级都高。
	ZL_OP_LEVEL_QUESTION,	//问号优先级
	ZL_OP_LEVEL_LOGIC,		//逻辑运算优先级
	ZL_OP_LEVEL_RELATION,	//关系比较运算优先级
	ZL_OP_LEVEL_BITS,		//按位双目运算符优先级
	ZL_OP_LEVEL_PLUS_MINIS,	//加减运算优先级
	ZL_OP_LEVEL_TIM_DIV,	//乘除运算优先级
	ZL_OP_LEVEL_PP_MM,		//加加减减运算优先级
	ZL_OP_LEVEL_REVERSE,	//取反运算优先级
	ZL_OP_LEVEL_ASSIGN_WHEN_IN_RIGHT, //当赋值语句在其他运算符右侧时，比除了点运算符之外的优先级都高
	ZL_OP_LEVEL_DOT			//点操作优先级
}ZENGL_OP_LEVEL;

typedef enum _ZENGL_OP_POS_LEFT_OR_RIGHT{
	ZL_OP_POS_START_NONE, //默认初始值
	ZL_OP_POS_IN_LEFT,  //加加减减等运算符在目标左侧时
	ZL_OP_POS_IN_RIGHT, //加加减减等运算符在目标右侧时
}ZENGL_OP_POS_LEFT_OR_RIGHT;

typedef struct _ZENGL_AST_CHILD_NODE_TYPE{
	ZL_INT size;
	ZL_INT count;
	ZL_INT childnum[ZL_AST_CHILD_NODE_SIZE]; //指向一个整形数组，里面存放着子节点的节点索引。
	ZL_INT *extchilds; //超过3个的子节点都存放在extchilds扩展子节点中。
}ZENGL_AST_CHILD_NODE_TYPE;  //语法树里每个节点的子节点的类型定义。

typedef struct _ZENGL_AST_NODE_TYPE{
	ZL_BOOL isvalid; //节点是否有效，有效表示该节点存放了一个token信息，已被占用，不能再存信息。无效则表示该节点还没存放token信息，可以用来存放信息。
	ZENGL_TOKENTYPE toktype; //表示该节点存放了什么类型的token，如ZL_TK_ID表示标示符，ZL_TK_NUM表示数字等。
	ZENGL_OP_POS_LEFT_OR_RIGHT leftOrRight; //对于加加减减运算符，用于判断是在左侧，还是在右侧
	ZL_BOOL isvalid_qst_colon; //判断是否是有效的问号或冒号，即问号冒号是否一一匹配
	ZENGL_RESERVE_TYPE reserve;  //当节点是关键字时，通过reserve成员对应的枚举值判断是哪个关键字。
	ZENGL_TOKEN_CATEGORY tokcategory;  //token所属的分类，如加减运算符都属于ZL_OP_LEVEL_PLUS_MINIS类型
	ZENGL_OP_LEVEL tok_op_level;  //操作运算符优先级。
	ZL_INT strindex; //token字符串信息在Token_StringPool字符串池中的索引，当需要获取该token的字符串信息时，就根据索引去查找，这样只需要为所有的token分配一个大的内存来存放所有token的字符串信息，不需要为每个token都进行分配，减少开销。
	ZENGL_AST_CHILD_NODE_TYPE childs;
	ZL_INT parentnode; //该节点的父节点索引。
	ZL_INT nextnode;
	ZL_INT line_no; //行号
	ZL_INT col_no; //列号
	ZL_CHAR * filename; //当前token所在的文件名
}ZENGL_AST_NODE_TYPE;  //语法树里的每个节点的类型定义

typedef struct _ZENGL_AST_TYPE{
	ZL_BOOL isInit;
	ZL_INT size;
	ZL_INT count;
	ZENGL_AST_NODE_TYPE *nodes;
	ZL_INT rootnode;
}ZENGL_AST_TYPE;  //AST抽象语法树节点动态数组的类型定义

typedef struct _ZENGL_TOKEN_STRING_POOL{
	ZL_BOOL isInit;
	ZL_INT size;
	ZL_INT count;
	ZL_CHAR * ptr; //动态分配的字符串池缓存
}ZENGL_TOKEN_STRING_POOL; //将所有token的字符串信息都存放在该结构体定义的字符串池里，通过索引在字符串池中查找token的字符串信息。

typedef struct _ZENGL_PARSER_OP_LEVEL_STACK_TYPE{
	ZENGL_TOKEN_CATEGORY tokcategory;  //堆栈节点的token分类
	ZENGL_OP_LEVEL tok_op_level;  //堆栈节点的优先级
	ZL_INT nodenum; //节点号
}ZENGL_PARSER_OP_LEVEL_STACK_TYPE;  //堆栈里存放元素的类型定义，主要用于临时存放节点优先级用的。

typedef struct _ZENGL_PARSER_OP_LEVEL_STACKLIST_TYPE{
	ZL_BOOL isInit;
	ZL_INT size;  //动态数组的大小
	ZL_INT count;  //堆栈动态数组中存放的元素个数
	ZENGL_PARSER_OP_LEVEL_STACK_TYPE * stacks;  //指向动态数组的指针
}ZENGL_PARSER_OP_LEVEL_STACKLIST_TYPE;  //堆栈动态数组的类型定义。主要用于临时存放节点优先级用的。

typedef struct _ZENGL_PARSER_EXP_STRUCT{
	ZENGL_STATES state;
	ZL_INT p;
	ZL_INT tmpcount; //恢复优先级堆栈顶用的
	ZL_INT tmpnode;  //临时保存节点号信息
	ZENGL_PARSER_OP_LEVEL_STACK_TYPE retstack; //从优先级堆栈中弹出的元素
	ZL_INT express_count;  //express函数递归调用的深度，因为在express函数里可能会递归调用自己多次，在从顶层返回时需要对curnode进行特殊处理，所以需要express_count来记录递归深度。
}ZENGL_PARSER_EXP_STRUCT; //在zengl_express函数中会用到的一些变量，放到结构体中，方便在其他函数中访问

typedef struct _ZENGL_AST_SCAN_STACK_TYPE{
	ZL_INT nodenum;
	ZL_INT curchild;
	ZL_INT childcnt;
	ZL_INT next;
}ZENGL_AST_SCAN_STACK_TYPE; //新的AST语法树扫描中采用了堆栈的方法，这里定义的是AST节点扫描堆栈中每个元素的结构定义

typedef struct _ZENGL_AST_SCAN_STACKLIST_TYPE{
	ZL_BOOL isInit;
	ZL_INT size;
	ZL_INT count;
	ZENGL_AST_SCAN_STACK_TYPE * stacks;
}ZENGL_AST_SCAN_STACKLIST_TYPE; //AST语法树扫描中采用的堆栈动态数组的定义。

/********************************************************************************
		上面是和zengl_parser.c语法解析文件相关的结构体和枚举等定义
********************************************************************************/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************************
		下面是和虚拟机解释器相关的结构体和枚举等定义
********************************************************************************/

#define ZL_R_INST_LIST_SIZE 512 //汇编指令动态数组初始化和动态扩容的大小
#define ZL_R_VMEM_LIST_SIZE 512 //虚拟内存动态数组初始化和动态扩容的大小
#define ZL_R_VSTACK_LIST_SIZE 40 //虚拟堆栈动态数组初始化和动态扩容的大小。
#define ZL_R_MEM_BLOCK_SIZE 10	//数组之类的内存块初始化和动态扩容的大小
#define ZL_R_MEM_POOL_SIZE 40	//解释器内存池初始化和动态扩容的大小
#define ZL_R_MEM_FREE_POOL_SIZE 20 //解释器内存释放池初始化和动态扩容的大小
#define ZL_R_INST_DATA_STRING_POOL_SIZE 1024 //指令操作数字符串池初始化和动态扩容的大小
#define ZL_R_REGLIST_SIZE 8		//寄存器的数目包括默认初始值
#define ZL_R_MODULE_TABLE_SIZE 20 //模块动态数组初始化和动态扩容的大小
#define ZL_R_MOD_FUN_TABLE_SIZE 50 //模块函数动态数组初始化和动态扩容的大小
#define ZL_R_EXTRA_DATA_TABLE_SIZE 10 //用户额外数据动态数组初始化和动态扩容的大小
#define ZL_R_HASH_SIZE 211 //解释器相关的hash表数组的大小。
#define ZL_R_HASH_TOTAL_SIZE 633 //解释器目前有3个动态数组，所以一共633项
#define ZL_R_HASH_SHIFT 4 //解释器hash表索引时的计算因子
#define ZENGL_RUN_REG(regnum) run->reg_list[regnum] //寄存器宏
#define ZENGL_RUN_REGVAL(regnum) run->reg_list[regnum].val //寄存器里值的简写宏
#define ZL_R_REG_PC ZENGL_RUN_REGVAL(ZL_R_RT_PC).dword  //当前PC寄存器的值。
#define ZENGL_RUN_INST(index) run->inst_list.insts[index] //读取指令数组里的某条指令。
#define ZL_R_CUR_INST ZENGL_RUN_INST(ZL_R_REG_PC) //当前PC寄存器对应的指令。
#define ZENGL_RUN_VMEM_OP_GET(memtype,retval,srcdest,errorno) \
	switch(memtype) \
	{ \
	case ZL_R_DT_MEM: \
		retval = run->VMemListOps(VM_ARG,ZL_R_VMOPT_GETMEM,ZL_R_CUR_INST.srcdest.val.mem,retval); \
		break; \
	case ZL_R_DT_ARGMEM: \
		retval = run->VStackListOps(VM_ARG,ZL_R_VMOPT_GETMEM,ZENGL_RUN_REGVAL(ZL_R_RT_ARG).dword + ZL_R_CUR_INST.srcdest.val.mem,retval,ZL_TRUE); \
		break; \
	case ZL_R_DT_LOCMEM: \
		retval = run->VStackListOps(VM_ARG,ZL_R_VMOPT_GETMEM,ZENGL_RUN_REGVAL(ZL_R_RT_LOC).dword + ZL_R_CUR_INST.srcdest.val.mem,retval,ZL_TRUE); \
		break; \
	default: \
		run->exit(VM_ARG,errorno,ZL_R_REG_PC); \
	}
#define ZENGL_RUN_VMEM_OP(memtype,mem_op,arg_loc_op,argval,srcdest,errorno) \
	switch(memtype) \
	{ \
	case ZL_R_DT_MEM: \
		run->VMemListOps(VM_ARG,mem_op,ZL_R_CUR_INST.srcdest.val.mem,argval); \
		break; \
	case ZL_R_DT_ARGMEM: \
		run->VStackListOps(VM_ARG,arg_loc_op,ZENGL_RUN_REGVAL(ZL_R_RT_ARG).dword + ZL_R_CUR_INST.srcdest.val.mem,argval,ZL_TRUE); \
		break; \
	case ZL_R_DT_LOCMEM: \
		run->VStackListOps(VM_ARG,arg_loc_op,ZENGL_RUN_REGVAL(ZL_R_RT_LOC).dword + ZL_R_CUR_INST.srcdest.val.mem,argval,ZL_TRUE); \
		break; \
	default: \
		run->exit(VM_ARG,errorno,ZL_R_REG_PC); \
	}
#define ZENGL_RUN_VMEM_OP_GET_BY_RUNTYPE(runtype,retval,mem,errorno) \
	switch(runtype) \
	{ \
	case ZL_R_RDT_ADDR: \
		retval = run->VMemListOps(VM_ARG,ZL_R_VMOPT_GETMEM,mem,retval); \
		break; \
	case ZL_R_RDT_ADDR_LOC: \
		retval = run->VStackListOps(VM_ARG,ZL_R_VMOPT_GETMEM,mem,retval,ZL_TRUE); \
		break; \
	default: \
		run->exit(VM_ARG,errorno,ZL_R_REG_PC); \
	}

typedef enum _ZENGL_RUN_INST_TYPE{
	ZL_R_IT_NONE,			//默认初始值
	ZL_R_IT_MOV,			//MOV指令
	ZL_R_IT_PRINT,			//PRINT指令
	ZL_R_IT_PLUS,			//PLUS指令
	ZL_R_IT_MINIS,			//MINIS指令
	ZL_R_IT_MOD,			//MOD指令
	ZL_R_IT_TIMES,			//TIMES指令
	ZL_R_IT_DIVIDE,			//DIVIDE指令
	ZL_R_IT_PUSH,			//PUSH指令
	ZL_R_IT_POP,			//POP指令
	ZL_R_IT_JE,				//JE指令
	ZL_R_IT_JNE,			//JNE指令
	ZL_R_IT_JMP,			//JMP指令
	ZL_R_IT_GREAT,			//GREAT指令
	ZL_R_IT_LESS,			//LESS指令
	ZL_R_IT_EQUAL,			//EQUAL指令
	ZL_R_IT_NOT_EQ,			//NOT_EQ指令
	ZL_R_IT_GREAT_EQ,		//GREAT_EQ指令
	ZL_R_IT_LESS_EQ,		//LESS_EQ指令
	ZL_R_IT_AND,			//AND指令
	ZL_R_IT_OR,				//OR指令
	ZL_R_IT_REVERSE,		//REVERSE指令
	ZL_R_IT_ADDGET,			//ADDGET指令
	ZL_R_IT_MINIS_GET,		//MINIS_GET指令
	ZL_R_IT_GETADD,			//GETADD指令
	ZL_R_IT_GET_MINIS,		//GET_MINIS指令
	ZL_R_IT_ADDONE,			//ADDONE指令
	ZL_R_IT_MINIS_ONE,		//MINIS_ONE指令
	ZL_R_IT_FUNARG,			//FUNARG指令
	ZL_R_IT_ARG_SET,		//ARG_SET指令
	ZL_R_IT_PUSH_LOC,		//PUSH_LOC指令
	ZL_R_IT_RET,			//RET指令
	ZL_R_IT_RESET,			//RESET指令
	ZL_R_IT_CALL,			//CALL指令
	ZL_R_IT_USE,			//USE指令
	ZL_R_IT_ADDR,			//ADDR指令
	ZL_R_IT_SET_ARRAY,		//SET_ARRAY指令
	ZL_R_IT_GET_ARRAY_ADDR,	//GET_ARRAY_ADDR指令
	ZL_R_IT_ADDGET_ARRAY,	//ADDGET_ARRAY指令
	ZL_R_IT_MINISGET_ARRAY,	//MINISGET_ARRAY指令
	ZL_R_IT_GETADD_ARRAY,	//GETADD_ARRAY指令
	ZL_R_IT_GETMINIS_ARRAY,	//GETMINIS_ARRAY指令
	ZL_R_IT_GET_ARRAY,		//GET_ARRAY指令
	ZL_R_IT_SWITCH,			//SWITCH指令
	ZL_R_IT_LONG,			//LONG指令
	ZL_R_IT_BIT_AND,		//BIT_AND指令
	ZL_R_IT_BIT_OR,			//BIT_OR指令
	ZL_R_IT_BIT_XOR,		//BIT_XOR指令
	ZL_R_IT_BIT_RIGHT,		//BIT_RIGHT指令
	ZL_R_IT_BIT_LEFT,		//BIT_LEFT指令
	ZL_R_IT_BIT_REVERSE,	//BIT_REVERSE指令
	ZL_R_IT_END,			//END指令
}ZENGL_RUN_INST_TYPE; //指令类型

typedef enum _ZENGL_RUN_INST_OP_DATA_TYPE{
	ZL_R_DT_NONE,	//默认初始值
	ZL_R_DT_NUM,	//整数操作数
	ZL_R_DT_FLOAT,	//浮点数操作数
	ZL_R_DT_STR,	//字符串操作数
	ZL_R_DT_REG,	//REG寄存器操作数
	ZL_R_DT_MEM,	//全局变量内存地址操作数
	ZL_R_DT_ARGMEM,	//函数参数内存地址操作数
	ZL_R_DT_LOCMEM,	//函数中的局部变量内存地址操作数
	ZL_R_DT_LDADDR,	//供ld链接器使用的伪地址，链接器动态数组的伪地址
	ZL_R_DT_LDFUNID, //如果是函数调用，则存放的是函数的ID而非伪地址，在替换地址时，先函数ID得到伪地址，再由伪地址得到真实的汇编地址，这样函数就可以定义在脚本的任意合法位置，如果直接使用伪地址，那么函数就只能定义在函数调用之前
}ZENGL_RUN_INST_OP_DATA_TYPE; //指令操作数类型

typedef enum _ZENGL_RUN_REG_TYPE{
	ZL_R_RT_NONE,	//默认初始值
	ZL_R_RT_AX,		//AX寄存器
	ZL_R_RT_BX,		//BX寄存器
	ZL_R_RT_ARG,	//ARG寄存器
	ZL_R_RT_LOC,	//LOC寄存器
	ZL_R_RT_ARGTMP,	//ARGTMP寄存器
	ZL_R_RT_ARRAY_ITEM, //ARRAY_ITEM寄存器
	ZL_R_RT_PC,		//PC寄存器
}ZENGL_RUN_REG_TYPE; //寄存器类型

typedef enum _ZENGL_RUN_RUNTIME_OP_DATA_TYPE{
	ZL_R_RDT_NONE,			//默认初始值
	ZL_R_RDT_INT,			//整数
	ZL_R_RDT_FLOAT,			//浮点数
	ZL_R_RDT_STR,			//字符串
	ZL_R_RDT_ADDR,			//全局变量的引用
	ZL_R_RDT_ADDR_LOC,		//局部变量的引用
	ZL_R_RDT_ADDR_MEMBLK,	//内存块引用
	ZL_R_RDT_MEM_BLOCK,		//数组,类对应的内存块
}ZENGL_RUN_RUNTIME_OP_DATA_TYPE; //内存，寄存器里的值的枚举类型（运行时类型），整数，浮点，字符串，引用，数组内存块等。

typedef enum _ZENGL_RUN_VMEM_OP_TYPE{
	ZL_R_VMOPT_NONE,			//默认初始值
	ZL_R_VMOPT_ADDMEM_NONE,		//添加未初始化的虚拟内存
	ZL_R_VMOPT_ADDMEM_INT,		//虚拟内存添加整数
	ZL_R_VMOPT_ADDMEM_DOUBLE,	//虚拟内存添加浮点数
	ZL_R_VMOPT_ADDMEM_STR,		//虚拟内存添加字符串
	ZL_R_VMOPT_ADDMEM_ADDR,		//虚拟内存添加引用
	ZL_R_VMOPT_ADDMEM_MEMBLOCK,	//虚拟内存添加内存块
	ZL_R_VMOPT_GETMEM,			//获取虚拟内存的值
	ZL_R_VMOPT_SETMEM_NONE,		//设置虚拟内存为未初始化
	ZL_R_VMOPT_SETMEM_INT,		//设置虚拟内存为整数
	ZL_R_VMOPT_SETMEM_DOUBLE,	//设置虚拟内存为浮点数
	ZL_R_VMOPT_SETMEM_STR,		//设置虚拟内存为字符串
	ZL_R_VMOPT_SETMEM_ADDR,		//设置虚拟内存为引用
	ZL_R_VMOPT_SETMEM_MEMBLOCK,	//设置虚拟内存为内存块
}ZENGL_RUN_VMEM_OP_TYPE; //虚拟内存操作类型

typedef enum _ZENGL_RUN_HASH_TYPES{
	ZL_R_HASH_TYPE_MODULE_TABLE = 0,	//解释器模块动态数组所在hash表区域
	ZL_R_HASH_TYPE_MOD_FUN_TABLE = 1,	//解释器模块函数动态数组对应hash表区域
	ZL_R_HASH_TYPE_EXTRA_DATA_TABLE = 2,//用户为解释器提供的额外数据构成的动态数组所对应的hash表区域
}ZENGL_RUN_HASH_TYPES;

typedef struct _ZENGL_RUN_MEM_POOL_POINT_TYPE{
	ZL_BOOL isvalid;
	ZL_VOID * point;
	ZL_INT size;
}ZENGL_RUN_MEM_POOL_POINT_TYPE;

typedef struct _ZENGL_RUN_MEM_POOL_TYPE{
	ZL_BOOL isInit;
	ZL_INT size;
	ZL_INT totalsize; //内存池动态分配的总大小，包括内存池自身的大小
	ZL_INT count;
	ZENGL_RUN_MEM_POOL_POINT_TYPE * points;
}ZENGL_RUN_MEM_POOL_TYPE; //解释器的内存池类型定义，points成员指向一个指针数组，里面存放着解释器分配过的指针。

typedef struct _ZENGL_RUN_MEM_FREE_POOL_TYPE{
	ZL_INT size;
	ZL_INT count;
	ZL_INT mempool_index; //此为下面的frees指针在内存池中的索引
	ZL_INT * frees; //内存释放池的动态数组
}ZENGL_RUN_MEM_FREE_POOL_TYPE; //内存释放池的类型定义，存放了内存池中被释放掉的索引，方便重利用

typedef struct _ZENGL_RUN_INST_OP_DATA{
	ZENGL_RUN_INST_OP_DATA_TYPE type;
	union{
		ZENGL_RUN_REG_TYPE reg; //寄存器的枚举索引。
		ZL_INT str_Index; //字符串信息在解释器字符串池中的索引
		ZL_INT mem; //内存寻址值。
		ZL_LONG num;//使用长整数，方便64位移植
		ZL_DOUBLE floatnum; //浮点数
	}val;
}ZENGL_RUN_INST_OP_DATA; //解释器的指令操作数

typedef struct _ZENGL_RUN_INST_LIST_MEMBER{
	ZL_BOOL isvalid;
	ZL_INT pc;
	ZL_INT nodenum; //汇编指令对应的编译器中AST的节点号，用于代码调试
	ZENGL_RUN_INST_TYPE type;
	ZENGL_RUN_INST_OP_DATA src;
	ZENGL_RUN_INST_OP_DATA dest;
}ZENGL_RUN_INST_LIST_MEMBER; //解释器指令集中单个指令的类型定义

typedef struct _ZENGL_RUN_INST_LIST{
	ZL_BOOL isInit;
	ZL_INT size;
	ZL_INT count;
	ZL_INT mempool_index; //此字段为下面的insts指针在内存池中的索引。
	ZENGL_RUN_INST_LIST_MEMBER * insts;
}ZENGL_RUN_INST_LIST; //解释器的指令集

typedef struct _ZENGL_RUN_INFO_STRING_TYPE{
	ZL_CHAR * str; //信息的字符串指针
	ZL_INT mempool_index; //此为上面的str指针在内存池中的索引
	ZL_INT size;  //字符串的动态大小
	ZL_INT count; //存放的字符数
	ZL_INT cur;  //当前游标
}ZENGL_RUN_INFO_STRING_TYPE; //存放普通打印信息或错误信息的类型

typedef struct _ZENGL_RUN_RUNTIME_OP_DATA{
	ZENGL_RUN_RUNTIME_OP_DATA_TYPE runType; //操作数的运行时类型
	ZL_INT str_Index;	//str_Index是下面struct里的str指针在内存池中的索引，可以加快一些内存池的操作。
	ZL_INT memblk_Index;//memblk_Index是下面struct里的memblock在内存池中的索引。
	struct{
		ZL_INT dword;
		ZL_VOID * str;
		ZL_VOID * memblock;
		ZL_DOUBLE qword;
	}val;
}ZENGL_RUN_RUNTIME_OP_DATA; //zenglrun_main.c的zenglrun_RunInsts函数里用于存放源操作数的临时变量，以及寄存器里的值的结构定义

typedef struct _ZENGL_RUN_INST_DATA_STRING_POOL{
	ZL_BOOL isInit;
	ZL_INT size;
	ZL_INT count;
	ZL_INT mempool_index; //下面的ptr指针在内存池中的索引
	ZL_CHAR * ptr;
}ZENGL_RUN_INST_DATA_STRING_POOL; //指令操作数字符串池，汇编指令中的字符串常量将放置在字符串池中。

typedef struct _ZENGL_RUN_VIRTUAL_MEM_STRUCT{
	ZENGL_RUN_RUNTIME_OP_DATA_TYPE runType; //虚拟内存中操作数的运行时类型
	ZL_BOOL isvalid; //判断内存是否被占用
	ZL_INT str_Index;	//str_Index是下面struct里的str指针在内存池中的索引，可以加快一些内存池的操作。
	ZL_INT memblk_Index;//memblk_Index是下面struct里的memblock在内存池中的索引。
	struct{
		ZL_CHAR byte;
		ZL_WCHAR_T word;
		ZL_INT dword;
		ZL_VOID * str;
		ZL_VOID * memblock;
		ZL_DOUBLE qword;
	}val;
}ZENGL_RUN_VIRTUAL_MEM_STRUCT; //每个虚拟内存单元和虚拟堆栈单元的内存结构体类型定义。

typedef struct _ZENGL_RUN_VIRTUAL_MEM_LIST{
	ZL_BOOL isInit;
	ZL_INT size;
	ZL_INT count;
	ZL_INT refcount; //内存块的引用计数，主要用于数组之类的动态分配的内存块。
	ZL_INT mempool_index; //下面的mem_array指针在内存池中的索引
	ZENGL_RUN_VIRTUAL_MEM_STRUCT * mem_array;
}ZENGL_RUN_VIRTUAL_MEM_LIST;

typedef struct _ZENGL_RUN_MODULE_TABLE_MEMBER{
	ZL_BOOL isvalid;
	ZL_INT ID;
	ZL_INT strIndex; //模块名在字符串池中的索引
	ZL_VM_API_MOD_INIT_FUNC init_func; //用户自定义的模块初始化函数
	ZL_INT next;
}ZENGL_RUN_MODULE_TABLE_MEMBER;

typedef struct _ZENGL_RUN_MODULE_TABLE{
	ZL_BOOL isInit;
	ZL_INT size;
	ZL_INT count;
	ZL_INT mempool_index; //下面的modules指针在内存池中的索引
	ZENGL_RUN_MODULE_TABLE_MEMBER * modules;
}ZENGL_RUN_MODULE_TABLE; //模块动态数组定义

typedef struct _ZENGL_RUN_MOD_FUN_TABLE_MEMBER{
	ZL_BOOL isvalid;
	ZL_INT moduleID;
	ZL_INT strIndex; //模块函数名在字符串池中的索引
	ZL_VM_API_MOD_FUN_HANDLE handle; //用户自定义的模块函数处理句柄
	ZL_INT next;
}ZENGL_RUN_MOD_FUN_TABLE_MEMBER;

typedef struct _ZENGL_RUN_MOD_FUN_TABLE{
	ZL_BOOL isInit;
	ZL_INT size;
	ZL_INT count;
	ZL_INT mempool_index; //下面的mod_funs指针在内存池中的索引
	ZENGL_RUN_MOD_FUN_TABLE_MEMBER * mod_funs;
}ZENGL_RUN_MOD_FUN_TABLE; //模块函数动态数组的定义

typedef struct _ZENGL_RUN_EXTRA_DATA_TABLE_MEMBER{
	ZL_BOOL isvalid;
	ZL_INT strIndex; //额外数据名称在字符串池中的索引
	ZL_VOID * point; //额外数据指针
	ZL_INT next;
}ZENGL_RUN_EXTRA_DATA_TABLE_MEMBER; //用户额外数据动态数组中成员的定义

typedef struct _ZENGL_RUN_EXTRA_DATA_TABLE{
	ZL_BOOL isInit;
	ZL_INT size;
	ZL_INT count;
	ZL_INT mempool_index;
	ZENGL_RUN_EXTRA_DATA_TABLE_MEMBER * extras;
}ZENGL_RUN_EXTRA_DATA_TABLE; //用户额外数据动态数组的定义

/********************************************************************************
		上面是和虚拟机解释器相关的结构体和枚举等定义
********************************************************************************/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*zengl编译器结构体定义*/
typedef struct _ZENGL_COMPILE_TYPE
{
	/*编译器的成员列表*/
	ZENGL_TOKEN_INFO_TYPE tokenInfo; //存放当前扫描到的token信息
	ZENGL_SOURCE_TYPE source; //source成员的类型定义，里面包含要操作的文件的文件指针，文件名等信息。
	ZENGL_MEM_POOL_TYPE mempool; //编译器的内存池
	jmp_buf jumpBuffer; //出错时统一跳转的位置
	ZL_INT line_no; //扫描器扫描到的行号
	ZL_INT col_no;  //扫描器扫描到的列号
	ZL_BOOL is_inConvChr; //用于解析字符串中的'\'斜杠转义字符的
	ZL_BOOL is_inDefRsv;  //用于判断是否在def关键字后面，def后面的常量名不做DEF解析(因为解析后就不知道原常量名是什么了，产生的错误提示信息就会有误)，只对def后面的常量值做DEF解析
	ZL_INT basesize; //编译器结构体的基础大小
	ZL_INT totalsize; //编译器分配的内存池的大小和内存池中分配的内存的大小总和
	ZL_CLOCK_T start_time; //编译器开始执行时的时间，毫秒为单位
	ZL_CLOCK_T end_time; //编译器结束时的时间，毫秒为单位
	ZL_CLOCK_T total_time; //执行结束时的总时间，毫秒为单位
	ZL_CLOCK_T total_print_time; //编译器打印调试信息所消耗的时间，毫秒为单位
	ZENGL_INFO_STRING_TYPE infoFullString; //里面存放了完整的经过解析后的普通打印信息字符串
	ZENGL_INFO_STRING_TYPE errorFullString; //里面存放了完整的经过解析后的错误信息的字符串
	ZL_CONST ZL_CHAR ** reserveString; //各种关键字定义
	ZL_CONST ZL_CHAR ** TokenOperateString; //所有操作类型运算符的字符串信息
	ZL_INT TokenOperateStringCount; //TokenOperateString成员的个数
	ZENGL_STRING_POOL_TYPE def_StringPool; //def宏定义常量的字符串池
	ZENGL_DEF_TABLE def_table; //宏定义动态数组。
	ZL_BOOL isinCompiling; //判断编译器是否正在编译
	ZL_BOOL isDestroyed; //判断编译器的内存池等资源是否被释放了
	ZL_BOOL isReUse;	//用户是否需要重利用虚拟机之前已经编译好的资源，如果需要则不执行具体的编译操作，可以直接执行之前编译好的指令代码
	/*和zengl_symbol.c符号表相关的成员*/
	ZL_INT HashTable[ZL_SYM_HASH_TOTAL_SIZE]; //hash表中存放了各种动态数组元素的索引值。
	ZENGL_LINECOL_TABLE LineCols; //行列号表的动态数组。
	ZL_CHAR PathFileName[ZL_FILE_MAX_PATH];	//inc加载文件时，需要根据文件名生成绝对路径。
	ZENGL_FILE_STACKLIST_TYPE FileStackList; //按加载的先后顺序存放inc加载的文件信息的堆栈。
	ZENGL_SYM_GLOBAL_TABLE SymGlobalTable;	//全局变量符号表
	ZENGL_SYM_LOCAL_TABLE SymLocalTable;	//局部变量(包括函数参数)符号表
	ZENGL_SYM_CLASS_TABLE SymClassTable; //类符号表(存放类信息的动态数组)
	ZENGL_SYM_CLASSMEMBER_TABLE SymClassMemberTable; //类成员符号表(存放所有类成员信息的动态数组)
	ZENGL_SYM_FUN_TABLE SymFunTable; //函数符号表
	/*和zengl_assemble.c汇编代码生成相关的成员*/
	ZENGL_ASM_GENCODE_STRUCT gencode_struct; //在zengl_AsmGenCodes函数中会用到的一些变量，统一放在一个结构体中
	ZENGL_ASM_STACKLIST_TYPE AsmGCStackList; //assemble生成汇编代码时需要用到的解决内部嵌套问题的堆栈
	ZL_INT AsmGCAddrNum; //ifadr,ifend,foradr,forend等的计数器。
	ZL_BOOL AsmGCIsInClass; //判断是否在生成class类结构的汇编代码过程中。
	ZENGL_RUN_INST_OP_DATA memDataForDot; //用于生成点运算符的汇编指令
	/*和zengl_ld.c链接程序相关的成员*/
	ZENGL_LD_ADDRLIST_TYPE LDAddrList; //链接地址动态数组
	/*zengl_parser.c语法解析相关的成员*/
	ZENGL_AST_TYPE AST_nodes; //抽象语法树动态数组，里面存放了所有的语法树节点信息
	ZENGL_TOKEN_STRING_POOL Token_StringPool; //存放token字符串信息的字符串池
	ZL_INT parser_curnode; //语法树当前扫描游标
	ZENGL_PARSER_EXP_STRUCT exp_struct; //在zengl_express函数中会用到的一些变量，统一放在一个结构体中
	ZENGL_PARSER_OP_LEVEL_STACKLIST_TYPE parser_opLevel_stackList; //语法解析时需要用到的优先级堆栈
	ZENGL_AST_SCAN_STACKLIST_TYPE AST_TreeScanStackList; //语法树扫描堆栈

	/*编译器相关的自定义函数*/
	ZL_VOID (* exit)(ZL_VOID * ,ZENGL_ERRORNO , ...); //编译器退出函数，可以输出相关的出错信息 对应 zengl_exit
	ZL_VOID (* memFreeAll)(ZL_VOID * VM_ARG); //编译器释放内存池资源 对应 zengl_freeall
	ZL_CHAR (* getNextchar)(ZL_VOID *); //从文件中获取下一个字符的函数 对应 zengl_getNextchar
	ZENGL_TOKENTYPE (* getToken)(ZL_VOID *); //获取token信息的函数 对应 zengl_getToken
	ZL_VOID (* makeTokenInfoLineCol)(ZL_VOID * VM_ARG,ZL_INT line,ZL_INT col,ZL_CHAR * filename); //生成token的行列号，文件名信息 对应 zengl_makeTokenInfoLineCol
	ZL_VOID (* makeTokenStr)(ZL_VOID * VM_ARG,ZL_CHAR ch); //生成token的字符串信息 对应 zengl_makeTokenStr
	ZL_VOID * (* memAlloc)(ZL_VOID * VM_ARG , ZL_INT size); //编译器在内存池中分配内存函数 对应 zengl_malloc
	ZL_VOID * (* memReAlloc)(ZL_VOID * VM_ARG , ZL_VOID * point , ZL_INT size); //调整内存池中某个指针的内存大小 对应 zengl_realloc
	ZL_VOID (* memFreeOnce)(ZL_VOID * VM_ARG,ZL_VOID * point); //释放编译器内存池中的某个指针 对应 zengl_freeonce
	ZL_VOID (* ungetchar)(ZL_VOID * VM_ARG); //将源脚本字符扫描游标回退一格，同时将列号减一 对应 zengl_ungetchar
	ZL_VOID (* convertStrToDec)(ZL_VOID * VM_ARG); //将字符串转为有效的十进制数 对应 zengl_convertStrToDec
	ZL_VOID (* freeTokenStr)(ZL_VOID * VM_ARG); //修改游标等成员来释放当前扫描token的动态字符串信息，以供下一次扫描token时使用 对应 zengl_freeTokenStr
	ZL_VOID (* convertOctalToDec)(ZL_VOID * VM_ARG); //将当前扫描token由八进制转为十进制 对应 zengl_convertOctalToDec
	ZL_VOID (* convertHexToDec)(ZL_VOID * VM_ARG); //将扫描token由十六进制格式转为十进制格式 对应 zengl_convertHexToDec
	ZL_VOID (* makeTokenStrForString)(ZL_VOID * VM_ARG,ZL_CHAR ch); //用于专门解析字符串类型的token 对应 zengl_makeTokenStrForString
	ZENGL_TOKENTYPE (* lookupReserve)(ZL_VOID * VM_ARG , ZENGL_TOKENTYPE token); //查找关键字的类型 对应 zengl_lookupReserve
	ZL_VOID (* AddDefConst)(ZL_VOID * VM_ARG); //将宏和宏对应的值加入到哈希表和def宏定义的动态数组中 对应 zengl_AddDefConst
	ZL_INT (* DefPoolAddString)(ZL_VOID * VM_ARG , ZL_CHAR * src); //将参数src添加到def_StringPool字符串池中 对应 zengl_DefPoolAddString
	ZL_VOID (* initDefStringPool)(ZL_VOID * VM_ARG); //初始化def常量宏字符串池 对应 zengl_initDefStringPool
	ZL_VOID (* insert_HashTableForDef)(ZL_VOID * VM_ARG , ZL_INT nameIndex, ZENGL_TOKENTYPE tokentype,ZL_INT valIndex,
								ZL_INT line,ZL_INT col); //将def定义的宏名和宏对应的值插入到动态数组中，同时将索引加入到哈希表中 对应 zengl_insert_HashTableForDef
	ZL_CHAR * (* DefPoolGetPtr)(ZL_VOID * VM_ARG , ZL_INT index); //根据索引值从字符串池中查找宏定义字符串的指针信息 对应 zengl_DefPoolGetPtr
	ZL_INT (* insert_DefTable)(ZL_VOID * VM_ARG,ZL_INT nameIndex,ZENGL_TOKENTYPE tokentype,ZL_INT valIndex,ZL_INT line,ZL_INT col); //将每条def定义的宏名称和宏对应的值加入到def_table动态数组中 对应 zengl_insert_DefTable
	ZL_VOID (* initDefTable)(ZL_VOID * VM_ARG); //初始化def_table（def宏定义的动态数组） 对应 zengl_initDefTable
	ZL_VOID (* include_file)(ZL_VOID * VM_ARG); //当词法扫描器扫描到inc关键字时，就会调用该函数来加载文件。对应 zengl_include_file
	ZL_CHAR * (* makePathFileName)(ZL_VOID * VM_ARG,ZL_CHAR * filename); //inc加载文件时，得到文件的完整路径信息 对应 zengl_makePathFileName
	ZL_VOID (* push_FileStack)(ZL_VOID * VM_ARG , ZENGL_SOURCE_TYPE * src,ZL_INT line,ZL_INT col); //将原来的文件信息压入栈。对应 zengl_push_FileStack
	ZL_VOID (* pop_FileStack)(ZL_VOID * VM_ARG , ZENGL_SOURCE_TYPE * dest); //将上次压入栈的文件信息弹出，从弹出的文件信息里恢复原文件的扫描 对应 zengl_pop_FileStack
	ZL_VOID (* initFileStack)(ZL_VOID * VM_ARG); //初始化inc加载文件的堆栈 对应 zengl_initFileStack
	ZENGL_TOKENTYPE (* ReplaceDefConst)(ZL_VOID * VM_ARG, ZENGL_TOKENTYPE token); //常量宏替换函数 对应 zengl_ReplaceDefConst
	ZL_INT (* lookupDefTable)(ZL_VOID * VM_ARG, ZL_CHAR * name); //根据参数name在哈希表中查找索引 对应 zengl_lookupDefTable
	ZL_CHAR * (* makeInfoString)(ZL_VOID * VM_ARG,ZENGL_INFO_STRING_TYPE * infoStringPtr , ZL_CONST ZL_CHAR * format , ZENGL_SYS_ARG_LIST arglist); //生成格式化信息字符串 对应 zengl_makeInfoString
	ZL_VOID (* freeInfoString)(ZL_VOID * VM_ARG,ZENGL_INFO_STRING_TYPE * infoStringPtr); //释放格式化信息字符串的游标和大小信息 对应 zengl_freeInfoString
	ZL_VOID (* info)(ZL_VOID * VM_ARG , ZL_CONST ZL_CHAR * format , ...); //先生成格式化信息字符串，再调用用户自定义的函数，从而将字符串传递给用户函数 对应 zengl_info
	ZL_VOID (* init)(ZL_VOID * VM_ARG,ZL_CHAR * script_file,ZENGL_EXPORT_VM_MAIN_ARGS * vm_main_args); //编译器初始化 对应 zengl_init
	ZL_INT (* main)(ZL_VOID * VM_ARG,ZL_CHAR * script_file,ZENGL_EXPORT_VM_MAIN_ARGS * vm_main_args); //编译器的入口函数 对应 zengl_main
	/*定义在zengl_symbol.c中的相关函数*/
	ZL_INT (* hash) (ZL_VOID * VM_ARG , ZL_CHAR * key ); //hash哈希散列算法 对应 zengl_Sym_Hash
	ZL_INT (* insert_LineCol)(ZL_VOID * VM_ARG , ZL_INT line , ZL_INT col,ZL_CHAR * filename); //将line,col行列号，以及行列号所在的文件名插入到LineCols行列号动态数组中 对应 zengl_insert_LineCol
	ZL_VOID (* initLineColTable)(ZL_VOID * VM_ARG); //初始化LineCols行列号动态数组，用于记录符号表中每个全局符号出现过的行列号信息。对应 zengl_initLineColTable
	ZL_VOID (* buildSymTable)(ZL_VOID * VM_ARG); //组建符号表的主程式。对应 zengl_buildSymTable
	ZL_VOID (* ScanInsertGlobalSym)(ZL_VOID * VM_ARG,ZL_INT arg_nodenum); //扫描并插入全局变量符号表 对应 zengl_ScanInsertGlobalSym
	ZL_VOID (* SymInsertDotID)(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT parentNum); //判断点运算符里的节点是成员还是类还是全局变量，并将全局变量插入全局符号表中。对应 zengl_SymInsertDotID
	ZL_INT (* SymInsertHashTableForGlobal)(ZL_VOID * VM_ARG,ZL_INT nodenum); //将全局变量加入到哈希表和全局符号表动态数组中。对应 zengl_SymInsertHashTableForGlobal
	ZL_INT (* SymInsertGlobalTable)(ZL_VOID * VM_ARG,ZL_INT nameIndex,ZL_INT line,ZL_INT col,ZL_CHAR * filename); //将某个变量标示符插入到全局变量符号表里，并确定该变量对应的内存地址 对应 zengl_SymInsertGlobalTable
	ZL_VOID (* SymInitGlobalTable)(ZL_VOID * VM_ARG); //初始化全局变量符号表 对应 zengl_SymInitGlobalTable
	ZL_INT (* SymInsertHashTableForClass)(ZL_VOID * VM_ARG,ZL_INT nodenum); //将nodenum类名所在的节点加入到哈希表和类符号表所在的动态数组中 对应 zengl_SymInsertHashTableForClass
	ZL_INT (* SymInsertClassTable)(ZL_VOID * VM_ARG,ZL_INT nameIndex,ZL_INT nodenum,ZL_INT line,ZL_INT col,ZL_CHAR * filename); //将类名和该类定义所在的行列号,文件名,节点号信息插入到类符号表对应的动态数组中 对应 zengl_SymInsertClassTable
	ZL_VOID (* SymInitClassTable)(ZL_VOID * VM_ARG);//初始化类符号表对应的动态数组 对应 zengl_SymInitClassTable
	ZL_INT (* SymInsertHashTableForFun)(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT classid); //将函数节点信息插入到函数表动态数组中，并将动态数组的索引保存到HashTable中 对应 zengl_SymInsertHashTableForFun
	ZL_INT (* SymInsertFunTable)(ZL_VOID * VM_ARG,ZL_INT nameIndex,ZL_INT classid,ZL_INT line,ZL_INT col,ZL_CHAR * filename); //将函数名和函数所在的行列号等信息插入到函数符号表对应的动态数组中。对应 zengl_SymInsertFunTable
	ZL_VOID (* SymInitFunTable)(ZL_VOID * VM_ARG);	//初始化函数符号表对应的动态数组 对应 zengl_SymInitFunTable
	ZL_VOID (* ScanInsertClassMemberSym)(ZL_VOID * VM_ARG,ZL_INT arg_nodenum,ZL_INT classid); //扫描类成员，并将类成员加入类成员符号表，以及将类函数加入函数符号表中。对应 zengl_ScanInsertClassMemberSym
	ZL_INT (* SymLookupClass)(ZL_VOID * VM_ARG,ZL_INT nodenum); //根据节点号查找类ID信息 对应 zengl_SymLookupClass
	ZL_INT (* SymLookupClassByName)(ZL_VOID * VM_ARG,ZL_CHAR * name); //根据类名查找类ID信息 对应 zengl_SymLookupClassByName
	ZL_INT (* SymLookupClassMember)(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT parent_classid); //从SymClassMemberTable中查找parent_classid对应的类的成员nodenum的信息 对应 zengl_SymLookupClassMember
	ZL_INT (* SymInsertHashTableForClassMember)(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT index,ZL_INT classid,ZL_INT cls_stmt_classid); //将nodenum类成员所在的节点加入到哈希表和类成员符号表所在的动态数组中 对应 zengl_SymInsertHashTableForClassMember
	ZL_INT (* SymInsertClassMemberTable)(ZL_VOID * VM_ARG,ZL_INT nameIndex,ZL_INT index,ZL_INT classid,ZL_INT cls_stmt_classid,ZL_INT nodenum); //将类成员名，类成员在类结构中的索引值，所属类ID等信息加入类成员符号表中 对应 zengl_SymInsertClassMemberTable
	ZL_VOID (* SymInitClassMemberTable)(ZL_VOID * VM_ARG); //初始化类成员符号表对应的动态数组 对应 zengl_SymInitClassMemberTable
	ZENGL_RUN_INST_OP_DATA (* SymLookupID)(ZL_VOID * VM_ARG,ZL_INT nodenum); //通过节点索引来查找该节点的变量标示符在自定义的虚拟内存中的内存地址 对应 zengl_SymLookupID
	ZL_VOID (* SymInsertClassID)(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT classid); //为nodenum节点设置classid类id 对应 zengl_SymInsertClassID
	ZL_INT (* SymLookupID_ForDot)(ZL_VOID * VM_ARG,ZL_INT nodenum); //查找nodenum对应节点的classid值，主要用于生成点运算符的汇编指令时 对应 zengl_SymLookupID_ForDot
	ZL_INT (* SymLookupID_ByName)(ZL_VOID * VM_ARG,ZL_CHAR * name); //通过变量名称查找变量标示符在自定义的虚拟内存中的内存地址，主要用于对外的API接口中 对应 zengl_SymLookupID_ByName
	ZL_INT (* SymLookupFun)(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT classid);	//根据节点号查找函数ID信息 对应 zengl_SymLookupFun
	ZL_INT (* SymLookupFunByName)(ZL_VOID * VM_ARG,ZL_CHAR * name,ZL_INT classid); //根据函数名称查找函数ID信息 对应 zengl_SymLookupFunByName
	ZL_VOID (* SymScanFunArg)(ZL_VOID * VM_ARG,ZL_INT nodenum); //使用AST扫描堆栈来扫描语法树中函数的参数 对应 zengl_SymScanFunArg
	ZL_VOID (* SymScanFunGlobal)(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT global_nodenum); //使用AST扫描堆栈来扫描语法树中函数的global关键字声明的全局变量 对应 zengl_SymScanFunGlobal
	ZL_VOID (* SymScanClassStatement)(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT cls_statement_nodenum); //扫描类声明语句里声明的变量 对应 zengl_SymScanClassStatement
	ZL_VOID (* SymScanDotForClass)(ZL_VOID * VM_ARG,ZL_INT nodenum); //扫描类引用时的节点，将类成员转为数组的索引压入栈中 对应 zengl_SymScanDotForClass
	ZL_VOID (* SymScanUseRsv)(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT use_nodenum); //使用AST扫描堆栈来扫描语法树中use关键字声明的模块名 对应 zengl_SymScanUseRsv
	ZL_VOID (* SymScanFunLocal)(ZL_VOID * VM_ARG,ZL_INT nodenum); //使用AST扫描堆栈来扫描语法树中函数的局部变量 对应 zengl_SymScanFunLocal
	ZL_BOOL (* SymInsertHashTableForLocal)(ZL_VOID * VM_ARG,ZL_INT nodenum,ZENGL_SYM_ENUM_LOCAL_TYPE type); //将局部变量和参数插入到局部变量符号表中，并将符号表动态数组的索引加入到哈希表中 对应 zengl_SymInsertHashTableForLocal
	ZL_INT (* SymInsertLocalTable)(ZL_VOID * VM_ARG,ZL_INT nameIndex,ZENGL_SYM_ENUM_LOCAL_TYPE type); //将局部变量名和局部变量的类型插入到SymLocalTable动态数组中。对应 zengl_SymInsertLocalTable
	ZL_VOID (* SymInitLocalTable)(ZL_VOID * VM_ARG); //初始化SymLocalTable局部符号表对应的动态数组 对应 zengl_SymInitLocalTable
	ZL_VOID (* SymPrintTables)(ZL_VOID * VM_ARG);	  //打印符号表 对应 zengl_SymPrintTables
	/*定义在zengl_assemble.c中的相关函数*/
	ZL_VOID (* buildAsmCode)(ZL_VOID * VM_ARG); //组建汇编代码的主程式 对应 zengl_buildAsmCode
	ZL_VOID (* AsmGenCodes)(ZL_VOID * VM_ARG,ZL_INT nodenum); //该函数根据AST抽象语法树的节点索引将某节点转为汇编代码。对应 zengl_AsmGenCodes
	ZL_INT (* AsmGCStackPush)(ZL_VOID * VM_ARG,ZL_INT num,ZENGL_ASM_STACK_ENUM type); //将数字压入汇编堆栈 对应 zengl_AsmGCStackPush
	ZL_INT (* AsmGCStackPop)(ZL_VOID * VM_ARG,ZENGL_ASM_STACK_ENUM type,ZL_BOOL isremove); //弹出汇编栈 对应 zengl_AsmGCStackPop
	ZL_VOID (* AsmGCStackInit)(ZL_VOID * VM_ARG); //汇编堆栈初始化 对应 zengl_AsmGCStackInit
	ZL_VOID (* AsmGCElif)(ZL_VOID * VM_ARG,ZENGL_AST_CHILD_NODE_TYPE * ifchnum,ZL_INT num); //zengl_AsmGCElif函数用于生成elif代码块对应的汇编指令 对应 zengl_AsmGCElif
	ZL_VOID (* AsmGCBreak_Codes)(ZL_VOID * VM_ARG,ZL_INT nodenum); //break语句的汇编代码生成 对应 zengl_AsmGCBreak_Codes
	ZL_VOID (* AsmGCContinue_Codes)(ZL_VOID * VM_ARG,ZL_INT nodenum); //continue语句的汇编代码生成 对应 zengl_AsmGCContinue_Codes
	ZL_VOID (* AsmScanCaseMinMax)(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_BOOL * hasminmax,ZL_INT * minarg,ZL_INT * maxarg,ZL_BOOL * hasdefault,
								ZENGL_ASM_CASE_JMP_TABLE * table); //扫描switch...case ，找出其中的case的最大值，最小值，以及判断是否有default默认节点 对应 zengl_AsmScanCaseMinMax
	ZL_INT (* GetNodeInt)(ZL_VOID * VM_ARG,ZL_INT nodenum); //返回节点的字符串信息的整数形式 对应 zengl_GetNodeInt
	ZL_VOID (* AsmAddCaseJmpTable)(ZL_VOID * VM_ARG,ZENGL_ASM_CASE_JMP_TABLE * table,ZL_INT num); //将case对应的比较数字添加到跳转表中 对应 zengl_AsmAddCaseJmpTable
	ZL_VOID (* AsmInitCaseJmpTable)(ZL_VOID * VM_ARG,ZENGL_ASM_CASE_JMP_TABLE * table); //初始化switch case的跳转表 对应 zengl_AsmInitCaseJmpTable
	ZL_VOID (* AsmSortCaseJmpTable)(ZL_VOID * VM_ARG,ZENGL_ASM_CASE_JMP_TABLE * table,ZL_INT nodenum); //将switch...case跳转表进行从小到大的排序 对应 zengl_AsmSortCaseJmpTable
	/*定义在zengl_ld.c中的相关函数*/
	ZL_INT (* LDAddrListSet)(ZL_VOID * VM_ARG,ZL_INT index,ZL_INT loc); //设置链接动态数组中某索引(伪地址)对应的实际汇编代码位置 对应 zengl_LDAddrListSet
	ZL_VOID (* LDAddrListInit)(ZL_VOID * VM_ARG); //链接动态数组初始化 对应 zengl_LDAddrListInit
	ZL_VOID (* LDAddrListReplaceAll)(ZL_VOID * VM_ARG); //将所有的伪地址替换为真实的汇编代码位置，从而完成链接工作 对应 zengl_LDAddrListReplaceAll
	/*定义在zengl_parser.c中的相关函数*/
	ZL_VOID (* ASTAddNode)(ZL_VOID * VM_ARG,ZENGL_TOKENTYPE token); //将token加入AST抽象语法树 对应 zengl_ASTAddNode
	ZL_VOID (* initAST)(ZL_VOID * VM_ARG); //初始化AST抽象语法树的动态数组。对应 zengl_initAST
	ZL_INT (* TokenStringPoolAddStr)(ZL_VOID * VM_ARG , ZL_CHAR * src); //向token字符串池动态缓存添加字符串，并返回该字符串在缓存中的索引信息 对应 zengl_TokenStringPoolAddStr
	ZL_VOID (* initTokenStringPool)(ZL_VOID * VM_ARG); //初始化token字符串池的动态缓存 对应 zengl_initTokenStringPool
	ZL_VOID (* printNode)(ZL_VOID * VM_ARG , ZL_INT nodenum); //打印token的信息 对应 zengl_printNode
	ZL_CHAR * (* getTokenStr)(ZL_VOID * VM_ARG , ZENGL_AST_NODE_TYPE * nodes,ZL_INT nodenum); //根据节点号获取token对应的字符串信息 对应 zengl_getTokenStr
	ZL_CHAR * (* TokenStringPoolGetPtr)(ZL_VOID * VM_ARG , ZL_INT index); //根据索引index从token字符串池缓存中得到对应的字符串信息 对应 zengl_TokenStringPoolGetPtr
	ZL_VOID (* printASTnodes)(ZL_VOID * VM_ARG); //打印出AST抽象语法树的所有节点信息。采用了新的AST扫描堆栈的方法进行遍历扫描。对应 zengl_printASTnodes
	ZL_VOID (* push_AST_TreeScanStack)(ZL_VOID * VM_ARG , ZL_INT nodenum); //AST扫描堆栈的压栈函数 对应 zengl_push_AST_TreeScanStack
	ZENGL_AST_SCAN_STACK_TYPE (* pop_AST_TreeScanStack)(ZL_VOID * VM_ARG,ZL_BOOL remove); //AST扫描堆栈的弹出栈函数 对应 zengl_pop_AST_TreeScanStack
	ZL_VOID (* init_AST_TreeScanStack)(ZL_VOID * VM_ARG); //初始化AST扫描堆栈AST_TreeScanStackList 对应 zengl_init_AST_TreeScanStack
	ZL_VOID (* buildAST)(ZL_VOID * VM_ARG); //该函数通过循环来构建所有语句AST 对应 zengl_buildAST
	ZL_INT (* statement)(ZL_VOID * VM_ARG); //生成语句比如print 'hello'之类的语句的AST语法树 对应 zengl_statement
	ZL_VOID (* KeywordCompleteDetect)(ZL_VOID * VM_ARG); //关键词完整性检查，例如if结构必须有endif相匹配等 对应 zengl_KeywordCompleteDetect
	ZL_VOID (* TokenSyntaxDetect)(ZL_VOID * VM_ARG);	//use，global，break，continue，类声明等语句的语法检查 对应 zengl_TokenSyntaxDetect
	ZL_VOID (* if_stmt)(ZL_VOID * VM_ARG,ZL_INT p); //if-elif-else控制语句的AST抽象语法树的生成函数。对应 zengl_if_stmt
	ZL_INT (* if_stmt_sequence)(ZL_VOID * VM_ARG); //if-elif-else-endif语句代码块中所有表达式的AST抽象语法树的生成 对应 zengl_if_stmt_sequence
	ZL_VOID (* for_stmt)(ZL_VOID * VM_ARG,ZL_INT p); //for...endfor控制语句的AST抽象语法树的生成函数。对应 zengl_for_stmt
	ZL_INT (* for_stmt_sequence)(ZL_VOID * VM_ARG); //for...endfor语句代码块中所有表达式的AST抽象语法树的生成 对应 zengl_for_stmt_sequence
	ZL_VOID (* fun_stmt)(ZL_VOID * VM_ARG,ZL_INT p); //fun...endfun的AST抽象语法树的生成函数。对应 zengl_fun_stmt
	ZL_INT (* fun_stmt_sequence)(ZL_VOID * VM_ARG); //fun...endfun语句代码块中所有表达式的AST抽象语法树的生成 对应 zengl_fun_stmt_sequence
	ZL_VOID (* switch_stmt)(ZL_VOID * VM_ARG,ZL_INT p); //switch...case结构的AST生成。对应 zengl_switch_stmt
	ZL_INT (* switch_stmt_sequence)(ZL_VOID * VM_ARG); //switch...case语句代码块中所有表达式的AST抽象语法树的生成 对应 zengl_switch_stmt_sequence
	ZL_VOID (* class_stmt)(ZL_VOID * VM_ARG,ZL_INT p); //class...endcls或endclass结构的AST生成。对应 zengl_class_stmt
	ZL_INT (* class_stmt_sequence)(ZL_VOID * VM_ARG); //class...endcls或endclass语句代码块中所有表达式的AST抽象语法树的生成 对应 zengl_class_stmt_sequence
	ZL_VOID (* while_stmt)(ZL_VOID * VM_ARG,ZL_INT p); //while...endwhile结构的AST生成。对应 zengl_while_stmt
	ZL_INT (* while_stmt_sequence)(ZL_VOID * VM_ARG); //while...endwhile语句代码块中所有表达式的AST抽象语法树的生成 对应 zengl_while_stmt_sequence
	ZL_VOID (* do_stmt)(ZL_VOID * VM_ARG,ZL_INT p);	//do...dowhile结构的AST生成。对应 zengl_do_stmt
	ZL_INT (* do_stmt_sequence)(ZL_VOID * VM_ARG);	//do...dowhile语句代码块中所有表达式的AST抽象语法树的生成 对应 zengl_do_stmt_sequence
	ZL_INT (* detectSetRBracketToSemi)(ZL_VOID * VM_ARG); //将if之类的关键字右侧的右括号临时转为分号，这样就可以得到if括号中的条件判断表达式的AST 对应 zengl_detectSetRBracketToSemi
	ZL_INT (* express)(ZL_VOID * VM_ARG); //第三个版本的语法分析函数，采用堆栈和优先级来生成语法树。对应 zengl_express
	ZL_VOID (* parser_addcurnode)(ZL_VOID * VM_ARG); //增加parser_curnode当前语法分析节点的节点号 对应 zengl_parser_addcurnode
	ZL_VOID (* parser_errorExit)(ZL_VOID * VM_ARG,ZENGL_ERRORNO errorno, ...); //语法分析时调用的打印输出错误信息的函数 对应 zengl_parser_errorExit
	ZL_VOID (* opLevel_push_stack)(ZL_VOID * VM_ARG,ZL_INT nodenum); //将某节点压入栈，当需要比较优先级时，再弹出栈 对应 zengl_opLevel_push_stack
	ZENGL_PARSER_OP_LEVEL_STACK_TYPE (* opLevel_pop_stack)(ZL_VOID * VM_ARG,ZL_INT index); //将最后压入栈的节点弹出。对应 zengl_opLevel_pop_stack
	ZL_VOID (* initOpLevelStackList)(ZL_VOID * VM_ARG); //初始化优先级堆栈动态数组 对应 zengl_initOpLevelStackList
	ZL_VOID (* detectCurnodeSyntax)(ZL_VOID * VM_ARG); //根据当前curnode节点和curnode+1节点来初步判断是否有语法错误 对应 zengl_detectCurnodeSyntax
	ZL_VOID (* OpLevelForTwo)(ZL_VOID * VM_ARG); //使用优先级堆栈处理加减乘除等双目运算符 对应 zengl_OpLevelForTwo
	ZL_VOID (* OpLevelForAssign)(ZL_VOID * VM_ARG); //使用优先级堆栈处理赋值语句的双目运算符 对应 zengl_OpLevelForAssign
	ZL_VOID (* OpLevelForSEMI)(ZL_VOID * VM_ARG); //使用优先级堆栈处理分号结束符 对应 zengl_OpLevelForSEMI
	ZL_VOID (* OpLevelForPPMM)(ZL_VOID * VM_ARG); //使用优先级堆栈处理加加减减运算符 对应 zengl_OpLevelForPPMM
	ZL_VOID (* PPMM_GET_POS)(ZL_VOID * VM_ARG); //获取加加减减运算符的左右侧的位置标识 对应 zengl_PPMM_GET_POS
	ZL_VOID (* OpLevelForRBRACKET)(ZL_VOID * VM_ARG); //使用优先级堆栈处理右括号 对应 zengl_OpLevelForRBRACKET
	ZL_VOID (* OpLevelForRMBRACKET)(ZL_VOID * VM_ARG); //使用优先级堆栈处理右中括号 对应 zengl_OpLevelForRMBRACKET
	ZL_VOID (* OpLevelForQuestion)(ZL_VOID * VM_ARG); //使用优先级堆栈处理问号运算符 对应 zengl_OpLevelForQuestion
	ZL_VOID (* OpLevelForColon)(ZL_VOID * VM_ARG); //使用优先级堆栈处理冒号运算符 对应 zengl_OpLevelForColon
	ZL_VOID (* CheckQstColonValid)(ZL_VOID * VM_ARG); //检测问号冒号是否一一匹配 对应 zengl_CheckQstColonValid
	ZL_VOID (* ASTAddNodeChild)(ZL_VOID * VM_ARG,ZL_INT parent,ZL_INT child); //将child对应的节点加入到parent节点的子节点中 对应 zengl_ASTAddNodeChild
	ZL_BOOL (* CheckIsNegative)(ZL_VOID * VM_ARG); //初步判断当前的减号是否是负号 对应 zengl_CheckIsNegative
	ZL_BOOL (* CheckIsBitAnd)(ZL_VOID * VM_ARG); //初步判断当前的"&"符号是否是按位与运算符 对应 zengl_CheckIsBitAnd

	/*下面是用户自定义的函数*/
	ZL_INT (* userdef_info)(ZL_CHAR * infoStrPtr, ZL_INT infoStrCount, ZL_VOID * VM_ARG); //用户自定义的显示普通信息的函数，用户可以自定义信息的打印和输出方式。
	ZL_INT (* userdef_compile_error)(ZL_CHAR * infoStrPtr, ZL_INT infoStrCount, ZL_VOID * VM_ARG); //用户自定义的显示错误信息的函数。
}ZENGL_COMPILE_TYPE;
/*zengl编译器结构体定义结束*/

/*虚拟机的解释器结构体定义*/
typedef struct _ZENGL_RUN_TYPE
{
	/*解释器的成员列表*/
	ZL_INT basesize; //解释器结构体的基础大小
	ZL_INT totalsize; //当前解释器的内存池大小和内存池分配的内存大小的总和
	ZL_CLOCK_T start_time; //解释器开始执行时的时间，毫秒为单位
	ZL_CLOCK_T end_time; //解释器结束时的时间，毫秒为单位
	ZL_CLOCK_T total_time; //执行结束时的总时间，毫秒为单位
	ZL_BOOL isinRunning; //判断解释器是否正在运行中
	ZL_BOOL isDestroyed; //判断内存池资源是否被释放掉了
	ZL_BOOL isUserWantStop; //用户是否需要中途停止脚本
	ZL_BOOL is_inMemBlkSetVal; //判断是否处于对数组元素进行设置的状态，此状态下即使是引用也直接读取出来，例如test[a] = &b;就会将b的引用读取并设置到test[a]中。
	jmp_buf jumpBuffer; //解释器出错时统一跳转的位置
	ZENGL_RUN_MEM_POOL_TYPE mempool; //解释器的内存池
	ZENGL_RUN_MEM_FREE_POOL_TYPE memfreepool; //解释器的内存释放池
	ZENGL_RUN_INST_LIST inst_list; //汇编指令动态数组
	ZENGL_RUN_VIRTUAL_MEM_LIST vmem_list; //虚拟全局内存动态数组
	ZENGL_RUN_VIRTUAL_MEM_LIST vstack_list; //虚拟栈空间动态数组，里面存放着虚拟机运行时的堆栈数据，如局部变量，参数等
	ZENGL_RUN_RUNTIME_OP_DATA reg_list[ZL_R_REGLIST_SIZE]; //静态数组里面存放着所有寄存器的运行时值。
	ZENGL_RUN_INFO_STRING_TYPE infoFullString;  //里面存放了完整的经过解析后的调试信息字符串
	ZENGL_RUN_INFO_STRING_TYPE printFullString; //里面存放了完整解析后的用于PRINT之类的指令的输出字符串
	ZENGL_RUN_INFO_STRING_TYPE errorFullString; //里面存放了完整的经过解析后的错误信息的字符串
	ZL_CONST ZL_CHAR ** instTypeString;	//解释器中各种指令在打印输出时的显示字符串
	ZL_CONST ZL_CHAR ** registerTypeString; //解释器中各种寄存器在打印输出时的显示字符串
	ZENGL_RUN_INST_DATA_STRING_POOL InstData_StringPool; //指令操作数的字符串池
	ZL_INT HashTable[ZL_R_HASH_TOTAL_SIZE]; //hash表中存放了各种动态数组元素的索引值。
	ZENGL_RUN_MODULE_TABLE moduleTable; //模块动态数组，里面存放了各种脚本模块的初始化函数等信息
	ZENGL_RUN_MOD_FUN_TABLE ModFunTable; //模块函数动态数组，里面存放了各种脚本模块函数的处理句柄等信息
	ZENGL_RUN_EXTRA_DATA_TABLE ExtraDataTable; //用户给解释器提供的额外数据构成的动态数组
	ZL_INT CurRunModFunIndex; //当前正在运行的模块函数在模块函数动态数组中的索引值，通过该索引值，API接口就可以获取该模块函数的相关信息，如用户自定义的模块函数名等

	/*定义在zenglrun_func.c中的相关函数*/
	ZL_VOID (* init)(ZL_VOID * VM_ARG); //解释器初始化 对应 zenglrun_init
	ZL_VOID * (* memAlloc)(ZL_VOID * VM_ARG,ZL_INT size,ZL_INT * index); //为内存池分配内存和指针的函数 对应 zenglrun_memAlloc
	ZL_VOID * (* memReAlloc)(ZL_VOID * VM_ARG , ZL_VOID * point , ZL_INT size, ZL_INT * index); //调整point指针的大小为size，同时设置index为该指针在内存池中的索引。对应 zenglrun_memReAlloc
	ZL_VOID * (* memReUsePtr)(ZL_VOID * VM_ARG,ZL_VOID * point,ZL_INT size,ZL_INT * index); //根据需求的size大小，对内存池的指针进行调整大小等重利用操作 对应 zenglrun_memReUsePtr
	ZL_VOID (* exit)(ZL_VOID * VM_ARG,ZENGL_ERRORNO errorno, ...); //解释器退出函数 对应 zenglrun_exit
	ZL_VOID (* exit_forApiSetErrThenStop)(ZL_VOID * VM_ARG); //专门为zenglApi_SetErrThenStop这个API接口定制的退出函数 对应 zenglrun_exit_forApiSetErrThenStop
	ZL_VOID (* ExitPrintSourceInfo)(ZL_VOID * VM_ARG,ZENGL_ERRORNO errorno, ...); //当解释器出错退出时，打印出当前汇编代码对应的AST节点的行列号文件名信息 对应 zenglrun_ExitPrintSourceInfo
	ZL_VOID (* memFreeAll)(ZL_VOID * VM_ARG); //解释器释放内存池资源 对应 zenglrun_memFreeAll
	ZL_CHAR * (* SetApiErrorEx)(ZL_VOID * VM_ARG,ZENGL_ERRORNO errorno , ...); //专门用于设置API出错信息的扩展函数 对应 zenglrun_SetApiErrorEx
	ZL_CHAR * (* SetApiError)(ZL_VOID * VM_ARG,ZENGL_ERRORNO errorno ,ZENGL_SYS_ARG_LIST arglist); //专门用于设置API出错信息的函数 对应 zenglrun_SetApiError
	ZL_CHAR * (* makeInfoString)(ZL_VOID * VM_ARG,ZENGL_RUN_INFO_STRING_TYPE * infoStringPtr , ZL_CONST ZL_CHAR * format , ZENGL_SYS_ARG_LIST arglist); //生成格式化信息字符串 对应 zenglrun_makeInfoString
	ZL_VOID (* freeInfoString)(ZL_VOID * VM_ARG,ZENGL_RUN_INFO_STRING_TYPE * infoStringPtr); //释放格式化信息字符串的游标和大小信息 对应 zenglrun_freeInfoString
	ZL_VOID (* info)(ZL_VOID * VM_ARG , ZL_CONST ZL_CHAR * format , ...); //生成infoFullString或errorFullString的格式化字符串信息，并使用用户自定义函数进行输出显示 对应 zenglrun_info
	ZL_VOID (* print)(ZL_VOID * VM_ARG , ZL_CONST ZL_CHAR * format , ...); //生成printFullString的格式化字符串信息，并使用用户自定义函数输出显示 对应 zenglrun_print
	ZL_INT (* InstDataStringPoolAdd)(ZL_VOID * VM_ARG , ZL_CHAR * src); //指令操作数字符串池添加字符串 对应 zenglrun_InstDataStringPoolAdd
	ZL_VOID (* initInstDataStringPool)(ZL_VOID * VM_ARG); //指令操作数字符串池的初始化 对应 zenglrun_initInstDataStringPool
	ZL_CHAR * (* InstDataStringPoolGetPtr)(ZL_VOID * VM_ARG , ZL_INT index); //根据索引值从字符串池中查找指令操作数字符串的指针信息 对应 zenglrun_InstDataStringPoolGetPtr
	ZL_VOID * (* strcat)(ZL_VOID * VM_ARG,ZL_CHAR * dest,ZL_INT * destIndex,ZL_CHAR * src); //使用C库的strcat会破坏内存池，所以就自建一个zenglrun_strcat 对应 zenglrun_strcat
	ZL_VOID (* memFreeIndex)(ZL_VOID * VM_ARG,ZL_VOID * point,ZL_INT * srcindex); //释放内存池中指定索引对应的指针。对应 zenglrun_memFreeIndex
	ZL_VOID (* memFreeOnce)(ZL_VOID * VM_ARG,ZL_VOID * point); //释放内存池中的某个指针 对应 zenglrun_memFreeOnce
	ZL_VOID * (* strcat2)(ZL_VOID * VM_ARG,ZL_CHAR * dest,ZL_INT * destIndex,ZL_CHAR * src1,ZL_CHAR * src2); //将src1和src2两字符串连接在一起设置dest。对应 zenglrun_strcat2
	ZL_INT (* Hash)(ZL_VOID * VM_ARG,ZL_CHAR * key); //用于计算解释器中的哈希表索引 对应 zenglrun_Hash
	ZL_INT (* InsertModuleTable)(ZL_VOID * VM_ARG,ZL_CHAR * moduleName,ZL_VM_API_MOD_INIT_FUNC module_init_function); //将某模块名插入到模块动态数组中 对应 zenglrun_InsertModuleTable
	ZL_VOID (* initModuleTable)(ZL_VOID * VM_ARG); //模块动态数组初始化 对应 zenglrun_initModuleTable
	ZL_INT (* InsertModFunTable)(ZL_VOID * VM_ARG,ZL_INT moduleID,ZL_CHAR * functionName,ZL_VM_API_MOD_FUN_HANDLE handle); //将模块函数信息插入到模块函数动态数组中 对应 zenglrun_InsertModFunTable
	ZL_VOID (* initModFunTable)(ZL_VOID * VM_ARG); //模块函数动态数组初始化 对应 zenglrun_initModFunTable
	ZL_INT (* LookupModuleTable)(ZL_VOID * VM_ARG,ZL_CHAR * moduleName); //查找某模块的信息，返回该模块在模块动态数组中的索引 对应 zenglrun_LookupModuleTable
	ZL_INT (* LookupModFunTable)(ZL_VOID * VM_ARG,ZL_CHAR * functionName); //查找某模块函数的信息，返回该模块函数在动态数组中的索引 对应 zenglrun_LookupModFunTable
	ZL_VOID (* initExtraDataTable)(ZL_VOID * VM_ARG); //初始化用户额外数据动态数组 对应 zenglrun_initExtraDataTable
	ZL_INT (* InsertExtraDataTable)(ZL_VOID * VM_ARG,ZL_CHAR * extraDataName,ZL_VOID * point); //将数据插入到额外数据动态数组中 对应 zenglrun_InsertExtraDataTable
	/*定义在zenglrun_main.c中的相关函数*/
	ZL_VOID (* AddInst)(ZL_VOID * VM_ARG,ZL_INT pc,ZL_INT nodenum,ZENGL_RUN_INST_TYPE type, 
						 ZENGL_RUN_INST_OP_DATA_TYPE dest_type , ZL_DOUBLE dest_val ,
						 ZENGL_RUN_INST_OP_DATA_TYPE src_type , ZL_DOUBLE src_val); //添加汇编指令 对应 zenglrun_AddInst
	ZL_VOID (* initInstList)(ZL_VOID * VM_ARG); //初始化解释器的汇编指令集 对应 zenglrun_initInstList
	ZENGL_RUN_VIRTUAL_MEM_STRUCT (* VMemListOps)(ZL_VOID * VM_ARG,ZENGL_RUN_VMEM_OP_TYPE opType,ZL_INT memloc,
												  ZENGL_RUN_VIRTUAL_MEM_STRUCT argval); //虚拟机的虚拟内存操作函数 对应 zenglrun_VMemListOps
	ZL_VOID (* initVMemList)(ZL_VOID * VM_ARG); //初始化全局虚拟内存动态数组 对应 zenglrun_initVMemList
	ZENGL_RUN_VIRTUAL_MEM_STRUCT (* VStackListOps)(ZL_VOID * VM_ARG,ZENGL_RUN_VMEM_OP_TYPE opType,ZL_INT index,
												  ZENGL_RUN_VIRTUAL_MEM_STRUCT argval,ZL_BOOL valid); //虚拟堆栈空间的操作函数 对应 zenglrun_VStackListOps
	ZL_VOID (* initVStackList)(ZL_VOID * VM_ARG); //初始化虚拟机的堆栈空间。对应 zenglrun_initVStackList
	ZL_VOID (* printInstList)(ZL_VOID * VM_ARG,ZL_CHAR * head_title); //打印输出指令集 对应 zenglrun_printInstList
	ZL_VOID (* RunInsts)(ZL_VOID * VM_ARG); //虚拟机执行汇编指令的主程式。对应 zenglrun_RunInsts
	ZL_VOID (* RegAssignStr)(ZL_VOID * VM_ARG,ZENGL_RUN_REG_TYPE reg,ZL_VOID * str); //将某字符串赋值给reg寄存器 对应 zenglrun_RegAssignStr
	ZENGL_RUN_RUNTIME_OP_DATA_TYPE (* op_minis)(ZL_VOID * VM_ARG); //MINIS减法指令的相关处理程式。op是operate操作的缩写 对应 zenglrun_op_minis
	ZL_VOID (* op_je)(ZL_VOID * VM_ARG,ZENGL_RUN_RUNTIME_OP_DATA * src); //JE指令执行的操作，AX寄存器里存放着之前表达式的结果，当AX为0或空时，将修改PC寄存器的值，使脚本发生跳转 对应 zenglrun_op_je
	ZL_VOID (* op_jne)(ZL_VOID * VM_ARG,ZENGL_RUN_RUNTIME_OP_DATA * src); //JNE指令，和JE指令刚好相反。AX寄存器里存放着之前表达式的结果，当AX不为0，即为TRUE时，将修改PC寄存器的值，使脚本发生跳转 对应 zenglrun_op_jne
	ZL_VOID (* op_bits)(ZL_VOID * VM_ARG); //按位与，或，异或等位运算指令的处理程式 对应 zenglrun_op_bits
	ZL_VOID (* op_relation)(ZL_VOID * VM_ARG); //大于小于等于之类的比较运算符指令的处理程式。对应 zenglrun_op_relation
	ZL_VOID (* op_logic)(ZL_VOID * VM_ARG); //AND(且)，OR（或），REVERSE（取反）逻辑运算符的处理程式。对应 zenglrun_op_logic
	ZL_VOID (* op_addminisget)(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem,ZENGL_RUN_INST_TYPE type,ZENGL_RUN_INST_OP_DATA_TYPE memtype); //当加加或减减运算符在变量标示符的前面时，先将变量值加一或减一，再返回结果 对应 zenglrun_op_addminisget
	ZL_VOID (* op_getaddminis)(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem,ZENGL_RUN_INST_TYPE type,ZENGL_RUN_INST_OP_DATA_TYPE memtype); //加加减减运算符在变量标示符后面时，先取值后对其进行加一或减一操作 对应 zenglrun_op_getaddminis
	ZL_VOID (* op_addminisone)(ZL_VOID * VM_ARG,ZENGL_RUN_INST_TYPE type); //将表达式的结果进行加一，减一，再返回结果。对应 zenglrun_op_addminisone
	ZL_VOID (* op_addr)(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem); //ADDR引用汇编指令的操作 对应 zenglrun_op_addr
	ZL_VOID (* op_set_array)(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem); //设置数组元素 对应 zenglrun_op_set_array
	ZENGL_RUN_VIRTUAL_MEM_LIST * (* alloc_memblock)(ZL_VOID * VM_ARG,ZL_INT * index); //为内存块分配动态内存，通过ZENGL_RUN_VIRTUAL_MEM_LIST结构来管理内存块，ZENGL_RUN_VIRTUAL_MEM_LIST结构中的mem_array字段是新分配的内存块指针 对应 zenglrun_alloc_memblock
	ZL_INT (* memblock_getindex)(ZL_VOID * VM_ARG,ZL_INT i,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem); //获取数组内存块的索引 对应 zenglrun_memblock_getindex
	ZENGL_RUN_VIRTUAL_MEM_LIST * (* realloc_memblock)(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_LIST * ptr,ZL_INT index); //为内存块动态调整大小 对应 zenglrun_realloc_memblock
	ZL_VOID (* assign_memblock)(ZL_VOID * VM_ARG,ZL_VOID ** dest_arg,ZL_VOID * src_arg); //将dest_arg设为src_arg所指向的内存块，同时增加refcount内存块的引用计数 对应 zenglrun_assign_memblock
	ZL_VOID (* memblock_setval)(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_LIST * ptr,ZL_INT index,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem); //设置数组对应内存块ptr的index索引位置的值 对应 zenglrun_memblock_setval
	ZENGL_RUN_VIRTUAL_MEM_STRUCT (* VMemBlockOps)(ZL_VOID * VM_ARG,ZENGL_RUN_VMEM_OP_TYPE op,ZENGL_RUN_VIRTUAL_MEM_LIST * ptr,ZL_INT index,
												   ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem); //对数组ptr里的index索引指向的内存进行操作 对应 zenglrun_VMemBlockOps
	ZL_VOID (* memblock_free)(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_LIST * memblock,ZL_INT * index); //释放memblock所管理的内存块 对应 zenglrun_memblock_free
	ZL_VOID (* op_get_array)(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem); //获取数组中的某元素 对应 zenglrun_op_get_array
	ZL_VOID (* op_get_array_addr)(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem); //获取内存块元素的引用，如test = &testarray[0];的语句 对应 zenglrun_op_get_array_addr
	ZL_VOID (* op_addminis_one_array)(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem,ZENGL_RUN_INST_TYPE op); //对数组元素进行加加，减减运算 对应 zenglrun_op_addminis_one_array
	ZL_VOID (* memblock_freeall_local)(ZL_VOID * VM_ARG); //释放栈中参数部分和局部变量部分的所有内存块 对应 zenglrun_memblock_freeall_local
	ZL_VOID (* FreeAllForReUse)(ZL_VOID * VM_ARG); //重利用虚拟机时，释放掉全局虚拟内存，栈内存等里面的内存块和引用 对应 zenglrun_FreeAllForReUse
	ZL_VOID (* op_switch)(ZL_VOID * VM_ARG); //SWITCH指令的处理 对应 zenglrun_op_switch
	ZL_INT (* getRegInt)(ZL_VOID * VM_ARG,ZENGL_RUN_REG_TYPE reg); //返回寄存器值的整数形式 对应 zenglrun_getRegInt
	ZL_INT (* main)(ZL_VOID * VM_ARG);	//解释器的入口函数 对应 zenglrun_main

	/*下面是用户自定义的函数*/
	ZL_INT (* userdef_run_info)(ZL_CHAR * infoStrPtr, ZL_INT infoStrCount, ZL_VOID * VM_ARG); //用户自定义的解释器中显示普通信息的函数，用户可以自定义调试信息的打印和输出方式。
	ZL_INT (* userdef_run_print)(ZL_CHAR * infoStrPtr, ZL_INT infoStrCount, ZL_VOID * VM_ARG); //用户自定义的解释器PRINT之类的指令对应的打印输出方式。
	ZL_INT (* userdef_run_error)(ZL_CHAR * infoStrPtr, ZL_INT infoStrCount, ZL_VOID * VM_ARG); //用户自定义的解释器中显示错误信息的函数。
}ZENGL_RUN_TYPE;
/*虚拟机的解释器结构体定义结束*/

/*虚拟机结构体定义*/
typedef struct _ZENGL_VM_TYPE
{
	ZENGL_COMPILE_TYPE compile;
	ZENGL_RUN_TYPE run;
	ZENGL_ERRORNO errorno;
	ZL_CONST ZL_CHAR ** errorString;
	ZL_BOOL isCompileError;
	ZL_BOOL isRunError;
	ZL_INT signer;   //VM虚拟机是否有效的signer签名标志
	ZL_INT basesize; //当前虚拟机包含结构体的基础大小
	ZL_INT totalsize; //当前虚拟机占用内存的大小总和
	ZL_CLOCK_T start_time; //虚拟机开始执行的时间，毫秒为单位
	ZL_CLOCK_T end_time; //虚拟机结束时的时间，毫秒为单位
	ZL_CLOCK_T total_time; //执行结束时的总时间，毫秒为单位
	ZENGL_EXPORT_VM_MAIN_ARGS * vm_main_args; //用户传递给虚拟机的一些参数
	ZL_BOOL isinApiRun; //判断是否通过zenglApi_Run运行的虚拟机
	ZL_BOOL isUseApiSetErrThenStop; //判断用户是否在模块函数中通过调用zenglApi_SetErrThenStop来停止虚拟机的
	ZENGL_ENCRYPT initEncrypt; //加密运算相关结构体的初始化状态，用于初始化 ZENGL_SOURCE_TYPE 中的encrypt成员用的
	ZENGL_API_STATES ApiState; //API接口触发的虚拟机的各种状态，用于判断API调用的位置是否合法
	ZL_CHAR * ApiError; //专门用于存放API的出错信息的

	/*虚拟机相关的自定义函数*/
	ZL_VOID (* rc4InitState)(ZL_VOID * VM_ARG,ZL_CHAR * key,ZL_INT len); //RC4使用初始密钥来初始化state状态盒子 对应 zenglVM_rc4InitState
	ZL_VOID (* init)(ZL_VOID * VM_ARG,ZENGL_EXPORT_VM_MAIN_ARGS * vm_main_args); //虚拟机初始化函数 对应 zenglVM_init
}ZENGL_VM_TYPE;
/*虚拟机结构体定义结束*/

//下面是定义在zengl_locals.c中的全局变量
ZL_EXTERN ZL_CONST ZL_CHAR * ZL_Error_String[]; //存放各种出错信息！
ZL_EXTERN ZL_CONST ZL_CHAR * ZL_Reserves_String[]; //各种关键字(保留字)定义
ZL_EXTERN ZL_CONST ZL_CHAR * ZL_Token_Operate_String[]; //所有操作类型运算符的字符串信息
ZL_EXTERN ZL_CONST ZL_CHAR * ZL_Run_Inst_Type_String[];	//解释器中各种指令在打印输出时的显示字符串
ZL_EXTERN ZL_CONST ZL_CHAR * ZL_Run_Register_Type_String[]; //解释器中各种寄存器在打印输出时的显示字符串
ZL_EXTERN ZENGL_VM_TYPE ZL_Api_Const_VM; //虚拟机初始化模板

//下面是定义在zengl_main.c中的函数
ZL_VOID zengl_exit(ZL_VOID * VM_ARG,ZENGL_ERRORNO errorno, ...); //编译器退出函数，可以输出相关的出错信息
ZL_VOID zengl_freeall(ZL_VOID * VM_ARG); //编译器释放内存池资源
ZL_CHAR zengl_getNextchar(ZL_VOID * VM_ARG); //从文件中获取下一个字符
ZENGL_TOKENTYPE zengl_getToken(ZL_VOID * VM_ARG); //获取token信息
ZL_VOID zengl_makeTokenInfoLineCol(ZL_VOID * VM_ARG,ZL_INT line,ZL_INT col,ZL_CHAR * filename); //生成token的行列号等信息
ZL_VOID zengl_makeTokenStr(ZL_VOID * VM_ARG,ZL_CHAR ch); //生成token的字符串信息
ZL_VOID * zengl_malloc(ZL_VOID * VM_ARG , ZL_INT size); //编译器分配内存函数，将编译器相关内存指针统一交给编译器的内存池管理
ZL_VOID * zengl_realloc(ZL_VOID * VM_ARG , ZL_VOID * point , ZL_INT size); //调整内存池中某个指针的内存大小
ZL_VOID zengl_freeonce(ZL_VOID * VM_ARG,ZL_VOID * point); //释放编译器内存池中的某个指针
ZL_VOID zengl_ungetchar(ZL_VOID * VM_ARG); //将源脚本字符扫描游标回退一格，同时将列号减一
ZL_VOID zengl_convertStrToDec(ZL_VOID * VM_ARG); //将字符串转为有效的十进制数，再存放到tokenInfo中 
ZL_VOID zengl_freeTokenStr(ZL_VOID * VM_ARG); //修改游标等成员来释放当前扫描token的动态字符串信息，以供下一次扫描token时使用
ZL_VOID zengl_convertOctalToDec(ZL_VOID * VM_ARG); //将当前扫描token由八进制转为十进制
ZL_VOID zengl_convertHexToDec(ZL_VOID * VM_ARG); //将扫描token由十六进制格式转为十进制格式。
ZL_VOID zengl_makeTokenStrForString(ZL_VOID * VM_ARG,ZL_CHAR ch); //用于专门解析字符串类型的token的
ZENGL_TOKENTYPE zengl_lookupReserve(ZL_VOID * VM_ARG , ZENGL_TOKENTYPE token); //查询关键字的枚举值
ZL_VOID zengl_AddDefConst(ZL_VOID * VM_ARG); //词法扫描时如果遇到def关键字，就调用zengl_AddDefConst函数将宏和宏对应的值加入到哈希表
ZL_INT zengl_DefPoolAddString(ZL_VOID * VM_ARG , ZL_CHAR * src); //将参数src添加到def_StringPool字符串池中。并返回该字符串在池中的索引
ZL_VOID zengl_initDefStringPool(ZL_VOID * VM_ARG); //def定义后面的常量和值的字符串信息都存放在def_StringPool的字符串池中。初始化def_StringPool字符串池。
ZL_VOID zengl_insert_HashTableForDef(ZL_VOID * VM_ARG , ZL_INT nameIndex, ZENGL_TOKENTYPE tokentype,ZL_INT valIndex,
									 ZL_INT line,ZL_INT col); //将def定义的宏名和宏对应的值插入到动态数组中，同时将索引加入到哈希表中。
ZL_CHAR * zengl_DefPoolGetPtr(ZL_VOID * VM_ARG , ZL_INT index); //根据索引值从字符串池中查找宏定义字符串的指针信息
ZL_INT zengl_insert_DefTable(ZL_VOID * VM_ARG,ZL_INT nameIndex,ZENGL_TOKENTYPE tokentype,ZL_INT valIndex,ZL_INT line,ZL_INT col); //将每条def定义的宏名称和宏对应的值加入到def_table动态数组中。
ZL_VOID zengl_initDefTable(ZL_VOID * VM_ARG); //初始化def_table（def宏定义的动态数组）
ZL_VOID zengl_include_file(ZL_VOID * VM_ARG); //当词法扫描器扫描到inc关键字时，就会调用该函数来加载文件。
ZL_CHAR * zengl_makePathFileName(ZL_VOID * VM_ARG,ZL_CHAR * filename); //inc加载文件时，先得到当前扫描的文件的路径信息
ZL_VOID zengl_push_FileStack(ZL_VOID * VM_ARG , ZENGL_SOURCE_TYPE * src,ZL_INT line,ZL_INT col); //将原来的文件信息压入栈。
ZL_VOID zengl_pop_FileStack(ZL_VOID * VM_ARG , ZENGL_SOURCE_TYPE * dest); //将上次压入栈的文件信息弹出，从弹出的文件信息里恢复原文件的扫描。
ZL_VOID zengl_initFileStack(ZL_VOID * VM_ARG); //初始化inc加载文件的堆栈
ZENGL_TOKENTYPE zengl_ReplaceDefConst(ZL_VOID * VM_ARG, ZENGL_TOKENTYPE token); //替换宏为具体的值
ZL_INT zengl_lookupDefTable(ZL_VOID * VM_ARG, ZL_CHAR * name); //查找def宏表
ZL_CHAR * zengl_makeInfoString(ZL_VOID * VM_ARG,ZENGL_INFO_STRING_TYPE * infoStringPtr , ZL_CONST ZL_CHAR * format , ZENGL_SYS_ARG_LIST arglist); //生成格式化信息字符串
ZL_VOID zengl_freeInfoString(ZL_VOID * VM_ARG,ZENGL_INFO_STRING_TYPE * infoStringPtr); //释放格式化信息字符串的游标和大小信息
ZL_VOID zengl_info(ZL_VOID * VM_ARG , ZL_CONST ZL_CHAR * format , ...); //在用户函数中可以执行一些常规的打印操作，或者将字符串输出到文件等
ZL_VOID zengl_init(ZL_VOID * VM_ARG,ZL_CHAR * script_file,ZENGL_EXPORT_VM_MAIN_ARGS * vm_main_args); //编译器初始化
ZL_INT zengl_main(ZL_VOID * VM_ARG,ZL_CHAR * script_file,ZENGL_EXPORT_VM_MAIN_ARGS * vm_main_args); //编译器入口函数
ZL_VOID zengl_rc4InitState(ZL_VOID * VM_ARG,ZL_CHAR * key,ZL_INT len); //RC4使用初始密钥来初始化state状态盒子
ZL_VOID zenglVM_rc4InitState(ZL_VOID * VM_ARG,ZL_CHAR * key,ZL_INT len); //RC4使用初始密钥来初始化state状态盒子
ZL_VOID zenglVM_init(ZL_VOID * VM_ARG,ZENGL_EXPORT_VM_MAIN_ARGS * vm_main_args); //虚拟机初始化

//下面是定义在zengl_symbol.c中的函数
ZL_INT zengl_Sym_Hash (ZL_VOID * VM_ARG , ZL_CHAR * key ); //hash哈希散列算法
ZL_INT zengl_insert_LineCol(ZL_VOID * VM_ARG , ZL_INT line , ZL_INT col,ZL_CHAR * filename); //将line,col行列号，以及行列号所在的文件名插入到LineCols行列号动态数组中。
ZL_VOID zengl_initLineColTable(ZL_VOID * VM_ARG); //初始化LineCols行列号动态数组，用于记录符号表中每个全局符号出现过的行列号信息。
ZL_VOID zengl_buildSymTable(ZL_VOID * VM_ARG); //组建符号表的主程式。
ZL_VOID zengl_ScanInsertGlobalSym(ZL_VOID * VM_ARG,ZL_INT arg_nodenum); //扫描并插入全局变量符号表
ZL_VOID zengl_SymInsertDotID(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT parentNum); //判断点运算符里的节点是成员还是类还是全局变量，并将全局变量插入全局符号表中。
ZL_INT zengl_SymInsertHashTableForGlobal(ZL_VOID * VM_ARG,ZL_INT nodenum); //将全局变量加入到哈希表和全局符号表动态数组中。
ZL_INT zengl_SymInsertGlobalTable(ZL_VOID * VM_ARG,ZL_INT nameIndex,ZL_INT line,ZL_INT col,ZL_CHAR * filename); //将某个变量标示符插入到全局变量符号表里，并确定该变量对应的内存地址
ZL_VOID zengl_SymInitGlobalTable(ZL_VOID * VM_ARG); //初始化全局变量符号表
ZL_INT zengl_SymInsertHashTableForClass(ZL_VOID * VM_ARG,ZL_INT nodenum); //将nodenum类名所在的节点加入到哈希表和类符号表所在的动态数组中
ZL_INT zengl_SymInsertClassTable(ZL_VOID * VM_ARG,ZL_INT nameIndex,ZL_INT nodenum,ZL_INT line,ZL_INT col,ZL_CHAR * filename); //将类名和该类定义所在的行列号,文件名插入到类符号表对应的动态数组中
ZL_VOID zengl_SymInitClassTable(ZL_VOID * VM_ARG);	//初始化类符号表对应的动态数组
ZL_INT zengl_SymInsertHashTableForFun(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT classid); //将函数节点信息插入到函数表动态数组中，并将动态数组的索引保存到HashTable中
ZL_INT zengl_SymInsertFunTable(ZL_VOID * VM_ARG,ZL_INT nameIndex,ZL_INT classid,ZL_INT line,ZL_INT col,ZL_CHAR * filename); //将函数名和函数所在的行列号等信息插入到函数符号表对应的动态数组中。
ZL_VOID zengl_SymInitFunTable(ZL_VOID * VM_ARG);	//初始化函数符号表对应的动态数组
ZL_VOID zengl_ScanInsertClassMemberSym(ZL_VOID * VM_ARG,ZL_INT arg_nodenum,ZL_INT classid); //扫描类成员，并将类成员加入类成员符号表，以及将类函数加入函数符号表中。
ZL_INT zengl_SymLookupClass(ZL_VOID * VM_ARG,ZL_INT nodenum); //根据节点号查找类ID信息
ZL_INT zengl_SymLookupClassByName(ZL_VOID * VM_ARG,ZL_CHAR * name); //根据类名查找类ID信息
ZL_INT zengl_SymLookupClassMember(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT parent_classid); //从SymClassMemberTable中查找parent_classid对应的类的成员nodenum的信息
ZL_INT zengl_SymInsertHashTableForClassMember(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT index,ZL_INT classid,ZL_INT cls_stmt_classid); //将nodenum类成员所在的节点加入到哈希表和类成员符号表所在的动态数组中
ZL_INT zengl_SymInsertClassMemberTable(ZL_VOID * VM_ARG,ZL_INT nameIndex,ZL_INT index,ZL_INT classid,ZL_INT cls_stmt_classid,ZL_INT nodenum); //将类成员名，类成员在类结构中的索引值，所属类ID等信息加入类成员符号表中
ZL_VOID zengl_SymInitClassMemberTable(ZL_VOID * VM_ARG); //初始化类成员符号表对应的动态数组
ZENGL_RUN_INST_OP_DATA zengl_SymLookupID(ZL_VOID * VM_ARG,ZL_INT nodenum); //通过节点索引来查找该节点的变量标示符在自定义的虚拟内存中的内存地址
ZL_VOID zengl_SymInsertClassID(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT classid); //为nodenum节点设置classid类id
ZL_INT zengl_SymLookupID_ForDot(ZL_VOID * VM_ARG,ZL_INT nodenum); //查找nodenum对应节点的classid值，主要用于生成点运算符的汇编指令时
ZL_INT zengl_SymLookupID_ByName(ZL_VOID * VM_ARG,ZL_CHAR * name); //通过变量名称查找变量标示符在自定义的虚拟内存中的内存地址，主要用于对外的API接口中
ZL_INT zengl_SymLookupFun(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT classid);	//根据节点号查找函数ID信息
ZL_INT zengl_SymLookupFunByName(ZL_VOID * VM_ARG,ZL_CHAR * name,ZL_INT classid); //根据函数名称查找函数ID信息
ZL_VOID zengl_SymScanFunArg(ZL_VOID * VM_ARG,ZL_INT nodenum); //使用AST扫描堆栈来扫描语法树中函数的参数
ZL_VOID zengl_SymScanFunGlobal(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT global_nodenum); //使用AST扫描堆栈来扫描语法树中函数的global关键字声明的全局变量
ZL_VOID zengl_SymScanClassStatement(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT cls_statement_nodenum); //扫描类声明语句里声明的变量
ZL_VOID zengl_SymScanDotForClass(ZL_VOID * VM_ARG,ZL_INT nodenum); //扫描类引用时的节点，将类成员转为数组的索引压入栈中
ZL_VOID zengl_SymScanUseRsv(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT use_nodenum); //使用AST扫描堆栈来扫描语法树中use关键字声明的模块名
ZL_VOID zengl_SymScanFunLocal(ZL_VOID * VM_ARG,ZL_INT nodenum); //使用AST扫描堆栈来扫描语法树中函数的局部变量
ZL_BOOL zengl_SymInsertHashTableForLocal(ZL_VOID * VM_ARG,ZL_INT nodenum,ZENGL_SYM_ENUM_LOCAL_TYPE type); //将局部变量和参数插入到局部变量符号表中，并将符号表动态数组的索引加入到哈希表中
ZL_INT zengl_SymInsertLocalTable(ZL_VOID * VM_ARG,ZL_INT nameIndex,ZENGL_SYM_ENUM_LOCAL_TYPE type); //将局部变量名和局部变量的类型插入到SymLocalTable动态数组中。
ZL_VOID zengl_SymInitLocalTable(ZL_VOID * VM_ARG); //初始化SymLocalTable局部符号表对应的动态数组
ZL_VOID zengl_SymPrintTables(ZL_VOID * VM_ARG);		//打印符号表

//下面是定义在zengl_assemble.c中的函数
ZL_VOID zengl_buildAsmCode(ZL_VOID * VM_ARG); //组建汇编代码的主程式
ZL_VOID zengl_AsmGenCodes(ZL_VOID * VM_ARG,ZL_INT nodenum); //该函数根据AST抽象语法树的节点索引将某节点转为汇编代码。
ZL_INT zengl_AsmGCStackPush(ZL_VOID * VM_ARG,ZL_INT num,ZENGL_ASM_STACK_ENUM type); //将数字压入汇编堆栈
ZL_INT zengl_AsmGCStackPop(ZL_VOID * VM_ARG,ZENGL_ASM_STACK_ENUM type,ZL_BOOL isremove); //弹出汇编栈
ZL_VOID zengl_AsmGCStackInit(ZL_VOID * VM_ARG); //汇编堆栈初始化
ZL_VOID zengl_AsmGCElif(ZL_VOID * VM_ARG,ZENGL_AST_CHILD_NODE_TYPE * ifchnum,ZL_INT num); //zengl_AsmGCElif函数用于生成elif代码块对应的汇编指令
ZL_VOID zengl_AsmGCBreak_Codes(ZL_VOID * VM_ARG,ZL_INT nodenum); //break语句的汇编代码生成
ZL_VOID zengl_AsmGCContinue_Codes(ZL_VOID * VM_ARG,ZL_INT nodenum); //continue语句的汇编代码生成
ZL_VOID zengl_AsmScanCaseMinMax(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_BOOL * hasminmax,ZL_INT * minarg,ZL_INT * maxarg,ZL_BOOL * hasdefault,
								ZENGL_ASM_CASE_JMP_TABLE * table); //扫描switch...case ，找出其中的case的最大值，最小值，以及判断是否有default默认节点
ZL_INT zengl_GetNodeInt(ZL_VOID * VM_ARG,ZL_INT nodenum); //返回节点的字符串信息的整数形式
ZL_VOID zengl_AsmAddCaseJmpTable(ZL_VOID * VM_ARG,ZENGL_ASM_CASE_JMP_TABLE * table,ZL_INT num); //将case对应的比较数字添加到跳转表中
ZL_VOID zengl_AsmInitCaseJmpTable(ZL_VOID * VM_ARG,ZENGL_ASM_CASE_JMP_TABLE * table); //初始化switch case的跳转表
ZL_VOID zengl_AsmSortCaseJmpTable(ZL_VOID * VM_ARG,ZENGL_ASM_CASE_JMP_TABLE * table,ZL_INT nodenum); //将switch...case跳转表进行从小到大的排序

//下面是定义在zengl_ld.c中的函数
ZL_INT zengl_LDAddrListSet(ZL_VOID * VM_ARG,ZL_INT index,ZL_INT loc); //设置链接动态数组中某索引(伪地址)对应的实际汇编代码位置
ZL_VOID zengl_LDAddrListInit(ZL_VOID * VM_ARG); //链接动态数组初始化
ZL_VOID zengl_LDAddrListReplaceAll(ZL_VOID * VM_ARG); //将所有的伪地址替换为真实的汇编代码位置，从而完成链接工作

//下面是定义在zengl_parser.c中的函数
ZL_VOID zengl_ASTAddNode(ZL_VOID * VM_ARG,ZENGL_TOKENTYPE token); //将token加入AST抽象语法树
ZL_VOID zengl_initAST(ZL_VOID * VM_ARG); //初始化AST抽象语法树的动态数组。
ZL_INT zengl_TokenStringPoolAddStr(ZL_VOID * VM_ARG , ZL_CHAR * src); //向token字符串池动态缓存添加字符串，并返回该字符串在缓存中的索引信息
ZL_VOID zengl_initTokenStringPool(ZL_VOID * VM_ARG); //初始化token字符串池的动态缓存
ZL_VOID zengl_printNode(ZL_VOID * VM_ARG , ZL_INT nodenum); //打印token的信息
ZL_CHAR * zengl_getTokenStr(ZL_VOID * VM_ARG , ZENGL_AST_NODE_TYPE * nodes,ZL_INT nodenum); //根据节点号获取token对应的字符串信息
ZL_CHAR * zengl_TokenStringPoolGetPtr(ZL_VOID * VM_ARG , ZL_INT index); //根据索引index从token字符串池缓存中得到对应的字符串信息
ZL_VOID zengl_printASTnodes(ZL_VOID * VM_ARG); //打印出AST抽象语法树的所有节点信息。采用了新的AST扫描堆栈的方法进行遍历扫描。
ZL_VOID zengl_push_AST_TreeScanStack(ZL_VOID * VM_ARG , ZL_INT nodenum); //AST扫描堆栈的压栈函数
ZENGL_AST_SCAN_STACK_TYPE zengl_pop_AST_TreeScanStack(ZL_VOID * VM_ARG,ZL_BOOL remove); //AST扫描堆栈的弹出栈函数
ZL_VOID zengl_init_AST_TreeScanStack(ZL_VOID * VM_ARG); //初始化AST扫描堆栈AST_TreeScanStackList
ZL_VOID zengl_buildAST(ZL_VOID * VM_ARG); //该函数通过循环来构建所有语句AST
ZL_INT zengl_statement(ZL_VOID * VM_ARG); //生成语句比如print 'hello'之类的语句的AST语法树
ZL_VOID zengl_KeywordCompleteDetect(ZL_VOID * VM_ARG); //关键词完整性检查，例如if结构必须有endif相匹配等
ZL_VOID zengl_TokenSyntaxDetect(ZL_VOID * VM_ARG);	//use，global，break，continue，类声明等语句的语法检查
ZL_VOID zengl_if_stmt(ZL_VOID * VM_ARG,ZL_INT p); //if-elif-else控制语句的AST抽象语法树的生成函数。
ZL_INT zengl_if_stmt_sequence(ZL_VOID * VM_ARG); //if-elif-else-endif语句代码块中所有表达式的AST抽象语法树的生成
ZL_VOID zengl_for_stmt(ZL_VOID * VM_ARG,ZL_INT p); //for...endfor控制语句的AST抽象语法树的生成函数。
ZL_INT zengl_for_stmt_sequence(ZL_VOID * VM_ARG); //for...endfor语句代码块中所有表达式的AST抽象语法树的生成
ZL_VOID zengl_fun_stmt(ZL_VOID * VM_ARG,ZL_INT p); //fun...endfun的AST抽象语法树的生成函数。
ZL_INT zengl_fun_stmt_sequence(ZL_VOID * VM_ARG); //fun...endfun语句代码块中所有表达式的AST抽象语法树的生成
ZL_VOID zengl_switch_stmt(ZL_VOID * VM_ARG,ZL_INT p); //switch...case结构的AST生成。
ZL_INT zengl_switch_stmt_sequence(ZL_VOID * VM_ARG); //switch...case语句代码块中所有表达式的AST抽象语法树的生成
ZL_VOID zengl_class_stmt(ZL_VOID * VM_ARG,ZL_INT p); //class...endcls或endclass结构的AST生成。
ZL_INT zengl_class_stmt_sequence(ZL_VOID * VM_ARG); //class...endcls或endclass语句代码块中所有表达式的AST抽象语法树的生成
ZL_VOID zengl_while_stmt(ZL_VOID * VM_ARG,ZL_INT p); //while...endwhile结构的AST生成。
ZL_INT zengl_while_stmt_sequence(ZL_VOID * VM_ARG); //while...endwhile语句代码块中所有表达式的AST抽象语法树的生成
ZL_VOID zengl_do_stmt(ZL_VOID * VM_ARG,ZL_INT p);	//do...dowhile结构的AST生成。
ZL_INT zengl_do_stmt_sequence(ZL_VOID * VM_ARG);	//do...dowhile语句代码块中所有表达式的AST抽象语法树的生成
ZL_INT zengl_detectSetRBracketToSemi(ZL_VOID * VM_ARG); //将if之类的关键字右侧的右括号临时转为分号，这样就可以得到if括号中的条件判断表达式的AST
ZL_INT zengl_express(ZL_VOID * VM_ARG); //第三个版本的语法分析函数，采用堆栈和优先级来生成语法树。
ZL_VOID zengl_parser_addcurnode(ZL_VOID * VM_ARG); //增加parser_curnode当前语法分析节点的节点号，这个函数可以有效的防止parser_curnode溢出。
ZL_VOID zengl_parser_errorExit(ZL_VOID * VM_ARG,ZENGL_ERRORNO errorno, ...);  //语法分析时调用的打印输出错误信息的函数
ZL_VOID zengl_opLevel_push_stack(ZL_VOID * VM_ARG,ZL_INT nodenum); //将某节点压入栈，当需要比较优先级时，再弹出栈
ZENGL_PARSER_OP_LEVEL_STACK_TYPE zengl_opLevel_pop_stack(ZL_VOID * VM_ARG,ZL_INT index); //将最后压入栈的节点弹出。
ZL_VOID zengl_initOpLevelStackList(ZL_VOID * VM_ARG); //初始化优先级堆栈动态数组
ZL_VOID zengl_detectCurnodeSyntax(ZL_VOID * VM_ARG); //根据当前curnode节点和curnode+1节点来初步判断是否有语法错误
ZL_VOID zengl_OpLevelForTwo(ZL_VOID * VM_ARG); //使用优先级堆栈处理加减乘除等双目运算符
ZL_VOID zengl_OpLevelForAssign(ZL_VOID * VM_ARG); //使用优先级堆栈处理赋值语句的双目运算符
ZL_VOID zengl_OpLevelForSEMI(ZL_VOID * VM_ARG); //使用优先级堆栈处理分号结束符
ZL_VOID zengl_OpLevelForPPMM(ZL_VOID * VM_ARG); //使用优先级堆栈处理加加减减运算符
ZL_VOID zengl_PPMM_GET_POS(ZL_VOID * VM_ARG); //获取加加减减运算符的左右侧的位置标识
ZL_VOID zengl_OpLevelForRBRACKET(ZL_VOID * VM_ARG); //使用优先级堆栈处理右括号
ZL_VOID zengl_OpLevelForRMBRACKET(ZL_VOID * VM_ARG); //使用优先级堆栈处理右中括号
ZL_VOID zengl_OpLevelForQuestion(ZL_VOID * VM_ARG); //使用优先级堆栈处理问号运算符
ZL_VOID zengl_OpLevelForColon(ZL_VOID * VM_ARG); //使用优先级堆栈处理冒号运算符
ZL_VOID zengl_CheckQstColonValid(ZL_VOID * VM_ARG); //检测问号冒号是否一一匹配
ZL_VOID zengl_ASTAddNodeChild(ZL_VOID * VM_ARG,ZL_INT parent,ZL_INT child); //将child对应的节点加入到parent节点的子节点中。
ZL_BOOL zengl_CheckIsNegative(ZL_VOID * VM_ARG); //初步判断当前的减号是否是负号
ZL_BOOL zengl_CheckIsBitAnd(ZL_VOID * VM_ARG); //初步判断当前的"&"符号是否是按位与运算符

//下面是定义在zenglrun_func.c中的函数
ZL_VOID zenglrun_init(ZL_VOID * VM_ARG); //解释器初始化
ZL_VOID * zenglrun_memAlloc(ZL_VOID * VM_ARG,ZL_INT size,ZL_INT * index); //为内存池分配内存和指针的函数
ZL_VOID * zenglrun_memReAlloc(ZL_VOID * VM_ARG , ZL_VOID * point , ZL_INT size, ZL_INT * index); //调整point指针的大小为size，同时设置index为该指针在内存池中的索引。
ZL_VOID * zenglrun_memReUsePtr(ZL_VOID * VM_ARG,ZL_VOID * point,ZL_INT size,ZL_INT * index); //根据需求的size大小，对内存池的指针进行调整大小等重利用操作
ZL_VOID zenglrun_exit(ZL_VOID * VM_ARG,ZENGL_ERRORNO errorno, ...); //解释器退出函数
ZL_VOID zenglrun_exit_forApiSetErrThenStop(ZL_VOID * VM_ARG); //专门为zenglApi_SetErrThenStop这个API接口定制的退出函数
ZL_VOID zenglrun_ExitPrintSourceInfo(ZL_VOID * VM_ARG,ZENGL_ERRORNO errorno, ...); //当解释器出错退出时，打印出当前汇编代码对应的AST节点的行列号文件名信息
ZL_VOID zenglrun_memFreeAll(ZL_VOID * VM_ARG); //解释器释放内存池资源
ZL_CHAR * zenglrun_SetApiErrorEx(ZL_VOID * VM_ARG,ZENGL_ERRORNO errorno , ...); //专门用于设置API出错信息的扩展函数
ZL_CHAR * zenglrun_SetApiError(ZL_VOID * VM_ARG,ZENGL_ERRORNO errorno ,ZENGL_SYS_ARG_LIST arglist); //专门用于设置API出错信息的函数
ZL_CHAR * zenglrun_makeInfoString(ZL_VOID * VM_ARG,ZENGL_RUN_INFO_STRING_TYPE * infoStringPtr , ZL_CONST ZL_CHAR * format , ZENGL_SYS_ARG_LIST arglist); //生成格式化信息字符串
ZL_VOID zenglrun_freeInfoString(ZL_VOID * VM_ARG,ZENGL_RUN_INFO_STRING_TYPE * infoStringPtr); //释放格式化信息字符串的游标和大小信息
ZL_VOID zenglrun_info(ZL_VOID * VM_ARG , ZL_CONST ZL_CHAR * format , ...); //生成infoFullString或errorFullString的格式化字符串信息，并使用用户自定义函数进行输出显示
ZL_VOID zenglrun_print(ZL_VOID * VM_ARG , ZL_CONST ZL_CHAR * format , ...); //生成printFullString的格式化字符串信息，并使用用户自定义函数输出显示
ZL_INT zenglrun_InstDataStringPoolAdd(ZL_VOID * VM_ARG , ZL_CHAR * src); //指令操作数字符串池添加字符串
ZL_VOID zenglrun_initInstDataStringPool(ZL_VOID * VM_ARG); //指令操作数字符串池的初始化
ZL_CHAR * zenglrun_InstDataStringPoolGetPtr(ZL_VOID * VM_ARG , ZL_INT index); //根据索引值从字符串池中查找指令操作数字符串的指针信息
ZL_VOID * zenglrun_strcat(ZL_VOID * VM_ARG,ZL_CHAR * dest,ZL_INT * destIndex,ZL_CHAR * src); //使用C库的strcat会破坏内存池，所以就自建一个zenglrun_strcat
ZL_VOID zenglrun_memFreeIndex(ZL_VOID * VM_ARG,ZL_VOID * point,ZL_INT * srcindex); //释放内存池中指定索引对应的指针。
ZL_VOID zenglrun_memFreeOnce(ZL_VOID * VM_ARG,ZL_VOID * point); //释放内存池中的某个指针
ZL_VOID * zenglrun_strcat2(ZL_VOID * VM_ARG,ZL_CHAR * dest,ZL_INT * destIndex,ZL_CHAR * src1,ZL_CHAR * src2); //将src1和src2两字符串连接在一起设置dest。
ZL_INT zenglrun_Hash(ZL_VOID * VM_ARG,ZL_CHAR * key); //用于计算解释器中的哈希表索引
ZL_INT zenglrun_InsertModuleTable(ZL_VOID * VM_ARG,ZL_CHAR * moduleName,ZL_VM_API_MOD_INIT_FUNC module_init_function); //将某模块名插入到模块动态数组中
ZL_VOID zenglrun_initModuleTable(ZL_VOID * VM_ARG); //模块动态数组初始化
ZL_INT zenglrun_InsertModFunTable(ZL_VOID * VM_ARG,ZL_INT moduleID,ZL_CHAR * functionName,ZL_VM_API_MOD_FUN_HANDLE handle); //将模块函数信息插入到模块函数动态数组中
ZL_VOID zenglrun_initModFunTable(ZL_VOID * VM_ARG); //模块函数动态数组初始化
ZL_INT zenglrun_LookupModuleTable(ZL_VOID * VM_ARG,ZL_CHAR * moduleName); //查找某模块的信息，返回该模块在模块动态数组中的索引
ZL_INT zenglrun_LookupModFunTable(ZL_VOID * VM_ARG,ZL_CHAR * functionName); //查找某模块函数的信息，返回该模块函数在动态数组中的索引
ZL_VOID zenglrun_initExtraDataTable(ZL_VOID * VM_ARG); //初始化用户额外数据动态数组
ZL_INT zenglrun_InsertExtraDataTable(ZL_VOID * VM_ARG,ZL_CHAR * extraDataName,ZL_VOID * point); //将数据插入到额外数据动态数组中

//下面是定义在zenglrun_main.c中的函数
ZL_VOID zenglrun_AddInst(ZL_VOID * VM_ARG,ZL_INT pc,ZL_INT nodenum,ZENGL_RUN_INST_TYPE type, 
						 ZENGL_RUN_INST_OP_DATA_TYPE dest_type , ZL_DOUBLE dest_val ,
						 ZENGL_RUN_INST_OP_DATA_TYPE src_type , ZL_DOUBLE src_val); //添加汇编指令
ZL_VOID zenglrun_initInstList(ZL_VOID * VM_ARG); //初始化解释器的汇编指令集
ZENGL_RUN_VIRTUAL_MEM_STRUCT zenglrun_VMemListOps(ZL_VOID * VM_ARG,ZENGL_RUN_VMEM_OP_TYPE opType,ZL_INT memloc,
												  ZENGL_RUN_VIRTUAL_MEM_STRUCT argval); //虚拟机的虚拟内存操作函数
ZL_VOID zenglrun_initVMemList(ZL_VOID * VM_ARG); //初始化全局虚拟内存动态数组
ZENGL_RUN_VIRTUAL_MEM_STRUCT zenglrun_VStackListOps(ZL_VOID * VM_ARG,ZENGL_RUN_VMEM_OP_TYPE opType,ZL_INT index,
												  ZENGL_RUN_VIRTUAL_MEM_STRUCT argval,ZL_BOOL valid); //虚拟堆栈空间的操作函数
ZL_VOID zenglrun_initVStackList(ZL_VOID * VM_ARG); //初始化虚拟机的堆栈空间。
ZL_VOID zenglrun_printInstList(ZL_VOID * VM_ARG,ZL_CHAR * head_title); //打印输出指令集
ZL_VOID zenglrun_RunInsts(ZL_VOID * VM_ARG); //虚拟机执行汇编指令的主程式。
ZL_VOID zenglrun_RegAssignStr(ZL_VOID * VM_ARG,ZENGL_RUN_REG_TYPE reg,ZL_VOID * str); //将某字符串赋值给reg寄存器
ZENGL_RUN_RUNTIME_OP_DATA_TYPE zenglrun_op_minis(ZL_VOID * VM_ARG); //MINIS减法指令的相关处理程式。op是operate操作的缩写
ZL_VOID zenglrun_op_je(ZL_VOID * VM_ARG,ZENGL_RUN_RUNTIME_OP_DATA * src); //JE指令执行的操作，AX寄存器里存放着之前表达式的结果，当AX为0或空时，将修改PC寄存器的值，使脚本发生跳转
ZL_VOID zenglrun_op_jne(ZL_VOID * VM_ARG,ZENGL_RUN_RUNTIME_OP_DATA * src); //JNE指令，和JE指令刚好相反。AX寄存器里存放着之前表达式的结果，当AX不为0，即为TRUE时，将修改PC寄存器的值，使脚本发生跳转
ZL_VOID zenglrun_op_bits(ZL_VOID * VM_ARG); //按位与，或，异或等位运算指令的处理程式
ZL_VOID zenglrun_op_relation(ZL_VOID * VM_ARG); //大于小于等于之类的比较运算符指令的处理程式。
ZL_VOID zenglrun_op_logic(ZL_VOID * VM_ARG); //AND(且)，OR（或），REVERSE（取反）逻辑运算符的处理程式。
ZL_VOID zenglrun_op_addminisget(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem,ZENGL_RUN_INST_TYPE type,ZENGL_RUN_INST_OP_DATA_TYPE memtype); //当加加或减减运算符在变量标示符的前面时，先将变量值加一或减一，再返回结果
ZL_VOID zenglrun_op_getaddminis(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem,ZENGL_RUN_INST_TYPE type,ZENGL_RUN_INST_OP_DATA_TYPE memtype); //加加减减运算符在变量标示符后面时，先取值后对其进行加一或减一操作
ZL_VOID zenglrun_op_addminisone(ZL_VOID * VM_ARG,ZENGL_RUN_INST_TYPE type); //将表达式的结果进行加一，减一，再返回结果。
ZL_VOID zenglrun_op_addr(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem); //ADDR引用汇编指令的操作
ZL_VOID zenglrun_op_set_array(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem); //设置数组元素
ZENGL_RUN_VIRTUAL_MEM_LIST * zenglrun_alloc_memblock(ZL_VOID * VM_ARG,ZL_INT * index); //为内存块分配动态内存，通过ZENGL_RUN_VIRTUAL_MEM_LIST结构来管理内存块，ZENGL_RUN_VIRTUAL_MEM_LIST结构中的mem_array字段是新分配的内存块指针
ZL_INT zenglrun_memblock_getindex(ZL_VOID * VM_ARG,ZL_INT i,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem); //获取数组内存块的索引
ZENGL_RUN_VIRTUAL_MEM_LIST * zenglrun_realloc_memblock(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_LIST * ptr,ZL_INT index); //为内存块动态调整大小
ZL_VOID zenglrun_assign_memblock(ZL_VOID * VM_ARG,ZL_VOID ** dest_arg,ZL_VOID * src_arg); //将dest_arg设为src_arg所指向的内存块，同时增加refcount内存块的引用计数
ZL_VOID zenglrun_memblock_setval(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_LIST * ptr,ZL_INT index,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem); //设置数组对应内存块ptr的index索引位置的值
ZENGL_RUN_VIRTUAL_MEM_STRUCT zenglrun_VMemBlockOps(ZL_VOID * VM_ARG,ZENGL_RUN_VMEM_OP_TYPE op,ZENGL_RUN_VIRTUAL_MEM_LIST * ptr,ZL_INT index,
												   ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem); //对数组ptr里的index索引指向的内存进行操作
ZL_VOID zenglrun_memblock_free(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_LIST * memblock,ZL_INT * index); //释放memblock所管理的内存块
ZL_VOID zenglrun_op_get_array(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem); //获取数组中的某元素
ZL_VOID zenglrun_op_get_array_addr(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem); //获取内存块元素的引用，如test = &testarray[0];的语句
ZL_VOID zenglrun_op_addminis_one_array(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem,ZENGL_RUN_INST_TYPE op); //对数组元素进行加加，减减运算
ZL_VOID zenglrun_memblock_freeall_local(ZL_VOID * VM_ARG); //释放栈中参数部分和局部变量部分的所有内存块
ZL_VOID zenglrun_FreeAllForReUse(ZL_VOID * VM_ARG); //重利用虚拟机时，释放掉全局虚拟内存，栈内存等里面的内存块和引用
ZL_VOID zenglrun_op_switch(ZL_VOID * VM_ARG); //SWITCH指令的处理
ZL_INT zenglrun_getRegInt(ZL_VOID * VM_ARG,ZENGL_RUN_REG_TYPE reg); //返回寄存器值的整数形式
ZL_INT zenglrun_main(ZL_VOID * VM_ARG);	//解释器的入口函数

#endif /* _ZENGL_GLOBAL_H_ */
