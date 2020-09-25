#define ZL_EXP_OS_IN_ZENGLOX //在加载下面的zengl头文件之前，windows系统请定义ZL_EXP_OS_IN_WINDOWS，linux , mac系统请定义ZL_EXP_OS_IN_LINUX
#include "zengl_exportfuns.h"
#include <stdlib.h>

#define STRNULL '\0'
#define DEBUG_INPUT_MAX 50

#define MAIN_INFO_STRING_SIZE 200

typedef struct{
	char str[50];
	int ptrIndex;	//str在内存池中分配的指针的索引
	int size;
	int count;
	int cur;
}ReadStr_Type; //字符串动态数组，用于存放用户从终端输入的信息

typedef struct{
	ZL_EXP_CHAR * function_name;
	ZL_EXP_CHAR * class_name;
	ZL_EXP_CHAR * error_string;
	int default_cmd_action;
}FatalError_Type;

typedef struct _MAIN_INFO_STRING{
	ZL_EXP_CHAR * str;   //字符串指针
	int size;  //字符串的动态大小
	int count; //存放的字符数
	int cur;   //当前游标
} MAIN_INFO_STRING;

FILE * debuglog = NULL;
BOOL need_debuglog = FALSE;
ReadStr_Type ReadStr;
int random_seed=0;

FatalError_Type MainFatalError = {0};

static void main_get_stack_backtrace(ZL_EXP_VOID * VM_ARG, MAIN_INFO_STRING * debug_info);

static void main_free_info_string(ZL_EXP_VOID * VM_ARG, MAIN_INFO_STRING * infoStringPtr);

static ZL_EXP_CHAR * main_fatal_error_copy_string(ZL_EXP_CHAR * from, ZL_EXP_CHAR * to)
{
	int from_len = 0;
	if(to != NULL) {
		free(to);
	}
	from_len = strlen(from);
	if(from_len <= 0)
		return NULL;
	to = malloc(from_len + 1);
	memcpy(to, from, from_len);
	to[from_len] = '\0';
	return to;
}

static void main_fatal_error_set_function_name(ZL_EXP_CHAR * function_name)
{
	MainFatalError.function_name = main_fatal_error_copy_string(function_name, MainFatalError.function_name);
}

static void main_fatal_error_set_class_name(ZL_EXP_CHAR * class_name)
{
	MainFatalError.class_name = main_fatal_error_copy_string(class_name, MainFatalError.class_name);
}

static void main_fatal_error_set_error_string(ZL_EXP_CHAR * error_string)
{
	MainFatalError.error_string = main_fatal_error_copy_string(error_string, MainFatalError.error_string);
}

static int main_fatal_error_callback_exec(ZL_EXP_VOID * VM, ZL_EXP_CHAR * script_file, ZL_EXP_CHAR * fatal_error)
{
	MAIN_INFO_STRING debug_info = {0};
	if(MainFatalError.function_name == NULL) {
		return 0;
	}
	main_get_stack_backtrace(VM, &debug_info);
	zenglApi_ReUse(VM,0);
	zenglApi_Push(VM,ZL_EXP_FAT_STR,fatal_error,0,0);
	zenglApi_Push(VM,ZL_EXP_FAT_STR,debug_info.str,0,0);
	main_free_info_string(VM, &debug_info);
	if(zenglApi_Call(VM, script_file, MainFatalError.function_name, MainFatalError.class_name) == -1) {
		return -1;
	}
	return 0;
}

static void main_make_info_string(ZL_EXP_VOID * VM_ARG, MAIN_INFO_STRING * infoStringPtr, const ZL_EXP_CHAR * format, ...)
{
	va_list arglist;
	int retcount = -1;
	if(infoStringPtr->str == NULL)
	{
		infoStringPtr->size = MAIN_INFO_STRING_SIZE;
		infoStringPtr->str = zenglApi_AllocMem(VM_ARG,infoStringPtr->size * sizeof(ZL_EXP_CHAR));
	}
	do
	{
		va_start(arglist, format);
		retcount = vsnprintf((infoStringPtr->str + infoStringPtr->cur),
							(infoStringPtr->size - infoStringPtr->count), format, arglist);
		va_end(arglist);
		if(retcount >= 0 && retcount < (infoStringPtr->size - infoStringPtr->count))
		{
			infoStringPtr->count += retcount;
			infoStringPtr->cur = infoStringPtr->count;
			infoStringPtr->str[infoStringPtr->cur] = '\0';
			return;
		}

		infoStringPtr->size += MAIN_INFO_STRING_SIZE;
		infoStringPtr->str = zenglApi_ReAllocMem(VM_ARG, infoStringPtr->str, infoStringPtr->size * sizeof(ZL_EXP_CHAR));
	} while(ZL_EXP_TRUE);
}

static void main_free_info_string(ZL_EXP_VOID * VM_ARG, MAIN_INFO_STRING * infoStringPtr)
{
	if(infoStringPtr->str != NULL) {
		zenglApi_FreeMem(VM_ARG, infoStringPtr->str);
		infoStringPtr->str = NULL;
	}
	infoStringPtr->count = infoStringPtr->cur = infoStringPtr->size = 0;
}

static void main_get_stack_backtrace(ZL_EXP_VOID * VM_ARG, MAIN_INFO_STRING * debug_info)
{
	int arg = -1;
	int loc = -1;
	int pc = -1;
	int ret;
	int line = 0;
	ZL_EXP_CHAR * fileName = ZL_EXP_NULL;
	ZL_EXP_CHAR * className = ZL_EXP_NULL;
	ZL_EXP_CHAR * funcName = ZL_EXP_NULL;
	while(ZL_EXP_TRUE)
	{
		ret = zenglApi_DebugGetTrace(VM_ARG,&arg,&loc,&pc,&fileName,&line,&className,&funcName);
		if(ret == 1)
		{
			main_make_info_string(VM_ARG, debug_info, " %s:%d ",fileName,line);
			if(className != ZL_EXP_NULL)
				main_make_info_string(VM_ARG, debug_info, "%s:",className);
			if(funcName != ZL_EXP_NULL)
				main_make_info_string(VM_ARG, debug_info, "%s",funcName);
			main_make_info_string(VM_ARG, debug_info, "\n");
			continue;
		}
		else if(ret == 0)
		{
			main_make_info_string(VM_ARG, debug_info, " %s:%d ",fileName,line);
			if(className != ZL_EXP_NULL)
				main_make_info_string(VM_ARG, debug_info, "%s:",className);
			if(funcName != ZL_EXP_NULL)
				main_make_info_string(VM_ARG, debug_info, "%s",funcName);
			main_make_info_string(VM_ARG, debug_info, "\n");
			break;
		}
		else if(ret == -1)
		{
			main_make_info_string(VM_ARG, debug_info, "%s",zenglApi_GetErrorString(VM_ARG));
			break;
		}
	}
}

