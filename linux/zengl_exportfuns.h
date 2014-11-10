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

#ifndef _ZENGL_EXPORT_FUNCTIONS_H_
#define _ZENGL_EXPORT_FUNCTIONS_H_

#define ZL_EXP_MAJOR_VERSION 1 //zengl主版本号
#define ZL_EXP_MINOR_VERSION 5 //zengl子版本号
#define ZL_EXP_REVISION 0      //zengl修正版本号
#define ZL_EXP_VOID void //采用自定义的宏来代替void , char之类的C标准类型，方便以后的统一调整，这几个类型宏也可以用typedef来处理。
#ifdef ZL_EXP_OS_IN_ARM_GCC
	#define ZL_EXP_CHAR signed char //使用signed表示有符号的意思，因为ARM GCC下char默认是unsigned的(嵌入式上面会引发很多问题！)，所以有必要在这里指明是signed
#else
	#define ZL_EXP_CHAR char
#endif
#define ZL_EXP_INT int
#define ZL_EXP_LONG long
#define ZL_EXP_DOUBLE double
#define ZL_EXP_NULL 0 //指针为0的宏定义
#define ZL_EXP_FALSE 0 //逻辑假
#define ZL_EXP_TRUE 1 //逻辑真
typedef unsigned char ZL_EXP_BOOL; //定义bool类型

typedef ZL_EXP_VOID (* ZL_VM_API_MODS_INIT)(ZL_EXP_VOID * VM_ARG); //全局模块初始化函数，会在run解释器入口处执行
typedef ZL_EXP_VOID (* ZL_VM_API_MOD_INIT_FUNC)(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT moduleID); //单个模块初始化函数，use指令执行时会调用该函数
typedef ZL_EXP_VOID (* ZL_VM_API_MOD_FUN_HANDLE)(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount); //脚本中的模块函数在执行时调用的用户自定义函数
typedef ZL_EXP_INT (* ZL_VM_API_INFO_FUN_HANDLE)(ZL_EXP_CHAR * infoStrPtr, ZL_EXP_INT infoStrCount, ZL_EXP_VOID * VM_ARG); //一些调试信息或print指令等调用的用户自定义的函数指针
typedef ZL_EXP_INT (* ZL_VM_API_DEBUG_BREAK_HANDLE)(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * filename,ZL_EXP_INT line,ZL_EXP_INT breakIndex,ZL_EXP_CHAR * log); //用户自定义的断点调试函数
typedef ZL_EXP_INT (* ZL_VM_API_DEBUG_CON_ERROR_HANDLE)(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * filename,ZL_EXP_INT line,ZL_EXP_INT breakIndex,ZL_EXP_CHAR * error); //用户自定义的条件断点中条件出错时的调用函数

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
	ZL_EXP_FAT_INVALID,	 //无效的参数类型,可以用于指示API错误
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
		ZL_EXP_LONG integer; //当函数调用的参数为整数时
		ZL_EXP_DOUBLE floatnum; //浮点数
		ZL_EXP_CHAR * str; //字符串
		ZENGL_EXPORT_MEMBLOCK memblock; //内存块类型
		ZENGL_EXPORT_ADDR addr; //引用类型
	}val;
}ZENGL_EXPORT_MOD_FUN_ARG; //通过API获取的参数结构体定义

typedef struct _ZENGL_EXPORT_VM_MAIN_ARGS{
	ZL_EXP_VOID * userdef_info; //用户自定义的显示普通信息的函数，用户可以自定义信息的打印和输出方式。
	ZL_EXP_VOID * userdef_compile_error; //用户自定义的显示错误信息的函数。
	ZL_EXP_VOID * userdef_run_info; //用户自定义的解释器中显示普通信息的函数，用户可以自定义信息的打印和输出方式。
	ZL_EXP_VOID * userdef_run_print; //用户自定义的解释器PRINT之类的指令对应的打印输出方式。
	ZL_EXP_VOID * userdef_run_error; //用户自定义的解释器中显示错误信息的函数。
	ZL_EXP_VOID * userdef_module_init; //用户自定义的全局模块初始化函数，在run解释器入口处会被调用
	ZL_EXP_INT flags; //用户自定义的一些编译器或解释器的选项
}ZENGL_EXPORT_VM_MAIN_ARGS;

/*以下为zenglApi接口的声明，目前一共有50个API接口函数(不包括底部声明的那些内建模块函数)*/

/*通过zenglApi_Load可以加载并执行script_file脚本*/
ZL_EXPORT ZL_EXP_INT zenglApi_Load(ZL_EXP_CHAR * script_file,ZENGL_EXPORT_VM_MAIN_ARGS * vm_main_args);

/*创建并返回新的虚拟机指针，该指针必须通过zenglApi_Close进行释放，否则会发生内存泄漏*/
ZL_EXPORT ZL_EXP_VOID * zenglApi_Open();

