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

/*编译器退出函数，可以输出相关的出错信息*/
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
	compile->total_time = compile->end_time - compile->start_time; //得到总的编译时间
	if(VM->errorno != ZL_NO_ERR_SUCCESS)
		VM->isCompileError = ZL_TRUE;
	if((VM->vm_main_args->flags & isNeedDebugInfo) == isNeedDebugInfo) //如果在调试模式下且需要输出调试信息，则打印信息，且不释放资源，以供解释器用
	{
		compile->info(VM_ARG,"\n compile time:%.16g s (real compile time: %.16g s , debug print time: %.16g s) totalsize: %.16g Kbyte\n",(ZL_DOUBLE)compile->total_time / CLOCKS_PER_SEC,
			(ZL_DOUBLE)(compile->total_time - compile->total_print_time) / CLOCKS_PER_SEC,
			(ZL_DOUBLE)compile->total_print_time / CLOCKS_PER_SEC,
			(ZL_FLOAT)compile->totalsize / 1024); //for debug
	}

	if((VM->vm_main_args->flags & ZL_EXP_CP_AF_IN_DEBUG_MODE) == 0 && VM->isinApiRun == ZL_FALSE)//非调试模式，则释放资源
	{
		compile->memFreeAll(VM_ARG);
		compile->infoFullString.str = compile->errorFullString.str = ZL_NULL;
		compile->isDestroyed = ZL_TRUE;
	}
	compile->isinCompiling = ZL_FALSE;
	if(VM->errorno == ZL_NO_ERR_SUCCESS)
		ZENGL_SYS_JMP_LONGJMP_TO(compile->jumpBuffer, 1);
	else
		ZENGL_SYS_JMP_LONGJMP_TO(compile->jumpBuffer, -1);
}

