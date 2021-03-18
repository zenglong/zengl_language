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

#include "zengl_global.h"

/**
 * ͨ���û��Զ����def��ֵ��ѯ��������ѯ����Ӧ�ĺ�ֵ
 * �������ҳ����ĺ�ֵ��token�������õ�token�������Լ�����ֵ��def_StringPool�ַ������е�����ֵ���õ�valIndex����
 * �����ѯ���˺�ֵ�ͷ���ZL_TRUE�����򷵻�ZL_FALSE
 */
static ZL_BOOL zengl_static_DefLookupHandle(ZL_VOID * VM_ARG, ZENGL_TOKENTYPE * token, ZL_INT * valIndex)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_COMPILE_TYPE * compile = &VM->compile;
	ZL_BOOL hasFound = ZL_FALSE;
	if(compile->def_lookup.lookupHandle != ZL_NULL)
	{
		compile->def_lookup.hasFound = ZL_FALSE;
		compile->def_lookup.isInLookupHandle = ZL_TRUE;
		compile->def_lookup.lookupHandle(VM_ARG, compile->tokenInfo.str); //TODO
		compile->def_lookup.isInLookupHandle = ZL_FALSE;
		if(compile->def_lookup.hasFound)
		{
			hasFound = ZL_TRUE;
			(*token) = compile->def_lookup.token;
			(*valIndex) = compile->def_lookup.valIndex;
			compile->def_lookup.hasFound = ZL_FALSE;
			compile->def_lookup.token = ZL_TK_START_NONE;
			compile->def_lookup.valIndex = 0;
		}
	}
	return hasFound;
}

/*�������˳����������������صĳ�����Ϣ*/
ZL_VOID zengl_exit(ZL_VOID * VM_ARG,ZENGL_ERRORNO errorno, ...)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_COMPILE_TYPE * compile = &VM->compile;
	ZL_INT isNeedDebugInfo = ZL_EXP_CP_AF_IN_DEBUG_MODE | ZL_EXP_CP_AF_OUTPUT_DEBUG_INFO;
	VM->errorno = errorno;
	if(VM->errorno != ZL_NO_ERR_SUCCESS)
	{
		ZENGL_SYS_ARG_LIST arg;
		ZENGL_SYS_ARG_START(arg,errorno);
		compile->makeInfoString(VM_ARG,&compile->errorFullString, VM->errorString[VM->errorno] , arg);
		if(compile->userdef_compile_error != ZL_NULL)
			compile->userdef_compile_error(compile->errorFullString.str,compile->errorFullString.count,VM_ARG);
		if(VM->isinApiRun == ZL_FALSE)
			compile->freeInfoString(VM_ARG,&compile->errorFullString);
		ZENGL_SYS_ARG_END(arg);
	}
	compile->end_time = ZENGL_SYS_TIME_CLOCK();
	compile->total_time = compile->end_time - compile->start_time; //�õ��ܵı���ʱ��
	if(VM->errorno != ZL_NO_ERR_SUCCESS)
		VM->isCompileError = ZL_TRUE;
	if((VM->vm_main_args->flags & isNeedDebugInfo) == isNeedDebugInfo) //����ڵ���ģʽ������Ҫ���������Ϣ�����ӡ��Ϣ���Ҳ��ͷ���Դ���Թ���������
	{
		compile->info(VM_ARG,"\n compile time:%.16g s (real compile time: %.16g s , debug print time: %.16g s) totalsize: %.16g Kbyte\n",(ZL_DOUBLE)compile->total_time / CLOCKS_PER_SEC,
			(ZL_DOUBLE)(compile->total_time - compile->total_print_time) / CLOCKS_PER_SEC,
			(ZL_DOUBLE)compile->total_print_time / CLOCKS_PER_SEC,
			(ZL_FLOAT)compile->totalsize / 1024); //for debug
	}

	if((VM->vm_main_args->flags & ZL_EXP_CP_AF_IN_DEBUG_MODE) == 0 && VM->isinApiRun == ZL_FALSE)//�ǵ���ģʽ�����ͷ���Դ
	{
		compile->memFreeAll(VM_ARG);
		compile->infoFullString.str = compile->errorFullString.str = ZL_NULL;
		compile->isDestroyed = ZL_TRUE;
	}
	compile->isinCompiling = ZL_FALSE;
	/**
	 * �������������򿪵Ľű��ļ�û�б��رյĻ�(compile->source.file�����ڿ�ָ��ʱ)���ͽ���رյ�������ᷢ���ڴ�й¶
	 */
	if(compile->source.file != ZL_NULL) {
		ZENGL_SYS_FILE_CLOSE(compile->source.file);
		compile->source.file = ZL_NULL; // �ڹر��ļ��󣬽��ļ�ָ������Ϊ0�������ڵ���ReUse�ӿں�������֮ǰ�Ľű�����ʱ���Ͳ�����Ϊ�ٴιر��ļ������¶δ���
	}
	if(VM->errorno == ZL_NO_ERR_SUCCESS)
		ZENGL_SYS_JMP_LONGJMP_TO(compile->jumpBuffer, 1);
	else
		ZENGL_SYS_JMP_LONGJMP_TO(compile->jumpBuffer, -1);
}

/*�������ͷ��ڴ����Դ*/
ZL_VOID zengl_freeall(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZL_INT i;
	for(i=0;i < compile->mempool.size;i++)
	{
		if(compile->mempool.points[i].point != ZL_NULL)
			ZENGL_SYS_MEM_FREE(compile->mempool.points[i].point);
	}
	ZENGL_SYS_MEM_FREE(compile->mempool.points);
}

/*
���ļ����ַ����ű��л�ȡ��һ���ַ�
*/
ZL_CHAR zengl_getNextchar(ZL_VOID * VM_ARG)
{
	ZL_CHAR ch = ZL_STRNULL;
	ZL_UCHAR tmpch;
	ZL_INT i,j,t;
	ZL_BYTE * state;
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_SOURCE_TYPE * source = &(compile->source);

	if(source->run_str == ZL_NULL) //��������ַ����ű����������ͨ���ļ��ű�
	{
		if(source->file == ZL_NULL)
		{
			source->file = ZENGL_SYS_FILE_OPEN(source->filename,"rb"); //һ��Ҫ��rb��������Ƽ��ܽű����������
			if(source->file == ZL_NULL)
				compile->exit(VM_ARG, ZL_ERR_FILE_CAN_NOT_OPEN ,source->filename);
		}
		if(source->needread || source->cur >= source->buf_read_len)
		{
			if((source->buf_read_len = ZENGL_SYS_FILE_READ(source->buf,sizeof(ZL_UCHAR),ZL_FILE_BUF_SIZE,source->file)) == 0)
			{
				if(ZENGL_SYS_FILE_EOF(source->file))
					return ZL_FILE_EOF;
				else
					compile->exit(VM_ARG, ZL_ERR_FILE_CAN_NOT_GETS ,source->filename);
			}
			source->needread = ZL_FALSE;
			source->cur = 0;
		}
		tmpch = source->buf[source->cur++];
	}
	else //�ַ����ű�
	{
		if(source->cur >= source->run_str_len)
		{
			source->cur++;
			return ZL_FILE_EOF;
		}
		else
			tmpch = source->run_str[source->cur++];
	}
	switch(source->encrypt.type)
	{
	case ZL_ENC_TYPE_XOR: //XOR��ͨ�����ܷ�ʽ
		ch = (ZL_CHAR)(tmpch ^ (ZL_UCHAR)source->encrypt.xor.xor_key_str[source->encrypt.xor.xor_key_cur]);
		if((++source->encrypt.xor.xor_key_cur) >= source->encrypt.xor.xor_key_len)
			source->encrypt.xor.xor_key_cur = 0;
		break;
	case ZL_ENC_TYPE_RC4: //RC4���ܷ�ʽ
		i = source->encrypt.rc4.i;
		j = source->encrypt.rc4.j;
		state = source->encrypt.rc4.state;
		source->encrypt.rc4.i = i = (i + 1) & 0xFF;
		source->encrypt.rc4.j = j = (j + state[i]) & 0xFF;
		t = state[i];
		state[i] = state[j]; 
		state[j] = t;
		ch = state[(state[i] + state[j]) & 0xFF] ^ tmpch;
		break;
	case ZL_ENC_TYPE_NONE: //û�м��ܵ������ֱ�ӷ����ַ�
	default:
		ch = (ZL_CHAR)tmpch;
		break;
	}
	compile->col_no++;
	return ch;
}