/*使用zenglApi_Open打开的虚拟机来运行script_file对应的脚本*/
ZL_EXPORT ZL_EXP_INT zenglApi_Run(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * script_file);

/*
使用zenglApi_Open打开的虚拟机来运行script_str指向的字符串脚本，len为字符串脚本的有效长度，script_name为该字符串脚本的自定义名字，
script_name可以用于调试信息输出，同时还可以给script_name设置一个路径前缀，例如"script/runstr"，这样字符串脚本内部inc加载其他脚本时就会按照"script/"路径来加载
*/
ZL_EXPORT ZL_EXP_INT zenglApi_RunStr(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * script_str,ZL_EXP_INT len,ZL_EXP_CHAR * script_name);

/*关闭zenglApi_Open打开的虚拟机指针，并释放虚拟机占用的系统资源*/
ZL_EXPORT ZL_EXP_INT zenglApi_Close(ZL_EXP_VOID * VM_ARG);

/*API接口，重置参数对应的虚拟机*/
ZL_EXPORT ZL_EXP_VOID * zenglApi_Reset(ZL_EXP_VOID * VM_ARG);

/*API接口，重利用虚拟机之前的编译资源，这样就可以直接执行之前已经编译好的指令代码
 参数isClear不为0则执行虚拟内存的清理工作，返回-1表示出错，返回0表示什么都没做，返回1表示正常执行*/
ZL_EXPORT ZL_EXP_INT zenglApi_ReUse(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT isClear);

/*API接口，返回编译运行时的出错信息*/
ZL_EXPORT ZL_EXP_CHAR * zenglApi_GetErrorString(ZL_EXP_VOID * VM_ARG);

/*API接口，设置虚拟机运行的一些选项标记*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetFlags(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_VM_MAIN_ARG_FLAGS flag);

/*API接口，设置虚拟机一些可选的操作句柄，例如PRINT指令对应的用户自定义输出函数等*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetHandle(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_VM_FLAGS_HANDLE_TYPE handleType,ZL_EXP_VOID * handle);

/*API接口，将用户自定义的参数压入虚拟栈中*/
ZL_EXPORT ZL_EXP_INT zenglApi_Push(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_MOD_FUN_ARG_TYPE type,ZL_EXP_CHAR * arg_str,ZL_EXP_LONG arg_integer,ZL_EXP_DOUBLE arg_floatnum);

/*API接口，调用脚本中的某函数*/
ZL_EXPORT ZL_EXP_INT zenglApi_Call(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * script_file,ZL_EXP_CHAR * function_name,ZL_EXP_CHAR * class_name);

/*API接口，通过此接口获取某个变量值的字符串格式*/
ZL_EXPORT ZL_EXP_CHAR * zenglApi_GetValueAsString(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * valueName);

/*API接口，通过此接口获取某个变量值的整数格式*/
ZL_EXPORT ZL_EXP_INT zenglApi_GetValueAsInt(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * valueName,ZL_EXP_LONG * retValue);

/*API接口，通过此接口获取某个变量值的浮点数格式*/
ZL_EXPORT ZL_EXP_INT zenglApi_GetValueAsDouble(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * valueName,ZL_EXP_DOUBLE * retValue);

/*API接口，用户可以通过此接口定义模块初始化函数，脚本中使用use指令时就会调用该函数来初始化模块里的各模块函数*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetModInitHandle(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * moduleName,ZL_EXP_VOID * modInitFun);

/*API接口，用户通过此接口可以自定义某模块中的函数处理句柄*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetModFunHandle(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT moduleID,ZL_EXP_CHAR * functionName,ZL_EXP_VOID * funHandle);

/*API接口，获取脚本中模块函数调用时的参数信息
  argnum 为参数位置，从1开始，1代表第一个参数，2代表第二个参数，以此类推*/
ZL_EXPORT ZL_EXP_INT zenglApi_GetFunArg(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argnum,ZENGL_EXPORT_MOD_FUN_ARG * retval);

/*API接口，退出当前脚本*/
ZL_EXPORT ZL_EXP_INT zenglApi_Exit(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * errorStr, ...);

/*API接口，用于在用户自定义的模块函数中设置出错信息，然后设置虚拟机停止执行，比zenglApi_Exit好的地方在于，不会长跳转直接结束，而是返回由用户决定退出的时机，有效防止外部C++调用出现内存泄漏或访问异常*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetErrThenStop(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * errorStr, ...);

/*设置模块函数的返回值*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetRetVal(ZL_EXP_VOID * VM_ARG,
										 ZENGL_EXPORT_MOD_FUN_ARG_TYPE type,ZL_EXP_CHAR * arg_str,ZL_EXP_LONG arg_integer,ZL_EXP_DOUBLE arg_floatnum);

/*API接口，将返回值设为内存块*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetRetValAsMemBlock(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_MEMBLOCK * memblock);

/*根据size参数创建足够容纳size个元素的内存块(内存块大小不一定等于size，只是大于等于size)，数组，类变量都属于内存块*/
ZL_EXPORT ZL_EXP_INT zenglApi_CreateMemBlock(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_MEMBLOCK * memblock,ZL_EXP_INT size);

