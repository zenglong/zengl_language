#include <string.h>
#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "md5.h"
#define ZL_EXP_OS_IN_ARM_GCC
#include "zengl_exportfuns.h"

#define MAIN_INFO_STRING_SIZE 200 // 动态字符串的初始化和动态扩容的字节大小

typedef struct{
    ZL_EXP_CHAR * function_name; // 回调函数名
    ZL_EXP_CHAR * class_name;    // 如果回调函数属于某个类，即回调函数是某个类里的方法，则该字段会记录具体的类名
    ZL_EXP_CHAR * error_string;  // 记录具体的运行时错误信息
    int default_cmd_action;      // 是否需要执行默认动作，默认情况下，会将错误信息输出到命令行，如果在脚本的回调函数里，已经将错误信息输出到了命令行的话，可以将该字段设为0，从而不会将错误信息再次输出到命令行
}FatalError_Type; // 脚本发生严重的运行时错误时，需要调用的脚本中的回调函数，脚本可以在该回调函数中获取错误信息和函数栈追踪信息，从而可以在脚本中直接处理错误信息(例如写入日志或输出到终端等)

typedef struct _strPrintEnv{
    JNIEnv* env;
    jobject obj;
    jmethodID functionID;
    FILE * debuglog;
    FatalError_Type * MainFatalError;
}strPrintEnv; // 需要传递给zengl虚拟机的额外参数

typedef struct _MAIN_INFO_STRING{
    ZL_EXP_CHAR * str;   //字符串指针
    int size;  //字符串的动态大小
    int count; //存放的字符数
    int cur;   //当前游标
} MAIN_INFO_STRING; // 动态字符串类型，该结构中的str字符串指针所指向的字符串可以根据需要，动态的扩充大小

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
static void main_fatal_error_free_ptrs(FatalError_Type * MainFatalError)
{
    if(MainFatalError->function_name != NULL) {
        free(MainFatalError->function_name);
        MainFatalError->function_name = NULL;
    }
    if(MainFatalError->class_name != NULL) {
        free(MainFatalError->class_name);
        MainFatalError->class_name = NULL;
    }
    if(MainFatalError->error_string != NULL) {
        free(MainFatalError->error_string);
        MainFatalError->error_string = NULL;
    }
}

/**
 * 将运行时错误回调函数名，类名，错误信息以及是否执行默认动作等重置为默认值
 */
static void main_fatal_error_reset_to_default_values(FatalError_Type * MainFatalError)
{
    main_fatal_error_free_ptrs(MainFatalError);
    MainFatalError->default_cmd_action = 1;
}

/**
 * 设置运行时错误回调函数名
 */
static void main_fatal_error_set_function_name(ZL_EXP_CHAR * function_name, FatalError_Type * MainFatalError)
{
    MainFatalError->function_name = main_fatal_error_copy_string(function_name, MainFatalError->function_name);
}

/**
 * 设置运行时错误回调相关的类名，如果回调函数属于某个类中定义的方法的话，就需要通过此函数来设置回调相关的类名
 */
static void main_fatal_error_set_class_name(ZL_EXP_CHAR * class_name, FatalError_Type * MainFatalError)
{
    MainFatalError->class_name = main_fatal_error_copy_string(class_name, MainFatalError->class_name);
}

/**
 * 设置运行时错误发生时，需要传递给回调函数的错误信息
 */
static void main_fatal_error_set_error_string(ZL_EXP_CHAR * error_string, FatalError_Type * MainFatalError)
{
    MainFatalError->error_string = main_fatal_error_copy_string(error_string, MainFatalError->error_string);
}

/**
 * 当脚本发生运行时错误时，如果脚本中设置过运行时错误回调函数的话，就调用该函数来处理运行时错误，
 * 同时会将错误信息和函数栈追踪信息，通过参数传递给回调函数
 */