ZL_EXP_CHAR * getDebugArg(ZL_EXP_CHAR * str,ZL_EXP_INT * start,ZL_EXP_BOOL needNull)
{
	int i;
	char * ret;
	if((*start) < 0)
	{
		(*start) = -1;
		return ZL_EXP_NULL;
	}
	for(i=(*start);;i++)
	{
		if(str[i] == ' ' || str[i] == '\t')
			continue;
		else if(str[i] == STRNULL)
		{
			(*start) = -1;
			return ZL_EXP_NULL;
		}
		else
		{
			ret = str + i;
			while(++i)
			{
				if(str[i] == ' ' || str[i] == '\t')
				{
					if(needNull != ZL_EXP_FALSE)
						str[i] = STRNULL;
					(*start) = i+1;
					break;
				}
				else if(str[i] == STRNULL)
				{
					(*start) = -1;
					break;
				}
			}
			return ret;
		} //else
	}//for(i=(*start);;i++)
	(*start) = -1;
	return ZL_EXP_NULL;
}

ZL_EXP_VOID main_print_array(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_MEMBLOCK memblock,ZL_EXP_INT recur_count);

ZL_EXP_VOID main_print_debug(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * debug_str)
{
	ZENGL_EXPORT_MOD_FUN_ARG reg_debug;
	ZL_EXP_INT debug_str_len = strlen(debug_str);
	ZL_EXP_BOOL hasSemi = ZL_EXP_FALSE;
	zenglApi_GetDebug(VM_ARG,&reg_debug);
	if(debug_str_len > 0 && debug_str[debug_str_len - 1] == ';')
	{
		hasSemi = ZL_EXP_TRUE;
		debug_str[debug_str_len - 1] = ' ';
	}
	printf("%s:",debug_str);
	switch(reg_debug.type)
	{
	case ZL_EXP_FAT_NONE:
		printf("none type , number equal %ld",reg_debug.val.integer);
		break;
	case ZL_EXP_FAT_INT:
		printf("integer:%ld",reg_debug.val.integer);
		break;
	case ZL_EXP_FAT_FLOAT:
		printf("float:%.16g",reg_debug.val.floatnum);
		break;
	case ZL_EXP_FAT_STR:
		printf("string:%s",reg_debug.val.str);
		break;
	case ZL_EXP_FAT_MEMBLOCK:
		printf("array or class obj:\n");
		main_print_array(VM_ARG,reg_debug.val.memblock,0);
		break;
	case ZL_EXP_FAT_ADDR:
	case ZL_EXP_FAT_ADDR_LOC:
	case ZL_EXP_FAT_ADDR_MEMBLK:
		printf("addr type");
		break;
	case ZL_EXP_FAT_INVALID:
		printf("invalid type");
		break;
	}
	if(hasSemi == ZL_EXP_TRUE)
		debug_str[debug_str_len - 1] = ';';
	printf("\n");
}

ZL_EXP_BOOL main_isNumber(ZL_EXP_CHAR * str)
{
	int len = strlen(str);
	int i;
	for(i=0;i<len;i++)
	{
		if(!isdigit(str[i]))
			return ZL_EXP_FALSE;
	}
	return ZL_EXP_TRUE;
}

