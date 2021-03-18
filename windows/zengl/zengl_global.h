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

#include "zengl_global_header.h"

/*API�ӿڵĸ���״̬ö�ٶ���*/
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
	ZL_API_ST_DEBUG_HANDLE,
}ZENGL_API_STATES;
/*API�ӿڵĸ���״̬ö�ٶ������*/

/*token�ڵ����Ͷ���*/
typedef enum _ZENGL_TOKENTYPE{

	ZL_TK_START_NONE,		//��ʼֵ������Ӧ�κ�token
	ZL_TK_ID,				//����֮��ı�ʶ��token
	ZL_TK_RESERVE,			//�ؼ���token
	ZL_TK_NUM,				//��123֮������ֵ�token
	ZL_TK_FLOAT,			//��3.14159֮��ĸ�����
	ZL_TK_STR,				//�ַ���token
	ZL_TK_ASSIGN,			//��ֵ�����token
	ZL_TK_EQUAL,			//"=="���������token
	ZL_TK_GREAT_EQ,			//">="���ڵ��������token
	ZL_TK_GREAT,			//">"���������token
	ZL_TK_LESS_EQ,			//"<="С�ڵ��������token
	ZL_TK_LESS,				//"<"С�������token
	ZL_TK_NOT_EQ,			//"!="�����������token
	ZL_TK_REVERSE,			//"!"ȡ�������token
	ZL_TK_AND,				//"&&"�߼��������token
	ZL_TK_ADDRESS,			//"&"���������token
	ZL_TK_OR,				//"||"�߼��������token
	ZL_TK_PLUS_PLUS,		//++�����token
	ZL_TK_PLUS_ASSIGN,		//+=�����token
	ZL_TK_PLUS,				//�ӷ������token
	ZL_TK_MINIS_MINIS,		//--�����token
	ZL_TK_MINIS_ASSIGN,		//-=�����token
	ZL_TK_MINIS,			//���������token
	ZL_TK_NEGATIVE,			//���ŵ�Ŀ�����token
	ZL_TK_TIMES_ASSIGN,		//*=�����token
	ZL_TK_TIMES,			//�˷������token
	ZL_TK_DIVIDE_ASSIGN,	// "/=" ����ֵtoken
	ZL_TK_DIVIDE,			//���������token
	ZL_TK_MOD_ASSIGN,		// "%=" ȡ�ำֵ�����token
	ZL_TK_MOD,				//ȡ�������token
	ZL_TK_LBRACKET,			//������token
	ZL_TK_RBRACKET,			//������token
	ZL_TK_LMBRACKET,		//��������token
	ZL_TK_RMBRACKET,		//��������token
	ZL_TK_ENDFILE,			//�ļ�������token
	ZL_TK_SEMI,				//�ֺ�token
	ZL_TK_COMMA,			//����token
	ZL_TK_COLON,			//ð��token
	ZL_TK_QUESTION_MARK,	//�ʺ�token
	ZL_TK_DOT,				//�������
	ZL_TK_BIT_AND,			//"&"��λ��˫Ŀ�����token
	ZL_TK_BIT_AND_ASSIGN,	//&=�����token
	ZL_TK_BIT_OR,			//"|"��λ��˫Ŀ�����token
	ZL_TK_BIT_OR_ASSIGN,	//|=�����token
	ZL_TK_BIT_XOR,			//"^"��λ��������token
	ZL_TK_BIT_XOR_ASSIGN,	//^=�����token
	ZL_TK_BIT_RIGHT,		//">>"���������token
	ZL_TK_BIT_RIGHT_ASSIGN,	//">>="���Ƹ�ֵ�����token
	ZL_TK_BIT_LEFT,			//"<<"���������token
	ZL_TK_BIT_LEFT_ASSIGN,	//"<<="���Ƹ�ֵ�����token
	ZL_TK_BIT_REVERSE,		//"~"��λȡ�������token
	ZL_TK_FUNCALL,			//��������token ����max(a,b); ��ʾmax���� 
	ZL_TK_ARRAY_ITEM,		//����Ԫ����ʽ ����test[2];��ʾtest����ĵ�����Ԫ�أ���0��ʼ
	ZL_TK_CLASS_STATEMENT,	//��������䣬���� Poker x,y;����PokerΪĳ���� x,yΪ���������ı���
	ZL_TK_ERROR				//δ�����token

}ZENGL_TOKENTYPE;
/*token�ڵ����Ͷ������*/

/*��switch case��Ҫ�õ��ĸ���״̬��ö�ٶ���*/
typedef enum _ZENGL_STATES{
	ZL_ST_START,				//��ʼ״̬
	ZL_ST_DOWN,					//����״̬
	ZL_ST_INID,					//���ڱ�ʶ��ɨ��״̬
	ZL_ST_INEIGHT_HEX,			//����ɨ��˽��ƺ�ʮ�����Ƶ�״̬
	ZL_ST_INEIGHT,				//����ɨ��˽��Ƶ�״̬
	ZL_ST_INHEX,				//����ɨ��ʮ�����Ƶ�״̬
	ZL_ST_INNUM,				//��������ɨ��״̬
	ZL_ST_INFLOAT,				//���ڸ�����ɨ��״̬
	ZL_ST_INSTR,				//�����ַ���ɨ��״̬
	ZL_ST_INPLUS,				//���ڼӺ��������ɨ��״̬
	ZL_ST_INMINIS,				//���ڼ����������ɨ��״̬
	ZL_ST_INTIMES,				//���ڳ˺��������ɨ��״̬
	ZL_ST_INDIVIDE,				//���ڳ����������ɨ��״̬
	ZL_ST_INMOD,				//����ȡ���������ɨ��״̬
	ZL_ST_INASSIGN,				//���ڸ�ֵ�������ɨ��״̬
	ZL_ST_INGREAT,				//���ڴ����������ɨ��״̬
	ZL_ST_INLESS,				//����С���������ɨ��״̬
	ZL_ST_INNOT_EQ,				//���ڲ������������ɨ��״̬
	ZL_ST_INAND,				//�����߼����������ɨ��״̬
	ZL_ST_INOR,					//�����߼����������ɨ��״̬
	ZL_ST_INCOMMENT,			//����ע��ɨ��״̬
	ZL_ST_INMULTI_COMMENT,		//����ע��ɨ��״̬
	ZL_ST_INENDMULTI_COMMENT,	//����ע��ɨ�����״̬
	ZL_ST_INXOR,				//��λ����������ɨ��״̬
	ZL_ST_INBIT_RIGHT,			//�����������ɨ��״̬
	ZL_ST_INBIT_LEFT,			//�����������ɨ��״̬
	ZL_ST_PARSER_INSEMI,		//��zengl_parser.c����ӵķֺ�token״̬
	ZL_ST_PARSER_INEQUAL,		//��zengl_parser.c����ӵĵ���token״̬
	ZL_ST_PARSER_INGREAT_EQ,	//��zengl_parser.c����ӵĴ��ڵ���token״̬
	ZL_ST_PARSER_INLESS_EQ,		//��zengl_parser.c����ӵ�С�ڵ���token״̬
	ZL_ST_PARSER_INREVERSE,		//��zengl_parser.c����ӵ�ȡ�������token״̬
	ZL_ST_PARSER_INADDRESS,		//��zengl_parser.c����ӵ����������token״̬
	ZL_ST_PARSER_INPLUS_PLUS,	//��zengl_parser.c����ӵ�++�����token״̬
	ZL_ST_PARSER_INPLUS_ASSIGN,	//��zengl_parser.c����ӵ�+=�����token״̬
	ZL_ST_PARSER_INMINIS_MINIS,	//��zengl_parser.c����ӵ�--�����token״̬
	ZL_ST_PARSER_INMINIS_ASSIGN,//��zengl_parser.c����ӵ�-=�����token״̬
	ZL_ST_PARSER_INTIMES_ASSIGN,//��zengl_parser.c����ӵ�*=�����token״̬
	ZL_ST_PARSER_INDIVIDE_ASSIGN,//��zengl_parser.c����ӵ�/=�����token״̬
	ZL_ST_PARSER_INMOD_ASSIGN,	//��zengl_parser.c����ӵ�%=�����token״̬
	ZL_ST_PARSER_INLBRACKET,	//��zengl_parser.c����ӵ�������token״̬
	ZL_ST_PARSER_INRBRACKET,	//��zengl_parser.c����ӵ�������token״̬
	ZL_ST_PARSER_INLMBRACKET,	//��zengl_parser.c����ӵ���������token״̬
	ZL_ST_PARSER_INRMBRACKET,	//��zengl_parser.c����ӵ���������token״̬
	ZL_ST_PARSER_INCOMMA,		//��zengl_parser.c����ӵĶ��������token״̬
	ZL_ST_PARSER_INCOLON,		//��zengl_parser.c����ӵ�ð�������token״̬
	ZL_ST_PARSER_INQUESTION_MARK,//��zengl_parser.c����ӵ��ʺ������token״̬
	ZL_ST_PARSER_INDOT,			//��zengl_parser.c����ӵĵ������token״̬
	ZL_ST_PARSER_INNEGATIVE,	//��zengl_parser.c����ӵĸ��ŵ�Ŀ�����token״̬
	ZL_ST_PARSER_INBIT_AND,		//��zengl_parser.c����ӵİ�λ�������token״̬
	ZL_ST_PARSER_INBIT_AND_ASSIGN,//��zengl_parser.c����ӵ�&=�����token״̬
	ZL_ST_PARSER_INBIT_OR,		//��zengl_parser.c����ӵİ�λ�������token״̬
	ZL_ST_PARSER_INBIT_OR_ASSIGN,//��zengl_parser.c����ӵ�|=�����token״̬
	ZL_ST_PARSER_INBIT_XOR,		//��zengl_parser.c����ӵİ�λ��������token״̬
	ZL_ST_PARSER_INBIT_XOR_ASSIGN,//��zengl_parser.c����ӵ�^=�����token״̬
	ZL_ST_PARSER_INBIT_RIGHT,	//��zengl_parser.c����ӵ����������token״̬
	ZL_ST_PARSER_INBIT_RIGHT_ASSIGN,//��zengl_parser.c����ӵ�>>=���Ƹ�ֵ�����token״̬
	ZL_ST_PARSER_INBIT_LEFT,	//��zengl_parser.c����ӵ����������token״̬
	ZL_ST_PARSER_INBIT_LEFT_ASSIGN,//��zengl_parser.c����ӵ�<<=���Ƹ�ֵ�����token״̬
	ZL_ST_PARSER_INBIT_REVERSE,	//��zengl_parser.c����ӵ�~��λȡ�������token״̬
	ZL_ST_PARSER_STMT_INIF,		//��������if�ؼ������AST��״̬��
	ZL_ST_PARSER_STMT_INFOR,	//��������for�ؼ������AST��״̬��
	ZL_ST_PARSER_STMT_INFUN,	//��������fun�ؼ������AST��״̬��
	ZL_ST_PARSER_STMT_INSWITCH,	//��������switch�ؼ������AST��״̬��
	ZL_ST_PARSER_STMT_INCLASS,	//��������class�ؼ������AST��״̬��
	ZL_ST_PARSER_STMT_INWHILE,	//��������while�ؼ������AST��״̬��
	ZL_ST_PARSER_STMT_INDOWHILE,//��������do...dowhile�ؼ������AST��״̬��
	ZL_ST_ASM_CODE_INPRINT,		//��������print�ؼ��ֵĻ��ָ��
	ZL_ST_ASM_CODE_INPLUS_MINIS,//��������+,-,+=,-=������Ļ��ָ��
	ZL_ST_ASM_CODE_INTIME_DIVIDE,//��������%,*,/,%=,*=,/=������Ļ��ָ��
	ZL_ST_ASM_CODE_INRELATION,	//�������ɴ��ڣ�С�ڣ����ڣ������ڣ����ڵ��ڣ�С�ڵ���������Ļ��ָ��
	ZL_ST_ASM_CODE_INAND_OR,	//���������߼��룬�߼���������Ļ��ָ��
	ZL_ST_ASM_CODE_INREVERSE,	//���������߼���������Ļ��ָ��
	ZL_ST_ASM_CODE_INBIT_REVERSE,//�������ɰ�λȡ��������Ļ��ָ��
	ZL_ST_ASM_CODE_INIF,		//��������if���Ļ��ָ��
	ZL_ST_ASM_CODE_INELIF,		//��������elif���Ļ��ָ��
	ZL_ST_ASM_CODE_IN_PP_MM,	//�������ɼӼӼ����Ļ��ָ��
	ZL_ST_ASM_CODE_INFOR,		//��������for���Ļ��ָ��
	ZL_ST_ASM_CODE_INFUN,		//��������fun���Ļ��ָ��
	ZL_ST_ASM_CODE_INFUNCALL,	//��������funcall�������õĻ��ָ��
	ZL_ST_ASM_CODE_INCOMMA,		//�������ɶ������Ļ��ָ��
	ZL_ST_ASM_CODE_INUSE,		//��������use���Ļ��ָ��
	ZL_ST_ASM_CODE_INBREAK,		//��������break���Ļ��ָ��
	ZL_ST_ASM_CODE_INCONTINUE,	//��������continue���Ļ��ָ��
	ZL_ST_ASM_CODE_INGLOBAL,	//��������global���Ļ��ָ��
	ZL_ST_ASM_CODE_INRETURN,	//��������return���Ļ��ָ��
	ZL_ST_ASM_CODE_INADDRESS,	//���������������Ļ��ָ��
	ZL_ST_ASM_CODE_INARRAY_ITEM,//������������Ԫ�صĻ��ָ��
	ZL_ST_ASM_CODE_INCLASS,		//��������class�ṹ�Ļ��ָ��
	ZL_ST_ASM_CODE_INCLASS_STATEMENT,//�����������������Ļ��ָ��
	ZL_ST_ASM_CODE_INDOT,		//�����������Ա��������Ļ��ָ��
	ZL_ST_ASM_CODE_INDOT_SYM_SCAN, //�������ɶԵ����������ɨ��ʱ�Ļ��ָ��
	ZL_ST_ASM_CODE_INSWITCH,	//��������switch�ṹ�Ļ��ָ��
	ZL_ST_ASM_CODE_INWHILE,		//��������while�ṹ�Ļ��ָ��
	ZL_ST_ASM_CODE_INDOWHILE,	//��������do...dowhileѭ���ṹ�Ļ��ָ��
	ZL_ST_ASM_CODE_INCOLON,		//��������ð��������Ļ��ָ��
	ZL_ST_ASM_CODE_INQUESTION,	//���������ʺ�������Ļ��ָ��
	ZL_ST_ASM_CODE_INNEGATIVE,	//�������ɸ��ŵ�Ŀ������Ļ��ָ��
	ZL_ST_ASM_CODE_INBITS,		//�������ɰ�λ�룬������˫Ŀλ������Ļ��ָ��
}ZENGL_STATES;
/*��switch case��Ҫ�õ��ĸ���״̬��ö�ٶ������*/

/*������Ҫ�õ���ϣ�������ö��*/
typedef enum _ZENGL_HASH_TYPES{
	ZL_HASH_TYPE_SYM_TABLE = 0,
	ZL_HASH_TYPE_FUN_TABLE = 1,
	ZL_HASH_TYPE_LOCAL_TABLE = 2,
	ZL_HASH_TYPE_CLASS_TABLE = 4,
	ZL_HASH_TYPE_CLASSMEMBER_TABLE = 5,
	ZL_HASH_TYPE_DEF_TABLE = 6,
}ZENGL_HASH_TYPES;
/*������Ҫ�õ���ϣ�������ö�ٶ������*/

/*�����㷨����ö��*/
typedef enum _ZENGL_ENCRYPT_TYPES{
	ZL_ENC_TYPE_NONE,  //û�м��ܣ�Ĭ�ϳ�ʼֵ
	ZL_ENC_TYPE_XOR,   //��ͨ������
	ZL_ENC_TYPE_RC4,   //RC4����
}ZENGL_ENCRYPT_TYPES;
/*�����㷨����ö�ٶ������*/

typedef struct _ZENGL_TOKEN_STRING_TYPE{
	ZL_CHAR * str; //token���ַ�����Ϣ
	ZL_INT size; //�ַ����Ķ�̬��С
	ZL_INT count; //��ŵ��ַ���
	ZL_INT cur; //��ǰ�α�
	ZL_INT start_line; //token������
	ZL_INT start_col; //token������
	ZL_CHAR * filename; //����inc���ص��ļ���
	ZENGL_RESERVE_TYPE reserve; //���token�ǹؼ��֣����Źؼ��ֵ�������Ϣ
}ZENGL_TOKEN_INFO_TYPE;  //���ɨ�赽��token��Ϣ��

typedef struct _ZENGL_INFO_STRING_TYPE{
	ZL_CHAR * str; //��Ϣ���ַ���ָ��
	ZL_INT size;  //�ַ����Ķ�̬��С
	ZL_INT count; //��ŵ��ַ���
	ZL_INT cur;  //��ǰ�α�
}ZENGL_INFO_STRING_TYPE; //�����ͨ��ӡ��Ϣ�������Ϣ������

typedef struct _ZENGL_XOR_ENCRYPT{
	ZL_CHAR * xor_key_str;
	ZL_INT xor_key_len;
	ZL_INT xor_key_cur;
}ZENGL_XOR_ENCRYPT; //XOR��ͨ������������صĽṹ�嶨��

typedef struct _ZENGL_RC4_ENCRYPT{
	ZL_BYTE state[ZL_RC4_STATE_SIZE];
	ZL_INT i;
	ZL_INT j;
}ZENGL_RC4_ENCRYPT; //RC4����������صĽṹ�嶨��

typedef struct _ZENGL_ENCRYPT{
	ZENGL_ENCRYPT_TYPES type;
	ZENGL_XOR_ENCRYPT xor;
	ZENGL_RC4_ENCRYPT rc4;
}ZENGL_ENCRYPT; //��������ṹ�嶨��

typedef struct _ZENGL_SOURCE_TYPE{
	ZL_CHAR *filename;
	FILE *file;
	ZL_UCHAR buf[ZL_FILE_BUF_SIZE];
	ZL_INT buf_read_len;
	ZL_INT cur;
	ZL_BOOL needread;
	ZENGL_ENCRYPT encrypt; //��������ṹ���Ա
	ZL_UCHAR * run_str; //zenglApi_RunStr���õ��ַ����ű�
	ZL_INT run_str_len; //�ַ����ű��ĳ���
}ZENGL_SOURCE_TYPE;  //�ű�Դ�ļ����Ͷ��壬�������Ҫ�����Ľű��ļ����ļ�ָ�룬�ļ����ȳ�Ա��

typedef struct _ZENGL_MEM_POOL_POINT_TYPE{
	ZL_VOID * point;
	ZL_INT size;
}ZENGL_MEM_POOL_POINT_TYPE;

typedef struct _ZENGL_MEM_POOL_TYPE{
	ZL_INT size;
	ZL_INT totalsize; //�ڴ�ض�̬������ܴ�С�������ڴ������Ĵ�С
	ZL_INT count;
	ZENGL_MEM_POOL_POINT_TYPE * points;
}ZENGL_MEM_POOL_TYPE;  //�ڴ�����Ͷ��壬points��Աָ��һ��ָ�����飬�������ű������������ָ�롣

typedef struct _ZENGL_STRING_POOL_TYPE{
	ZL_BOOL isInit;
	ZL_INT size;
	ZL_INT count;
	ZL_CHAR * ptr; //��̬����Ļ���
}ZENGL_STRING_POOL_TYPE; //�����г�������ַ�����Ϣ��������ַ������ͨ�������ڳػ����в��ҳ������ַ�����Ϣ��

typedef struct _ZENGL_DEF_TABLE_MEMBER{
	ZL_BOOL isvalid;
	ZL_INT nameIndex; //����������ʽ����������Ϊ�ַ����ص����ﵽһ���̶�ʱ���ͻ����ݣ������ָ�룬ָ���ַ�ͻ�ʧЧ��
	ZENGL_TOKENTYPE tokentype;
	ZL_INT valIndex;  //����������ʽ������������ԭ����ָ�����͡�
	ZL_INT linecols;
	ZL_INT next;
} ZENGL_DEF_TABLE_MEMBER; //def�궨�嶯̬������ĳ�Ա�ṹ

typedef struct _ZENGL_DEF_TABLE{
	ZL_BOOL isInit;
	ZL_INT size;
	ZL_INT count;
	ZENGL_DEF_TABLE_MEMBER * defs;
} ZENGL_DEF_TABLE;  //def�궨�嶯̬����

typedef struct _ZENGL_DEF_LOOKUP_TYPE{
	ZL_BOOL isInLookupHandle;  // �жϵ�ǰ�Ƿ����������õ�lookupHandle�������õ��У���zenglApi_SetDefLookupResult�ӿ�ֻ�����û����õ�lookupHandle������ʹ��
	ZL_VM_API_DEF_LOOKUP_HANDLE lookupHandle; // �û��Զ����def��ֵ��ѯ������ͨ���ú������Ը����Զ���Ĳ�ѯ������������Ӧ�ĺ�ֵ(�Ӷ�ʵ���û�������ű��е����Զ���ĺ�ֵ)
	ZL_BOOL hasFound;          // ���ֶ������ж��Ƿ��ѯ���˶�Ӧ�ĺ�ֵ
	ZENGL_TOKENTYPE token;     // ��ѯ���ĺ�ֵ��token�ڵ����ͣ������жϺ�ֵ���������ͣ����Ǹ��������ͣ��������ַ�������
	ZL_INT valIndex;           // ��ѯ���ĺ�ֵ��def_StringPool�ַ������е�����(��ֵ�ڱ���ʱ�����ַ�����ʽ���д洢�ģ�������������ָ��ʱ��תΪ��Ӧ����������������)
} ZENGL_DEF_LOOKUP_TYPE; // �ýṹ���ڴ洢��def��ֵ��ѯ��ص��û��Զ��庯�����Լ��洢���ݸò�ѯ������ѯ�����ĺ�ֵ���

typedef struct _ZENGL_LINECOL{
	ZL_INT lineno;
	ZL_INT colno;
	ZL_CHAR * filename; //��Ϊ������inc�����Լ����ļ����������к���Ϣ�ﻹ��Ҫ���ļ�����Ϣ��
	ZL_INT next; //��һ��Ԫ����LineColTable��Ӧ��lines��̬�����е�������
} ZENGL_LINECOL;  //���ű���ı�ʾ����Ӧ�����кţ�һ���������ű�ʾ�����ֵ�ÿ��λ�á�

typedef struct _ZENGL_LINECOL_TABLE{
	ZL_BOOL isInit;
	ZL_INT size;
	ZL_INT count;
	ZENGL_LINECOL * lines;
} ZENGL_LINECOL_TABLE; //���ű����кŽṹ�Ķ�̬���顣

typedef struct _ZENGL_FILE_STACK_TYPE{
	ZENGL_SOURCE_TYPE source;	 //sourceΪZENGL_SOURCE_TYPE���ͣ���������Ҫ���ص��ļ������ļ�ָ����ļ�����ȡ�
	ZL_INT line;
	ZL_INT col;
}ZENGL_FILE_STACK_TYPE; //�ļ���ջ��ÿ���ļ��Ľṹ			

typedef struct _ZENGL_FILE_STACKLIST_TYPE{
	ZL_BOOL isInit;
	ZL_INT size;
	ZL_INT count;
	ZL_INT filenames_size;  // filenames��̬����Ĵ�С�����洢�ĳ�Ա�����ڸô�Сʱ���ͻὫfilenames������ж�̬����
	ZL_INT filenames_count; // filenames��̬������ʵ�ʴ洢�ĳ�Ա��
	ZENGL_FILE_STACK_TYPE * stacks;
	ZL_CHAR ** filenames; // �ö�̬�����н��洢����inc���ع����ļ����ļ���ָ��
}ZENGL_FILE_STACKLIST_TYPE; //����inc���ص��ļ���ջ		

/********************************************************************************
		�����Ǻ�zengl_symbol.c���ű�����صĽṹ���ö�ٵȶ���
********************************************************************************/

#define ZL_SYM_SELF_TOKEN_STR "self" // ʹ��self����������������ʾ��ǰ�����������
#define ZL_SYM_GLOBAL_TABLE_SIZE 211 //ȫ�ֱ������ű�̬�����ʼ���Ͷ�̬���ݵĴ�С
#define ZL_SYM_LOCAL_TABLE_SIZE 211	 //�ֲ�����(������������)���ű�̬�����ʼ���Ͷ�̬���ݵĴ�С
#define ZL_SYM_CLASS_TABLE_SIZE 50 //�������Ϣ�Ķ�̬�����ʼ���Ͷ�̬���ݵĴ�С
#define ZL_SYM_CLASSMEMBER_TABLE_SIZE 211 //���Ա���ű�̬�����ʼ���Ͷ�̬���ݵĴ�С
#define ZL_SYM_FUN_TABLE_SIZE 100 //�������ű�Ķ�̬�����ʼ���Ͷ�̬���ݵĴ�С
#define ZL_SYM_SELF_CLASS_TABLE_SIZE 20 // self�ڵ��Ӧ������Ϣ�Ķ�̬����ĳ�ʼ���Ͷ�̬���ݵĴ�С