/*编译器释放内存池资源*/
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
从文件或字符串脚本中获取下一个字符
*/
ZL_CHAR zengl_getNextchar(ZL_VOID * VM_ARG)
{
	ZL_CHAR ch = ZL_STRNULL;
	ZL_UCHAR tmpch;
	ZL_INT i,j,t;
	ZL_BYTE * state;
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_SOURCE_TYPE * source = &(compile->source);

	if(source->run_str == ZL_NULL) //如果不是字符串脚本，则就是普通的文件脚本
	{
		if(source->file == ZL_NULL)
		{
			source->file = ZENGL_SYS_FILE_OPEN(source->filename,"rb"); //一定要是rb否则二进制加密脚本解析会出错
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
	else //字符串脚本
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
	case ZL_ENC_TYPE_XOR: //XOR普通异或加密方式
		ch = (ZL_CHAR)(tmpch ^ (ZL_UCHAR)source->encrypt.xor.xor_key_str[source->encrypt.xor.xor_key_cur]);
		if((++source->encrypt.xor.xor_key_cur) >= source->encrypt.xor.xor_key_len)
			source->encrypt.xor.xor_key_cur = 0;
		break;
	case ZL_ENC_TYPE_RC4: //RC4加密方式
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
	case ZL_ENC_TYPE_NONE: //没有加密的情况下直接返回字符
	default:
		ch = (ZL_CHAR)tmpch;
		break;
	}
	compile->col_no++;
	return ch;
}

/*获取token信息*/
ZENGL_TOKENTYPE zengl_getToken(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_STATES state = ZL_ST_START; //设置起始状态为START
	ZENGL_TOKENTYPE token = ZL_TK_ID;
	ZL_CHAR startStr_ch;
	ZL_CHAR ch;
	while(state!=ZL_ST_DOWN) //当state状态为ZL_ST_DOWN时，表示找到一个变量或者别的token(一般变量,加减符号,数字等每个扫描出来的元素都称作token)
	{
		ch = compile->getNextchar(VM_ARG); //打开一个文件，并从中读取一个字符
		switch(state)
		{
		case ZL_ST_START:
			compile->makeTokenInfoLineCol(VM_ARG,compile->line_no,compile->col_no,compile->source.filename);
			if(ch==' ' || ch=='\t' || ch=='\n' || ch=='\r') //如果是换行回车之类的就跳过。
			{
				if(ch == '\n')
				{
					compile->line_no++; //回车换行时修改对应的行列号
					compile->col_no=0;
				}
				continue;
			}
			else if(ch >= 0 && ZENGL_SYS_CTYPE_IS_ALPHA(ch)) //判断读取的字符是否是英文字母。
			{
				state = ZL_ST_INID; //如果是字母，我们就将state状态机设置为ZL_ST_INID 。
				compile->makeTokenStr(VM_ARG,ch); //然后将读取出来的ch字符通过函数makeTokenStr加入到tokenInfo的动态字符串里
			}
			else if(ch == '0') //如果是0开头就有可能是十六进制如0x12就是十六进制，也有可能是八进制如0e12就是八进制。还有可能是普通的十进制如0123就是123十进制。
			{
				state = ZL_ST_INEIGHT_HEX;
				compile->makeTokenStr(VM_ARG,ch);
			}
			else if(ch >= 0 && ZENGL_SYS_CTYPE_IS_DIGIT(ch)) //判断读取的字符是否是数字
			{
				state = ZL_ST_INNUM; //如果是数字，我们就将state状态机设置为ZL_ST_INNUM 。
				compile->makeTokenStr(VM_ARG,ch); //然后将读取出来的ch字符通过函数makeTokenStr加入到tokenInfo的动态字符串里
			}
			else if(ch == '\'' || ch == '"') //判断是否是字符串，字符串以单引号或双引号开头
			{
				state = ZL_ST_INSTR;
				startStr_ch = ch;
				if(compile->tokenInfo.str == ZL_NULL) //如果没有初始化，就通过makeTokenStr来初始化，再由freeTokenStr重置游标
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
					state = ZL_ST_INPLUS; //如果字符是‘+’号就将state状态机设为ZL_ST_INPLUS，并进入ZL_ST_INPLUS状态机，因为+可能和赋值等连用，所以还需要进一步判断。
					break;
				case '-':
					state = ZL_ST_INMINIS; //和上面同理
					break;
				case '*':
					state = ZL_ST_INTIMES; //和上面同理
					break;
				case '/':
					state = ZL_ST_INDIVIDE;
					break;
				case '%':
					state = ZL_ST_INMOD; //取余运算符。
					break;
				case '=':
					state = ZL_ST_INASSIGN;
					break;
				case '>':
					state = ZL_ST_INGREAT;  //大于字符进入ZL_ST_INGREAT状态
					break;
				case '<':
					state = ZL_ST_INLESS;  //小于字符进入ZL_ST_INLESS状态
					break;
				case '!':
					state = ZL_ST_INNOT_EQ;  //不等于或取反符号进入ZL_ST_INNOT_EQ状态
					break;
				case '&':
					state = ZL_ST_INAND;  //逻辑且
					break;
				case '|':
					state = ZL_ST_INOR;  //逻辑或
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
					token = ZL_TK_LMBRACKET; //左中括号和下面的右中括号用于数组元素的引用。
					break;
				case ']':
					state = ZL_ST_DOWN;
					token = ZL_TK_RMBRACKET;
					break;
				case ZL_FILE_EOF:
					state = ZL_ST_DOWN; //EOF字符表示读取到了文件的结尾，则返回ZL_TK_ENDFILE的token，在外层main函数的主循环就会结束扫描。
					token = ZL_TK_ENDFILE;
					break;
				case ';':
					state = ZL_ST_DOWN; //分号表示单条语句结束
					token = ZL_TK_SEMI;
					break;
				case ',':
					state = ZL_ST_DOWN;
					token = ZL_TK_COMMA; //逗号运算符
					break;
				case ':':	 //冒号，一般是switch...case结构中。
					state = ZL_ST_DOWN;
					token = ZL_TK_COLON;
					break;
				case '?':	 //问号，主要是... ? ... : ...结构,问号前的表达式为真，则执行问号后的语句，否则执行冒号后的语句。
					state = ZL_ST_DOWN;
					token = ZL_TK_QUESTION_MARK;
					break;
				case '.':	 //点运算符，用于类成员的引用
					state = ZL_ST_DOWN;
					token = ZL_TK_DOT;
					break;
				case '^':	//按位异或运算符
					state = ZL_ST_INXOR;
					break;
				case '~':	//按位取反运算符
					state = ZL_ST_DOWN;
					token = ZL_TK_BIT_REVERSE;
					break;
				default:
					state = ZL_ST_DOWN; //其他情况下表示读取到了未定义的token，那么就返回ZL_TK_ERROR。
					token = ZL_TK_ERROR;
					break;
				}//switch(ch)
				compile->makeTokenStr(VM_ARG,ch);
			}
			break;
		case ZL_ST_INID:
			if(ch >= 0 && (ZENGL_SYS_CTYPE_IS_ALNUM(ch) || ch == '_')) //在ZL_ST_INID状态下，一直读取字符，直到该字符不是字母为止，并将读取的字母通过makeTokenStr构成完整的标示符。标示符还可以包含下划线和数字，但必须是以字母开头！
				compile->makeTokenStr(VM_ARG,ch);
			else
			{
				state = ZL_ST_DOWN;
				token = ZL_TK_ID; //将token设为ZL_TK_ID表示读取到了一个标示符。ZL_TK_ID里的ID即identifier 英文缩写。
				compile->ungetchar(VM_ARG); //因为多读取了一个非字母的字符，所以用ungetchar函数来回退一个字符，供下一次扫描使用。
			}
			break;
		case ZL_ST_INEIGHT_HEX: 
			if(ch == 'e') //如果是0e开头就是8进制，如0e123
			{
				state = ZL_ST_INEIGHT;
				compile->makeTokenStr(VM_ARG,ch);
				break;
			}
			else if(ch == 'x')	//如果是0x开头就是十六进制，如0x123
			{
				state = ZL_ST_INHEX;
				compile->makeTokenStr(VM_ARG,ch);
				break;
			}	//如果既不是0e，也不是0x开头，如0123，则自动进入下面的ZL_ST_INNUM状态
		case ZL_ST_INNUM: //在ZL_ST_INNUM状态下，一直读取字符，直到该字符不是数字为止，并将读取的单个数字通过makeTokenStr构成完整的数值。
			if(ch >= 0 && ZENGL_SYS_CTYPE_IS_DIGIT(ch))
				compile->makeTokenStr(VM_ARG,ch);
			else if(ch == '.')  //如果是小数点，就说明是浮点数，进入ZL_ST_INFLOAT状态
			{
				compile->makeTokenStr(VM_ARG,ch);
				state = ZL_ST_INFLOAT;
			}
			else
			{
				compile->convertStrToDec(VM_ARG); //转为有效的十进制，如0123转换后变为123，开头无效的0会过滤掉。
				state = ZL_ST_DOWN;
				token = ZL_TK_NUM;
				compile->ungetchar(VM_ARG);
			}
			break;
		case ZL_ST_INEIGHT:
			if(ch >= '0' && ch <= '7') //八进制是0到7之间的数。
				compile->makeTokenStr(VM_ARG,ch);
			else if(ch >= '8' && ch <= '9')
				compile->exit(VM_ARG, ZL_ERR_CP_INVALID_OCTAL,
				compile->tokenInfo.start_line,compile->tokenInfo.start_col,compile->tokenInfo.filename);
			else
			{
				compile->convertOctalToDec(VM_ARG); //将八进制转为十进制
				state = ZL_ST_DOWN;
				token = ZL_TK_NUM;
				compile->ungetchar(VM_ARG);
			}
			break;
		case ZL_ST_INHEX:
			if(ch >= 0 && ZENGL_SYS_CTYPE_IS_HEXDIGIT(ch))	//判断是否是十六进制数。
				compile->makeTokenStr(VM_ARG,ch);
			else
			{
				compile->convertHexToDec(VM_ARG); //将十六进制转为十进制。
				state = ZL_ST_DOWN;
				token = ZL_TK_NUM; //将token设为NUM表示读取到了一个数字。
				compile->ungetchar(VM_ARG);
			}
			break;
		case ZL_ST_INFLOAT:  //在ZL_ST_INFLOAT状态下，一直读取字符，直到该字符不是数字为止，这样就可以得到一个浮点数。
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
			if(ch == ZL_FILE_EOF) //如果一直读到文件结尾都没找到结束用的单引号或双引号，则输出语法错误。
			{
				compile->exit(VM_ARG, ZL_ERR_CP_INVALID_STR_NO_ENDCHAR,
					compile->tokenInfo.start_line, compile->tokenInfo.start_col, compile->tokenInfo.filename,
					compile->tokenInfo.str,startStr_ch);
			}
			else if(ch != startStr_ch || compile->is_inConvChr > 0) //如果再次遇到startStr_ch即字符串开始的单引号或双引号，就表示一个字符串结束
			{
				compile->makeTokenStrForString(VM_ARG,ch); //makeTokenStrForString用于处理字符串的token,同时对需要进行转义的字符进行特殊处理。
			}
			else
			{
				state = ZL_ST_DOWN;
				token = ZL_TK_STR;  //将token设为ZL_TK_STR表示读取到了一个字符串
			}
			break;
		case ZL_ST_INASSIGN:  //如果是'=='就是ZL_TK_EQUAL等于运算符，如果只有一个'='就是ZL_TK_ASSIGN赋值运算符
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
		case ZL_ST_INGREAT:  //如果是'>='，token就是ZL_TK_GREAT_EQ大于等于运算符，如果是'>>'则是右移或右移赋值运算符，否则就是ZL_TK_GREAT大于运算符
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
		case ZL_ST_INLESS:  //如果后面还接了等号即'<='，那么token就是ZL_TK_LESS_EQ小于等于运算符，如果是"<<"则是左移或左移赋值运算符，否则就是ZL_TK_LESS小于运算符
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
		case ZL_ST_INNOT_EQ:  //!后面接了'='即'!='则表示token是ZL_TK_NOT_EQ不等于运算符，否则就是'!'即ZL_TK_REVERSE取反运算符
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
			if(ch == '&') //两个&在一起即'&&'表示token是ZL_TK_AND逻辑且运算符。
			{
				compile->makeTokenStr(VM_ARG,ch);
				token = ZL_TK_AND;
			}
			else if(ch == '=')
			{
				compile->makeTokenStr(VM_ARG,ch);
				token = ZL_TK_BIT_AND_ASSIGN; //&=运算符
			}
			else  //只有一个&表示token是变量引用符，也有可能是按位与，放在zengl_parser.c中再进行判断
			{
				token = ZL_TK_ADDRESS; //ZL_TK_ADDRESS是变量引用的token的枚举值。 
				compile->ungetchar(VM_ARG);
			}
			break;
		case ZL_ST_INOR:
			state = ZL_ST_DOWN;
			if(ch == '|') //两个|在一起即'||'表示token是逻辑或运算符
			{
				compile->makeTokenStr(VM_ARG,ch);
				token = ZL_TK_OR;
			}
			else if(ch == '=')
			{
				compile->makeTokenStr(VM_ARG,ch);
				token = ZL_TK_BIT_OR_ASSIGN; //|=运算符
			}
			else //只有一个"|"则表示按位或运算符
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
				token = ZL_TK_PLUS_PLUS; //++运算符
			}
			else if(ch == '=')
			{
				compile->makeTokenStr(VM_ARG,ch);
				token = ZL_TK_PLUS_ASSIGN; //+=运算符
			}
			else
			{
				token = ZL_TK_PLUS; //加法符号
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
			else if(ch == '/') //如果遇到'//' ，说明是注释，则进入ZL_ST_INCOMMENT
			{
				state = ZL_ST_INCOMMENT;
			}
			else if(ch == '*') //如果遇到'/*' ，说明是多行注释，则进入ZL_ST_INMULTI_COMMENT
			{
				state = ZL_ST_INMULTI_COMMENT;
			}
			else
			{
				token = ZL_TK_DIVIDE;
				compile->ungetchar(VM_ARG);
			}
			break;
		case ZL_ST_INCOMMENT:	  //单行注释
			if(ch == '\n') //遇到换行符，则对应修改行列号的全局变量，因为是单行注释，所以将state设为ZL_ST_START，说明注释结束，重头开始解析下一个token
			{
				compile->line_no++;
				compile->col_no=0;
				state = ZL_ST_START;
				compile->freeTokenStr(VM_ARG);
			}
			else if(ch == ZL_FILE_EOF) //如果遇到文件结尾，则将state设为ZL_ST_DOWN，并返回ZL_TK_ENDFILE文件结束的token
			{
				state = ZL_ST_DOWN;
				token = ZL_TK_ENDFILE;
			}
			else
				continue;  //将注释部分continue跳过
			break;
		case ZL_ST_INMULTI_COMMENT: //多行注释
			if(ch == '*')  //多行注释遇到*号时表示多行注释有可能结束。
				state = ZL_ST_INENDMULTI_COMMENT;	 //进入ZL_ST_INENDMULTI_COMMENT状态
			else if(ch == '\n')	//遇到换行符，对应修改行列号的全局变量，并continue跳过
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
			if(ch == '/') //如果是 */ 则说明多行注释结束。
			{
				state = ZL_ST_START;
				compile->freeTokenStr(VM_ARG);
			}
			else if(ch == '*') //在多行注释里的星号直接continue，这样多行注释才不会出错
			{
				continue;
			}
			else if(ch == '\n')	//遇到换行符，对应修改行列号，同时说明前面的*号是普通字符而非结束符，继续进入ZL_ST_INMULTI_COMMENT状态
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
				state = ZL_ST_INMULTI_COMMENT; //如果不是*/则说明是普通的*号字符，继续进入ZL_ST_INMULTI_COMMENT状态
			break;
		case ZL_ST_INMOD:
			state = ZL_ST_DOWN;
			if(ch == '=')
			{
				compile->makeTokenStr(VM_ARG,ch);
				token = ZL_TK_MOD_ASSIGN; //先取余后赋值运算符。
			}
			else
			{
				token = ZL_TK_MOD; //取余运算符。
				compile->ungetchar(VM_ARG);
			}
			break;
		case ZL_ST_INXOR:
			state = ZL_ST_DOWN;
			if(ch == '=')
			{
				compile->makeTokenStr(VM_ARG,ch);
				token = ZL_TK_BIT_XOR_ASSIGN; //^=运算符
			}
			else
			{
				token = ZL_TK_BIT_XOR; //"^"按位异或运算符
				compile->ungetchar(VM_ARG);
			}
			break;
		case ZL_ST_INBIT_RIGHT:
			state = ZL_ST_DOWN;
			if(ch == '=')
			{
				compile->makeTokenStr(VM_ARG,ch);
				token = ZL_TK_BIT_RIGHT_ASSIGN; //">>="右移赋值运算符token
			}
			else
			{
				token = ZL_TK_BIT_RIGHT; //">>"右移运算符token
				compile->ungetchar(VM_ARG);
			}
			break;
		case ZL_ST_INBIT_LEFT:
			state = ZL_ST_DOWN;
			if(ch == '=')
			{
				compile->makeTokenStr(VM_ARG,ch);
				token = ZL_TK_BIT_LEFT_ASSIGN; //"<<="左移赋值运算符token
			}
			else
			{
				token = ZL_TK_BIT_LEFT; //"<<"左移运算符token
				compile->ungetchar(VM_ARG);
			}
			break;
		}//switch(state)
	}//while(state!=ZL_ST_DOWN)
	if(token == ZL_TK_ID)
	{
		token = compile->lookupReserve(VM_ARG,token);

		if(token == ZL_TK_ID && !compile->is_inDefRsv) //def后面的常量名不做解析，其他情况下都可以做解析
		{
			token = compile->ReplaceDefConst(VM_ARG,token);
		}
	}
	return token;
}//ZENGL_TOKENTYPE zengl_getToken(void * VM_ARG)

