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

#define ZL_EXP_MAJOR_VERSION 1 //zengl���汾��
#define ZL_EXP_MINOR_VERSION 8 //zengl�Ӱ汾��
#define ZL_EXP_REVISION 3      //zengl�����汾��
#define ZL_EXP_VOID void //�����Զ���ĺ�������void , char֮���C��׼���ͣ������Ժ��ͳһ�������⼸�����ͺ�Ҳ������typedef������
#ifdef ZL_EXP_OS_IN_ARM_GCC
	#define ZL_EXP_CHAR signed char //ʹ��signed��ʾ�з��ŵ���˼����ΪARM GCC��charĬ����unsigned��(Ƕ��ʽ����������ܶ����⣡)�������б�Ҫ������ָ����signed
#else
	#define ZL_EXP_CHAR char
#endif
#define ZL_EXP_UCHAR unsigned char
#define ZL_EXP_BYTE unsigned char
#define ZL_EXP_INT int
#define ZL_EXP_LONG long
#define ZL_EXP_DOUBLE double
#define ZL_EXP_NULL 0 //ָ��Ϊ0�ĺ궨��
#define ZL_EXP_FALSE 0 //�߼���
#define ZL_EXP_TRUE 1 //�߼���
#define ZL_EXP_API_CACHE_SIGNER 0x43434C5A // ����ǩ���������ж��Ƿ�����Ч�Ļ�������
typedef unsigned char ZL_EXP_BOOL; //����bool����

typedef ZL_EXP_VOID (* ZL_VM_API_MODS_INIT)(ZL_EXP_VOID * VM_ARG); //ȫ��ģ���ʼ������������run��������ڴ�ִ��
typedef ZL_EXP_VOID (* ZL_VM_API_MOD_INIT_FUNC)(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT moduleID); //����ģ���ʼ��������useָ��ִ��ʱ����øú���
typedef ZL_EXP_VOID (* ZL_VM_API_MOD_FUN_HANDLE)(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount); //�ű��е�ģ�麯����ִ��ʱ���õ��û��Զ��庯��
typedef ZL_EXP_INT (* ZL_VM_API_INFO_FUN_HANDLE)(ZL_EXP_CHAR * infoStrPtr, ZL_EXP_INT infoStrCount, ZL_EXP_VOID * VM_ARG); //һЩ������Ϣ��printָ��ȵ��õ��û��Զ���ĺ���ָ��
typedef ZL_EXP_INT (* ZL_VM_API_DEBUG_BREAK_HANDLE)(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * filename,ZL_EXP_INT line,ZL_EXP_INT breakIndex,ZL_EXP_CHAR * log); //�û��Զ���Ķϵ���Ժ���
typedef ZL_EXP_INT (* ZL_VM_API_DEBUG_CON_ERROR_HANDLE)(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * filename,ZL_EXP_INT line,ZL_EXP_INT breakIndex,ZL_EXP_CHAR * error); //�û��Զ���������ϵ�����������ʱ�ĵ��ú���

typedef enum _ZENGL_EXPORT_VM_MAIN_ARG_FLAGS{
	ZL_EXP_CP_AF_IN_DEBUG_MODE = 0x1,		//�����������ڴ��е��﷨�����ű����Դ���Թ����������Ե���;
	ZL_EXP_CP_AF_OUTPUT_DEBUG_INFO = 0x2,	//��ӡ����������ķ��ű�ȵ�����Ϣ
}ZENGL_EXPORT_VM_MAIN_ARG_FLAGS;