typedef enum _ZENGL_SYM_ENUM_LOCAL_TYPE{
	ZL_SYM_ENUM_LOCAL_TYPE_START,	//Ĭ�ϳ�ʼֵ������Ӧ�κ�����
	ZL_SYM_ENUM_LOCAL_TYPE_ARG,		//�ű������Ĳ�������
	ZL_SYM_ENUM_LOCAL_TYPE_LOCAL,	//������ľֲ���������
	ZL_SYM_ENUM_LOCAL_TYPE_GLOBAL,	//������global������ȫ�ֱ���
}ZENGL_SYM_ENUM_LOCAL_TYPE; //�ֲ����ű��г�Ա�����ͣ�����������ͣ��ֲ��������ͣ�������global�ؼ���������ȫ�ֱ���

typedef struct _ZENGL_SYM_GLOBAL_TABLE_MEMBER{
	ZL_BOOL isvalid; //�жϷ��ű�ĳ���Ƿ�ռ�á�
	ZL_INT nameIndex; //ʹ���ַ���������������ָ��
	ZL_INT classid; //�������е���id
	ZL_INT linecols;
	ZL_INT memloc ; /* memory location for variable �������ڴ��ַ*/
	ZL_INT next;
}ZENGL_SYM_GLOBAL_TABLE_MEMBER; //ȫ�ֱ������ű���ĳ�Ա

typedef struct _ZENGL_SYM_GLOBAL_TABLE{
	ZL_BOOL isInit; //�ж�sym��̬�����Ƿ��ʼ��
	ZL_INT size;
	ZL_INT count;
	ZENGL_SYM_GLOBAL_TABLE_MEMBER * sym;
	ZL_INT global_memloc; //ȫ�ֱ������ڴ��ַ������
}ZENGL_SYM_GLOBAL_TABLE; //ȫ�ֱ������ű�

typedef struct _ZENGL_SYM_LOCAL_TABLE_MEMBER{
	ZL_BOOL isvalid; //�жϷ��ű�ĳ���Ƿ�ռ�á�
	ZL_INT nameIndex; //ʹ���ַ���������������ָ��
	ZENGL_SYM_ENUM_LOCAL_TYPE type; //��Ա����
	ZL_INT localid;
	ZL_INT funid; //�ֲ��������ں�����idֵ��
	ZL_INT classid;  //�ֲ�������Ӧ����id
	ZL_INT next;
}ZENGL_SYM_LOCAL_TABLE_MEMBER; //����,�ֲ��������ű���ĳ�Ա

typedef struct _ZENGL_SYM_LOCAL_TABLE{
	ZL_BOOL isInit;
	ZL_INT size;
	ZL_INT count;
	ZENGL_SYM_LOCAL_TABLE_MEMBER * local;
}ZENGL_SYM_LOCAL_TABLE; //�ű����������;ֲ��������ű�

typedef struct _ZENGL_SYM_CLASS_TABLE_MEMBER{
	ZL_BOOL isvalid;
	ZL_INT nameIndex;
	ZL_INT nodenum; //�ඨ����ڵ��Ӧ��AST�ڵ��
	ZL_INT linecols;
	ZL_INT classid; //��id�����Ψһ��ʶ
	ZL_INT next;
}ZENGL_SYM_CLASS_TABLE_MEMBER; //ÿ���ඨ�嶼Ҫ��ZENGL_SYM_CLASS_TABLE��ע��һ����Ա����ΪZENGL_SYM_CLASS_TABLE��̬�����еĳ�Ա�Ľṹ���塣

typedef struct _ZENGL_SYM_CLASS_TABLE{
	ZL_BOOL isInit; //�ж�class��̬�����Ƿ��ʼ��
	ZL_INT size;
	ZL_INT count;
	ZENGL_SYM_CLASS_TABLE_MEMBER * classTable;
	ZL_INT global_classid; //��id������
}ZENGL_SYM_CLASS_TABLE; //�������Ϣ�Ķ�̬����

typedef struct _ZENGL_SYM_CLASSMEMBER_TABLE_MEMBER{
	ZL_BOOL isvalid;
	ZL_INT nameIndex;
	ZL_INT index;
	ZL_INT nodenum; //���Ա���ڵ�AST�ڵ�ţ�����ͨ����AST�ڵ���Ϣ�õ����Ա�����к���Ϣ�ȣ�Ҳ������linecols������linecols���ж��⿪����linecos���Լ�¼������кţ������˴�ֻ��Ҫһ�����к�
	ZL_INT classid;	//���Ա����class�ṹ��Ӧ����id
	ZL_INT cls_stmt_classid;	//����������Ӧ����ID
	ZL_INT next;
}ZENGL_SYM_CLASSMEMBER_TABLE_MEMBER; //���Ա���ű�̬�����е���Ԫ�صĶ���

typedef struct _ZENGL_SYM_CLASSMEMBER_TABLE{
	ZL_BOOL isInit; //�ж����Ա���ű�̬�����Ƿ��ʼ��
	ZL_INT size;
	ZL_INT count;
	ZENGL_SYM_CLASSMEMBER_TABLE_MEMBER * members;
}ZENGL_SYM_CLASSMEMBER_TABLE; //���Ա���ű�̬����Ķ���

typedef struct _ZENGL_SYM_FUN_TABLE_MEMBER{
	ZL_BOOL isvalid;
	ZL_INT nameIndex; //ʹ���ַ����������������ַ���ָ��
	ZL_INT linecols;
	ZL_INT funid;
	ZL_INT LDAdr;	//����α��ַ
	ZL_INT classid; //������ຯ�����������ID
	ZL_INT next;
}ZENGL_SYM_FUN_TABLE_MEMBER; //������̬�����г�Ա�Ľṹ����

typedef struct _ZENGL_SYM_FUN_TABLE{
	ZL_BOOL isInit; //�жϺ�����̬�����Ƿ��ʼ��
	ZL_INT size;
	ZL_INT count;
	ZENGL_SYM_FUN_TABLE_MEMBER * funs;
	ZL_INT global_funid; //����id������
}ZENGL_SYM_FUN_TABLE; //������̬����Ľṹ����

typedef struct _ZENGL_SYM_SELF_CLASS_TABLE_MEMBER{
	ZL_BOOL isvalid;       // �жϵ�ǰ��Ա�Ƿ���Ч
	ZL_INT self_nodenum;   // self�ڵ��AST�ڵ��
	ZL_INT class_nodenum;  // self�ڵ����ڵ����class�ڵ��AST�ڵ��
	ZL_INT classid;        // self�ڵ��Ӧ����ID
}ZENGL_SYM_SELF_CLASS_TABLE_MEMBER; // self�ڵ��Ӧ������Ϣ�Ķ�̬����ĳ�Ա�Ľṹ����

typedef struct _ZENGL_SYM_SELF_CLASS_TABLE{
	ZL_BOOL isInit; // �ж�self�ڵ��Ӧ������Ϣ�Ķ�̬�����Ƿ��ʼ��
	ZL_INT size;    // ��̬����С�󣬵�ǰ�������ɵĳ�Ա������count��Ч��Ա������sizeʱ����Զ�̬������ж�̬���ݣ�ͬʱ����size��ֵ
	ZL_INT count;   // ��̬�����е���Ч��Ա��
	ZENGL_SYM_SELF_CLASS_TABLE_MEMBER * members; // ָ��̬�����ָ�룬��̬�����е�ÿ����Ա�Ľṹ����ZENGL_SYM_SELF_CLASS_TABLE_MEMBER����
	ZL_INT cur_class_nodenum; // �������ڽ����class�ڵ����AST�����﷨��ʱ���Ὣ�ýڵ��AST�ڵ�ż�¼��cur_class_nodenum�ֶΣ������������self�ڵ�Ϳ��Ժ͸�class�ڵ㽨�������ˣ��Ӷ�����֪��self�ڵ������ĸ�����
}ZENGL_SYM_SELF_CLASS_TABLE; // �洢self�ڵ��Ӧ������Ϣ�Ķ�̬����Ľṹ����

/********************************************************************************
		�����Ǻ�zengl_symbol.c���ű�����صĽṹ���ö�ٵȶ���
********************************************************************************/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************************
		�����Ǻ�zengl_assemble.c������������صĽṹ���ö�ٵȶ���
********************************************************************************/

#define ZL_ASM_ADDR_TYPE_NUM 22 //ΪZL_ASM_STACK_ENUM_IF_ADDR,ZL_ASM_STACK_ENUM_IF_END,ZL_ASM_STACK_ENUM_FOR_ADDR,ZL_ASM_STACK_ENUM_FOR_END...��Ԫ�أ��Ժ�����ʱ��ĿҪ��Ӧ���ӣ�
#define ZL_ASM_STACK_LIST_SIZE 20 //���ɻ�����ʱ��Ϊ����Ƕ��ʽif�Ƚṹ��Ҫ�����ջ�ṹ�������Ƕ�ջ��̬����ĳ�ʼ���Ͷ�̬���ݴ�С��
#define ZL_ASM_LOOP_STACK_LIST_SIZE 20 //���ڴ���ݹ���ö����õĻ��ģ���ջ��̬����ĳ�ʼ���붯̬���ݵĴ�С
#define ZL_ASM_CASE_JMP_TABLE_SIZE 15 //switch...case����ת��ĳ�ʼֵ�����ݴ�С

typedef enum _ZENGL_ASM_STACK_ENUM{
	ZL_ASM_STACK_ENUM_IF_ADDR,			 //if,elif���ʽ�ж�Ϊfalseʱ����Ҫ��ת�ĵ�ַ
	ZL_ASM_STACK_ENUM_IF_END,			 //if-elif-else���Ľ���λ��
	ZL_ASM_STACK_ENUM_FOR_ADDR,			 //����for(i=0;i<10;i++) �м��i<10�ж�������תλ��
	ZL_ASM_STACK_ENUM_FOR_END,			 //for...endfor�ṹ�Ľ���λ��
	ZL_ASM_STACK_ENUM_FOR_CONTINUE,		 //for�ṹ��continue������ת��λ��
	ZL_ASM_STACK_ENUM_FUN_ADDR,			 //��������ʼ������λ��
	ZL_ASM_STACK_ENUM_FUN_END,			 //�����Ľ���λ��
	ZL_ASM_STACK_ENUM_ARRAY_ITEM_OP_TYPE,//�ж϶�����Ԫ�صĲ�����ʽ����ͬ�Ĳ��������ɲ�ͬ�Ļ����룬��test[a] = 3;��������SET_ARRAY��ָ�b = test[a];������GET_ARRAY��ָ���
	ZL_ASM_STACK_ENUM_IS_IN_ARRAYITEM_OR_FUNCALL, //�жϵ�ǰ���ʽ�Ƿ�λ�������������ʽ�л������õĲ����б��У���test[a+b]��a+bλ��test�����������ʽ�У�����a+b���ɻ�����󣬻���PUSH��ֵѹ��ջ�Թ�����Ԫ�ؽ�������������test(a+b-c)��a+b-c���ɻ�����󣬻���PUSHѹ��ջ��Ϊ����test�Ĳ���ֵ��
	ZL_ASM_STACK_ENUM_AND_ADDR,			 //�߼�������������i==0 && j==1��������i==0���Ϊfalseʱ����תλ�ã���i==0Ϊfalseʱ�Ͳ���Ҫ���ж�j==1�ˣ����ԾͿ���ֱ����ת�����ĩβ

	ZL_ASM_STACK_ENUM_OR_ADDR,			 //�߼�������������i==0 || j==1��������i==0���Ϊtrueʱ����תλ�ã���i==0Ϊtrueʱ�Ͳ���Ҫ���ж�j==1�ˣ����ԾͿ���ֱ����ת�����ĩβ
	ZL_ASM_STACK_ENUM_SWITCH_TABLE,		 //switch...case����ת��λ��
	ZL_ASM_STACK_ENUM_SWITCH_DEFAULT,	 //switch...case��default����תλ��
	ZL_ASM_STACK_ENUM_SWITCH_END,		 //switch...case�ṹ�Ľ���λ��
	ZL_ASM_STACK_ENUM_WHILE_ADDR,		 //while�����жϵĿ�ͷλ��
	ZL_ASM_STACK_ENUM_WHILE_END,		 //while�ṹ�Ľ���λ��
	ZL_ASM_STACK_ENUM_DO_ADDR,			 //do...dowhile�ṹ��ͷ���λ��
	ZL_ASM_STACK_ENUM_DO_CONTINUE,		 //do...dowhile�ṹ��continue��Ҫ��ת��λ��
	ZL_ASM_STACK_ENUM_DO_END,			 //do...dowhile�ṹ�Ľ���λ��
	ZL_ASM_STACK_ENUM_QUESTION_ADDR,	 //����a==b ? 1 : 2;�ʺ�ð�Žṹ�е�a==bΪfalseʱ����ת��2�Ļ�����λ��

	ZL_ASM_STACK_ENUM_QUESTION_END,		 //�ʺ�ð�Žṹ�Ľ���λ��
	ZL_ASM_STACK_ENUM_FUN_CLASSID,		 //�����ຯ��ID��Ϣ
}ZENGL_ASM_STACK_ENUM; //���ջ����if,for,fun,array����,and,or�Ƚṹ��Ƕ����صĺ�

typedef enum _ZENGL_ASM_ARRAY_ITEM_OP_TYPE{
	ZL_ASM_AI_OP_NONE,			//Ĭ�ϳ�ʼֵ
	ZL_ASM_AI_OP_IN_MOV,		//������Ԫ�ؽ��и�ֵ����
	ZL_ASM_AI_OP_IN_ADDR,		//������Ԫ�ؽ������ò���
	ZL_ASM_AI_OP_IN_ADDGET,		//������Ԫ�ؽ��мӼ�(�ȼ�һ���ȡ)�Ĳ���
	ZL_ASM_AI_OP_IN_MINISGET,	//������Ԫ�ؽ��м���(�ȼ�һ���ȡ)�Ĳ���
	ZL_ASM_AI_OP_IN_GETADD,		//������Ԫ�ؽ��мӼ�(�Ȼ�ȡ���һ)�Ĳ���
	ZL_ASM_AI_OP_IN_GETMINIS,	//������Ԫ�ؽ��м���(�Ȼ�ȡ���һ)�Ĳ���
}ZENGL_ASM_ARRAY_ITEM_OP_TYPE;

typedef struct _ZENGL_ASM_GENCODE_STRUCT{
	//ZENGL_STATES state; //״̬�� ��ûʹ��
	//ZL_CHAR ID_memloc_str[30]; //������ʶ�����ڴ��еĵ�ַ��Ӧ���ַ�����ʽ
	ZL_INT pc;
	ZL_BOOL is_inFun; //���ɻ����ʱ�Ƿ�λ�ں�����
	ZL_INT localID;	//���ڽű������Ĳ����;ֲ�������ID������
}ZENGL_ASM_GENCODE_STRUCT; //zengl_AsmGenCodes�����л��õ���һЩ���������ڽṹ���У�����������������

typedef struct _ZENGL_ASM_STACK_TYPE{
	ZL_INT addrnum; //��Ӧ�Ļ��ָ��λ�á�
	ZENGL_ASM_STACK_ENUM addrtype; //��ַ���ͣ���if��ַ����for�ṹ�ĵ�ַ�ȡ�
	ZL_INT before_index; //�������ZENGL_ASM_STACKLIST_TYPE�ṹ�����ends�ֶ�һ��õ�һ����ַ����
	ZL_BOOL isvalid;
}ZENGL_ASM_STACK_TYPE; //����ջ��ÿ��Ԫ�صĽṹ����

typedef struct _ZENGL_ASM_STACKLIST_TYPE{
	ZL_BOOL isInit;
	ZL_INT size;
	ZL_INT count;
	ZL_INT ends[ZL_ASM_ADDR_TYPE_NUM]; //����ջ����ends�ֶ��Լ�����ASM_STACK_TYPE���before_index�ֻ��ֳ�if��ַ���ɵ������for�Ƚṹ�ĵ�ַ���ɵ�����
	ZENGL_ASM_STACK_TYPE * stacks;
}ZENGL_ASM_STACKLIST_TYPE; //Ϊ�˽��if,for�ṹ��Ƕ��ʽ���⣬��Ҫ�����ɻ�����ʱ����ģ���ջ��

typedef struct _ZENGL_ASM_LOOP_STACK_TYPE{
	ZL_INT nodenum; // ѹ��ģ��ջ��AST�ڵ��
	ZL_INT orig_nodenum; // ԭʼ�ڵ��
	ZL_INT extData[1]; // ѹ��ջ�Ķ�������
	ZL_INT stackValIndex; // �϶�Ķ������ݱ��浽stackVals�У�stackValIndexΪstackVals�е��ֽ�����ֵ��֮���Դ洢����ֵ������ΪstackVals������ʱ����ָ���п��ܻᷢ���ı䣬���ֻ�ܴ洢��������������ȷ�������λ��
	ZL_INT stackValCnt; // ������stackVals�еĶ������ݵĳߴ��С(���ֽ�Ϊ��λ)
	ZENGL_STATES state; // ѹ��ģ��ջ��״̬��
}ZENGL_ASM_LOOP_STACK_TYPE; // ���ģ���ջ(���ѭ��������ݹ����)��ÿ��Ԫ�صĽṹ����

typedef struct _ZENGL_ASM_LOOP_STACKLIST_TYPE{
	ZL_BOOL isInit;
	ZL_BOOL isInitStackVals;
	ZL_INT size;
	ZL_INT stackValSize;
	ZL_INT count;
	ZL_INT stackValCount;
	ZENGL_ASM_LOOP_STACK_TYPE * stacks; // ģ��ջ��̬�����ָ��ֵ
	ZL_BYTE * stackVals; // ʹ��ģ��ջ����ݹ麯������ʱ�����ĳ�������������а����϶�ľֲ�����ʱ������Ҫ����Щ�ֲ�������ֵ���浽stackVals�У��������´�ͨ��ģ��ջ�������������ʱ�����ָܻ���ȷ�ľֲ�����ֵ�����ٵľֲ��������Ա�����stacks���extData��
}ZENGL_ASM_LOOP_STACKLIST_TYPE; // ͨ������ģ��ջ�������zengl_AsmGenCodes�ݹ���ù�������ܵ��µ��ڴ�ջ�������

typedef struct _ZENGL_ASM_CASE_JMP_TABLE_MEMBER{
	ZL_LONG caseNum;
	ZL_INT caseAddr;
}ZENGL_ASM_CASE_JMP_TABLE_MEMBER; //switch...case�ṹ��ת��ĳ�Ա

typedef struct _ZENGL_ASM_CASE_JMP_TABLE{
	ZL_BOOL isInit;
	ZL_INT size;
	ZL_INT count;
	ZENGL_ASM_CASE_JMP_TABLE_MEMBER * member;
}ZENGL_ASM_CASE_JMP_TABLE; //switch...case�ṹ����ת��

typedef enum _ZENGL_ASMGC_DOT_STATUS{
	ZENGL_ASMGC_DOT_STATUS_START,
	ZENGL_ASMGC_DOT_STATUS_FINISH_SCAN_DOT,
	ZENGL_ASMGC_DOT_STATUS_FINISH_FUNCALL
} ZENGL_ASMGC_DOT_STATUS;

typedef struct _ZENGL_ASMGC_DOT_STACK_VAL{
	ZENGL_ASMGC_DOT_STATUS status;
} ZENGL_ASMGC_DOT_STACK_VAL;

/********************************************************************************
		�����Ǻ�zengl_assemble.c������������صĽṹ���ö�ٵȶ���
********************************************************************************/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************************
		�����Ǻ�zengl_ld.c��������صĽṹ���ö�ٵȶ���
********************************************************************************/

#define ZL_LD_ADDRLIST_SIZE 20 //���ӵ�ַ��̬�����ʼ�������ݵĴ�С��

typedef struct _ZENGL_LD_ADDR_TYPE{
	ZL_BOOL isvalid; //�ж��Ƿ���Ч���Ƿ�����ʹ��
	ZL_INT loc; //��α��ַ��Ӧ�Ļ�����λ��
}ZENGL_LD_ADDR_TYPE;

typedef struct _ZENGL_LD_ADDRLIST_TYPE{
	ZL_BOOL isInit; //�ж϶�̬�����Ƿ��ʼ��
	ZL_INT size; //��̬������ܴ�С
	ZL_INT count;//��̬�����д�ŵ�Ԫ�ظ���
	ZENGL_LD_ADDR_TYPE * addr; //ָ��̬�����ڴ�ռ��ָ��
}ZENGL_LD_ADDRLIST_TYPE; //���ӵ�ַ��̬��������Ͷ���

/********************************************************************************
		�����Ǻ�zengl_ld.c��������صĽṹ���ö�ٵȶ���
********************************************************************************/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************************
		�����Ǻ�zengl_parser.c�﷨�����ļ���صĽṹ���ö�ٵȶ���
********************************************************************************/

#define ZL_AST_CHILD_NODE_SIZE 3 //�﷨���ڵ�Ļ����ӽڵ������﷨���ӽڵ��ɻ����ӽڵ����չ�ӽڵ㹹��
#define ZL_AST_SIZE 200 //AST�����﷨����̬����ĳ�ʼ���Ͷ�̬���ݵĴ�С��
#define ZL_TK_STRING_POOL_SIZE 1024 //token�ַ����صĳ�ʼ���Ͷ�̬���ݵĴ�С��
#define ZL_OPLVL_STACKLIST_SIZE 40 //�����ջ��̬����ĳ�ʼ���Ͷ�̬���ݵĴ�С������Ķ�ջ��Ҫ��Ϊparser.c���������﷨��ʱ�������ͱȽϽڵ����ȼ��õġ�
#define ZL_AST_SCAN_STACKLIST_SIZE 60 //���µ����ö�ջ���нڵ�ɨ���У����õ��Ķ�ջ��̬����ĳ�ʼ���Ͷ�̬���ݵĴ�С��
#define ZENGL_AST_ISTOKTYPEX(nodenum,tokname) (nodenum >=0 && nodes[nodenum].isvalid == ZL_TRUE  &&  \
							  nodes[nodenum].toktype == tokname)
#define ZENGL_AST_ISTOKTYPEXOR(nodenum,tokname1,tokname2) (nodenum >=0 && nodes[nodenum].isvalid == ZL_TRUE  &&  \
							  (nodes[nodenum].toktype == tokname1 || \
							   nodes[nodenum].toktype == tokname2))
#define ZENGL_AST_ISTOKCATEX(nodenum,tokname1) (nodenum >=0 && nodes[nodenum].isvalid == ZL_TRUE  &&  \
							  (nodes[nodenum].tokcategory == tokname1))
#define ZENGL_AST_ISTOK_VALIDX(nodenum) (nodenum >=0 && nodes[nodenum].isvalid == ZL_TRUE) //�ж�ĳ�ڵ��Ƿ���Ч
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
													ZL_TKCG_OP_QUESTION)  //�ж�nodenum�ڵ��Ƿ��Ǳ��ʽ���������Ӽ��˳����Ƚ����㣬�߼���������ǵȡ�
#define ZENGL_AST_ISTOKEXPRESS(nodenum) (ZENGL_AST_ISTOKCATEXOP(nodenum) || ZENGL_AST_ISTOKTYPEXOR(nodenum,ZL_TK_FUNCALL,ZL_TK_ARRAY_ITEM)) //�ж�token�Ƿ��Ǳ��ʽ

typedef enum _ZENGL_TOKEN_CATEGORY{  //token�����ö��ֵ����Ӽ����������OP_PLUS_MINIS���͵ȵȡ�OP_FACTORΪ�������ӵ�token���ͣ�ָ����������Ĳ�����������������֣��ַ����ȡ�
	ZL_TKCG_OP_START,		//Ĭ�ϳ�ʼֵ
	ZL_TKCG_OP_PLUS_MINIS,	//�Ӽ������������
	ZL_TKCG_OP_ASSIGN,		//��ֵ��������
	ZL_TKCG_OP_TIM_DIV,		//�˳������������
	ZL_TKCG_OP_BITS,		//��λ�룬�����֮���λ�����
	ZL_TKCG_OP_RELATION,	//���ڣ�����֮��Ĺ�ϵ�Ƚ������������
	ZL_TKCG_OP_LOGIC,		//�߼����߼���֮����߼������������
	ZL_TKCG_OP_FACTOR,		//������ʶ�������֣��ַ���֮��Ĳ�����������
	ZL_TKCG_OP_COMMA,		//���Ų�������
	ZL_TKCG_OP_QUESTION,	//�ʺŲ�������
	ZL_TKCG_OP_PP_MM,		//�ӼӼ�����������
	ZL_TKCG_OP_DOT			//�������������
}ZENGL_TOKEN_CATEGORY;