static int main_fatal_error_callback_exec(ZL_EXP_VOID * VM, ZL_EXP_CHAR * script_file, ZL_EXP_CHAR * fatal_error, FatalError_Type * MainFatalError)
{
    MAIN_INFO_STRING debug_info = {0};
    if(MainFatalError->function_name == NULL) {
        return 0;
    }
    main_get_stack_backtrace(VM, &debug_info);
    zenglApi_ReUse(VM,0);
    zenglApi_Push(VM,ZL_EXP_FAT_STR,fatal_error,0,0);
    zenglApi_Push(VM,ZL_EXP_FAT_STR,debug_info.str,0,0);
    main_free_info_string(VM, &debug_info);
    if(zenglApi_Call(VM, script_file, MainFatalError->function_name, MainFatalError->class_name) == -1) {
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
        infoStringPtr->str = zenglApi_AllocMem(VM_ARG,(ZL_EXP_INT)(infoStringPtr->size * sizeof(ZL_EXP_CHAR)));
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
        infoStringPtr->str = zenglApi_ReAllocMem(VM_ARG, infoStringPtr->str, (ZL_EXP_INT)(infoStringPtr->size * sizeof(ZL_EXP_CHAR)));
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
 * 调用java提供的函数，将arg指向的字符串给打印显示出来
 * @param myenv
 * @param arg
 */
void java_printcall(strPrintEnv * myenv ,ZL_EXP_CHAR * arg)
{
    JNIEnv* env = myenv->env;
    jobject obj = myenv->obj;
    jmethodID functionID = myenv->functionID;
    (*env)->CallVoidMethod(env,obj,functionID,(*env)->NewStringUTF(env,arg));
}

/**
 * 将append_path路径追加到full_path中，如果追加路径后，full_path长度会超出full_path_size时，路径将会被截断
 */
static int main_full_path_append(ZL_EXP_CHAR * full_path, int full_path_length, int full_path_size, ZL_EXP_CHAR * append_path)
{
    int append_path_length = strlen(append_path);
    int max_length = full_path_size - full_path_length - 1;
    if(append_path_length > max_length)
        append_path_length = max_length;
    if(append_path_length > 0)
        strncpy((full_path + full_path_length), append_path, append_path_length);
    return append_path_length;
}

/**
 * 计算md5值
 * @param buf 用于存储根据str参数生成的md5值
 * @param str 源字符串
 * @param isLowerCase 是否生成小写的md5
 * @param is32 生成32位还是16位的md5
 */
static void main_compute_md5(ZL_EXP_CHAR * buf, ZL_EXP_CHAR * str, ZL_EXP_BOOL isLowerCase, ZL_EXP_BOOL is32)
{
    MD5_CTX md5;
    unsigned char * encrypt = (unsigned char *)str;
    unsigned char decrypt[16];
    ZL_EXP_CHAR * p;
    const ZL_EXP_CHAR * format;
    int start_idx, end_idx, i;
    MD5Init(&md5);
    MD5Update(&md5,encrypt,strlen((ZL_EXP_CHAR *)encrypt));
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
static void main_get_zengl_cache_path(ZL_EXP_CHAR * cache_path, int cache_path_size, ZL_EXP_CHAR * full_path, ZL_EXP_CHAR * script_root_dir)
{
    ZL_EXP_CHAR fullpath_md5[33];
    ZL_EXP_CHAR cache_prefix[20] = {0};
    const ZL_EXP_CHAR * cache_path_prefix = "/caches/"; // 缓存文件都放在caches目录中
    int append_length;
    main_compute_md5(fullpath_md5, full_path, ZL_EXP_TRUE, ZL_EXP_TRUE); // 将full_path进行md5编码
    // 在缓存路径前面加上zengl版本号和指针长度，不同的zengl版本生成的缓存有可能会不一样，另外，32位和64位环境下生成的内存缓存数据也是不一样的
    // 32位系统中生成的缓存数据放到64位中运行，或者反过来，都会报内存相关的错误
    sprintf(cache_prefix, "%d_%d_%d_%ld_", ZL_EXP_MAJOR_VERSION, ZL_EXP_MINOR_VERSION, ZL_EXP_REVISION, (long)sizeof(ZL_EXP_CHAR *));
    append_length = main_full_path_append(cache_path, 0, cache_path_size, (char *)script_root_dir);
    append_length += main_full_path_append(cache_path, append_length, cache_path_size, (char *)cache_path_prefix);
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
static void main_try_to_reuse_zengl_cache(ZL_EXP_VOID * VM, ZL_EXP_CHAR * cache_path, ZL_EXP_CHAR * full_path, ZL_EXP_BOOL * is_reuse_cache,
        strPrintEnv * myenv)
{
    FILE * ptr_fp;
    ZL_EXP_VOID * cachePoint;
    ZENGL_EXPORT_API_CACHE_TYPE * api_cache;
    ZL_EXP_LONG offset, cache_mtime, file_mtime;
    ZL_EXP_BYTE * mempoolPtr;
    ZL_EXP_CHAR ** filenames, * filename;
    ZL_EXP_INT cacheSize, i;
    ZL_EXP_CHAR tmp[1024];
    struct stat stat_result;
    (* is_reuse_cache) = ZL_EXP_FALSE;
    if(stat(cache_path, &stat_result)==0) { // 获取缓存文件的修改时间
        cache_mtime = (ZL_EXP_LONG)stat_result.st_mtime;
    }
    else { // 获取文件的状态信息失败，可能缓存文件不存在，需要重新编译生成缓存，直接返回
        sprintf(tmp,"stat cache file: \"%s\" failed, maybe no cache file [recompile]\n", cache_path);
        java_printcall(myenv,tmp);
        return ;
    }
    if(stat(full_path, &stat_result)==0) { // 获取主执行脚本的修改时间
        file_mtime = (ZL_EXP_LONG)stat_result.st_mtime;
        sprintf(tmp,"%s mtime:%ld", full_path, file_mtime);
        java_printcall(myenv,tmp);
        if(file_mtime >= cache_mtime) { // 如果主执行脚本的修改时间大于等于缓存数据的修改时间，则说明主执行脚本的内容发生了改变，需要重新编译生成新的缓存
            sprintf(tmp," [changed] [recompile]\n");
            java_printcall(myenv,tmp);
            return;
        }
        sprintf(tmp,"\n");
        java_printcall(myenv,tmp);
    }
    else { // 主执行脚本不存在，直接返回
        sprintf(tmp,"warning stat script file: \"%s\" failed, maybe no such file! [recompile]\n", full_path);
        java_printcall(myenv,tmp);
        return ;
    }
    // 打开缓存文件
    if((ptr_fp = fopen(cache_path, "rb")) == NULL) {
        sprintf(tmp,"no cache file: \"%s\" [recompile]\n", cache_path);
        java_printcall(myenv,tmp);
        return ;
    }
    fseek(ptr_fp,0L,SEEK_END);
    cacheSize = ftell(ptr_fp); // 得到缓存数据的大小
    fseek(ptr_fp,0L,SEEK_SET);
    cachePoint = malloc(cacheSize); // 根据缓存大小分配堆空间，先将缓存数据读取到该堆内存中
    if(fread(cachePoint, cacheSize, 1, ptr_fp) != 1) { // 读取缓存数据
        sprintf(tmp,"read cache file \"%s\" failed [recompile]\n", cache_path);
        java_printcall(myenv,tmp);
        goto end;
    }
    api_cache = (ZENGL_EXPORT_API_CACHE_TYPE *)cachePoint;
    if(api_cache->signer != ZL_EXP_API_CACHE_SIGNER) { // 根据缓存签名判断是否是有效的缓存数据
        sprintf(tmp,"invalid cache file \"%s\" [recompile]\n", cache_path);
        java_printcall(myenv,tmp);
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
            sprintf(tmp,"%s", filename);
            java_printcall(myenv,tmp);
            if(stat(filename, &stat_result)==0) {
                file_mtime = (ZL_EXP_LONG)stat_result.st_mtime;
                sprintf(tmp," mtime:%ld", file_mtime);
                java_printcall(myenv,tmp);
                if(file_mtime >= cache_mtime){
                    sprintf(tmp," [changed] [recompile]\n");
                    java_printcall(myenv,tmp);
                    goto end;
                }
            }
            else {
                sprintf(tmp," stat failed [recompile]\n");
                java_printcall(myenv,tmp);
                goto end;
            }
            sprintf(tmp,"\n");
            java_printcall(myenv,tmp);
        }
    }
    // 通过zenglApi_ReUseCacheMemData接口函数，将编译好的缓存数据加载到编译器和解释器中，这样就可以跳过编译过程，直接运行
    if(zenglApi_ReUseCacheMemData(VM, cachePoint, cacheSize) == -1) {
        sprintf(tmp,"reuse cache file \"%s\" failed: %s [recompile]\n", cache_path, zenglApi_GetErrorString(VM));
        java_printcall(myenv,tmp);
        goto end;
    }
    (* is_reuse_cache) = ZL_EXP_TRUE;
    sprintf(tmp,"reuse cache file: \"%s\" mtime:%ld\n", cache_path, cache_mtime);
    java_printcall(myenv,tmp);
    end:
    fclose(ptr_fp);
    free(cachePoint);
}

/**
 * 在编译执行结束后，生成缓存数据并写入缓存文件
 */
static void main_write_zengl_cache_to_file(ZL_EXP_VOID * VM, ZL_EXP_CHAR * cache_path, strPrintEnv * myenv)
{
    FILE * ptr_fp;
    ZL_EXP_VOID * cachePoint;
    ZL_EXP_INT cacheSize;
    ZL_EXP_CHAR tmp[1024];
    // 通过zenglApi_CacheMemData接口函数，将编译器和解释器中的主要的内存数据缓存到cachePoint对应的内存中
    if(zenglApi_CacheMemData(VM, &cachePoint, &cacheSize) == -1) {
        sprintf(tmp,"write zengl cache to file \"%s\" failed: %s\n", cache_path,zenglApi_GetErrorString(VM));
        java_printcall(myenv,tmp);
        return;
    }

    // 打开cache_path对应的缓存文件
    if((ptr_fp = fopen(cache_path, "wb")) == NULL) {
        sprintf(tmp,"write zengl cache to file \"%s\" failed: open failed\n", cache_path);
        java_printcall(myenv,tmp);
        return;
    }

    // 将缓存数据写入缓存文件
    if( fwrite(cachePoint, cacheSize, 1, ptr_fp) != 1) {
        sprintf(tmp,"write zengl cache to file \"%s\" failed: write failed\n", cache_path);
        java_printcall(myenv,tmp);
    }
    else {
        sprintf(tmp,"write zengl cache to file \"%s\" success \n", cache_path);
        java_printcall(myenv,tmp);
    }
    fclose(ptr_fp);
}

/**
 * 脚本中print语句对应的用户自定义C函数句柄
 * @param infoStrPtr print需要打印的字符串信息
 * @param infoStrCount 需要打印的字符串的字节大小
 * @param VM_ARG zengl虚拟机指针
 * @return
 */
ZL_EXP_INT run_print(ZL_EXP_CHAR * infoStrPtr, ZL_EXP_INT infoStrCount, ZL_EXP_VOID * VM_ARG)
{
    strPrintEnv * myenv = (strPrintEnv *)zenglApi_GetExtraData(VM_ARG,"extra");
    java_printcall(myenv,infoStrPtr);
    java_printcall(myenv,"\n");
    return 0;
}

/**
 * 用户自定义的处理编译信息的C函数，下面会将编译相关的信息写入日志文件，编译信息里包含了编译生成的语法树等信息
 * @param infoStrPtr 编译相关信息对应的字符串
 * @param infoStrCount 编译相关信息对应的字符串的字节大小
 * @param VM_ARG zengl虚拟机指针
 * @return
 */
ZL_EXP_INT debug_compile_info(ZL_EXP_CHAR * infoStrPtr, ZL_EXP_INT infoStrCount,ZL_EXP_VOID * VM_ARG)
{
    strPrintEnv * myenv = (strPrintEnv *)zenglApi_GetExtraData(VM_ARG,"extra");
    fprintf(myenv->debuglog,"%s",infoStrPtr);
    return 0;
}

/**
 * 用户自定义的处理运行时信息的C函数，下面会将运行时相关的信息写入日志文件，运行时信息里包含了编译生成的需要运行的虚拟汇编指令等信息
 * @param infoStrPtr 运行时信息对应的字符串
 * @param infoStrCount 运行时信息对应的字符串的字节大小
 * @param VM_ARG zengl虚拟机指针
 * @return
 */
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
                sprintf(tmp,"%ld",arg.val.integer);
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
    ZL_EXP_CHAR tmp[200];
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
                sprintf(tmp,"[%d] %ld\n",i-1,mblk_val.val.integer);
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
    strPrintEnv * myenv = (strPrintEnv *)zenglApi_GetExtraData(VM_ARG,"extra");
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
    main_fatal_error_reset_to_default_values(myenv->MainFatalError);
    main_fatal_error_set_function_name(function_name, myenv->MainFatalError);
    if(argcount > 1) {
        zenglApi_GetFunArg(VM_ARG,2,&arg);
        if(arg.type != ZL_EXP_FAT_STR) {
            zenglApi_Exit(VM_ARG,"the second argument [class_name] of %s must be string", func_name);
        }
        class_name = (ZL_EXP_CHAR *)arg.val.str;
        if(strlen(class_name) > 0) {
            main_fatal_error_set_class_name(class_name, myenv->MainFatalError);
        }
        if(argcount > 2) {
            zenglApi_GetFunArg(VM_ARG,3,&arg);
            if(arg.type != ZL_EXP_FAT_INT) {
                zenglApi_Exit(VM_ARG,"the third argument [default_cmd_action] of %s must be integer", func_name);
            }
            myenv->MainFatalError->default_cmd_action = (int)arg.val.integer;
        }
    }
    zenglApi_SetRetVal(VM_ARG, ZL_EXP_FAT_INT, ZL_EXP_NULL, 0, 0);
}

/**
 * builtin模块相关的初始化代码，里面定义了builtin模块相关的模块函数以及对应的底层C处理函数
 * @param VM_ARG zengl虚拟机指针
 * @param moduleID 当前模块的模块ID
 */
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
    zenglApi_SetModFunHandle(VM_ARG,moduleID,"bltFatalErrorCallback",main_builtin_fatal_error_callback);
}

/**
 * 执行zengl脚本的入口函数
 * @param env java传递过来的包含环境信息的指针
 * @param obj java传递过来的MainActivity对象指针
 * @param Path zengl测试脚本在手机存储中的目录路径
 * @param s 需要执行的脚本文件名
 * @param debuginfo 是否需要将zengl脚本相关的编译信息写入zengl_debuglogs.txt日志文件
 * @return
 */
JNIEXPORT jstring JNICALL
Java_com_zengl_script_MainActivity_RunZenglFromJNI( JNIEnv* env,
                                                          jobject obj,
                                                          jstring Path,
                                                          jstring s,
                                                          jint debuginfo )
{
    ZL_EXP_CHAR * run_str = "test='hello world i\\'m zengl!';";
    ZL_EXP_INT run_str_len = strlen(run_str);
    ZL_EXP_CHAR * teststr = ZL_EXP_NULL;
    ZL_EXP_BOOL is_reuse_cache;
    ZL_EXP_CHAR cache_path[1024];
    ZL_EXP_CHAR tmp_str[30];
    jstring ret = (*env)->NewStringUTF(env,""); //需要初始化为空字符串，否则返回时就容易出现 libc SIGABRT 错误
    jclass activityClass = (*env)->GetObjectClass(env,obj);
    jmethodID functionID = (*env)->GetMethodID(env,activityClass,"PrintCallBack",
                                               "(Ljava/lang/String;)V");
    FILE * debuglog = ZL_EXP_NULL;
    FatalError_Type MainFatalError = {0};
    MainFatalError.default_cmd_action = 1;
    strPrintEnv myenv = {env,obj,functionID,debuglog,&MainFatalError};
    ZL_EXP_VOID * VM = ZL_EXP_NULL;
    ZL_EXP_CHAR scriptPath[1024] = {0};
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

    // 根据脚本文件名得到缓存文件的路径信息
    main_get_zengl_cache_path(cache_path, sizeof(cache_path), scriptPath, tmpPath);
    // 尝试重利用缓存数据
    sprintf(tmp_str,"{ ************ \n");
    java_printcall(&myenv,tmp_str);
    main_try_to_reuse_zengl_cache(VM, cache_path, scriptPath, &is_reuse_cache, &myenv);
    sprintf(tmp_str," ************ }\n");
    java_printcall(&myenv,tmp_str);
    if(zenglApi_Run(VM,scriptPath) == -1) { //编译执行zengl脚本
        main_fatal_error_set_error_string(zenglApi_GetErrorString(VM), &MainFatalError);
        if(main_fatal_error_callback_exec(VM, scriptPath, MainFatalError.error_string, &MainFatalError) == -1) {
            java_printcall(&myenv,"fatal error callback failed: \n");
            java_printcall(&myenv, zenglApi_GetErrorString(VM));
        }
        else if(MainFatalError.default_cmd_action) {
            java_printcall(&myenv, MainFatalError.error_string);
        }
    }
    else if(!is_reuse_cache) { // 如果没有重利用缓存数据，则生成新的缓存数据，并将其写入缓存文件中
        sprintf(tmp_str,"{ ============ \n");
        java_printcall(&myenv,tmp_str);
        main_write_zengl_cache_to_file(VM, cache_path, &myenv);
        sprintf(tmp_str," ============ } \n");
        java_printcall(&myenv,tmp_str);
    }

    // 如果设置了运行时错误回调函数的话，就将运行时错误回调函数名，类名和错误信息相关的字符串给释放掉
    main_fatal_error_free_ptrs(&MainFatalError);

    zenglApi_Close(VM);
    if(debuglog != NULL)
        fclose(debuglog);
    return ret;
}