/*��ȡtoken��Ϣ*/
ZENGL_TOKENTYPE zengl_getToken(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_STATES state = ZL_ST_START; //������ʼ״̬ΪSTART
	ZENGL_TOKENTYPE token = ZL_TK_ID;
	ZL_CHAR startStr_ch;
	ZL_CHAR ch;
	while(state!=ZL_ST_DOWN) //��state״̬ΪZL_ST_DOWNʱ����ʾ�ҵ�һ���������߱��token(һ�����,�Ӽ�����,���ֵ�ÿ��ɨ�������Ԫ�ض�����token)
	{
		ch = compile->getNextchar(VM_ARG); //��һ���ļ��������ж�ȡһ���ַ�
		switch(state)
		{
		case ZL_ST_START:
			compile->makeTokenInfoLineCol(VM_ARG,compile->line_no,compile->col_no,compile->source.filename);
			if(ch==' ' || ch=='\t' || ch=='\n' || ch=='\r') //����ǻ��лس�֮��ľ�������
			{
				if(ch == '\n')
				{
					compile->line_no++; //�س�����ʱ�޸Ķ�Ӧ�����к�
					compile->col_no=0;
				}
				continue;
			}
			else if(ch >= 0 && (ZENGL_SYS_CTYPE_IS_ALPHA(ch) || ch == '_')) //�ж϶�ȡ���ַ��Ƿ���Ӣ����ĸ���»��ߣ���ʶ��������Ӣ����ĸ���»��߿�ͷ
			{
				state = ZL_ST_INID; //�������ĸ�����Ǿͽ�state״̬������ΪZL_ST_INID ��
				compile->makeTokenStr(VM_ARG,ch); //Ȼ�󽫶�ȡ������ch�ַ�ͨ������makeTokenStr���뵽tokenInfo�Ķ�̬�ַ�����
			}
			else if(ch == '0') //�����0��ͷ���п�����ʮ��������0x12����ʮ�����ƣ�Ҳ�п����ǰ˽�����0e12���ǰ˽��ơ����п�������ͨ��ʮ������0123����123ʮ���ơ�
			{
				state = ZL_ST_INEIGHT_HEX;
				compile->makeTokenStr(VM_ARG,ch);
			}
			else if(ch >= 0 && ZENGL_SYS_CTYPE_IS_DIGIT(ch)) //�ж϶�ȡ���ַ��Ƿ�������
			{
				state = ZL_ST_INNUM; //��������֣����Ǿͽ�state״̬������ΪZL_ST_INNUM ��
				compile->makeTokenStr(VM_ARG,ch); //Ȼ�󽫶�ȡ������ch�ַ�ͨ������makeTokenStr���뵽tokenInfo�Ķ�̬�ַ�����
			}
			else if(ch == '\'' || ch == '"') //�ж��Ƿ����ַ������ַ����Ե����Ż�˫���ſ�ͷ
			{
				state = ZL_ST_INSTR;
				startStr_ch = ch;
				if(compile->tokenInfo.str == ZL_NULL) //���û�г�ʼ������ͨ��makeTokenStr����ʼ��������freeTokenStr�����α�
				{
					compile->makeTokenStr(VM_ARG,ch);
					compile->freeTokenStr(VM_ARG);
				}
				compile->tokenInfo.str[0] = ZL_STRNULL;
			}
			else
			{
				switch(ch)
				{
				case '+':
					state = ZL_ST_INPLUS; //����ַ��ǡ�+���žͽ�state״̬����ΪZL_ST_INPLUS��������ZL_ST_INPLUS״̬������Ϊ+���ܺ͸�ֵ�����ã����Ի���Ҫ��һ���жϡ�
					break;
				case '-':
					state = ZL_ST_INMINIS; //������ͬ��
					break;
				case '*':
					state = ZL_ST_INTIMES; //������ͬ��
					break;
				case '/':
					state = ZL_ST_INDIVIDE;
					break;
				case '%':
					state = ZL_ST_INMOD; //ȡ���������
					break;
				case '=':
					state = ZL_ST_INASSIGN;
					break;
				case '>':
					state = ZL_ST_INGREAT;  //�����ַ�����ZL_ST_INGREAT״̬
					break;
				case '<':
					state = ZL_ST_INLESS;  //С���ַ�����ZL_ST_INLESS״̬
					break;
				case '!':
					state = ZL_ST_INNOT_EQ;  //�����ڻ�ȡ�����Ž���ZL_ST_INNOT_EQ״̬
					break;
				case '&':
					state = ZL_ST_INAND;  //�߼���
					break;
				case '|':
					state = ZL_ST_INOR;  //�߼���
					break;
				case '(':
					state = ZL_ST_DOWN;
					token = ZL_TK_LBRACKET;
					break;
				case ')':
					state = ZL_ST_DOWN;
					token = ZL_TK_RBRACKET;
					break;
				case '[':
					state = ZL_ST_DOWN;
					token = ZL_TK_LMBRACKET; //�������ź����������������������Ԫ�ص����á�
					break;
				case ']':
					state = ZL_ST_DOWN;
					token = ZL_TK_RMBRACKET;
					break;
				case ZL_FILE_EOF:
					state = ZL_ST_DOWN; //EOF�ַ���ʾ��ȡ�����ļ��Ľ�β���򷵻�ZL_TK_ENDFILE��token�������main��������ѭ���ͻ����ɨ�衣
					token = ZL_TK_ENDFILE;
					break;
				case ';':
					state = ZL_ST_DOWN; //�ֺű�ʾ����������
					token = ZL_TK_SEMI;
					break;
				case ',':
					state = ZL_ST_DOWN;
					token = ZL_TK_COMMA; //���������
					break;
				case ':':	 //ð�ţ�һ����switch...case�ṹ�С�
					state = ZL_ST_DOWN;
					token = ZL_TK_COLON;
					break;
				case '?':	 //�ʺţ���Ҫ��... ? ... : ...�ṹ,�ʺ�ǰ�ı��ʽΪ�棬��ִ���ʺź����䣬����ִ��ð�ź����䡣
					state = ZL_ST_DOWN;
					token = ZL_TK_QUESTION_MARK;
					break;
				case '.':	 //����������������Ա������
					state = ZL_ST_DOWN;
					token = ZL_TK_DOT;
					break;
				case '^':	//��λ��������
					state = ZL_ST_INXOR;
					break;
				case '~':	//��λȡ�������
					state = ZL_ST_DOWN;
					token = ZL_TK_BIT_REVERSE;
					break;
				default:
					state = ZL_ST_DOWN; //��������±�ʾ��ȡ����δ�����token����ô�ͷ���ZL_TK_ERROR��
					token = ZL_TK_ERROR;
					break;
				}//switch(ch)
				compile->makeTokenStr(VM_ARG,ch);
			}
			break;
		case ZL_ST_INID:
			if(ch >= 0 && (ZENGL_SYS_CTYPE_IS_ALNUM(ch) || ch == '_')) //��ZL_ST_INID״̬�£�һֱ��ȡ�ַ���ֱ�����ַ�������ĸΪֹ��������ȡ����ĸͨ��makeTokenStr���������ı�ʾ������ʾ�������԰����»��ߺ����֣�������������ĸ��ͷ��
				compile->makeTokenStr(VM_ARG,ch);
			else
			{
				state = ZL_ST_DOWN;
				token = ZL_TK_ID; //��token��ΪZL_TK_ID��ʾ��ȡ����һ����ʾ����ZL_TK_ID���ID��identifier Ӣ����д��
				compile->ungetchar(VM_ARG); //��Ϊ���ȡ��һ������ĸ���ַ���������ungetchar����������һ���ַ�������һ��ɨ��ʹ�á�
			}
			break;
		case ZL_ST_INEIGHT_HEX: 
			if(ch == 'e') //�����0e��ͷ����8���ƣ���0e123
			{
				state = ZL_ST_INEIGHT;
				compile->makeTokenStr(VM_ARG,ch);
				break;
			}
			else if(ch == 'x')	//�����0x��ͷ����ʮ�����ƣ���0x123
			{
				state = ZL_ST_INHEX;
				compile->makeTokenStr(VM_ARG,ch);
				break;
			}	//����Ȳ���0e��Ҳ����0x��ͷ����0123�����Զ����������ZL_ST_INNUM״̬
		case ZL_ST_INNUM: //��ZL_ST_INNUM״̬�£�һֱ��ȡ�ַ���ֱ�����ַ���������Ϊֹ��������ȡ�ĵ�������ͨ��makeTokenStr������������ֵ��
			if(ch >= 0 && ZENGL_SYS_CTYPE_IS_DIGIT(ch))
				compile->makeTokenStr(VM_ARG,ch);
			else if(ch == '.')  //�����С���㣬��˵���Ǹ�����������ZL_ST_INFLOAT״̬
			{
				compile->makeTokenStr(VM_ARG,ch);
				state = ZL_ST_INFLOAT;
			}
			else
			{
				compile->convertStrToDec(VM_ARG); //תΪ��Ч��ʮ���ƣ���0123ת�����Ϊ123����ͷ��Ч��0����˵���
				state = ZL_ST_DOWN;
				token = ZL_TK_NUM;
				compile->ungetchar(VM_ARG);
			}
			break;
		case ZL_ST_INEIGHT:
			if(ch >= '0' && ch <= '7') //�˽�����0��7֮�������
				compile->makeTokenStr(VM_ARG,ch);
			else if(ch >= '8' && ch <= '9')
				compile->exit(VM_ARG, ZL_ERR_CP_INVALID_OCTAL,
				compile->tokenInfo.start_line,compile->tokenInfo.start_col,compile->tokenInfo.filename);
			else
			{
				compile->convertOctalToDec(VM_ARG); //���˽���תΪʮ����
				state = ZL_ST_DOWN;
				token = ZL_TK_NUM;
				compile->ungetchar(VM_ARG);
			}
			break;
		case ZL_ST_INHEX:
			if(ch >= 0 && ZENGL_SYS_CTYPE_IS_HEXDIGIT(ch))	//�ж��Ƿ���ʮ����������
				compile->makeTokenStr(VM_ARG,ch);
			else
			{
				compile->convertHexToDec(VM_ARG); //��ʮ������תΪʮ���ơ�
				state = ZL_ST_DOWN;
				token = ZL_TK_NUM; //��token��ΪNUM��ʾ��ȡ����һ�����֡�
				compile->ungetchar(VM_ARG);
			}
			break;
		case ZL_ST_INFLOAT:  //��ZL_ST_INFLOAT״̬�£�һֱ��ȡ�ַ���ֱ�����ַ���������Ϊֹ�������Ϳ��Եõ�һ����������
			if(ch >= 0 && ZENGL_SYS_CTYPE_IS_DIGIT(ch))
				compile->makeTokenStr(VM_ARG,ch);
			else
			{
				state = ZL_ST_DOWN;
				token = ZL_TK_FLOAT;
				compile->ungetchar(VM_ARG);
			}
			break;
		case ZL_ST_INSTR:
			if(ch == ZL_FILE_EOF) //���һֱ�����ļ���β��û�ҵ������õĵ����Ż�˫���ţ�������﷨����
			{
				compile->exit(VM_ARG, ZL_ERR_CP_INVALID_STR_NO_ENDCHAR,
					compile->tokenInfo.start_line, compile->tokenInfo.start_col, compile->tokenInfo.filename,
					compile->tokenInfo.str,startStr_ch);
			}
			else if(ch != startStr_ch || compile->is_inConvChr > 0) //����ٴ�����startStr_ch���ַ�����ʼ�ĵ����Ż�˫���ţ��ͱ�ʾһ���ַ�������
			{
				compile->makeTokenStrForString(VM_ARG,ch); //makeTokenStrForString���ڴ����ַ�����token,ͬʱ����Ҫ����ת����ַ��������⴦��
			}
			else
			{
				state = ZL_ST_DOWN;
				token = ZL_TK_STR;  //��token��ΪZL_TK_STR��ʾ��ȡ����һ���ַ���
			}
			break;
		case ZL_ST_INASSIGN:  //�����'=='����ZL_TK_EQUAL��������������ֻ��һ��'='����ZL_TK_ASSIGN��ֵ�����
			state = ZL_ST_DOWN;
			if(ch == '=')
			{
				compile->makeTokenStr(VM_ARG,ch);
				token = ZL_TK_EQUAL;
			}
			else
			{
				token = ZL_TK_ASSIGN;
				compile->ungetchar(VM_ARG);
			}
			break;
		case ZL_ST_INGREAT:  //�����'>='��token����ZL_TK_GREAT_EQ���ڵ���������������'>>'�������ƻ����Ƹ�ֵ��������������ZL_TK_GREAT���������
			state = ZL_ST_DOWN;
			if(ch == '=')
			{
				compile->makeTokenStr(VM_ARG,ch);
				token = ZL_TK_GREAT_EQ;
			}
			else if(ch == '>')
			{
				compile->makeTokenStr(VM_ARG,ch);
				state = ZL_ST_INBIT_RIGHT;
			}
			else
			{
				token = ZL_TK_GREAT;
				compile->ungetchar(VM_ARG);
			}
			break;
		case ZL_ST_INLESS:  //������滹���˵Ⱥż�'<='����ôtoken����ZL_TK_LESS_EQС�ڵ���������������"<<"�������ƻ����Ƹ�ֵ��������������ZL_TK_LESSС�������
			state = ZL_ST_DOWN;
			if(ch == '=')
			{
				compile->makeTokenStr(VM_ARG,ch);
				token = ZL_TK_LESS_EQ;
			}
			else if(ch == '<')
			{
				compile->makeTokenStr(VM_ARG,ch);
				state = ZL_ST_INBIT_LEFT;
			}
			else
			{
				token = ZL_TK_LESS;
				compile->ungetchar(VM_ARG);
			}
			break;
		case ZL_ST_INNOT_EQ:  //!�������'='��'!='���ʾtoken��ZL_TK_NOT_EQ��������������������'!'��ZL_TK_REVERSEȡ�������
			state = ZL_ST_DOWN;
			if(ch == '=')
			{
				compile->makeTokenStr(VM_ARG,ch);
				token = ZL_TK_NOT_EQ;
			}
			else
			{
				token = ZL_TK_REVERSE;
				compile->ungetchar(VM_ARG);
			}
			break;
		case ZL_ST_INAND: 
			state = ZL_ST_DOWN;
			if(ch == '&') //����&��һ��'&&'��ʾtoken��ZL_TK_AND�߼����������
			{
				compile->makeTokenStr(VM_ARG,ch);
				token = ZL_TK_AND;
			}
			else if(ch == '=')
			{
				compile->makeTokenStr(VM_ARG,ch);
				token = ZL_TK_BIT_AND_ASSIGN; //&=�����
			}
			else  //ֻ��һ��&��ʾtoken�Ǳ������÷���Ҳ�п����ǰ�λ�룬����zengl_parser.c���ٽ����ж�
			{
				token = ZL_TK_ADDRESS; //ZL_TK_ADDRESS�Ǳ������õ�token��ö��ֵ�� 
				compile->ungetchar(VM_ARG);
			}
			break;
		case ZL_ST_INOR:
			state = ZL_ST_DOWN;
			if(ch == '|') //����|��һ��'||'��ʾtoken���߼��������
			{
				compile->makeTokenStr(VM_ARG,ch);
				token = ZL_TK_OR;
			}
			else if(ch == '=')
			{
				compile->makeTokenStr(VM_ARG,ch);
				token = ZL_TK_BIT_OR_ASSIGN; //|=�����
			}
			else //ֻ��һ��"|"���ʾ��λ�������
			{
				token = ZL_TK_BIT_OR;
				compile->ungetchar(VM_ARG);
			}
			break;
		case ZL_ST_INPLUS:
			state = ZL_ST_DOWN;
			if(ch == '+')
			{
				compile->makeTokenStr(VM_ARG,ch);
				token = ZL_TK_PLUS_PLUS; //++�����
			}
			else if(ch == '=')
			{
				compile->makeTokenStr(VM_ARG,ch);
				token = ZL_TK_PLUS_ASSIGN; //+=�����
			}
			else
			{
				token = ZL_TK_PLUS; //�ӷ�����
				compile->ungetchar(VM_ARG);
			}
			break;
		case ZL_ST_INMINIS:
			state = ZL_ST_DOWN;
			if(ch == '-')
			{
				compile->makeTokenStr(VM_ARG,ch);
				token = ZL_TK_MINIS_MINIS;
			}
			else if(ch == '=')
			{
				compile->makeTokenStr(VM_ARG,ch);
				token = ZL_TK_MINIS_ASSIGN;
			}
			else
			{
				token = ZL_TK_MINIS;
				compile->ungetchar(VM_ARG);
			}
			break;
		case ZL_ST_INTIMES:
			state = ZL_ST_DOWN;
			if(ch == '=')
			{
				compile->makeTokenStr(VM_ARG,ch);
				token = ZL_TK_TIMES_ASSIGN;
			}
			else
			{
				token = ZL_TK_TIMES;
				compile->ungetchar(VM_ARG);
			}
			break;
		case ZL_ST_INDIVIDE:
			state = ZL_ST_DOWN;
			if(ch == '=')
			{
				compile->makeTokenStr(VM_ARG,ch);
				token = ZL_TK_DIVIDE_ASSIGN;
			}
			else if(ch == '/') //�������'//' ��˵����ע�ͣ������ZL_ST_INCOMMENT
			{
				state = ZL_ST_INCOMMENT;
			}
			else if(ch == '*') //�������'/*' ��˵���Ƕ���ע�ͣ������ZL_ST_INMULTI_COMMENT
			{
				state = ZL_ST_INMULTI_COMMENT;
			}
			else
			{
				token = ZL_TK_DIVIDE;
				compile->ungetchar(VM_ARG);
			}
			break;
		case ZL_ST_INCOMMENT:	  //����ע��
			if(ch == '\n') //�������з������Ӧ�޸����кŵ�ȫ�ֱ�������Ϊ�ǵ���ע�ͣ����Խ�state��ΪZL_ST_START��˵��ע�ͽ�������ͷ��ʼ������һ��token
			{
				compile->line_no++;
				compile->col_no=0;
				state = ZL_ST_START;
				compile->freeTokenStr(VM_ARG);
			}
			else if(ch == ZL_FILE_EOF) //��������ļ���β����state��ΪZL_ST_DOWN��������ZL_TK_ENDFILE�ļ�������token
			{
				state = ZL_ST_DOWN;
				token = ZL_TK_ENDFILE;
			}
			else
				continue;  //��ע�Ͳ���continue����
			break;
		case ZL_ST_INMULTI_COMMENT: //����ע��
			if(ch == '*')  //����ע������*��ʱ��ʾ����ע���п��ܽ�����
				state = ZL_ST_INENDMULTI_COMMENT;	 //����ZL_ST_INENDMULTI_COMMENT״̬
			else if(ch == '\n')	//�������з�����Ӧ�޸����кŵ�ȫ�ֱ�������continue����
			{
				compile->line_no++;
				compile->col_no=0;
				continue;
			}
			else if(ch == ZL_FILE_EOF)
				compile->exit(VM_ARG, ZL_ERR_CP_GT_INVALID_MULTI_COMMENT,
				compile->tokenInfo.start_line,compile->tokenInfo.start_col,compile->tokenInfo.filename,
				compile->tokenInfo.str);
			else
				continue;
			break;
		case ZL_ST_INENDMULTI_COMMENT:
			if(ch == '/') //����� */ ��˵������ע�ͽ�����
			{
				state = ZL_ST_START;
				compile->freeTokenStr(VM_ARG);
			}
			else if(ch == '*') //�ڶ���ע������Ǻ�ֱ��continue����������ע�ͲŲ������
			{
				continue;
			}
			else if(ch == '\n')	//�������з�����Ӧ�޸����кţ�ͬʱ˵��ǰ���*������ͨ�ַ����ǽ���������������ZL_ST_INMULTI_COMMENT״̬
			{
				compile->line_no++;
				compile->col_no=0;
				state = ZL_ST_INMULTI_COMMENT;
			}
			else if(ch == ZL_FILE_EOF)
				compile->exit(VM_ARG, ZL_ERR_CP_GT_INVALID_MULTI_COMMENT,
				compile->tokenInfo.start_line,compile->tokenInfo.start_col,compile->tokenInfo.filename,
				compile->tokenInfo.str);
			else
				state = ZL_ST_INMULTI_COMMENT; //�������*/��˵������ͨ��*���ַ�����������ZL_ST_INMULTI_COMMENT״̬
			break;
		case ZL_ST_INMOD:
			state = ZL_ST_DOWN;
			if(ch == '=')
			{
				compile->makeTokenStr(VM_ARG,ch);
				token = ZL_TK_MOD_ASSIGN; //��ȡ���ֵ�������
			}
			else
			{
				token = ZL_TK_MOD; //ȡ���������
				compile->ungetchar(VM_ARG);
			}
			break;
		case ZL_ST_INXOR:
			state = ZL_ST_DOWN;
			if(ch == '=')
			{
				compile->makeTokenStr(VM_ARG,ch);
				token = ZL_TK_BIT_XOR_ASSIGN; //^=�����
			}
			else
			{
				token = ZL_TK_BIT_XOR; //"^"��λ��������
				compile->ungetchar(VM_ARG);
			}
			break;
		case ZL_ST_INBIT_RIGHT:
			state = ZL_ST_DOWN;
			if(ch == '=')
			{
				compile->makeTokenStr(VM_ARG,ch);
				token = ZL_TK_BIT_RIGHT_ASSIGN; //">>="���Ƹ�ֵ�����token
			}
			else
			{
				token = ZL_TK_BIT_RIGHT; //">>"���������token
				compile->ungetchar(VM_ARG);
			}
			break;
		case ZL_ST_INBIT_LEFT:
			state = ZL_ST_DOWN;
			if(ch == '=')
			{
				compile->makeTokenStr(VM_ARG,ch);
				token = ZL_TK_BIT_LEFT_ASSIGN; //"<<="���Ƹ�ֵ�����token
			}
			else
			{
				token = ZL_TK_BIT_LEFT; //"<<"���������token
				compile->ungetchar(VM_ARG);
			}
			break;
		}//switch(state)
	}//while(state!=ZL_ST_DOWN)
	if(token == ZL_TK_ID)
	{
		token = compile->lookupReserve(VM_ARG,token);

		if(token == ZL_TK_ID && !compile->is_inDefRsv) //def����ĳ�����������������������¶�����������
		{
			token = compile->ReplaceDefConst(VM_ARG,token);
		}
	}
	return token;
}//ZENGL_TOKENTYPE zengl_getToken(void * VM_ARG)

