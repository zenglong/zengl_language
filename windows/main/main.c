#define ZL_EXP_OS_IN_WINDOWS //�ڼ��������zenglͷ�ļ�֮ǰ��windowsϵͳ�붨��ZL_EXP_OS_IN_WINDOWS��linux , macϵͳ�붨��ZL_EXP_OS_IN_LINUX
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
	#define MAIN_SYS_SPRINTF_ARG_NUM _vsnprintf //ʹ��va_list���������ַ����ĸ�ʽ���������������VC6�������µ�vsnprintf��Ҫ��ǰ����»��ߣ�
#else
	#define MAIN_SYS_SPRINTF_ARG_NUM vsnprintf //ʹ��va_list���������ַ����ĸ�ʽ���������
#endif

#define STRNULL '\0'
#define DEBUG_INPUT_MAX 50
#define DEF_VERSION "dv01" // ����û��Զ���ĺ�ֵ���õİ汾�ţ��ð汾�Ż�д�뻺��·���У��������û���def��ѯ������ѯ���ĺ�ֵ�����仯ʱ��ֻҪ�����汾�ţ��Ϳ��Լ�ʱ���½ű��Ļ�����

#define MAIN_INFO_STRING_SIZE 200 // ��̬�ַ����ĳ�ʼ���Ͷ�̬���ݵ��ֽڴ�С

typedef struct{
	char str[50];
	int ptrIndex;	//str���ڴ���з����ָ�������
	int size;
	int count;
	int cur;
}ReadStr_Type; //�ַ�����̬���飬���ڴ���û����ն��������Ϣ

typedef struct _MAIN_DATA{
	ZENGL_EXPORT_MEMBLOCK extra_memblock; // �����õ�����֮����ڴ��
} MAIN_DATA; // ���ݸ�zengl�ű��Ĳ����õĶ�������

typedef struct{
	ZL_EXP_CHAR * function_name; // �ص�������
	ZL_EXP_CHAR * class_name;    // ����ص���������ĳ���࣬���ص�������ĳ������ķ���������ֶλ��¼���������
	ZL_EXP_CHAR * error_string;  // ��¼���������ʱ������Ϣ
	int default_cmd_action;      // �Ƿ���Ҫִ��Ĭ�϶�����Ĭ������£��Ὣ������Ϣ����������У�����ڽű��Ļص�������Ѿ���������Ϣ������������еĻ������Խ����ֶ���Ϊ0���Ӷ����Ὣ������Ϣ�ٴ������������
}FatalError_Type; // �ű��������ص�����ʱ����ʱ����Ҫ���õĽű��еĻص��������ű������ڸûص������л�ȡ������Ϣ�ͺ���ջ׷����Ϣ���Ӷ������ڽű���ֱ�Ӵ��������Ϣ(����д����־��������ն˵�)

typedef struct _MAIN_INFO_STRING{
	ZL_EXP_CHAR * str;   //�ַ���ָ��
	int size;  //�ַ����Ķ�̬��С
	int count; //��ŵ��ַ���
	int cur;   //��ǰ�α�
} MAIN_INFO_STRING; // ��̬�ַ������ͣ��ýṹ�е�str�ַ���ָ����ָ����ַ������Ը�����Ҫ����̬�������С

FILE * debuglog;
ReadStr_Type ReadStr;
int random_seed=0;

// MainFatalErrorȫ�ֱ������ڼ�¼������������ʱ����ʱ���ű��ص������������Ϣ������ű��ص������ĺ�������������������
FatalError_Type MainFatalError = {0};

/**
 * ��ȡ����ջ׷����Ϣ���Ӷ�����֪����ǰִ�еĴ��룬�Ǵ���Щ����(���߽ű����ĸ�λ��)ִ�н�����
 */
static void main_get_stack_backtrace(ZL_EXP_VOID * VM_ARG, MAIN_INFO_STRING * debug_info);

/**
 * �ͷŶ�̬�ַ���ռ�õ��ڴ���Դ
 */
static void main_free_info_string(ZL_EXP_VOID * VM_ARG, MAIN_INFO_STRING * infoStringPtr);

/**
 * ��������ʱ����ص��������������ʹ�����Ϣ��ص��ַ�����������
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
 * ������ʱ����ص��������������ʹ�����Ϣ��ص��ַ������ͷŵ�
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
 * ������ʱ����ص���������������������Ϣ�Լ��Ƿ�ִ��Ĭ�϶���������ΪĬ��ֵ
 */
static void main_fatal_error_reset_to_default_values()
{
	main_fatal_error_free_ptrs();
	MainFatalError.default_cmd_action = 1;
}

/**
 * ��������ʱ����ص�������
 */
static void main_fatal_error_set_function_name(ZL_EXP_CHAR * function_name)
{
	MainFatalError.function_name = main_fatal_error_copy_string(function_name, MainFatalError.function_name);
}

/**
 * ��������ʱ����ص���ص�����������ص���������ĳ�����ж���ķ����Ļ�������Ҫͨ���˺��������ûص���ص�����
 */
static void main_fatal_error_set_class_name(ZL_EXP_CHAR * class_name)
{
	MainFatalError.class_name = main_fatal_error_copy_string(class_name, MainFatalError.class_name);
}

/**
 * ��������ʱ������ʱ����Ҫ���ݸ��ص������Ĵ�����Ϣ
 */
static void main_fatal_error_set_error_string(ZL_EXP_CHAR * error_string)
{
	MainFatalError.error_string = main_fatal_error_copy_string(error_string, MainFatalError.error_string);
}

/**
 * ���ű���������ʱ����ʱ������ű������ù�����ʱ����ص������Ļ����͵��øú�������������ʱ����
 * ͬʱ�Ὣ������Ϣ�ͺ���ջ׷����Ϣ��ͨ���������ݸ��ص�����
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
 * ���ַ���׷�ӵ�MAIN_INFO_STRING���Ͷ���Ķ�̬�ַ�����ĩβ��format��������Ҫ׷�ӵ��ַ����ĸ�ʽ�������ʽ���Բο�vsnprintf�⺯��������ĵ�
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
 * �ͷŶ�̬�ַ���ռ�õ��ڴ���Դ
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
 * ��ȡ����ջ׷����Ϣ���Ӷ�����֪����ǰִ�еĴ��룬�Ǵ���Щ����(���߽ű����ĸ�λ��)ִ�н�����
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
 * ��append_path·��׷�ӵ�full_path�У����׷��·����full_path���Ȼᳬ��full_path_sizeʱ��·�����ᱻ�ض�
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
 * ����full_path�ű�·�����õ�����Ҫ���ɵĻ����ļ���·����Ϣ
 */
