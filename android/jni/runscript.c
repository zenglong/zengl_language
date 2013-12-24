/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <string.h>
#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#define ZL_EXP_OS_IN_ARM_GCC
#include "zengl_exportfuns.h"

typedef struct _strPrintEnv{
	JNIEnv* env;
	jobject obj;
	jmethodID functionID;
	FILE * debuglog;
}strPrintEnv;

void java_printcall(strPrintEnv * myenv ,ZL_EXP_CHAR * arg)
{
	JNIEnv* env = myenv->env;
	jobject obj = myenv->obj;
	jmethodID functionID = myenv->functionID;
	(*env)->CallVoidMethod(env,obj,functionID,(*env)->NewStringUTF(env,arg));
}

ZL_EXP_INT run_print(ZL_EXP_CHAR * infoStrPtr, ZL_EXP_INT infoStrCount, ZL_EXP_VOID * VM_ARG)
{
	strPrintEnv * myenv = (strPrintEnv *)zenglApi_GetExtraData(VM_ARG,"extra");
	java_printcall(myenv,infoStrPtr);
	java_printcall(myenv,"\n");
	return 0;
}

ZL_EXP_INT debug_compile_info(ZL_EXP_CHAR * infoStrPtr, ZL_EXP_INT infoStrCount,ZL_EXP_VOID * VM_ARG)
{
	strPrintEnv * myenv = (strPrintEnv *)zenglApi_GetExtraData(VM_ARG,"extra");
	fprintf(myenv->debuglog,"%s",infoStrPtr);
	return 0;
}

ZL_EXP_INT debug_run_info(ZL_EXP_CHAR * infoStrPtr, ZL_EXP_INT infoStrCount,ZL_EXP_VOID * VM_ARG)
{
	strPrintEnv * myenv = (strPrintEnv *)zenglApi_GetExtraData(VM_ARG,"extra");
	fprintf(myenv->debuglog,"%s",infoStrPtr);
	return 0;
}

/*builtin模块函数*/
ZL_EXP_VOID builtin_printf(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount)
{
	ZENGL_EXPORT_MOD_FUN_ARG arg = {ZL_EXP_FAT_NONE,{0}};
	ZL_EXP_INT i;
	strPrintEnv * myenv = (strPrintEnv *)zenglApi_GetExtraData(VM_ARG,"extra");
	ZL_EXP_CHAR tmp[50];
	if(argcount < 1)
		zenglApi_Exit(VM_ARG,"printf函数的参数不可以为空");
	for(i=1;i<=argcount;i++)
	{
		zenglApi_GetFunArg(VM_ARG,i,&arg);
		switch(arg.type)
		{
		case ZL_EXP_FAT_INT:
			sprintf(tmp,"%d",arg.val.integer);
			java_printcall(myenv,tmp);
			break;
		case ZL_EXP_FAT_FLOAT:
			sprintf(tmp,"%.16g",arg.val.floatnum);
			java_printcall(myenv,tmp);
			break;
		case ZL_EXP_FAT_STR:
			java_printcall(myenv,arg.val.str);
			break;
		default:
			zenglApi_Exit(VM_ARG,"printf函数第%d个参数类型无效，目前只支持字符串，整数，浮点数类型的参数",i);
			break;
		}
	}
	return;
}