/*生成token的行列号等信息*/
ZL_VOID zengl_makeTokenInfoLineCol(ZL_VOID * VM_ARG,ZL_INT line,ZL_INT col,ZL_CHAR * filename)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	compile->tokenInfo.start_line = line;
	compile->tokenInfo.start_col = col;
	compile->tokenInfo.filename = filename; //生成token行列号信息时加入文件名信息。
}

/*生成token的字符串信息*/
ZL_VOID zengl_makeTokenStr(ZL_VOID * VM_ARG,ZL_CHAR ch)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	if(compile->tokenInfo.str == ZL_NULL || (compile->tokenInfo.cur >= (compile->tokenInfo.size - 3)))
	{
		compile->tokenInfo.size += ZL_TOKENSTR_SIZE;
		if(compile->tokenInfo.str == ZL_NULL) //为当前扫描token的字符串信息分配内存
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
	if(ch == '\n')  //当字符串中存在换行符号时，也对应的增加行列号，这样就不会出现符号表的行列号与代码文件不符的情况。
	{
		compile->line_no++;
		compile->col_no=0;
	}
}

/*编译器分配内存函数，将编译器相关内存指针统一交给编译器的内存池管理*/
ZL_VOID * zengl_malloc(ZL_VOID * VM_ARG , ZL_INT size)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_COMPILE_TYPE * compile = &VM->compile;
	ZL_INT i = 0;
	ZL_INT increase = 0; //统计内存增量
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
		if(compile->mempool.points[i].point == ZL_NULL) //从内存池中找寻null空指针，找到后为其分配内存，并将指针值返回。
		{
			point = ZENGL_SYS_MEM_ALLOC(size);
			if(point != ZL_NULL)
			{
				compile->mempool.points[i].point = point;
				compile->mempool.points[i].size = size;
				compile->mempool.count++;
				increase += size;
				compile->mempool.totalsize += increase; //增加内存池内存占用量统计数
				compile->totalsize += increase; //增加编译器内存占用量统计数
				VM->totalsize += increase; //增加虚拟机的内存占用量统计数
				return point;
			}
			else
				compile->exit(VM_ARG, ZL_ERR_CP_MP_MALLOC_FAILED);
		}
	}
	compile->exit(VM_ARG, ZL_ERR_CP_MP_MALLOC_NO_NULL_POINT);
	return ZL_NULL;
}