/*����token�����кŵ���Ϣ*/
ZL_VOID zengl_makeTokenInfoLineCol(ZL_VOID * VM_ARG,ZL_INT line,ZL_INT col,ZL_CHAR * filename)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	compile->tokenInfo.start_line = line;
	compile->tokenInfo.start_col = col;
	compile->tokenInfo.filename = filename; //����token���к���Ϣʱ�����ļ�����Ϣ��
}

/*����token���ַ�����Ϣ*/
ZL_VOID zengl_makeTokenStr(ZL_VOID * VM_ARG,ZL_CHAR ch)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	if(compile->tokenInfo.str == ZL_NULL || (compile->tokenInfo.cur >= (compile->tokenInfo.size - 3)))
	{
		compile->tokenInfo.size += ZL_TOKENSTR_SIZE;
		if(compile->tokenInfo.str == ZL_NULL) //Ϊ��ǰɨ��token���ַ�����Ϣ�����ڴ�
			compile->tokenInfo.str = compile->memAlloc(VM_ARG,compile->tokenInfo.size);
		else
			compile->tokenInfo.str = compile->memReAlloc(VM_ARG,compile->tokenInfo.str,compile->tokenInfo.size);
		if(compile->tokenInfo.str == ZL_NULL)
		{
			compile->exit(VM_ARG, ZL_ERR_CP_MTSTR_MALLOC_FAILED);
		}
	}
	compile->tokenInfo.str[compile->tokenInfo.cur++] = ch;
	compile->tokenInfo.str[compile->tokenInfo.cur] = ZL_STRNULL;
	compile->tokenInfo.count++;
	if(ch == '\n')  //���ַ����д��ڻ��з���ʱ��Ҳ��Ӧ���������кţ������Ͳ�����ַ��ű�����к�������ļ������������
	{
		compile->line_no++;
		compile->col_no=0;
	}
}