typedef enum _ZENGL_EXPORT_VM_FLAGS_HANDLE_TYPE{
	ZL_EXP_VFLAG_HANDLE_NONE,			//Ĭ�ϳ�ʼֵ
	ZL_EXP_VFLAG_HANDLE_COMPILE_INFO,	//userdef_info�û��Զ������ʾ��ͨ��Ϣ�ĺ���
	ZL_EXP_VFLAG_HANDLE_COMPILE_ERROR,	//userdef_compile_error�û��Զ������ʾ������Ϣ�ĺ�����
	ZL_EXP_VFLAG_HANDLE_RUN_INFO,		//userdef_run_info�û��Զ���Ľ���������ʾ��ͨ��Ϣ�ĺ���
	ZL_EXP_VFLAG_HANDLE_RUN_PRINT,		//userdef_run_print����ʱ��PRINTָ���Ӧ���û��Զ��庯�����
	ZL_EXP_VFLAG_HANDLE_RUN_ERROR,		//userdef_run_error�û��Զ���Ľ���������ʾ������Ϣ�ĺ�����
	ZL_EXP_VFLAG_HANDLE_MODULE_INIT,	//userdef_module_init�û��Զ����ģ���ʼ������
}ZENGL_EXPORT_VM_FLAGS_HANDLE_TYPE;

typedef enum _ZENGL_EXPORT_MOD_FUN_ARG_TYPE{
	ZL_EXP_FAT_NONE,
	ZL_EXP_FAT_INT,
	ZL_EXP_FAT_FLOAT,
	ZL_EXP_FAT_STR,
	ZL_EXP_FAT_MEMBLOCK, //�ڴ������
	ZL_EXP_FAT_ADDR,	 //ȫ�ֱ���������
	ZL_EXP_FAT_ADDR_LOC, //�ֲ�����������
	ZL_EXP_FAT_ADDR_MEMBLK,//�ڴ�������
	ZL_EXP_FAT_INVALID,	 //��Ч�Ĳ�������,��������ָʾAPI����
}ZENGL_EXPORT_MOD_FUN_ARG_TYPE; //ͨ��API��ȡ�Ĳ�������

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
	ZL_EXP_VOID * ptr; //�ڴ��ָ��
	ZL_EXP_INT index;//�ڴ��ָ�����ڴ���е�����
}ZENGL_EXPORT_MEMBLOCK; //���飬��������ڴ��

typedef struct _ZENGL_EXPORT_ADDR{
	ZL_EXP_INT index;
	ZENGL_EXPORT_MEMBLOCK memblock;
}ZENGL_EXPORT_ADDR; //ȫ�ֱ������ֲ��������ڴ�������

typedef struct _ZENGL_EXPORT_MOD_FUN_ARG{
	ZENGL_EXPORT_MOD_FUN_ARG_TYPE type;
	struct{
		ZL_EXP_LONG integer; //���������õĲ���Ϊ����ʱ
		ZL_EXP_DOUBLE floatnum; //������
		ZL_EXP_CHAR * str; //�ַ���
		ZENGL_EXPORT_MEMBLOCK memblock; //�ڴ������
		ZENGL_EXPORT_ADDR addr; //��������
	}val;
}ZENGL_EXPORT_MOD_FUN_ARG; //ͨ��API��ȡ�Ĳ����ṹ�嶨��

typedef struct _ZENGL_EXPORT_VM_MAIN_ARGS{
	ZL_EXP_VOID * userdef_info; //�û��Զ������ʾ��ͨ��Ϣ�ĺ������û������Զ�����Ϣ�Ĵ�ӡ�������ʽ��
	ZL_EXP_VOID * userdef_compile_error; //�û��Զ������ʾ������Ϣ�ĺ�����
	ZL_EXP_VOID * userdef_run_info; //�û��Զ���Ľ���������ʾ��ͨ��Ϣ�ĺ������û������Զ�����Ϣ�Ĵ�ӡ�������ʽ��
	ZL_EXP_VOID * userdef_run_print; //�û��Զ���Ľ�����PRINT֮���ָ���Ӧ�Ĵ�ӡ�����ʽ��
	ZL_EXP_VOID * userdef_run_error; //�û��Զ���Ľ���������ʾ������Ϣ�ĺ�����
	ZL_EXP_VOID * userdef_module_init; //�û��Զ����ȫ��ģ���ʼ����������run��������ڴ��ᱻ����
	ZL_EXP_INT flags; //�û��Զ����һЩ���������������ѡ��
}ZENGL_EXPORT_VM_MAIN_ARGS;