typedef enum _ZENGL_OP_LEVEL{  //��������������ȼ�ö��ֵ���縳ֵ���߼�����ϵ�Ƚϣ��Ӽ����˳���ȡ���ȵ����ȼ���Ĭ�������ö��ֵԽ������ȼ�ҲԽ��
	ZL_OP_LEVEL_START,		//Ĭ�ϳ�ʼֵ
	ZL_OP_LEVEL_COMMA,		//�������ȼ�
	ZL_OP_LEVEL_ASSIGN,		//��ֵ���ȼ�������ֵ���͵ķ��������ʱ���ȼ�Ϊ��ǰ���ȼ��������Ҳ�ʱ���ȼ��ֱȳ��˵��������������ȼ����ߡ�
	ZL_OP_LEVEL_QUESTION,	//�ʺ����ȼ�
	ZL_OP_LEVEL_LOGIC,		//�߼��������ȼ�
	ZL_OP_LEVEL_RELATION,	//��ϵ�Ƚ��������ȼ�
	ZL_OP_LEVEL_BITS,		//��λ˫Ŀ��������ȼ�
	ZL_OP_LEVEL_PLUS_MINIS,	//�Ӽ��������ȼ�
	ZL_OP_LEVEL_TIM_DIV,	//�˳��������ȼ�
	ZL_OP_LEVEL_PP_MM,		//�ӼӼ����������ȼ�
	ZL_OP_LEVEL_REVERSE,	//ȡ���������ȼ�
	ZL_OP_LEVEL_ASSIGN_WHEN_IN_RIGHT, //����ֵ���������������Ҳ�ʱ���ȳ��˵������֮������ȼ�����
	ZL_OP_LEVEL_DOT			//��������ȼ�
}ZENGL_OP_LEVEL;

typedef enum _ZENGL_OP_POS_LEFT_OR_RIGHT{
	ZL_OP_POS_START_NONE, //Ĭ�ϳ�ʼֵ
	ZL_OP_POS_IN_LEFT,  //�ӼӼ������������Ŀ�����ʱ
	ZL_OP_POS_IN_RIGHT, //�ӼӼ������������Ŀ���Ҳ�ʱ
}ZENGL_OP_POS_LEFT_OR_RIGHT;

typedef struct _ZENGL_AST_CHILD_NODE_TYPE{
	ZL_INT size;
	ZL_INT count;
	ZL_INT childnum[ZL_AST_CHILD_NODE_SIZE]; //ָ��һ���������飬���������ӽڵ�Ľڵ�������
	ZL_INT *extchilds; //����3�����ӽڵ㶼�����extchilds��չ�ӽڵ��С�
}ZENGL_AST_CHILD_NODE_TYPE;  //�﷨����ÿ���ڵ���ӽڵ�����Ͷ��塣

typedef struct _ZENGL_AST_NODE_TYPE{
	ZL_BOOL isvalid; //�ڵ��Ƿ���Ч����Ч��ʾ�ýڵ�����һ��token��Ϣ���ѱ�ռ�ã������ٴ���Ϣ����Ч���ʾ�ýڵ㻹û���token��Ϣ���������������Ϣ��
	ZENGL_TOKENTYPE toktype; //��ʾ�ýڵ�����ʲô���͵�token����ZL_TK_ID��ʾ��ʾ����ZL_TK_NUM��ʾ���ֵȡ�
	ZENGL_OP_POS_LEFT_OR_RIGHT leftOrRight; //���ڼӼӼ���������������ж�������࣬�������Ҳ�
	ZL_BOOL isvalid_qst_colon; //�ж��Ƿ�����Ч���ʺŻ�ð�ţ����ʺ�ð���Ƿ�һһƥ��
	ZENGL_RESERVE_TYPE reserve;  //���ڵ��ǹؼ���ʱ��ͨ��reserve��Ա��Ӧ��ö��ֵ�ж����ĸ��ؼ��֡�
	ZENGL_TOKEN_CATEGORY tokcategory;  //token�����ķ��࣬��Ӽ������������ZL_OP_LEVEL_PLUS_MINIS����
	ZENGL_OP_LEVEL tok_op_level;  //������������ȼ���
	ZL_INT strindex; //token�ַ�����Ϣ��Token_StringPool�ַ������е�����������Ҫ��ȡ��token���ַ�����Ϣʱ���͸�������ȥ���ң�����ֻ��ҪΪ���е�token����һ������ڴ����������token���ַ�����Ϣ������ҪΪÿ��token�����з��䣬���ٿ�����
	ZENGL_AST_CHILD_NODE_TYPE childs;
	ZL_INT parentnode; //�ýڵ�ĸ��ڵ�������
	ZL_INT nextnode;
	ZL_INT line_no; //�к�
	ZL_INT col_no; //�к�
	ZL_CHAR * filename; //��ǰtoken���ڵ��ļ���
}ZENGL_AST_NODE_TYPE;  //�﷨�����ÿ���ڵ�����Ͷ���

typedef struct _ZENGL_AST_TYPE{
	ZL_BOOL isInit;
	ZL_INT size;
	ZL_INT count;
	ZENGL_AST_NODE_TYPE *nodes;
	ZL_INT rootnode;
}ZENGL_AST_TYPE;  //AST�����﷨���ڵ㶯̬��������Ͷ���

typedef struct _ZENGL_TOKEN_STRING_POOL{
	ZL_BOOL isInit;
	ZL_INT size;
	ZL_INT count;
	ZL_CHAR * ptr; //��̬������ַ����ػ���
}ZENGL_TOKEN_STRING_POOL; //������token���ַ�����Ϣ������ڸýṹ�嶨����ַ������ͨ���������ַ������в���token���ַ�����Ϣ��

typedef struct _ZENGL_PARSER_OP_LEVEL_STACK_TYPE{
	ZENGL_TOKEN_CATEGORY tokcategory;  //��ջ�ڵ��token����
	ZENGL_OP_LEVEL tok_op_level;  //��ջ�ڵ�����ȼ�
	ZL_INT nodenum; //�ڵ��
}ZENGL_PARSER_OP_LEVEL_STACK_TYPE;  //��ջ����Ԫ�ص����Ͷ��壬��Ҫ������ʱ��Žڵ����ȼ��õġ�

typedef struct _ZENGL_PARSER_OP_LEVEL_STACKLIST_TYPE{
	ZL_BOOL isInit;
	ZL_INT size;  //��̬����Ĵ�С
	ZL_INT count;  //��ջ��̬�����д�ŵ�Ԫ�ظ���
	ZENGL_PARSER_OP_LEVEL_STACK_TYPE * stacks;  //ָ��̬�����ָ��
}ZENGL_PARSER_OP_LEVEL_STACKLIST_TYPE;  //��ջ��̬��������Ͷ��塣��Ҫ������ʱ��Žڵ����ȼ��õġ�

typedef struct _ZENGL_PARSER_EXP_STRUCT{
	ZENGL_STATES state;
	ZL_INT p;
	ZL_INT tmpcount; //�ָ����ȼ���ջ���õ�
	ZL_INT tmpnode;  //��ʱ����ڵ����Ϣ
	ZENGL_PARSER_OP_LEVEL_STACK_TYPE retstack; //�����ȼ���ջ�е�����Ԫ��
	ZL_INT express_count;  //express�����ݹ���õ���ȣ���Ϊ��express��������ܻ�ݹ�����Լ���Σ��ڴӶ��㷵��ʱ��Ҫ��curnode�������⴦��������Ҫexpress_count����¼�ݹ���ȡ�
}ZENGL_PARSER_EXP_STRUCT; //��zengl_express�����л��õ���һЩ�������ŵ��ṹ���У����������������з���

typedef struct _ZENGL_AST_SCAN_STACK_TYPE{
	ZL_INT nodenum;
	ZL_INT curchild;
	ZL_INT childcnt;
	ZL_INT next;
}ZENGL_AST_SCAN_STACK_TYPE; //�µ�AST�﷨��ɨ���в����˶�ջ�ķ��������ﶨ�����AST�ڵ�ɨ���ջ��ÿ��Ԫ�صĽṹ����

typedef struct _ZENGL_AST_SCAN_STACKLIST_TYPE{
	ZL_BOOL isInit;
	ZL_INT size;
	ZL_INT count;
	ZENGL_AST_SCAN_STACK_TYPE * stacks;
}ZENGL_AST_SCAN_STACKLIST_TYPE; //AST�﷨��ɨ���в��õĶ�ջ��̬����Ķ��塣

/********************************************************************************
		�����Ǻ�zengl_parser.c�﷨�����ļ���صĽṹ���ö�ٵȶ���
********************************************************************************/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************************
		�����Ǻ��������������صĽṹ���ö�ٵȶ���
********************************************************************************/

#include "zenglrun_hash_array.h"

#define ZL_R_INST_LIST_SIZE 512 //���ָ�̬�����ʼ���Ͷ�̬���ݵĴ�С
#define ZL_R_VMEM_LIST_SIZE 512 //�����ڴ涯̬�����ʼ���Ͷ�̬���ݵĴ�С
#define ZL_R_VSTACK_LIST_SIZE 40 //�����ջ��̬�����ʼ���Ͷ�̬���ݵĴ�С��
#define ZL_R_MEM_BLOCK_SIZE 10	//����֮����ڴ���ʼ���Ͷ�̬���ݵĴ�С
#define ZL_R_MEM_POOL_SIZE 40	//�������ڴ�س�ʼ���Ͷ�̬���ݵĴ�С
#define ZL_R_MEM_FREE_POOL_SIZE 20 //�������ڴ��ͷųس�ʼ���Ͷ�̬���ݵĴ�С
#define ZL_R_INST_DATA_STRING_POOL_SIZE 1024 //ָ��������ַ����س�ʼ���Ͷ�̬���ݵĴ�С
#define ZL_R_REGLIST_SIZE 9		//�Ĵ�������Ŀ����Ĭ�ϳ�ʼֵ
#define ZL_R_MODULE_TABLE_SIZE 20 //ģ�鶯̬�����ʼ���Ͷ�̬���ݵĴ�С
#define ZL_R_MOD_FUN_TABLE_SIZE 50 //ģ�麯����̬�����ʼ���Ͷ�̬���ݵĴ�С
#define ZL_R_EXTRA_DATA_TABLE_SIZE 10 //�û��������ݶ�̬�����ʼ���Ͷ�̬���ݵĴ�С
#define ZL_R_HASH_SIZE 211 //��������ص�hash������Ĵ�С��
#define ZL_R_HASH_TOTAL_SIZE 633 //������Ŀǰ��3����̬���飬����һ��633��
#define ZL_R_HASH_SHIFT 4 //������hash������ʱ�ļ�������
#define ZENGL_RUN_REG(regnum) run->reg_list[regnum] //�Ĵ�����
#define ZENGL_RUN_REGVAL(regnum) run->reg_list[regnum].val //�Ĵ�����ֵ�ļ�д��
#define ZL_R_REG_PC ZENGL_RUN_REGVAL(ZL_R_RT_PC).dword  //��ǰPC�Ĵ�����ֵ��
#define ZENGL_RUN_INST(index) run->inst_list.insts[index] //��ȡָ���������ĳ��ָ�
#define ZL_R_CUR_INST ZENGL_RUN_INST(ZL_R_REG_PC) //��ǰPC�Ĵ�����Ӧ��ָ�
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
	ZL_R_IT_NONE,			//Ĭ�ϳ�ʼֵ
	ZL_R_IT_MOV,			//MOVָ��
	ZL_R_IT_PRINT,			//PRINTָ��
	ZL_R_IT_PLUS,			//PLUSָ��
	ZL_R_IT_MINIS,			//MINISָ��
	ZL_R_IT_MOD,			//MODָ��
	ZL_R_IT_TIMES,			//TIMESָ��
	ZL_R_IT_DIVIDE,			//DIVIDEָ��
	ZL_R_IT_PUSH,			//PUSHָ��
	ZL_R_IT_POP,			//POPָ��
	ZL_R_IT_JE,				//JEָ��
	ZL_R_IT_JNE,			//JNEָ��
	ZL_R_IT_JMP,			//JMPָ��
	ZL_R_IT_GREAT,			//GREATָ��
	ZL_R_IT_LESS,			//LESSָ��
	ZL_R_IT_EQUAL,			//EQUALָ��
	ZL_R_IT_NOT_EQ,			//NOT_EQָ��
	ZL_R_IT_GREAT_EQ,		//GREAT_EQָ��
	ZL_R_IT_LESS_EQ,		//LESS_EQָ��
	ZL_R_IT_AND,			//ANDָ��
	ZL_R_IT_OR,				//ORָ��
	ZL_R_IT_REVERSE,		//REVERSEָ��
	ZL_R_IT_ADDGET,			//ADDGETָ��
	ZL_R_IT_MINIS_GET,		//MINIS_GETָ��
	ZL_R_IT_GETADD,			//GETADDָ��
	ZL_R_IT_GET_MINIS,		//GET_MINISָ��
	ZL_R_IT_ADDONE,			//ADDONEָ��
	ZL_R_IT_MINIS_ONE,		//MINIS_ONEָ��
	ZL_R_IT_FUNARG,			//FUNARGָ��
	ZL_R_IT_ARG_SET,		//ARG_SETָ��
	ZL_R_IT_PUSH_LOC,		//PUSH_LOCָ��
	ZL_R_IT_RET,			//RETָ��
	ZL_R_IT_RETURN,			//RETURNָ��
	ZL_R_IT_RESET,			//RESETָ��
	ZL_R_IT_CALL,			//CALLָ��
	ZL_R_IT_USE,			//USEָ��
	ZL_R_IT_ADDR,			//ADDRָ��
	ZL_R_IT_SET_ARRAY,		//SET_ARRAYָ��
	ZL_R_IT_GET_ARRAY_ADDR,	//GET_ARRAY_ADDRָ��
	ZL_R_IT_ADDGET_ARRAY,	//ADDGET_ARRAYָ��
	ZL_R_IT_MINISGET_ARRAY,	//MINISGET_ARRAYָ��
	ZL_R_IT_GETADD_ARRAY,	//GETADD_ARRAYָ��
	ZL_R_IT_GETMINIS_ARRAY,	//GETMINIS_ARRAYָ��
	ZL_R_IT_GET_ARRAY,		//GET_ARRAYָ��
	ZL_R_IT_SWITCH,			//SWITCHָ��
	ZL_R_IT_LONG,			//LONGָ��
	ZL_R_IT_BIT_AND,		//BIT_ANDָ��
	ZL_R_IT_BIT_OR,			//BIT_ORָ��
	ZL_R_IT_BIT_XOR,		//BIT_XORָ��
	ZL_R_IT_BIT_RIGHT,		//BIT_RIGHTָ��
	ZL_R_IT_BIT_LEFT,		//BIT_LEFTָ��
	ZL_R_IT_BIT_REVERSE,	//BIT_REVERSEָ��
	ZL_R_IT_BREAK,			//BREAKָ��
	ZL_R_IT_SINGLE_BREAK,	//SINGLE_BREAKָ��
	ZL_R_IT_END,			//ENDָ��
}ZENGL_RUN_INST_TYPE; //ָ������

typedef enum _ZENGL_RUN_INST_OP_DATA_TYPE{
	ZL_R_DT_NONE,	//Ĭ�ϳ�ʼֵ
	ZL_R_DT_NUM,	//����������
	ZL_R_DT_FLOAT,	//������������
	ZL_R_DT_STR,	//�ַ���������
	ZL_R_DT_REG,	//REG�Ĵ���������
	ZL_R_DT_MEM,	//ȫ�ֱ����ڴ��ַ������
	ZL_R_DT_ARGMEM,	//���������ڴ��ַ������
	ZL_R_DT_LOCMEM,	//�����еľֲ������ڴ��ַ������
	ZL_R_DT_LDADDR,	//��ld������ʹ�õ�α��ַ����������̬�����α��ַ
	ZL_R_DT_LDFUNID, //����Ǻ������ã����ŵ��Ǻ�����ID����α��ַ�����滻��ַʱ���Ⱥ���ID�õ�α��ַ������α��ַ�õ���ʵ�Ļ���ַ�����������Ϳ��Զ����ڽű�������Ϸ�λ�ã����ֱ��ʹ��α��ַ����ô������ֻ�ܶ����ں�������֮ǰ
}ZENGL_RUN_INST_OP_DATA_TYPE; //ָ�����������

typedef enum _ZENGL_RUN_REG_TYPE{
	ZL_R_RT_NONE,	//Ĭ�ϳ�ʼֵ
	ZL_R_RT_AX,		//AX�Ĵ���
	ZL_R_RT_BX,		//BX�Ĵ���
	ZL_R_RT_ARG,	//ARG�Ĵ���
	ZL_R_RT_LOC,	//LOC�Ĵ���
	ZL_R_RT_ARGTMP,	//ARGTMP�Ĵ���
	ZL_R_RT_ARRAY_ITEM, //ARRAY_ITEM�Ĵ���
	ZL_R_RT_DEBUG,	//DEBUG���ԼĴ���
	ZL_R_RT_PC,		//PC�Ĵ���
}ZENGL_RUN_REG_TYPE; //�Ĵ�������

typedef enum _ZENGL_RUN_RUNTIME_OP_DATA_TYPE{
	ZL_R_RDT_NONE,			//Ĭ�ϳ�ʼֵ
	ZL_R_RDT_INT,			//����
	ZL_R_RDT_FLOAT,			//������
	ZL_R_RDT_STR,			//�ַ���
	ZL_R_RDT_ADDR,			//ȫ�ֱ���������
	ZL_R_RDT_ADDR_LOC,		//�ֲ�����������
	ZL_R_RDT_ADDR_MEMBLK,	//�ڴ������
	ZL_R_RDT_MEM_BLOCK,		//����,���Ӧ���ڴ��
}ZENGL_RUN_RUNTIME_OP_DATA_TYPE; //�ڴ棬�Ĵ������ֵ��ö�����ͣ�����ʱ���ͣ������������㣬�ַ��������ã������ڴ��ȡ�

typedef enum _ZENGL_RUN_VMEM_OP_TYPE{
	ZL_R_VMOPT_NONE,			//Ĭ�ϳ�ʼֵ
	ZL_R_VMOPT_ADDMEM_NONE,		//���δ��ʼ���������ڴ�
	ZL_R_VMOPT_ADDMEM_INT,		//�����ڴ��������
	ZL_R_VMOPT_ADDMEM_DOUBLE,	//�����ڴ���Ӹ�����
	ZL_R_VMOPT_ADDMEM_STR,		//�����ڴ�����ַ���
	ZL_R_VMOPT_ADDMEM_ADDR,		//�����ڴ��������
	ZL_R_VMOPT_ADDMEM_MEMBLOCK,	//�����ڴ�����ڴ��
	ZL_R_VMOPT_GETMEM,			//��ȡ�����ڴ��ֵ
	ZL_R_VMOPT_SETMEM_NONE,		//���������ڴ�Ϊδ��ʼ��
	ZL_R_VMOPT_SETMEM_INT,		//���������ڴ�Ϊ����
	ZL_R_VMOPT_SETMEM_DOUBLE,	//���������ڴ�Ϊ������
	ZL_R_VMOPT_SETMEM_STR,		//���������ڴ�Ϊ�ַ���
	ZL_R_VMOPT_SETMEM_ADDR,		//���������ڴ�Ϊ����
	ZL_R_VMOPT_SETMEM_MEMBLOCK,	//���������ڴ�Ϊ�ڴ��
}ZENGL_RUN_VMEM_OP_TYPE; //�����ڴ��������

typedef enum _ZENGL_RUN_HASH_TYPES{
	ZL_R_HASH_TYPE_MODULE_TABLE = 0,	//������ģ�鶯̬��������hash������
	ZL_R_HASH_TYPE_MOD_FUN_TABLE = 1,	//������ģ�麯����̬�����Ӧhash������
	ZL_R_HASH_TYPE_EXTRA_DATA_TABLE = 2,//�û�Ϊ�������ṩ�Ķ������ݹ��ɵĶ�̬��������Ӧ��hash������
}ZENGL_RUN_HASH_TYPES;

typedef struct _ZENGL_RUN_MEM_POOL_POINT_TYPE{
	ZL_BOOL isvalid;
	ZL_VOID * point;
	ZL_INT size;
}ZENGL_RUN_MEM_POOL_POINT_TYPE;

typedef struct _ZENGL_RUN_MEM_POOL_TYPE{
	ZL_BOOL isInit;
	ZL_INT size;
	ZL_INT totalsize; //�ڴ�ض�̬������ܴ�С�������ڴ������Ĵ�С
	ZL_INT count;
	ZENGL_RUN_MEM_POOL_POINT_TYPE * points;
}ZENGL_RUN_MEM_POOL_TYPE; //���������ڴ�����Ͷ��壬points��Աָ��һ��ָ�����飬�������Ž������������ָ�롣

typedef struct _ZENGL_RUN_MEM_FREE_POOL_TYPE{
	ZL_INT size;
	ZL_INT count;
	ZL_INT mempool_index; //��Ϊ�����freesָ�����ڴ���е�����
	ZL_INT * frees; //�ڴ��ͷųصĶ�̬����
}ZENGL_RUN_MEM_FREE_POOL_TYPE; //�ڴ��ͷųص����Ͷ��壬������ڴ���б��ͷŵ�������������������

typedef struct _ZENGL_RUN_INST_OP_DATA{
	ZENGL_RUN_INST_OP_DATA_TYPE type;
	union{
		ZENGL_RUN_REG_TYPE reg; //�Ĵ�����ö��������
		ZL_INT str_Index; //�ַ�����Ϣ�ڽ������ַ������е�����
		ZL_INT mem; //�ڴ�Ѱֵַ��
		ZL_LONG num;//ʹ�ó�����������64λ��ֲ
		ZL_DOUBLE floatnum; //������
	}val;
}ZENGL_RUN_INST_OP_DATA; //��������ָ�������

typedef struct _ZENGL_RUN_INST_LIST_MEMBER{
	ZL_BOOL isvalid;
	ZL_INT pc;
	ZL_INT nodenum; //���ָ���Ӧ�ı�������AST�Ľڵ�ţ����ڴ������
	ZENGL_RUN_INST_TYPE type;
	ZENGL_RUN_INST_OP_DATA src;
	ZENGL_RUN_INST_OP_DATA dest;
}ZENGL_RUN_INST_LIST_MEMBER; //������ָ��е���ָ������Ͷ���

typedef struct _ZENGL_RUN_INST_LIST{
	ZL_BOOL isInit;
	ZL_INT size;
	ZL_INT count;
	ZL_INT mempool_index; //���ֶ�Ϊ�����instsָ�����ڴ���е�������
	ZENGL_RUN_INST_LIST_MEMBER * insts;
}ZENGL_RUN_INST_LIST; //��������ָ�

typedef struct _ZENGL_RUN_INFO_STRING_TYPE{
	ZL_CHAR * str; //��Ϣ���ַ���ָ��
	ZL_INT mempool_index; //��Ϊ�����strָ�����ڴ���е�����
	ZL_INT size;  //�ַ����Ķ�̬��С
	ZL_INT count; //��ŵ��ַ���
	ZL_INT cur;  //��ǰ�α�
}ZENGL_RUN_INFO_STRING_TYPE; //�����ͨ��ӡ��Ϣ�������Ϣ������

typedef struct _ZENGL_RUN_RUNTIME_OP_DATA{
	ZENGL_RUN_RUNTIME_OP_DATA_TYPE runType; //������������ʱ����
	ZL_INT str_Index;	//str_Index������struct���strָ�����ڴ���е����������Լӿ�һЩ�ڴ�صĲ�����
	ZL_INT memblk_Index;//memblk_Index������struct���memblock���ڴ���е�������
	struct{
		ZL_LONG dword; //ʹ��long���ͣ�����64λ��ֲ
		ZL_VOID * str;
		ZL_VOID * memblock;
		ZL_DOUBLE qword;
	}val;
}ZENGL_RUN_RUNTIME_OP_DATA; //zenglrun_main.c��zenglrun_RunInsts���������ڴ��Դ����������ʱ�������Լ��Ĵ������ֵ�Ľṹ����

typedef struct _ZENGL_RUN_INST_DATA_STRING_POOL{
	ZL_BOOL isInit;
	ZL_INT size;
	ZL_INT count;
	ZL_INT mempool_index; //�����ptrָ�����ڴ���е�����
	ZL_CHAR * ptr;
}ZENGL_RUN_INST_DATA_STRING_POOL; //ָ��������ַ����أ����ָ���е��ַ����������������ַ������С�

typedef struct _ZENGL_RUN_VIRTUAL_MEM_STRUCT{
	ZENGL_RUN_RUNTIME_OP_DATA_TYPE runType; //�����ڴ��в�����������ʱ����
	ZL_BOOL isvalid; //�ж��ڴ��Ƿ�ռ��
	ZL_INT str_Index;	//str_Index������struct���strָ�����ڴ���е����������Լӿ�һЩ�ڴ�صĲ�����
	ZL_INT memblk_Index;//memblk_Index������struct���memblock���ڴ���е�������
	struct{
		ZL_CHAR byte;
		ZL_WCHAR_T word;
		ZL_LONG dword; //ʹ��long���ͣ�����64λ��ֲ
		ZL_VOID * str;
		ZL_VOID * memblock;
		ZL_DOUBLE qword;
	}val;
}ZENGL_RUN_VIRTUAL_MEM_STRUCT; //ÿ�������ڴ浥Ԫ�������ջ��Ԫ���ڴ�ṹ�����Ͷ��塣

typedef struct _ZENGL_RUN_VIRTUAL_MEM_LIST{
	ZL_BOOL isInit;
	ZL_INT size;
	ZL_INT count;
	ZL_INT nncount; //(Non-NONE value count): ͳ������֮����ڴ�����ж��ٸ���NONE���͵ĳ�Ա
	ZL_INT refcount; //�ڴ������ü�������Ҫ��������֮��Ķ�̬������ڴ�顣
	ZL_INT mempool_index; //�����mem_arrayָ�����ڴ���е�����
	ZENGL_RUN_VIRTUAL_MEM_STRUCT * mem_array;
	ZENGL_RUN_HASH_ARRAY hash_array; // ��ʹ���ַ�����Ϊkey������mem_array�еĳ�Աʱ���ȸ���hash_array��keyתΪ��������ֵ����������ֵ�����ʾ���ĳ�Ա
}ZENGL_RUN_VIRTUAL_MEM_LIST;