ZL_EXP_INT main_debug_break(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * cur_filename,ZL_EXP_INT cur_line,ZL_EXP_INT breakIndex,ZL_EXP_CHAR * log)
{
	char * str = ZL_EXP_NULL;
	char * tmpstr = ZL_EXP_NULL;
	char ch;
	char * command,* arg;
	int i,start;
	int exit = 0;
	int str_size = 0;
	int str_count = 0;
	int tmplen;
	if(log != ZL_EXP_NULL)
	{
		if(zenglApi_Debug(VM_ARG,log) == -1)
		{
			printf("log日志断点错误：%s",zenglApi_GetErrorString(VM_ARG));
		}
		else
		{
			main_print_debug(VM_ARG,log);
			return 0;
		}
	}
	printf("* %s:%d ",cur_filename,cur_line);
	if(breakIndex == -1)
		printf("Single Break [current]\n");
	else
		printf("Break index:%d [current]\n",breakIndex);
	if(str == ZL_EXP_NULL)
	{
		str_size = DEBUG_INPUT_MAX;
		str = zenglApi_AllocMem(VM_ARG,str_size);
	}
	while(!exit)
	{
		printf(">>> debug input:");
		ch = getchar();
		for(i=0;ch!='\n';i++)
		{
			if(i >= str_size - 10) //i到达最后一个元素时，对str进行扩容，str_size - 10让str可以在尾部预留一段空间
			{
				str_size += DEBUG_INPUT_MAX;
				tmpstr = zenglApi_AllocMem(VM_ARG,str_size);
				strcpy(tmpstr,str);
				zenglApi_FreeMem(VM_ARG,str);
				str = tmpstr;
			}
			str[i] = ch;
			ch = getchar();
		}
		str[i] = STRNULL;
		str_count = i;
		start = 0;
		command = getDebugArg(str,&start,ZL_EXP_TRUE);
		if(command == ZL_EXP_NULL || strlen(command) != 1)
		{
			printf("command must be a char\n");
			continue;
		}
		switch(command[0])
		{
		case 'p':
			{
				arg = getDebugArg(str,&start,ZL_EXP_FALSE);
				tmplen = arg != ZL_EXP_NULL ? strlen(arg) : 0;
				if(arg != ZL_EXP_NULL && tmplen > 0)
				{
					if(arg[tmplen - 1] != ';' && str_count < str_size - 1)
					{
						arg[tmplen] = ';';
						arg[tmplen+1] = STRNULL;
					}
					if(zenglApi_Debug(VM_ARG,arg) == -1)
					{
						printf("p debug error：%s\n",zenglApi_GetErrorString(VM_ARG));
						continue;
					}
					main_print_debug(VM_ARG,arg);
				}
				else
					printf("p miss args \n");
			}
			break;
		case 'b':
			{
				char * filename = ZL_EXP_NULL;
				int line = 0;
				int count = 0;
				arg = getDebugArg(str,&start,ZL_EXP_TRUE);
				if(arg != ZL_EXP_NULL && strlen(arg) > 0)
					filename = arg;
				else
				{
					printf("b miss file name arg\n");
					continue;
				}
				arg = getDebugArg(str,&start,ZL_EXP_TRUE);
				if(arg != ZL_EXP_NULL && strlen(arg) > 0)
					line = atoi(arg);
				else
				{
					printf("b miss line num arg\n");
					continue;
				}
				arg = getDebugArg(str,&start,ZL_EXP_TRUE);
				if(arg != ZL_EXP_NULL && strlen(arg) > 0)
					count = atoi(arg);
				if(zenglApi_DebugSetBreak(VM_ARG,filename,line,ZL_EXP_NULL,ZL_EXP_NULL,count,ZL_EXP_FALSE) == -1)
					printf("b debug error:%s\n",zenglApi_GetErrorString(VM_ARG));
				else
					printf("set breakpoint success\n");
			}
			break;
		case 'B':
			{
				int size;
				int totalcount;
				int i;
				char * filename = ZL_EXP_NULL;
				char * condition = ZL_EXP_NULL;
				char * log = ZL_EXP_NULL;
				int count;
				int line;
				ZL_EXP_BOOL disabled;
				if(breakIndex == -1)
					printf("* %s:%d Single Break [current]\n",cur_filename,cur_line);
				size = zenglApi_DebugGetBreak(VM_ARG,-1,ZL_EXP_NULL,ZL_EXP_NULL,ZL_EXP_NULL,ZL_EXP_NULL,&totalcount,ZL_EXP_NULL,ZL_EXP_NULL);
				for(i=0;i<size;i++)
				{
					if(zenglApi_DebugGetBreak(VM_ARG,i,&filename,&line,&condition,&log,&count,&disabled,ZL_EXP_NULL) == -1)
						continue;
					else
					{
						printf("[%d] %s:%d",i,filename,line);
						if(condition != ZL_EXP_NULL)
							printf(" C:%s",condition);
						if(log != ZL_EXP_NULL)
							printf(" L:%s",log);
						printf(" N:%d",count);
						if(disabled == ZL_EXP_FALSE)
							printf(" D:enable");
						else
							printf(" D:disable");
						if(i == breakIndex)
							printf(" [current]");
						printf("\n");
					}
				}
				printf("total:%d\n",totalcount);
			}
			break;
		case 'T':
			{	
				int arg = -1;
				int loc = -1;
				int pc = -1;
				int ret;
				int line = 0;
				char * fileName = ZL_EXP_NULL;
				char * className = ZL_EXP_NULL;
				char * funcName = ZL_EXP_NULL;
				while(ZL_EXP_TRUE)
				{
					ret = zenglApi_DebugGetTrace(VM_ARG,&arg,&loc,&pc,&fileName,&line,&className,&funcName);
					if(ret == 1)
					{
						printf(" %s:%d ",fileName,line);
						if(className != ZL_EXP_NULL)
							printf("%s:",className);
						if(funcName != ZL_EXP_NULL)
							printf("%s",funcName);
						printf("\n");
						continue;
					}
					else if(ret == 0)
					{
						printf(" %s:%d ",fileName,line);
						if(className != ZL_EXP_NULL)
							printf("%s:",className);
						if(funcName != ZL_EXP_NULL)
							printf("%s",funcName);
						printf("\n");
						break;
					}
					else if(ret == -1)
					{
						printf("%s",zenglApi_GetErrorString(VM_ARG));
						break;
					}
				}
			}
			break;
		case 'r':
			{
				int arg = -1;
				int loc = -1;
				int pc = -1;
				int tmpPC;
				int ret;
				int size,i;
				int line = 0;
				char * fileName = ZL_EXP_NULL;
				ZL_EXP_BOOL hasBreaked = ZL_EXP_FALSE;
				ret = zenglApi_DebugGetTrace(VM_ARG,&arg,&loc,&pc,&fileName,&line,ZL_EXP_NULL,ZL_EXP_NULL);
				if(ret == 1)
				{
					zenglApi_DebugGetTrace(VM_ARG,&arg,&loc,&pc,&fileName,&line,ZL_EXP_NULL,ZL_EXP_NULL);
					pc++;
					size = zenglApi_DebugGetBreak(VM_ARG,-1,ZL_EXP_NULL,ZL_EXP_NULL,ZL_EXP_NULL,ZL_EXP_NULL,ZL_EXP_NULL,ZL_EXP_NULL,ZL_EXP_NULL);
					for(i=0;i<size;i++)
					{
						if(zenglApi_DebugGetBreak(VM_ARG,i,ZL_EXP_NULL,ZL_EXP_NULL,ZL_EXP_NULL,ZL_EXP_NULL,ZL_EXP_NULL,ZL_EXP_NULL,&tmpPC) == -1)
							continue;
						else if(pc == tmpPC)
						{
							hasBreaked = ZL_EXP_TRUE;
							break;
						}
					}
					if(!hasBreaked)
					{
						if(zenglApi_DebugSetBreakEx(VM_ARG,pc,ZL_EXP_NULL,ZL_EXP_NULL,1,ZL_EXP_FALSE) == -1)
							printf("%s",zenglApi_GetErrorString(VM_ARG));
						else
							exit = 1;
					}
					else
						exit = 1;
				}
				else if(ret == 0)
					exit = 1;
				else if(ret == -1)
					printf("%s",zenglApi_GetErrorString(VM_ARG));
			}
			break;
		case 'd':
			{
				int index;
				arg = getDebugArg(str,&start,ZL_EXP_TRUE);
				if(arg != ZL_EXP_NULL && strlen(arg) > 0 && main_isNumber(arg))
					index = atoi(arg);
				else
				{
					printf("d miss breakpoint index arg\n");
					continue;
				}
				if(zenglApi_DebugDelBreak(VM_ARG,index) == -1)
					printf("d debug error: invalid breakpoint index");
				else
					printf("delete breakpoint success");
				printf("\n");
			}
			break;
		case 'D':
			{
				int index;
				char * filename = ZL_EXP_NULL;
				char * condition = ZL_EXP_NULL;
				char * log = ZL_EXP_NULL;
				int count;
				int line;
				ZL_EXP_BOOL disabled;
				arg = getDebugArg(str,&start,ZL_EXP_TRUE);
				if(arg != ZL_EXP_NULL && strlen(arg) > 0 && main_isNumber(arg))
					index = atoi(arg);
				else
				{
					printf("D miss breakpoint index arg\n");
					continue;
				}
				if(zenglApi_DebugGetBreak(VM_ARG,index,&filename,&line,&condition,&log,&count,&disabled,ZL_EXP_NULL) == -1)
				{
					printf("D debug error: invalid breakpoint index\n");
					continue;
				}
				else
				{
					if(zenglApi_DebugSetBreak(VM_ARG,filename,line,condition,log,count,ZL_EXP_TRUE) == -1)
						printf("D disable error:%s",zenglApi_GetErrorString(VM_ARG));
					else
						printf("D disable breakpoint success");
					printf("\n");
				}
			}
			break;
		case 'C':
			{
				int index;
				char * newCondition;
				char * filename = ZL_EXP_NULL;
				char * condition = ZL_EXP_NULL;
				char * log = ZL_EXP_NULL;
				int count;
				int line;
				ZL_EXP_BOOL disabled;
				arg = getDebugArg(str,&start,ZL_EXP_TRUE);
				if(arg != ZL_EXP_NULL && strlen(arg) > 0 && main_isNumber(arg))
					index = atoi(arg);
				else
				{
					printf("C miss breakpoint index arg\n");
					continue;
				}
				arg = getDebugArg(str,&start,ZL_EXP_FALSE);
				tmplen = arg != ZL_EXP_NULL ? strlen(arg) : 0;
				if(arg != ZL_EXP_NULL && tmplen > 0)
				{
					if(arg[tmplen - 1] != ';' && str_count < str_size - 1)
					{
						arg[tmplen] = ';';
						arg[tmplen+1] = STRNULL;
					}
					newCondition = arg;
				}
				else
					newCondition = ZL_EXP_NULL;
				if(zenglApi_DebugGetBreak(VM_ARG,index,&filename,&line,&condition,&log,&count,&disabled,ZL_EXP_NULL) == -1)
				{
					printf("C debug error: invalid breakpoint index\n");
					continue;
				}
				else
				{
					if(zenglApi_DebugSetBreak(VM_ARG,filename,line,newCondition,log,count,disabled) == -1)
						printf("C set condition error:%s",zenglApi_GetErrorString(VM_ARG));
					else
						printf("C set condition breakpoint success");
					printf("\n");
				}
			}
			break;
		case 'L':
			{
				int index;
				char * newLog;
				char * filename = ZL_EXP_NULL;
				char * condition = ZL_EXP_NULL;
				char * log = ZL_EXP_NULL;
				int count;
				int line;
				ZL_EXP_BOOL disabled;
				arg = getDebugArg(str,&start,ZL_EXP_TRUE);
				if(arg != ZL_EXP_NULL && strlen(arg) > 0 && main_isNumber(arg))
					index = atoi(arg);
				else
				{
					printf("L miss breakpoint index arg\n");
					continue;
				}
				arg = getDebugArg(str,&start,ZL_EXP_FALSE);
				tmplen = arg != ZL_EXP_NULL ? strlen(arg) : 0;
				if(arg != ZL_EXP_NULL && tmplen > 0)
				{
					if(arg[tmplen - 1] != ';' && str_count < str_size - 1)
					{
						arg[tmplen] = ';';
						arg[tmplen+1] = STRNULL;
					}
					newLog = arg;
				}
				else
					newLog = ZL_EXP_NULL;
				if(zenglApi_DebugGetBreak(VM_ARG,index,&filename,&line,&condition,&log,&count,&disabled,ZL_EXP_NULL) == -1)
				{
					printf("L debug error: invalid breakpoint index\n");
					continue;
				}
				else
				{
					if(zenglApi_DebugSetBreak(VM_ARG,filename,line,condition,newLog,count,disabled) == -1)
						printf("L set log error:%s",zenglApi_GetErrorString(VM_ARG));
					else
						printf("L set log breakpoint success");
					printf("\n");
				}
			}
			break;
		case 'N':
			{
				int index;
				int newCount;
				char * filename = ZL_EXP_NULL;
				char * condition = ZL_EXP_NULL;
				char * log = ZL_EXP_NULL;
				int count;
				int line;
				ZL_EXP_BOOL disabled;
				arg = getDebugArg(str,&start,ZL_EXP_TRUE);
				if(arg != ZL_EXP_NULL && strlen(arg) > 0 && main_isNumber(arg))
					index = atoi(arg);
				else
				{
					printf("N miss breakpoint index arg\n");
					continue;
				}
				arg = getDebugArg(str,&start,ZL_EXP_TRUE);
				if(arg != ZL_EXP_NULL && strlen(arg) > 0 && main_isNumber(arg))
					newCount = atoi(arg);
				else
				{
					printf("N miss breakpoint count arg\n");
					continue;
				}
				if(zenglApi_DebugGetBreak(VM_ARG,index,&filename,&line,&condition,&log,&count,&disabled,ZL_EXP_NULL) == -1)
				{
					printf("N debug error: invalid breakpoint index\n");
					continue;
				}
				else
				{
					if(zenglApi_DebugSetBreak(VM_ARG,filename,line,condition,log,newCount,disabled) == -1)
						printf("N set count error:%s",zenglApi_GetErrorString(VM_ARG));
					else
						printf("N set breakpoint count success");
					printf("\n");
				}
			}
			break;
		case 's':
			zenglApi_DebugSetSingleBreak(VM_ARG,ZL_EXP_TRUE);
			exit = 1;
			break;
		case 'S':
			zenglApi_DebugSetSingleBreak(VM_ARG,ZL_EXP_FALSE);
			exit = 1;
			break;
		case 'c':
			exit = 1;
			break;
		case 'h':
			printf(" p [print val info] usage:p express\n"
				" b [set breakpoint] usage:b filename lineNumber\n"
				" B [list breakpoints] usage:B\n"
				" T [show backtrace] usage:T\n"
				" d [del breakpoint] usage:d breakIndex\n"
				" D [disable breakpoint] usage:D breakIndex\n"
				" C [set condition breakpoint] usage:C breakIndex condition-express\n"
				" L [set log breakpoint] usage:L breakIndex log-express\n"
				" N [set breakpoint count] usage:N breakIndex count\n"
				" s [step into] usage:s\n"
				" S [step over] usage:S\n"
				" r [execve to return] usage:r\n"
				" c [continue] usage:c\n");
			break;
		default:
			printf("invalid command\n");
			break;
		}
	} //while(!exit)
	if(str != ZL_EXP_NULL)
		zenglApi_FreeMem(VM_ARG,str);
	return 0;
}