typedef struct _ZENGL_EXPORT_API_CACHE_TYPE {
	ZL_EXP_INT signer; // ���û���ǩ���������ж��Ƿ�����Ч�Ļ�������
	ZL_EXP_INT mempoolRealSize; // ��Ҫ�Ž�������ڴ�ص�ʵ�ʴ�С
	ZL_EXP_INT mempoolOffset; // �ڴ���ڻ����еĵ�ƫ��ֵ
	ZL_EXP_INT totalSize; // �������ݵ��ܴ�С
	ZL_EXP_INT filenames_count; // inc���ص��ļ�����
	ZL_EXP_CHAR ** filenames; // inc���ص��ļ�����̬����
}ZENGL_EXPORT_API_CACHE_TYPE; // �������ݵ�ͷ���ṹ

/*����ΪzenglApi�ӿڵ�������Ŀǰһ����50��API�ӿں���(�������ײ���������Щ�ڽ�ģ�麯��)*/

/*ͨ��zenglApi_Load���Լ��ز�ִ��script_file�ű�*/
ZL_EXPORT ZL_EXP_INT zenglApi_Load(ZL_EXP_CHAR * script_file,ZENGL_EXPORT_VM_MAIN_ARGS * vm_main_args);

/*�����������µ������ָ�룬��ָ�����ͨ��zenglApi_Close�����ͷţ�����ᷢ���ڴ�й©*/
ZL_EXPORT ZL_EXP_VOID * zenglApi_Open();

/*ʹ��zenglApi_Open�򿪵������������script_file��Ӧ�Ľű�*/
ZL_EXPORT ZL_EXP_INT zenglApi_Run(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * script_file);

/*
ʹ��zenglApi_Open�򿪵������������script_strָ����ַ����ű���lenΪ�ַ����ű�����Ч���ȣ�script_nameΪ���ַ����ű����Զ������֣�
script_name�������ڵ�����Ϣ�����ͬʱ�����Ը�script_name����һ��·��ǰ׺������"script/runstr"�������ַ����ű��ڲ�inc���������ű�ʱ�ͻᰴ��"script/"·��������
*/
ZL_EXPORT ZL_EXP_INT zenglApi_RunStr(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * script_str,ZL_EXP_INT len,ZL_EXP_CHAR * script_name);

/*�ر�zenglApi_Open�򿪵������ָ�룬���ͷ������ռ�õ�ϵͳ��Դ*/
ZL_EXPORT ZL_EXP_INT zenglApi_Close(ZL_EXP_VOID * VM_ARG);

/*API�ӿڣ����ò�����Ӧ�������*/
ZL_EXPORT ZL_EXP_VOID * zenglApi_Reset(ZL_EXP_VOID * VM_ARG);

/*API�ӿڣ������������֮ǰ�ı�����Դ�������Ϳ���ֱ��ִ��֮ǰ�Ѿ�����õ�ָ�����
 ����isClear��Ϊ0��ִ�������ڴ��������������-1��ʾ��������0��ʾʲô��û��������1��ʾ����ִ��*/
ZL_EXPORT ZL_EXP_INT zenglApi_ReUse(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT isClear);

/*API�ӿڣ����ر�������ʱ�ĳ�����Ϣ*/
ZL_EXPORT ZL_EXP_CHAR * zenglApi_GetErrorString(ZL_EXP_VOID * VM_ARG);

/*API�ӿڣ�������������е�һЩѡ����*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetFlags(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_VM_MAIN_ARG_FLAGS flag);

/*API�ӿڣ����������һЩ��ѡ�Ĳ������������PRINTָ���Ӧ���û��Զ������������*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetHandle(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_VM_FLAGS_HANDLE_TYPE handleType,ZL_EXP_VOID * handle);

/*API�ӿڣ����û��Զ���Ĳ���ѹ������ջ��*/
ZL_EXPORT ZL_EXP_INT zenglApi_Push(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_MOD_FUN_ARG_TYPE type,ZL_EXP_CHAR * arg_str,ZL_EXP_LONG arg_integer,ZL_EXP_DOUBLE arg_floatnum);

/*API�ӿڣ����ýű��е�ĳ����*/
ZL_EXPORT ZL_EXP_INT zenglApi_Call(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * script_file,ZL_EXP_CHAR * function_name,ZL_EXP_CHAR * class_name);