/*调整内存池中某个指针的内存大小*/
ZL_VOID * zengl_realloc(ZL_VOID * VM_ARG , ZL_VOID * point , ZL_INT size)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_COMPILE_TYPE * compile = &VM->compile;
	ZL_INT i;
	ZL_INT increase = 0; //统计内存增量
	if(point == ZL_NULL)
	{
		point = compile->memAlloc(VM_ARG,size);
		return point;
	}
	for(i=0;i<compile->mempool.size;i++)
	{
		if(compile->mempool.points[i].point == point) //在内存池中找到对应的指针，然后通过系统函数调整内存大小。
		{
			point = ZENGL_SYS_MEM_RE_ALLOC(point,size);
			if(point != ZL_NULL)
				compile->mempool.points[i].point = point;
			else
				compile->exit(VM_ARG, ZL_ERR_CP_MP_REALLOC_FAILED);
			increase = size - compile->mempool.points[i].size;
			compile->mempool.points[i].size = size;
			compile->mempool.totalsize += increase; //增加内存池内存占用量统计数
			compile->totalsize += increase; //增加编译器内存占用量统计数
			VM->totalsize += increase; //增加虚拟机的内存占用量统计数
			return point;
		}
	}
	point = compile->memAlloc(VM_ARG,size);
	return point;
}