/*递归打印出数组信息*/
ZL_EXP_VOID print_array(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_MEMBLOCK memblock,ZL_EXP_INT recur_count,strPrintEnv * myenv)
{
	ZL_EXP_INT size,i,j;
	ZL_EXP_CHAR tmp[60];
	ZENGL_EXPORT_MOD_FUN_ARG mblk_val = {ZL_EXP_FAT_NONE,{0}};
	zenglApi_GetMemBlockInfo(VM_ARG,&memblock,&size,ZL_EXP_NULL);
	for(i=1;i<=size;i++)
	{
		mblk_val = zenglApi_GetMemBlock(VM_ARG,&memblock,i);
		switch(mblk_val.type)
		{
		case ZL_EXP_FAT_INT:
		case ZL_EXP_FAT_FLOAT:
		case ZL_EXP_FAT_STR:
		case ZL_EXP_FAT_MEMBLOCK:
			for(j=0;j<recur_count;j++)
				java_printcall(myenv,"  ");
			break;
		}
		switch(mblk_val.type)
		{
		case ZL_EXP_FAT_INT:
			sprintf(tmp,"[%d] %d\n",i-1,mblk_val.val.integer);
			java_printcall(myenv,tmp);
			break;
		case ZL_EXP_FAT_FLOAT:
			sprintf(tmp,"[%d] %.16g\n",i-1,mblk_val.val.floatnum);
			java_printcall(myenv,tmp);
			break;
		case ZL_EXP_FAT_STR:
			sprintf(tmp,"[%d] ",i-1);
			java_printcall(myenv,tmp);
			java_printcall(myenv,mblk_val.val.str);
			break;
		case ZL_EXP_FAT_MEMBLOCK:
			sprintf(tmp,"[%d] <array or class obj type> begin:\n",i-1);
			java_printcall(myenv,tmp);
			print_array(VM_ARG,mblk_val.val.memblock,recur_count+1,myenv);
			sprintf(tmp,"[%d] <array or class obj type> end\n",i-1);
			java_printcall(myenv,tmp);
			break;
		}
	}
}

/*bltPrintArray模块函数，打印数组中的元素*/
ZL_EXP_VOID builtin_print_array(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount)
{
	ZENGL_EXPORT_MOD_FUN_ARG arg = {ZL_EXP_FAT_NONE,{0}};
	strPrintEnv * myenv = (strPrintEnv *)zenglApi_GetExtraData(VM_ARG,"extra");
	if(argcount < 1)
		zenglApi_Exit(VM_ARG,"bltPrintArray函数参数不可为空，必须指定一个数组或类对象为参数");
	zenglApi_GetFunArg(VM_ARG,1,&arg);
	if(arg.type != ZL_EXP_FAT_MEMBLOCK)
		zenglApi_Exit(VM_ARG,"bltPrintArray函数的参数不是数组或类对象");
	print_array(VM_ARG,arg.val.memblock,0,myenv);
}

/*bltGetZLVersion模块函数，获取当前zengl版本号信息的字符串形式*/
ZL_EXP_VOID builtin_get_zl_version(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount)
{
	ZL_EXP_CHAR version[20] = {0};
	sprintf(version,"%d.%d.%d",ZL_EXP_MAJOR_VERSION,ZL_EXP_MINOR_VERSION,ZL_EXP_REVISION);
	zenglApi_SetRetVal(VM_ARG,ZL_EXP_FAT_STR,version,0,0);
}

ZL_EXP_VOID builtin_module_init(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT moduleID)
{
	zenglApi_SetModFunHandle(VM_ARG,moduleID,"printf",builtin_printf);
	zenglApi_SetModFunHandle(VM_ARG,moduleID,"bltRandom",zenglApiBMF_bltRandom); //使用虚拟机zenglApi_BltModFuns.c中定义的bltRandom
	zenglApi_SetModFunHandle(VM_ARG,moduleID,"array",zenglApiBMF_array);  //使用虚拟机zenglApi_BltModFuns.c中定义的array
	zenglApi_SetModFunHandle(VM_ARG,moduleID,"unset",zenglApiBMF_unset);  //使用虚拟机zenglApi_BltModFuns.c中定义的unset
	zenglApi_SetModFunHandle(VM_ARG,moduleID,"bltExit",zenglApiBMF_bltExit);  //使用虚拟机zenglApi_BltModFuns.c中定义的bltExit
	zenglApi_SetModFunHandle(VM_ARG,moduleID,"bltConvToInt",zenglApiBMF_bltConvToInt);  //使用虚拟机zenglApi_BltModFuns.c中定义的bltConvToInt
	zenglApi_SetModFunHandle(VM_ARG,moduleID,"bltIntToStr",zenglApiBMF_bltIntToStr);  //使用虚拟机zenglApi_BltModFuns.c中定义的bltIntToStr
	zenglApi_SetModFunHandle(VM_ARG,moduleID,"bltPrintArray",builtin_print_array);
	zenglApi_SetModFunHandle(VM_ARG,moduleID,"bltGetZLVersion",builtin_get_zl_version);
}