/*�����������ڴ溯����������������ڴ�ָ��ͳһ�������������ڴ�ع���*/
ZL_VOID * zengl_malloc(ZL_VOID * VM_ARG , ZL_INT size)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_COMPILE_TYPE * compile = &VM->compile;
	ZL_INT i = 0;
	ZL_INT increase = 0; //ͳ���ڴ�����
	ZL_VOID * point = ZL_NULL;
	if(compile->mempool.count == compile->mempool.size)
	{
		i = compile->mempool.size;
		compile->mempool.size += ZL_MEM_POOL_SIZE;
		increase += ZL_MEM_POOL_SIZE * sizeof(ZENGL_MEM_POOL_POINT_TYPE);
		compile->mempool.points = ZENGL_SYS_MEM_RE_ALLOC(compile->mempool.points,compile->mempool.size * sizeof(ZENGL_MEM_POOL_POINT_TYPE));
		ZENGL_SYS_MEM_SET((ZL_VOID *)(compile->mempool.points + (compile->mempool.size - ZL_MEM_POOL_SIZE)),0,
			(ZL_SIZE_T)increase);
	}
	for(;i < compile->mempool.size;i++)
	{
		if(compile->mempool.points[i].point == ZL_NULL) //���ڴ������Ѱnull��ָ�룬�ҵ���Ϊ������ڴ棬����ָ��ֵ���ء�
		{
			point = ZENGL_SYS_MEM_ALLOC(size);
			if(point != ZL_NULL)
			{
				compile->mempool.points[i].point = point;
				compile->mempool.points[i].size = size;
				compile->mempool.count++;
				increase += size;
				compile->mempool.totalsize += increase; //�����ڴ���ڴ�ռ����ͳ����
				compile->totalsize += increase; //���ӱ������ڴ�ռ����ͳ����
				VM->totalsize += increase; //������������ڴ�ռ����ͳ����
				return point;
			}
			else
				compile->exit(VM_ARG, ZL_ERR_CP_MP_MALLOC_FAILED);
		}
	}
	compile->exit(VM_ARG, ZL_ERR_CP_MP_MALLOC_NO_NULL_POINT);
	return ZL_NULL;
}

/*�����ڴ����ĳ��ָ����ڴ��С*/
ZL_VOID * zengl_realloc(ZL_VOID * VM_ARG , ZL_VOID * point , ZL_INT size)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_COMPILE_TYPE * compile = &VM->compile;
	ZL_INT i;
	ZL_INT increase = 0; //ͳ���ڴ�����
	if(point == ZL_NULL)
	{
		point = compile->memAlloc(VM_ARG,size);
		return point;
	}
	for(i=0;i<compile->mempool.size;i++)
	{
		if(compile->mempool.points[i].point == point) //���ڴ�����ҵ���Ӧ��ָ�룬Ȼ��ͨ��ϵͳ���������ڴ��С��
		{
			point = ZENGL_SYS_MEM_RE_ALLOC(point,size);
			if(point != ZL_NULL)
				compile->mempool.points[i].point = point;
			else
				compile->exit(VM_ARG, ZL_ERR_CP_MP_REALLOC_FAILED);
			increase = size - compile->mempool.points[i].size;
			compile->mempool.points[i].size = size;
			compile->mempool.totalsize += increase; //�����ڴ���ڴ�ռ����ͳ����
			compile->totalsize += increase; //���ӱ������ڴ�ռ����ͳ����
			VM->totalsize += increase; //������������ڴ�ռ����ͳ����
			return point;
		}
	}
	point = compile->memAlloc(VM_ARG,size);
	return point;
}

/*�ͷű������ڴ���е�ĳ��ָ��*/
ZL_VOID zengl_freeonce(ZL_VOID * VM_ARG,ZL_VOID * point)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_COMPILE_TYPE * compile = &VM->compile;
	ZL_INT i=0;
	ZL_INT increase = 0; //ͳ���ڴ�����
	for(;i < compile->mempool.size;i++)
	{
		if(compile->mempool.points[i].point == point)
		{
			ZENGL_SYS_MEM_FREE(compile->mempool.points[i].point);
			compile->mempool.points[i].point = ZL_NULL;
			increase -= compile->mempool.points[i].size;
			compile->mempool.points[i].size = 0;
			compile->mempool.count--;
			compile->mempool.count = compile->mempool.count < 0 ? 0 : compile->mempool.count;
			compile->mempool.totalsize += increase; //�����ڴ���ڴ�ռ����ͳ����
			compile->totalsize += increase; //���ӱ������ڴ�ռ����ͳ����
			VM->totalsize += increase; //������������ڴ�ռ����ͳ����
			break;
		}
	}
	if(i >= compile->mempool.size)
		return;
}

/*��Դ�ű��ַ�ɨ���α����һ��ͬʱ���кż�һ*/
ZL_VOID zengl_ungetchar(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_SOURCE_TYPE * source = &(compile->source);
	ZL_INT j_orig , t;
	ZL_BYTE * state;
	source->cur > 0 ? source->cur-- : source->cur;
	switch(source->encrypt.type)
	{
	case ZL_ENC_TYPE_XOR: //XOR��ͨ�������α�ع�
		source->encrypt.xor.xor_key_cur > 0 ? source->encrypt.xor.xor_key_cur-- : 
			(source->encrypt.xor.xor_key_cur = source->encrypt.xor.xor_key_len - 1);
		break;
	case ZL_ENC_TYPE_RC4: //RC4״̬���Ӽ�i,jָ��ع�
		state = source->encrypt.rc4.state;
		j_orig = (source->encrypt.rc4.j - state[source->encrypt.rc4.j]) & 0xFF;
		t = state[source->encrypt.rc4.i]; 
		state[source->encrypt.rc4.i] = state[source->encrypt.rc4.j]; 
		state[source->encrypt.rc4.j] = t; 
		source->encrypt.rc4.i = (source->encrypt.rc4.i - 1) & 0xFF;
		source->encrypt.rc4.j = j_orig;
		break;
	}
	compile->col_no > 0 ? compile->col_no-- : compile->col_no;
}

/*
���ַ���תΪ��Ч��ʮ���������ٴ�ŵ�tokenInfo�� 
��0123����zengl_convertStrToDecת����ͱ�Ϊ��Ч��ʮ������123
*/
ZL_VOID zengl_convertStrToDec(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZL_INT i,len;
	ZL_INT idec=0;
	ZL_CHAR buf[100];
	ZL_CHAR tmpch;
	ZL_BOOL isNegative = ZL_FALSE;

	for(i = 0; ;i++)
	{
		tmpch = compile->tokenInfo.str[i];
		if(tmpch >= '0' && tmpch <= '9')
			idec = 10 * idec + (tmpch - '0');
		else if(i == 0 && tmpch == '-')
			isNegative = ZL_TRUE;
		else
			break;
	}
	if(isNegative == ZL_TRUE) 
		idec = -idec;
	ZENGL_SYS_SPRINTF(buf,"%d",idec);
	len = ZENGL_SYS_STRLEN(buf);
	compile->freeTokenStr(VM_ARG);
	for(i=0;i<len;i++)
		compile->makeTokenStr(VM_ARG,buf[i]);
}

/*�޸��α�ȳ�Ա���ͷŵ�ǰɨ��token�Ķ�̬�ַ�����Ϣ���Թ���һ��ɨ��tokenʱʹ��*/
ZL_VOID zengl_freeTokenStr(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	compile->tokenInfo.cur = 0;  //tokenȫ����ʱ�������ַ����α�����Ϊ0
	compile->tokenInfo.count = 0; //�������ַ���������Ϊ0
	compile->tokenInfo.reserve = -1;  //token��ö��ֵ����Ϊ-1
}

/*
����ǰɨ��token�ɰ˽���תΪʮ����
*/
ZL_VOID zengl_convertOctalToDec(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZL_INT i,mid=0,len;
	ZL_INT idec=0;
	ZL_CHAR * str,buf[100];
	ZL_BOOL isNegative = ZL_FALSE;
	if(compile->tokenInfo.count <= 2 ||
	   (compile->tokenInfo.count == 3 && compile->tokenInfo.str[0] == '-'))
		compile->exit(VM_ARG, ZL_ERR_CP_INVALID_OCTAL,
		compile->tokenInfo.start_line,compile->tokenInfo.start_col,compile->tokenInfo.filename);

	if(compile->tokenInfo.str[0] == '-')
	{
		str = compile->tokenInfo.str + 3;
		isNegative = ZL_TRUE;
	}
	else
		str = compile->tokenInfo.str + 2;
	len = ZENGL_SYS_STRLEN(str);
	for(i=0;i < len;i++)
	{
		if(str[i] >= '0' && str[i] <= '7')
			mid = str[i] - '0';
		else
			compile->exit(VM_ARG, ZL_ERR_CP_INVALID_OCTAL,
			compile->tokenInfo.start_line,compile->tokenInfo.start_col,compile->tokenInfo.filename);

		mid <<= ((len - i - 1)*3);
		idec |= mid;
	}
	if(isNegative == ZL_TRUE)
		idec = -idec;
	ZENGL_SYS_SPRINTF(buf,"%d",idec);
	len = ZENGL_SYS_STRLEN(buf);
	compile->freeTokenStr(VM_ARG);
	for(i=0;i<len;i++)
		compile->makeTokenStr(VM_ARG,buf[i]);
}

/*
��ɨ��token��ʮ�����Ƹ�ʽתΪʮ���Ƹ�ʽ��
*/
ZL_VOID zengl_convertHexToDec(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZL_INT i,mid=0,len;
	ZL_INT idec=0;
	ZL_CHAR * str,buf[100];
	ZL_BOOL isNegative = ZL_FALSE;
	if(compile->tokenInfo.count <= 2 ||
	   (compile->tokenInfo.count == 3 && compile->tokenInfo.str[0] == '-'))
		compile->exit(VM_ARG, ZL_ERR_CP_INVALID_HEX,
		compile->tokenInfo.start_line,compile->tokenInfo.start_col,compile->tokenInfo.filename);

	if(compile->tokenInfo.str[0] == '-')
	{
		str = compile->tokenInfo.str + 3;
		isNegative = ZL_TRUE;
	}
	else
		str = compile->tokenInfo.str + 2;
	len = ZENGL_SYS_STRLEN(str);
	for(i=0;i < len;i++)
	{
		if(str[i] >= '0' && str[i] <= '9')
			mid = str[i] - '0';
		else if(str[i] >= 'a' && str[i] <= 'f')
			mid = str[i] - 'a' + 10;
		else if(str[i] >= 'A' && str[i] <= 'F')
			mid = str[i] - 'A' + 10;
		else
			compile->exit(VM_ARG, ZL_ERR_CP_INVALID_HEX,
			compile->tokenInfo.start_line,compile->tokenInfo.start_col,compile->tokenInfo.filename);

		mid <<= ((len - i - 1)<<2);	 //(len-i-1)<<2�൱��(len-i-1)*4
		idec |= mid;
	}
	if(isNegative == ZL_TRUE)
		idec = -idec;
	ZENGL_SYS_SPRINTF(buf,"%d",idec);
	len = ZENGL_SYS_STRLEN(buf);
	compile->freeTokenStr(VM_ARG);
	for(i=0;i<len;i++)
		compile->makeTokenStr(VM_ARG,buf[i]);
}