static void main_get_zengl_cache_path(char * cache_path, int cache_path_size, char * full_path)
{
	char fullpath_md5[33];
	char cache_prefix[30] = {0};
	const char * cache_path_prefix = "caches/"; // �����ļ�������cachesĿ¼��
	int append_length;
	main_compute_md5(fullpath_md5, full_path, ZL_EXP_TRUE, ZL_EXP_TRUE); // ��full_path����md5����
	// �ڻ���·��ǰ�����zengl�汾�ź�ָ�볤�ȣ���ͬ��zengl�汾���ɵĻ����п��ܻ᲻һ�������⣬32λ��64λ���������ɵ��ڴ滺������Ҳ�ǲ�һ����
	// 32λϵͳ�����ɵĻ������ݷŵ�64λ�����У����߷����������ᱨ�ڴ���صĴ���
	sprintf(cache_prefix, "%d_%d_%d_%ld_%s_", ZL_EXP_MAJOR_VERSION, ZL_EXP_MINOR_VERSION, ZL_EXP_REVISION, sizeof(char *), DEF_VERSION);
	append_length = main_full_path_append(cache_path, 0, cache_path_size, (char *)cache_path_prefix);
	append_length += main_full_path_append(cache_path, append_length, cache_path_size, cache_prefix);
	append_length += main_full_path_append(cache_path, append_length, cache_path_size, fullpath_md5);
	cache_path[append_length] = '\0';
}

/**
 * ����������full_path�ű��ļ���Ӧ�Ļ������ݣ�cache_path��ʾ�����������ڵ��ļ�·��
 * ��������ļ������ڣ�����������ɻ����ļ������full_path�ű��ļ����ݷ����˸ı��������صĽű��ļ����ݷ����˸ı䣬Ҳ���������ɻ���
 * �ⲿ������ͨ��is_reuse_cache������ֵ���ж��Ƿ���Ҫ���ɻ����ļ������is_reuse_cacheΪZL_EXP_FALSE���ͱ�ʾû�������û��棬����Ҫ���ɻ����ļ�
 * ���is_reuse_cacheΪZL_EXP_TRUE����˵���������˻��棬����Ҫ�����ɻ����ļ���
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
	if(stat(cache_path, &stat_result)==0) { // ��ȡ�����ļ����޸�ʱ��
		cache_mtime = (ZL_EXP_LONG)stat_result.st_mtime;
	}
	else { // ��ȡ�ļ���״̬��Ϣʧ�ܣ����ܻ����ļ������ڣ���Ҫ���±������ɻ��棬ֱ�ӷ���
		printf("stat cache file: \"%s\" failed, maybe no cache file [recompile]\n", cache_path);
		return ;
	}
	if(stat(full_path, &stat_result)==0) { // ��ȡ��ִ�нű����޸�ʱ��
		file_mtime = (ZL_EXP_LONG)stat_result.st_mtime;
		printf("%s mtime:%ld", full_path, file_mtime);
		if(file_mtime >= cache_mtime) { // �����ִ�нű����޸�ʱ����ڵ��ڻ������ݵ��޸�ʱ�䣬��˵����ִ�нű������ݷ����˸ı䣬��Ҫ���±��������µĻ���
			printf(" [changed] [recompile]\n");
			return;
		}
		printf("\n");
	}
	else { // ��ִ�нű������ڣ�ֱ�ӷ���
		printf("warning stat script file: \"%s\" failed, maybe no such file! [recompile]\n", full_path);
		return ;
	}
	// �򿪻����ļ�
	if((ptr_fp = fopen(cache_path, "rb")) == NULL) {
		printf("no cache file: \"%s\" [recompile]\n", cache_path);
		return ;
	}
	fseek(ptr_fp,0L,SEEK_END);
	cacheSize = ftell(ptr_fp); // �õ��������ݵĴ�С
	fseek(ptr_fp,0L,SEEK_SET);
	cachePoint = malloc(cacheSize); // ���ݻ����С����ѿռ䣬�Ƚ��������ݶ�ȡ���ö��ڴ���
	if(fread(cachePoint, cacheSize, 1, ptr_fp) != 1) { // ��ȡ��������
		printf("read cache file \"%s\" failed [recompile]\n", cache_path);
		goto end;
	}
	api_cache = (ZENGL_EXPORT_API_CACHE_TYPE *)cachePoint;
	if(api_cache->signer != ZL_EXP_API_CACHE_SIGNER) { // ���ݻ���ǩ���ж��Ƿ�����Ч�Ļ�������
		printf("invalid cache file \"%s\" [recompile]\n", cache_path);
		goto end;
	}
	mempoolPtr = ((ZL_EXP_BYTE *)cachePoint + api_cache->mempoolOffset);
	offset = (ZL_EXP_LONG)api_cache->filenames;
	filenames = (ZL_EXP_CHAR **)(mempoolPtr + offset - 1);
	if(api_cache->filenames_count > 0) {
		// ѭ���жϼ��صĽű��ļ��������Ƿ����˸ı䣬����ı��ˣ�����Ҫ���±��������µĻ���
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
	// ͨ��zenglApi_ReUseCacheMemData�ӿں�����������õĻ������ݼ��ص��������ͽ������У������Ϳ�������������̣�ֱ������
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
 * �ڱ���ִ�н��������ɻ������ݲ�д�뻺���ļ�
 */