/*释放编译器内存池中的某个指针*/
ZL_VOID zengl_freeonce(ZL_VOID * VM_ARG,ZL_VOID * point)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_COMPILE_TYPE * compile = &VM->compile;
	ZL_INT i=0;
	ZL_INT increase = 0; //统计内存增量
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
			compile->mempool.totalsize += increase; //增加内存池内存占用量统计数
			compile->totalsize += increase; //增加编译器内存占用量统计数
			VM->totalsize += increase; //增加虚拟机的内存占用量统计数
			break;
		}
	}
	if(i >= compile->mempool.size)
		return;
}

/*将源脚本字符扫描游标回退一格，同时将列号减一*/
ZL_VOID zengl_ungetchar(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_SOURCE_TYPE * source = &(compile->source);
	ZL_INT j_orig , t;
	ZL_BYTE * state;
	source->cur > 0 ? source->cur-- : source->cur;
	switch(source->encrypt.type)
	{
	case ZL_ENC_TYPE_XOR: //XOR普通异或加密游标回滚
		source->encrypt.xor.xor_key_cur > 0 ? source->encrypt.xor.xor_key_cur-- : 
			(source->encrypt.xor.xor_key_cur = source->encrypt.xor.xor_key_len - 1);
		break;
	case ZL_ENC_TYPE_RC4: //RC4状态盒子及i,j指针回滚
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
将字符串转为有效的十进制数，再存放到tokenInfo中 
如0123经过zengl_convertStrToDec转换后就变为有效的十进制数123
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

/*修改游标等成员来释放当前扫描token的动态字符串信息，以供下一次扫描token时使用*/
ZL_VOID zengl_freeTokenStr(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	compile->tokenInfo.cur = 0;  //token全局临时变量的字符串游标重置为0
	compile->tokenInfo.count = 0; //包含的字符串数重置为0
	compile->tokenInfo.reserve = -1;  //token的枚举值重置为-1
}

/*
将当前扫描token由八进制转为十进制
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
将扫描token由十六进制格式转为十进制格式。
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

		mid <<= ((len - i - 1)<<2);	 //(len-i-1)<<2相当于(len-i-1)*4
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
下面的函数是用于专门解析字符串类型的token的
字符串中的斜杠会将单引号，双引号，斜杠，r，n，t，v，a，b，f字符进行转义，其他字符则不转义
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
		compile->line_no++;  //遇到换行符，则修改行列号的值。
		compile->col_no=0;
	}
	if(compile->is_inConvChr == ZL_TRUE)
	{
		compile->is_inConvChr = ZL_FALSE; //转义解析结束，则设为ZL_FALSE，关闭此次转义
	}
}

/*查询关键字的枚举值*/
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
			case ZL_RSV_ENDSWITCH: //endswitch等价于endswt
				compile->tokenInfo.reserve = ZL_RSV_ENDSWT;
				break;
			case ZL_RSV_ENDCLASS: //endclass等价于endcls
				compile->tokenInfo.reserve = ZL_RSV_ENDCLS;
				break;
			default:
				compile->tokenInfo.reserve = i;  //设置关键词的枚举值
				break;
			}
			break;
		}
		i++;
	}
	return token;
}