/*API�ӿڣ�ͨ���˽ӿڻ�ȡĳ������ֵ���ַ�����ʽ*/
ZL_EXPORT ZL_EXP_CHAR * zenglApi_GetValueAsString(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * valueName);

/*API�ӿڣ�ͨ���˽ӿڻ�ȡĳ������ֵ��������ʽ*/
ZL_EXPORT ZL_EXP_INT zenglApi_GetValueAsInt(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * valueName,ZL_EXP_LONG * retValue);

/*API�ӿڣ�ͨ���˽ӿڻ�ȡĳ������ֵ�ĸ�������ʽ*/
ZL_EXPORT ZL_EXP_INT zenglApi_GetValueAsDouble(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * valueName,ZL_EXP_DOUBLE * retValue);

/*API�ӿڣ��û�����ͨ���˽ӿڶ���ģ���ʼ���������ű���ʹ��useָ��ʱ�ͻ���øú�������ʼ��ģ����ĸ�ģ�麯��*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetModInitHandle(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * moduleName,ZL_EXP_VOID * modInitFun);

/*API�ӿڣ��û�ͨ���˽ӿڿ����Զ���ĳģ���еĺ���������*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetModFunHandle(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT moduleID,ZL_EXP_CHAR * functionName,ZL_EXP_VOID * funHandle);

/*API�ӿڣ���ȡ�ű���ģ�麯������ʱ�Ĳ�����Ϣ
  argnum Ϊ����λ�ã���1��ʼ��1�����һ��������2����ڶ����������Դ�����*/
ZL_EXPORT ZL_EXP_INT zenglApi_GetFunArg(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argnum,ZENGL_EXPORT_MOD_FUN_ARG * retval);

/*API�ӿڣ��˳���ǰ�ű�*/
ZL_EXPORT ZL_EXP_INT zenglApi_Exit(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * errorStr, ...);

/*API�ӿڣ��������û��Զ����ģ�麯�������ó�����Ϣ��Ȼ�����������ִֹͣ�У���zenglApi_Exit�õĵط����ڣ����᳤��תֱ�ӽ��������Ƿ������û������˳���ʱ������Ч��ֹ�ⲿC++���ó����ڴ�й©������쳣*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetErrThenStop(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * errorStr, ...);

/*����ģ�麯���ķ���ֵ*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetRetVal(ZL_EXP_VOID * VM_ARG,
										 ZENGL_EXPORT_MOD_FUN_ARG_TYPE type,ZL_EXP_CHAR * arg_str,ZL_EXP_LONG arg_integer,ZL_EXP_DOUBLE arg_floatnum);

/*API�ӿڣ�������ֵ��Ϊ�ڴ��*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetRetValAsMemBlock(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_MEMBLOCK * memblock);

/*����size���������㹻����size��Ԫ�ص��ڴ��(�ڴ���С��һ������size��ֻ�Ǵ��ڵ���size)�����飬������������ڴ��*/
ZL_EXPORT ZL_EXP_INT zenglApi_CreateMemBlock(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_MEMBLOCK * memblock,ZL_EXP_INT size);

/*API�ӿڣ�����������ڴ������ü���ֵ(���add_refcount�Ǹ��������Ǽ������ü���ֵ)������0��ʾ�ɹ�������-1��ʾʧ��*/
ZL_EXPORT ZL_EXP_INT zenglApi_AddMemBlockRefCount(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_MEMBLOCK * memblock,ZL_EXP_INT add_refcount);

