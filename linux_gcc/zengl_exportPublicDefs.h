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

#ifndef _ZENGL_EXPORT_PUBLIC_DEFS_H_
#define _ZENGL_EXPORT_PUBLIC_DEFS_H_

#define ZL_EXP_MAJOR_VERSION 1 //zengl主版本号
#define ZL_EXP_MINOR_VERSION 2 //zengl子版本号
#define ZL_EXP_REVISION 2      //zengl修正版本号
#define ZL_EXP_VOID void //采用自定义的宏来代替void , char之类的C标准类型，方便以后的统一调整，这几个类型宏也可以用typedef来处理。
#define ZL_EXP_CHAR char
#define ZL_EXP_INT int
#define ZL_EXP_DOUBLE double
#define ZL_EXP_NULL 0 //指针为0的宏定义

typedef ZL_EXP_VOID (* ZL_VM_API_MOD_INIT_FUNC)(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT moduleID);
typedef ZL_EXP_VOID (* ZL_VM_API_MOD_FUN_HANDLE)(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount);

typedef enum _ZENGL_EXPORT_VM_MAIN_ARG_FLAGS{
	ZL_EXP_CP_AF_IN_DEBUG_MODE = 0x1,		//保留编译器内存中的语法树符号表等资源，以供解释器调试等用途
	ZL_EXP_CP_AF_OUTPUT_DEBUG_INFO = 0x2,	//打印输出编译器的符号表等调试信息
}ZENGL_EXPORT_VM_MAIN_ARG_FLAGS;

typedef enum _ZENGL_EXPORT_VM_FLAGS_HANDLE_TYPE{
	ZL_EXP_VFLAG_HANDLE_NONE,			//默认初始值
	ZL_EXP_VFLAG_HANDLE_COMPILE_INFO,	//userdef_info用户自定义的显示普通信息的函数
	ZL_EXP_VFLAG_HANDLE_COMPILE_ERROR,	//userdef_compile_error用户自定义的显示错误信息的函数。
	ZL_EXP_VFLAG_HANDLE_RUN_INFO,		//userdef_run_info用户自定义的解释器中显示普通信息的函数
	ZL_EXP_VFLAG_HANDLE_RUN_PRINT,		//userdef_run_print运行时的PRINT指令对应的用户自定义函数句柄
	ZL_EXP_VFLAG_HANDLE_RUN_ERROR,		//userdef_run_error用户自定义的解释器中显示错误信息的函数。
	ZL_EXP_VFLAG_HANDLE_MODULE_INIT,	//userdef_module_init用户自定义的模块初始化函数
}ZENGL_EXPORT_VM_FLAGS_HANDLE_TYPE;

typedef enum _ZENGL_EXPORT_MOD_FUN_ARG_TYPE{
	ZL_EXP_FAT_NONE,
	ZL_EXP_FAT_INT,
	ZL_EXP_FAT_FLOAT,
	ZL_EXP_FAT_STR,
	ZL_EXP_FAT_MEMBLOCK, //内存块类型
	ZL_EXP_FAT_ADDR,	 //全局变量的引用
	ZL_EXP_FAT_ADDR_LOC, //局部变量的引用
	ZL_EXP_FAT_ADDR_MEMBLK,//内存块的引用
}ZENGL_EXPORT_MOD_FUN_ARG_TYPE; //通过API获取的参数类型

#ifdef __cplusplus 
	#ifdef ZL_EXP_OS_IN_WINDOWS
		#define ZL_EXPORT extern "C" __declspec (dllexport) 
	#else 
		#define ZL_EXPORT extern "C" __attribute__((visibility("default")))
	#endif 
#else 
	#ifdef ZL_EXP_OS_IN_WINDOWS
		#define ZL_EXPORT __declspec (dllexport) 
	#else 
		#define ZL_EXPORT __attribute__((visibility("default")))
	#endif 
#endif

typedef struct _ZENGL_EXPORT_MEMBLOCK{
	ZL_EXP_VOID * ptr; //内存块指针
	ZL_EXP_INT index;//内存块指针在内存池中的索引
}ZENGL_EXPORT_MEMBLOCK; //数组，类变量等内存块

typedef struct _ZENGL_EXPORT_ADDR{
	ZL_EXP_INT index;
	ZENGL_EXPORT_MEMBLOCK memblock;
}ZENGL_EXPORT_ADDR; //全局变量，局部变量，内存块的引用

typedef struct _ZENGL_EXPORT_MOD_FUN_ARG{
	ZENGL_EXPORT_MOD_FUN_ARG_TYPE type;
	struct{
		ZL_EXP_INT integer; //当函数调用的参数为整数时
		ZL_EXP_DOUBLE floatnum; //浮点数
		ZL_EXP_CHAR * str; //字符串
		ZENGL_EXPORT_MEMBLOCK memblock; //内存块类型
		ZENGL_EXPORT_ADDR addr; //引用类型
	}val;
}ZENGL_EXPORT_MOD_FUN_ARG; //通过API获取的参数结构体定义

typedef struct _ZENGL_EXPORT_VM_MAIN_ARGS{
	ZL_EXP_INT (* userdef_info)(ZL_EXP_CHAR * infoStrPtr, ZL_EXP_INT infoStrCount); //用户自定义的显示普通信息的函数，用户可以自定义信息的打印和输出方式。
	ZL_EXP_INT (* userdef_compile_error)(ZL_EXP_CHAR * infoStrPtr, ZL_EXP_INT infoStrCount); //用户自定义的显示错误信息的函数。
	ZL_EXP_INT (* userdef_run_info)(ZL_EXP_CHAR * infoStrPtr, ZL_EXP_INT infoStrCount); //用户自定义的解释器中显示普通信息的函数，用户可以自定义信息的打印和输出方式。
	ZL_EXP_INT (* userdef_run_print)(ZL_EXP_CHAR * infoStrPtr, ZL_EXP_INT infoStrCount); //用户自定义的解释器PRINT之类的指令对应的打印输出方式。
	ZL_EXP_INT (* userdef_run_error)(ZL_EXP_CHAR * infoStrPtr, ZL_EXP_INT infoStrCount); //用户自定义的解释器中显示错误信息的函数。
	ZL_EXP_VOID (* userdef_module_init)(ZL_EXP_VOID * VM_ARG); //用户自定义的模块初始化函数
	ZL_EXP_INT flags; //用户自定义的一些编译器或解释器的选项
}ZENGL_EXPORT_VM_MAIN_ARGS;

#endif/* _ZENGL_EXPORT_PUBLIC_DEFS_H_ */