/*
����ĺ���������ר�Ž����ַ������͵�token��
�ַ����е�б�ܻὫ�����ţ�˫���ţ�б�ܣ�r��n��t��v��a��b��f�ַ�����ת�壬�����ַ���ת��
*/
ZL_VOID zengl_makeTokenStrForString(ZL_VOID * VM_ARG,ZL_CHAR ch)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZL_INT count = 0;
	if(compile->tokenInfo.str == ZL_NULL || (compile->tokenInfo.cur >= (compile->tokenInfo.size-3)))
	{
		compile->tokenInfo.size += ZL_TOKENSTR_SIZE;
		if(compile->tokenInfo.str == ZL_NULL)
			compile->tokenInfo.str = compile->memAlloc(VM_ARG,compile->tokenInfo.size);
		else
			compile->tokenInfo.str = compile->memReAlloc(VM_ARG,compile->tokenInfo.str,compile->tokenInfo.size);
		if(compile->tokenInfo.str == ZL_NULL)
		{
			compile->exit(VM_ARG, ZL_ERR_CP_MTSFS_MALLOC_FAILED);
		}
	}
	if(ch == '\\' && compile->is_inConvChr == ZL_FALSE)
	{
		compile->is_inConvChr = ZL_TRUE;
		return;
	}
	switch(ch)
	{
	case '\\':
	case '\'':
	case '"':
		compile->tokenInfo.str[compile->tokenInfo.cur++] = ch;
		count++;
		break;
	case 'n':
		if(compile->is_inConvChr == ZL_TRUE)
			compile->tokenInfo.str[compile->tokenInfo.cur++] = '\n';
		else
			compile->tokenInfo.str[compile->tokenInfo.cur++] = ch;
		count++;
		break;
	case 'r':
		if(compile->is_inConvChr == ZL_TRUE)
			compile->tokenInfo.str[compile->tokenInfo.cur++] = '\r';
		else
			compile->tokenInfo.str[compile->tokenInfo.cur++] = ch;
		count++;
		break;
	case 't':
		if(compile->is_inConvChr == ZL_TRUE)
			compile->tokenInfo.str[compile->tokenInfo.cur++] = '\t';
		else
			compile->tokenInfo.str[compile->tokenInfo.cur++] = ch;
		count++;
		break;
	case 'v':
		if(compile->is_inConvChr == ZL_TRUE)
			compile->tokenInfo.str[compile->tokenInfo.cur++] = '\v';
		else
			compile->tokenInfo.str[compile->tokenInfo.cur++] = ch;
		count++;
		break;
	case 'a':
		if(compile->is_inConvChr == ZL_TRUE)
			compile->tokenInfo.str[compile->tokenInfo.cur++] = '\a';
		else
			compile->tokenInfo.str[compile->tokenInfo.cur++] = ch;
		count++;
		break;
	case 'b':
		if(compile->is_inConvChr == ZL_TRUE)
			compile->tokenInfo.str[compile->tokenInfo.cur++] = '\b';
		else
			compile->tokenInfo.str[compile->tokenInfo.cur++] = ch;
		count++;
		break;
	case 'f':
		if(compile->is_inConvChr == ZL_TRUE)
			compile->tokenInfo.str[compile->tokenInfo.cur++] = '\f';
		else
			compile->tokenInfo.str[compile->tokenInfo.cur++] = ch;
		count++;
		break;
	default:
		if(compile->is_inConvChr == ZL_TRUE)
		{
			compile->tokenInfo.str[compile->tokenInfo.cur++] = '\\';
			count++;
		}
		compile->tokenInfo.str[compile->tokenInfo.cur++] = ch;
		count++;
		break;
	}
	compile->tokenInfo.str[compile->tokenInfo.cur] = ZL_STRNULL;
	compile->tokenInfo.count += count;
	if(ch == '\n')
	{
		compile->line_no++;  //�������з������޸����кŵ�ֵ��
		compile->col_no=0;
	}
	if(compile->is_inConvChr == ZL_TRUE)
	{
		compile->is_inConvChr = ZL_FALSE; //ת���������������ΪZL_FALSE���رմ˴�ת��
	}
}

/*��ѯ�ؼ��ֵ�ö��ֵ*/
ZENGL_TOKENTYPE zengl_lookupReserve(ZL_VOID * VM_ARG , ZENGL_TOKENTYPE token)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZL_INT len = ZENGL_SYS_STRLEN(compile->tokenInfo.str);
	ZL_INT i = 1;
	while(compile->reserveString[i] != ZL_NULL)
	{
		ZL_INT len_reserve = ZENGL_SYS_STRLEN(compile->reserveString[i]);
		if(len == len_reserve &&
			compile->reserveString[i][0] == compile->tokenInfo.str[0] &&
			!ZENGL_SYS_STRNCMP(compile->reserveString[i],compile->tokenInfo.str,len))
		{
			token = ZL_TK_RESERVE;
			switch(i)
			{
			case ZL_RSV_ENDSWITCH: //endswitch�ȼ���endswt
				compile->tokenInfo.reserve = ZL_RSV_ENDSWT;
				break;
			case ZL_RSV_ENDCLASS: //endclass�ȼ���endcls
				compile->tokenInfo.reserve = ZL_RSV_ENDCLS;
				break;
			default:
				compile->tokenInfo.reserve = i;  //���ùؼ��ʵ�ö��ֵ
				break;
			}
			break;
		}
		i++;
	}
	return token;
}

/*
�ʷ�ɨ��ʱ�������def�ؼ��֣��͵���zengl_AddDefConst��������ͺ��Ӧ��ֵ���뵽��ϣ���
def�궨��Ķ�̬�����С��궨������ַ�����Ϣ�������def_StringPool�ַ������С�
*/
ZL_VOID zengl_AddDefConst(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_TOKENTYPE token;
	ZL_BOOL defLookupHasFound = ZL_FALSE;
	ZL_INT nameIndex, valIndex;
	ZL_INT temp_line,temp_col;
	ZL_INT def_start_line,def_start_col; //def�������ʼ���кţ���def�ؼ��ֵ����к�Ϊ׼
	compile->freeTokenStr(VM_ARG);
	def_start_line = temp_line = compile->tokenInfo.start_line;
	def_start_col = temp_col = compile->tokenInfo.start_col;
	compile->is_inDefRsv = ZL_TRUE;
	token = compile->getToken(VM_ARG);
	compile->is_inDefRsv = ZL_FALSE;
	if(token != ZL_TK_ID)
		compile->exit(VM_ARG,ZL_ERR_CP_DEF_MUST_WITH_ID,
		temp_line,temp_col,compile->tokenInfo.filename);
	if(compile->SymIsSelfToken(VM_ARG, ZL_NULL)) { // def�ؼ��ֺ��治����ʹ��self��Ϊ�����ƣ���Ϊself�Ѿ�����������ṹ�б�ʾ��ǰ�������������
		compile->exit(VM_ARG,ZL_ERR_CP_DEF_CAN_NOT_WITH_SELF,
				temp_line,temp_col,compile->tokenInfo.filename);
	}
	nameIndex = compile->DefPoolAddString(VM_ARG,compile->tokenInfo.str);
	compile->freeTokenStr(VM_ARG);
	temp_line = compile->tokenInfo.start_line;
	temp_col = compile->tokenInfo.start_col;
	valIndex = 0;
	token = compile->getToken(VM_ARG);
	// ���def�ĺ�ֵ��ID��ʶ�����򽫱�ʶ����Ӧ���ַ�����Ϊ��ѯ���ƣ����ݸ��û��Զ���Ĳ�ѯ������������ѯ�����Ϊ���յĺ�ֵ
	if(token == ZL_TK_ID)
	{
		if(compile->def_lookup.lookupHandle != ZL_NULL)
		{
			defLookupHasFound = zengl_static_DefLookupHandle(VM_ARG, &token, &valIndex);
		}
		if(!defLookupHasFound)
		{
			compile->exit(VM_ARG,ZL_ERR_CP_DEF_LOOKUP_NOT_FOUND,
					compile->tokenInfo.str,
					compile->tokenInfo.start_line,
					compile->tokenInfo.start_col,
					compile->tokenInfo.filename,
					compile->tokenInfo.str);
		}
	}
	else if(token == ZL_TK_MINIS) //def����ĺ�ֵ�����Ǹ���
		token = compile->getToken(VM_ARG);
	if(token == ZL_TK_NUM || token == ZL_TK_FLOAT || token == ZL_TK_STR)
	{
		if(valIndex == 0)
		{
			valIndex =  compile->DefPoolAddString(VM_ARG,compile->tokenInfo.str);
		}
		compile->insert_HashTableForDef(VM_ARG,nameIndex,token,valIndex,
			def_start_line,def_start_col);
		compile->freeTokenStr(VM_ARG);
		temp_line = compile->tokenInfo.start_line;
		temp_col = compile->tokenInfo.start_col;
		token = compile->getToken(VM_ARG);
		if(token != ZL_TK_SEMI)
			compile->exit(VM_ARG,ZL_ERR_CP_DEF_NO_END_SEMI,
			temp_line,temp_col,compile->tokenInfo.filename);
	}
	else
		compile->exit(VM_ARG,ZL_ERR_CP_DEF_INVALID_VALUE_TYPE,
		temp_line,temp_col,compile->tokenInfo.filename);
}

/*
������src��ӵ�def_StringPool�ַ������С������ظ��ַ����ڳ��е�����
*/
ZL_INT zengl_DefPoolAddString(ZL_VOID * VM_ARG , ZL_CHAR * src)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZL_INT len;
	ZL_INT i,j;
	if(!compile->def_StringPool.isInit)
		compile->initDefStringPool(VM_ARG);
	if(src == ZL_NULL)
		return 0;
	len = ZENGL_SYS_STRLEN(src);
	if(compile->def_StringPool.count == compile->def_StringPool.size ||
		compile->def_StringPool.count + len + 1 > compile->def_StringPool.size)
	{
		compile->def_StringPool.size += ZL_DEF_STR_POOL_SIZE;
		compile->def_StringPool.ptr  = compile->memReAlloc(VM_ARG,compile->def_StringPool.ptr,
			compile->def_StringPool.size * sizeof(ZL_CHAR));
		ZENGL_SYS_MEM_SET(compile->def_StringPool.ptr + (compile->def_StringPool.size - ZL_DEF_STR_POOL_SIZE),0,
			ZL_DEF_STR_POOL_SIZE * sizeof(ZL_CHAR));
	}
	for(i=compile->def_StringPool.count,j=0;
		i<compile->def_StringPool.size && j<len;i++,j++)
	{
		compile->def_StringPool.ptr[i] = src[j];
	}
	if(i >= compile->def_StringPool.size)
		compile->exit(VM_ARG,ZL_ERR_CP_DEF_STR_POOL_I_OUT_OF_BOUND);
	else
		compile->def_StringPool.ptr[i] = ZL_STRNULL;
	i = compile->def_StringPool.count;
	compile->def_StringPool.count += len +1;
	return i;
}