/*
词法扫描时如果遇到def关键字，就调用zengl_AddDefConst函数将宏和宏对应的值加入到哈希表和
def宏定义的动态数组中。宏定义里的字符串信息都存放在def_StringPool字符串池中。
*/
ZL_VOID zengl_AddDefConst(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_TOKENTYPE token;
	ZL_INT nameIndex, valIndex;
	ZL_INT temp_line,temp_col;
	ZL_INT def_start_line,def_start_col; //def定义的起始行列号，以def关键字的行列号为准
	compile->freeTokenStr(VM_ARG);
	def_start_line = temp_line = compile->tokenInfo.start_line;
	def_start_col = temp_col = compile->tokenInfo.start_col;
	compile->is_inDefRsv = ZL_TRUE;
	token = compile->getToken(VM_ARG);
	compile->is_inDefRsv = ZL_FALSE;
	if(token != ZL_TK_ID)
		compile->exit(VM_ARG,ZL_ERR_CP_DEF_MUST_WITH_ID,
		temp_line,temp_col,compile->tokenInfo.filename);
	nameIndex = compile->DefPoolAddString(VM_ARG,compile->tokenInfo.str);
	compile->freeTokenStr(VM_ARG);
	temp_line = compile->tokenInfo.start_line;
	temp_col = compile->tokenInfo.start_col;
	token = compile->getToken(VM_ARG);
	if(token == ZL_TK_MINIS) //def定义的宏值可以是负数
		token = compile->getToken(VM_ARG);
	if(token == ZL_TK_NUM || token == ZL_TK_FLOAT || token == ZL_TK_STR)
	{
		valIndex =  compile->DefPoolAddString(VM_ARG,compile->tokenInfo.str);
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
将参数src添加到def_StringPool字符串池中。并返回该字符串在池中的索引
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
def定义后面的常量和值的字符串信息都存放在def_StringPool的字符串池中。
这个函数用于初始化def_StringPool字符串池。
*/
ZL_VOID zengl_initDefStringPool(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	if(compile->def_StringPool.isInit)
		return;
	compile->def_StringPool.count = 2; //只有大于等于索引2的才是有效的信息。
	compile->def_StringPool.size = ZL_DEF_STR_POOL_SIZE;
	compile->def_StringPool.ptr = compile->memAlloc(VM_ARG,compile->def_StringPool.size * sizeof(ZL_CHAR));
	if(compile->def_StringPool.ptr == ZL_NULL)
		compile->exit(VM_ARG , ZL_ERR_CP_DEF_STR_POOL_MALLOC_FAILED);
	ZENGL_SYS_MEM_SET(compile->def_StringPool.ptr, 0 , compile->def_StringPool.size * sizeof(ZL_CHAR));
	compile->def_StringPool.isInit = ZL_TRUE;
}

/*
将def定义的宏名和宏对应的值插入到动态数组中，同时将索引加入到哈希表中。
以后就可以通过哈希表查找索引，再由索引查找动态数组里的元素。
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
根据索引值从字符串池中查找宏定义字符串的指针信息
*/
ZL_CHAR * zengl_DefPoolGetPtr(ZL_VOID * VM_ARG , ZL_INT index)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	if(index < 2 || index >  compile->def_StringPool.count - 1)
		return ZL_NULL;
	return compile->def_StringPool.ptr + index;
}

/*
将每条def定义的宏名称和宏对应的值加入到def_table动态数组中。
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
		compile->def_table.defs[tmpindex].nameIndex = nameIndex; //存放的是字符串池的索引信息，防止字符串池因扩容而改变指针地址。
		compile->def_table.defs[tmpindex].tokentype = tokentype;
		compile->def_table.defs[tmpindex].valIndex = valIndex;
		compile->def_table.defs[tmpindex].isvalid = ZL_TRUE;
		compile->def_table.defs[tmpindex].linecols = compile->insert_LineCol(VM_ARG,line,col,compile->source.filename); //行列号信息里加入了文件名信息。
		compile->def_table.count++;
		return tmpindex;
	}
	else
		compile->exit(VM_ARG,ZL_ERR_CP_DEF_TABLE_NO_NONVALID_INDEX);
	return -1;
}

/*
初始化def_table（def宏定义的动态数组）
*/
ZL_VOID zengl_initDefTable(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	if(compile->def_table.isInit)
		return;
	compile->def_table.count = 1; //只有大于0的才是有效元素。
	compile->def_table.size += ZL_DEF_TABLE_SIZE;
	compile->def_table.defs = compile->memAlloc(VM_ARG,compile->def_table.size * sizeof(ZENGL_DEF_TABLE_MEMBER));
	if(compile->def_table.defs == ZL_NULL)
		compile->exit(VM_ARG,ZL_ERR_CP_DEF_TABLE_MALLOC_FAILED);
	ZENGL_SYS_MEM_SET(compile->def_table.defs,0,compile->def_table.size * sizeof(ZENGL_DEF_TABLE_MEMBER));
	compile->def_table.isInit = ZL_TRUE;
}

/*
当词法扫描器扫描到inc关键字时，就会调用该函数来加载文件。
先调用push_FileStack将原来的文件信息压入栈。
然后将compile中的source(当前扫描的文件)的信息用inc后的文件信息替换掉。
这样词法扫描器就会对要加载的文件进行扫描了。
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
		compile->source.file = ZL_NULL;	 //file文件指针字段设为NULL，这样扫描器就会重新去打开新的文件。
		compile->source.needread = ZL_TRUE;
		compile->source.cur = 0;
		compile->source.encrypt = VM->initEncrypt;
		compile->source.filename = newfilename; //设置新的扫描文件。
		compile->source.run_str = ZL_NULL; //将字符串脚本设置为NULL，这样就不会影响到要加载的脚本文件的解析
		compile->source.run_str_len = 0;   //重置字符串脚本长度。
		compile->line_no = 1;  //重置扫描的行列号为第1行第0列。
		compile->col_no = 0;
	}
	else
		compile->exit(VM_ARG,ZL_ERR_CP_INC_MUST_WITH_STRING,
		compile->tokenInfo.start_line,compile->tokenInfo.start_col,compile->tokenInfo.filename);
} 

/*
inc加载文件时，先得到当前扫描的文件的路径信息，
再将路径信息和要加载的文件名合在一起构成要加载文件的完整路径信息。
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
将原来的文件信息压入栈。
当词法扫描到inc关键词时，会根据inc关键词后面的文件名进入该文件进行扫描，从而完成
加载工作，但是当扫描完加载的文件时，编译器必须能够从原来inc语句的后面继续扫描，
所以就需要堆栈将inc加载前的原来的文件信息，包括下次需要继续扫描的行列号信息都压入
栈，这样才能在加载完文件后，又继续上次的文件的扫描工作。
例如：file1.zl文件里有条inc file2.zl;的语句，那么在inc准备加载file2.zl(即准备进入
file2.zl进行扫描)前，必须先将file1.zl压入栈，当file2.zl扫描结束后，
再将file1.zl信息弹出栈，接着从file1.zl的inc语句后继续扫描。
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
将上次压入栈的文件信息弹出，从弹出的文件信息里恢复原文件的扫描。
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
初始化inc加载文件的堆栈
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
在词法扫描时如果遇到id标识符则会调用zengl_ReplaceDefConst函数
该函数先调用lookupDefTable函数判断该标识符是否是一个宏，如果是一个宏
则将该标识符对应的token信息进行替换
例如 def NAME 2; myname = NAME; 在词法扫描时就会先将NAME设为宏，当扫描到
myname = NAME; 时就会将NAME替换为数字2
如果不是宏，则返回原来的token信息。
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
根据参数name先在哈希表中查找索引，并和def_table宏定义动态数组中
的元素的name字段进行比较，如果一致则说明该name是一个宏，就将对应的索引返回。
如果不是宏则返回0
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

/*生成格式化信息字符串*/
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
		// 在64位系统中, GCC和clang会将va_list指向某种特殊的结构，该结构中存储了可变参数位置信息，
		// 直接将va_list传递给vsnprintf的话，相当于将特殊结构的指针传递过去，vsnprintf根据指针，会修改该结构指向的参数位置，那么下一次再次执行
		// vsnprintf时，虽然va_list指针没变化，但是va_list对应的结构，实际指向的可能是一个无效的参数。因此，需要先使用va_copy生成一个拷贝，
		// 再将va_list的拷贝(包含了特殊结构的拷贝)传递给vsnprintf，
		// 32位系统，不需要va_copy也可以正常工作，因为32位系统中的va_list是一个简单的栈指针，直接指向了可变参数位置，
		// 32位中，va_list传递给vsnprintf时，本身就是以栈指针拷贝的方式传递的，所以32位系统中没有va_copy也可以正常工作，
		// 不过，为了让代码能够在32位和64位中都正常运作，就统一使用va_copy的方式来处理，在VS2008之类的没有
		// va_copy的环境中，会将va_copy定义为memcpy
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

/*释放格式化信息字符串的游标和大小信息*/
ZL_VOID zengl_freeInfoString(ZL_VOID * VM_ARG,ZENGL_INFO_STRING_TYPE * infoStringPtr)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	infoStringPtr->cur = infoStringPtr->count = 0;
	if(infoStringPtr->size > ZL_INFO_STRING_SIZE * 3) //如果超过基础大小3倍，那么调整到3倍，以免占用过多资源
	{
		infoStringPtr->size = ZL_INFO_STRING_SIZE * 3;
		infoStringPtr->str = compile->memReAlloc(VM_ARG,infoStringPtr->str,infoStringPtr->size * sizeof(ZL_CHAR));
	}
}