static void main_write_zengl_cache_to_file(ZL_EXP_VOID * VM, char * cache_path)
{
	FILE * ptr_fp;
	ZL_EXP_VOID * cachePoint;
	ZL_EXP_INT cacheSize;
	// ͨ��zenglApi_CacheMemData�ӿں��������������ͽ������е���Ҫ���ڴ����ݻ��浽cachePoint��Ӧ���ڴ���
	if(zenglApi_CacheMemData(VM, &cachePoint, &cacheSize) == -1) {
		printf("write zengl cache to file \"%s\" failed: %s\n", cache_path,zenglApi_GetErrorString(VM));
		return;
	}

	// ��cache_path��Ӧ�Ļ����ļ�
	if((ptr_fp = fopen(cache_path, "wb")) == NULL) {
		printf("write zengl cache to file \"%s\" failed: open failed\n", cache_path);
		return;
	}

	// ����������д�뻺���ļ�
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
			printf("log��־�ϵ����%s",zenglApi_GetErrorString(VM_ARG));
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
			if(i >= str_size - 10) //i�������һ��Ԫ��ʱ����str�������ݣ�str_size - 10��str������β��Ԥ��һ�οռ�
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
			printf("���������һ���ַ�\n");
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
						printf("p���Դ���%s\n",zenglApi_GetErrorString(VM_ARG));
						continue;
					}
					main_print_debug(VM_ARG,arg);
				}
				else
					printf("p����ȱ�ٲ���\n");
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
					printf("b����ȱ���ļ�������\n");
					continue;
				}
				arg = getDebugArg(str,&start,ZL_EXP_TRUE);
				if(arg != ZL_EXP_NULL && strlen(arg) > 0)
					line = atoi(arg);
				else
				{
					printf("b����ȱ���кŲ���\n");
					continue;
				}
				arg = getDebugArg(str,&start,ZL_EXP_TRUE);
				if(arg != ZL_EXP_NULL && strlen(arg) > 0)
					count = atoi(arg);
				if(zenglApi_DebugSetBreak(VM_ARG,filename,line,ZL_EXP_NULL,ZL_EXP_NULL,count,ZL_EXP_FALSE) == -1)
					printf("b����error:%s\n",zenglApi_GetErrorString(VM_ARG));
				else
					printf("���öϵ�ɹ�\n");
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
					// zenglApi_DebugGetTrace(VM_ARG,&arg,&loc,&pc,&fileName,&line,ZL_EXP_NULL,ZL_EXP_NULL); // ���л���r���ص���һ�����һ�㣬�෵��һ�㣬����ע�͵�
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
					printf("d����ȱ�ٶϵ���������\n");
					continue;
				}
				if(zenglApi_DebugDelBreak(VM_ARG,index) == -1)
					printf("d����error:��Ч�Ķϵ�����");
				else
					printf("ɾ���ϵ�ɹ�");
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
					printf("D����ȱ�ٶϵ���������\n");
					continue;
				}
				if(zenglApi_DebugGetBreak(VM_ARG,index,&filename,&line,&condition,&log,&count,&disabled,ZL_EXP_NULL) == -1)
				{
					printf("D����error:��Ч�Ķϵ�����\n");
					continue;
				}
				else
				{
					if(zenglApi_DebugSetBreak(VM_ARG,filename,line,condition,log,count,ZL_EXP_TRUE) == -1)
						printf("D������öϵ�error:%s",zenglApi_GetErrorString(VM_ARG));
					else
						printf("D������öϵ�ɹ�");
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
					printf("C����ȱ�ٶϵ���������\n");
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
					printf("C����error:��Ч�Ķϵ�����\n");
					continue;
				}
				else
				{
					if(zenglApi_DebugSetBreak(VM_ARG,filename,line,newCondition,log,count,disabled) == -1)
						printf("C�������������ϵ�error:%s",zenglApi_GetErrorString(VM_ARG));
					else
						printf("C�������������ϵ�ɹ�");
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
					printf("L����ȱ�ٶϵ���������\n");
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
					printf("L����error:��Ч�Ķϵ�����\n");
					continue;
				}
				else
				{
					if(zenglApi_DebugSetBreak(VM_ARG,filename,line,condition,newLog,count,disabled) == -1)
						printf("L����������־�ϵ�error:%s",zenglApi_GetErrorString(VM_ARG));
					else
						printf("L����������־�ϵ�ɹ�");
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
					printf("N����ȱ�ٶϵ���������\n");
					continue;
				}
				arg = getDebugArg(str,&start,ZL_EXP_TRUE);
				if(arg != ZL_EXP_NULL && strlen(arg) > 0 && main_isNumber(arg))
					newCount = atoi(arg);
				else
				{
					printf("N����ȱ�ٶϵ��������\n");
					continue;
				}
				if(zenglApi_DebugGetBreak(VM_ARG,index,&filename,&line,&condition,&log,&count,&disabled,ZL_EXP_NULL) == -1)
				{
					printf("N����error:��Ч�Ķϵ�����\n");
					continue;
				}
				else
				{
					if(zenglApi_DebugSetBreak(VM_ARG,filename,line,condition,log,newCount,disabled) == -1)
						printf("N�������öϵ����error:%s",zenglApi_GetErrorString(VM_ARG));
					else
						printf("N�������öϵ�����ɹ�");
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
			printf(" p ���Ա�����Ϣ usage:p express\n"
				" b ���öϵ� usage:b filename lineNumber\n"
				" B �鿴�ϵ��б� usage:B\n"
				" T �鿴�ű������Ķ�ջ������Ϣ usage:T\n"
				" d ɾ��ĳ�ϵ� usage:d breakIndex\n"
				" D ����ĳ�ϵ� usage:D breakIndex\n"
				" C ���������ϵ� usage:C breakIndex condition-express\n"
				" L ������־�ϵ� usage:L breakIndex log-express\n"
				" N ���öϵ���� usage:N breakIndex count\n"
				" s �������� usage:s\n"
				" S �������� usage:S\n"
				" r ִ�е����� usage:r\n"
				" c ����ִ�� usage:c\n");
			break;
		default:
			printf("��Ч������\n");
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
	���û����������д�뵽ReadStrȫ���ַ�����̬�����С�
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
	��ȡ��һ���û����������ݺ���Ҫ��ReadStr�ַ�����̬������α�ͼ���������Ϊ0�������´λ�ȡ��������ʱ���ܵõ���ȷ�����ݣ��������ܵ���һ���������ݵ�Ӱ�졣 
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

/*builtinģ�麯��*/
ZL_EXP_VOID main_builtin_printf(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount)
{
	ZENGL_EXPORT_MOD_FUN_ARG arg = {ZL_EXP_FAT_NONE,{0}};
	ZL_EXP_INT i;
	if(argcount < 1)
		zenglApi_Exit(VM_ARG,"printf�����Ĳ���������Ϊ��");
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
			zenglApi_Exit(VM_ARG,"printf������%d������������Ч��Ŀǰֻ֧���ַ��������������������͵Ĳ���",i);
			break;
		}
	}
	return;
}