typedef struct _ZENGL_RUN_MODULE_TABLE_MEMBER{
	ZL_BOOL isvalid;
	ZL_INT ID;
	ZL_INT strIndex; //ģ�������ַ������е�����
	ZL_VM_API_MOD_INIT_FUNC init_func; //�û��Զ����ģ���ʼ������
	ZL_INT next;
}ZENGL_RUN_MODULE_TABLE_MEMBER;

typedef struct _ZENGL_RUN_MODULE_TABLE{
	ZL_BOOL isInit;
	ZL_INT size;
	ZL_INT count;
	ZL_INT mempool_index; //�����modulesָ�����ڴ���е�����
	ZENGL_RUN_MODULE_TABLE_MEMBER * modules;
}ZENGL_RUN_MODULE_TABLE; //ģ�鶯̬���鶨��

typedef struct _ZENGL_RUN_MOD_FUN_TABLE_MEMBER{
	ZL_BOOL isvalid;
	ZL_INT moduleID;
	ZL_INT strIndex; //ģ�麯�������ַ������е�����
	ZL_VM_API_MOD_FUN_HANDLE handle; //�û��Զ����ģ�麯��������
	ZL_INT next;
}ZENGL_RUN_MOD_FUN_TABLE_MEMBER;

typedef struct _ZENGL_RUN_MOD_FUN_TABLE{
	ZL_BOOL isInit;
	ZL_INT size;
	ZL_INT count;
	ZL_INT mempool_index; //�����mod_funsָ�����ڴ���е�����
	ZENGL_RUN_MOD_FUN_TABLE_MEMBER * mod_funs;
}ZENGL_RUN_MOD_FUN_TABLE; //ģ�麯����̬����Ķ���

typedef struct _ZENGL_RUN_EXTRA_DATA_TABLE_MEMBER{
	ZL_BOOL isvalid;
	ZL_INT strIndex; //���������������ַ������е�����
	ZL_VOID * point; //��������ָ��
	ZL_INT next;
}ZENGL_RUN_EXTRA_DATA_TABLE_MEMBER; //�û��������ݶ�̬�����г�Ա�Ķ���

typedef struct _ZENGL_RUN_EXTRA_DATA_TABLE{
	ZL_BOOL isInit;
	ZL_INT size;
	ZL_INT count;
	ZL_INT mempool_index;
	ZENGL_RUN_EXTRA_DATA_TABLE_MEMBER * extras;
}ZENGL_RUN_EXTRA_DATA_TABLE; //�û��������ݶ�̬����Ķ���

/********************************************************************************
		�����Ǻ��������������صĽṹ���ö�ٵȶ���
********************************************************************************/

/********************************************************************************
		�����Ǻ��������������صĽṹ���ö�ٵȶ���
********************************************************************************/

#define ZL_DBG_BREAK_POINT_SIZE 10 //��Ŷϵ�Ķ�̬�����ʼ���Ͷ�̬���ݵĴ�С

typedef enum _ZENGL_DEBUG_FLAG{
	ZL_DBG_FLAG_RESTORE_BREAK = 1, //��Ҫ�ָ��ϵ��־
	ZL_DBG_FLAG_SET_SINGLE_STEP_IN = 2, //��Ҫ���õ��������־
	ZL_DBG_FLAG_SET_SINGLE_STEP_OUT = 4, //��Ҫ���õ���������־
}ZENGL_DEBUG_FLAG;

typedef struct _ZENGL_DEBUG_BREAK_POINT_MEMBER{
	ZL_BOOL isvalid;
	ZL_BOOL disabled; //�Ƿ���öϵ�
	ZL_BOOL needRestore; //�Ƿ���Ҫ�ָ��ϵ�
	ZL_INT pc; //�ϵ��ڽ������ж�Ӧ��ָ��PCֵ
	ZL_INT line; //�ϵ������к�
	ZL_INT count; //�ϵ�ִ�д���
	ZL_CHAR * filename; //�ϵ����ڵ�Դ�ű��ļ���
	ZL_CHAR * condition; //�����ϵ��Ӧ���������
	ZL_CHAR * log; //��־�ϵ��Ӧ�ĵ�������
	ZL_LONG orig_inst_src_num; //�ϵ��Ӧ��ԭָ���src���numֵ
	ZENGL_RUN_INST_TYPE orig_inst_type; //�ϵ��Ӧ��ԭָ��
}ZENGL_DEBUG_BREAK_POINT_MEMBER;

typedef struct _ZENGL_DEBUG_BREAK_POINT_LIST{
	ZL_BOOL isInit;
	ZL_INT size;
	ZL_INT count;
	ZL_INT mempool_index;
	ZENGL_DEBUG_BREAK_POINT_MEMBER * members;
}ZENGL_DEBUG_BREAK_POINT_LIST; //�������ϵ��б�

typedef struct _ZENGL_DEBUG_SINGLE_BREAK{
	ZL_BOOL isvalid;
	ZL_INT compare_pc;
	ZL_INT orig_pc;
	ZENGL_RUN_INST_TYPE orig_inst_type;
}ZENGL_DEBUG_SINGLE_BREAK; //����������ִ��ʱ���жϽṹ