ZL_EXP_INT main_debug_conditionError(ZL_EXP_VOID * VM_ARG,ZL_EXP_CHAR * filename,ZL_EXP_INT line,ZL_EXP_INT breakIndex,ZL_EXP_CHAR * error)
{
	char * condition;
	zenglApi_DebugGetBreak(VM_ARG,breakIndex,ZL_EXP_NULL,ZL_EXP_NULL,&condition,ZL_EXP_NULL,ZL_EXP_NULL,ZL_EXP_NULL,ZL_EXP_NULL);
	printf("\n%s [%d] <%d %s> error:%s\n",filename,line,breakIndex,condition,error);
	return 0;
}

/*
	将用户输入的数据写入到ReadStr全局字符串动态数组中。
*/
void makeReadStr(char ch)
{
	if(ReadStr.cur < 45)
	{
		ReadStr.str[ReadStr.cur++] = ch;
		ReadStr.str[ReadStr.cur] = STRNULL;
		ReadStr.count++;
	}
}

/*
	获取完一次用户的输入数据后，需要对ReadStr字符串动态数组的游标和计数器重置为0，这样下次获取输入数据时才能得到正确的数据，不至于受到上一次输入数据的影响。 
*/
void freeReadStr()
{
	ReadStr.cur = 0;
	ReadStr.count = 0;
}

ZL_EXP_INT main_userdef_info(ZL_EXP_CHAR * infoStrPtr, ZL_EXP_INT infoStrCount,ZL_EXP_VOID * VM_ARG)
{
	UNUSED(infoStrCount);
	UNUSED(VM_ARG);
	if(need_debuglog && debuglog != NULL)
		fprintf(debuglog,"%s",infoStrPtr);
	return 0;
}

ZL_EXP_INT main_userdef_compile_error(ZL_EXP_CHAR * infoStrPtr, ZL_EXP_INT infoStrCount,ZL_EXP_VOID * VM_ARG)
{
	UNUSED(infoStrCount);
	UNUSED(VM_ARG);
	printf("%s",infoStrPtr);
	return 0;
}