/*
先调用zengl_makeInfoString生成格式化信息字符串，再调用用户自定义的函数，从而将字符串传递给用户函数，
在用户函数中可以执行一些常规的打印操作，或者将字符串输出到文件等
*/
ZL_VOID zengl_info(ZL_VOID * VM_ARG , ZL_CONST ZL_CHAR * format , ...)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_SYS_ARG_LIST arg;
	ZENGL_SYS_ARG_START(arg,format);
	if(compile->errorFullString.count > 0 && ((ZENGL_VM_TYPE *)VM_ARG)->isCompileError == ZL_FALSE) //在exit错误退出时，且正在输出错误信息时，使用errorFullString来输出。
	{
		compile->makeInfoString(VM_ARG,&compile->errorFullString,format,arg);
	}
	else //否则，使用普通方式进行输出
	{
		compile->makeInfoString(VM_ARG,&compile->infoFullString,format,arg);
		if(compile->userdef_info != ZL_NULL)
			compile->userdef_info(compile->infoFullString.str,compile->infoFullString.count,VM_ARG);
		compile->freeInfoString(VM_ARG,&compile->infoFullString);
	}
	ZENGL_SYS_ARG_END(arg);
}

/*编译器初始化*/
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
	compile->source.encrypt = VM->initEncrypt; //使用虚拟机的初始加密结构体来初始化source的encrypt成员
}