/*�����ڴ����index - 1������Ӧ��ֵ��index����Ϊ1��ʾ������ڴ��ĵ�һ��Ԫ�أ��Դ����ƣ�����0��ʾ�ɹ�������-1��ʾretval���ʹ���*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetMemBlock(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_MEMBLOCK * memblock,ZL_EXP_INT index,ZENGL_EXPORT_MOD_FUN_ARG * retval);

/*ͨ���ַ���key������������ڴ��ĳ�Ա���ýӿڻ��Ƚ��ַ���תΪ��Ӧ������ֵ���ٵ���SetMemBlock�ӿ�ȥִ�о�������ù���*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetMemBlockByHashKey(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_MEMBLOCK * memblock,ZL_EXP_CHAR * key,ZENGL_EXPORT_MOD_FUN_ARG * retval);

/*��ȡ������ڴ���е�index - 1��������Ԫ�أ�index����Ϊ1��ʾ������ڴ��ĵ�һ��Ԫ�أ��Դ�����*/
ZL_EXPORT ZENGL_EXPORT_MOD_FUN_ARG zenglApi_GetMemBlock(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_MEMBLOCK * memblock,ZL_EXP_INT index);

/*����key����ȡ������ڴ���еĳ�Ա���˽ӿڻ��Ƚ�keyתΪ����ֵ���ٸ�������ֵ�������zenglApi_GetMemBlock�ӿ������ض�Ӧ�ĳ�Ա*/
ZL_EXPORT ZENGL_EXPORT_MOD_FUN_ARG zenglApi_GetMemBlockByHashKey(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_MEMBLOCK * memblock,ZL_EXP_CHAR * key);

/*��ȡ��argnum�����������͵���Ϣ��argnum��1��ʼ��ʾ��һ��������֮ǰ��GetFunArg����ֻ�ܻ�ȡ������ֵ��������������ã���ֱ�ӻ�ȡ���õı�����ֵ��
�����޷�֪��������������Ϣ�������޷�֪��ĳ�������Ƿ������õ����ͣ��ú�������Ի�ȡ����Щ��Ϣ��������������������ͣ��������������ȼ�*/
ZL_EXPORT ZL_EXP_INT zenglApi_GetFunArgInfo(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argnum,ZENGL_EXPORT_MOD_FUN_ARG * retval);

/*���ýű�ģ�麯���е�argnum��������ֵ��argnum��1��ʼ��ʾ��һ������*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetFunArg(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argnum,ZENGL_EXPORT_MOD_FUN_ARG * setval);

/*�ýӿ�ΪzenglApi_SetFunArg����չ������������addr_levelС��0ʱ��zenglApi_SetFunArgEx�ȼ���zenglApi_SetFunArg��
  ��addr_level���ڵ���0ʱ���Ϳ��ԶԲ�ͬ��������ñ���������ò���*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetFunArgEx(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argnum,ZENGL_EXPORT_MOD_FUN_ARG * setval,ZL_EXP_INT addr_level);

/*��ȡ�ڴ���size��������Ϣ*/
ZL_EXPORT ZL_EXP_INT zenglApi_GetMemBlockInfo(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_MEMBLOCK * memblock,ZL_EXP_INT * mblk_size,ZL_EXP_INT * mblk_count);

/*��ȡ�ڴ���з�NONE��Ա������*/
ZL_EXPORT ZL_EXP_INT zenglApi_GetMemBlockNNCount(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_MEMBLOCK * memblock);

/*��������ֵ���ӹ�ϣ���л�ȡ��Ӧ���ַ���key*/
ZL_EXPORT ZL_EXP_INT zenglApi_GetMemBlockHashKey(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_MEMBLOCK * memblock, ZL_EXP_INT memblock_index, ZL_EXP_CHAR ** key);

/*API�ӿڣ��û�����ͨ���˽ӿ�����;ֹͣ�ű�*/
ZL_EXPORT ZL_EXP_INT zenglApi_Stop(ZL_EXP_VOID * VM_ARG);

/*API�ӿڣ��û�����ͨ���˽ӿ�����һЩ�������ݵ�ָ��*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetExtraData(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * extraDataName,ZL_EXP_VOID * point);

/*API�ӿڣ��û�����ͨ���˽ӿڵõ���������*/
ZL_EXPORT ZL_EXP_VOID * zenglApi_GetExtraData(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * extraDataName);

/*API�ӿڣ��û�����ͨ���˽ӿڵõ�����������ݣ���Ϊ�����GetExtraData����չ��������������extraDataName��������ʱ������NULL�������ǲ���������Ϣ�˳��������
  ����Ҳ����ͬʱ�����������Ƿ����*/