/*
def�������ĳ�����ֵ���ַ�����Ϣ�������def_StringPool���ַ������С�
����������ڳ�ʼ��def_StringPool�ַ����ء�
*/
ZL_VOID zengl_initDefStringPool(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	if(compile->def_StringPool.isInit)
		return;
	compile->def_StringPool.count = 2; //ֻ�д��ڵ�������2�Ĳ�����Ч����Ϣ��
	compile->def_StringPool.size = ZL_DEF_STR_POOL_SIZE;
	compile->def_StringPool.ptr = compile->memAlloc(VM_ARG,compile->def_StringPool.size * sizeof(ZL_CHAR));
	if(compile->def_StringPool.ptr == ZL_NULL)
		compile->exit(VM_ARG , ZL_ERR_CP_DEF_STR_POOL_MALLOC_FAILED);
	ZENGL_SYS_MEM_SET(compile->def_StringPool.ptr, 0 , compile->def_StringPool.size * sizeof(ZL_CHAR));
	compile->def_StringPool.isInit = ZL_TRUE;
}

/*
��def����ĺ����ͺ��Ӧ��ֵ���뵽��̬�����У�ͬʱ���������뵽��ϣ���С�
�Ժ�Ϳ���ͨ����ϣ����������������������Ҷ�̬�������Ԫ�ء�
*/
ZL_VOID zengl_insert_HashTableForDef(ZL_VOID * VM_ARG , ZL_INT nameIndex, ZENGL_TOKENTYPE tokentype,ZL_INT valIndex,
									 ZL_INT line,ZL_INT col)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZL_CHAR * name = compile->DefPoolGetPtr(VM_ARG,nameIndex);
	ZL_INT h = compile->hash(VM_ARG,name) + ZL_SYM_HASH_SIZE * ZL_HASH_TYPE_DEF_TABLE;
	ZL_INT tmpindex = compile->HashTable[h];
	while(tmpindex != 0 && compile->def_table.defs[tmpindex].isvalid == ZL_TRUE &&
		ZENGL_SYS_STRCMP(name,compile->DefPoolGetPtr(VM_ARG,compile->def_table.defs[tmpindex].nameIndex)) != 0)
		tmpindex = compile->def_table.defs[tmpindex].next;
	if(tmpindex == 0)
	{
		tmpindex = compile->HashTable[h];
		compile->HashTable[h] = compile->insert_DefTable(VM_ARG,nameIndex,tokentype,valIndex,line,col);
		compile->def_table.defs[compile->HashTable[h]].next = tmpindex;
	}
	else if(compile->def_table.defs[tmpindex].isvalid == ZL_FALSE)
		compile->exit(VM_ARG,ZL_ERR_CP_DEF_TABLE_INVALID_INDEX);
	else
	{
		compile->exit(VM_ARG,ZL_ERR_CP_DEF_TABLE_SAME_DEF_FOUND,
			name,
			compile->LineCols.lines[compile->def_table.defs[tmpindex].linecols].lineno,
			compile->LineCols.lines[compile->def_table.defs[tmpindex].linecols].colno,
			compile->LineCols.lines[compile->def_table.defs[tmpindex].linecols].filename,
			line,col,compile->source.filename,
			name
			);
	}
}

/*
��������ֵ���ַ������в��Һ궨���ַ�����ָ����Ϣ
*/
ZL_CHAR * zengl_DefPoolGetPtr(ZL_VOID * VM_ARG , ZL_INT index)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	if(index < 2 || index >  compile->def_StringPool.count - 1)
		return ZL_NULL;
	return compile->def_StringPool.ptr + index;
}

/*
��ÿ��def����ĺ����ƺͺ��Ӧ��ֵ���뵽def_table��̬�����С�
*/
ZL_INT zengl_insert_DefTable(ZL_VOID * VM_ARG,ZL_INT nameIndex,ZENGL_TOKENTYPE tokentype,ZL_INT valIndex,ZL_INT line,ZL_INT col)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZL_INT tmpindex;
	if(!compile->def_table.isInit)
		compile->initDefTable(VM_ARG);
	if(compile->def_table.count == compile->def_table.size)
	{
		compile->def_table.size += ZL_DEF_TABLE_SIZE;
		compile->def_table.defs = compile->memReAlloc(VM_ARG,compile->def_table.defs,
			compile->def_table.size * sizeof(ZENGL_DEF_TABLE_MEMBER));
		ZENGL_SYS_MEM_SET(compile->def_table.defs + (compile->def_table.size - ZL_DEF_TABLE_SIZE),0,
			ZL_DEF_TABLE_SIZE * sizeof(ZENGL_DEF_TABLE_MEMBER));
	}
	tmpindex = compile->def_table.count;
	if(compile->def_table.defs[tmpindex].isvalid == ZL_FALSE)
	{
		compile->def_table.defs[tmpindex].nameIndex = nameIndex; //��ŵ����ַ����ص�������Ϣ����ֹ�ַ����������ݶ��ı�ָ���ַ��
		compile->def_table.defs[tmpindex].tokentype = tokentype;
		compile->def_table.defs[tmpindex].valIndex = valIndex;
		compile->def_table.defs[tmpindex].isvalid = ZL_TRUE;
		compile->def_table.defs[tmpindex].linecols = compile->insert_LineCol(VM_ARG,line,col,compile->source.filename); //���к���Ϣ��������ļ�����Ϣ��
		compile->def_table.count++;
		return tmpindex;
	}
	else
		compile->exit(VM_ARG,ZL_ERR_CP_DEF_TABLE_NO_NONVALID_INDEX);
	return -1;
}

/*
��ʼ��def_table��def�궨��Ķ�̬���飩
*/
ZL_VOID zengl_initDefTable(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	if(compile->def_table.isInit)
		return;
	compile->def_table.count = 1; //ֻ�д���0�Ĳ�����ЧԪ�ء�
	compile->def_table.size += ZL_DEF_TABLE_SIZE;
	compile->def_table.defs = compile->memAlloc(VM_ARG,compile->def_table.size * sizeof(ZENGL_DEF_TABLE_MEMBER));
	if(compile->def_table.defs == ZL_NULL)
		compile->exit(VM_ARG,ZL_ERR_CP_DEF_TABLE_MALLOC_FAILED);
	ZENGL_SYS_MEM_SET(compile->def_table.defs,0,compile->def_table.size * sizeof(ZENGL_DEF_TABLE_MEMBER));
	compile->def_table.isInit = ZL_TRUE;
}

/*
���ʷ�ɨ����ɨ�赽inc�ؼ���ʱ���ͻ���øú����������ļ���
�ȵ���push_FileStack��ԭ�����ļ���Ϣѹ��ջ��
Ȼ��compile�е�source(��ǰɨ����ļ�)����Ϣ��inc����ļ���Ϣ�滻����
�����ʷ�ɨ�����ͻ��Ҫ���ص��ļ�����ɨ���ˡ�
*/
ZL_VOID zengl_include_file(ZL_VOID * VM_ARG)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_COMPILE_TYPE * compile = &VM->compile;
	ZENGL_TOKENTYPE token;
	ZL_CHAR * newfilename;
	compile->freeTokenStr(VM_ARG);
	token = compile->getToken(VM_ARG);
	if(token == ZL_TK_STR)
	{
		newfilename = compile->makePathFileName(VM_ARG,compile->tokenInfo.str);
		compile->freeTokenStr(VM_ARG);
		token = compile->getToken(VM_ARG);
		if(token != ZL_TK_SEMI)
			compile->exit(VM_ARG,ZL_ERR_CP_INC_NO_END_SEMI,
			compile->tokenInfo.start_line,compile->tokenInfo.start_col,compile->tokenInfo.filename);
		compile->push_FileStack(VM_ARG,&compile->source,compile->line_no,compile->col_no, newfilename);
		compile->source.file = ZL_NULL;	 //file�ļ�ָ���ֶ���ΪNULL������ɨ�����ͻ�����ȥ���µ��ļ���
		compile->source.needread = ZL_TRUE;
		compile->source.cur = 0;
		compile->source.encrypt = VM->initEncrypt;
		compile->source.filename = newfilename; //�����µ�ɨ���ļ���
		compile->source.run_str = ZL_NULL; //���ַ����ű�����ΪNULL�������Ͳ���Ӱ�쵽Ҫ���صĽű��ļ��Ľ���
		compile->source.run_str_len = 0;   //�����ַ����ű����ȡ�
		compile->line_no = 1;  //����ɨ������к�Ϊ��1�е�0�С�
		compile->col_no = 0;
	}
	else
		compile->exit(VM_ARG,ZL_ERR_CP_INC_MUST_WITH_STRING,
		compile->tokenInfo.start_line,compile->tokenInfo.start_col,compile->tokenInfo.filename);
} 

/*
inc�����ļ�ʱ���ȵõ���ǰɨ����ļ���·����Ϣ��
�ٽ�·����Ϣ��Ҫ���ص��ļ�������һ�𹹳�Ҫ�����ļ�������·����Ϣ��
*/
ZL_CHAR * zengl_makePathFileName(ZL_VOID * VM_ARG,ZL_CHAR * filename)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZL_INT cpylen=-1;
	ZL_INT i,len = ZENGL_SYS_STRLEN(compile->source.filename);
	for(i=len-1;i>=0;i--)
	{
		if(compile->source.filename[i] == '/')
		{
			cpylen = i+1;
			break;
		}
	}
	len = ZENGL_SYS_STRLEN(filename);
	if(cpylen > 0)
	{
		if(cpylen >= ZL_FILE_MAX_PATH)
			compile->exit(VM_ARG,ZL_ERR_CP_INC_FILENAME_TOO_LONG,compile->source.filename);
		ZENGL_SYS_STRNCPY(compile->PathFileName,compile->source.filename,cpylen);
		if(cpylen + len >= ZL_FILE_MAX_PATH)
			compile->exit(VM_ARG,ZL_ERR_CP_INC_FILENAME_TOO_LONG_WHEN_MERGE,compile->source.filename,filename);
		ZENGL_SYS_STRNCPY(compile->PathFileName + cpylen,filename,len);
		compile->PathFileName[cpylen + len] = ZL_STRNULL;
	}
	else
	{
		if(len >= ZL_FILE_MAX_PATH)
			compile->exit(VM_ARG,ZL_ERR_CP_INC_FILENAME_TOO_LONG,filename);
		ZENGL_SYS_STRNCPY(compile->PathFileName,filename,len);
		compile->PathFileName[len] = ZL_STRNULL;
	}
	len = ZENGL_SYS_STRLEN(compile->PathFileName);
	filename = compile->memAlloc(VM_ARG,len+1);
	ZENGL_SYS_STRNCPY(filename,compile->PathFileName,len);
	filename[len] = ZL_STRNULL;
	return filename;
}