/*readģ�麯��*/
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

/*bltRandomģ�麯��*/
ZL_EXP_VOID main_builtin_random(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount)
{
	time_t t;
	if(random_seed == 0) //��һ��ʹ��ʱ����Ϊ������ӡ�
	{
		srand((unsigned) time(&t));
		random_seed = rand();
	}
	else //����ʱ��ʹ����һ�����ɵ��������Ϊ�������
	{
		srand(random_seed);
		random_seed = rand();
	}
	zenglApi_SetRetVal(VM_ARG,ZL_EXP_FAT_INT,ZL_EXP_NULL,(long)random_seed,0);
}

/*arrayģ�麯�������ڴ���zengl�ű��Ķ�̬����*/
ZL_EXP_VOID main_builtin_array(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount)
{
	ZENGL_EXPORT_MEMBLOCK memblock = {0};
	ZENGL_EXPORT_MOD_FUN_ARG arg = {ZL_EXP_FAT_NONE,{0}};
	ZL_EXP_INT i;
	if(argcount == 0) //���array����û���������򴴽�һ��Ĭ�ϴ�С��δ��ʼ��������
	{
		zenglApi_CreateMemBlock(VM_ARG,&memblock,0);
		zenglApi_SetRetValAsMemBlock(VM_ARG,&memblock);
	}
	else if(argcount >= 1) //������˲�����ʹ�ò�����Ϊ�����ĳ�ʼֵ
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
				zenglApi_Exit(VM_ARG,"array������%d������������Ч",i);
				break;
			}
		}
		zenglApi_SetRetValAsMemBlock(VM_ARG,&memblock);
	}
	else
		zenglApi_Exit(VM_ARG,"array�����쳣����������С��0");
}

/*�ݹ��ӡ��������Ϣ*/
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
			for(j=0;j<recur_count;j++) printf("  "); // �ݹ���÷��غ����ǰ׺�ո��Ա����Ű�
			if(key != ZL_EXP_NULL)
				printf("[%d]{%s} <array or class obj type> end\n",i-1,key);
			else
				printf("[%d] <array or class obj type> end\n",i-1);
			break;
		}
	}
}

/*bltPrintArrayģ�麯������ӡ�����е�Ԫ��*/
ZL_EXP_VOID main_builtin_print_array(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount)
{
	ZENGL_EXPORT_MOD_FUN_ARG arg = {ZL_EXP_FAT_NONE,{0}};
	if(argcount < 1)
		zenglApi_Exit(VM_ARG,"bltPrintArray������������Ϊ�գ�����ָ��һ������������Ϊ����");
	zenglApi_GetFunArg(VM_ARG,1,&arg);
	if(arg.type != ZL_EXP_FAT_MEMBLOCK)
		zenglApi_Exit(VM_ARG,"bltPrintArray�����Ĳ�����������������");
	main_print_array(VM_ARG,arg.val.memblock,0);
}

/*bltTestAddrģ�麯��(��������)�����Բ�������*/
ZL_EXP_VOID main_builtin_test_addr(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount)
{
	ZENGL_EXPORT_MOD_FUN_ARG arg = {ZL_EXP_FAT_NONE,{0}};
	if(argcount < 2)
		zenglApi_Exit(VM_ARG,"bltTestAddr��������������������");
	zenglApi_GetFunArgInfo(VM_ARG,1,&arg);
	if(arg.type != ZL_EXP_FAT_ADDR && 
		arg.type != ZL_EXP_FAT_ADDR_LOC &&
		arg.type != ZL_EXP_FAT_ADDR_MEMBLK
		)
		zenglApi_Exit(VM_ARG,"��һ�����������Ǳ��������ã��������Ա�����ã�����������Ա������");
	zenglApi_GetFunArg(VM_ARG,1,&arg); //��ȡ��һ��������ֵ��zenglApi_GetFunArg���Եݹ����ã��ҵ����õı�����ֵ
	if(arg.type != ZL_EXP_FAT_STR)
		zenglApi_Exit(VM_ARG,"bltTestAddr����Ŀǰֻ�ܽ����ַ�����Ϊ����");
	printf("the value of first arg is %s\n",arg.val.str);
	zenglApi_GetFunArg(VM_ARG,2,&arg); //�õ��ڶ������������øò������ַ���ֵ���õ�һ���������õı���
	if(arg.type != ZL_EXP_FAT_STR)
		zenglApi_Exit(VM_ARG,"bltTestAddr����Ŀǰֻ�ܽ����ַ�����Ϊ����");
	zenglApi_SetFunArg(VM_ARG,1,&arg);
}

/*bltTestPrintByArrayKeyģ�麯��(��������)�����ڲ���zenglApi_GetMemBlockByHashKey�ӿں���*/
ZL_EXP_VOID main_builtin_test_print_by_array_key(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount)
{
	ZENGL_EXPORT_MOD_FUN_ARG arg = {ZL_EXP_FAT_NONE,{0}};
	ZENGL_EXPORT_MEMBLOCK memblock = {0};
	ZENGL_EXPORT_MOD_FUN_ARG mblk_val = {ZL_EXP_FAT_NONE,{0}};
	if(argcount != 2)
		zenglApi_Exit(VM_ARG,"usage:bltTestPrintByArrayKey(array, key)");
	zenglApi_GetFunArg(VM_ARG,1,&arg);
	if(arg.type != ZL_EXP_FAT_MEMBLOCK)
		zenglApi_Exit(VM_ARG,"bltTestPrintByArrayKey�����ĵ�һ�����������ǹ�ϣ����");
	memblock = arg.val.memblock;
	zenglApi_GetFunArg(VM_ARG,2,&arg);
	if(arg.type != ZL_EXP_FAT_STR)
		zenglApi_Exit(VM_ARG,"bltTestPrintByArrayKey�����ĵڶ��������������ַ�������ʾ�����е�key");
	// ͨ��zenglApi_GetMemBlockByHashKey�ӿڣ�ʵ����ģ�麯���У������ַ���key����ȡ����ĳ�Ա
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
		zenglApi_Exit(VM_ARG,"bltTestPrintByArrayKey������⵽��Ч������");
		break;
	}
}