ZL_EXPORT ZL_EXP_VOID * zenglApi_GetExtraDataEx(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * extraDataName);

/*API�ӿڣ��û�ͨ���˽ӿ����ýű�Դ�����XOR��ͨ������������Կ*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetSourceXorKey(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * xor_key_str);

/*API�ӿڣ��û�ͨ���˽ӿ����ýű�Դ�����RC4������Կ*/
ZL_EXPORT ZL_EXP_INT zenglApi_SetSourceRC4Key(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * rc4_key_str,ZL_EXP_INT rc4_key_len);

/*API�ӿڣ��û�ͨ���˽ӿڽ��ַ���������������У�������C++�оͿ�����ǰ��Դ�ַ�����Դ���ֶ��ͷŵ�����������������е��·������Դ����ڽ���ʱ�Զ��ͷŵ���
  ��ֹ�ڴ�й©*/
ZL_EXPORT ZL_EXP_CHAR * zenglApi_AllocMemForString(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * src_str);

/*API�ӿڣ��û�ͨ���˽ӿ���������з���һ���ڴ�ռ�*/
ZL_EXPORT ZL_EXP_VOID * zenglApi_AllocMem(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT size);

/*API�ӿڣ���AllocMem�������Դ�ֶ��ͷŵ�����ֹ��ԴԽ��Խ��*/
ZL_EXPORT ZL_EXP_INT zenglApi_FreeMem(ZL_EXP_VOID * VM_ARG,ZL_EXP_VOID * ptr);

/*API�ӿڣ���ptrָ��Ĵ�С����Ϊsize�ߴ�*/
ZL_EXPORT ZL_EXP_VOID * zenglApi_ReAllocMem(ZL_EXP_VOID * VM_ARG,ZL_EXP_VOID * ptr,ZL_EXP_INT size);

/*API�ӿڣ���fileName��������ڵ�ǰ�ű�������·����Ϣ�����ɵ�·����Ϣ��ŵ��û��ṩ��destPathBuffer��������*/
ZL_EXPORT ZL_EXP_INT zenglApi_makePathFileName(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * fileName,ZL_EXP_CHAR * destPathBuffer,ZL_EXP_INT bufferSize);

/*API�ӿڣ���ȡ��ǰ����ģ�麯�����û��Զ�������*/
ZL_EXPORT ZL_EXP_INT zenglApi_GetModFunName(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR ** modfun_name);

/*API�ӿڣ����Խӿ�*/
ZL_EXPORT ZL_EXP_INT zenglApi_Debug(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * debug_str);

/*API�ӿڣ���ȡ���ԼĴ�����ĵ��Խ��*/
ZL_EXPORT ZL_EXP_INT zenglApi_GetDebug(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_MOD_FUN_ARG * retval);

/*API�ӿڣ����õ��Զϵ�*/
ZL_EXPORT ZL_EXP_INT zenglApi_DebugSetBreak(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * filename,ZL_EXP_INT line,
											ZL_EXP_CHAR * condition,ZL_EXP_CHAR * log,ZL_EXP_INT count,ZL_EXP_BOOL disabled);

/*API�ӿڣ����õ��Զϵ����չ������ֱ�Ӹ���ָ��PCֵ����������*/
ZL_EXPORT ZL_EXP_INT zenglApi_DebugSetBreakEx(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT pc,ZL_EXP_CHAR * condition,ZL_EXP_CHAR * log,ZL_EXP_INT count,ZL_EXP_BOOL disabled);

/*API�ӿڣ���ȡindex������Ӧ�Ķϵ���Ϣ*/
ZL_EXPORT ZL_EXP_INT zenglApi_DebugGetBreak(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT index,ZL_EXP_CHAR ** filename,ZL_EXP_INT * line,
											ZL_EXP_CHAR ** condition,ZL_EXP_CHAR ** log,ZL_EXP_INT * count,ZL_EXP_BOOL * disabled,ZL_EXP_INT * pc);

/*API�ӿڣ�ɾ��index������Ӧ�Ķϵ�*/
ZL_EXPORT ZL_EXP_INT zenglApi_DebugDelBreak(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT index);

