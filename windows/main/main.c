#define ZL_EXP_OS_IN_WINDOWS //在加载下面的zengl头文件之前，windows系统请定义ZL_EXP_OS_IN_WINDOWS，linux , mac系统请定义ZL_EXP_OS_IN_LINUX
#include "zengl_exportfuns.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <stdarg.h>

#define STRNULL '\0'

typedef struct{
	char str[50];
	int ptrIndex;	//str在内存池中分配的指针的索引
	int size;
	int count;
	int cur;
}ReadStr_Type; //字符串动态数组，用于存放用户从终端输入的信息

FILE * debuglog;
ReadStr_Type ReadStr;
int random_seed=0;

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
			printf("%d",arg.val.integer);
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
	zenglApi_SetRetVal(VM_ARG,ZL_EXP_FAT_INT,ZL_EXP_NULL,random_seed,0);
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
		}
		switch(mblk_val.type)
		{
		case ZL_EXP_FAT_INT:
			printf("[%d] %d\n",i-1,mblk_val.val.integer);
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

/**
	用户程序执行入口。
*/
int main(int argc,char * argv[])
{
	int len = 0;
	int testint;
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
	ZL_EXP_INT builtinID,sdlID;
	ZL_EXP_VOID * VM;

	if(argc < 2)
	{
		printf("usage: %s <filename> ... (用法错误，应该是：程序名 + zengl脚本文件名的形式)\n",argv[0]);
		#ifdef ZL_EXP_OS_IN_WINDOWS
			system("pause");
		#endif
		exit(-1);
	}

	printf("compiling(编译中)...\n");
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

	//zenglApi_SetSourceRC4Key(VM,rc4_key_str,rc4_key_len);

	if(zenglApi_Run(VM,argv[1]) == -1) //编译执行zengl脚本
		main_exit(VM,"错误：编译<%s>失败：%s\n",argv[1],zenglApi_GetErrorString(VM));

	if(zenglApi_GetValueAsInt(VM,"i",&testint) != -1)
		printf("after run , the i is %d\n",testint);

	zenglApi_Reset(VM);

	zenglApi_SetModInitHandle(VM,"builtin",main_builtin_module_init);

	printf("\n[next test zenglApi_RunStr]: \n");
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
		main_exit(VM,"错误：编译<test2.zl>失败：%s\n",zenglApi_GetErrorString(VM));

	zenglApi_Close(VM);

	fclose(debuglog);
	printf("compile finished(编译结束)\n");

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
