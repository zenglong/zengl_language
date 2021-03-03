#define ZL_EXP_OS_IN_WINDOWS //在加载下面的zengl头文件之前，windows系统请定义ZL_EXP_OS_IN_WINDOWS，linux , mac系统请定义ZL_EXP_OS_IN_LINUX
#include "zengl_exportfuns.h"
#include "md5.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <stdarg.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/stat.h>
#ifndef ZL_EXP_OS_IN_WINDOWS
	#include <unistd.h>
#endif

#ifdef ZL_LANG_IN_VC6
	#define MAIN_SYS_SPRINTF_ARG_NUM _vsnprintf //使用va_list和限制了字符数的格式化输出函数，另外VC6编译器下的vsnprintf需要在前面加下划线！
#else
	#define MAIN_SYS_SPRINTF_ARG_NUM vsnprintf //使用va_list和限制了字符数的格式化输出函数
#endif

#define STRNULL '\0'
#define DEBUG_INPUT_MAX 50
#define DEF_VERSION "dv01"

#define MAIN_INFO_STRING_SIZE 200 // 动态字符串的初始化和动态扩容的字节大小

typedef struct{
	char str[50];
	int ptrIndex;	//str在内存池中分配的指针的索引
	int size;
	int count;
	int cur;
}ReadStr_Type; //字符串动态数组，用于存放用户从终端输入的信息

typedef struct _MAIN_DATA{
	ZENGL_EXPORT_MEMBLOCK extra_memblock; // 测试用的数组之类的内存块
} MAIN_DATA; // 传递给zengl脚本的测试用的额外数据

typedef struct{
	ZL_EXP_CHAR * function_name; // 回调函数名
	ZL_EXP_CHAR * class_name;    // 如果回调函数属于某个类，即回调函数是某个类里的方法，则该字段会记录具体的类名
	ZL_EXP_CHAR * error_string;  // 记录具体的运行时错误信息
	int default_cmd_action;      // 是否需要执行默认动作，默认情况下，会将错误信息输出到命令行，如果在脚本的回调函数里，已经将错误信息输出到了命令行的话，可以将该字段设为0，从而不会将错误信息再次输出到命令行
}FatalError_Type; // 脚本发生严重的运行时错误时，需要调用的脚本中的回调函数，脚本可以在该回调函数中获取错误信息和函数栈追踪信息，从而可以在脚本中直接处理错误信息(例如写入日志或输出到终端等)

typedef struct _MAIN_INFO_STRING{
	ZL_EXP_CHAR * str;   //字符串指针
	int size;  //字符串的动态大小
	int count; //存放的字符数
	int cur;   //当前游标
} MAIN_INFO_STRING; // 动态字符串类型，该结构中的str字符串指针所指向的字符串可以根据需要，动态的扩充大小

FILE * debuglog;
ReadStr_Type ReadStr;
int random_seed=0;

// MainFatalError全局变量用于记录，当发生运行时错误时，脚本回调函数的相关信息，例如脚本回调函数的函数名，所属的类名等
FatalError_Type MainFatalError = {0};

/**
 * 获取函数栈追踪信息，从而可以知道当前执行的代码，是从哪些函数(或者脚本的哪个位置)执行进来的
 */
static void main_get_stack_backtrace(ZL_EXP_VOID * VM_ARG, MAIN_INFO_STRING * debug_info);

/**
 * 释放动态字符串占用的内存资源
 */
static void main_free_info_string(ZL_EXP_VOID * VM_ARG, MAIN_INFO_STRING * infoStringPtr);

/**
 * 用于运行时错误回调函数名，类名和错误信息相关的字符串拷贝操作
 */
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

/**
 * 将运行时错误回调函数名，类名和错误信息相关的字符串给释放掉
 */
static void main_fatal_error_free_ptrs()
{
	if(MainFatalError.function_name != NULL) {
		free(MainFatalError.function_name);
		MainFatalError.function_name = NULL;
	}
	if(MainFatalError.class_name != NULL) {
		free(MainFatalError.class_name);
		MainFatalError.class_name = NULL;
	}
	if(MainFatalError.error_string != NULL) {
		free(MainFatalError.error_string);
		MainFatalError.error_string = NULL;
	}
}

/**
 * 将运行时错误回调函数名，类名，错误信息以及是否执行默认动作等重置为默认值
 */
static void main_fatal_error_reset_to_default_values()
{
	main_fatal_error_free_ptrs();
	MainFatalError.default_cmd_action = 1;
}

/**
 * 设置运行时错误回调函数名
 */
static void main_fatal_error_set_function_name(ZL_EXP_CHAR * function_name)
{
	MainFatalError.function_name = main_fatal_error_copy_string(function_name, MainFatalError.function_name);
}

/**
 * 设置运行时错误回调相关的类名，如果回调函数属于某个类中定义的方法的话，就需要通过此函数来设置回调相关的类名
 */
static void main_fatal_error_set_class_name(ZL_EXP_CHAR * class_name)
{
	MainFatalError.class_name = main_fatal_error_copy_string(class_name, MainFatalError.class_name);
}

/**
 * 设置运行时错误发生时，需要传递给回调函数的错误信息
 */
static void main_fatal_error_set_error_string(ZL_EXP_CHAR * error_string)
{
	MainFatalError.error_string = main_fatal_error_copy_string(error_string, MainFatalError.error_string);
}

/**
 * 当脚本发生运行时错误时，如果脚本中设置过运行时错误回调函数的话，就调用该函数来处理运行时错误，
 * 同时会将错误信息和函数栈追踪信息，通过参数传递给回调函数
 */
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