/*
��ԭ�����ļ���Ϣѹ��ջ��
���ʷ�ɨ�赽inc�ؼ���ʱ�������inc�ؼ��ʺ�����ļ���������ļ�����ɨ�裬�Ӷ����
���ع��������ǵ�ɨ������ص��ļ�ʱ�������������ܹ���ԭ��inc���ĺ������ɨ�裬
���Ծ���Ҫ��ջ��inc����ǰ��ԭ�����ļ���Ϣ�������´���Ҫ����ɨ������к���Ϣ��ѹ��
ջ�����������ڼ������ļ����ּ����ϴε��ļ���ɨ�蹤����
���磺file1.zl�ļ�������inc file2.zl;����䣬��ô��inc׼������file2.zl(��׼������
file2.zl����ɨ��)ǰ�������Ƚ�file1.zlѹ��ջ����file2.zlɨ�������
�ٽ�file1.zl��Ϣ����ջ�����Ŵ�file1.zl��inc�������ɨ�衣
*/
ZL_VOID zengl_push_FileStack(ZL_VOID * VM_ARG , ZENGL_SOURCE_TYPE * src,ZL_INT line,ZL_INT col, ZL_CHAR * inc_filename)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_SOURCE_TYPE * dest;
	if(!compile->FileStackList.isInit)
		compile->initFileStack(VM_ARG);
	if(compile->FileStackList.count == compile->FileStackList.size)
	{
		compile->FileStackList.size += ZL_FILE_STACK_SIZE;
		compile->FileStackList.stacks = compile->memReAlloc(VM_ARG,compile->FileStackList.stacks,
			compile->FileStackList.size * sizeof(ZENGL_FILE_STACK_TYPE));
		ZENGL_SYS_MEM_SET(compile->FileStackList.stacks + (compile->FileStackList.size - ZL_FILE_STACK_SIZE),0,
			ZL_FILE_STACK_SIZE * sizeof(ZENGL_FILE_STACK_TYPE));
	}
	if(compile->FileStackList.filenames_count == compile->FileStackList.filenames_size)
	{
		compile->FileStackList.filenames_size += ZL_FILE_STACK_SIZE;
		compile->FileStackList.filenames = compile->memReAlloc(VM_ARG,compile->FileStackList.filenames,
				compile->FileStackList.filenames_size * sizeof(ZL_CHAR *));
		ZENGL_SYS_MEM_SET(compile->FileStackList.filenames + (compile->FileStackList.filenames_size - ZL_FILE_STACK_SIZE), 0,
				ZL_FILE_STACK_SIZE * sizeof(ZL_CHAR *));
	}
	dest = &compile->FileStackList.stacks[compile->FileStackList.count].source;
	ZENGL_SYS_MEM_COPY(dest,src,sizeof(ZENGL_SOURCE_TYPE));
	compile->FileStackList.stacks[compile->FileStackList.count].line = line;
	compile->FileStackList.stacks[compile->FileStackList.count].col = col;
	compile->FileStackList.count++;
	compile->FileStackList.filenames[compile->FileStackList.filenames_count] = inc_filename;
	compile->FileStackList.filenames_count++;
}

/*
���ϴ�ѹ��ջ���ļ���Ϣ�������ӵ������ļ���Ϣ��ָ�ԭ�ļ���ɨ�衣
*/
ZL_VOID zengl_pop_FileStack(ZL_VOID * VM_ARG , ZENGL_SOURCE_TYPE * dest)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_SOURCE_TYPE * src;
	if(compile->FileStackList.count <= 0)
		compile->exit(VM_ARG,ZL_ERR_CP_INC_FILESTACKLIST_OVERFLOW);
	if(dest->file != ZL_NULL)
		ZENGL_SYS_FILE_CLOSE(dest->file);
	src = &compile->FileStackList.stacks[compile->FileStackList.count - 1].source;
	ZENGL_SYS_MEM_COPY(dest,src,sizeof(ZENGL_SOURCE_TYPE));
	compile->line_no = compile->FileStackList.stacks[compile->FileStackList.count - 1].line;
	compile->col_no = compile->FileStackList.stacks[compile->FileStackList.count - 1].col;
	compile->FileStackList.count--;
}

/*
��ʼ��inc�����ļ��Ķ�ջ
*/
ZL_VOID zengl_initFileStack(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	if(compile->FileStackList.isInit)
		return;
	compile->FileStackList.filenames_count = compile->FileStackList.count = 0;
	compile->FileStackList.filenames_size = compile->FileStackList.size = ZL_FILE_STACK_SIZE;
	compile->FileStackList.stacks = compile->memAlloc(VM_ARG,compile->FileStackList.size * sizeof(ZENGL_FILE_STACK_TYPE));
	if(compile->FileStackList.stacks == ZL_NULL)
		compile->exit(VM_ARG,ZL_ERR_CP_INC_FILESTACKLIST_MALLOC_FAILED);
	compile->FileStackList.filenames = compile->memAlloc(VM_ARG,compile->FileStackList.filenames_size * sizeof(ZL_CHAR *));
	if(compile->FileStackList.filenames == ZL_NULL)
		compile->exit(VM_ARG,ZL_ERR_CP_INC_FILESTACKLIST_MALLOC_FAILED);
	ZENGL_SYS_MEM_SET(compile->FileStackList.stacks,0,compile->FileStackList.size * sizeof(ZENGL_FILE_STACK_TYPE));
	ZENGL_SYS_MEM_SET(compile->FileStackList.filenames,0,compile->FileStackList.filenames_size * sizeof(ZL_CHAR *));
	compile->FileStackList.isInit = ZL_TRUE;
}

/*
�ڴʷ�ɨ��ʱ�������id��ʶ��������zengl_ReplaceDefConst����
�ú����ȵ���lookupDefTable�����жϸñ�ʶ���Ƿ���һ���꣬�����һ����
�򽫸ñ�ʶ����Ӧ��token��Ϣ�����滻
���� def NAME 2; myname = NAME; �ڴʷ�ɨ��ʱ�ͻ��Ƚ�NAME��Ϊ�꣬��ɨ�赽
myname = NAME; ʱ�ͻὫNAME�滻Ϊ����2
������Ǻ꣬�򷵻�ԭ����token��Ϣ��
*/
ZENGL_TOKENTYPE zengl_ReplaceDefConst(ZL_VOID * VM_ARG, ZENGL_TOKENTYPE token)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZL_INT index = compile->lookupDefTable(VM_ARG,compile->tokenInfo.str);
	ZL_INT len;
	ZL_CHAR * valstr;
	if(index == 0)
		return token;
	token = compile->def_table.defs[index].tokentype;
	valstr = compile->DefPoolGetPtr(VM_ARG,compile->def_table.defs[index].valIndex);
	len = ZENGL_SYS_STRLEN(valstr);
	if(compile->tokenInfo.size < len + 1)
	{
		while(compile->tokenInfo.size < len + 1)
			compile->tokenInfo.size += ZL_TOKENSTR_SIZE;
		compile->tokenInfo.str = compile->memReAlloc(VM_ARG,compile->tokenInfo.str,compile->tokenInfo.size);
		if(compile->tokenInfo.str == ZL_NULL)
			compile->exit(VM_ARG,ZL_ERR_CP_DEF_TABLE_TK_STR_REALLOC_FAILED_WHEN_REPLACE);
	}
	ZENGL_SYS_STRNCPY(compile->tokenInfo.str,valstr,len);
	compile->tokenInfo.str[len] = ZL_STRNULL;
	compile->tokenInfo.count = len;
	compile->tokenInfo.cur = len;
	return token;
}

/*
���ݲ���name���ڹ�ϣ���в�������������def_table�궨�嶯̬������
��Ԫ�ص�name�ֶν��бȽϣ����һ����˵����name��һ���꣬�ͽ���Ӧ���������ء�
������Ǻ��򷵻�0
*/
ZL_INT zengl_lookupDefTable(ZL_VOID * VM_ARG, ZL_CHAR * name)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZL_INT h;
	ZL_INT tmpindex;
	if(!compile->def_table.isInit)
		compile->initDefTable(VM_ARG);
	h = compile->hash(VM_ARG,name) +  + ZL_SYM_HASH_SIZE * ZL_HASH_TYPE_DEF_TABLE;
	tmpindex = compile->HashTable[h];
	while(tmpindex != 0 && compile->def_table.defs[tmpindex].isvalid == ZL_TRUE &&
		ZENGL_SYS_STRCMP(name,compile->DefPoolGetPtr(VM_ARG,compile->def_table.defs[tmpindex].nameIndex)) != 0)
		tmpindex = compile->def_table.defs[tmpindex].next;
	if(tmpindex == 0)
		return 0;
	else if(compile->def_table.defs[tmpindex].isvalid == ZL_FALSE)
		compile->exit(VM_ARG,ZL_ERR_CP_DEF_TABLE_INVALID_INDEX_WHEN_LOOKUP);
	else
		return tmpindex;
	return 0;
}

/*���ɸ�ʽ����Ϣ�ַ���*/
ZL_CHAR * zengl_makeInfoString(ZL_VOID * VM_ARG,ZENGL_INFO_STRING_TYPE * infoStringPtr , ZL_CONST ZL_CHAR * format , ZENGL_SYS_ARG_LIST arglist)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_SYS_ARG_LIST tmp_arglist;
	ZL_INT retcount = -1;
	ZENGL_SYS_ARG_END(tmp_arglist);
	if(infoStringPtr->str == ZL_NULL)
	{
		infoStringPtr->size = ZL_INFO_STRING_SIZE;
		infoStringPtr->str = compile->memAlloc(VM_ARG,infoStringPtr->size * sizeof(ZL_CHAR));
	}
	do
	{
		// ��64λϵͳ��, GCC��clang�Ὣva_listָ��ĳ������Ľṹ���ýṹ�д洢�˿ɱ����λ����Ϣ��
		// ֱ�ӽ�va_list���ݸ�vsnprintf�Ļ����൱�ڽ�����ṹ��ָ�봫�ݹ�ȥ��vsnprintf����ָ�룬���޸ĸýṹָ��Ĳ���λ�ã���ô��һ���ٴ�ִ��
		// vsnprintfʱ����Ȼva_listָ��û�仯������va_list��Ӧ�Ľṹ��ʵ��ָ��Ŀ�����һ����Ч�Ĳ�������ˣ���Ҫ��ʹ��va_copy����һ��������
		// �ٽ�va_list�Ŀ���(����������ṹ�Ŀ���)���ݸ�vsnprintf��
		// 32λϵͳ������Ҫva_copyҲ����������������Ϊ32λϵͳ�е�va_list��һ���򵥵�ջָ�룬ֱ��ָ���˿ɱ����λ�ã�
		// 32λ�У�va_list���ݸ�vsnprintfʱ�����������ջָ�뿽���ķ�ʽ���ݵģ�����32λϵͳ��û��va_copyҲ��������������
		// ������Ϊ���ô����ܹ���32λ��64λ�ж�������������ͳһʹ��va_copy�ķ�ʽ��������VS2008֮���û��
		// va_copy�Ļ����У��Ὣva_copy����Ϊmemcpy
		ZENGL_SYS_ARG_COPY(tmp_arglist, arglist);
		retcount = ZENGL_SYS_SPRINTF_ARG_NUM((infoStringPtr->str + infoStringPtr->cur),
			(infoStringPtr->size - infoStringPtr->count),format,tmp_arglist);
		ZENGL_SYS_ARG_END(tmp_arglist);
		if(retcount >= 0 && retcount < (infoStringPtr->size - infoStringPtr->count))
		{
			infoStringPtr->count += retcount;
			infoStringPtr->cur = infoStringPtr->count;
			infoStringPtr->str[infoStringPtr->cur] = ZL_STRNULL;
			return infoStringPtr->str;
		}

		infoStringPtr->size += ZL_INFO_STRING_SIZE;
		infoStringPtr->str = compile->memReAlloc(VM_ARG,infoStringPtr->str,infoStringPtr->size * sizeof(ZL_CHAR));
	} while(ZL_TRUE);
	return ZL_NULL;
}