ZL_EXP_INT main_userdef_run_info(ZL_EXP_CHAR * infoStrPtr, ZL_EXP_INT infoStrCount,ZL_EXP_VOID * VM_ARG)
{
	UNUSED(infoStrCount);
	UNUSED(VM_ARG);
	if(need_debuglog && debuglog != NULL)
		fprintf(debuglog,"%s",infoStrPtr);
	return 0;
}

ZL_EXP_INT main_userdef_run_print(ZL_EXP_CHAR * infoStrPtr, ZL_EXP_INT infoStrCount,ZL_EXP_VOID * VM_ARG)
{
	UNUSED(infoStrCount);
	UNUSED(VM_ARG);
	printf("%s\n",infoStrPtr);
	return 0;
}

ZL_EXP_INT main_userdef_run_error(ZL_EXP_CHAR * infoStrPtr, ZL_EXP_INT infoStrCount,ZL_EXP_VOID * VM_ARG)
{
	UNUSED(infoStrCount);
	UNUSED(VM_ARG);
	printf("%s",infoStrPtr);
	return 0;
}

/*builtin模块函数*/
ZL_EXP_VOID main_builtin_printf(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount)
{
	ZENGL_EXPORT_MOD_FUN_ARG arg = {ZL_EXP_FAT_NONE,{0}};
	ZL_EXP_INT i;
	if(argcount < 1)
		zenglApi_Exit(VM_ARG,"printf函数的参数不可以为空");
	for(i=1;i<=argcount;i++)
	{
		zenglApi_GetFunArg(VM_ARG,i,&arg);
		switch(arg.type)
		{
		case ZL_EXP_FAT_INT:
			printf("%ld",arg.val.integer);
			break;
		case ZL_EXP_FAT_FLOAT:
			printf("%.16g",arg.val.floatnum);
			break;
		case ZL_EXP_FAT_STR:
			printf("%s",arg.val.str);
			break;
		default:
			zenglApi_Exit(VM_ARG,"printf函数第%d个参数类型无效，目前只支持字符串，整数，浮点数类型的参数",i);
			break;
		}
	}
	return;
}

/*read模块函数*/
ZL_EXP_VOID main_builtin_read(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount)
{
	UNUSED(argcount);
	char ch;
	ch = getchar();
	while(ch!='\n')
	{
		makeReadStr(ch);
		ch = getchar();
	}
	if(ReadStr.count == 0)
		zenglApi_SetRetVal(VM_ARG,ZL_EXP_FAT_STR,"",0,0);
	else
		zenglApi_SetRetVal(VM_ARG,ZL_EXP_FAT_STR,ReadStr.str,0,0);
	freeReadStr();
}

/*bltRandom模块函数*/
ZL_EXP_VOID main_builtin_random(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount)
{
	UNUSED(argcount);
	if(random_seed == 0) //第一次使用时间作为随机种子。
	{
		srand((unsigned) timer_get_tick());
		random_seed = rand();
	}
	else //其他时候使用上一次生成的随机数作为随机种子
	{
		srand(random_seed);
		random_seed = rand();
	}
	zenglApi_SetRetVal(VM_ARG,ZL_EXP_FAT_INT,ZL_EXP_NULL,(long)random_seed,0);
}

/*array模块函数，用于创建zengl脚本的动态数组*/
ZL_EXP_VOID main_builtin_array(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount)
{
	ZENGL_EXPORT_MEMBLOCK memblock = {0};
	ZENGL_EXPORT_MOD_FUN_ARG arg = {ZL_EXP_FAT_NONE,{0}};
	ZL_EXP_INT i;
	if(argcount == 0) //如果array函数没带参数，则创建一个默认大小的未初始化的数组
	{
		zenglApi_CreateMemBlock(VM_ARG,&memblock,0);
		zenglApi_SetRetValAsMemBlock(VM_ARG,&memblock);
	}
	else if(argcount >= 1) //如果带了参数则使用参数作为函数的初始值
	{
		zenglApi_CreateMemBlock(VM_ARG,&memblock,0);
		for(i=1;i<=argcount;i++)
		{
			zenglApi_GetFunArg(VM_ARG,i,&arg);
			switch(arg.type)
			{
			case ZL_EXP_FAT_INT:
			case ZL_EXP_FAT_FLOAT:
			case ZL_EXP_FAT_STR:
			case ZL_EXP_FAT_MEMBLOCK:
			case ZL_EXP_FAT_ADDR:
			case ZL_EXP_FAT_ADDR_LOC:
			case ZL_EXP_FAT_ADDR_MEMBLK:
				zenglApi_SetMemBlock(VM_ARG,&memblock,i,&arg);
				break;
			default:
				zenglApi_Exit(VM_ARG,"array函数第%d个参数类型无效",i);
				break;
			}
		}
		zenglApi_SetRetValAsMemBlock(VM_ARG,&memblock);
	}
	else
		zenglApi_Exit(VM_ARG,"array函数异常：参数个数小于0");
}

/*递归打印出数组信息*/
ZL_EXP_VOID main_print_array(ZL_EXP_VOID * VM_ARG,ZENGL_EXPORT_MEMBLOCK memblock,ZL_EXP_INT recur_count)
{
	ZL_EXP_INT size,i,j;
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
				printf("  ");
			break;
		default:
			break;
		}
		switch(mblk_val.type)
		{
		case ZL_EXP_FAT_INT:
			printf("[%d] %ld\n",i-1,mblk_val.val.integer);
			break;
		case ZL_EXP_FAT_FLOAT:
			printf("[%d] %.16g\n",i-1,mblk_val.val.floatnum);
			break;
		case ZL_EXP_FAT_STR:
			printf("[%d] %s\n",i-1,mblk_val.val.str);
			break;
		case ZL_EXP_FAT_MEMBLOCK:
			printf("[%d] <array or class obj type> begin:\n",i-1);
			main_print_array(VM_ARG,mblk_val.val.memblock,recur_count+1);
			printf("[%d] <array or class obj type> end\n",i-1);
			break;
		default:
			break;
		}
	}
}

/*bltPrintArray模块函数，打印数组中的元素*/
ZL_EXP_VOID main_builtin_print_array(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount)
{
	ZENGL_EXPORT_MOD_FUN_ARG arg = {ZL_EXP_FAT_NONE,{0}};
	if(argcount < 1)
		zenglApi_Exit(VM_ARG,"bltPrintArray函数参数不可为空，必须指定一个数组或类对象为参数");
	zenglApi_GetFunArg(VM_ARG,1,&arg);
	if(arg.type != ZL_EXP_FAT_MEMBLOCK)
		zenglApi_Exit(VM_ARG,"bltPrintArray函数的参数不是数组或类对象");
	main_print_array(VM_ARG,arg.val.memblock,0);
}