/**
 * 将字符串追加到MAIN_INFO_STRING类型定义的动态字符串的末尾，format定义了需要追加的字符串的格式，具体格式可以参考vsnprintf库函数的相关文档
 */
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
		retcount = MAIN_SYS_SPRINTF_ARG_NUM((infoStringPtr->str + infoStringPtr->cur),
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

/**
 * 释放动态字符串占用的内存资源
 */
static void main_free_info_string(ZL_EXP_VOID * VM_ARG, MAIN_INFO_STRING * infoStringPtr)
{
	if(infoStringPtr->str != NULL) {
		zenglApi_FreeMem(VM_ARG, infoStringPtr->str);
		infoStringPtr->str = NULL;
	}
	infoStringPtr->count = infoStringPtr->cur = infoStringPtr->size = 0;
}

/**
 * 获取函数栈追踪信息，从而可以知道当前执行的代码，是从哪些函数(或者脚本的哪个位置)执行进来的
 */
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

/**
 * 将append_path路径追加到full_path中，如果追加路径后，full_path长度会超出full_path_size时，路径将会被截断
 */
static int main_full_path_append(char * full_path, int full_path_length, int full_path_size, char * append_path)
{
	int append_path_length = strlen(append_path);
	int max_length = full_path_size - full_path_length - 1;
	if(append_path_length > max_length)
		append_path_length = max_length;
	if(append_path_length > 0)
		strncpy((full_path + full_path_length), append_path, append_path_length);
	return append_path_length;
}

static void main_compute_md5(char * buf, char * str, ZL_EXP_BOOL isLowerCase, ZL_EXP_BOOL is32)
{
	MD5_CTX md5;
	unsigned char * encrypt = (unsigned char *)str;
	unsigned char decrypt[16];
	char * p;
	const char * format;
	int start_idx, end_idx, i;
	MD5Init(&md5);
	MD5Update(&md5,encrypt,strlen((char *)encrypt));
	MD5Final(&md5,decrypt);
	p = buf;
	start_idx = is32 ? 0 : 4;
	end_idx = is32 ? 16 : 12;
	format = isLowerCase ? "%02x" : "%02X";
	for(i = start_idx; i < end_idx; i++) {
		sprintf(p, format, decrypt[i]);
		p += 2;
	}
	(*p) = '\0';
}

/**
 * 根据full_path脚本路径，得到最终要生成的缓存文件的路径信息
 */
static void main_get_zengl_cache_path(char * cache_path, int cache_path_size, char * full_path)
{
	char fullpath_md5[33];
	char cache_prefix[30] = {0};
	const char * cache_path_prefix = "caches/"; // 缓存文件都放在caches目录中
	int append_length;
	main_compute_md5(fullpath_md5, full_path, ZL_EXP_TRUE, ZL_EXP_TRUE); // 将full_path进行md5编码
	// 在缓存路径前面加上zengl版本号和指针长度，不同的zengl版本生成的缓存有可能会不一样，另外，32位和64位环境下生成的内存缓存数据也是不一样的
	// 32位系统中生成的缓存数据放到64位中运行，或者反过来，都会报内存相关的错误
	sprintf(cache_prefix, "%d_%d_%d_%ld_%s_", ZL_EXP_MAJOR_VERSION, ZL_EXP_MINOR_VERSION, ZL_EXP_REVISION, sizeof(char *), DEF_VERSION);
	append_length = main_full_path_append(cache_path, 0, cache_path_size, (char *)cache_path_prefix);
	append_length += main_full_path_append(cache_path, append_length, cache_path_size, cache_prefix);
	append_length += main_full_path_append(cache_path, append_length, cache_path_size, fullpath_md5);
	cache_path[append_length] = '\0';
}

/**
 * 尝试重利用full_path脚本文件对应的缓存数据，cache_path表示缓存数据所在的文件路径
 * 如果缓存文件不存在，则会重新生成缓存文件，如果full_path脚本文件内容发生了改变或者其加载的脚本文件内容发生了改变，也会重新生成缓存
 * 外部调用者通过is_reuse_cache变量的值来判断是否需要生成缓存文件，如果is_reuse_cache为ZL_EXP_FALSE，就表示没有重利用缓存，则需要生成缓存文件
 * 如果is_reuse_cache为ZL_EXP_TRUE，则说明重利用了缓存，不需要再生成缓存文件了
 */
static void main_try_to_reuse_zengl_cache(ZL_EXP_VOID * VM, char * cache_path, char * full_path, ZL_EXP_BOOL * is_reuse_cache)
{
	FILE * ptr_fp;
	ZL_EXP_VOID * cachePoint;
	ZENGL_EXPORT_API_CACHE_TYPE * api_cache;
	ZL_EXP_LONG offset, cache_mtime, file_mtime;
	ZL_EXP_BYTE * mempoolPtr;
	ZL_EXP_CHAR ** filenames, * filename;
	ZL_EXP_INT cacheSize, i;
	struct stat stat_result;
	(* is_reuse_cache) = ZL_EXP_FALSE;
	if(stat(cache_path, &stat_result)==0) { // 获取缓存文件的修改时间
		cache_mtime = (ZL_EXP_LONG)stat_result.st_mtime;
	}
	else { // 获取文件的状态信息失败，可能缓存文件不存在，需要重新编译生成缓存，直接返回
		printf("stat cache file: \"%s\" failed, maybe no cache file [recompile]\n", cache_path);
		return ;
	}
	if(stat(full_path, &stat_result)==0) { // 获取主执行脚本的修改时间
		file_mtime = (ZL_EXP_LONG)stat_result.st_mtime;
		printf("%s mtime:%ld", full_path, file_mtime);
		if(file_mtime >= cache_mtime) { // 如果主执行脚本的修改时间大于等于缓存数据的修改时间，则说明主执行脚本的内容发生了改变，需要重新编译生成新的缓存
			printf(" [changed] [recompile]\n");
			return;
		}
		printf("\n");
	}
	else { // 主执行脚本不存在，直接返回
		printf("warning stat script file: \"%s\" failed, maybe no such file! [recompile]\n", full_path);
		return ;
	}
	// 打开缓存文件
	if((ptr_fp = fopen(cache_path, "rb")) == NULL) {
		printf("no cache file: \"%s\" [recompile]\n", cache_path);
		return ;
	}
	fseek(ptr_fp,0L,SEEK_END);
	cacheSize = ftell(ptr_fp); // 得到缓存数据的大小
	fseek(ptr_fp,0L,SEEK_SET);
	cachePoint = malloc(cacheSize); // 根据缓存大小分配堆空间，先将缓存数据读取到该堆内存中
	if(fread(cachePoint, cacheSize, 1, ptr_fp) != 1) { // 读取缓存数据
		printf("read cache file \"%s\" failed [recompile]\n", cache_path);
		goto end;
	}
	api_cache = (ZENGL_EXPORT_API_CACHE_TYPE *)cachePoint;
	if(api_cache->signer != ZL_EXP_API_CACHE_SIGNER) { // 根据缓存签名判断是否是有效的缓存数据
		printf("invalid cache file \"%s\" [recompile]\n", cache_path);
		goto end;
	}
	mempoolPtr = ((ZL_EXP_BYTE *)cachePoint + api_cache->mempoolOffset);
	offset = (ZL_EXP_LONG)api_cache->filenames;
	filenames = (ZL_EXP_CHAR **)(mempoolPtr + offset - 1);
	if(api_cache->filenames_count > 0) {
		// 循环判断加载的脚本文件的内容是否发生了改变，如果改变了，则需要重新编译生成新的缓存
		for(i=0; i < api_cache->filenames_count; i++) {
			offset = (ZL_EXP_LONG)(filenames[i]);
			filename = (ZL_EXP_CHAR *)(mempoolPtr + offset - 1);
			printf("%s", filename);
			if(stat(filename, &stat_result)==0) {
				file_mtime = (ZL_EXP_LONG)stat_result.st_mtime;
				printf(" mtime:%ld", file_mtime);
				if(file_mtime >= cache_mtime){
					printf(" [changed] [recompile]\n");
					goto end;
				}
			}
			else {
				printf(" stat failed [recompile]\n");
				goto end;
			}
			printf("\n");
		}
	}
	// 通过zenglApi_ReUseCacheMemData接口函数，将编译好的缓存数据加载到编译器和解释器中，这样就可以跳过编译过程，直接运行
	if(zenglApi_ReUseCacheMemData(VM, cachePoint, cacheSize) == -1) {
		printf("reuse cache file \"%s\" failed: %s [recompile]\n", cache_path, zenglApi_GetErrorString(VM));
		goto end;
	}
	(* is_reuse_cache) = ZL_EXP_TRUE;
	printf("reuse cache file: \"%s\" mtime:%ld\n", cache_path, cache_mtime);
end:
	fclose(ptr_fp);
	free(cachePoint);
}

/**
 * 在编译执行结束后，生成缓存数据并写入缓存文件
 */
static void main_write_zengl_cache_to_file(ZL_EXP_VOID * VM, char * cache_path)
{
	FILE * ptr_fp;
	ZL_EXP_VOID * cachePoint;
	ZL_EXP_INT cacheSize;
	// 通过zenglApi_CacheMemData接口函数，将编译器和解释器中的主要的内存数据缓存到cachePoint对应的内存中
	if(zenglApi_CacheMemData(VM, &cachePoint, &cacheSize) == -1) {
		printf("write zengl cache to file \"%s\" failed: %s\n", cache_path,zenglApi_GetErrorString(VM));
		return;
	}

	// 打开cache_path对应的缓存文件
	if((ptr_fp = fopen(cache_path, "wb")) == NULL) {
		printf("write zengl cache to file \"%s\" failed: open failed\n", cache_path);
		return;
	}

	// 将缓存数据写入缓存文件
	if( fwrite(cachePoint, cacheSize, 1, ptr_fp) != 1)
		printf("write zengl cache to file \"%s\" failed: write failed\n", cache_path);
	else
		printf("write zengl cache to file \"%s\" success \n", cache_path);
	fclose(ptr_fp);
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
			printf("命令必须是一个字符\n");
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
						printf("p调试错误：%s\n",zenglApi_GetErrorString(VM_ARG));
						continue;
					}
					main_print_debug(VM_ARG,arg);
				}
				else
					printf("p命令缺少参数\n");
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
					printf("b命令缺少文件名参数\n");
					continue;
				}
				arg = getDebugArg(str,&start,ZL_EXP_TRUE);
				if(arg != ZL_EXP_NULL && strlen(arg) > 0)
					line = atoi(arg);
				else
				{
					printf("b命令缺少行号参数\n");
					continue;
				}
				arg = getDebugArg(str,&start,ZL_EXP_TRUE);
				if(arg != ZL_EXP_NULL && strlen(arg) > 0)
					count = atoi(arg);
				if(zenglApi_DebugSetBreak(VM_ARG,filename,line,ZL_EXP_NULL,ZL_EXP_NULL,count,ZL_EXP_FALSE) == -1)
					printf("b命令error:%s\n",zenglApi_GetErrorString(VM_ARG));
				else
					printf("设置断点成功\n");
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
					// zenglApi_DebugGetTrace(VM_ARG,&arg,&loc,&pc,&fileName,&line,ZL_EXP_NULL,ZL_EXP_NULL); // 该行会让r返回到上一层的上一层，多返回一层，所以注释掉
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
					printf("d命令缺少断点索引参数\n");
					continue;
				}
				if(zenglApi_DebugDelBreak(VM_ARG,index) == -1)
					printf("d命令error:无效的断点索引");
				else
					printf("删除断点成功");
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
					printf("D命令缺少断点索引参数\n");
					continue;
				}
				if(zenglApi_DebugGetBreak(VM_ARG,index,&filename,&line,&condition,&log,&count,&disabled,ZL_EXP_NULL) == -1)
				{
					printf("D命令error:无效的断点索引\n");
					continue;
				}
				else
				{
					if(zenglApi_DebugSetBreak(VM_ARG,filename,line,condition,log,count,ZL_EXP_TRUE) == -1)
						printf("D命令禁用断点error:%s",zenglApi_GetErrorString(VM_ARG));
					else
						printf("D命令禁用断点成功");
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
					printf("C命令缺少断点索引参数\n");
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
					printf("C命令error:无效的断点索引\n");
					continue;
				}
				else
				{
					if(zenglApi_DebugSetBreak(VM_ARG,filename,line,newCondition,log,count,disabled) == -1)
						printf("C命令设置条件断点error:%s",zenglApi_GetErrorString(VM_ARG));
					else
						printf("C命令设置条件断点成功");
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
					printf("L命令缺少断点索引参数\n");
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
					printf("L命令error:无效的断点索引\n");
					continue;
				}
				else
				{
					if(zenglApi_DebugSetBreak(VM_ARG,filename,line,condition,newLog,count,disabled) == -1)
						printf("L命令设置日志断点error:%s",zenglApi_GetErrorString(VM_ARG));
					else
						printf("L命令设置日志断点成功");
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
					printf("N命令缺少断点索引参数\n");
					continue;
				}
				arg = getDebugArg(str,&start,ZL_EXP_TRUE);
				if(arg != ZL_EXP_NULL && strlen(arg) > 0 && main_isNumber(arg))
					newCount = atoi(arg);
				else
				{
					printf("N命令缺少断点次数参数\n");
					continue;
				}
				if(zenglApi_DebugGetBreak(VM_ARG,index,&filename,&line,&condition,&log,&count,&disabled,ZL_EXP_NULL) == -1)
				{
					printf("N命令error:无效的断点索引\n");
					continue;
				}
				else
				{
					if(zenglApi_DebugSetBreak(VM_ARG,filename,line,condition,log,newCount,disabled) == -1)
						printf("N命令设置断点次数error:%s",zenglApi_GetErrorString(VM_ARG));
					else
						printf("N命令设置断点次数成功");
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
			printf(" p 调试变量信息 usage:p express\n"
				" b 设置断点 usage:b filename lineNumber\n"
				" B 查看断点列表 usage:B\n"
				" T 查看脚本函数的堆栈调用信息 usage:T\n"
				" d 删除某断点 usage:d breakIndex\n"
				" D 禁用某断点 usage:D breakIndex\n"
				" C 设置条件断点 usage:C breakIndex condition-express\n"
				" L 设置日志断点 usage:L breakIndex log-express\n"
				" N 设置断点次数 usage:N breakIndex count\n"
				" s 单步步入 usage:s\n"
				" S 单步步过 usage:S\n"
				" r 执行到返回 usage:r\n"
				" c 继续执行 usage:c\n");
			break;
		default:
			printf("无效的命令\n");
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
	fprintf(debuglog,"%s",infoStrPtr);
	return 0;
}