/*编译器入口函数*/
ZL_INT zengl_main(ZL_VOID * VM_ARG,ZL_CHAR * script_file,ZENGL_EXPORT_VM_MAIN_ARGS * vm_main_args)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_COMPILE_TYPE * compile = &VM->compile;
	ZENGL_RUN_TYPE * run = &VM->run;
	ZENGL_TOKENTYPE token;
	ZL_INT retcode;
	ZL_INT isNeedDebugInfo = ZL_EXP_CP_AF_IN_DEBUG_MODE | ZL_EXP_CP_AF_OUTPUT_DEBUG_INFO;
	
	if(compile->isReUse == ZL_TRUE && run->inst_list.count > 0) //如果用户设置了重利用，且run解释器中存有已经编译好的指令，则跳过一些主要的编译过程
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
				else if(compile->FileStackList.count > 0) //如果文件堆栈中还有元素，说明当前处于被加载文件中，将堆栈中保存的文件信息弹出，恢复原文件的扫描
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
		compile->buildAST(VM_ARG);		//组建AST抽象语法树
		compile->buildSymTable(VM_ARG); //组建符号表
		compile->buildAsmCode(VM_ARG);	//组建汇编代码
		compile->LDAddrListReplaceAll(VM_ARG); //将所有的伪地址替换为真实的汇编代码位置，从而完成链接工作	

		/**
		 * 在编译过程中，如果调用了解释器的函数时，例如buildAsmCode组建汇编代码，向解释器写入虚拟汇编指令时
		 * 如果发生错误，是不会立即退出的，只会将错误信息写入到errorFullString中，因此，这里，如果检测到
		 * 错误，就退出编译器，并将错误信息传递给用户自定义函数(如果定义了的话)
		 */
		if(compile->errorFullString.str != ZL_NULL && compile->errorFullString.count > 0)
		{
			compile->exit(VM_ARG, ZL_ERR_RUN_ERROR_EXIT_WHEN_IN_COMPILE);
		}

end:

		if((VM->vm_main_args->flags & isNeedDebugInfo) == isNeedDebugInfo) //用户自定义的调试模式下
		{
			compile->printASTnodes(VM_ARG); //打印AST抽象语法树的节点信息
			compile->info(VM_ARG,"\n\n the symbol table:\n");
			compile->SymPrintTables(VM_ARG); //打印符号表
			run->printInstList(VM_ARG,"\n[zenglrun assemble code]:\n"); //调用解释器的函数来输出显示指令集，用于分析调试
		}
		compile->exit(VM_ARG,ZL_NO_ERR_SUCCESS);
	}
	if(retcode == 1)
		return 0;
	else
		return -1;
	return 0;
}

/*RC4使用初始密钥来初始化state状态盒子*/
ZL_VOID zenglVM_rc4InitState(ZL_VOID * VM_ARG,ZL_CHAR * key,ZL_INT len)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZL_INT i,j = 0,t;
	ZL_BYTE * state = VM->initEncrypt.rc4.state;
	
	for (i=0; i < 256; ++i) //将盒子里的元素用0到255初始化
		state[i] = i;
	for (i=0; i < 256; ++i) { //将盒子里的元素顺序打乱
		j = (j + state[i] + key[i % len]) % 256;
		t = state[i]; 
		state[i] = state[j]; 
		state[j] = t; 
	}
	VM->initEncrypt.rc4.i = VM->initEncrypt.rc4.j = 0; //将两个指针初始化为0
}

/*虚拟机初始化*/
ZL_VOID zenglVM_init(ZL_VOID * VM_ARG,ZENGL_EXPORT_VM_MAIN_ARGS * vm_main_args)
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	VM->start_time = ZENGL_SYS_TIME_CLOCK();
	VM->compile.totalsize = VM->compile.basesize = sizeof(VM->compile);
	VM->run.totalsize = VM->run.basesize = sizeof(VM->run);
	VM->totalsize = VM->basesize = sizeof(ZENGL_VM_TYPE);
	VM->vm_main_args = vm_main_args;
}