/*bltTestAddr模块函数(仅供测试)，测试参数引用*/
ZL_EXP_VOID main_builtin_test_addr(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount)
{
	ZENGL_EXPORT_MOD_FUN_ARG arg = {ZL_EXP_FAT_NONE,{0}};
	if(argcount < 2)
		zenglApi_Exit(VM_ARG,"bltTestAddr函数参数不可少于两个");
	zenglApi_GetFunArgInfo(VM_ARG,1,&arg);
	if(arg.type != ZL_EXP_FAT_ADDR && 
		arg.type != ZL_EXP_FAT_ADDR_LOC &&
		arg.type != ZL_EXP_FAT_ADDR_MEMBLK
		)
		zenglApi_Exit(VM_ARG,"第一个参数必须是变量的引用，或数组成员的引用，或者类对象成员的引用");
	zenglApi_GetFunArg(VM_ARG,1,&arg); //获取第一个参数的值，zenglApi_GetFunArg可以递归引用，找到引用的变量的值
	if(arg.type != ZL_EXP_FAT_STR)
		zenglApi_Exit(VM_ARG,"bltTestAddr函数目前只能接受字符串作为参数");
	printf("the value of first arg is %s\n",arg.val.str);
	zenglApi_GetFunArg(VM_ARG,2,&arg); //得到第二个参数，并用该参数的字符串值设置第一个参数引用的变量
	if(arg.type != ZL_EXP_FAT_STR)
		zenglApi_Exit(VM_ARG,"bltTestAddr函数目前只能接受字符串作为参数");
	zenglApi_SetFunArg(VM_ARG,1,&arg);
}

/*bltSetArray模块函数，使用第2个，第3个等参数来设置第一个参数对应的数组中的元素*/
ZL_EXP_VOID main_builtin_set_array(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount)
{
	ZENGL_EXPORT_MOD_FUN_ARG arg = {ZL_EXP_FAT_NONE,{0}};
	ZENGL_EXPORT_MEMBLOCK memblock = {0};
	ZL_EXP_INT i;
	if(argcount < 2)
		zenglApi_Exit(VM_ARG,"bltSetArray函数参数不可少于两个");
	zenglApi_GetFunArg(VM_ARG,1,&arg);
	if(arg.type != ZL_EXP_FAT_MEMBLOCK)
		zenglApi_Exit(VM_ARG,"bltSetArray函数第一个参数必须是数组或类对象或这两者的引用类型");
	memblock = arg.val.memblock;
	for(i=2;i<=argcount;i++)
	{
		zenglApi_GetFunArg(VM_ARG,i,&arg);
		zenglApi_SetMemBlock(VM_ARG,&memblock,i-1,&arg);
	}
}

ZL_EXP_VOID main_userdef_module_init(ZL_EXP_VOID * VM_ARG);

/*bltLoadScript模块函数，新建一个虚拟机，加载并执行某个脚本*/
ZL_EXP_VOID main_builtin_load_script(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount)
{
	UNUSED(argcount);
	ZENGL_EXPORT_MOD_FUN_ARG arg = {ZL_EXP_FAT_NONE,{0}};
	ZL_EXP_CHAR * scriptName = ZL_EXP_NULL;
	ZENGL_EXPORT_VM_MAIN_ARGS vm_main_args = {main_userdef_info , 
					  main_userdef_compile_error,
					  main_userdef_run_info,
					  main_userdef_run_print,
					  main_userdef_run_error,
					  main_userdef_module_init,
					  ZL_EXP_CP_AF_IN_DEBUG_MODE | 
					  ZL_EXP_CP_AF_OUTPUT_DEBUG_INFO};
	if(!need_debuglog)
		vm_main_args.flags = ZL_EXP_CP_AF_IN_DEBUG_MODE;

	zenglApi_GetFunArg(VM_ARG,1,&arg); //获取第一个参数为脚本名
	if(arg.type != ZL_EXP_FAT_STR)
		zenglApi_Exit(VM_ARG,"bltLoadScript函数第一个参数必须字符串，代表要加载的脚本文件名");
	scriptName = arg.val.str;
	zenglApi_Load(scriptName,&vm_main_args); 
}

/*bltGetZLVersion模块函数，获取当前zengl版本号信息的字符串形式*/
ZL_EXP_VOID main_builtin_get_zl_version(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount)
{
	UNUSED(argcount);
	ZL_EXP_CHAR version[20] = {0};
	sprintf(version,"%d.%d.%d",ZL_EXP_MAJOR_VERSION,ZL_EXP_MINOR_VERSION,ZL_EXP_REVISION);
	zenglApi_SetRetVal(VM_ARG,ZL_EXP_FAT_STR,version,0,0);
}

/*bltGetExtraData模块函数，获取用户额外数据*/
ZL_EXP_VOID main_builtin_get_extraData(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount)
{
	ZENGL_EXPORT_MOD_FUN_ARG arg = {ZL_EXP_FAT_NONE,{0}};
	ZL_EXP_CHAR * extraName;
	if(argcount != 1)
		zenglApi_Exit(VM_ARG,"bltGetExtraData函数必须有一个参数");
	zenglApi_GetFunArg(VM_ARG,1,&arg); //获取第一个参数为脚本名
	if(arg.type != ZL_EXP_FAT_STR)
		zenglApi_Exit(VM_ARG,"bltGetExtraData函数第一个参数必须是字符串，表示额外数据名称");
	extraName = arg.val.str;
	zenglApi_SetRetVal(VM_ARG,ZL_EXP_FAT_STR,(ZL_EXP_CHAR *)zenglApi_GetExtraData(VM_ARG,extraName),0,0);
}

ZL_EXP_VOID main_builtin_fatal_error_callback(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount)
{
	ZENGL_EXPORT_MOD_FUN_ARG arg = {ZL_EXP_FAT_NONE,{0}};
	const ZL_EXP_CHAR * func_name = "bltFatalErrorCallback";
	ZL_EXP_CHAR * function_name = NULL;
	ZL_EXP_CHAR * class_name = NULL;
	if(argcount < 1)
		zenglApi_Exit(VM_ARG,"usage: %s(function_name[, class_name[, default_cmd_action]])", func_name);
	zenglApi_GetFunArg(VM_ARG,1,&arg);
	if(arg.type != ZL_EXP_FAT_STR) {
		zenglApi_Exit(VM_ARG,"the first argument [function_name] of %s must be string", func_name);
	}
	function_name = (ZL_EXP_CHAR *)arg.val.str;
	if(strlen(function_name) == 0) {
		zenglApi_Exit(VM_ARG,"the first argument [function_name] of %s can't be empty", func_name);
	}
	main_fatal_error_set_function_name(function_name);
	if(argcount > 1) {
		zenglApi_GetFunArg(VM_ARG,2,&arg);
		if(arg.type != ZL_EXP_FAT_STR) {
			zenglApi_Exit(VM_ARG,"the second argument [class_name] of %s must be string", func_name);
		}
		class_name = (ZL_EXP_CHAR *)arg.val.str;
		if(strlen(class_name) > 0) {
			main_fatal_error_set_class_name(class_name);
		}
		if(argcount > 2) {
			zenglApi_GetFunArg(VM_ARG,3,&arg);
			if(arg.type != ZL_EXP_FAT_INT) {
				zenglApi_Exit(VM_ARG,"the third argument [default_cmd_action] of %s must be integer", func_name);
			}
			MainFatalError.default_cmd_action = (int)arg.val.integer;
		}
	}
	zenglApi_SetRetVal(VM_ARG, ZL_EXP_FAT_INT, ZL_EXP_NULL, 0, 0);
}