/********************************************************************************
		�����Ǻ��������������صĽṹ���ö�ٵȶ���
********************************************************************************/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*zengl�������ṹ�嶨��*/
typedef struct _ZENGL_COMPILE_TYPE
{
	/*�������ĳ�Ա�б�*/
	ZENGL_TOKEN_INFO_TYPE tokenInfo; //��ŵ�ǰɨ�赽��token��Ϣ
	ZENGL_SOURCE_TYPE source; //source��Ա�����Ͷ��壬�������Ҫ�������ļ����ļ�ָ�룬�ļ�������Ϣ��
	ZENGL_MEM_POOL_TYPE mempool; //���������ڴ��
	jmp_buf jumpBuffer; //����ʱͳһ��ת��λ��
	ZL_INT line_no; //ɨ����ɨ�赽���к�
	ZL_INT col_no;  //ɨ����ɨ�赽���к�
	ZL_BOOL is_inConvChr; //���ڽ����ַ����е�'\'б��ת���ַ���
	ZL_BOOL is_inDefRsv;  //�����ж��Ƿ���def�ؼ��ֺ��棬def����ĳ���������DEF����(��Ϊ������Ͳ�֪��ԭ��������ʲô�ˣ������Ĵ�����ʾ��Ϣ�ͻ�����)��ֻ��def����ĳ���ֵ��DEF����
	ZL_INT basesize; //�������ṹ��Ļ�����С
	ZL_INT totalsize; //������������ڴ�صĴ�С���ڴ���з�����ڴ�Ĵ�С�ܺ�
	ZL_CLOCK_T start_time; //��������ʼִ��ʱ��ʱ�䣬����Ϊ��λ
	ZL_CLOCK_T end_time; //����������ʱ��ʱ�䣬����Ϊ��λ
	ZL_CLOCK_T total_time; //ִ�н���ʱ����ʱ�䣬����Ϊ��λ
	ZL_CLOCK_T total_print_time; //��������ӡ������Ϣ�����ĵ�ʱ�䣬����Ϊ��λ
	ZENGL_INFO_STRING_TYPE infoFullString; //�������������ľ������������ͨ��ӡ��Ϣ�ַ���
	ZENGL_INFO_STRING_TYPE errorFullString; //�������������ľ���������Ĵ�����Ϣ���ַ���
	ZL_CONST ZL_CHAR ** reserveString; //���ֹؼ��ֶ���
	ZL_CONST ZL_CHAR ** TokenOperateString; //���в���������������ַ�����Ϣ
	ZL_INT TokenOperateStringCount; //TokenOperateString��Ա�ĸ���
	ZENGL_STRING_POOL_TYPE def_StringPool; //def�궨�峣�����ַ�����
	ZENGL_DEF_TABLE def_table; //�궨�嶯̬���顣
	ZENGL_DEF_LOOKUP_TYPE def_lookup; // �ýṹ���ڴ洢��def��ֵ��ѯ��ص��û��Զ��庯�����Լ��洢���ݸò�ѯ������ѯ�����ĺ�ֵ���
	ZL_BOOL isinCompiling; //�жϱ������Ƿ����ڱ���
	ZL_BOOL isDestroyed; //�жϱ��������ڴ�ص���Դ�Ƿ��ͷ���
	ZL_BOOL isReUse;	//�û��Ƿ���Ҫ�����������֮ǰ�Ѿ�����õ���Դ�������Ҫ��ִ�о���ı������������ֱ��ִ��֮ǰ����õ�ָ�����
	/*��zengl_symbol.c���ű���صĳ�Ա*/
	ZL_INT HashTable[ZL_SYM_HASH_TOTAL_SIZE]; //hash���д���˸��ֶ�̬����Ԫ�ص�����ֵ��
	ZENGL_LINECOL_TABLE LineCols; //���кű�Ķ�̬���顣
	ZL_CHAR PathFileName[ZL_FILE_MAX_PATH];	//inc�����ļ�ʱ����Ҫ�����ļ������ɾ���·����
	ZENGL_FILE_STACKLIST_TYPE FileStackList; //�����ص��Ⱥ�˳����inc���ص��ļ���Ϣ�Ķ�ջ��
	ZENGL_SYM_GLOBAL_TABLE SymGlobalTable;	//ȫ�ֱ������ű�
	ZENGL_SYM_LOCAL_TABLE SymLocalTable;	//�ֲ�����(������������)���ű�
	ZENGL_SYM_CLASS_TABLE SymClassTable; //����ű�(�������Ϣ�Ķ�̬����)
	ZENGL_SYM_CLASSMEMBER_TABLE SymClassMemberTable; //���Ա���ű�(����������Ա��Ϣ�Ķ�̬����)
	ZENGL_SYM_FUN_TABLE SymFunTable; //�������ű�
	ZENGL_SYM_SELF_CLASS_TABLE SymSelfClassTable; // self�ڵ�����Ϣ��̬����
	/*��zengl_assemble.c������������صĳ�Ա*/
	ZENGL_ASM_GENCODE_STRUCT gencode_struct; //��zengl_AsmGenCodes�����л��õ���һЩ������ͳһ����һ���ṹ����
	ZENGL_ASM_STACKLIST_TYPE AsmGCStackList; //assemble���ɻ�����ʱ��Ҫ�õ��Ľ���ڲ�Ƕ������Ķ�ջ
	ZENGL_ASM_LOOP_STACKLIST_TYPE AsmGCLoopStackList; // ͨ��ģ���ջ���ѭ������������������ĵݹ����
	ZL_INT AsmGCAddrNum; //ifadr,ifend,foradr,forend�ȵļ�������
	ZL_BOOL AsmGCIsInClass; //�ж��Ƿ�������class��ṹ�Ļ���������С�
	ZENGL_RUN_INST_OP_DATA memDataForDot; //�������ɵ�������Ļ��ָ��
	/*��zengl_ld.c���ӳ�����صĳ�Ա*/
	ZENGL_LD_ADDRLIST_TYPE LDAddrList; //���ӵ�ַ��̬����
	/*zengl_parser.c�﷨������صĳ�Ա*/
	ZENGL_AST_TYPE AST_nodes; //�����﷨����̬���飬�����������е��﷨���ڵ���Ϣ
	ZENGL_TOKEN_STRING_POOL Token_StringPool; //���token�ַ�����Ϣ���ַ�����
	ZL_INT parser_curnode; //�﷨����ǰɨ���α�
	ZENGL_PARSER_EXP_STRUCT exp_struct; //��zengl_express�����л��õ���һЩ������ͳһ����һ���ṹ����
	ZENGL_PARSER_OP_LEVEL_STACKLIST_TYPE parser_opLevel_stackList; //�﷨����ʱ��Ҫ�õ������ȼ���ջ
	ZENGL_AST_SCAN_STACKLIST_TYPE AST_TreeScanStackList; //�﷨��ɨ���ջ

	/*��������ص��Զ��庯��*/
	ZL_VOID (* exit)(ZL_VOID * ,ZENGL_ERRORNO , ...); //�������˳����������������صĳ�����Ϣ ��Ӧ zengl_exit
	ZL_VOID (* memFreeAll)(ZL_VOID * VM_ARG); //�������ͷ��ڴ����Դ ��Ӧ zengl_freeall
	ZL_CHAR (* getNextchar)(ZL_VOID *); //���ļ��л�ȡ��һ���ַ��ĺ��� ��Ӧ zengl_getNextchar
	ZENGL_TOKENTYPE (* getToken)(ZL_VOID *); //��ȡtoken��Ϣ�ĺ��� ��Ӧ zengl_getToken
	ZL_VOID (* makeTokenInfoLineCol)(ZL_VOID * VM_ARG,ZL_INT line,ZL_INT col,ZL_CHAR * filename); //����token�����кţ��ļ�����Ϣ ��Ӧ zengl_makeTokenInfoLineCol
	ZL_VOID (* makeTokenStr)(ZL_VOID * VM_ARG,ZL_CHAR ch); //����token���ַ�����Ϣ ��Ӧ zengl_makeTokenStr
	ZL_VOID * (* memAlloc)(ZL_VOID * VM_ARG , ZL_INT size); //���������ڴ���з����ڴ溯�� ��Ӧ zengl_malloc
	ZL_VOID * (* memReAlloc)(ZL_VOID * VM_ARG , ZL_VOID * point , ZL_INT size); //�����ڴ����ĳ��ָ����ڴ��С ��Ӧ zengl_realloc
	ZL_VOID (* memFreeOnce)(ZL_VOID * VM_ARG,ZL_VOID * point); //�ͷű������ڴ���е�ĳ��ָ�� ��Ӧ zengl_freeonce
	ZL_VOID (* ungetchar)(ZL_VOID * VM_ARG); //��Դ�ű��ַ�ɨ���α����һ��ͬʱ���кż�һ ��Ӧ zengl_ungetchar
	ZL_VOID (* convertStrToDec)(ZL_VOID * VM_ARG); //���ַ���תΪ��Ч��ʮ������ ��Ӧ zengl_convertStrToDec
	ZL_VOID (* freeTokenStr)(ZL_VOID * VM_ARG); //�޸��α�ȳ�Ա���ͷŵ�ǰɨ��token�Ķ�̬�ַ�����Ϣ���Թ���һ��ɨ��tokenʱʹ�� ��Ӧ zengl_freeTokenStr
	ZL_VOID (* convertOctalToDec)(ZL_VOID * VM_ARG); //����ǰɨ��token�ɰ˽���תΪʮ���� ��Ӧ zengl_convertOctalToDec
	ZL_VOID (* convertHexToDec)(ZL_VOID * VM_ARG); //��ɨ��token��ʮ�����Ƹ�ʽתΪʮ���Ƹ�ʽ ��Ӧ zengl_convertHexToDec
	ZL_VOID (* makeTokenStrForString)(ZL_VOID * VM_ARG,ZL_CHAR ch); //����ר�Ž����ַ������͵�token ��Ӧ zengl_makeTokenStrForString
	ZENGL_TOKENTYPE (* lookupReserve)(ZL_VOID * VM_ARG , ZENGL_TOKENTYPE token); //���ҹؼ��ֵ����� ��Ӧ zengl_lookupReserve
	ZL_VOID (* AddDefConst)(ZL_VOID * VM_ARG); //����ͺ��Ӧ��ֵ���뵽��ϣ���def�궨��Ķ�̬������ ��Ӧ zengl_AddDefConst
	ZL_INT (* DefPoolAddString)(ZL_VOID * VM_ARG , ZL_CHAR * src); //������src��ӵ�def_StringPool�ַ������� ��Ӧ zengl_DefPoolAddString
	ZL_VOID (* initDefStringPool)(ZL_VOID * VM_ARG); //��ʼ��def�������ַ����� ��Ӧ zengl_initDefStringPool
	ZL_VOID (* insert_HashTableForDef)(ZL_VOID * VM_ARG , ZL_INT nameIndex, ZENGL_TOKENTYPE tokentype,ZL_INT valIndex,
								ZL_INT line,ZL_INT col); //��def����ĺ����ͺ��Ӧ��ֵ���뵽��̬�����У�ͬʱ���������뵽��ϣ���� ��Ӧ zengl_insert_HashTableForDef
	ZL_CHAR * (* DefPoolGetPtr)(ZL_VOID * VM_ARG , ZL_INT index); //��������ֵ���ַ������в��Һ궨���ַ�����ָ����Ϣ ��Ӧ zengl_DefPoolGetPtr
	ZL_INT (* insert_DefTable)(ZL_VOID * VM_ARG,ZL_INT nameIndex,ZENGL_TOKENTYPE tokentype,ZL_INT valIndex,ZL_INT line,ZL_INT col); //��ÿ��def����ĺ����ƺͺ��Ӧ��ֵ���뵽def_table��̬������ ��Ӧ zengl_insert_DefTable
	ZL_VOID (* initDefTable)(ZL_VOID * VM_ARG); //��ʼ��def_table��def�궨��Ķ�̬���飩 ��Ӧ zengl_initDefTable
	ZL_VOID (* include_file)(ZL_VOID * VM_ARG); //���ʷ�ɨ����ɨ�赽inc�ؼ���ʱ���ͻ���øú����������ļ�����Ӧ zengl_include_file
	ZL_CHAR * (* makePathFileName)(ZL_VOID * VM_ARG,ZL_CHAR * filename); //inc�����ļ�ʱ���õ��ļ�������·����Ϣ ��Ӧ zengl_makePathFileName
	ZL_VOID (* push_FileStack)(ZL_VOID * VM_ARG , ZENGL_SOURCE_TYPE * src,ZL_INT line,ZL_INT col, ZL_CHAR * inc_filename); //��ԭ�����ļ���Ϣѹ��ջ����Ӧ zengl_push_FileStack
	ZL_VOID (* pop_FileStack)(ZL_VOID * VM_ARG , ZENGL_SOURCE_TYPE * dest); //���ϴ�ѹ��ջ���ļ���Ϣ�������ӵ������ļ���Ϣ��ָ�ԭ�ļ���ɨ�� ��Ӧ zengl_pop_FileStack
	ZL_VOID (* initFileStack)(ZL_VOID * VM_ARG); //��ʼ��inc�����ļ��Ķ�ջ ��Ӧ zengl_initFileStack
	ZENGL_TOKENTYPE (* ReplaceDefConst)(ZL_VOID * VM_ARG, ZENGL_TOKENTYPE token); //�������滻���� ��Ӧ zengl_ReplaceDefConst
	ZL_INT (* lookupDefTable)(ZL_VOID * VM_ARG, ZL_CHAR * name); //���ݲ���name�ڹ�ϣ���в������� ��Ӧ zengl_lookupDefTable
	ZL_CHAR * (* makeInfoString)(ZL_VOID * VM_ARG,ZENGL_INFO_STRING_TYPE * infoStringPtr , ZL_CONST ZL_CHAR * format , ZENGL_SYS_ARG_LIST arglist); //���ɸ�ʽ����Ϣ�ַ��� ��Ӧ zengl_makeInfoString
	ZL_VOID (* freeInfoString)(ZL_VOID * VM_ARG,ZENGL_INFO_STRING_TYPE * infoStringPtr); //�ͷŸ�ʽ����Ϣ�ַ������α�ʹ�С��Ϣ ��Ӧ zengl_freeInfoString
	ZL_VOID (* info)(ZL_VOID * VM_ARG , ZL_CONST ZL_CHAR * format , ...); //�����ɸ�ʽ����Ϣ�ַ������ٵ����û��Զ���ĺ������Ӷ����ַ������ݸ��û����� ��Ӧ zengl_info
	ZL_VOID (* init)(ZL_VOID * VM_ARG,ZL_CHAR * script_file,ZENGL_EXPORT_VM_MAIN_ARGS * vm_main_args); //��������ʼ�� ��Ӧ zengl_init
	ZL_INT (* main)(ZL_VOID * VM_ARG,ZL_CHAR * script_file,ZENGL_EXPORT_VM_MAIN_ARGS * vm_main_args); //����������ں��� ��Ӧ zengl_main
	/*������zengl_symbol.c�е���غ���*/
	ZL_INT (* hash) (ZL_VOID * VM_ARG , ZL_CHAR * key ); //hash��ϣɢ���㷨 ��Ӧ zengl_Sym_Hash
	ZL_INT (* insert_LineCol)(ZL_VOID * VM_ARG , ZL_INT line , ZL_INT col,ZL_CHAR * filename); //��line,col���кţ��Լ����к����ڵ��ļ������뵽LineCols���кŶ�̬������ ��Ӧ zengl_insert_LineCol
	ZL_VOID (* initLineColTable)(ZL_VOID * VM_ARG); //��ʼ��LineCols���кŶ�̬���飬���ڼ�¼���ű���ÿ��ȫ�ַ��ų��ֹ������к���Ϣ����Ӧ zengl_initLineColTable
	ZL_VOID (* buildSymTable)(ZL_VOID * VM_ARG); //�齨���ű������ʽ����Ӧ zengl_buildSymTable
	ZL_VOID (* ScanInsertGlobalSym)(ZL_VOID * VM_ARG,ZL_INT arg_nodenum); //ɨ�貢����ȫ�ֱ������ű� ��Ӧ zengl_ScanInsertGlobalSym
	ZL_VOID (* SymInsertDotID)(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT parentNum); //�жϵ��������Ľڵ��ǳ�Ա�����໹��ȫ�ֱ���������ȫ�ֱ�������ȫ�ַ��ű��С���Ӧ zengl_SymInsertDotID
	ZL_INT (* SymInsertHashTableForGlobal)(ZL_VOID * VM_ARG,ZL_INT nodenum); //��ȫ�ֱ������뵽��ϣ���ȫ�ַ��ű�̬�����С���Ӧ zengl_SymInsertHashTableForGlobal
	ZL_INT (* SymInsertGlobalTable)(ZL_VOID * VM_ARG,ZL_INT nameIndex,ZL_INT line,ZL_INT col,ZL_CHAR * filename); //��ĳ��������ʾ�����뵽ȫ�ֱ������ű����ȷ���ñ�����Ӧ���ڴ��ַ ��Ӧ zengl_SymInsertGlobalTable
	ZL_VOID (* SymInitGlobalTable)(ZL_VOID * VM_ARG); //��ʼ��ȫ�ֱ������ű� ��Ӧ zengl_SymInitGlobalTable
	ZL_INT (* SymInsertHashTableForClass)(ZL_VOID * VM_ARG,ZL_INT nodenum); //��nodenum�������ڵĽڵ���뵽��ϣ�������ű����ڵĶ�̬������ ��Ӧ zengl_SymInsertHashTableForClass
	ZL_INT (* SymInsertClassTable)(ZL_VOID * VM_ARG,ZL_INT nameIndex,ZL_INT nodenum,ZL_INT line,ZL_INT col,ZL_CHAR * filename); //�������͸��ඨ�����ڵ����к�,�ļ���,�ڵ����Ϣ���뵽����ű��Ӧ�Ķ�̬������ ��Ӧ zengl_SymInsertClassTable
	ZL_VOID (* SymInitClassTable)(ZL_VOID * VM_ARG);//��ʼ������ű��Ӧ�Ķ�̬���� ��Ӧ zengl_SymInitClassTable
	ZL_INT (* SymInsertHashTableForFun)(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT classid); //�������ڵ���Ϣ���뵽������̬�����У�������̬������������浽HashTable�� ��Ӧ zengl_SymInsertHashTableForFun
	ZL_INT (* SymInsertFunTable)(ZL_VOID * VM_ARG,ZL_INT nameIndex,ZL_INT classid,ZL_INT line,ZL_INT col,ZL_CHAR * filename); //���������ͺ������ڵ����кŵ���Ϣ���뵽�������ű��Ӧ�Ķ�̬�����С���Ӧ zengl_SymInsertFunTable
	ZL_VOID (* SymInitFunTable)(ZL_VOID * VM_ARG);	//��ʼ���������ű��Ӧ�Ķ�̬���� ��Ӧ zengl_SymInitFunTable
	ZL_VOID (* ScanInsertClassMemberSym)(ZL_VOID * VM_ARG,ZL_INT arg_nodenum,ZL_INT classid); //ɨ�����Ա���������Ա�������Ա���ű��Լ����ຯ�����뺯�����ű��С���Ӧ zengl_ScanInsertClassMemberSym
	ZL_INT (* SymLookupClass)(ZL_VOID * VM_ARG,ZL_INT nodenum); //���ݽڵ�Ų�����ID��Ϣ ��Ӧ zengl_SymLookupClass
	ZL_INT (* SymLookupClassByName)(ZL_VOID * VM_ARG,ZL_CHAR * name); //��������������ID��Ϣ ��Ӧ zengl_SymLookupClassByName
	ZL_INT (* SymLookupClassMember)(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT parent_classid); //��SymClassMemberTable�в���parent_classid��Ӧ����ĳ�Աnodenum����Ϣ ��Ӧ zengl_SymLookupClassMember
	ZL_INT (* SymInsertHashTableForClassMember)(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT index,ZL_INT classid,ZL_INT cls_stmt_classid); //��nodenum���Ա���ڵĽڵ���뵽��ϣ������Ա���ű����ڵĶ�̬������ ��Ӧ zengl_SymInsertHashTableForClassMember
	ZL_INT (* SymInsertClassMemberTable)(ZL_VOID * VM_ARG,ZL_INT nameIndex,ZL_INT index,ZL_INT classid,ZL_INT cls_stmt_classid,ZL_INT nodenum); //�����Ա�������Ա����ṹ�е�����ֵ��������ID����Ϣ�������Ա���ű��� ��Ӧ zengl_SymInsertClassMemberTable
	ZL_VOID (* SymInitClassMemberTable)(ZL_VOID * VM_ARG); //��ʼ�����Ա���ű��Ӧ�Ķ�̬���� ��Ӧ zengl_SymInitClassMemberTable
	ZENGL_RUN_INST_OP_DATA (* SymLookupID)(ZL_VOID * VM_ARG,ZL_INT nodenum); //ͨ���ڵ����������Ҹýڵ�ı�����ʾ�����Զ���������ڴ��е��ڴ��ַ ��Ӧ zengl_SymLookupID
	ZL_VOID (* SymInsertClassID)(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT classid); //Ϊnodenum�ڵ�����classid��id ��Ӧ zengl_SymInsertClassID
	ZL_INT (* SymLookupID_ForDot)(ZL_VOID * VM_ARG,ZL_INT nodenum); //����nodenum��Ӧ�ڵ��classidֵ����Ҫ�������ɵ�������Ļ��ָ��ʱ ��Ӧ zengl_SymLookupID_ForDot
	ZL_INT (* SymLookupID_ByName)(ZL_VOID * VM_ARG,ZL_CHAR * name); //ͨ���������Ʋ��ұ�����ʾ�����Զ���������ڴ��е��ڴ��ַ����Ҫ���ڶ����API�ӿ��� ��Ӧ zengl_SymLookupID_ByName
	ZL_INT (* SymLookupFun)(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT classid);	//���ݽڵ�Ų��Һ���ID��Ϣ ��Ӧ zengl_SymLookupFun
	ZL_INT (* SymLookupFunByName)(ZL_VOID * VM_ARG,ZL_CHAR * name,ZL_INT classid); //���ݺ������Ʋ��Һ���ID��Ϣ ��Ӧ zengl_SymLookupFunByName
	ZL_VOID (* SymScanFunArg)(ZL_VOID * VM_ARG,ZL_INT nodenum); //ʹ��ASTɨ���ջ��ɨ���﷨���к����Ĳ��� ��Ӧ zengl_SymScanFunArg
	ZL_VOID (* SymScanFunGlobal)(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT global_nodenum); //ʹ��ASTɨ���ջ��ɨ���﷨���к�����global�ؼ���������ȫ�ֱ��� ��Ӧ zengl_SymScanFunGlobal
	ZL_VOID (* SymScanClassStatement)(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT cls_statement_nodenum); //ɨ������������������ı��� ��Ӧ zengl_SymScanClassStatement
	ZENGL_STATES (* SymScanDotForClass)(ZL_VOID * VM_ARG,ZL_INT nodenum, ZENGL_ASM_LOOP_STACK_TYPE ** loopStackTopArg); //ɨ��������ʱ�Ľڵ㣬�����ԱתΪ���������ѹ��ջ�� ��Ӧ zengl_SymScanDotForClass
	ZL_VOID (* SymScanUseRsv)(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT use_nodenum); //ʹ��ASTɨ���ջ��ɨ���﷨����use�ؼ���������ģ���� ��Ӧ zengl_SymScanUseRsv
	ZL_VOID (* SymScanFunLocal)(ZL_VOID * VM_ARG,ZL_INT nodenum); //ʹ��ASTɨ���ջ��ɨ���﷨���к����ľֲ����� ��Ӧ zengl_SymScanFunLocal
	ZL_BOOL (* SymInsertHashTableForLocal)(ZL_VOID * VM_ARG,ZL_INT nodenum,ZENGL_SYM_ENUM_LOCAL_TYPE type); //���ֲ������Ͳ������뵽�ֲ��������ű��У��������ű�̬������������뵽��ϣ���� ��Ӧ zengl_SymInsertHashTableForLocal
	ZL_INT (* SymInsertLocalTable)(ZL_VOID * VM_ARG,ZL_INT nameIndex,ZENGL_SYM_ENUM_LOCAL_TYPE type); //���ֲ��������;ֲ����������Ͳ��뵽SymLocalTable��̬�����С���Ӧ zengl_SymInsertLocalTable
	ZL_VOID (* SymInitLocalTable)(ZL_VOID * VM_ARG); //��ʼ��SymLocalTable�ֲ����ű��Ӧ�Ķ�̬���� ��Ӧ zengl_SymInitLocalTable
	ZL_BOOL (* SymIsSelfToken)(ZL_VOID * VM_ARG, ZL_CHAR * token_name); // ����token�����ж��Ƿ���self��token ��Ӧ zengl_SymIsSelfToken
	ZL_BOOL (* SymAddNodeNumToSelfClassTable)(ZL_VOID * VM_ARG, ZL_INT self_nodenum); // ���ĳ����ʶ���ڵ���self�ڵ㣬�򽫸ýڵ��AST�ڵ�ż��뵽self�ڵ�����Ϣ��̬������ ��Ӧ zengl_SymAddNodeNumToSelfClassTable
	ZL_VOID (* SymPrintTables)(ZL_VOID * VM_ARG);	  //��ӡ���ű� ��Ӧ zengl_SymPrintTables
	/*������zengl_assemble.c�е���غ���*/
	ZL_VOID (* buildAsmCode)(ZL_VOID * VM_ARG); //�齨�����������ʽ ��Ӧ zengl_buildAsmCode
	ZL_VOID (* AsmGenCodes)(ZL_VOID * VM_ARG,ZL_INT nodenum); //�ú�������AST�����﷨���Ľڵ�������ĳ�ڵ�תΪ�����롣��Ӧ zengl_AsmGenCodes
	ZL_INT (* AsmGCStackPush)(ZL_VOID * VM_ARG,ZL_INT num,ZENGL_ASM_STACK_ENUM type); //������ѹ�����ջ ��Ӧ zengl_AsmGCStackPush
	ZL_INT (* AsmGCStackPop)(ZL_VOID * VM_ARG,ZENGL_ASM_STACK_ENUM type,ZL_BOOL isremove); //�������ջ ��Ӧ zengl_AsmGCStackPop
	ZL_VOID (* AsmGCStackInit)(ZL_VOID * VM_ARG); //����ջ��ʼ�� ��Ӧ zengl_AsmGCStackInit
	//ZL_VOID (* AsmGCElif)(ZL_VOID * VM_ARG,ZENGL_AST_CHILD_NODE_TYPE * ifchnum,ZL_INT num); //zengl_AsmGCElif������������elif������Ӧ�Ļ��ָ�� ��Ӧ zengl_AsmGCElif
	//ZL_VOID (* AsmGCBreak_Codes)(ZL_VOID * VM_ARG,ZL_INT nodenum); //break���Ļ��������� ��Ӧ zengl_AsmGCBreak_Codes
	//ZL_VOID (* AsmGCContinue_Codes)(ZL_VOID * VM_ARG,ZL_INT nodenum); //continue���Ļ��������� ��Ӧ zengl_AsmGCContinue_Codes
	ZL_VOID (* AsmScanCaseMinMax)(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_BOOL * hasminmax,ZL_INT * minarg,ZL_INT * maxarg,ZL_BOOL * hasdefault,
								ZENGL_ASM_CASE_JMP_TABLE * table); //ɨ��switch...case ���ҳ����е�case�����ֵ����Сֵ���Լ��ж��Ƿ���defaultĬ�Ͻڵ� ��Ӧ zengl_AsmScanCaseMinMax
	ZL_LONG (* GetNodeInt)(ZL_VOID * VM_ARG,ZL_INT nodenum); //���ؽڵ���ַ�����Ϣ��������ʽ ��Ӧ zengl_GetNodeInt
	ZL_VOID (* AsmAddCaseJmpTable)(ZL_VOID * VM_ARG,ZENGL_ASM_CASE_JMP_TABLE * table,ZL_LONG num); //��case��Ӧ�ıȽ�������ӵ���ת���� ��Ӧ zengl_AsmAddCaseJmpTable
	ZL_VOID (* AsmInitCaseJmpTable)(ZL_VOID * VM_ARG,ZENGL_ASM_CASE_JMP_TABLE * table); //��ʼ��switch case����ת�� ��Ӧ zengl_AsmInitCaseJmpTable
	ZL_VOID (* AsmSortCaseJmpTable)(ZL_VOID * VM_ARG,ZENGL_ASM_CASE_JMP_TABLE * table,ZL_INT nodenum); //��switch...case��ת����д�С��������� ��Ӧ zengl_AsmSortCaseJmpTable
	/*������zengl_ld.c�е���غ���*/
	ZL_INT (* LDAddrListSet)(ZL_VOID * VM_ARG,ZL_INT index,ZL_INT loc); //�������Ӷ�̬������ĳ����(α��ַ)��Ӧ��ʵ�ʻ�����λ�� ��Ӧ zengl_LDAddrListSet
	ZL_VOID (* LDAddrListInit)(ZL_VOID * VM_ARG); //���Ӷ�̬�����ʼ�� ��Ӧ zengl_LDAddrListInit
	ZL_VOID (* LDAddrListReplaceAll)(ZL_VOID * VM_ARG); //�����е�α��ַ�滻Ϊ��ʵ�Ļ�����λ�ã��Ӷ�������ӹ��� ��Ӧ zengl_LDAddrListReplaceAll
	/*������zengl_parser.c�е���غ���*/
	ZL_VOID (* ASTAddNode)(ZL_VOID * VM_ARG,ZENGL_TOKENTYPE token); //��token����AST�����﷨�� ��Ӧ zengl_ASTAddNode
	ZL_VOID (* initAST)(ZL_VOID * VM_ARG); //��ʼ��AST�����﷨���Ķ�̬���顣��Ӧ zengl_initAST
	ZL_INT (* TokenStringPoolAddStr)(ZL_VOID * VM_ARG , ZL_CHAR * src); //��token�ַ����ض�̬��������ַ����������ظ��ַ����ڻ����е�������Ϣ ��Ӧ zengl_TokenStringPoolAddStr
	ZL_VOID (* initTokenStringPool)(ZL_VOID * VM_ARG); //��ʼ��token�ַ����صĶ�̬���� ��Ӧ zengl_initTokenStringPool
	ZL_VOID (* printNode)(ZL_VOID * VM_ARG , ZL_INT nodenum); //��ӡtoken����Ϣ ��Ӧ zengl_printNode
	ZL_CHAR * (* getTokenStr)(ZL_VOID * VM_ARG , ZENGL_AST_NODE_TYPE * nodes,ZL_INT nodenum); //���ݽڵ�Ż�ȡtoken��Ӧ���ַ�����Ϣ ��Ӧ zengl_getTokenStr
	ZL_CHAR * (* TokenStringPoolGetPtr)(ZL_VOID * VM_ARG , ZL_INT index); //��������index��token�ַ����ػ����еõ���Ӧ���ַ�����Ϣ ��Ӧ zengl_TokenStringPoolGetPtr
	ZL_VOID (* printASTnodes)(ZL_VOID * VM_ARG); //��ӡ��AST�����﷨�������нڵ���Ϣ���������µ�ASTɨ���ջ�ķ������б���ɨ�衣��Ӧ zengl_printASTnodes
	ZL_VOID (* push_AST_TreeScanStack)(ZL_VOID * VM_ARG , ZL_INT nodenum); //ASTɨ���ջ��ѹջ���� ��Ӧ zengl_push_AST_TreeScanStack
	ZENGL_AST_SCAN_STACK_TYPE (* pop_AST_TreeScanStack)(ZL_VOID * VM_ARG,ZL_BOOL remove); //ASTɨ���ջ�ĵ���ջ���� ��Ӧ zengl_pop_AST_TreeScanStack
	ZL_VOID (* init_AST_TreeScanStack)(ZL_VOID * VM_ARG); //��ʼ��ASTɨ���ջAST_TreeScanStackList ��Ӧ zengl_init_AST_TreeScanStack
	ZL_VOID (* buildAST)(ZL_VOID * VM_ARG); //�ú���ͨ��ѭ���������������AST ��Ӧ zengl_buildAST
	ZL_INT (* statement)(ZL_VOID * VM_ARG); //����������print 'hello'֮�������AST�﷨�� ��Ӧ zengl_statement
	ZL_VOID (* KeywordCompleteDetect)(ZL_VOID * VM_ARG); //�ؼ��������Լ�飬����if�ṹ������endif��ƥ��� ��Ӧ zengl_KeywordCompleteDetect
	ZL_VOID (* TokenSyntaxDetect)(ZL_VOID * VM_ARG);	//use��global��break��continue���������������﷨��� ��Ӧ zengl_TokenSyntaxDetect
	ZL_VOID (* if_stmt)(ZL_VOID * VM_ARG,ZL_INT p); //if-elif-else��������AST�����﷨�������ɺ�������Ӧ zengl_if_stmt
	ZL_INT (* if_stmt_sequence)(ZL_VOID * VM_ARG); //if-elif-else-endif������������б��ʽ��AST�����﷨�������� ��Ӧ zengl_if_stmt_sequence
	ZL_VOID (* for_stmt)(ZL_VOID * VM_ARG,ZL_INT p); //for...endfor��������AST�����﷨�������ɺ�������Ӧ zengl_for_stmt
	ZL_INT (* for_stmt_sequence)(ZL_VOID * VM_ARG); //for...endfor������������б��ʽ��AST�����﷨�������� ��Ӧ zengl_for_stmt_sequence
	ZL_VOID (* fun_stmt)(ZL_VOID * VM_ARG,ZL_INT p); //fun...endfun��AST�����﷨�������ɺ�������Ӧ zengl_fun_stmt
	ZL_INT (* fun_stmt_sequence)(ZL_VOID * VM_ARG); //fun...endfun������������б��ʽ��AST�����﷨�������� ��Ӧ zengl_fun_stmt_sequence
	ZL_VOID (* switch_stmt)(ZL_VOID * VM_ARG,ZL_INT p); //switch...case�ṹ��AST���ɡ���Ӧ zengl_switch_stmt
	ZL_INT (* switch_stmt_sequence)(ZL_VOID * VM_ARG); //switch...case������������б��ʽ��AST�����﷨�������� ��Ӧ zengl_switch_stmt_sequence
	ZL_VOID (* class_stmt)(ZL_VOID * VM_ARG,ZL_INT p); //class...endcls��endclass�ṹ��AST���ɡ���Ӧ zengl_class_stmt
	ZL_INT (* class_stmt_sequence)(ZL_VOID * VM_ARG); //class...endcls��endclass������������б��ʽ��AST�����﷨�������� ��Ӧ zengl_class_stmt_sequence
	ZL_VOID (* while_stmt)(ZL_VOID * VM_ARG,ZL_INT p); //while...endwhile�ṹ��AST���ɡ���Ӧ zengl_while_stmt
	ZL_INT (* while_stmt_sequence)(ZL_VOID * VM_ARG); //while...endwhile������������б��ʽ��AST�����﷨�������� ��Ӧ zengl_while_stmt_sequence
	ZL_VOID (* do_stmt)(ZL_VOID * VM_ARG,ZL_INT p);	//do...dowhile�ṹ��AST���ɡ���Ӧ zengl_do_stmt
	ZL_INT (* do_stmt_sequence)(ZL_VOID * VM_ARG);	//do...dowhile������������б��ʽ��AST�����﷨�������� ��Ӧ zengl_do_stmt_sequence
	ZL_INT (* detectSetRBracketToSemi)(ZL_VOID * VM_ARG); //��if֮��Ĺؼ����Ҳ����������ʱתΪ�ֺţ������Ϳ��Եõ�if�����е������жϱ��ʽ��AST ��Ӧ zengl_detectSetRBracketToSemi
	ZL_INT (* express)(ZL_VOID * VM_ARG); //�������汾���﷨�������������ö�ջ�����ȼ��������﷨������Ӧ zengl_express
	ZL_VOID (* parser_addcurnode)(ZL_VOID * VM_ARG); //����parser_curnode��ǰ�﷨�����ڵ�Ľڵ�� ��Ӧ zengl_parser_addcurnode
	ZL_VOID (* parser_errorExit)(ZL_VOID * VM_ARG,ZENGL_ERRORNO errorno, ...); //�﷨����ʱ���õĴ�ӡ���������Ϣ�ĺ��� ��Ӧ zengl_parser_errorExit
	ZL_VOID (* opLevel_push_stack)(ZL_VOID * VM_ARG,ZL_INT nodenum); //��ĳ�ڵ�ѹ��ջ������Ҫ�Ƚ����ȼ�ʱ���ٵ���ջ ��Ӧ zengl_opLevel_push_stack
	ZENGL_PARSER_OP_LEVEL_STACK_TYPE (* opLevel_pop_stack)(ZL_VOID * VM_ARG,ZL_INT index); //�����ѹ��ջ�Ľڵ㵯������Ӧ zengl_opLevel_pop_stack
	ZL_VOID (* initOpLevelStackList)(ZL_VOID * VM_ARG); //��ʼ�����ȼ���ջ��̬���� ��Ӧ zengl_initOpLevelStackList
	ZL_VOID (* detectCurnodeSyntax)(ZL_VOID * VM_ARG); //���ݵ�ǰcurnode�ڵ��curnode+1�ڵ��������ж��Ƿ����﷨���� ��Ӧ zengl_detectCurnodeSyntax
	ZL_VOID (* OpLevelForTwo)(ZL_VOID * VM_ARG); //ʹ�����ȼ���ջ����Ӽ��˳���˫Ŀ����� ��Ӧ zengl_OpLevelForTwo
	ZL_VOID (* OpLevelForAssign)(ZL_VOID * VM_ARG); //ʹ�����ȼ���ջ����ֵ����˫Ŀ����� ��Ӧ zengl_OpLevelForAssign
	ZL_VOID (* OpLevelForSEMI)(ZL_VOID * VM_ARG); //ʹ�����ȼ���ջ����ֺŽ����� ��Ӧ zengl_OpLevelForSEMI
	ZL_VOID (* OpLevelForPPMM)(ZL_VOID * VM_ARG); //ʹ�����ȼ���ջ����ӼӼ�������� ��Ӧ zengl_OpLevelForPPMM
	ZL_VOID (* PPMM_GET_POS)(ZL_VOID * VM_ARG); //��ȡ�ӼӼ�������������Ҳ��λ�ñ�ʶ ��Ӧ zengl_PPMM_GET_POS
	ZL_VOID (* OpLevelForRBRACKET)(ZL_VOID * VM_ARG); //ʹ�����ȼ���ջ���������� ��Ӧ zengl_OpLevelForRBRACKET
	ZL_VOID (* OpLevelForRMBRACKET)(ZL_VOID * VM_ARG); //ʹ�����ȼ���ջ������������ ��Ӧ zengl_OpLevelForRMBRACKET
	ZL_VOID (* OpLevelForQuestion)(ZL_VOID * VM_ARG); //ʹ�����ȼ���ջ�����ʺ������ ��Ӧ zengl_OpLevelForQuestion
	ZL_VOID (* OpLevelForColon)(ZL_VOID * VM_ARG); //ʹ�����ȼ���ջ����ð������� ��Ӧ zengl_OpLevelForColon
	ZL_VOID (* CheckQstColonValid)(ZL_VOID * VM_ARG); //����ʺ�ð���Ƿ�һһƥ�� ��Ӧ zengl_CheckQstColonValid
	ZL_VOID (* ASTAddNodeChild)(ZL_VOID * VM_ARG,ZL_INT parent,ZL_INT child); //��child��Ӧ�Ľڵ���뵽parent�ڵ���ӽڵ��� ��Ӧ zengl_ASTAddNodeChild
	ZL_BOOL (* CheckIsNegative)(ZL_VOID * VM_ARG); //�����жϵ�ǰ�ļ����Ƿ��Ǹ��� ��Ӧ zengl_CheckIsNegative
	ZL_BOOL (* CheckIsBitAnd)(ZL_VOID * VM_ARG); //�����жϵ�ǰ��"&"�����Ƿ��ǰ�λ������� ��Ӧ zengl_CheckIsBitAnd

	/*�������û��Զ���ĺ���*/
	ZL_INT (* userdef_info)(ZL_CHAR * infoStrPtr, ZL_INT infoStrCount, ZL_VOID * VM_ARG); //�û��Զ������ʾ��ͨ��Ϣ�ĺ������û������Զ�����Ϣ�Ĵ�ӡ�������ʽ��
	ZL_INT (* userdef_compile_error)(ZL_CHAR * infoStrPtr, ZL_INT infoStrCount, ZL_VOID * VM_ARG); //�û��Զ������ʾ������Ϣ�ĺ�����
}ZENGL_COMPILE_TYPE;
/*zengl�������ṹ�嶨�����*/