/*API�ӿڣ����õ��Զϵ㴥��ʱ���õ��û��Զ��庯��*/
ZL_EXPORT ZL_EXP_INT zenglApi_DebugSetBreakHandle(ZL_EXP_VOID * VM_ARG,ZL_EXP_VOID * handle,ZL_EXP_VOID * conditionErrorHandle,ZL_EXP_BOOL break_start,ZL_EXP_BOOL OutputDebugInfo);

/*API�ӿڣ����õ����жϣ�isStepIn������Ϊ0��Ϊ�������룬����Ϊ��������*/
ZL_EXPORT ZL_EXP_INT zenglApi_DebugSetSingleBreak(ZL_EXP_VOID * VM_ARG,ZL_EXP_BOOL isStepIn);

/*API�ӿڣ���ȡ�ű������Ķ�ջ������Ϣ*/
ZL_EXPORT ZL_EXP_INT zenglApi_DebugGetTrace(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT * argArg,ZL_EXP_INT * argLOC,ZL_EXP_INT * argPC,
											ZL_EXP_CHAR ** fileName,ZL_EXP_INT * line,ZL_EXP_CHAR ** className,ZL_EXP_CHAR ** funcName);

/**
 * API�ӿڣ����������ͽ���������Ҫ���ڴ����ݻ���������������ڴ����ݿ��Դ洢���ļ����߱�ĵط���
 * ֮��������û����������ڴ���������������̣�ֱ��ִ��������ָ������������ڴ�����ֻ�������ڵ�ǰ����
 */
ZL_EXPORT ZL_EXP_INT zenglApi_CacheMemData(ZL_EXP_VOID * VM_ARG, ZL_EXP_VOID ** cachePoint, ZL_EXP_INT * cacheSize);

/* �����û������ݣ��Ϳ�������������� */
ZL_EXPORT ZL_EXP_INT zenglApi_ReUseCacheMemData(ZL_EXP_VOID * VM_ARG, ZL_EXP_VOID * cachePoint, ZL_EXP_INT cacheSize);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************************
		�����������Ϊ�û��ṩ���ڽ�ģ�麯��������
********************************************************************************/

/*array�ڽ�ģ�麯�������ڴ���zengl�ű��Ķ�̬����*/
ZL_EXPORT ZL_EXP_VOID zenglApiBMF_array(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount);

/*bltExitģ�麯����ֱ���˳�zengl�ű�*/
ZL_EXPORT ZL_EXP_VOID zenglApiBMF_bltExit(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount);

/*bltConvToIntģ�麯����������תΪ������ʽ*/
ZL_EXPORT ZL_EXP_VOID zenglApiBMF_bltConvToInt(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount);

/*	bltIntToStrģ�麯����
	������תΪ�ַ�������ʽ������bltIntToStr(25,5,'0')��ô�õ��Ľ�������ַ���'00025'
	��Ϊ�ڶ�������5��ָ�ܿ�ȣ���һ������25�Ŀ��ֻ��2��С��5��
	������߻��Ե���������'0'���롣
	�������ʹ�ò��룬������bltIntToStr(25,0,'0')�����ĸ�ʽ�����ڶ����ܿ�Ȳ�����Ϊ0���Ͳ���Ӧ�ò��빦��
*/
ZL_EXPORT ZL_EXP_VOID zenglApiBMF_bltIntToStr(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount);

/*bltRandomģ�麯�������������*/
ZL_EXPORT ZL_EXP_VOID zenglApiBMF_bltRandom(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount);

/*unsetģ�麯���������в��������õı���������Ԫ�ػ����Ա������Ϊδ��ʼ��״̬
  δ��ʼ��״̬�ںܶೡ�Ͽ��Բ���������0һ����Ч������ģ�麯������Ҫ���ǿ������������������͵ı���*/
ZL_EXPORT ZL_EXP_VOID zenglApiBMF_unset(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount);

/********************************************************************************
		�����������Ϊ�û��ṩ���ڽ�ģ�麯��������
********************************************************************************/

#endif/* _ZENGL_EXPORT_FUNCTIONS_H_ */