/*debug调试模块函数*/
ZL_EXP_VOID main_builtin_debug(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount)
{
	ZENGL_EXPORT_MOD_FUN_ARG arg = {ZL_EXP_FAT_NONE,{0}};
	ZL_EXP_CHAR * debug_str;
	if(argcount != 1)
		zenglApi_Exit(VM_ARG,"debug函数必须有一个参数");
	zenglApi_GetFunArg(VM_ARG,1,&arg); //获取第一个参数为脚本名
	if(arg.type != ZL_EXP_FAT_STR)
		zenglApi_Exit(VM_ARG,"debug函数第一个参数必须是字符串，表示调试字符串");
	debug_str = arg.val.str;
	if(zenglApi_Debug(VM_ARG,debug_str) == -1)
	{
		zenglApi_Exit(VM_ARG,"%s",zenglApi_GetErrorString(VM_ARG));
	}
	zenglApi_SetRetVal(VM_ARG,ZL_EXP_FAT_INT,ZL_EXP_NULL,0,0);
}

/*sdl模块函数*/
ZL_EXP_VOID main_sdl_init(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount)
{
	char * dest_str = ZL_EXP_NULL;
	char * src_str = "hello world test MemForString API\n";
	printf("sdl init argcount is %d\n",argcount);
	dest_str = zenglApi_AllocMemForString(VM_ARG,src_str);
	printf("%s",dest_str);
	return;
}

ZL_EXP_VOID main_builtin_module_init(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT moduleID)
{
	zenglApi_SetModFunHandle(VM_ARG,moduleID,"printf",main_builtin_printf);
	zenglApi_SetModFunHandle(VM_ARG,moduleID,"read",main_builtin_read);
	//zenglApi_SetModFunHandle(VM_ARG,moduleID,"bltRandom",main_builtin_random); //使用main.c中定义的bltRandom
	zenglApi_SetModFunHandle(VM_ARG,moduleID,"bltRandom",zenglApiBMF_bltRandom); //使用虚拟机zenglApi_BltModFuns.c中定义的bltRandom
	//zenglApi_SetModFunHandle(VM_ARG,moduleID,"array",main_builtin_array); //使用main.c中定义的array
	zenglApi_SetModFunHandle(VM_ARG,moduleID,"array",zenglApiBMF_array);  //使用虚拟机zenglApi_BltModFuns.c中定义的array
	zenglApi_SetModFunHandle(VM_ARG,moduleID,"unset",zenglApiBMF_unset);  //使用虚拟机zenglApi_BltModFuns.c中定义的unset
	zenglApi_SetModFunHandle(VM_ARG,moduleID,"bltExit",zenglApiBMF_bltExit);  //使用虚拟机zenglApi_BltModFuns.c中定义的bltExit
	zenglApi_SetModFunHandle(VM_ARG,moduleID,"bltConvToInt",zenglApiBMF_bltConvToInt);  //使用虚拟机zenglApi_BltModFuns.c中定义的bltConvToInt
	zenglApi_SetModFunHandle(VM_ARG,moduleID,"bltIntToStr",zenglApiBMF_bltIntToStr);  //使用虚拟机zenglApi_BltModFuns.c中定义的bltIntToStr
	zenglApi_SetModFunHandle(VM_ARG,moduleID,"bltPrintArray",main_builtin_print_array);
	zenglApi_SetModFunHandle(VM_ARG,moduleID,"bltTestAddr",main_builtin_test_addr);
	zenglApi_SetModFunHandle(VM_ARG,moduleID,"bltSetArray",main_builtin_set_array);
	zenglApi_SetModFunHandle(VM_ARG,moduleID,"bltLoadScript",main_builtin_load_script);
	zenglApi_SetModFunHandle(VM_ARG,moduleID,"bltGetZLVersion",main_builtin_get_zl_version);
	zenglApi_SetModFunHandle(VM_ARG,moduleID,"bltGetExtraData",main_builtin_get_extraData);
	zenglApi_SetModFunHandle(VM_ARG,moduleID,"bltFatalErrorCallback",main_builtin_fatal_error_callback);
	zenglApi_SetModFunHandle(VM_ARG,moduleID,"debug",main_builtin_debug);
}

ZL_EXP_VOID main_sdl_module_init(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT moduleID)
{
	zenglApi_SetModFunHandle(VM_ARG,moduleID,"sdlInit",main_sdl_init);
}

ZL_EXP_VOID main_userdef_module_init(ZL_EXP_VOID * VM_ARG)
{
	zenglApi_SetModInitHandle(VM_ARG,"builtin",main_builtin_module_init);
	zenglApi_SetModInitHandle(VM_ARG,"sdl",main_sdl_module_init);
}

void main_exit(void * VM,char * err_format,...)
{
	va_list arg;
	va_start(arg,err_format);
	vprintf(err_format,arg);
	va_end(arg);
	zenglApi_Close(VM);
	fclose(debuglog);
	#ifdef ZL_EXP_OS_IN_WINDOWS
		system("pause");
	#endif
	exit(-1);
}

 /* 
    函数名：getFileSize(char * strFileName)  
    功能：获取指定文件的大小 
    参数： 
        strFileName (char *)：文件名 
    返回值： 
        size (int)：文件大小 
 */  
int getFileSize(char * strFileName)   
{  
    FILE * fp = fopen(strFileName, "r"); 
	int size;
    fseek(fp, 0L, SEEK_END);
    size = ftell(fp);  
    fclose(fp);  
    return size;  
}