/*������Ľ������ṹ�嶨��*/
typedef struct _ZENGL_RUN_TYPE
{
	/*�������ĳ�Ա�б�*/
	ZL_INT basesize; //�������ṹ��Ļ�����С
	ZL_INT totalsize; //��ǰ���������ڴ�ش�С���ڴ�ط�����ڴ��С���ܺ�
	ZL_CLOCK_T start_time; //��������ʼִ��ʱ��ʱ�䣬����Ϊ��λ
	ZL_CLOCK_T end_time; //����������ʱ��ʱ�䣬����Ϊ��λ
	ZL_CLOCK_T total_time; //ִ�н���ʱ����ʱ�䣬����Ϊ��λ
	ZL_BOOL isinRunning; //�жϽ������Ƿ�����������
	ZL_BOOL isDestroyed; //�ж��ڴ����Դ�Ƿ��ͷŵ���
	ZL_BOOL isUserWantStop; //�û��Ƿ���Ҫ��;ֹͣ�ű�
	ZL_BOOL is_inMemBlkSetVal; //�ж��Ƿ��ڶ�����Ԫ�ؽ������õ�״̬����״̬�¼�ʹ������Ҳֱ�Ӷ�ȡ����������test[a] = &b;�ͻὫb�����ö�ȡ�����õ�test[a]�С�
	jmp_buf jumpBuffer; //����������ʱͳһ��ת��λ��
	ZENGL_RUN_MEM_POOL_TYPE mempool; //���������ڴ��
	ZENGL_RUN_MEM_FREE_POOL_TYPE memfreepool; //���������ڴ��ͷų�
	ZENGL_RUN_INST_LIST inst_list; //���ָ�̬����
	ZENGL_RUN_VIRTUAL_MEM_LIST vmem_list; //����ȫ���ڴ涯̬����
	ZENGL_RUN_VIRTUAL_MEM_LIST vstack_list; //����ջ�ռ䶯̬���飬�����������������ʱ�Ķ�ջ���ݣ���ֲ�������������
	ZENGL_RUN_RUNTIME_OP_DATA reg_list[ZL_R_REGLIST_SIZE]; //reg_list���������������мĴ���������ʱֵ��
	ZENGL_RUN_INFO_STRING_TYPE infoFullString;  //�������������ľ���������ĵ�����Ϣ�ַ���
	ZENGL_RUN_INFO_STRING_TYPE printFullString; //�����������������������PRINT֮���ָ�������ַ���
	ZENGL_RUN_INFO_STRING_TYPE errorFullString; //�������������ľ���������Ĵ�����Ϣ���ַ���
	ZL_CONST ZL_CHAR ** instTypeString;	//�������и���ָ���ڴ�ӡ���ʱ����ʾ�ַ���
	ZL_CONST ZL_CHAR ** registerTypeString; //�������и��ּĴ����ڴ�ӡ���ʱ����ʾ�ַ���
	ZENGL_RUN_INST_DATA_STRING_POOL InstData_StringPool; //ָ����������ַ�����
	ZL_INT HashTable[ZL_R_HASH_TOTAL_SIZE]; //hash���д���˸��ֶ�̬����Ԫ�ص�����ֵ��
	ZENGL_RUN_MODULE_TABLE moduleTable; //ģ�鶯̬���飬�������˸��ֽű�ģ��ĳ�ʼ����������Ϣ
	ZENGL_RUN_MOD_FUN_TABLE ModFunTable; //ģ�麯����̬���飬�������˸��ֽű�ģ�麯���Ĵ���������Ϣ
	ZENGL_RUN_EXTRA_DATA_TABLE ExtraDataTable; //�û����������ṩ�Ķ������ݹ��ɵĶ�̬����
	ZL_INT CurRunModFunIndex; //��ǰ�������е�ģ�麯����ģ�麯����̬�����е�����ֵ��ͨ��������ֵ��API�ӿھͿ��Ի�ȡ��ģ�麯���������Ϣ�����û��Զ����ģ�麯������

	/*������zenglrun_func.c�е���غ���*/
	ZL_VOID (* init)(ZL_VOID * VM_ARG); //��������ʼ�� ��Ӧ zenglrun_init
	ZL_VOID * (* memAlloc)(ZL_VOID * VM_ARG,ZL_INT size,ZL_INT * index); //Ϊ�ڴ�ط����ڴ��ָ��ĺ��� ��Ӧ zenglrun_memAlloc
	ZL_VOID * (* memReAlloc)(ZL_VOID * VM_ARG , ZL_VOID * point , ZL_INT size, ZL_INT * index); //����pointָ��Ĵ�СΪsize��ͬʱ����indexΪ��ָ�����ڴ���е���������Ӧ zenglrun_memReAlloc
	ZL_INT (* memFindPtrIndex)(ZL_VOID * VM_ARG , ZL_VOID * point); //����pointָ����ڴ���в���ָ���Ӧ������ֵ ��Ӧ zenglrun_memFindPtrIndex
	ZL_VOID * (* memReUsePtr)(ZL_VOID * VM_ARG,ZL_VOID * point,ZL_INT size,ZL_INT * index); //���������size��С�����ڴ�ص�ָ����е�����С�������ò��� ��Ӧ zenglrun_memReUsePtr
	ZL_VOID (* exit)(ZL_VOID * VM_ARG,ZENGL_ERRORNO errorno, ...); //�������˳����� ��Ӧ zenglrun_exit
	ZL_VOID (* exit_forApiSetErrThenStop)(ZL_VOID * VM_ARG); //ר��ΪzenglApi_SetErrThenStop���API�ӿڶ��Ƶ��˳����� ��Ӧ zenglrun_exit_forApiSetErrThenStop
	ZL_VOID (* ExitPrintSourceInfo)(ZL_VOID * VM_ARG,ZENGL_ERRORNO errorno, ...); //�������������˳�ʱ����ӡ����ǰ�������Ӧ��AST�ڵ�����к��ļ�����Ϣ ��Ӧ zenglrun_ExitPrintSourceInfo
	ZL_VOID (* memFreeAll)(ZL_VOID * VM_ARG); //�������ͷ��ڴ����Դ ��Ӧ zenglrun_memFreeAll
	ZL_CHAR * (* SetApiErrorEx)(ZL_VOID * VM_ARG,ZENGL_ERRORNO errorno , ...); //ר����������API������Ϣ����չ���� ��Ӧ zenglrun_SetApiErrorEx
	ZL_CHAR * (* SetApiError)(ZL_VOID * VM_ARG,ZENGL_ERRORNO errorno ,ZENGL_SYS_ARG_LIST arglist); //ר����������API������Ϣ�ĺ��� ��Ӧ zenglrun_SetApiError
	ZL_CHAR * (* makeInfoString)(ZL_VOID * VM_ARG,ZENGL_RUN_INFO_STRING_TYPE * infoStringPtr , ZL_CONST ZL_CHAR * format , ZENGL_SYS_ARG_LIST arglist); //���ɸ�ʽ����Ϣ�ַ��� ��Ӧ zenglrun_makeInfoString
	ZL_VOID (* freeInfoString)(ZL_VOID * VM_ARG,ZENGL_RUN_INFO_STRING_TYPE * infoStringPtr); //�ͷŸ�ʽ����Ϣ�ַ������α�ʹ�С��Ϣ ��Ӧ zenglrun_freeInfoString
	ZL_VOID (* info)(ZL_VOID * VM_ARG , ZL_CONST ZL_CHAR * format , ...); //����infoFullString��errorFullString�ĸ�ʽ���ַ�����Ϣ����ʹ���û��Զ��庯�����������ʾ ��Ӧ zenglrun_info
	ZL_VOID (* print)(ZL_VOID * VM_ARG , ZL_CONST ZL_CHAR * format , ...); //����printFullString�ĸ�ʽ���ַ�����Ϣ����ʹ���û��Զ��庯�������ʾ ��Ӧ zenglrun_print
	ZL_INT (* InstDataStringPoolAdd)(ZL_VOID * VM_ARG , ZL_CHAR * src); //ָ��������ַ���������ַ��� ��Ӧ zenglrun_InstDataStringPoolAdd
	ZL_VOID (* initInstDataStringPool)(ZL_VOID * VM_ARG); //ָ��������ַ����صĳ�ʼ�� ��Ӧ zenglrun_initInstDataStringPool
	ZL_CHAR * (* InstDataStringPoolGetPtr)(ZL_VOID * VM_ARG , ZL_INT index); //��������ֵ���ַ������в���ָ��������ַ�����ָ����Ϣ ��Ӧ zenglrun_InstDataStringPoolGetPtr
	ZL_VOID * (* strcat)(ZL_VOID * VM_ARG,ZL_CHAR * dest,ZL_INT * destIndex,ZL_CHAR * src); //ʹ��C���strcat���ƻ��ڴ�أ����Ծ��Խ�һ��zenglrun_strcat ��Ӧ zenglrun_strcat
	ZL_VOID (* memFreeIndex)(ZL_VOID * VM_ARG,ZL_VOID * point,ZL_INT * srcindex); //�ͷ��ڴ����ָ��������Ӧ��ָ�롣��Ӧ zenglrun_memFreeIndex
	ZL_VOID (* memFreeOnce)(ZL_VOID * VM_ARG,ZL_VOID * point); //�ͷ��ڴ���е�ĳ��ָ�� ��Ӧ zenglrun_memFreeOnce
	ZL_VOID * (* strcat2)(ZL_VOID * VM_ARG,ZL_CHAR * dest,ZL_INT * destIndex,ZL_CHAR * src1,ZL_CHAR * src2); //��src1��src2���ַ���������һ������dest����Ӧ zenglrun_strcat2
	ZL_INT (* Hash)(ZL_VOID * VM_ARG,ZL_CHAR * key); //���ڼ���������еĹ�ϣ������ ��Ӧ zenglrun_Hash
	ZL_INT (* InsertModuleTable)(ZL_VOID * VM_ARG,ZL_CHAR * moduleName,ZL_VM_API_MOD_INIT_FUNC module_init_function); //��ĳģ�������뵽ģ�鶯̬������ ��Ӧ zenglrun_InsertModuleTable
	ZL_VOID (* initModuleTable)(ZL_VOID * VM_ARG); //ģ�鶯̬�����ʼ�� ��Ӧ zenglrun_initModuleTable
	ZL_INT (* InsertModFunTable)(ZL_VOID * VM_ARG,ZL_INT moduleID,ZL_CHAR * functionName,ZL_VM_API_MOD_FUN_HANDLE handle); //��ģ�麯����Ϣ���뵽ģ�麯����̬������ ��Ӧ zenglrun_InsertModFunTable
	ZL_VOID (* initModFunTable)(ZL_VOID * VM_ARG); //ģ�麯����̬�����ʼ�� ��Ӧ zenglrun_initModFunTable
	ZL_INT (* LookupModuleTable)(ZL_VOID * VM_ARG,ZL_CHAR * moduleName); //����ĳģ�����Ϣ�����ظ�ģ����ģ�鶯̬�����е����� ��Ӧ zenglrun_LookupModuleTable
	ZL_INT (* LookupModFunTable)(ZL_VOID * VM_ARG,ZL_CHAR * functionName); //����ĳģ�麯������Ϣ�����ظ�ģ�麯���ڶ�̬�����е����� ��Ӧ zenglrun_LookupModFunTable
	ZL_VOID (* initExtraDataTable)(ZL_VOID * VM_ARG); //��ʼ���û��������ݶ�̬���� ��Ӧ zenglrun_initExtraDataTable
	ZL_INT (* InsertExtraDataTable)(ZL_VOID * VM_ARG,ZL_CHAR * extraDataName,ZL_VOID * point); //�����ݲ��뵽�������ݶ�̬������ ��Ӧ zenglrun_InsertExtraDataTable
	/*������zenglrun_main.c�е���غ���*/
	ZL_VOID (* AddInst)(ZL_VOID * VM_ARG,ZL_INT pc,ZL_INT nodenum,ZENGL_RUN_INST_TYPE type, 
						 ZENGL_RUN_INST_OP_DATA_TYPE dest_type , ZL_DOUBLE dest_val ,
						 ZENGL_RUN_INST_OP_DATA_TYPE src_type , ZL_DOUBLE src_val); //��ӻ��ָ�� ��Ӧ zenglrun_AddInst
	ZL_VOID (* initInstList)(ZL_VOID * VM_ARG); //��ʼ���������Ļ��ָ� ��Ӧ zenglrun_initInstList
	ZENGL_RUN_VIRTUAL_MEM_STRUCT (* VMemListOps)(ZL_VOID * VM_ARG,ZENGL_RUN_VMEM_OP_TYPE opType,ZL_INT memloc,
												  ZENGL_RUN_VIRTUAL_MEM_STRUCT argval); //������������ڴ�������� ��Ӧ zenglrun_VMemListOps
	ZL_VOID (* initVMemList)(ZL_VOID * VM_ARG); //��ʼ��ȫ�������ڴ涯̬���� ��Ӧ zenglrun_initVMemList
	ZENGL_RUN_VIRTUAL_MEM_STRUCT (* VStackListOps)(ZL_VOID * VM_ARG,ZENGL_RUN_VMEM_OP_TYPE opType,ZL_INT index,
												  ZENGL_RUN_VIRTUAL_MEM_STRUCT argval,ZL_BOOL valid); //�����ջ�ռ�Ĳ������� ��Ӧ zenglrun_VStackListOps
	ZL_VOID (* initVStackList)(ZL_VOID * VM_ARG); //��ʼ��������Ķ�ջ�ռ䡣��Ӧ zenglrun_initVStackList
	ZL_VOID (* printInstList)(ZL_VOID * VM_ARG,ZL_CHAR * head_title); //��ӡ���ָ� ��Ӧ zenglrun_printInstList
	ZL_VOID (* RunInsts)(ZL_VOID * VM_ARG); //�����ִ�л��ָ�������ʽ����Ӧ zenglrun_RunInsts
	ZL_VOID (* RegAssignStr)(ZL_VOID * VM_ARG,ZENGL_RUN_REG_TYPE reg,ZL_VOID * str); //��ĳ�ַ�����ֵ��reg�Ĵ��� ��Ӧ zenglrun_RegAssignStr
	ZENGL_RUN_RUNTIME_OP_DATA_TYPE (* op_minis)(ZL_VOID * VM_ARG); //MINIS����ָ�����ش����ʽ��op��operate��������д ��Ӧ zenglrun_op_minis
	ZL_VOID (* op_je)(ZL_VOID * VM_ARG,ZENGL_RUN_RUNTIME_OP_DATA * src); //JEָ��ִ�еĲ�����AX�Ĵ���������֮ǰ���ʽ�Ľ������AXΪ0���ʱ�����޸�PC�Ĵ�����ֵ��ʹ�ű�������ת ��Ӧ zenglrun_op_je
	ZL_VOID (* op_jne)(ZL_VOID * VM_ARG,ZENGL_RUN_RUNTIME_OP_DATA * src); //JNEָ���JEָ��պ��෴��AX�Ĵ���������֮ǰ���ʽ�Ľ������AX��Ϊ0����ΪTRUEʱ�����޸�PC�Ĵ�����ֵ��ʹ�ű�������ת ��Ӧ zenglrun_op_jne
	ZL_VOID (* op_bits)(ZL_VOID * VM_ARG); //��λ�룬������λ����ָ��Ĵ����ʽ ��Ӧ zenglrun_op_bits
	ZL_VOID (* op_relation)(ZL_VOID * VM_ARG); //����С�ڵ���֮��ıȽ������ָ��Ĵ����ʽ����Ӧ zenglrun_op_relation
	ZL_VOID (* op_logic)(ZL_VOID * VM_ARG); //AND(��)��OR���򣩣�REVERSE��ȡ�����߼�������Ĵ����ʽ����Ӧ zenglrun_op_logic
	ZL_VOID (* op_addminisget)(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem,ZENGL_RUN_INST_TYPE type,ZENGL_RUN_INST_OP_DATA_TYPE memtype); //���Ӽӻ����������ڱ�����ʾ����ǰ��ʱ���Ƚ�����ֵ��һ���һ���ٷ��ؽ�� ��Ӧ zenglrun_op_addminisget
	ZL_VOID (* op_getaddminis)(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem,ZENGL_RUN_INST_TYPE type,ZENGL_RUN_INST_OP_DATA_TYPE memtype); //�ӼӼ���������ڱ�����ʾ������ʱ����ȡֵ�������м�һ���һ���� ��Ӧ zenglrun_op_getaddminis
	ZL_VOID (* op_addminisone)(ZL_VOID * VM_ARG,ZENGL_RUN_INST_TYPE type); //�����ʽ�Ľ�����м�һ����һ���ٷ��ؽ������Ӧ zenglrun_op_addminisone
	ZL_VOID (* op_addr)(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem); //ADDR���û��ָ��Ĳ��� ��Ӧ zenglrun_op_addr
	ZL_VOID (* op_set_array)(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem); //��������Ԫ�� ��Ӧ zenglrun_op_set_array
	ZENGL_RUN_VIRTUAL_MEM_LIST * (* alloc_memblock)(ZL_VOID * VM_ARG,ZL_INT * index); //Ϊ�ڴ����䶯̬�ڴ棬ͨ��ZENGL_RUN_VIRTUAL_MEM_LIST�ṹ�������ڴ�飬ZENGL_RUN_VIRTUAL_MEM_LIST�ṹ�е�mem_array�ֶ����·�����ڴ��ָ�� ��Ӧ zenglrun_alloc_memblock
	ZL_INT (* memblock_getindex)(ZL_VOID * VM_ARG,ZL_INT i,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem, ZENGL_RUN_VIRTUAL_MEM_LIST * memblock); //��ȡ�����ڴ������� ��Ӧ zenglrun_memblock_getindex_ext
	ZENGL_RUN_VIRTUAL_MEM_LIST * (* realloc_memblock)(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_LIST * ptr,ZL_INT index); //Ϊ�ڴ�鶯̬������С ��Ӧ zenglrun_realloc_memblock
	ZL_VOID (* assign_memblock)(ZL_VOID * VM_ARG,ZL_VOID ** dest_arg,ZL_VOID * src_arg); //��dest_arg��Ϊsrc_arg��ָ����ڴ�飬ͬʱ����refcount�ڴ������ü��� ��Ӧ zenglrun_assign_memblock
	ZL_VOID (* memblock_setval)(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_LIST * ptr,ZL_INT index,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem); //���������Ӧ�ڴ��ptr��index����λ�õ�ֵ ��Ӧ zenglrun_memblock_setval
	ZENGL_RUN_VIRTUAL_MEM_STRUCT (* VMemBlockOps)(ZL_VOID * VM_ARG,ZENGL_RUN_VMEM_OP_TYPE op,ZENGL_RUN_VIRTUAL_MEM_LIST * ptr,ZL_INT index,
												   ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem); //������ptr���index����ָ����ڴ���в��� ��Ӧ zenglrun_VMemBlockOps
	ZL_VOID (* memblock_free)(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_LIST * memblock,ZL_INT * index); //�ͷ�memblock��������ڴ�� ��Ӧ zenglrun_memblock_free
	ZL_VOID (* op_get_array)(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem); //��ȡ�����е�ĳԪ�� ��Ӧ zenglrun_op_get_array
	ZL_VOID (* op_get_array_addr)(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem); //��ȡ�ڴ��Ԫ�ص����ã���test = &testarray[0];����� ��Ӧ zenglrun_op_get_array_addr
	ZL_VOID (* op_addminis_one_array)(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem,ZENGL_RUN_INST_TYPE op); //������Ԫ�ؽ��мӼӣ��������� ��Ӧ zenglrun_op_addminis_one_array
	ZL_VOID (* memblock_freeall_local)(ZL_VOID * VM_ARG); //�ͷ�ջ�в������ֺ;ֲ��������ֵ������ڴ�� ��Ӧ zenglrun_memblock_freeall_local
	ZL_VOID (* FreeAllForReUse)(ZL_VOID * VM_ARG); //�����������ʱ���ͷŵ�ȫ�������ڴ棬ջ�ڴ��������ڴ������� ��Ӧ zenglrun_FreeAllForReUse
	ZL_VOID (* op_switch)(ZL_VOID * VM_ARG); //SWITCHָ��Ĵ��� ��Ӧ zenglrun_op_switch
	ZL_LONG (* getRegInt)(ZL_VOID * VM_ARG,ZENGL_RUN_REG_TYPE reg); //���ؼĴ���ֵ��������ʽ ��Ӧ zenglrun_getRegInt
	ZL_INT (* main)(ZL_VOID * VM_ARG);	//����������ں��� ��Ӧ zenglrun_main

	/*�������û��Զ���ĺ���*/
	ZL_INT (* userdef_run_info)(ZL_CHAR * infoStrPtr, ZL_INT infoStrCount, ZL_VOID * VM_ARG); //�û��Զ���Ľ���������ʾ��ͨ��Ϣ�ĺ������û������Զ��������Ϣ�Ĵ�ӡ�������ʽ��
	ZL_INT (* userdef_run_print)(ZL_CHAR * infoStrPtr, ZL_INT infoStrCount, ZL_VOID * VM_ARG); //�û��Զ���Ľ�����PRINT֮���ָ���Ӧ�Ĵ�ӡ�����ʽ��
	ZL_INT (* userdef_run_error)(ZL_CHAR * infoStrPtr, ZL_INT infoStrCount, ZL_VOID * VM_ARG); //�û��Զ���Ľ���������ʾ������Ϣ�ĺ�����
}ZENGL_RUN_TYPE;
/*������Ľ������ṹ�嶨�����*/

struct _ZENGL_VM_TYPE;  
typedef struct _ZENGL_VM_TYPE ZENGL_VM_TYPE;  

/*������������ṹ*/
typedef struct _ZENGL_DEBUG_TYPE
{
	ZENGL_VM_TYPE * DeubugVM; //�����õ����������
	ZENGL_VM_TYPE * DeubugPVM; //����������ĸ������
	ZL_INT orig_run_totalsize;
	ZL_INT orig_vm_totalsize;
	ZL_INT api_call_pc; //ʹ��zenglApi_Callʱִ�еĽű������ĵ�һ��ָ���PCֵ
	ZL_INT api_call_arg; //ʹ��zenglApi_Callʱ�ĳ�ʼARG
	ZL_BOOL break_start; //�Ƿ��ڽű��ĵ�һ��ָ����öϵ�
	ZL_BOOL output_debug_info; //�Ƿ���������ַ����ķ��ű����Ϣ
	ZL_INT flag; //һЩ��־�������ж��Ƿ���Ҫ�ָ��ϵ㼰�Ƿ���Ҫ���õ���ִ�еı�־
	ZENGL_DEBUG_BREAK_POINT_LIST BreakPoint; //�������Ķϵ�
	ZENGL_DEBUG_SINGLE_BREAK singleBreak; //�������Ե��жϽṹ
	/*������zenglDebug.c�е���غ���*/
	ZL_INT (* Compile)(ZL_VOID * VM_ARG,ZL_CHAR * script_file,ZENGL_EXPORT_VM_MAIN_ARGS * vm_main_args); //�������ı��벿�� ��Ӧ zenglDebug_Compile
	ZL_INT (* Run)(ZL_VOID * VM_ARG); //�������Ľ���ִ�в��� ��Ӧ zenglDebug_Run
	ZENGL_TOKENTYPE (* ReplaceDefConst)(ZL_VOID * VM_ARG, ZENGL_TOKENTYPE token); //���������滻�곣���ĺ��� ��Ӧ zenglDebug_ReplaceDefConst
	ZL_INT (* lookupDefTable)(ZL_VOID * VM_ARG, ZL_CHAR * name); //���������Һ궨����滻���� ��Ӧ zenglDebug_lookupDefTable
	ZENGL_RUN_INST_OP_DATA (* SymLookupID)(ZL_VOID * VM_ARG,ZL_INT nodenum); //������ͨ���ڵ����������Ҹýڵ�ı�����ʾ�����Զ���������ڴ��е��ڴ��ַ ��Ӧ zenglDebug_SymLookupID
	ZL_INT (* SymLookupID_ForDot)(ZL_VOID * VM_ARG,ZL_INT nodenum); //����������nodenum��Ӧ�ڵ��classidֵ����Ҫ�������ɵ�������Ļ��ָ��ʱ ��Ӧ zenglDebug_SymLookupID_ForDot
	ZL_INT (* SymLookupClass)(ZL_VOID * VM_ARG,ZL_INT nodenum); //���������ݽڵ�Ų�����ID��Ϣ ��Ӧ zenglDebug_SymLookupClass
	ZL_INT (* SymLookupClassMember)(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT parent_classid); //��������SymClassMemberTable�в���parent_classid��Ӧ����ĳ�Աnodenum����Ϣ ��Ӧ zenglDebug_SymLookupClassMember
	ZL_INT (* SymLookupFun)(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT classid); // �������и��ݽڵ�Ų��Һ���ID��Ϣ ��Ӧ zenglDebug_SymLookupFun
	ZL_INT (* LookupModFunTable)(ZL_VOID * VM_ARG,ZL_CHAR * functionName); //�������в���ĳģ�麯������Ϣ�����ظ�ģ�麯���ڶ�̬�����е����� ��Ӧ zenglDebug_LookupModFunTable
	ZL_INT (* LookupFunID)(ZL_VOID * VM_ARG,ZL_INT nodenum); //������ͨ�����������ڵĽڵ�����ֵ�����Һ�����IDֵ ��Ӧ zenglDebug_LookupFunID
	ZL_INT (* SetFunInfo)(ZL_VOID * VM_ARG); //���õ��������ڵĽű��������� ��Ӧ zenglDebug_SetFunInfo
	ZL_INT (* GetTraceInfo)(ZL_VOID * VM_ARG,ZL_CHAR * ApiName,ZL_INT * argArg,ZL_INT * argLOC,ZL_INT * argPC,
							   ZL_CHAR ** fileName,ZL_INT * line,ZL_CHAR ** className,ZL_CHAR ** funcName); //��������ջ�л�ȡ�ű������ĵ������ ��Ӧ zenglDebug_GetTraceInfo
	ZL_INT (* InitBreak)(ZL_VOID * VM_ARG); //��ʼ���������Ĵ�Ŷϵ�Ķ�̬���� ��Ӧ zenglDebug_InitBreak
	ZL_INT (* SetBreak)(ZL_VOID * VM_ARG,ZL_INT pc,ZL_CHAR * filename,ZL_INT line,ZL_CHAR * condition,ZL_CHAR * log,ZL_INT count,ZL_BOOL disabled); //�����������öϵ� ��Ӧ zenglDebug_SetBreak
	ZL_INT (* BreakStart)(ZL_VOID * VM_ARG); //����������һ��ָ����Ϊ��ִ��һ�εĶϵ� ��Ӧ zenglDebug_BreakStart
	ZL_INT (* DelBreak)(ZL_VOID * VM_ARG,ZL_INT index); //ɾ��index������Ӧ�ĵ��Զϵ� ��Ӧ zenglDebug_DelBreak
	ZL_INT (* RestoreBreaks)(ZL_VOID * VM_ARG); //�ϵ�ԭָ��ִ�����Ҫ�ָ���ָ���Ӧ�Ķϵ� ��Ӧ zenglDebug_RestoreBreaks
	ZL_INT (* CheckCondition)(ZL_VOID * VM_ARG,ZL_CHAR * condition); //���ϵ��Ƿ񵽴�ָ���жϵ����� ��Ӧ zenglDebug_CheckCondition
	ZL_INT (* RestoreSingleBreak)(ZL_VOID * VM_ARG); //�ָ������ж϶�Ӧ��ԭʼָ�� ��Ӧ zenglDebug_RestoreSingleBreak
	ZL_INT (* ChkAndSetSingleBreak)(ZL_VOID * VM_ARG); //������Ҫ�ж��Ƿ���Ҫ����һ��ָ�����õ����ж� ��Ӧ zenglDebug_ChkAndSetSingleBreak
	/*������Ҫ���õ��û��Զ��庯��*/
	ZL_INT (* userdef_debug_break)(ZL_VOID * VM_ARG,ZL_CHAR * filename,ZL_INT line,ZL_INT breakIndex,ZL_CHAR * log); //�û��Զ���Ķϵ���Ժ���
	ZL_INT (* userdef_debug_conditionError)(ZL_VOID * VM_ARG,ZL_CHAR * filename,ZL_INT line,ZL_INT breakIndex,ZL_CHAR * error); //�û��Զ���������ϵ�����������ʱ�ĵ��ú���
}ZENGL_DEBUG_TYPE;
/*������������ṹ�������*/

/*������ṹ�嶨��*/
struct _ZENGL_VM_TYPE
{
	ZENGL_COMPILE_TYPE compile;
	ZENGL_RUN_TYPE run;
	ZENGL_DEBUG_TYPE debug;
	ZENGL_ERRORNO errorno;
	ZL_CONST ZL_CHAR ** errorString;
	ZL_BOOL isCompileError;
	ZL_BOOL isRunError;
	ZL_INT signer;   //VM������Ƿ���Ч��signerǩ����־
	ZL_INT basesize; //��ǰ����������ṹ��Ļ�����С
	ZL_INT totalsize; //��ǰ�����ռ���ڴ�Ĵ�С�ܺ�
	ZL_CLOCK_T start_time; //�������ʼִ�е�ʱ�䣬����Ϊ��λ
	ZL_CLOCK_T end_time; //���������ʱ��ʱ�䣬����Ϊ��λ
	ZL_CLOCK_T total_time; //ִ�н���ʱ����ʱ�䣬����Ϊ��λ
	ZENGL_EXPORT_VM_MAIN_ARGS * vm_main_args; //�û����ݸ��������һЩ����
	ZL_BOOL isinApiRun; //�ж��Ƿ�ͨ��zenglApi_Run���е������
	ZL_BOOL isUseApiSetErrThenStop; //�ж��û��Ƿ���ģ�麯����ͨ������zenglApi_SetErrThenStop��ֹͣ�������
	ZENGL_ENCRYPT initEncrypt; //����������ؽṹ��ĳ�ʼ��״̬�����ڳ�ʼ�� ZENGL_SOURCE_TYPE �е�encrypt��Ա�õ�
	ZENGL_API_STATES ApiState; //API�ӿڴ�����������ĸ���״̬�������ж�API���õ�λ���Ƿ�Ϸ�
	ZL_CHAR * ApiError; //ר�����ڴ��API�ĳ�����Ϣ��

	/*�������ص��Զ��庯��*/
	ZL_VOID (* rc4InitState)(ZL_VOID * VM_ARG,ZL_CHAR * key,ZL_INT len); //RC4ʹ�ó�ʼ��Կ����ʼ��state״̬���� ��Ӧ zenglVM_rc4InitState
	ZL_VOID (* init)(ZL_VOID * VM_ARG,ZENGL_EXPORT_VM_MAIN_ARGS * vm_main_args); //�������ʼ������ ��Ӧ zenglVM_init
};
/*������ṹ�嶨�����*/

//�����Ƕ�����zengl_locals.c�е�ȫ�ֱ���
ZL_EXTERN ZL_CONST ZL_CHAR * ZL_Error_String[]; //��Ÿ��ֳ�����Ϣ��
ZL_EXTERN ZL_CONST ZL_CHAR * ZL_Reserves_String[]; //���ֹؼ���(������)����
ZL_EXTERN ZL_CONST ZL_CHAR * ZL_Token_Operate_String[]; //���в���������������ַ�����Ϣ
ZL_EXTERN ZL_CONST ZL_CHAR * ZL_Run_Inst_Type_String[];	//�������и���ָ���ڴ�ӡ���ʱ����ʾ�ַ���
ZL_EXTERN ZL_CONST ZL_CHAR * ZL_Run_Register_Type_String[]; //�������и��ּĴ����ڴ�ӡ���ʱ����ʾ�ַ���
ZL_EXTERN ZENGL_VM_TYPE ZL_Api_Const_VM; //�������ʼ��ģ��

//�����Ƕ�����zengl_main.c�еĺ���
ZL_VOID zengl_exit(ZL_VOID * VM_ARG,ZENGL_ERRORNO errorno, ...); //�������˳����������������صĳ�����Ϣ
ZL_VOID zengl_freeall(ZL_VOID * VM_ARG); //�������ͷ��ڴ����Դ
ZL_CHAR zengl_getNextchar(ZL_VOID * VM_ARG); //���ļ��л�ȡ��һ���ַ�
ZENGL_TOKENTYPE zengl_getToken(ZL_VOID * VM_ARG); //��ȡtoken��Ϣ
ZL_VOID zengl_makeTokenInfoLineCol(ZL_VOID * VM_ARG,ZL_INT line,ZL_INT col,ZL_CHAR * filename); //����token�����кŵ���Ϣ
ZL_VOID zengl_makeTokenStr(ZL_VOID * VM_ARG,ZL_CHAR ch); //����token���ַ�����Ϣ
ZL_VOID * zengl_malloc(ZL_VOID * VM_ARG , ZL_INT size); //�����������ڴ溯����������������ڴ�ָ��ͳһ�������������ڴ�ع���
ZL_VOID * zengl_realloc(ZL_VOID * VM_ARG , ZL_VOID * point , ZL_INT size); //�����ڴ����ĳ��ָ����ڴ��С
ZL_VOID zengl_freeonce(ZL_VOID * VM_ARG,ZL_VOID * point); //�ͷű������ڴ���е�ĳ��ָ��
ZL_VOID zengl_ungetchar(ZL_VOID * VM_ARG); //��Դ�ű��ַ�ɨ���α����һ��ͬʱ���кż�һ
ZL_VOID zengl_convertStrToDec(ZL_VOID * VM_ARG); //���ַ���תΪ��Ч��ʮ���������ٴ�ŵ�tokenInfo�� 
ZL_VOID zengl_freeTokenStr(ZL_VOID * VM_ARG); //�޸��α�ȳ�Ա���ͷŵ�ǰɨ��token�Ķ�̬�ַ�����Ϣ���Թ���һ��ɨ��tokenʱʹ��
ZL_VOID zengl_convertOctalToDec(ZL_VOID * VM_ARG); //����ǰɨ��token�ɰ˽���תΪʮ����
ZL_VOID zengl_convertHexToDec(ZL_VOID * VM_ARG); //��ɨ��token��ʮ�����Ƹ�ʽתΪʮ���Ƹ�ʽ��
ZL_VOID zengl_makeTokenStrForString(ZL_VOID * VM_ARG,ZL_CHAR ch); //����ר�Ž����ַ������͵�token��
ZENGL_TOKENTYPE zengl_lookupReserve(ZL_VOID * VM_ARG , ZENGL_TOKENTYPE token); //��ѯ�ؼ��ֵ�ö��ֵ
ZL_VOID zengl_AddDefConst(ZL_VOID * VM_ARG); //�ʷ�ɨ��ʱ�������def�ؼ��֣��͵���zengl_AddDefConst��������ͺ��Ӧ��ֵ���뵽��ϣ��
ZL_INT zengl_DefPoolAddString(ZL_VOID * VM_ARG , ZL_CHAR * src); //������src��ӵ�def_StringPool�ַ������С������ظ��ַ����ڳ��е�����
ZL_VOID zengl_initDefStringPool(ZL_VOID * VM_ARG); //def�������ĳ�����ֵ���ַ�����Ϣ�������def_StringPool���ַ������С���ʼ��def_StringPool�ַ����ء�
ZL_VOID zengl_insert_HashTableForDef(ZL_VOID * VM_ARG , ZL_INT nameIndex, ZENGL_TOKENTYPE tokentype,ZL_INT valIndex,
									 ZL_INT line,ZL_INT col); //��def����ĺ����ͺ��Ӧ��ֵ���뵽��̬�����У�ͬʱ���������뵽��ϣ���С�
ZL_CHAR * zengl_DefPoolGetPtr(ZL_VOID * VM_ARG , ZL_INT index); //��������ֵ���ַ������в��Һ궨���ַ�����ָ����Ϣ
ZL_INT zengl_insert_DefTable(ZL_VOID * VM_ARG,ZL_INT nameIndex,ZENGL_TOKENTYPE tokentype,ZL_INT valIndex,ZL_INT line,ZL_INT col); //��ÿ��def����ĺ����ƺͺ��Ӧ��ֵ���뵽def_table��̬�����С�
ZL_VOID zengl_initDefTable(ZL_VOID * VM_ARG); //��ʼ��def_table��def�궨��Ķ�̬���飩
ZL_VOID zengl_include_file(ZL_VOID * VM_ARG); //���ʷ�ɨ����ɨ�赽inc�ؼ���ʱ���ͻ���øú����������ļ���
ZL_CHAR * zengl_makePathFileName(ZL_VOID * VM_ARG,ZL_CHAR * filename); //inc�����ļ�ʱ���ȵõ���ǰɨ����ļ���·����Ϣ
ZL_VOID zengl_push_FileStack(ZL_VOID * VM_ARG , ZENGL_SOURCE_TYPE * src,ZL_INT line,ZL_INT col, ZL_CHAR * inc_filename); //��ԭ�����ļ���Ϣѹ��ջ��
ZL_VOID zengl_pop_FileStack(ZL_VOID * VM_ARG , ZENGL_SOURCE_TYPE * dest); //���ϴ�ѹ��ջ���ļ���Ϣ�������ӵ������ļ���Ϣ��ָ�ԭ�ļ���ɨ�衣
ZL_VOID zengl_initFileStack(ZL_VOID * VM_ARG); //��ʼ��inc�����ļ��Ķ�ջ
ZENGL_TOKENTYPE zengl_ReplaceDefConst(ZL_VOID * VM_ARG, ZENGL_TOKENTYPE token); //�滻��Ϊ�����ֵ
ZL_INT zengl_lookupDefTable(ZL_VOID * VM_ARG, ZL_CHAR * name); //����def���
ZL_CHAR * zengl_makeInfoString(ZL_VOID * VM_ARG,ZENGL_INFO_STRING_TYPE * infoStringPtr , ZL_CONST ZL_CHAR * format , ZENGL_SYS_ARG_LIST arglist); //���ɸ�ʽ����Ϣ�ַ���
ZL_VOID zengl_freeInfoString(ZL_VOID * VM_ARG,ZENGL_INFO_STRING_TYPE * infoStringPtr); //�ͷŸ�ʽ����Ϣ�ַ������α�ʹ�С��Ϣ
ZL_VOID zengl_info(ZL_VOID * VM_ARG , ZL_CONST ZL_CHAR * format , ...); //���û������п���ִ��һЩ����Ĵ�ӡ���������߽��ַ���������ļ���
ZL_VOID zengl_init(ZL_VOID * VM_ARG,ZL_CHAR * script_file,ZENGL_EXPORT_VM_MAIN_ARGS * vm_main_args); //��������ʼ��
ZL_INT zengl_main(ZL_VOID * VM_ARG,ZL_CHAR * script_file,ZENGL_EXPORT_VM_MAIN_ARGS * vm_main_args); //��������ں���
ZL_VOID zengl_rc4InitState(ZL_VOID * VM_ARG,ZL_CHAR * key,ZL_INT len); //RC4ʹ�ó�ʼ��Կ����ʼ��state״̬����
ZL_VOID zenglVM_rc4InitState(ZL_VOID * VM_ARG,ZL_CHAR * key,ZL_INT len); //RC4ʹ�ó�ʼ��Կ����ʼ��state״̬����
ZL_VOID zenglVM_init(ZL_VOID * VM_ARG,ZENGL_EXPORT_VM_MAIN_ARGS * vm_main_args); //�������ʼ��

//�����Ƕ�����zengl_symbol.c�еĺ���
ZL_INT zengl_Sym_Hash (ZL_VOID * VM_ARG , ZL_CHAR * key ); //hash��ϣɢ���㷨
ZL_INT zengl_insert_LineCol(ZL_VOID * VM_ARG , ZL_INT line , ZL_INT col,ZL_CHAR * filename); //��line,col���кţ��Լ����к����ڵ��ļ������뵽LineCols���кŶ�̬�����С�
ZL_VOID zengl_initLineColTable(ZL_VOID * VM_ARG); //��ʼ��LineCols���кŶ�̬���飬���ڼ�¼���ű���ÿ��ȫ�ַ��ų��ֹ������к���Ϣ��
ZL_VOID zengl_buildSymTable(ZL_VOID * VM_ARG); //�齨���ű������ʽ��
ZL_VOID zengl_ScanInsertGlobalSym(ZL_VOID * VM_ARG,ZL_INT arg_nodenum); //ɨ�貢����ȫ�ֱ������ű�
ZL_VOID zengl_SymInsertDotID(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT parentNum); //�жϵ��������Ľڵ��ǳ�Ա�����໹��ȫ�ֱ���������ȫ�ֱ�������ȫ�ַ��ű��С�
ZL_INT zengl_SymInsertHashTableForGlobal(ZL_VOID * VM_ARG,ZL_INT nodenum); //��ȫ�ֱ������뵽��ϣ���ȫ�ַ��ű�̬�����С�
ZL_INT zengl_SymInsertGlobalTable(ZL_VOID * VM_ARG,ZL_INT nameIndex,ZL_INT line,ZL_INT col,ZL_CHAR * filename); //��ĳ��������ʾ�����뵽ȫ�ֱ������ű����ȷ���ñ�����Ӧ���ڴ��ַ
ZL_VOID zengl_SymInitGlobalTable(ZL_VOID * VM_ARG); //��ʼ��ȫ�ֱ������ű�
ZL_INT zengl_SymInsertHashTableForClass(ZL_VOID * VM_ARG,ZL_INT nodenum); //��nodenum�������ڵĽڵ���뵽��ϣ�������ű����ڵĶ�̬������
ZL_INT zengl_SymInsertClassTable(ZL_VOID * VM_ARG,ZL_INT nameIndex,ZL_INT nodenum,ZL_INT line,ZL_INT col,ZL_CHAR * filename); //�������͸��ඨ�����ڵ����к�,�ļ������뵽����ű��Ӧ�Ķ�̬������
ZL_VOID zengl_SymInitClassTable(ZL_VOID * VM_ARG);	//��ʼ������ű��Ӧ�Ķ�̬����
ZL_INT zengl_SymInsertHashTableForFun(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT classid); //�������ڵ���Ϣ���뵽������̬�����У�������̬������������浽HashTable��
ZL_INT zengl_SymInsertFunTable(ZL_VOID * VM_ARG,ZL_INT nameIndex,ZL_INT classid,ZL_INT line,ZL_INT col,ZL_CHAR * filename); //���������ͺ������ڵ����кŵ���Ϣ���뵽�������ű��Ӧ�Ķ�̬�����С�
ZL_VOID zengl_SymInitFunTable(ZL_VOID * VM_ARG);	//��ʼ���������ű��Ӧ�Ķ�̬����
ZL_VOID zengl_ScanInsertClassMemberSym(ZL_VOID * VM_ARG,ZL_INT arg_nodenum,ZL_INT classid); //ɨ�����Ա���������Ա�������Ա���ű��Լ����ຯ�����뺯�����ű��С�
ZL_INT zengl_SymLookupClass(ZL_VOID * VM_ARG,ZL_INT nodenum); //���ݽڵ�Ų�����ID��Ϣ
ZL_INT zengl_SymLookupClassByName(ZL_VOID * VM_ARG,ZL_CHAR * name); //��������������ID��Ϣ
ZL_INT zengl_SymLookupClassMember(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT parent_classid); //��SymClassMemberTable�в���parent_classid��Ӧ����ĳ�Աnodenum����Ϣ
ZL_INT zengl_SymInsertHashTableForClassMember(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT index,ZL_INT classid,ZL_INT cls_stmt_classid); //��nodenum���Ա���ڵĽڵ���뵽��ϣ������Ա���ű����ڵĶ�̬������
ZL_INT zengl_SymInsertClassMemberTable(ZL_VOID * VM_ARG,ZL_INT nameIndex,ZL_INT index,ZL_INT classid,ZL_INT cls_stmt_classid,ZL_INT nodenum); //�����Ա�������Ա����ṹ�е�����ֵ��������ID����Ϣ�������Ա���ű���
ZL_VOID zengl_SymInitClassMemberTable(ZL_VOID * VM_ARG); //��ʼ�����Ա���ű��Ӧ�Ķ�̬����
ZENGL_RUN_INST_OP_DATA zengl_SymLookupID(ZL_VOID * VM_ARG,ZL_INT nodenum); //ͨ���ڵ����������Ҹýڵ�ı�����ʾ�����Զ���������ڴ��е��ڴ��ַ
ZL_VOID zengl_SymInsertClassID(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT classid); //Ϊnodenum�ڵ�����classid��id
ZL_INT zengl_SymLookupID_ForDot(ZL_VOID * VM_ARG,ZL_INT nodenum); //����nodenum��Ӧ�ڵ��classidֵ����Ҫ�������ɵ�������Ļ��ָ��ʱ
ZL_INT zengl_SymLookupID_ByName(ZL_VOID * VM_ARG,ZL_CHAR * name); //ͨ���������Ʋ��ұ�����ʾ�����Զ���������ڴ��е��ڴ��ַ����Ҫ���ڶ����API�ӿ���
ZL_INT zengl_SymLookupFun(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT classid);	//���ݽڵ�Ų��Һ���ID��Ϣ
ZL_INT zengl_SymLookupFunByName(ZL_VOID * VM_ARG,ZL_CHAR * name,ZL_INT classid); //���ݺ������Ʋ��Һ���ID��Ϣ
ZL_VOID zengl_SymScanFunArg(ZL_VOID * VM_ARG,ZL_INT nodenum); //ʹ��ASTɨ���ջ��ɨ���﷨���к����Ĳ���
ZL_VOID zengl_SymScanFunGlobal(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT global_nodenum); //ʹ��ASTɨ���ջ��ɨ���﷨���к�����global�ؼ���������ȫ�ֱ���
ZL_VOID zengl_SymScanClassStatement(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT cls_statement_nodenum); //ɨ������������������ı���
ZENGL_STATES zengl_SymScanDotForClass(ZL_VOID * VM_ARG,ZL_INT nodenum, ZENGL_ASM_LOOP_STACK_TYPE ** loopStackTopArg); //ɨ��������ʱ�Ľڵ㣬�����ԱתΪ���������ѹ��ջ��
ZL_VOID zengl_SymScanUseRsv(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT use_nodenum); //ʹ��ASTɨ���ջ��ɨ���﷨����use�ؼ���������ģ����
ZL_VOID zengl_SymScanFunLocal(ZL_VOID * VM_ARG,ZL_INT nodenum); //ʹ��ASTɨ���ջ��ɨ���﷨���к����ľֲ�����
ZL_BOOL zengl_SymInsertHashTableForLocal(ZL_VOID * VM_ARG,ZL_INT nodenum,ZENGL_SYM_ENUM_LOCAL_TYPE type); //���ֲ������Ͳ������뵽�ֲ��������ű��У��������ű�̬������������뵽��ϣ����
ZL_INT zengl_SymInsertLocalTable(ZL_VOID * VM_ARG,ZL_INT nameIndex,ZENGL_SYM_ENUM_LOCAL_TYPE type); //���ֲ��������;ֲ����������Ͳ��뵽SymLocalTable��̬�����С�
ZL_VOID zengl_SymInitLocalTable(ZL_VOID * VM_ARG); //��ʼ��SymLocalTable�ֲ����ű��Ӧ�Ķ�̬����
ZL_BOOL zengl_SymIsSelfToken(ZL_VOID * VM_ARG, ZL_CHAR * token_name); // ����token�����ж��Ƿ���self��token
ZL_BOOL zengl_SymAddNodeNumToSelfClassTable(ZL_VOID * VM_ARG, ZL_INT self_nodenum); // ���ĳ����ʶ���ڵ���self�ڵ㣬�򽫸ýڵ��AST�ڵ�ż��뵽self�ڵ�����Ϣ��̬������
ZL_VOID zengl_SymPrintTables(ZL_VOID * VM_ARG);		//��ӡ���ű�

//�����Ƕ�����zengl_assemble.c�еĺ���
ZL_VOID zengl_buildAsmCode(ZL_VOID * VM_ARG); //�齨�����������ʽ
ZL_VOID zengl_AsmGenCodes(ZL_VOID * VM_ARG,ZL_INT nodenum); //�ú�������AST�����﷨���Ľڵ�������ĳ�ڵ�תΪ�����롣
ZL_INT zengl_AsmGCStackPush(ZL_VOID * VM_ARG,ZL_INT num,ZENGL_ASM_STACK_ENUM type); //������ѹ�����ջ
ZL_INT zengl_AsmGCStackPop(ZL_VOID * VM_ARG,ZENGL_ASM_STACK_ENUM type,ZL_BOOL isremove); //�������ջ
ZL_VOID zengl_AsmGCStackInit(ZL_VOID * VM_ARG); //����ջ��ʼ��
ZL_VOID zengl_AsmGCLoopStackPush(ZL_VOID * VM_ARG, ZL_INT nodenum, ZENGL_STATES state); // ����Ϣѹ��ģ���ջ
ZENGL_STATES zengl_AsmGCLoopStackFinishTop(ZL_VOID * VM_ARG, ZL_INT nodenum); // ��ģ��ջ����Ԫ������������Ļ��ָ��󣬾ͽ�ջ�����������ջ�ڻ���Ԫ�صĻ����ͷ���ջ��Ԫ�ص�״̬������ģ��ջ���ѭ���������µ�ջ��Ԫ�صĻ��ָ����򷵻�ZL_ST_DOWN��ʾջ��Ԫ��ȫ�������
ZENGL_STATES zengl_AsmGCLoopStackFinishTopSimple(ZL_VOID * VM_ARG); // ��ģ��ջ����Ԫ������������Ļ��ָ��󣬾ͽ�ջ�����������Ǽ򻯵İ汾��û���������������õ�PUSH AX��������Ҫ����elif���ɻ��ָ���Լ�ɨ��������ʱʹ�õġ�
ZL_BYTE * zengl_AsmGCLoopStackValsPush(ZL_VOID * VM_ARG, ZENGL_ASM_LOOP_STACK_TYPE * loopStackTop, ZL_INT count); // ������������������ʹ�õ�һЩ�ֲ��������浽ģ���ջ��
ZL_VOID zengl_AsmGCLoopStackValsPop(ZL_VOID * VM_ARG, ZENGL_ASM_LOOP_STACK_TYPE * loopStackTop); // ������ֲ������������������ݵ�ģ��ջ��ջ������
//ZL_VOID zengl_AsmGCElif(ZL_VOID * VM_ARG,ZENGL_AST_CHILD_NODE_TYPE * ifchnum,ZL_INT num); //zengl_AsmGCElif������������elif������Ӧ�Ļ��ָ��
//ZL_VOID zengl_AsmGCBreak_Codes(ZL_VOID * VM_ARG,ZL_INT nodenum); //break���Ļ���������
//ZL_VOID zengl_AsmGCContinue_Codes(ZL_VOID * VM_ARG,ZL_INT nodenum); //continue���Ļ���������
ZL_VOID zengl_AsmScanCaseMinMax(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_BOOL * hasminmax,ZL_INT * minarg,ZL_INT * maxarg,ZL_BOOL * hasdefault,
								ZENGL_ASM_CASE_JMP_TABLE * table); //ɨ��switch...case ���ҳ����е�case�����ֵ����Сֵ���Լ��ж��Ƿ���defaultĬ�Ͻڵ�
ZL_LONG zengl_GetNodeInt(ZL_VOID * VM_ARG,ZL_INT nodenum); //���ؽڵ���ַ�����Ϣ��������ʽ
ZL_VOID zengl_AsmAddCaseJmpTable(ZL_VOID * VM_ARG,ZENGL_ASM_CASE_JMP_TABLE * table,ZL_LONG num); //��case��Ӧ�ıȽ�������ӵ���ת����
ZL_VOID zengl_AsmInitCaseJmpTable(ZL_VOID * VM_ARG,ZENGL_ASM_CASE_JMP_TABLE * table); //��ʼ��switch case����ת��
ZL_VOID zengl_AsmSortCaseJmpTable(ZL_VOID * VM_ARG,ZENGL_ASM_CASE_JMP_TABLE * table,ZL_INT nodenum); //��switch...case��ת����д�С���������

//�����Ƕ�����zengl_ld.c�еĺ���
ZL_INT zengl_LDAddrListSet(ZL_VOID * VM_ARG,ZL_INT index,ZL_INT loc); //�������Ӷ�̬������ĳ����(α��ַ)��Ӧ��ʵ�ʻ�����λ��
ZL_VOID zengl_LDAddrListInit(ZL_VOID * VM_ARG); //���Ӷ�̬�����ʼ��
ZL_VOID zengl_LDAddrListReplaceAll(ZL_VOID * VM_ARG); //�����е�α��ַ�滻Ϊ��ʵ�Ļ�����λ�ã��Ӷ�������ӹ���

//�����Ƕ�����zengl_parser.c�еĺ���
ZL_VOID zengl_ASTAddNode(ZL_VOID * VM_ARG,ZENGL_TOKENTYPE token); //��token����AST�����﷨��
ZL_VOID zengl_initAST(ZL_VOID * VM_ARG); //��ʼ��AST�����﷨���Ķ�̬���顣
ZL_INT zengl_TokenStringPoolAddStr(ZL_VOID * VM_ARG , ZL_CHAR * src); //��token�ַ����ض�̬��������ַ����������ظ��ַ����ڻ����е�������Ϣ
ZL_VOID zengl_initTokenStringPool(ZL_VOID * VM_ARG); //��ʼ��token�ַ����صĶ�̬����
ZL_VOID zengl_printNode(ZL_VOID * VM_ARG , ZL_INT nodenum); //��ӡtoken����Ϣ
ZL_CHAR * zengl_getTokenStr(ZL_VOID * VM_ARG , ZENGL_AST_NODE_TYPE * nodes,ZL_INT nodenum); //���ݽڵ�Ż�ȡtoken��Ӧ���ַ�����Ϣ
ZL_CHAR * zengl_TokenStringPoolGetPtr(ZL_VOID * VM_ARG , ZL_INT index); //��������index��token�ַ����ػ����еõ���Ӧ���ַ�����Ϣ
ZL_VOID zengl_printASTnodes(ZL_VOID * VM_ARG); //��ӡ��AST�����﷨�������нڵ���Ϣ���������µ�ASTɨ���ջ�ķ������б���ɨ�衣
ZL_VOID zengl_push_AST_TreeScanStack(ZL_VOID * VM_ARG , ZL_INT nodenum); //ASTɨ���ջ��ѹջ����
ZENGL_AST_SCAN_STACK_TYPE zengl_pop_AST_TreeScanStack(ZL_VOID * VM_ARG,ZL_BOOL remove); //ASTɨ���ջ�ĵ���ջ����
ZL_VOID zengl_init_AST_TreeScanStack(ZL_VOID * VM_ARG); //��ʼ��ASTɨ���ջAST_TreeScanStackList
ZL_VOID zengl_buildAST(ZL_VOID * VM_ARG); //�ú���ͨ��ѭ���������������AST
ZL_INT zengl_statement(ZL_VOID * VM_ARG); //����������print 'hello'֮�������AST�﷨��
ZL_VOID zengl_KeywordCompleteDetect(ZL_VOID * VM_ARG); //�ؼ��������Լ�飬����if�ṹ������endif��ƥ���
ZL_VOID zengl_TokenSyntaxDetect(ZL_VOID * VM_ARG);	//use��global��break��continue���������������﷨���
ZL_VOID zengl_if_stmt(ZL_VOID * VM_ARG,ZL_INT p); //if-elif-else��������AST�����﷨�������ɺ�����
ZL_INT zengl_if_stmt_sequence(ZL_VOID * VM_ARG); //if-elif-else-endif������������б��ʽ��AST�����﷨��������
ZL_VOID zengl_for_stmt(ZL_VOID * VM_ARG,ZL_INT p); //for...endfor��������AST�����﷨�������ɺ�����
ZL_INT zengl_for_stmt_sequence(ZL_VOID * VM_ARG); //for...endfor������������б��ʽ��AST�����﷨��������
ZL_VOID zengl_fun_stmt(ZL_VOID * VM_ARG,ZL_INT p); //fun...endfun��AST�����﷨�������ɺ�����
ZL_INT zengl_fun_stmt_sequence(ZL_VOID * VM_ARG); //fun...endfun������������б��ʽ��AST�����﷨��������
ZL_VOID zengl_switch_stmt(ZL_VOID * VM_ARG,ZL_INT p); //switch...case�ṹ��AST���ɡ�
ZL_INT zengl_switch_stmt_sequence(ZL_VOID * VM_ARG); //switch...case������������б��ʽ��AST�����﷨��������
ZL_VOID zengl_class_stmt(ZL_VOID * VM_ARG,ZL_INT p); //class...endcls��endclass�ṹ��AST���ɡ�
ZL_INT zengl_class_stmt_sequence(ZL_VOID * VM_ARG); //class...endcls��endclass������������б��ʽ��AST�����﷨��������
ZL_VOID zengl_while_stmt(ZL_VOID * VM_ARG,ZL_INT p); //while...endwhile�ṹ��AST���ɡ�
ZL_INT zengl_while_stmt_sequence(ZL_VOID * VM_ARG); //while...endwhile������������б��ʽ��AST�����﷨��������
ZL_VOID zengl_do_stmt(ZL_VOID * VM_ARG,ZL_INT p);	//do...dowhile�ṹ��AST���ɡ�
ZL_INT zengl_do_stmt_sequence(ZL_VOID * VM_ARG);	//do...dowhile������������б��ʽ��AST�����﷨��������
ZL_INT zengl_detectSetRBracketToSemi(ZL_VOID * VM_ARG); //��if֮��Ĺؼ����Ҳ����������ʱתΪ�ֺţ������Ϳ��Եõ�if�����е������жϱ��ʽ��AST
ZL_INT zengl_express(ZL_VOID * VM_ARG); //�������汾���﷨�������������ö�ջ�����ȼ��������﷨����
ZL_VOID zengl_parser_addcurnode(ZL_VOID * VM_ARG); //����parser_curnode��ǰ�﷨�����ڵ�Ľڵ�ţ��������������Ч�ķ�ֹparser_curnode�����
ZL_VOID zengl_parser_errorExit(ZL_VOID * VM_ARG,ZENGL_ERRORNO errorno, ...);  //�﷨����ʱ���õĴ�ӡ���������Ϣ�ĺ���
ZL_VOID zengl_opLevel_push_stack(ZL_VOID * VM_ARG,ZL_INT nodenum); //��ĳ�ڵ�ѹ��ջ������Ҫ�Ƚ����ȼ�ʱ���ٵ���ջ
ZENGL_PARSER_OP_LEVEL_STACK_TYPE zengl_opLevel_pop_stack(ZL_VOID * VM_ARG,ZL_INT index); //�����ѹ��ջ�Ľڵ㵯����
ZL_VOID zengl_initOpLevelStackList(ZL_VOID * VM_ARG); //��ʼ�����ȼ���ջ��̬����
ZL_VOID zengl_detectCurnodeSyntax(ZL_VOID * VM_ARG); //���ݵ�ǰcurnode�ڵ��curnode+1�ڵ��������ж��Ƿ����﷨����
ZL_VOID zengl_OpLevelForTwo(ZL_VOID * VM_ARG); //ʹ�����ȼ���ջ����Ӽ��˳���˫Ŀ�����
ZL_VOID zengl_OpLevelForAssign(ZL_VOID * VM_ARG); //ʹ�����ȼ���ջ����ֵ����˫Ŀ�����
ZL_VOID zengl_OpLevelForSEMI(ZL_VOID * VM_ARG); //ʹ�����ȼ���ջ����ֺŽ�����
ZL_VOID zengl_OpLevelForPPMM(ZL_VOID * VM_ARG); //ʹ�����ȼ���ջ����ӼӼ��������
ZL_VOID zengl_PPMM_GET_POS(ZL_VOID * VM_ARG); //��ȡ�ӼӼ�������������Ҳ��λ�ñ�ʶ
ZL_VOID zengl_OpLevelForRBRACKET(ZL_VOID * VM_ARG); //ʹ�����ȼ���ջ����������
ZL_VOID zengl_OpLevelForRMBRACKET(ZL_VOID * VM_ARG); //ʹ�����ȼ���ջ������������
ZL_VOID zengl_OpLevelForQuestion(ZL_VOID * VM_ARG); //ʹ�����ȼ���ջ�����ʺ������
ZL_VOID zengl_OpLevelForColon(ZL_VOID * VM_ARG); //ʹ�����ȼ���ջ����ð�������
ZL_VOID zengl_CheckQstColonValid(ZL_VOID * VM_ARG); //����ʺ�ð���Ƿ�һһƥ��
ZL_VOID zengl_ASTAddNodeChild(ZL_VOID * VM_ARG,ZL_INT parent,ZL_INT child); //��child��Ӧ�Ľڵ���뵽parent�ڵ���ӽڵ��С�
ZL_BOOL zengl_CheckIsNegative(ZL_VOID * VM_ARG); //�����жϵ�ǰ�ļ����Ƿ��Ǹ���
ZL_BOOL zengl_CheckIsBitAnd(ZL_VOID * VM_ARG); //�����жϵ�ǰ��"&"�����Ƿ��ǰ�λ�������

//�����Ƕ�����zenglrun_func.c�еĺ���
ZL_VOID zenglrun_init(ZL_VOID * VM_ARG); //��������ʼ��
ZL_VOID * zenglrun_memAlloc(ZL_VOID * VM_ARG,ZL_INT size,ZL_INT * index); //Ϊ�ڴ�ط����ڴ��ָ��ĺ���
ZL_VOID * zenglrun_memReAlloc(ZL_VOID * VM_ARG , ZL_VOID * point , ZL_INT size, ZL_INT * index); //����pointָ��Ĵ�СΪsize��ͬʱ����indexΪ��ָ�����ڴ���е�������
ZL_INT zenglrun_memFindPtrIndex(ZL_VOID * VM_ARG , ZL_VOID * point); //����pointָ����ڴ���в���ָ���Ӧ������ֵ
ZL_VOID * zenglrun_memReUsePtr(ZL_VOID * VM_ARG,ZL_VOID * point,ZL_INT size,ZL_INT * index); //���������size��С�����ڴ�ص�ָ����е�����С�������ò���
ZL_VOID zenglrun_exit(ZL_VOID * VM_ARG,ZENGL_ERRORNO errorno, ...); //�������˳�����
ZL_VOID zenglrun_exit_forApiSetErrThenStop(ZL_VOID * VM_ARG); //ר��ΪzenglApi_SetErrThenStop���API�ӿڶ��Ƶ��˳�����
ZL_VOID zenglrun_ExitPrintSourceInfo(ZL_VOID * VM_ARG,ZENGL_ERRORNO errorno, ...); //�������������˳�ʱ����ӡ����ǰ�������Ӧ��AST�ڵ�����к��ļ�����Ϣ
ZL_VOID zenglrun_memFreeAll(ZL_VOID * VM_ARG); //�������ͷ��ڴ����Դ
ZL_CHAR * zenglrun_SetApiErrorEx(ZL_VOID * VM_ARG,ZENGL_ERRORNO errorno , ...); //ר����������API������Ϣ����չ����
ZL_CHAR * zenglrun_SetApiError(ZL_VOID * VM_ARG,ZENGL_ERRORNO errorno ,ZENGL_SYS_ARG_LIST arglist); //ר����������API������Ϣ�ĺ���
ZL_CHAR * zenglrun_makeInfoString(ZL_VOID * VM_ARG,ZENGL_RUN_INFO_STRING_TYPE * infoStringPtr , ZL_CONST ZL_CHAR * format , ZENGL_SYS_ARG_LIST arglist); //���ɸ�ʽ����Ϣ�ַ���
ZL_VOID zenglrun_freeInfoString(ZL_VOID * VM_ARG,ZENGL_RUN_INFO_STRING_TYPE * infoStringPtr); //�ͷŸ�ʽ����Ϣ�ַ������α�ʹ�С��Ϣ
ZL_VOID zenglrun_info(ZL_VOID * VM_ARG , ZL_CONST ZL_CHAR * format , ...); //����infoFullString��errorFullString�ĸ�ʽ���ַ�����Ϣ����ʹ���û��Զ��庯�����������ʾ
ZL_VOID zenglrun_print(ZL_VOID * VM_ARG , ZL_CONST ZL_CHAR * format , ...); //����printFullString�ĸ�ʽ���ַ�����Ϣ����ʹ���û��Զ��庯�������ʾ
ZL_INT zenglrun_InstDataStringPoolAdd(ZL_VOID * VM_ARG , ZL_CHAR * src); //ָ��������ַ���������ַ���
ZL_VOID zenglrun_initInstDataStringPool(ZL_VOID * VM_ARG); //ָ��������ַ����صĳ�ʼ��
ZL_CHAR * zenglrun_InstDataStringPoolGetPtr(ZL_VOID * VM_ARG , ZL_INT index); //��������ֵ���ַ������в���ָ��������ַ�����ָ����Ϣ
ZL_VOID * zenglrun_strcat(ZL_VOID * VM_ARG,ZL_CHAR * dest,ZL_INT * destIndex,ZL_CHAR * src); //ʹ��C���strcat���ƻ��ڴ�أ����Ծ��Խ�һ��zenglrun_strcat
ZL_VOID zenglrun_memFreeIndex(ZL_VOID * VM_ARG,ZL_VOID * point,ZL_INT * srcindex); //�ͷ��ڴ����ָ��������Ӧ��ָ�롣
ZL_VOID zenglrun_memFreeOnce(ZL_VOID * VM_ARG,ZL_VOID * point); //�ͷ��ڴ���е�ĳ��ָ��
ZL_VOID * zenglrun_strcat2(ZL_VOID * VM_ARG,ZL_CHAR * dest,ZL_INT * destIndex,ZL_CHAR * src1,ZL_CHAR * src2); //��src1��src2���ַ���������һ������dest��
ZL_INT zenglrun_Hash(ZL_VOID * VM_ARG,ZL_CHAR * key); //���ڼ���������еĹ�ϣ������
ZL_INT zenglrun_InsertModuleTable(ZL_VOID * VM_ARG,ZL_CHAR * moduleName,ZL_VM_API_MOD_INIT_FUNC module_init_function); //��ĳģ�������뵽ģ�鶯̬������
ZL_VOID zenglrun_initModuleTable(ZL_VOID * VM_ARG); //ģ�鶯̬�����ʼ��
ZL_INT zenglrun_InsertModFunTable(ZL_VOID * VM_ARG,ZL_INT moduleID,ZL_CHAR * functionName,ZL_VM_API_MOD_FUN_HANDLE handle); //��ģ�麯����Ϣ���뵽ģ�麯����̬������
ZL_VOID zenglrun_initModFunTable(ZL_VOID * VM_ARG); //ģ�麯����̬�����ʼ��
ZL_INT zenglrun_LookupModuleTable(ZL_VOID * VM_ARG,ZL_CHAR * moduleName); //����ĳģ�����Ϣ�����ظ�ģ����ģ�鶯̬�����е�����
ZL_INT zenglrun_LookupModFunTable(ZL_VOID * VM_ARG,ZL_CHAR * functionName); //����ĳģ�麯������Ϣ�����ظ�ģ�麯���ڶ�̬�����е�����
ZL_VOID zenglrun_initExtraDataTable(ZL_VOID * VM_ARG); //��ʼ���û��������ݶ�̬����
ZL_INT zenglrun_InsertExtraDataTable(ZL_VOID * VM_ARG,ZL_CHAR * extraDataName,ZL_VOID * point); //�����ݲ��뵽�������ݶ�̬������

//�����Ƕ�����zenglrun_main.c�еĺ���
ZL_VOID zenglrun_AddInst(ZL_VOID * VM_ARG,ZL_INT pc,ZL_INT nodenum,ZENGL_RUN_INST_TYPE type, 
						 ZENGL_RUN_INST_OP_DATA_TYPE dest_type , ZL_DOUBLE dest_val ,
						 ZENGL_RUN_INST_OP_DATA_TYPE src_type , ZL_DOUBLE src_val); //��ӻ��ָ��
ZL_VOID zenglrun_initInstList(ZL_VOID * VM_ARG); //��ʼ���������Ļ��ָ�
ZENGL_RUN_VIRTUAL_MEM_STRUCT zenglrun_VMemListOps(ZL_VOID * VM_ARG,ZENGL_RUN_VMEM_OP_TYPE opType,ZL_INT memloc,
												  ZENGL_RUN_VIRTUAL_MEM_STRUCT argval); //������������ڴ��������
ZL_VOID zenglrun_initVMemList(ZL_VOID * VM_ARG); //��ʼ��ȫ�������ڴ涯̬����
ZENGL_RUN_VIRTUAL_MEM_STRUCT zenglrun_VStackListOps(ZL_VOID * VM_ARG,ZENGL_RUN_VMEM_OP_TYPE opType,ZL_INT index,
												  ZENGL_RUN_VIRTUAL_MEM_STRUCT argval,ZL_BOOL valid); //�����ջ�ռ�Ĳ�������
ZL_VOID zenglrun_initVStackList(ZL_VOID * VM_ARG); //��ʼ��������Ķ�ջ�ռ䡣
ZL_VOID zenglrun_printInstList(ZL_VOID * VM_ARG,ZL_CHAR * head_title); //��ӡ���ָ�
ZL_VOID zenglrun_RunInsts(ZL_VOID * VM_ARG); //�����ִ�л��ָ�������ʽ��
ZL_VOID zenglrun_RegAssignStr(ZL_VOID * VM_ARG,ZENGL_RUN_REG_TYPE reg,ZL_VOID * str); //��ĳ�ַ�����ֵ��reg�Ĵ���
ZENGL_RUN_RUNTIME_OP_DATA_TYPE zenglrun_op_minis(ZL_VOID * VM_ARG); //MINIS����ָ�����ش����ʽ��op��operate��������д
ZL_VOID zenglrun_op_je(ZL_VOID * VM_ARG,ZENGL_RUN_RUNTIME_OP_DATA * src); //JEָ��ִ�еĲ�����AX�Ĵ���������֮ǰ���ʽ�Ľ������AXΪ0���ʱ�����޸�PC�Ĵ�����ֵ��ʹ�ű�������ת
ZL_VOID zenglrun_op_jne(ZL_VOID * VM_ARG,ZENGL_RUN_RUNTIME_OP_DATA * src); //JNEָ���JEָ��պ��෴��AX�Ĵ���������֮ǰ���ʽ�Ľ������AX��Ϊ0����ΪTRUEʱ�����޸�PC�Ĵ�����ֵ��ʹ�ű�������ת
ZL_VOID zenglrun_op_bits(ZL_VOID * VM_ARG); //��λ�룬������λ����ָ��Ĵ����ʽ
ZL_VOID zenglrun_op_relation(ZL_VOID * VM_ARG); //����С�ڵ���֮��ıȽ������ָ��Ĵ����ʽ��
ZL_VOID zenglrun_op_logic(ZL_VOID * VM_ARG); //AND(��)��OR���򣩣�REVERSE��ȡ�����߼�������Ĵ����ʽ��
ZL_VOID zenglrun_op_addminisget(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem,ZENGL_RUN_INST_TYPE type,ZENGL_RUN_INST_OP_DATA_TYPE memtype); //���Ӽӻ����������ڱ�����ʾ����ǰ��ʱ���Ƚ�����ֵ��һ���һ���ٷ��ؽ��
ZL_VOID zenglrun_op_getaddminis(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem,ZENGL_RUN_INST_TYPE type,ZENGL_RUN_INST_OP_DATA_TYPE memtype); //�ӼӼ���������ڱ�����ʾ������ʱ����ȡֵ�������м�һ���һ����
ZL_VOID zenglrun_op_addminisone(ZL_VOID * VM_ARG,ZENGL_RUN_INST_TYPE type); //�����ʽ�Ľ�����м�һ����һ���ٷ��ؽ����
ZL_VOID zenglrun_op_addr(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem); //ADDR���û��ָ��Ĳ���
ZL_VOID zenglrun_op_set_array(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem); //��������Ԫ��
ZENGL_RUN_VIRTUAL_MEM_LIST * zenglrun_alloc_memblock(ZL_VOID * VM_ARG,ZL_INT * index); //Ϊ�ڴ����䶯̬�ڴ棬ͨ��ZENGL_RUN_VIRTUAL_MEM_LIST�ṹ�������ڴ�飬ZENGL_RUN_VIRTUAL_MEM_LIST�ṹ�е�mem_array�ֶ����·�����ڴ��ָ��
//ZL_INT zenglrun_memblock_getindex(ZL_VOID * VM_ARG,ZL_INT i,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem); //��ȡ�����ڴ�������
ZL_INT zenglrun_memblock_getindex_ext(ZL_VOID * VM_ARG,ZL_INT i,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem, ZENGL_RUN_VIRTUAL_MEM_LIST * memblock); // zenglrun_memblock_getindex����չ�汾�����ַ������й�ϣ���鴦��
ZENGL_RUN_VIRTUAL_MEM_LIST * zenglrun_realloc_memblock(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_LIST * ptr,ZL_INT index); //Ϊ�ڴ�鶯̬������С
ZL_VOID zenglrun_assign_memblock(ZL_VOID * VM_ARG,ZL_VOID ** dest_arg,ZL_VOID * src_arg); //��dest_arg��Ϊsrc_arg��ָ����ڴ�飬ͬʱ����refcount�ڴ������ü���
ZL_VOID zenglrun_memblock_setval(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_LIST * ptr,ZL_INT index,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem); //���������Ӧ�ڴ��ptr��index����λ�õ�ֵ
ZENGL_RUN_VIRTUAL_MEM_STRUCT zenglrun_VMemBlockOps(ZL_VOID * VM_ARG,ZENGL_RUN_VMEM_OP_TYPE op,ZENGL_RUN_VIRTUAL_MEM_LIST * ptr,ZL_INT index,
												   ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem); //������ptr���index����ָ����ڴ���в���
ZL_VOID zenglrun_memblock_free(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_LIST * memblock,ZL_INT * index); //�ͷ�memblock��������ڴ��
ZL_VOID zenglrun_op_get_array(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem); //��ȡ�����е�ĳԪ��
ZL_VOID zenglrun_op_get_array_addr(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem); //��ȡ�ڴ��Ԫ�ص����ã���test = &testarray[0];�����
ZL_VOID zenglrun_op_addminis_one_array(ZL_VOID * VM_ARG,ZENGL_RUN_VIRTUAL_MEM_STRUCT * tmpmem,ZENGL_RUN_INST_TYPE op); //������Ԫ�ؽ��мӼӣ���������
ZL_VOID zenglrun_memblock_freeall_local(ZL_VOID * VM_ARG); //�ͷ�ջ�в������ֺ;ֲ��������ֵ������ڴ��
ZL_VOID zenglrun_FreeAllForReUse(ZL_VOID * VM_ARG); //�����������ʱ���ͷŵ�ȫ�������ڴ棬ջ�ڴ��������ڴ�������
ZL_VOID zenglrun_op_switch(ZL_VOID * VM_ARG); //SWITCHָ��Ĵ���
ZL_LONG zenglrun_getRegInt(ZL_VOID * VM_ARG,ZENGL_RUN_REG_TYPE reg); //���ؼĴ���ֵ��������ʽ
ZL_INT zenglrun_main(ZL_VOID * VM_ARG);	//����������ں���

//�����Ƕ�����zenglrun_hash_array.c�еĺ���
ZL_INT zenglrun_getIndexFromHashCodeTable(ZL_VOID * VM_ARG, ZENGL_RUN_VIRTUAL_MEM_LIST * memblock, ZL_CHAR * key); // �ӹ�ϣ����Ĺ�ϣ���У������ַ���key��ȡ��Ӧ������ֵ
ZL_CHAR * zenglrun_getKeyFromHashCodeTable(ZL_VOID * VM_ARG, ZENGL_RUN_VIRTUAL_MEM_LIST * memblock, ZL_INT memblock_index); // �ӹ�ϣ����Ĺ�ϣ���У���������ֵ����ȡ��Ӧ���ַ���key

//�����Ƕ�����zenglDebug.c�еĺ���
ZL_INT zenglDebug_Compile(ZL_VOID * VM_ARG,ZL_CHAR * script_file,ZENGL_EXPORT_VM_MAIN_ARGS * vm_main_args); //�������ı��벿��
ZL_INT zenglDebug_Run(ZL_VOID * VM_ARG); //�������Ľ���ִ�в���
ZENGL_TOKENTYPE zenglDebug_ReplaceDefConst(ZL_VOID * VM_ARG, ZENGL_TOKENTYPE token); //���������滻�곣���ĺ���
ZL_INT zenglDebug_lookupDefTable(ZL_VOID * VM_ARG, ZL_CHAR * name); //���������Һ궨����滻����
ZENGL_RUN_INST_OP_DATA zenglDebug_SymLookupID(ZL_VOID * VM_ARG,ZL_INT nodenum); //������ͨ���ڵ����������Ҹýڵ�ı�����ʾ�����Զ���������ڴ��е��ڴ��ַ
ZL_INT zenglDebug_SymLookupID_ForDot(ZL_VOID * VM_ARG,ZL_INT nodenum); //����������nodenum��Ӧ�ڵ��classidֵ����Ҫ�������ɵ�������Ļ��ָ��ʱ
ZL_INT zenglDebug_SymLookupClass(ZL_VOID * VM_ARG,ZL_INT nodenum); //���������ݽڵ�Ų�����ID��Ϣ
ZL_INT zenglDebug_SymLookupClassMember(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT parent_classid); //��������SymClassMemberTable�в���parent_classid��Ӧ����ĳ�Աnodenum����Ϣ
ZL_INT zenglDebug_SymLookupFun(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT classid); // �������и��ݽڵ�Ų��Һ���ID��Ϣ
ZL_INT zenglDebug_LookupModFunTable(ZL_VOID * VM_ARG,ZL_CHAR * functionName); //�������в���ĳģ�麯������Ϣ�����ظ�ģ�麯���ڶ�̬�����е�����
ZL_INT zenglDebug_LookupFunID(ZL_VOID * VM_ARG,ZL_INT nodenum); //������ͨ�����������ڵĽڵ�����ֵ�����Һ�����IDֵ
ZL_INT zenglDebug_SetFunInfo(ZL_VOID * VM_ARG); //���õ��������ڵĽű���������
ZL_INT zenglDebug_GetTraceInfo(ZL_VOID * VM_ARG,ZL_CHAR * ApiName,ZL_INT * argArg,ZL_INT * argLOC,ZL_INT * argPC,
							   ZL_CHAR ** fileName,ZL_INT * line,ZL_CHAR ** className,ZL_CHAR ** funcName); //��������ջ�л�ȡ�ű������ĵ������
ZL_INT zenglDebug_InitBreak(ZL_VOID * VM_ARG); //��ʼ���������Ĵ�Ŷϵ�Ķ�̬����
ZL_INT zenglDebug_SetBreak(ZL_VOID * VM_ARG,ZL_INT pc,ZL_CHAR * filename,ZL_INT line,ZL_CHAR * condition,ZL_CHAR * log,ZL_INT count,ZL_BOOL disabled); //�����������öϵ�
ZL_INT zenglDebug_BreakStart(ZL_VOID * VM_ARG); //����������һ��ָ����Ϊ��ִ��һ�εĶϵ�
ZL_INT zenglDebug_DelBreak(ZL_VOID * VM_ARG,ZL_INT index); //ɾ��index������Ӧ�ĵ��Զϵ�
ZL_INT zenglDebug_RestoreBreaks(ZL_VOID * VM_ARG); //�ϵ�ԭָ��ִ�����Ҫ�ָ���ָ���Ӧ�Ķϵ�
ZL_INT zenglDebug_CheckCondition(ZL_VOID * VM_ARG,ZL_CHAR * condition); //���ϵ��Ƿ񵽴�ָ���жϵ�����
ZL_INT zenglDebug_RestoreSingleBreak(ZL_VOID * VM_ARG); //�ָ������ж϶�Ӧ��ԭʼָ��
ZL_INT zenglDebug_ChkAndSetSingleBreak(ZL_VOID * VM_ARG); //������Ҫ�ж��Ƿ���Ҫ����һ��ָ�����õ����ж�

#endif /* _ZENGL_GLOBAL_H_ */
