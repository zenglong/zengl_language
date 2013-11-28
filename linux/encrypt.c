#define ZL_EXP_OS_IN_LINUX //在加载下面的zengl头文件之前，windows系统请定义ZL_EXP_OS_IN_WINDOWS，linux , mac系统请定义ZL_EXP_OS_IN_LINUX
#include "zengl_exportfuns.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <stdarg.h>

enum ENCRYPT_TYPE{
	ENC_TYPE_NONE = 0,
	ENC_TYPE_XOR = 1,
	ENC_TYPE_RC4 = 2,
};

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
int main(int argc,char * argv[])
{
	enum ENCRYPT_TYPE isRC4 = ENC_TYPE_NONE;
	if(argc != 6)
	{
		printf("usage: %s <src> <dest> <key> -t <xor|rc4> (用法错误，应该是程序名，"
				"，src源文件名，dest要生成的目标文件名，key加密密钥，\n"
				"-t参数指定加密方式(-t参数和参数值放到其他参数的后面)，xor表示普通异或加密，rc4表示RC4加密)",argv[0]);
		#ifdef ZL_EXP_OS_IN_WINDOWS
			system("pause");
		#endif
		exit(-1);
	}

	if(strcmp(argv[4],"-t") == 0)
	{
		if(strcmp(argv[5],"xor") == 0)
			isRC4 = ENC_TYPE_XOR;
		else if(strcmp(argv[5],"rc4") == 0)
			isRC4 = ENC_TYPE_RC4;
		else
		{
			printf("加密方式只能是普通异或加密和RC4，请使用-t xor参数来表示普通异或加密方式，-t rc4参数来表示RC4方式\n");
			#ifdef ZL_EXP_OS_IN_WINDOWS
				system("pause");
			#endif
			exit(-1);
		}
	}

	if(isRC4 == ENC_TYPE_XOR)
		main_output_xor_source(argv[1],argv[2],argv[3]);
	else if(isRC4 == ENC_TYPE_RC4)
		main_output_rc4_source(argv[1],argv[2],argv[3]);
	else
		printf("您没有指定加密方式，请使用-t xor参数来表示普通异或加密方式，-t rc4参数来表示RC4方式，-t参数请放到其他参数的后面");

	#ifdef ZL_EXP_OS_IN_WINDOWS
		system("pause");
	#endif

	return 0;
}