/*bltGetArrayNNCountģ�麯������ȡ����֮����ڴ�����ж��ٸ���NONE���͵ĳ�Ա*/
ZL_EXP_VOID main_builtin_get_array_nncount(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount)
{
	ZENGL_EXPORT_MOD_FUN_ARG arg = {ZL_EXP_FAT_NONE,{0}};
	ZENGL_EXPORT_MEMBLOCK memblock = {0};
	ZL_EXP_INT nncount;
	if(argcount != 1)
		zenglApi_Exit(VM_ARG,"usage:bltGetArrayNNCount(array)");
	zenglApi_GetFunArg(VM_ARG,1,&arg);
	if(arg.type != ZL_EXP_FAT_MEMBLOCK)
		zenglApi_Exit(VM_ARG,"bltGetArrayNNCount�����ĵ�һ����������������");
	memblock = arg.val.memblock;
	nncount = zenglApi_GetMemBlockNNCount(VM_ARG, &memblock);
	zenglApi_SetRetVal(VM_ARG, ZL_EXP_FAT_INT, ZL_EXP_NULL, nncount, 0);
}

/*bltSetArrayģ�麯����ʹ�õ�2������3���Ȳ��������õ�һ��������Ӧ�������е�Ԫ��*/
ZL_EXP_VOID main_builtin_set_array(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount)
{
	ZENGL_EXPORT_MOD_FUN_ARG arg = {ZL_EXP_FAT_NONE,{0}};
	ZENGL_EXPORT_MEMBLOCK memblock = {0};
	ZL_EXP_INT i;
	if(argcount < 2)
		zenglApi_Exit(VM_ARG,"bltSetArray��������������������");
	zenglApi_GetFunArg(VM_ARG,1,&arg);
	if(arg.type != ZL_EXP_FAT_MEMBLOCK)
		zenglApi_Exit(VM_ARG,"bltSetArray������һ����������������������������ߵ���������");
	memblock = arg.val.memblock;
	for(i=2;i<=argcount;i++)
	{
		zenglApi_GetFunArg(VM_ARG,i,&arg);
		zenglApi_SetMemBlock(VM_ARG,&memblock,i-1,&arg);
	}
}

/*bltGetStrLenģ�麯������ȡ�ַ�������*/
ZL_EXP_VOID main_builtin_get_str_len(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount)
{
	ZENGL_EXPORT_MOD_FUN_ARG arg = {ZL_EXP_FAT_NONE,{0}};
	int str_len;
	if(argcount < 1)
		zenglApi_Exit(VM_ARG,"usage: bltGetStrLen(str)");
	zenglApi_GetFunArg(VM_ARG,1,&arg); //�õ���һ������
	if(arg.type != ZL_EXP_FAT_STR)
		zenglApi_Exit(VM_ARG,"bltGetStrLen�����ĵ�һ�������������ַ���");
	str_len = strlen(arg.val.str);
	zenglApi_SetRetVal(VM_ARG, ZL_EXP_FAT_INT, ZL_EXP_NULL, str_len, 0);
}

/**
 * bltCountģ�麯������ȡ�������Ч��Ա�������߻�ȡ�ַ�������Ч����
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

/*bltLoadScriptģ�麯�����½�һ������������ز�ִ��ĳ���ű�*/
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
	zenglApi_GetFunArg(VM_ARG,1,&arg); //��ȡ��һ������Ϊ�ű���
	if(arg.type != ZL_EXP_FAT_STR)
		zenglApi_Exit(VM_ARG,"bltLoadScript������һ�����������ַ���������Ҫ���صĽű��ļ���");
	scriptName = arg.val.str;
	zenglApi_Load(scriptName,&vm_main_args); 
}

/*bltGetZLVersionģ�麯������ȡ��ǰzengl�汾����Ϣ���ַ�����ʽ*/
ZL_EXP_VOID main_builtin_get_zl_version(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount)
{
	ZL_EXP_CHAR version[20] = {0};
	sprintf(version,"%d.%d.%d",ZL_EXP_MAJOR_VERSION,ZL_EXP_MINOR_VERSION,ZL_EXP_REVISION);
	zenglApi_SetRetVal(VM_ARG,ZL_EXP_FAT_STR,version,0,0);
}

/*bltGetExtraDataģ�麯������ȡ�û���������*/
ZL_EXP_VOID main_builtin_get_extraData(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount)
{
	ZENGL_EXPORT_MOD_FUN_ARG arg = {ZL_EXP_FAT_NONE,{0}};
	ZL_EXP_CHAR * extraName;
	if(argcount != 1)
		zenglApi_Exit(VM_ARG,"bltGetExtraData����������һ������");
	zenglApi_GetFunArg(VM_ARG,1,&arg); //��ȡ��һ������Ϊ�ű���
	if(arg.type != ZL_EXP_FAT_STR)
		zenglApi_Exit(VM_ARG,"bltGetExtraData������һ�������������ַ�������ʾ������������");
	extraName = arg.val.str;
	zenglApi_SetRetVal(VM_ARG,ZL_EXP_FAT_STR,(ZL_EXP_CHAR *)zenglApi_GetExtraData(VM_ARG,extraName),0,0);
}