/*设置内存块中index - 1索引对应的值，index参数为1表示数组等内存块的第一个元素，以此类推，返回0表示成功，返回-1表示retval类型错误*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetMemBlock(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_MEMBLOCK * memblock,ZL_EXP_INT index,ZENGL_EXPORT_MOD_FUN_ARG * retval);

/*获取数组等内存块中的index - 1索引处的元素，index参数为1表示数组等内存块的第一个元素，以此类推*/
ZL_EXPORT ZENGL_EXPORT_MOD_FUN_ARG zenglApi_GetMemBlock(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_MEMBLOCK * memblock,ZL_EXP_INT index);

/*获取第argnum个参数的类型等信息，argnum从1开始表示第一个参数，之前的GetFunArg函数只能获取参数的值，如果参数是引用，则直接获取引用的变量的值，
所以无法知道参数的类型信息，比如无法知道某个参数是否是引用等类型，该函数则可以获取到这些信息，如果参数不是引用类型，则这两个函数等价*/
ZL_EXPORT ZL_EXP_INT zenglApi_GetFunArgInfo(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argnum,ZENGL_EXPORT_MOD_FUN_ARG * retval);

/*设置脚本模块函数中第argnum个参数的值，argnum从1开始表示第一个参数*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetFunArg(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argnum,ZENGL_EXPORT_MOD_FUN_ARG * setval);

/*该接口为zenglApi_SetFunArg的扩展函数，当参数addr_level小于0时，zenglApi_SetFunArgEx等价于zenglApi_SetFunArg，
  当addr_level大于等于0时，就可以对不同级别的引用本身进行设置操作*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetFunArgEx(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argnum,ZENGL_EXPORT_MOD_FUN_ARG * setval,ZL_EXP_INT addr_level);

/*获取内存块的size容量等信息*/
ZL_EXPORT ZL_EXP_INT zenglApi_GetMemBlockInfo(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_MEMBLOCK * memblock,ZL_EXP_INT * mblk_size,ZL_EXP_INT * mblk_count);

/*API接口，用户可以通过此接口在中途停止脚本*/
ZL_EXPORT ZL_EXP_INT zenglApi_Stop(ZL_EXP_VOID * VM_ARG);

/*API接口，用户可以通过此接口设置一些额外数据的指针*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetExtraData(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * extraDataName,ZL_EXP_VOID * point);

/*API接口，用户可以通过此接口得到额外数据*/
ZL_EXPORT ZL_EXP_VOID * zenglApi_GetExtraData(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * extraDataName);

/*API接口，用户可以通过此接口得到或检测额外数据，此为上面的GetExtraData的扩展函数，当不存在extraDataName额外数据时，返回NULL，而不是产生错误信息退出虚拟机，
  所以也可以同时检测额外数据是否存在*/
ZL_EXPORT ZL_EXP_VOID * zenglApi_GetExtraDataEx(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * extraDataName);

/*API接口，用户通过此接口设置脚本源代码的XOR普通异或运算加密密钥*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetSourceXorKey(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * xor_key_str);

/*API接口，用户通过此接口设置脚本源代码的RC4加密密钥*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetSourceRC4Key(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * rc4_key_str,ZL_EXP_INT rc4_key_len);

/*API接口，用户通过此接口将字符串拷贝到虚拟机中，这样在C++中就可以提前将源字符串资源给手动释放掉，而拷贝到虚拟机中的新分配的资源则会在结束时自动释放掉，
  防止内存泄漏*/
ZL_EXPORT ZL_EXP_CHAR * zenglApi_AllocMemForString(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * src_str);

/*API接口，用户通过此接口在虚拟机中分配一段内存空间*/
ZL_EXPORT ZL_EXP_VOID * zenglApi_AllocMem(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT size);

/*API接口，将AllocMem分配的资源手动释放掉，防止资源越滚越大*/
ZL_EXPORT ZL_EXP_INT zenglApi_FreeMem(ZL_EXP_VOID * VM_ARG,ZL_EXP_VOID * ptr);

/*API接口，将ptr指针的大小调整为size尺寸*/
ZL_EXPORT ZL_EXP_VOID * zenglApi_ReAllocMem(ZL_EXP_VOID * VM_ARG,ZL_EXP_VOID * ptr,ZL_EXP_INT size);