/*�ͷŸ�ʽ����Ϣ�ַ������α�ʹ�С��Ϣ*/
ZL_VOID zengl_freeInfoString(ZL_VOID * VM_ARG,ZENGL_INFO_STRING_TYPE * infoStringPtr)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	infoStringPtr->cur = infoStringPtr->count = 0;
	if(infoStringPtr->size > ZL_INFO_STRING_SIZE * 3) //�������������С3������ô������3��������ռ�ù�����Դ
	{
		infoStringPtr->size = ZL_INFO_STRING_SIZE * 3;
		infoStringPtr->str = compile->memReAlloc(VM_ARG,infoStringPtr->str,infoStringPtr->size * sizeof(ZL_CHAR));
	}
}

/*
�ȵ���zengl_makeInfoString���ɸ�ʽ����Ϣ�ַ������ٵ����û��Զ���ĺ������Ӷ����ַ������ݸ��û�������
���û������п���ִ��һЩ����Ĵ�ӡ���������߽��ַ���������ļ���
*/
ZL_VOID zengl_info(ZL_VOID * VM_ARG , ZL_CONST ZL_CHAR * format , ...)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_SYS_ARG_LIST arg;
	ZENGL_SYS_ARG_START(arg,format);
	if(compile->errorFullString.count > 0 && ((ZENGL_VM_TYPE *)VM_ARG)->isCompileError == ZL_FALSE) //��exit�����˳�ʱ�����������������Ϣʱ��ʹ��errorFullString�������
	{
		compile->makeInfoString(VM_ARG,&compile->errorFullString,format,arg);
	}
	else //����ʹ����ͨ��ʽ�������
	{
		compile->makeInfoString(VM_ARG,&compile->infoFullString,format,arg);
		if(compile->userdef_info != ZL_NULL)
			compile->userdef_info(compile->infoFullString.str,compile->infoFullString.count,VM_ARG);
		compile->freeInfoString(VM_ARG,&compile->infoFullString);
	}
	ZENGL_SYS_ARG_END(arg);
}

/*��������ʼ��*/
ZL_VOID zengl_init(ZL_VOID * VM_ARG,ZL_CHAR * script_file,ZENGL_EXPORT_VM_MAIN_ARGS * vm_main_args)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_COMPILE_TYPE * compile = &VM->compile;
	ZL_INT i, len;
	ZL_CHAR * filename;
	compile->isinCompiling = ZL_TRUE;
	compile->start_time = ZENGL_SYS_TIME_CLOCK();
	if(compile->userdef_info == ZL_NULL)
		compile->userdef_info = (ZL_VM_API_INFO_FUN_HANDLE)vm_main_args->userdef_info;
	if(compile->userdef_compile_error == ZL_NULL)
		compile->userdef_compile_error = (ZL_VM_API_INFO_FUN_HANDLE)vm_main_args->userdef_compile_error;
	if(compile->TokenOperateStringCount == 0) {
		for(i=0;(ZL_LONG)(compile->TokenOperateString[i]) != -1L;i++)
			;
		compile->TokenOperateStringCount = i;
	}
	if(compile->isReUse == ZL_TRUE)
		return;
	len = ZENGL_SYS_STRLEN(script_file);
	filename = compile->memAlloc(VM_ARG,len+1);
	ZENGL_SYS_STRNCPY(filename,script_file,len);
	filename[len] = ZL_STRNULL;
	compile->source.filename = filename;
	compile->source.needread = ZL_TRUE;
	compile->source.encrypt = VM->initEncrypt; //ʹ��������ĳ�ʼ���ܽṹ������ʼ��source��encrypt��Ա
	compile->SymSelfClassTable.cur_class_nodenum = -1;
}

/*��������ں���*/
ZL_INT zengl_main(ZL_VOID * VM_ARG,ZL_CHAR * script_file,ZENGL_EXPORT_VM_MAIN_ARGS * vm_main_args)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_COMPILE_TYPE * compile = &VM->compile;
	ZENGL_RUN_TYPE * run = &VM->run;
	ZENGL_TOKENTYPE token;
	ZL_INT retcode;
	ZL_INT isNeedDebugInfo = ZL_EXP_CP_AF_IN_DEBUG_MODE | ZL_EXP_CP_AF_OUTPUT_DEBUG_INFO;
	
	if(compile->isReUse == ZL_TRUE && run->inst_list.count > 0) //����û������������ã���run�������д����Ѿ�����õ�ָ�������һЩ��Ҫ�ı������
		compile->isReUse = ZL_TRUE;
	else
		compile->isReUse = ZL_FALSE;

	compile->init(VM_ARG,script_file,vm_main_args);
	if((retcode = ZENGL_SYS_JMP_SETJMP(compile->jumpBuffer))==0)
	{
		if(compile->isReUse == ZL_TRUE)
			goto end;

		while(ZL_TRUE)
		{
			token = compile->getToken(VM_ARG);
			if(token == ZL_TK_RESERVE)
			{
				if(compile->tokenInfo.reserve == ZL_RSV_DEF )
				{
					compile->AddDefConst(VM_ARG);
					compile->freeTokenStr(VM_ARG);
					continue;
				}
				else if(compile->tokenInfo.reserve == ZL_RSV_INC)
				{
					compile->include_file(VM_ARG);
					compile->freeTokenStr(VM_ARG);
					continue;
				}
			}
			else if(token == ZL_TK_ERROR)
			{
				compile->exit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_TOKEN,
					compile->line_no,compile->col_no,compile->source.filename,compile->tokenInfo.str);
			}

			if(token != ZL_TK_ENDFILE)
			{
				compile->ASTAddNode(VM_ARG,token);
				compile->freeTokenStr(VM_ARG);
				continue;
			}
			else
			{
				if(compile->FileStackList.count == 0)
					break;
				else if(compile->FileStackList.count > 0) //����ļ���ջ�л���Ԫ�أ�˵����ǰ���ڱ������ļ��У�����ջ�б�����ļ���Ϣ�������ָ�ԭ�ļ���ɨ��
				{
					compile->pop_FileStack(VM_ARG,&compile->source);
					compile->freeTokenStr(VM_ARG);
					continue;
				}
				else
					compile->exit(VM_ARG,ZL_ERR_CP_INC_FILESTACKLIST_INVALID_COUNT);
			}
		}
		compile->KeywordCompleteDetect(VM_ARG);
		compile->buildAST(VM_ARG);		//�齨AST�����﷨��
		compile->buildSymTable(VM_ARG); //�齨���ű�
		compile->buildAsmCode(VM_ARG);	//�齨������
		compile->LDAddrListReplaceAll(VM_ARG); //�����е�α��ַ�滻Ϊ��ʵ�Ļ�����λ�ã��Ӷ�������ӹ���	

		/**
		 * �ڱ�������У���������˽������ĺ���ʱ������buildAsmCode�齨�����룬�������д��������ָ��ʱ
		 * ������������ǲ��������˳��ģ�ֻ�Ὣ������Ϣд�뵽errorFullString�У���ˣ���������⵽
		 * ���󣬾��˳�������������������Ϣ���ݸ��û��Զ��庯��(��������˵Ļ�)
		 */
		if(compile->errorFullString.str != ZL_NULL && compile->errorFullString.count > 0)
		{
			compile->exit(VM_ARG, ZL_ERR_RUN_ERROR_EXIT_WHEN_IN_COMPILE);
		}

end:

		if((VM->vm_main_args->flags & isNeedDebugInfo) == isNeedDebugInfo) //�û��Զ���ĵ���ģʽ��
		{
			compile->printASTnodes(VM_ARG); //��ӡAST�����﷨���Ľڵ���Ϣ
			compile->info(VM_ARG,"\n\n the symbol table:\n");
			compile->SymPrintTables(VM_ARG); //��ӡ���ű�
			run->printInstList(VM_ARG,"\n[zenglrun assemble code]:\n"); //���ý������ĺ����������ʾָ������ڷ�������
		}
		compile->exit(VM_ARG,ZL_NO_ERR_SUCCESS);
	}
	if(retcode == 1)
		return 0;
	else
		return -1;
	return 0;
}

/*RC4ʹ�ó�ʼ��Կ����ʼ��state״̬����*/
ZL_VOID zenglVM_rc4InitState(ZL_VOID * VM_ARG,ZL_CHAR * key,ZL_INT len)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZL_INT i,j = 0,t;
	ZL_BYTE * state = VM->initEncrypt.rc4.state;
	
	for (i=0; i < 256; ++i) //���������Ԫ����0��255��ʼ��
		state[i] = i;
	for (i=0; i < 256; ++i) { //���������Ԫ��˳�����
		j = (j + state[i] + key[i % len]) % 256;
		t = state[i]; 
		state[i] = state[j]; 
		state[j] = t; 
	}
	VM->initEncrypt.rc4.i = VM->initEncrypt.rc4.j = 0; //������ָ���ʼ��Ϊ0
}

/*�������ʼ��*/
ZL_VOID zenglVM_init(ZL_VOID * VM_ARG,ZENGL_EXPORT_VM_MAIN_ARGS * vm_main_args)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	VM->start_time = ZENGL_SYS_TIME_CLOCK();
	VM->compile.totalsize = VM->compile.basesize = sizeof(VM->compile);
	VM->run.totalsize = VM->run.basesize = sizeof(VM->run);
	VM->totalsize = VM->basesize = sizeof(ZENGL_VM_TYPE);
	VM->vm_main_args = vm_main_args;
}