/*bltGetExtraArrayģ�麯������ȡ�����õĹ�ϣ����(����������򴴽�������)������Ѿ�������ֱ�ӷ��ظ�����*/
ZL_EXP_VOID main_builtin_get_extraArray(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount)
{
	ZENGL_EXPORT_MOD_FUN_ARG arg = {ZL_EXP_FAT_NONE,{0}};
	MAIN_DATA * my_data = zenglApi_GetExtraData(VM_ARG, "my_data");

	// ���û�д�������ϣ���飬�򴴽���ϣ���飬�����ַ�����Ϊkey���������Ա�����������Ǹ���ptr�Ƿ����NULL���ж��Ƿ񴴽����ģ�
	// ��ˣ���Ҫ�ڽű�ִ��ǰ�����Ƚ�ptr��ֵ��ʾ������ΪNULL���������my_data����Դ�Ǿֲ������Ļ���ptr��Ĭ�ϳ�ʼֵ�Ͳ�����NULL
	if(my_data->extra_memblock.ptr == ZL_EXP_NULL) {
		if(zenglApi_CreateMemBlock(VM_ARG,&my_data->extra_memblock,0) == -1) {
			zenglApi_Exit(VM_ARG,zenglApi_GetErrorString(VM_ARG));
		}
		// �ֶ����Ӹ��ڴ������ü���ֵ��ʹ���ܹ��ڽű�ִ�е��������������ж�һֱ���ڣ������ᱻ�ͷŵ�
		// ������֮����ڴ���ڽű������б���ֵ���ֲ��������ڽű���������ʱ�����ͷŵ����оֲ�������Ӧ���ڴ��(����ĳ���ֲ�������return����)��
		// ͨ���ֶ������ڴ������ü���ֵ���������ű���������ʱ���ڴ������ü���ֵ�Ͳ��ᱻ��Ϊ0��Ҳ�Ͳ��ᱻ�ͷŵ�
		zenglApi_AddMemBlockRefCount(VM_ARG,&my_data->extra_memblock,1); 

		arg.type = ZL_EXP_FAT_STR;
		arg.val.str = "zengl";
		// ʹ��zenglApi_SetMemBlockByHashKey�ӿڽ��ַ�����Ϊkey���ù�ϣ����ĳ�Ա
		zenglApi_SetMemBlockByHashKey(VM_ARG, &my_data->extra_memblock, "name", &arg);

		arg.type = ZL_EXP_FAT_STR;
		arg.val.str = "programmer";
		zenglApi_SetMemBlockByHashKey(VM_ARG, &my_data->extra_memblock, "job", &arg);
		
		// �������Ĺ�ϣ������Ϊ�������
		zenglApi_SetRetValAsMemBlock(VM_ARG,&my_data->extra_memblock);
	}
	else {
		// ���֮ǰ�Ѿ���������ϣ���飬��ֱ�ӽ������鷵��
		zenglApi_SetRetValAsMemBlock(VM_ARG,&my_data->extra_memblock);
	}
}

/**
 * bltFatalErrorCallbackģ�麯�������õ���������ʱ����ʱ����Ҫ���õĽű�����������������ж���ķ�����������������ص�����
 * ��һ������function_name��ʾ��Ҫ���õĽű�������(�������ַ������ͣ��Ҳ���Ϊ���ַ���)
 * �ڶ�������class_name�ǿ�ѡ��������ʾ��Ҫ���õ�����(Ҳ�������ַ������ͣ������һ������function_name��ĳ�����ж���ķ����Ļ����Ϳ���ͨ���˲�������������)
 * 		- Ĭ��ֵ�ǿ��ַ�������ʾ����������������Ҫ�����ò������õ�����default_cmd_action����ʱ��Ҳ�����ֶ����ݿ��ַ�������ʾ����������
 * ����������default_cmd_actionҲ�ǿ�ѡ�ģ���ʾ��ִ��������ʱ����ص��������Ƿ���Ҫִ��Ĭ�ϵ����������Ϣ�������еĶ�����
 * 		- Ĭ��ֵΪ1����ʾ��Ҫִ��Ĭ�϶���������ű��ص��������Ѿ���������Ϣ������������еĻ����Ϳ��Խ��ò�������Ϊ0����ʾ����Ҫ��ִ��Ĭ�϶�����
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
	// ����������ʱ����ص�������֮ǰ���Ƚ�֮ǰ�������ù��Ļص�������صĺ�����������������ΪĬ��ֵ�����Է�ֹ�ܵ�֮ǰ���õ�Ӱ��
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

/*debug����ģ�麯��*/
ZL_EXP_VOID main_builtin_debug(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount)
{
	ZENGL_EXPORT_MOD_FUN_ARG arg = {ZL_EXP_FAT_NONE,{0}};
	ZL_EXP_CHAR * debug_str;
	if(argcount != 1)
		zenglApi_Exit(VM_ARG,"debug����������һ������");
	zenglApi_GetFunArg(VM_ARG,1,&arg); //��ȡ��һ������Ϊ�ű���
	if(arg.type != ZL_EXP_FAT_STR)
		zenglApi_Exit(VM_ARG,"debug������һ�������������ַ�������ʾ�����ַ���");
	debug_str = arg.val.str;
	if(zenglApi_Debug(VM_ARG,debug_str) == -1)
	{
		zenglApi_Exit(VM_ARG,"%s",zenglApi_GetErrorString(VM_ARG));
	}
	zenglApi_SetRetVal(VM_ARG,ZL_EXP_FAT_INT,ZL_EXP_NULL,0,0);
}

/*sdlģ�麯��*/
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
	//zenglApi_SetModFunHandle(VM_ARG,moduleID,"bltRandom",main_builtin_random); //ʹ��main.c�ж����bltRandom
	zenglApi_SetModFunHandle(VM_ARG,moduleID,"bltRandom",zenglApiBMF_bltRandom); //ʹ�������zenglApi_BltModFuns.c�ж����bltRandom
	//zenglApi_SetModFunHandle(VM_ARG,moduleID,"array",main_builtin_array); //ʹ��main.c�ж����array
	zenglApi_SetModFunHandle(VM_ARG,moduleID,"array",zenglApiBMF_array);  //ʹ�������zenglApi_BltModFuns.c�ж����array
	zenglApi_SetModFunHandle(VM_ARG,moduleID,"unset",zenglApiBMF_unset);  //ʹ�������zenglApi_BltModFuns.c�ж����unset
	zenglApi_SetModFunHandle(VM_ARG,moduleID,"bltExit",zenglApiBMF_bltExit);  //ʹ�������zenglApi_BltModFuns.c�ж����bltExit
	zenglApi_SetModFunHandle(VM_ARG,moduleID,"bltConvToInt",zenglApiBMF_bltConvToInt);  //ʹ�������zenglApi_BltModFuns.c�ж����bltConvToInt
	zenglApi_SetModFunHandle(VM_ARG,moduleID,"bltIntToStr",zenglApiBMF_bltIntToStr);  //ʹ�������zenglApi_BltModFuns.c�ж����bltIntToStr
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