ZL_EXP_INT main_userdef_compile_error(ZL_EXP_CHAR * infoStrPtr, ZL_EXP_INT infoStrCount,ZL_EXP_VOID * VM_ARG)
{
	printf("%s",infoStrPtr);
	return 0;
}

ZL_EXP_INT main_userdef_run_info(ZL_EXP_CHAR * infoStrPtr, ZL_EXP_INT infoStrCount,ZL_EXP_VOID * VM_ARG)
{
	fprintf(debuglog,"%s",infoStrPtr);
	return 0;
}

ZL_EXP_INT main_userdef_run_print(ZL_EXP_CHAR * infoStrPtr, ZL_EXP_INT infoStrCount,ZL_EXP_VOID * VM_ARG)
{
	printf("%s\n",infoStrPtr);
	return 0;
}

ZL_EXP_INT main_userdef_run_error(ZL_EXP_CHAR * infoStrPtr, ZL_EXP_INT infoStrCount,ZL_EXP_VOID * VM_ARG)
{
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
	time_t t;
	if(random_seed == 0) //第一次使用时间作为随机种子。
	{
		srand((unsigned) time(&t));
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
	ZL_EXP_CHAR * key;
	ZENGL_EXPORT_MOD_FUN_ARG mblk_val = {ZL_EXP_FAT_NONE,{0}};
	zenglApi_GetMemBlockInfo(VM_ARG,&memblock,&size,ZL_EXP_NULL);
	for(i=1;i<=size;i++)
	{
		mblk_val = zenglApi_GetMemBlock(VM_ARG,&memblock,i);
		zenglApi_GetMemBlockHashKey(VM_ARG,&memblock,i-1,&key);
		switch(mblk_val.type)
		{
		case ZL_EXP_FAT_INT:
		case ZL_EXP_FAT_FLOAT:
		case ZL_EXP_FAT_STR:
		case ZL_EXP_FAT_MEMBLOCK:
			for(j=0;j<recur_count;j++)
				printf("  ");
			break;
		}
		switch(mblk_val.type)
		{
		case ZL_EXP_FAT_INT:
			if(key != ZL_EXP_NULL)
				printf("[%d]{%s} %ld\n",i-1,key,mblk_val.val.integer);
			else
				printf("[%d] %ld\n",i-1,mblk_val.val.integer);
			break;
		case ZL_EXP_FAT_FLOAT:
			if(key != ZL_EXP_NULL)
				printf("[%d]{%s} %.16g\n",i-1,key,mblk_val.val.floatnum);
			else
				printf("[%d] %.16g\n",i-1,mblk_val.val.floatnum);
			break;
		case ZL_EXP_FAT_STR:
			if(key != ZL_EXP_NULL)
				printf("[%d]{%s} %s\n",i-1,key,mblk_val.val.str);
			else
				printf("[%d] %s\n",i-1,mblk_val.val.str);
			break;
		case ZL_EXP_FAT_MEMBLOCK:
			if(key != ZL_EXP_NULL)
				printf("[%d]{%s} <array or class obj type> begin:\n",i-1,key);
			else
				printf("[%d] <array or class obj type> begin:\n",i-1);
			main_print_array(VM_ARG,mblk_val.val.memblock,recur_count+1);
			for(j=0;j<recur_count;j++) printf("  "); // 递归调用返回后，输出前缀空格，以便于排版
			if(key != ZL_EXP_NULL)
				printf("[%d]{%s} <array or class obj type> end\n",i-1,key);
			else
				printf("[%d] <array or class obj type> end\n",i-1);
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

/*bltTestPrintByArrayKey模块函数(仅供测试)，用于测试zenglApi_GetMemBlockByHashKey接口函数*/
ZL_EXP_VOID main_builtin_test_print_by_array_key(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount)
{
	ZENGL_EXPORT_MOD_FUN_ARG arg = {ZL_EXP_FAT_NONE,{0}};
	ZENGL_EXPORT_MEMBLOCK memblock = {0};
	ZENGL_EXPORT_MOD_FUN_ARG mblk_val = {ZL_EXP_FAT_NONE,{0}};
	if(argcount != 2)
		zenglApi_Exit(VM_ARG,"usage:bltTestPrintByArrayKey(array, key)");
	zenglApi_GetFunArg(VM_ARG,1,&arg);
	if(arg.type != ZL_EXP_FAT_MEMBLOCK)
		zenglApi_Exit(VM_ARG,"bltTestPrintByArrayKey函数的第一个参数必须是哈希数组");
	memblock = arg.val.memblock;
	zenglApi_GetFunArg(VM_ARG,2,&arg);
	if(arg.type != ZL_EXP_FAT_STR)
		zenglApi_Exit(VM_ARG,"bltTestPrintByArrayKey函数的第二个参数必须是字符串，表示数组中的key");
	// 通过zenglApi_GetMemBlockByHashKey接口，实现在模块函数中，根据字符串key来获取数组的成员
	mblk_val = zenglApi_GetMemBlockByHashKey(VM_ARG, &memblock, arg.val.str);
	switch(mblk_val.type)
	{
	case ZL_EXP_FAT_INT:
		printf("%ld\n", mblk_val.val.integer);
		break;
	case ZL_EXP_FAT_FLOAT:
		printf("%.16g\n", mblk_val.val.floatnum);
		break;
	case ZL_EXP_FAT_STR:
		printf("%s\n", mblk_val.val.str);
		break;
	case ZL_EXP_FAT_MEMBLOCK:
		main_print_array(VM_ARG, mblk_val.val.memblock, 0);
		break;
	default:
		zenglApi_Exit(VM_ARG,"bltTestPrintByArrayKey函数检测到无效的类型");
		break;
	}
}

/*bltGetArrayNNCount模块函数，获取数组之类的内存块中有多少个非NONE类型的成员*/
ZL_EXP_VOID main_builtin_get_array_nncount(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount)
{
	ZENGL_EXPORT_MOD_FUN_ARG arg = {ZL_EXP_FAT_NONE,{0}};
	ZENGL_EXPORT_MEMBLOCK memblock = {0};
	ZL_EXP_INT nncount;
	if(argcount != 1)
		zenglApi_Exit(VM_ARG,"usage:bltGetArrayNNCount(array)");
	zenglApi_GetFunArg(VM_ARG,1,&arg);
	if(arg.type != ZL_EXP_FAT_MEMBLOCK)
		zenglApi_Exit(VM_ARG,"bltGetArrayNNCount函数的第一个参数必须是数组");
	memblock = arg.val.memblock;
	nncount = zenglApi_GetMemBlockNNCount(VM_ARG, &memblock);
	zenglApi_SetRetVal(VM_ARG, ZL_EXP_FAT_INT, ZL_EXP_NULL, nncount, 0);
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

/*bltGetStrLen模块函数，获取字符串长度*/
ZL_EXP_VOID main_builtin_get_str_len(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount)
{
	ZENGL_EXPORT_MOD_FUN_ARG arg = {ZL_EXP_FAT_NONE,{0}};
	int str_len;
	if(argcount < 1)
		zenglApi_Exit(VM_ARG,"usage: bltGetStrLen(str)");
	zenglApi_GetFunArg(VM_ARG,1,&arg); //得到第一个参数
	if(arg.type != ZL_EXP_FAT_STR)
		zenglApi_Exit(VM_ARG,"bltGetStrLen函数的第一个参数必须是字符串");
	str_len = strlen(arg.val.str);
	zenglApi_SetRetVal(VM_ARG, ZL_EXP_FAT_INT, ZL_EXP_NULL, str_len, 0);
}

/**
 * bltCount模块函数，获取数组的有效成员数，或者获取字符串的有效长度
 */
ZL_EXP_VOID main_builtin_count(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount)
{
	ZENGL_EXPORT_MOD_FUN_ARG arg = {ZL_EXP_FAT_NONE,{0}};
	int retcount;
	if(argcount < 1)
		zenglApi_Exit(VM_ARG,"usage: bltCount(data)");
	zenglApi_GetFunArg(VM_ARG,1,&arg);
	switch(arg.type) {
	case ZL_EXP_FAT_STR:
		retcount = strlen(arg.val.str);
		break;
	case ZL_EXP_FAT_MEMBLOCK:
		retcount = zenglApi_GetMemBlockNNCount(VM_ARG, &arg.val.memblock);
		break;
	default:
		retcount = 0;
		break;
	}
	zenglApi_SetRetVal(VM_ARG, ZL_EXP_FAT_INT, ZL_EXP_NULL, retcount, 0);
}

ZL_EXP_VOID main_userdef_module_init(ZL_EXP_VOID * VM_ARG);

/*bltLoadScript模块函数，新建一个虚拟机，加载并执行某个脚本*/
ZL_EXP_VOID main_builtin_load_script(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount)
{
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
	zenglApi_GetFunArg(VM_ARG,1,&arg); //获取第一个参数为脚本名
	if(arg.type != ZL_EXP_FAT_STR)
		zenglApi_Exit(VM_ARG,"bltLoadScript函数第一个参数必须字符串，代表要加载的脚本文件名");
	scriptName = arg.val.str;
	zenglApi_Load(scriptName,&vm_main_args); 
}

/*bltGetZLVersion模块函数，获取当前zengl版本号信息的字符串形式*/
ZL_EXP_VOID main_builtin_get_zl_version(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount)
{
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

/*bltGetExtraArray模块函数，获取测试用的哈希数组(如果不存在则创建该数组)，如果已经存在则直接返回该数组*/
ZL_EXP_VOID main_builtin_get_extraArray(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount)
{
	ZENGL_EXPORT_MOD_FUN_ARG arg = {ZL_EXP_FAT_NONE,{0}};
	MAIN_DATA * my_data = zenglApi_GetExtraData(VM_ARG, "my_data");

	// 如果没有创建过哈希数组，则创建哈希数组，并用字符串作为key设置数组成员，由于下面是根据ptr是否等于NULL来判断是否创建过的，
	// 因此，需要在脚本执行前，事先将ptr的值显示的设置为NULL，否则，如果my_data的来源是局部变量的话，ptr的默认初始值就不会是NULL
	if(my_data->extra_memblock.ptr == ZL_EXP_NULL) {
		if(zenglApi_CreateMemBlock(VM_ARG,&my_data->extra_memblock,0) == -1) {
			zenglApi_Exit(VM_ARG,zenglApi_GetErrorString(VM_ARG));
		}
		// 手动增加该内存块的引用计数值，使其能够在脚本执行的整个生命周期中都一直存在，而不会被释放掉
		// 当数组之类的内存块在脚本函数中被赋值给局部变量后，在脚本函数返回时，会释放掉所有局部变量对应的内存块(除非某个局部变量被return返回)，
		// 通过手动增加内存块的引用计数值，这样，脚本函数返回时，内存块的引用计数值就不会被减为0，也就不会被释放掉
		zenglApi_AddMemBlockRefCount(VM_ARG,&my_data->extra_memblock,1); 

		arg.type = ZL_EXP_FAT_STR;
		arg.val.str = "zengl";
		// 使用zenglApi_SetMemBlockByHashKey接口将字符串作为key设置哈希数组的成员
		zenglApi_SetMemBlockByHashKey(VM_ARG, &my_data->extra_memblock, "name", &arg);

		arg.type = ZL_EXP_FAT_STR;
		arg.val.str = "programmer";
		zenglApi_SetMemBlockByHashKey(VM_ARG, &my_data->extra_memblock, "job", &arg);
		
		// 将创建的哈希数组作为结果返回
		zenglApi_SetRetValAsMemBlock(VM_ARG,&my_data->extra_memblock);
	}
	else {
		// 如果之前已经创建过哈希数组，就直接将该数组返回
		zenglApi_SetRetValAsMemBlock(VM_ARG,&my_data->extra_memblock);
	}
}

/**
 * bltFatalErrorCallback模块函数，设置当发生运行时错误时，需要调用的脚本函数名，如果是类中定义的方法，还可以设置相关的类名
 * 第一个参数function_name表示需要设置的脚本函数名(必须是字符串类型，且不能为空字符串)
 * 第二个参数class_name是可选参数，表示需要设置的类名(也必须是字符串类型，如果第一个参数function_name是某个类中定义的方法的话，就可以通过此参数来设置类名)
 * 		- 默认值是空字符串，表示不设置类名，当需要跳过该参数设置第三个default_cmd_action参数时，也可以手动传递空字符串来表示不设置类名
 * 第三个参数default_cmd_action也是可选的，表示在执行完运行时错误回调函数后，是否还需要执行默认的输出错误信息到命令行的动作，
 * 		- 默认值为1，表示需要执行默认动作，如果脚本回调函数里已经将错误信息输出到了命令行的话，就可以将该参数设置为0，表示不需要再执行默认动作了
 */
ZL_EXP_VOID main_builtin_fatal_error_callback(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount)
{
	ZENGL_EXPORT_MOD_FUN_ARG arg = {ZL_EXP_FAT_NONE,{0}};
	const ZL_EXP_CHAR * func_name = "bltFatalErrorCallback";
	ZL_EXP_CHAR * function_name = NULL;
	ZL_EXP_CHAR * class_name = NULL;
	if(argcount < 1)
		zenglApi_Exit(VM_ARG,"usage: %s(function_name[, class_name = ''[, default_cmd_action = 1]])", func_name);
	zenglApi_GetFunArg(VM_ARG,1,&arg);
	if(arg.type != ZL_EXP_FAT_STR) {
		zenglApi_Exit(VM_ARG,"the first argument [function_name] of %s must be string", func_name);
	}
	function_name = (ZL_EXP_CHAR *)arg.val.str;
	if(strlen(function_name) == 0) {
		zenglApi_Exit(VM_ARG,"the first argument [function_name] of %s can't be empty", func_name);
	}
	// 在设置运行时错误回调函数名之前，先将之前可能设置过的回调函数相关的函数名，类名等重置为默认值，可以防止受到之前设置的影响
	main_fatal_error_reset_to_default_values();
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
	zenglApi_SetModFunHandle(VM_ARG,moduleID,"bltTestPrintByArrayKey",main_builtin_test_print_by_array_key);
	zenglApi_SetModFunHandle(VM_ARG,moduleID,"bltGetArrayNNCount",main_builtin_get_array_nncount);
	zenglApi_SetModFunHandle(VM_ARG,moduleID,"bltSetArray",main_builtin_set_array);
	zenglApi_SetModFunHandle(VM_ARG,moduleID,"bltGetStrLen",main_builtin_get_str_len);
	zenglApi_SetModFunHandle(VM_ARG,moduleID,"bltCount",main_builtin_count);
	zenglApi_SetModFunHandle(VM_ARG,moduleID,"bltLoadScript",main_builtin_load_script);
	zenglApi_SetModFunHandle(VM_ARG,moduleID,"bltGetZLVersion",main_builtin_get_zl_version);
	zenglApi_SetModFunHandle(VM_ARG,moduleID,"bltGetExtraData",main_builtin_get_extraData);
	zenglApi_SetModFunHandle(VM_ARG,moduleID,"bltGetExtraArray",main_builtin_get_extraArray);
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

ZL_EXP_VOID main_def_lookup_handle(ZL_EXP_VOID * VM_ARG, ZL_EXP_CHAR * defValName)
{
	if(strcmp(defValName, "___TRUE___") == 0)
	{
		zenglApi_SetDefLookupResult(VM_ARG, ZL_EXP_FAT_INT, "1");
	}
	else if(strcmp(defValName, "___FALSE___") == 0)
	{
		zenglApi_SetDefLookupResult(VM_ARG, ZL_EXP_FAT_INT, "0");
	}
	else if(strcmp(defValName, "___PI___") == 0)
	{
		zenglApi_SetDefLookupResult(VM_ARG, ZL_EXP_FAT_FLOAT, "3.141592653");
	}
	else if(strcmp(defValName, "___TEST_STR___") == 0)
	{
		zenglApi_SetDefLookupResult(VM_ARG, ZL_EXP_FAT_STR, "this is a test string by user defined");
	}
}

void main_exit(void * VM,char * err_format,...)
{
	va_list arg;
	va_start(arg,err_format);
	vprintf(err_format,arg);
	va_end(arg);
	zenglApi_Close(VM);
	main_fatal_error_free_ptrs();
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
	rc4_init(state,rc4_key_str,rc4_key_len);
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
/*void main(int argc,char * argv[])
{
	int len = 0;
	ZENGL_EXPORT_VM_MAIN_ARGS vm_main_args = {main_userdef_info , 
											  main_userdef_compile_error,
											  main_userdef_run_info,
											  main_userdef_run_print,
											  main_userdef_run_error,
											  main_userdef_module_init,
											  ZL_EXP_CP_AF_IN_DEBUG_MODE | 
											  ZL_EXP_CP_AF_OUTPUT_DEBUG_INFO};
	if(argc < 2)
	{
		printf("usage: %s <filename> ... (用法错误，应该是程序名加文件名加选项参数的形式，文件名通常是以.zlc结尾，也可以是.zl结尾)\n",argv[0]);
		#ifdef ZL_EXP_OS_IN_WINDOWS
			system("pause");
		#endif
		exit(-1);
	}

	printf("compiling(编译中)...\n");
	debuglog = fopen("main_debuglogs.txt","w+");
	if(zenglApi_Load(argv[1],&vm_main_args) == -1) //编译执行zengl脚本
	{
		printf("错误：编译<%s>失败！请查看debuglogs.txt分析出错原因！\n",argv[1]);
		#ifdef ZL_EXP_OS_IN_WINDOWS
			system("pause");
		#endif
		exit(-1);
	}
	fclose(debuglog);
	printf("compile finished(编译结束)\n");

	#ifdef ZL_EXP_OS_IN_WINDOWS
		system("pause");
	#endif
}*/

#ifdef ZL_EXP_OS_IN_WINDOWS
	#define stat _stat
#endif

/**
	用户程序执行入口。
*/
int main(int argc,char * argv[])
{
	int len = 0;
	//long testint;
	//int * testint_ptr;
	//double testdouble;
	char * teststr = 0;
	//char * xor_key_str = "xorkey_334566_hello_world";
	char * rc4_key_str = "rc4key_334566_hello_world";
	int rc4_key_len = strlen(rc4_key_str);
	char * run_str = 
		"//this is a test for zenglApi_RunStr \n"
		"inc 'test2.zl';\n"
		"a = 135; \n"
		"b=2;\n"
		"print 'a-b is '+(a-b); \n"
		"bltLoadScript('test3.zl'); \n"; //行尾设置\n换行符这样查找语法错误时，方便定位行号信息
	int run_str_len = strlen(run_str);
	MAIN_DATA my_data; // 测试用的额外数据
	ZL_EXP_INT builtinID,sdlID;
	ZL_EXP_VOID * VM;
	ZL_EXP_BOOL is_reuse_cache;
	char cache_path[200];

	MainFatalError.default_cmd_action = 1;

	if(argc < 2)
	{
		printf("usage: %s <filename> ... (用法错误，应该是：程序名 + zengl脚本文件名的形式，在后面加-d参数可以开启调试)\n",argv[0]);
		#ifdef ZL_EXP_OS_IN_WINDOWS
			system("pause");
		#endif
		exit(-1);
	}

	printf("run(编译执行中)...\n");
	debuglog = fopen("main_debuglogs.txt","w+");
	VM = zenglApi_Open();
	zenglApi_SetFlags(VM,(ZENGL_EXPORT_VM_MAIN_ARG_FLAGS)(ZL_EXP_CP_AF_IN_DEBUG_MODE | ZL_EXP_CP_AF_OUTPUT_DEBUG_INFO));
	zenglApi_SetHandle(VM,ZL_EXP_VFLAG_HANDLE_COMPILE_INFO,main_userdef_info);
	zenglApi_SetHandle(VM,ZL_EXP_VFLAG_HANDLE_RUN_INFO,main_userdef_run_info);
	zenglApi_SetHandle(VM,ZL_EXP_VFLAG_HANDLE_RUN_PRINT,main_userdef_run_print);
	//zenglApi_SetHandle(VM,ZL_EXP_VFLAG_HANDLE_MODULE_INIT,main_userdef_module_init); //也可以在此处设置模块初始化句柄
	if((builtinID = zenglApi_SetModInitHandle(VM,"builtin",main_builtin_module_init)) == -1)
		main_exit(VM,"设置use模块句柄失败:%s",zenglApi_GetErrorString(VM));

	if((sdlID = zenglApi_SetModInitHandle(VM,"sdl",main_sdl_module_init)) == -1)
		main_exit(VM,"设置use模块句柄失败:%s",zenglApi_GetErrorString(VM));

	if(zenglApi_SetModFunHandle(VM,builtinID,"bltTest",main_builtin_printf) == -1)
		main_exit(VM,"设置模块函数失败:%s",zenglApi_GetErrorString(VM));

	if(zenglApi_SetExtraData(VM,"name","my name is zengl") == -1)
		main_exit(VM,"设置额外数据失败:%s",zenglApi_GetErrorString(VM));

	if(zenglApi_SetExtraData(VM,"val","my val is zengl too") == -1)
		main_exit(VM,"设置额外数据失败:%s",zenglApi_GetErrorString(VM));

	my_data.extra_memblock.ptr = ZL_EXP_NULL; // 将ptr的初始值设置为NULL(空指针)
	my_data.extra_memblock.index = 0;

	if(zenglApi_SetExtraData(VM,"my_data",&my_data) == -1)
		main_exit(VM,"设置额外数据my_data失败:%s",zenglApi_GetErrorString(VM));

	//zenglApi_SetSourceRC4Key(VM,rc4_key_str,rc4_key_len);

	if(argc >= 3 && strcmp(argv[2],"-d") == 0)
		zenglApi_DebugSetBreakHandle(VM,main_debug_break,main_debug_conditionError,ZL_EXP_TRUE,ZL_EXP_FALSE); //设置调试API

	zenglApi_SetDefLookupHandle(VM, main_def_lookup_handle);

	// 根据脚本文件名得到缓存文件的路径信息
	main_get_zengl_cache_path(cache_path, sizeof(cache_path), argv[1]);
	// 尝试重利用缓存数据
	main_try_to_reuse_zengl_cache(VM, cache_path, argv[1], &is_reuse_cache);
	if(zenglApi_Run(VM,argv[1]) == -1) { //编译执行zengl脚本
		main_fatal_error_set_error_string(zenglApi_GetErrorString(VM));
		if(main_fatal_error_callback_exec(VM, argv[1], MainFatalError.error_string) == -1) {
			main_exit(VM,"\n执行脚本<%s>的FatalError回调函数失败：\n%s\n", argv[1], zenglApi_GetErrorString(VM));
		}
		else if(MainFatalError.default_cmd_action) {
				main_exit(VM,"错误：编译执行<%s>失败：%s\n", argv[1], MainFatalError.error_string);
		}
	}
	else if(!is_reuse_cache) // 如果没有重利用缓存数据，则生成新的缓存数据，并将其写入缓存文件中
		main_write_zengl_cache_to_file(VM, cache_path);

	/*if(zenglApi_GetValueAsInt(VM,"i",&testint) != -1)
		printf("after run , the i is %ld\n",testint);

	zenglApi_Reset(VM);

	zenglApi_SetFlags(VM,(ZENGL_EXPORT_VM_MAIN_ARG_FLAGS)(ZL_EXP_CP_AF_IN_DEBUG_MODE));

	zenglApi_SetModInitHandle(VM,"builtin",main_builtin_module_init);

	printf("\n[next test zenglApi_RunStr]: \n");
	//zenglApi_DebugSetBreakHandle(VM,main_debug_break,main_debug_conditionError,ZL_EXP_TRUE,ZL_EXP_FALSE);
	if(zenglApi_RunStr(VM,run_str,run_str_len,"runstr") == -1) //编译执行字符串脚本
		main_exit(VM,"错误：编译runstr失败：%s\n",zenglApi_GetErrorString(VM));

	printf("\n[next test zenglApi_Call and zenglApi_ReUse]: \n");

	zenglApi_Reset(VM);

	zenglApi_Push(VM,ZL_EXP_FAT_INT,0,1415,0);

	zenglApi_Push(VM,ZL_EXP_FAT_INT,0,3,0);

	if(zenglApi_Call(VM,"test2.zl","test","clsTest") == -1) //编译执行zengl脚本里的类函数
		main_exit(VM,"错误：编译<test fun call>失败：%s\n",zenglApi_GetErrorString(VM));

	zenglApi_ReUse(VM,0); //不清理虚拟内存的ReUse

	zenglApi_Push(VM,ZL_EXP_FAT_INT,0,14,0);

	zenglApi_Push(VM,ZL_EXP_FAT_FLOAT,0,0,3.14);

	if(zenglApi_Call(VM,"test2.zl","test","clsTest") == -1) //编译执行zengl脚本里的类函数
		main_exit(VM,"错误：编译<test fun call>失败：%s\n",zenglApi_GetErrorString(VM));

	zenglApi_ReUse(VM,1); //清理虚拟内存的ReUse

	if(zenglApi_Call(VM,"test2.zl","test","clsTest") == -1) //编译执行zengl脚本里的类函数
		main_exit(VM,"错误：编译<test fun call>失败：%s\n",zenglApi_GetErrorString(VM));

	zenglApi_ReUse(VM,1); //清理虚拟内存的ReUse

	zenglApi_SetModInitHandle(VM,"builtin",main_builtin_module_init);

	if(zenglApi_Run(VM,"test2.zl") == -1) //编译执行zengl脚本
		main_exit(VM,"错误：编译<test2.zl>失败：%s\n",zenglApi_GetErrorString(VM));*/

	zenglApi_Close(VM);

	main_fatal_error_free_ptrs();
	fclose(debuglog);
	printf("run finished(编译执行结束)\n");

	/*下面是脚本普通异或加密和还原的测试*/
	/*main_output_xor_source("test.zl","encrypt_script/test.zl",xor_key_str);
	main_output_xor_source("encrypt_script/test.zl","encrypt_script/test_src.zl",xor_key_str);
	main_output_xor_source("test2.zl","encrypt_script/test2.zl",xor_key_str);
	main_output_xor_source("encrypt_script/test2.zl","encrypt_script/test2_src.zl",xor_key_str);
	main_output_xor_source("test3.zl","encrypt_script/test3.zl",xor_key_str);
	main_output_xor_source("encrypt_script/test3.zl","encrypt_script/test3_src.zl",xor_key_str);*/
	/*下面是脚本RC4加密和还原的测试*/
	/*main_output_rc4_source("test.zl","encrypt_script/test.zl",rc4_key_str);
	main_output_rc4_source("encrypt_script/test.zl","encrypt_script/test_src.zl",rc4_key_str);
	main_output_rc4_source("test2.zl","encrypt_script/test2.zl",rc4_key_str);
	main_output_rc4_source("encrypt_script/test2.zl","encrypt_script/test2_src.zl",rc4_key_str);
	main_output_rc4_source("test3.zl","encrypt_script/test3.zl",rc4_key_str);
	main_output_rc4_source("encrypt_script/test3.zl","encrypt_script/test3_src.zl",rc4_key_str);*/

	#ifdef ZL_EXP_OS_IN_WINDOWS
		system("pause");
	#endif
	
	return 0;
}
