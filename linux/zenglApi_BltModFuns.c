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

/**
	该文件定义了虚拟机为用户提供的内建模块函数(只是一些很基本的脚本模块函数)，用户可以根据需要选择是否使用这些内建模块函数
	用户还可以根据此文件里的模块函数为模板，复制到自己的程序中进行修改替换。
*/

#include "zengl_global.h"

/*array内建模块函数，用于创建zengl脚本的动态数组*/
ZL_EXP_VOID zenglApiBMF_array(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount)
{
	ZENGL_EXPORT_MEMBLOCK memblock = {0};
	ZENGL_EXPORT_MOD_FUN_ARG arg = {ZL_EXP_FAT_NONE,{0}};
	ZL_EXP_INT i;
	if(argcount == 0) //如果array函数没带参数，则创建一个默认大小的未初始化的数组
	{
		if(zenglApi_CreateMemBlock(VM_ARG,&memblock,0) == -1)
			zenglApi_Exit(VM_ARG,zenglApi_GetErrorString(VM_ARG));
		zenglApi_SetRetValAsMemBlock(VM_ARG,&memblock);
	}
	else if(argcount >= 1) //如果带了参数则使用参数作为函数的初始值
	{
		if(zenglApi_CreateMemBlock(VM_ARG,&memblock,0) == -1)
			zenglApi_Exit(VM_ARG,zenglApi_GetErrorString(VM_ARG));
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

/*bltExit模块函数，直接退出zengl脚本*/
ZL_EXP_VOID zenglApiBMF_bltExit(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount)
{
	ZENGL_EXPORT_MOD_FUN_ARG arg = {ZL_EXP_FAT_NONE,{0}};
	if(argcount > 0)
	{
		zenglApi_GetFunArg(VM_ARG,1,&arg); //得到第一个参数
		if(arg.type != ZL_EXP_FAT_STR)
			zenglApi_Exit(VM_ARG,"bltExit函数的第一个参数必须是字符串，表示退出脚本时需要显示的信息");
		zenglApi_Exit(VM_ARG,arg.val.str);
	}
	else
	{
		zenglApi_Stop(VM_ARG); //如果没有参数则直接停止脚本的执行，不会产生出错信息
		return;
	}
}

/*bltConvToInt模块函数，将参数转为整数形式*/
ZL_EXP_VOID zenglApiBMF_bltConvToInt(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount)
{
	ZENGL_EXPORT_MOD_FUN_ARG arg = {ZL_EXP_FAT_NONE,{0}};
	ZL_EXP_INT ret;
	if(argcount != 1)
		zenglApi_Exit(VM_ARG,"bltConvToInt函数必须接受1个参数");
	zenglApi_GetFunArg(VM_ARG,1,&arg); //得到第一个参数
	switch(arg.type)
	{
	case ZL_EXP_FAT_FLOAT:
		ret = (ZL_EXP_INT)arg.val.floatnum;
		break;
	case ZL_EXP_FAT_STR:
		ret = atoi(arg.val.str);
		break;
	case ZL_EXP_FAT_INT:
		ret = arg.val.integer;
		break;
	default:
		zenglApi_Exit(VM_ARG,"bltConvToInt函数参数只能是整数，浮点数或字符串类型");
		break;
	}
	zenglApi_SetRetVal(VM_ARG,ZL_EXP_FAT_INT,ZL_EXP_NULL,ret,0);
}

/*	bltIntToStr模块函数：
	将整数转为字符串的形式，例如bltIntToStr(25,5,'0')那么得到的结果就是字符串'00025'
	因为第二个参数5是指总宽度，第一个参数25的宽度只有2，小于5，
	所以左边会以第三个参数'0'补齐。
	如果不想使用补齐，可以用bltIntToStr(25,0,'0')这样的格式，将第二个总宽度参数设为0，就不会应用补齐功能
*/
ZL_EXP_VOID zenglApiBMF_bltIntToStr(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount)
{
	ZENGL_EXPORT_MOD_FUN_ARG arg = {ZL_EXP_FAT_NONE,{0}};
	ZL_EXP_CHAR buf[100],dest[100];
	ZL_EXP_INT num,len,i;
	if(argcount != 3)
		zenglApi_Exit(VM_ARG,"bltIntToStr函数必须接受3个参数");
	zenglApi_GetFunArg(VM_ARG,1,&arg); //得到第一个参数
	if(arg.type != ZL_EXP_FAT_INT)
		zenglApi_Exit(VM_ARG,"bltIntToStr函数的第一个参数必须是整数，表示要进行转换的整数值");
	num = arg.val.integer;
	sprintf(buf,"%d",num);
	len = strlen(buf);
	zenglApi_GetFunArg(VM_ARG,2,&arg); //得到第二个参数
	if(arg.type != ZL_EXP_FAT_INT)
		zenglApi_Exit(VM_ARG,"bltIntToStr函数的第二个参数必须是整数，表示总宽度，当整数的宽度不足时，在左侧按第三个参数进行补充");
	num = arg.val.integer;
	zenglApi_GetFunArg(VM_ARG,3,&arg); //得到第三个参数
	if(arg.type != ZL_EXP_FAT_STR)
		zenglApi_Exit(VM_ARG,"bltIntToStr函数的第三个参数必须是字符串类型，表示要进行补充的元素");
	if(len < num)  //当第一个参数的字符串长度不足时，则用第三个参数来补齐。
	{
		for(i=0;i<num-len;i++)
			dest[i] = arg.val.str[0];
		strncpy(dest+num-len,buf,len);
		dest[num] = '\0';
	}
	if(len < num)
		zenglApi_SetRetVal(VM_ARG,ZL_EXP_FAT_STR,(char *)dest,0,0);
	else 
		zenglApi_SetRetVal(VM_ARG,ZL_EXP_FAT_STR,(char *)buf,0,0);
}

/*bltRandom模块函数，产生随机数*/
ZL_EXP_VOID zenglApiBMF_bltRandom(ZL_EXP_VOID * VM_ARG,ZL_EXP_INT argcount)
{
	time_t t;
	ZL_EXP_INT * random_seed;
	if((random_seed = zenglApi_GetExtraDataEx(VM_ARG,"ZL_SYS_ExtraData_RandomSeed")) == ZL_EXP_NULL)
	{
		random_seed = (ZL_EXP_INT *)zenglApi_AllocMem(VM_ARG,sizeof(ZL_EXP_INT));
		(*random_seed) = 0;
		zenglApi_SetExtraData(VM_ARG,"ZL_SYS_ExtraData_RandomSeed",random_seed); //使用额外数据来替代模块函数里的静态变量或全局变量，以解决多线程同时写入问题。
	}
	if((*random_seed) == 0) //第一次使用时间作为随机种子。
	{
		srand((unsigned) time(&t));
		(*random_seed) = rand();
	}
	else //其他时候使用上一次生成的随机数作为随机种子
	{
		srand((unsigned) (*random_seed));
		(*random_seed) = rand();
	}
	zenglApi_SetRetVal(VM_ARG,ZL_EXP_FAT_INT,ZL_EXP_NULL,(*random_seed),0);
}