/*API接口，由fileName构建相对于当前脚本的完整路径信息，生成的路径信息存放到用户提供的destPathBuffer缓冲区域*/
ZL_EXPORT ZL_EXP_INT zenglApi_makePathFileName(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * fileName,ZL_EXP_CHAR * destPathBuffer,ZL_EXP_INT bufferSize);

/*API接口，获取当前运行模块函数的用户自定义名称*/
ZL_EXPORT ZL_EXP_INT zenglApi_GetModFunName(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR ** modfun_name);

/*API接口，调试接口*/
ZL_EXPORT ZL_EXP_INT zenglApi_Debug(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * debug_str);

/*API接口，获取调试寄存器里的调试结果*/
ZL_EXPORT ZL_EXP_INT zenglApi_GetDebug(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_MOD_FUN_ARG * retval);

/*API接口，设置调试断点*/
ZL_EXPORT ZL_EXP_INT zenglApi_DebugSetBreak(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * filename,ZL_EXP_INT line,
											ZL_EXP_CHAR * condition,ZL_EXP_CHAR * log,ZL_EXP_INT count,ZL_EXP_BOOL disabled);

/*API接口，设置调试断点的扩展函数，直接根据指令PC值来进行设置*/
ZL_EXPORT ZL_EXP_INT zenglApi_DebugSetBreakEx(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT pc,ZL_EXP_CHAR * condition,ZL_EXP_CHAR * log,ZL_EXP_INT count,ZL_EXP_BOOL disabled);

/*API接口，获取index索引对应的断点信息*/
ZL_EXPORT ZL_EXP_INT zenglApi_DebugGetBreak(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT index,ZL_EXP_CHAR ** filename,ZL_EXP_INT * line,
											ZL_EXP_CHAR ** condition,ZL_EXP_CHAR ** log,ZL_EXP_INT * count,ZL_EXP_BOOL * disabled,ZL_EXP_INT * pc);

/*API接口，删除index索引对应的断点*/
ZL_EXPORT ZL_EXP_INT zenglApi_DebugDelBreak(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT index);

/*API接口，设置调试断点触发时调用的用户自定义函数*/
ZL_EXPORT ZL_EXP_INT zenglApi_DebugSetBreakHandle(ZL_EXP_VOID * VM_ARG,ZL_EXP_VOID * handle,ZL_EXP_VOID * conditionErrorHandle,ZL_EXP_BOOL break_start,ZL_EXP_BOOL OutputDebugInfo);

/*API接口，设置单步中断，isStepIn参数不为0则为单步步入，否则为单步步过*/
ZL_EXPORT ZL_EXP_INT zenglApi_DebugSetSingleBreak(ZL_EXP_VOID * VM_ARG,ZL_EXP_BOOL isStepIn);

/*API接口，获取脚本函数的堆栈调用信息*/
ZL_EXPORT ZL_EXP_INT zenglApi_DebugGetTrace(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT * argArg,ZL_EXP_INT * argLOC,ZL_EXP_INT * argPC,
											ZL_EXP_CHAR ** fileName,ZL_EXP_INT * line,ZL_EXP_CHAR ** className,ZL_EXP_CHAR ** funcName);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************************
		下面是虚拟机为用户提供的内建模块函数的声明
********************************************************************************/

/*array内建模块函数，用于创建zengl脚本的动态数组*/
ZL_EXPORT ZL_EXP_VOID zenglApiBMF_array(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount);

/*bltExit模块函数，直接退出zengl脚本*/
ZL_EXPORT ZL_EXP_VOID zenglApiBMF_bltExit(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount);

/*bltConvToInt模块函数，将参数转为整数形式*/
ZL_EXPORT ZL_EXP_VOID zenglApiBMF_bltConvToInt(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount);

/*	bltIntToStr模块函数：
	将整数转为字符串的形式，例如bltIntToStr(25,5,'0')那么得到的结果就是字符串'00025'
	因为第二个参数5是指总宽度，第一个参数25的宽度只有2，小于5，
	所以左边会以第三个参数'0'补齐。
	如果不想使用补齐，可以用bltIntToStr(25,0,'0')这样的格式，将第二个总宽度参数设为0，就不会应用补齐功能
*/
ZL_EXPORT ZL_EXP_VOID zenglApiBMF_bltIntToStr(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount);

/*bltRandom模块函数，产生随机数*/
ZL_EXPORT ZL_EXP_VOID zenglApiBMF_bltRandom(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount);

/*unset模块函数，将所有参数所引用的变量或数组元素或类成员等重置为未初始化状态
  未初始化状态在很多场合可以产生和整数0一样的效果，该模块函数最主要的是可以用来重置引用类型的变量*/
ZL_EXPORT ZL_EXP_VOID zenglApiBMF_unset(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount);

/********************************************************************************
		上面是虚拟机为用户提供的内建模块函数的声明
********************************************************************************/

#endif/* _ZENGL_EXPORT_FUNCTIONS_H_ */