/**
 * �û��Զ����def��ֵ��ѯ�������ò�ѯ��������ݲ�ѯ���Ʒ��ض�Ӧ�ĺ�ֵ
 * ���磺def TRUE ___TRUE___; �����䣬�ͻ���������������������___TRUE___��Ϊ��ѯ���ƴ��ݸ��ú�����
 * �����ھ�����ѯ�󣬾ͻ���������1��ΪTRUE�ĺ�ֵ������������Ч�� def TRUE 1;
 */
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
    ��������getFileSize(char * strFileName)  
    ���ܣ���ȡָ���ļ��Ĵ�С 
    ������ 
        strFileName (char *)���ļ��� 
    ����ֵ�� 
        size (int)���ļ���С 
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
		printf("xor_key_str��Կ����Ϊ���ַ���ָ��\n");
		return -1;
	}
	xor_key_len = strlen(xor_key_str);
	src_filesize = getFileSize(src_filename);
	src_pt = fopen(src_filename,"rb");
	if(src_pt == ZL_EXP_NULL)
	{
		printf("xor���ܵ�Դ�ű��ļ��޷���\n");
		return -1;
	}
	dest_pt = fopen(dest_filename,"wb+");
	if(dest_pt == ZL_EXP_NULL)
	{
		printf("xor���ܵ�Ŀ��ű��ļ��޷��򿪻��޷�����\n");
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

/*rc4��ʼ��״̬����*/
void rc4_init(unsigned char * state, unsigned char * key, int len)
{
   int i,j=0,t; 
   
   for (i=0; i < 256; ++i) //���������Ԫ����0��255��ʼ��
      state[i] = i; 
   for (i=0; i < 256; ++i) { //���������Ԫ��˳�����
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
	unsigned char state[256]; //rc4����������Կ����״̬����
	int i,j,t;
	int rc4_key_len;

	if(rc4_key_str == ZL_EXP_NULL)
	{
		printf("rc4_key_str��Կ����Ϊ���ַ���ָ��\n");
		return -1;
	}
	rc4_key_len = strlen(rc4_key_str);
	if(rc4_key_len > 256)
	{
		printf("rc4_key_str��Կ���Ȳ�Ҫ����256���ֽ�\n");
		return -1;
	}
	src_filesize = getFileSize(src_filename);
	src_pt = fopen(src_filename,"rb");
	if(src_pt == ZL_EXP_NULL)
	{
		printf("rc4���ܵ�Դ�ű��ļ��޷���\n");
		return -1;
	}
	dest_pt = fopen(dest_filename,"wb+");
	if(dest_pt == ZL_EXP_NULL)
	{
		printf("rc4���ܵ�Ŀ��ű��ļ��޷��򿪻��޷�����\n");
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
	�û�����ִ����ڡ�
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
		printf("usage: %s <filename> ... (�÷�����Ӧ���ǳ��������ļ�����ѡ���������ʽ���ļ���ͨ������.zlc��β��Ҳ������.zl��β)\n",argv[0]);
		#ifdef ZL_EXP_OS_IN_WINDOWS
			system("pause");
		#endif
		exit(-1);
	}

	printf("compiling(������)...\n");
	debuglog = fopen("main_debuglogs.txt","w+");
	if(zenglApi_Load(argv[1],&vm_main_args) == -1) //����ִ��zengl�ű�
	{
		printf("���󣺱���<%s>ʧ�ܣ���鿴debuglogs.txt��������ԭ��\n",argv[1]);
		#ifdef ZL_EXP_OS_IN_WINDOWS
			system("pause");
		#endif
		exit(-1);
	}
	fclose(debuglog);
	printf("compile finished(�������)\n");

	#ifdef ZL_EXP_OS_IN_WINDOWS
		system("pause");
	#endif
}*/

#ifdef ZL_EXP_OS_IN_WINDOWS
	#define stat _stat
#endif

/**
	�û�����ִ����ڡ�
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
		"bltLoadScript('test3.zl'); \n"; //��β����\n���з����������﷨����ʱ�����㶨λ�к���Ϣ
	int run_str_len = strlen(run_str);
	MAIN_DATA my_data; // �����õĶ�������
	ZL_EXP_INT builtinID,sdlID;
	ZL_EXP_VOID * VM;
	ZL_EXP_BOOL is_reuse_cache;
	char cache_path[200];

	MainFatalError.default_cmd_action = 1;

	if(argc < 2)
	{
		printf("usage: %s <filename> ... (�÷�����Ӧ���ǣ������� + zengl�ű��ļ�������ʽ���ں����-d�������Կ�������)\n",argv[0]);
		#ifdef ZL_EXP_OS_IN_WINDOWS
			system("pause");
		#endif
		exit(-1);
	}

	printf("run(����ִ����)...\n");
	debuglog = fopen("main_debuglogs.txt","w+");
	VM = zenglApi_Open();
	zenglApi_SetFlags(VM,(ZENGL_EXPORT_VM_MAIN_ARG_FLAGS)(ZL_EXP_CP_AF_IN_DEBUG_MODE | ZL_EXP_CP_AF_OUTPUT_DEBUG_INFO));
	zenglApi_SetHandle(VM,ZL_EXP_VFLAG_HANDLE_COMPILE_INFO,main_userdef_info);
	zenglApi_SetHandle(VM,ZL_EXP_VFLAG_HANDLE_RUN_INFO,main_userdef_run_info);
	zenglApi_SetHandle(VM,ZL_EXP_VFLAG_HANDLE_RUN_PRINT,main_userdef_run_print);
	//zenglApi_SetHandle(VM,ZL_EXP_VFLAG_HANDLE_MODULE_INIT,main_userdef_module_init); //Ҳ�����ڴ˴�����ģ���ʼ�����
	if((builtinID = zenglApi_SetModInitHandle(VM,"builtin",main_builtin_module_init)) == -1)
		main_exit(VM,"����useģ����ʧ��:%s",zenglApi_GetErrorString(VM));

	if((sdlID = zenglApi_SetModInitHandle(VM,"sdl",main_sdl_module_init)) == -1)
		main_exit(VM,"����useģ����ʧ��:%s",zenglApi_GetErrorString(VM));

	if(zenglApi_SetModFunHandle(VM,builtinID,"bltTest",main_builtin_printf) == -1)
		main_exit(VM,"����ģ�麯��ʧ��:%s",zenglApi_GetErrorString(VM));

	if(zenglApi_SetExtraData(VM,"name","my name is zengl") == -1)
		main_exit(VM,"���ö�������ʧ��:%s",zenglApi_GetErrorString(VM));

	if(zenglApi_SetExtraData(VM,"val","my val is zengl too") == -1)
		main_exit(VM,"���ö�������ʧ��:%s",zenglApi_GetErrorString(VM));

	my_data.extra_memblock.ptr = ZL_EXP_NULL; // ��ptr�ĳ�ʼֵ����ΪNULL(��ָ��)
	my_data.extra_memblock.index = 0;

	if(zenglApi_SetExtraData(VM,"my_data",&my_data) == -1)
		main_exit(VM,"���ö�������my_dataʧ��:%s",zenglApi_GetErrorString(VM));

	//zenglApi_SetSourceRC4Key(VM,rc4_key_str,rc4_key_len);

	if(argc >= 3 && strcmp(argv[2],"-d") == 0)
		zenglApi_DebugSetBreakHandle(VM,main_debug_break,main_debug_conditionError,ZL_EXP_TRUE,ZL_EXP_FALSE); //���õ���API

	// �����û��Զ����def��ֵ��ѯ����
	zenglApi_SetDefLookupHandle(VM, main_def_lookup_handle);

	// ���ݽű��ļ����õ������ļ���·����Ϣ
	main_get_zengl_cache_path(cache_path, sizeof(cache_path), argv[1]);
	// ���������û�������
	main_try_to_reuse_zengl_cache(VM, cache_path, argv[1], &is_reuse_cache);
	if(zenglApi_Run(VM,argv[1]) == -1) { //����ִ��zengl�ű�
		main_fatal_error_set_error_string(zenglApi_GetErrorString(VM));
		if(main_fatal_error_callback_exec(VM, argv[1], MainFatalError.error_string) == -1) {
			main_exit(VM,"\nִ�нű�<%s>��FatalError�ص�����ʧ�ܣ�\n%s\n", argv[1], zenglApi_GetErrorString(VM));
		}
		else if(MainFatalError.default_cmd_action) {
				main_exit(VM,"���󣺱���ִ��<%s>ʧ�ܣ�%s\n", argv[1], MainFatalError.error_string);
		}
	}
	else if(!is_reuse_cache) // ���û�������û������ݣ��������µĻ������ݣ�������д�뻺���ļ���
		main_write_zengl_cache_to_file(VM, cache_path);

	/*if(zenglApi_GetValueAsInt(VM,"i",&testint) != -1)
		printf("after run , the i is %ld\n",testint);

	zenglApi_Reset(VM);

	zenglApi_SetFlags(VM,(ZENGL_EXPORT_VM_MAIN_ARG_FLAGS)(ZL_EXP_CP_AF_IN_DEBUG_MODE));

	zenglApi_SetModInitHandle(VM,"builtin",main_builtin_module_init);

	printf("\n[next test zenglApi_RunStr]: \n");
	//zenglApi_DebugSetBreakHandle(VM,main_debug_break,main_debug_conditionError,ZL_EXP_TRUE,ZL_EXP_FALSE);
	if(zenglApi_RunStr(VM,run_str,run_str_len,"runstr") == -1) //����ִ���ַ����ű�
		main_exit(VM,"���󣺱���runstrʧ�ܣ�%s\n",zenglApi_GetErrorString(VM));

	printf("\n[next test zenglApi_Call and zenglApi_ReUse]: \n");

	zenglApi_Reset(VM);

	zenglApi_Push(VM,ZL_EXP_FAT_INT,0,1415,0);

	zenglApi_Push(VM,ZL_EXP_FAT_INT,0,3,0);

	if(zenglApi_Call(VM,"test2.zl","test","clsTest") == -1) //����ִ��zengl�ű�����ຯ��
		main_exit(VM,"���󣺱���<test fun call>ʧ�ܣ�%s\n",zenglApi_GetErrorString(VM));

	zenglApi_ReUse(VM,0); //�����������ڴ��ReUse

	zenglApi_Push(VM,ZL_EXP_FAT_INT,0,14,0);

	zenglApi_Push(VM,ZL_EXP_FAT_FLOAT,0,0,3.14);

	if(zenglApi_Call(VM,"test2.zl","test","clsTest") == -1) //����ִ��zengl�ű�����ຯ��
		main_exit(VM,"���󣺱���<test fun call>ʧ�ܣ�%s\n",zenglApi_GetErrorString(VM));

	zenglApi_ReUse(VM,1); //���������ڴ��ReUse

	if(zenglApi_Call(VM,"test2.zl","test","clsTest") == -1) //����ִ��zengl�ű�����ຯ��
		main_exit(VM,"���󣺱���<test fun call>ʧ�ܣ�%s\n",zenglApi_GetErrorString(VM));

	zenglApi_ReUse(VM,1); //���������ڴ��ReUse

	zenglApi_SetModInitHandle(VM,"builtin",main_builtin_module_init);

	if(zenglApi_Run(VM,"test2.zl") == -1) //����ִ��zengl�ű�
		main_exit(VM,"���󣺱���<test2.zl>ʧ�ܣ�%s\n",zenglApi_GetErrorString(VM));*/

	zenglApi_Close(VM);

	main_fatal_error_free_ptrs();
	fclose(debuglog);
	printf("run finished(����ִ�н���)\n");

	/*�����ǽű���ͨ�����ܺͻ�ԭ�Ĳ���*/
	/*main_output_xor_source("test.zl","encrypt_script/test.zl",xor_key_str);
	main_output_xor_source("encrypt_script/test.zl","encrypt_script/test_src.zl",xor_key_str);
	main_output_xor_source("test2.zl","encrypt_script/test2.zl",xor_key_str);
	main_output_xor_source("encrypt_script/test2.zl","encrypt_script/test2_src.zl",xor_key_str);
	main_output_xor_source("test3.zl","encrypt_script/test3.zl",xor_key_str);
	main_output_xor_source("encrypt_script/test3.zl","encrypt_script/test3_src.zl",xor_key_str);*/
	/*�����ǽű�RC4���ܺͻ�ԭ�Ĳ���*/
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