/* This is a trivial JNI example where we use a native method
 * to return a new VM String. See the corresponding Java source
 * file located at:
 *
 *   apps/samples/hello-jni/project/src/com/example/hellojni/HelloJni.java
 */
jstring
Java_com_zengl_runscript_RunScript_RunZenglFromJNI( JNIEnv* env,
                                                  jobject obj,
                                                  jstring Path,
                                                  jstring s,
                                                  jint debuginfo)
{
	ZL_EXP_CHAR * run_str = "test='hello world i\\'m zengl!';";
	ZL_EXP_INT run_str_len = strlen(run_str);
	ZL_EXP_CHAR * teststr = ZL_EXP_NULL;
	ZL_EXP_CHAR * err = ZL_EXP_NULL;
	jstring ret = (*env)->NewStringUTF(env,""); //需要初始化为空字符串，否则返回时就容易出现 libc SIGABRT 错误
	jclass activityClass = (*env)->GetObjectClass(env,obj);
	jmethodID functionID = (*env)->GetMethodID(env,activityClass,"PrintCallBack",
												"(Ljava/lang/String;)V");
	FILE * debuglog = ZL_EXP_NULL;
	strPrintEnv myenv = {env,obj,functionID,debuglog};
	ZL_EXP_VOID * VM = ZL_EXP_NULL;
	ZL_EXP_CHAR scriptPath[100] = {0};
	ZL_EXP_CHAR * tmp = (ZL_EXP_CHAR *)((*env)->GetStringUTFChars(env, s, NULL));
	ZL_EXP_CHAR * tmpPath = (ZL_EXP_CHAR *)((*env)->GetStringUTFChars(env, Path, NULL));
	sprintf(scriptPath,"%s/zengl_debuglogs.txt",tmpPath);
	debuglog = fopen(scriptPath,"w+");
	myenv.debuglog = debuglog;
	sprintf(scriptPath,"%s/%s",tmpPath,tmp);
	VM = zenglApi_Open();
	if(debuginfo == 1)
		zenglApi_SetFlags(VM,(ZENGL_EXPORT_VM_MAIN_ARG_FLAGS)(ZL_EXP_CP_AF_IN_DEBUG_MODE | ZL_EXP_CP_AF_OUTPUT_DEBUG_INFO));
	else
		zenglApi_SetFlags(VM,(ZENGL_EXPORT_VM_MAIN_ARG_FLAGS)(ZL_EXP_CP_AF_IN_DEBUG_MODE));
	zenglApi_SetHandle(VM,ZL_EXP_VFLAG_HANDLE_COMPILE_INFO,debug_compile_info);
	zenglApi_SetHandle(VM,ZL_EXP_VFLAG_HANDLE_RUN_INFO,debug_run_info);
	zenglApi_SetHandle(VM,ZL_EXP_VFLAG_HANDLE_RUN_PRINT,run_print);
	zenglApi_SetModInitHandle(VM,"builtin",builtin_module_init);
	zenglApi_SetExtraData(VM,"extra",&myenv);
	//if(zenglApi_RunStr(VM,run_str,run_str_len,"runstr") == -1) //编译执行字符串脚本
	if(zenglApi_Run(VM,scriptPath) == -1) //编译执行zengl脚本
		err = zenglApi_GetErrorString(VM);
	if(err != ZL_EXP_NULL)
		java_printcall(&myenv,err);
	zenglApi_Close(VM);
	fclose(debuglog);
    return ret;
}