int main_output_xor_source(char * src_filename,char * dest_filename,char * xor_key_str)
{
	FILE * src_pt = ZL_EXP_NULL ,* dest_pt = ZL_EXP_NULL;
	int src_filesize = 0;
	unsigned char buf[1024],enc_buf[1024],ch,enc_ch;
	int buf_len = 0,totalwrite_len = 0;
	int prev_percent = 0,cur_percent = 0;
	int cur = 0;
	int xor_key_cur = 0;
	int xor_key_len;

	if(xor_key_str == ZL_EXP_NULL)
	{
		printf("xor_key_str密钥不能为空字符串指针\n");
		return -1;
	}
	xor_key_len = strlen(xor_key_str);
	src_filesize = getFileSize(src_filename);
	src_pt = fopen(src_filename,"rb");
	if(src_pt == ZL_EXP_NULL)
	{
		printf("xor加密的源脚本文件无法打开\n");
		return -1;
	}
	dest_pt = fopen(dest_filename,"wb+");
	if(dest_pt == ZL_EXP_NULL)
	{
		printf("xor加密的目标脚本文件无法打开或无法创建\n");
		return -1;
	}
	while((buf_len = fread(buf,sizeof(unsigned char),1024,src_pt)) != 0)
	{
		for(cur = 0;cur < buf_len;cur++)
		{
			ch = buf[cur];
			enc_ch = ch ^ (unsigned char)xor_key_str[xor_key_cur];
			if((++xor_key_cur) >= xor_key_len)
				xor_key_cur = 0;
			enc_buf[cur] = enc_ch;
		}
		fwrite(enc_buf,sizeof(unsigned char),buf_len,dest_pt);
		totalwrite_len += buf_len;
		cur_percent = (int)(((float)totalwrite_len / (float)src_filesize) * 100);
		if(cur_percent != prev_percent)
			printf("%d%%...",cur_percent);
	}
	printf(" xor '%s' to '%s' is ok!\n",src_filename,dest_filename);
	fclose(src_pt);
	fclose(dest_pt);
	return 0;
}

/*rc4初始化状态盒子*/
void rc4_init(unsigned char * state, unsigned char * key, int len)
{
   int i,j=0,t; 
   
   for (i=0; i < 256; ++i) //将盒子里的元素用0到255初始化
      state[i] = i; 
   for (i=0; i < 256; ++i) { //将盒子里的元素顺序打乱
      j = (j + state[i] + key[i % len]) % 256; 
      t = state[i]; 
      state[i] = state[j]; 
      state[j] = t; 
   }   
}

int main_output_rc4_source(char * src_filename,char * dest_filename,char * rc4_key_str)
{
	FILE * src_pt = ZL_EXP_NULL ,* dest_pt = ZL_EXP_NULL;
	int src_filesize = 0;
	unsigned char buf[1024],enc_buf[1024];
	int buf_len = 0,totalwrite_len = 0;
	int prev_percent = 0,cur_percent = 0;
	int cur = 0;
	unsigned char state[256]; //rc4用于生成密钥流的状态盒子
	int i,j,t;
	int rc4_key_len;

	if(rc4_key_str == ZL_EXP_NULL)
	{
		printf("rc4_key_str密钥不能为空字符串指针\n");
		return -1;
	}
	rc4_key_len = strlen(rc4_key_str);
	if(rc4_key_len > 256)
	{
		printf("rc4_key_str密钥长度不要超过256个字节\n");
		return -1;
	}
	src_filesize = getFileSize(src_filename);
	src_pt = fopen(src_filename,"rb");
	if(src_pt == ZL_EXP_NULL)
	{
		printf("rc4加密的源脚本文件无法打开\n");
		return -1;
	}
	dest_pt = fopen(dest_filename,"wb+");
	if(dest_pt == ZL_EXP_NULL)
	{
		printf("rc4加密的目标脚本文件无法打开或无法创建\n");
		return -1;
	}
	rc4_init(state,(unsigned char *)rc4_key_str,rc4_key_len);
	i = 0;
	j = 0;
	while((buf_len = fread(buf,sizeof(unsigned char),1024,src_pt)) != 0)
	{
		for(cur = 0;cur < buf_len;cur++)
		{
			i = (i + 1) % 256;
			j = (j + state[i]) % 256;
			t = state[i];
			state[i] = state[j]; 
			state[j] = t;
			enc_buf[cur] = state[(state[i] + state[j]) % 256] ^ buf[cur];
		}
		fwrite(enc_buf,sizeof(unsigned char),buf_len,dest_pt);
		totalwrite_len += buf_len;
		cur_percent = (int)(((float)totalwrite_len / (float)src_filesize) * 100);
		if(cur_percent != prev_percent)
			printf("%d%%...",cur_percent);
	}
	printf(" rc4 '%s' to '%s' is ok!\n",src_filename,dest_filename);
	fclose(src_pt);
	fclose(dest_pt);
	return 0;
}

/**
	用户程序执行入口。
*/
int main(VOID * task, int argc, char * argv[])
{
	ZL_EXP_INT builtinID;
	ZL_EXP_VOID * VM;

	MainFatalError.default_cmd_action = 1;

	if(argc < 2)
	{
		printf("usage: %s [-v][<script_filename> [-l][-d]]\n",argv[0]);
		exit(-1);
	}
	else if(argc == 2)
	{
		if(strcmp(argv[1], "-v")==0)
		{
			printf("zengl lib version is v%d.%d.%d", 
				ZL_EXP_MAJOR_VERSION, ZL_EXP_MINOR_VERSION, ZL_EXP_REVISION);
			return 0;
		}
	}
	else if((strcmp(argv[2],"-l") == 0) || 
		 (strcmp(argv[2],"-d") == 0))
		need_debuglog = TRUE;

	ZENGL_EXPORT_VM_MAIN_ARG_FLAGS flags = 0;
	if(need_debuglog)
	{
		debuglog = fopen("hd/zl.log", "w");
		flags = (ZL_EXP_CP_AF_IN_DEBUG_MODE | ZL_EXP_CP_AF_OUTPUT_DEBUG_INFO);
	}
	else
		flags = ZL_EXP_CP_AF_IN_DEBUG_MODE;
	VM = zenglApi_Open();
	zenglApi_SetFlags(VM, flags);
	zenglApi_SetHandle(VM,ZL_EXP_VFLAG_HANDLE_COMPILE_INFO,main_userdef_info);
	zenglApi_SetHandle(VM,ZL_EXP_VFLAG_HANDLE_RUN_INFO,main_userdef_run_info);
	zenglApi_SetHandle(VM,ZL_EXP_VFLAG_HANDLE_RUN_PRINT,main_userdef_run_print);
	if((builtinID = zenglApi_SetModInitHandle(VM,"builtin",main_builtin_module_init)) == -1)
		main_exit(VM,"set use module failed:%s",zenglApi_GetErrorString(VM));

	if(argc >= 3 && strcmp(argv[2],"-d") == 0)
		zenglApi_DebugSetBreakHandle(VM,main_debug_break,main_debug_conditionError,ZL_EXP_TRUE,ZL_EXP_FALSE); //设置调试API

	if(zenglApi_Run(VM,argv[1]) == -1) { //编译执行zengl脚本
		main_fatal_error_set_error_string(zenglApi_GetErrorString(VM));
		if(main_fatal_error_callback_exec(VM, argv[1], MainFatalError.error_string) == -1) {
			main_exit(VM,"\n the fatal error callback of <%s> failed：\n%s\n", argv[1], zenglApi_GetErrorString(VM));
		}
		else if(MainFatalError.default_cmd_action) {
			main_exit(VM,"compile and run <%s> failed: %s\n", argv[1], MainFatalError.error_string);
		}
	}
	zenglApi_Close(VM);
	if(debuglog != NULL)
		fclose(debuglog);

	return 0;
}

