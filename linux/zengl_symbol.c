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
	该文件为符号表的处理文件。
*/

#include "zengl_global.h"

/**
	hash哈希散列算法，通过节点token的标示符名字来计算出该标示符在符号hash表里的索引值。
*/
ZL_INT zengl_Sym_Hash (ZL_VOID * VM_ARG , ZL_CHAR * key )
{
	ZL_INT temp = 0;
	ZL_INT i = 0;
	while (key[i] != ZL_STRNULL)
	{ 
		temp = ((temp << ZL_SYM_HASH_SHIFT) + key[i]) % ZL_SYM_HASH_SIZE;
		++i;
	}
	return temp;
}

/*
	将line,col行列号，以及行列号所在的文件名插入到LineCols行列号动态数组中。
*/
ZL_INT zengl_insert_LineCol(ZL_VOID * VM_ARG , ZL_INT line , ZL_INT col,ZL_CHAR * filename)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZL_INT tmpindex;
	if(!compile->LineCols.isInit)
		compile->initLineColTable(VM_ARG);
	if(compile->LineCols.count == compile->LineCols.size)
	{
		compile->LineCols.size += ZL_LINECOL_SIZE;
		compile->LineCols.lines = compile->memReAlloc(VM_ARG,compile->LineCols.lines,compile->LineCols.size * sizeof(ZENGL_LINECOL));
		ZENGL_SYS_MEM_SET(compile->LineCols.lines + (compile->LineCols.size - ZL_LINECOL_SIZE),0,
						ZL_LINECOL_SIZE * sizeof(ZENGL_LINECOL));
	}
	tmpindex = compile->LineCols.count;
	compile->LineCols.lines[tmpindex].lineno = line;
	compile->LineCols.lines[tmpindex].colno = col;
	compile->LineCols.lines[tmpindex].filename = filename;
	compile->LineCols.lines[tmpindex].next = 0;
	compile->LineCols.count++;
	return tmpindex;
}

/*
	初始化LineCols行列号动态数组，用于记录符号表中每个全局符号出现过的行列号信息。
*/
ZL_VOID zengl_initLineColTable(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	if(compile->LineCols.isInit)
		return;
	compile->LineCols.count = 1;
	compile->LineCols.size = ZL_LINECOL_SIZE;
	compile->LineCols.lines = compile->memAlloc(VM_ARG,compile->LineCols.size * sizeof(ZENGL_LINECOL));
	if(compile->LineCols.lines == ZL_NULL)
		compile->exit(VM_ARG,ZL_ERR_CP_LINECOL_MALLOC_FAILED);
	ZENGL_SYS_MEM_SET(compile->LineCols.lines , 0 , compile->LineCols.size * sizeof(ZENGL_LINECOL));
	compile->LineCols.isInit = ZL_TRUE;
}

/**
	组建符号表的主程式。
	循环扫描语法树的每个节点，如果是ID标示符就将其加入符号表里。如果是FUN函数，就加入到FunTable中等
*/
ZL_VOID zengl_buildSymTable(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZL_INT tmpNodeNum = compile->AST_nodes.rootnode;
	ZL_INT count = 0;
	ZL_INT i = 0;

	compile->AST_TreeScanStackList.count = 0;
	if(compile->AST_nodes.isInit == ZL_FALSE || compile->AST_nodes.count <= 0) //如果语法树没初始化或为空，则表示里面没有任何token (可能是一个空的字符串脚本或空的脚本文件) 则什么都不做，直接返回
		return;
	while(tmpNodeNum !=0 || count == 0)
	{
		switch(nodes[tmpNodeNum].reserve)
		{
		case ZL_RSV_USE:
			break;
		case ZL_RSV_CLASS:
			compile->SymInsertHashTableForClass(VM_ARG,nodes[tmpNodeNum].childs.childnum[0]);
			break;
		case ZL_RSV_FUN:
			compile->SymInsertHashTableForFun(VM_ARG,nodes[tmpNodeNum].childs.childnum[0],0);
			break;
		default:
			compile->ScanInsertGlobalSym(VM_ARG,tmpNodeNum);
			break;
		}
		tmpNodeNum = nodes[tmpNodeNum].nextnode;
		count++;
	}
	for(i=1;i<compile->SymClassTable.count;i++)
	{
		if(compile->SymClassTable.classTable[i].isvalid)
		{
			compile->ScanInsertClassMemberSym(VM_ARG,compile->SymClassTable.classTable[i].nodenum,
								compile->SymClassTable.classTable[i].classid);
		}
	}
}

/*
	扫描并插入全局变量符号表
*/
ZL_VOID zengl_ScanInsertGlobalSym(ZL_VOID * VM_ARG,ZL_INT arg_nodenum)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_SCAN_STACK_TYPE tmpstack;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZL_INT nodenum = arg_nodenum;
	ZL_INT parentnum = -1;
	compile->push_AST_TreeScanStack(VM_ARG,nodenum); //将nodenum节点压入栈，表示将该节点下的所有子节点进行扫描。
	do{
		tmpstack = compile->pop_AST_TreeScanStack(VM_ARG,ZL_FALSE); //返回前面压入栈的节点信息。参数ZL_FALSE表示只返回信息，暂不将节点从堆栈中删除。
		if(tmpstack.nodenum < 0) //如-1之类的空节点则跳过
		{
			compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE);
			continue;
		}
		if(tmpstack.childcnt > 0 &&
		   tmpstack.curchild < tmpstack.childcnt) //如果返回的节点里包含子节点，且curchild小于childcnt子节点数时，就继续扫描子节点。
		{
			if(tmpstack.curchild < ZL_AST_CHILD_NODE_SIZE) //如果小于ZL_AST_CHILD_NODE_SIZE,就对childnum里的基本子节点进行扫描，否则就将extchilds扩展子节点里的节点进行扫描。
				nodenum = nodes[tmpstack.nodenum].childs.childnum[tmpstack.curchild];
			else
				nodenum = nodes[tmpstack.nodenum].childs.extchilds[tmpstack.curchild -
																   ZL_AST_CHILD_NODE_SIZE];
			compile->AST_TreeScanStackList.stacks[compile->AST_TreeScanStackList.count - 1].curchild++; //将当前扫描的节点索引加一，这样下次就可以扫描下一个子节点。
			compile->push_AST_TreeScanStack(VM_ARG,nodenum); //将当前扫描的子节点压入栈，这样就和之前压入栈的节点在堆栈中构成了一个节点路径。
			continue; //continue后会跳到do...while开头，然后pop_AST_TreeScanStack就会将刚压入栈的子节点弹出来。
		}
		else if(nodes[tmpstack.nodenum].toktype == ZL_TK_ID ||
				nodes[tmpstack.nodenum].toktype == ZL_TK_ARRAY_ITEM)
		{
			parentnum = nodes[tmpstack.nodenum].parentnode;
			if(nodes[parentnum].toktype == ZL_TK_DOT) //如果ID的父节点是点运算符，则交由SymInsertDotID处理
				compile->SymInsertDotID(VM_ARG,tmpstack.nodenum,parentnum);
			else
				compile->SymInsertHashTableForGlobal(VM_ARG,tmpstack.nodenum);
		}

		if(tmpstack.next != 0 && compile->AST_TreeScanStackList.count > 1) //如果当前节点有next兄弟节点且不是根节点的情况下，就将当前节点弹出，并将next节点压入栈
		{
			compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE);
			compile->push_AST_TreeScanStack(VM_ARG,tmpstack.next);
			continue; //continue后会跳到do...while开头，然后pop_AST_TreeScanStack就会将刚压入栈的子节点弹出来。
		}
		else
			compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE); //当子节点和next节点都处理完了，就将节点从堆栈中弹出。
	}while(compile->AST_TreeScanStackList.count > 0); //如果堆栈中还有元素，说明还有节点没处理完，只有当堆栈里的元素个数为0时则表示所有AST里的节点都处理完了，就可以跳出循环返回了。
}

/*
	判断点运算符里的节点是成员还是类函数调用还是全局变量，并将全局变量插入全局符号表中。
*/
ZL_VOID zengl_SymInsertDotID(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT parentNum)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZL_INT first_num,second_num;
	first_num = nodes[parentNum].childs.childnum[0];
	second_num = nodes[parentNum].childs.childnum[1];
	if(first_num != nodenum ||
	   nodes[second_num].toktype == ZL_TK_FUNCALL)
	{
		return;
	}
	compile->SymInsertHashTableForGlobal(VM_ARG,nodenum);
}

/*
	将nodenum对应的节点的符号信息先通过SymInsertGlobalTable插入到SymGlobalTable符号表对应的动态数组中，该函数返回的动态数组的索引值再保存到HashTable中。
	下次查找时就可以由节点名和hash函数算法快速找到HashTable里的索引值，再由索引值就可以找到SymGlobalTable中的某个元素了。
*/
ZL_INT zengl_SymInsertHashTableForGlobal(ZL_VOID * VM_ARG,ZL_INT nodenum)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_AST_NODE_TYPE * node = nodes + nodenum;
	ZL_CHAR * name = compile->TokenStringPoolGetPtr(VM_ARG,nodes[nodenum].strindex);
	ZL_INT h = compile->hash(VM_ARG,name) + ZL_SYM_HASH_SIZE * ZL_HASH_TYPE_SYM_TABLE;
	ZL_INT tmpindex = compile->HashTable[h];

	while(tmpindex != 0 && compile->SymGlobalTable.sym[tmpindex].isvalid == ZL_TRUE &&
		  ZENGL_SYS_STRCMP(name,compile->TokenStringPoolGetPtr(VM_ARG,compile->SymGlobalTable.sym[tmpindex].nameIndex)) != 0)
		tmpindex = compile->SymGlobalTable.sym[tmpindex].next;
	if(tmpindex == 0)
	{
		tmpindex = compile->HashTable[h];
		compile->HashTable[h] = compile->SymInsertGlobalTable(VM_ARG,nodes[nodenum].strindex,
															node->line_no,node->col_no,node->filename);
		compile->SymGlobalTable.sym[compile->HashTable[h]].next = tmpindex;
		return compile->HashTable[h];
	}
	else if(compile->SymGlobalTable.sym[tmpindex].isvalid == ZL_FALSE)
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_GLOBAL_TABLE_FIND_NOT_VALID_INDEX);
	else
	{
		tmpindex = compile->SymGlobalTable.sym[tmpindex].linecols;
		if(tmpindex == 0)
			compile->exit(VM_ARG,ZL_ERR_CP_SYM_GLOBAL_TABLE_INDEX_LINECOL_NOT_INIT);
		else
		{
			while(compile->LineCols.lines[tmpindex].next)
				tmpindex = compile->LineCols.lines[tmpindex].next;
			compile->LineCols.lines[tmpindex].next = compile->insert_LineCol(VM_ARG,node->line_no,node->col_no,node->filename);
		}
	}
	return -1;
}

/*
	将某个变量标示符插入到全局变量符号表里，并确定该变量对应的内存地址
*/
ZL_INT zengl_SymInsertGlobalTable(ZL_VOID * VM_ARG,ZL_INT nameIndex,ZL_INT line,ZL_INT col,ZL_CHAR * filename)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZL_INT tmpindex;

	if(!compile->SymGlobalTable.isInit)
		compile->SymInitGlobalTable(VM_ARG);
	if(compile->SymGlobalTable.count == compile->SymGlobalTable.size)
	{
		compile->SymGlobalTable.size += ZL_SYM_GLOBAL_TABLE_SIZE;
		compile->SymGlobalTable.sym = compile->memReAlloc(VM_ARG,compile->SymGlobalTable.sym,
														compile->SymGlobalTable.size * sizeof(ZENGL_SYM_GLOBAL_TABLE_MEMBER));
		ZENGL_SYS_MEM_SET(compile->SymGlobalTable.sym + (compile->SymGlobalTable.size - ZL_SYM_GLOBAL_TABLE_SIZE),0,
						ZL_SYM_GLOBAL_TABLE_SIZE * sizeof(ZENGL_SYM_GLOBAL_TABLE_MEMBER));
	}
	tmpindex = compile->SymGlobalTable.count;
	if(compile->SymGlobalTable.sym[tmpindex].isvalid == ZL_FALSE)
	{
		compile->SymGlobalTable.sym[tmpindex].nameIndex = nameIndex;
		compile->SymGlobalTable.sym[tmpindex].memloc = compile->SymGlobalTable.global_memloc++;
		compile->SymGlobalTable.sym[tmpindex].linecols = compile->insert_LineCol(VM_ARG,line,col,filename);
		compile->SymGlobalTable.sym[tmpindex].next = 0;
		compile->SymGlobalTable.sym[tmpindex].isvalid = ZL_TRUE;
		compile->SymGlobalTable.count++;
		return tmpindex;
	}
	else
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_GLOBAL_TABLE_NO_NOT_VALID_INDEX);
	return -1;
}

/*
	初始化全局变量符号表
*/
ZL_VOID zengl_SymInitGlobalTable(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	if(compile->SymGlobalTable.isInit)
		return;
	compile->SymGlobalTable.count = 1; //从1索引开始才是有效的索引，小于等于0都属于无效索引，方便程序做判断。
	compile->SymGlobalTable.size = ZL_SYM_GLOBAL_TABLE_SIZE;
	compile->SymGlobalTable.global_memloc = 0;
	compile->SymGlobalTable.sym = compile->memAlloc(VM_ARG,compile->SymGlobalTable.size * sizeof(ZENGL_SYM_GLOBAL_TABLE_MEMBER));
	if(compile->SymGlobalTable.sym == ZL_NULL)
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_GLOBAL_TABLE_MALLOC_FAILED);
	ZENGL_SYS_MEM_SET(compile->SymGlobalTable.sym,0,compile->SymGlobalTable.size * sizeof(ZENGL_SYM_GLOBAL_TABLE_MEMBER));
	compile->SymGlobalTable.isInit = ZL_TRUE;
}

/*
	将nodenum类名所在的节点加入到哈希表和类符号表所在的动态数组中
*/
ZL_INT zengl_SymInsertHashTableForClass(ZL_VOID * VM_ARG,ZL_INT nodenum)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_AST_NODE_TYPE * node = nodes + nodenum;
	ZL_CHAR * name = compile->TokenStringPoolGetPtr(VM_ARG,nodes[nodenum].strindex);
	ZL_INT h = compile->hash(VM_ARG,name) + ZL_SYM_HASH_SIZE * ZL_HASH_TYPE_CLASS_TABLE;
	ZL_INT tmpindex = compile->HashTable[h];

	while(tmpindex != 0 && compile->SymClassTable.classTable[tmpindex].isvalid == ZL_TRUE &&
		  ZENGL_SYS_STRCMP(name,compile->TokenStringPoolGetPtr(VM_ARG,compile->SymClassTable.classTable[tmpindex].nameIndex)) != 0)
		tmpindex = compile->SymClassTable.classTable[tmpindex].next;
	if(tmpindex == 0)
	{
		tmpindex = compile->HashTable[h];
		compile->HashTable[h] = compile->SymInsertClassTable(VM_ARG,nodes[nodenum].strindex,nodes[nodenum].parentnode,
															node->line_no,node->col_no,node->filename);
		compile->SymClassTable.classTable[compile->HashTable[h]].next = tmpindex;
		return compile->HashTable[h];
	}
	else if(compile->SymClassTable.classTable[tmpindex].isvalid == ZL_FALSE)
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_CLASS_TABLE_FIND_NOT_VALID_INDEX);
	else
	{
		compile->parser_curnode = nodenum;
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYM_CLASS_HAS_BEEN_DEFINED,
			name,compile->LineCols.lines[compile->SymClassTable.classTable[tmpindex].linecols].lineno,
			compile->LineCols.lines[compile->SymClassTable.classTable[tmpindex].linecols].colno,
			compile->LineCols.lines[compile->SymClassTable.classTable[tmpindex].linecols].filename,
			name);
	}
	return -1;
}

/*
	将类名和该类定义所在的行列号,文件名,节点号信息插入到类符号表对应的动态数组中
	并得到该类的唯一类id
*/
ZL_INT zengl_SymInsertClassTable(ZL_VOID * VM_ARG,ZL_INT nameIndex,ZL_INT nodenum,ZL_INT line,ZL_INT col,ZL_CHAR * filename)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZL_INT tmpindex;

	if(!compile->SymClassTable.isInit)
		compile->SymInitClassTable(VM_ARG);
	if(compile->SymClassTable.count == compile->SymClassTable.size)
	{
		compile->SymClassTable.size += ZL_SYM_CLASS_TABLE_SIZE;
		compile->SymClassTable.classTable = compile->memReAlloc(VM_ARG,compile->SymClassTable.classTable,
														compile->SymClassTable.size * sizeof(ZENGL_SYM_CLASS_TABLE_MEMBER));
		ZENGL_SYS_MEM_SET(compile->SymClassTable.classTable + (compile->SymClassTable.size - ZL_SYM_CLASS_TABLE_SIZE),0,
						ZL_SYM_CLASS_TABLE_SIZE * sizeof(ZENGL_SYM_CLASS_TABLE_MEMBER));
	}
	tmpindex = compile->SymClassTable.count;
	if(compile->SymClassTable.classTable[tmpindex].isvalid == ZL_FALSE)
	{
		compile->SymClassTable.classTable[tmpindex].nameIndex = nameIndex;
		compile->SymClassTable.classTable[tmpindex].nodenum = nodenum;
		compile->SymClassTable.classTable[tmpindex].classid = compile->SymClassTable.global_classid++;
		compile->SymClassTable.classTable[tmpindex].linecols = compile->insert_LineCol(VM_ARG,line,col,filename);
		compile->SymClassTable.classTable[tmpindex].next = 0;
		compile->SymClassTable.classTable[tmpindex].isvalid = ZL_TRUE;
		compile->SymClassTable.count++;
		return tmpindex;
	}
	else
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_CLASS_TABLE_NO_NOT_VALID_INDEX);
	return -1;
}

/*
	初始化类符号表对应的动态数组
*/
ZL_VOID zengl_SymInitClassTable(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	if(compile->SymClassTable.isInit)
		return;
	compile->SymClassTable.count = 1; //从1索引开始才是有效的索引，小于等于0都属于无效索引，方便程序做判断。
	compile->SymClassTable.size = ZL_SYM_CLASS_TABLE_SIZE;
	compile->SymClassTable.global_classid = 1; //类id从1开始，0是无效的类id
	compile->SymClassTable.classTable = compile->memAlloc(VM_ARG,compile->SymClassTable.size * sizeof(ZENGL_SYM_CLASS_TABLE_MEMBER));
	if(compile->SymClassTable.classTable == ZL_NULL)
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_CLASS_TABLE_MALLOC_FAILED);
	ZENGL_SYS_MEM_SET(compile->SymClassTable.classTable,0,compile->SymClassTable.size * sizeof(ZENGL_SYM_CLASS_TABLE_MEMBER));
	compile->SymClassTable.isInit = ZL_TRUE;
}

/*
	将函数节点信息插入到函数表动态数组中，并将动态数组的索引保存到HashTable中
*/
ZL_INT zengl_SymInsertHashTableForFun(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT classid)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_AST_NODE_TYPE * node = nodes + nodenum;
	ZL_CHAR * name = compile->TokenStringPoolGetPtr(VM_ARG,nodes[nodenum].strindex);
	ZL_INT h = compile->hash(VM_ARG,name) + ZL_SYM_HASH_SIZE * ZL_HASH_TYPE_FUN_TABLE;
	ZL_INT tmpindex = compile->HashTable[h];

	while(tmpindex != 0 && compile->SymFunTable.funs[tmpindex].isvalid == ZL_TRUE &&
		 (classid != compile->SymFunTable.funs[tmpindex].classid ||
		  ZENGL_SYS_STRCMP(name,compile->TokenStringPoolGetPtr(VM_ARG,compile->SymFunTable.funs[tmpindex].nameIndex)) != 0)) //循环查找同名函数或类函数，如果同名则产生语法错误
		tmpindex = compile->SymFunTable.funs[tmpindex].next;
	if(tmpindex == 0)
	{
		tmpindex = compile->HashTable[h];
		compile->HashTable[h] = compile->SymInsertFunTable(VM_ARG,nodes[nodenum].strindex,classid,
															node->line_no,node->col_no,node->filename);
		compile->SymFunTable.funs[compile->HashTable[h]].next = tmpindex;
		return compile->HashTable[h];
	}
	else if(compile->SymFunTable.funs[tmpindex].isvalid == ZL_FALSE)
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_FUN_TABLE_FIND_NOT_VALID_INDEX);
	else
	{
		compile->parser_curnode = nodenum;
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYM_FUN_HAS_BEEN_DEFINED,
			name,compile->LineCols.lines[compile->SymFunTable.funs[tmpindex].linecols].lineno,
			compile->LineCols.lines[compile->SymFunTable.funs[tmpindex].linecols].colno,
			compile->LineCols.lines[compile->SymFunTable.funs[tmpindex].linecols].filename,
			name);
	}
	return -1;
}

/*
	将函数名和函数所在的行列号等信息插入到函数符号表对应的动态数组中。
*/
ZL_INT zengl_SymInsertFunTable(ZL_VOID * VM_ARG,ZL_INT nameIndex,ZL_INT classid,ZL_INT line,ZL_INT col,ZL_CHAR * filename)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZL_INT tmpindex;

	if(!compile->SymFunTable.isInit)
		compile->SymInitFunTable(VM_ARG);
	if(compile->SymFunTable.count == compile->SymFunTable.size)
	{
		compile->SymFunTable.size += ZL_SYM_FUN_TABLE_SIZE;
		compile->SymFunTable.funs = compile->memReAlloc(VM_ARG,compile->SymFunTable.funs,
														compile->SymFunTable.size * sizeof(ZENGL_SYM_FUN_TABLE_MEMBER));
		ZENGL_SYS_MEM_SET(compile->SymFunTable.funs + (compile->SymFunTable.size - ZL_SYM_FUN_TABLE_SIZE),0,
						ZL_SYM_FUN_TABLE_SIZE * sizeof(ZENGL_SYM_FUN_TABLE_MEMBER));
	}
	tmpindex = compile->SymFunTable.count;
	if(compile->SymFunTable.funs[tmpindex].isvalid == ZL_FALSE)
	{
		compile->SymFunTable.funs[tmpindex].nameIndex = nameIndex;
		compile->SymFunTable.funs[tmpindex].funid = compile->SymFunTable.global_funid++;
		compile->SymFunTable.funs[tmpindex].classid = classid;
		compile->SymFunTable.funs[tmpindex].linecols = compile->insert_LineCol(VM_ARG,line,col,filename);
		compile->SymFunTable.funs[tmpindex].next = 0;
		compile->SymFunTable.funs[tmpindex].isvalid = ZL_TRUE;
		compile->SymFunTable.count++;
		return tmpindex;
	}
	else
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_FUN_TABLE_NO_NOT_VALID_INDEX);
	return -1;
}

/*
	初始化函数符号表对应的动态数组
*/
ZL_VOID zengl_SymInitFunTable(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	if(compile->SymFunTable.isInit)
		return;
	compile->SymFunTable.count = 1; //从1索引开始才是有效的索引，小于等于0都属于无效索引，方便程序做判断。
	compile->SymFunTable.size = ZL_SYM_FUN_TABLE_SIZE;
	compile->SymFunTable.global_funid = 1; //函数id从1开始，0是无效的函数id
	compile->SymFunTable.funs = compile->memAlloc(VM_ARG,compile->SymFunTable.size * sizeof(ZENGL_SYM_FUN_TABLE_MEMBER));
	if(compile->SymFunTable.funs == ZL_NULL)
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_FUN_TABLE_MALLOC_FAILED);
	ZENGL_SYS_MEM_SET(compile->SymFunTable.funs,0,compile->SymFunTable.size * sizeof(ZENGL_SYM_FUN_TABLE_MEMBER));
	compile->SymFunTable.isInit = ZL_TRUE;
}

/*
	扫描类成员，并将类成员加入类成员符号表，以及将类函数加入函数符号表中。
*/
ZL_VOID zengl_ScanInsertClassMemberSym(ZL_VOID * VM_ARG,ZL_INT arg_nodenum,ZL_INT classid)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_SCAN_STACK_TYPE tmpstack;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZL_INT nodenum = arg_nodenum;
	ZL_INT cls_stmt_classid = 0;
	ZL_INT classmember_index = 0;
	compile->push_AST_TreeScanStack(VM_ARG,nodenum); //将nodenum节点压入栈，表示将该节点下的所有子节点进行扫描。
	compile->AST_TreeScanStackList.stacks[compile->AST_TreeScanStackList.count - 1].curchild = 1; //跳过类名节点
	do{
		tmpstack = compile->pop_AST_TreeScanStack(VM_ARG,ZL_FALSE); //返回前面压入栈的节点信息。参数ZL_FALSE表示只返回信息，暂不将节点从堆栈中删除。
		if(tmpstack.nodenum < 0) //如-1之类的空节点则跳过
		{
			compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE);
			continue;
		}
		if(tmpstack.childcnt > 0 &&
		   tmpstack.curchild < tmpstack.childcnt) //如果返回的节点里包含子节点，且curchild小于childcnt子节点数时，就继续扫描子节点。
		{
			if(nodes[tmpstack.nodenum].toktype == ZL_TK_CLASS_STATEMENT)
				cls_stmt_classid = compile->SymLookupClass(VM_ARG,tmpstack.nodenum);
			else if(nodes[tmpstack.nodenum].toktype == ZL_TK_RESERVE && nodes[tmpstack.nodenum].reserve == ZL_RSV_FUN)
			{
				compile->SymInsertHashTableForFun(VM_ARG,nodes[tmpstack.nodenum].childs.childnum[0],classid);
				compile->AST_TreeScanStackList.stacks[compile->AST_TreeScanStackList.count - 1].curchild = tmpstack.childcnt; //跳过class里的fun，因为类函数里的变量信息会在其他扫描函数中处理
				continue;
			}

			if(tmpstack.curchild < ZL_AST_CHILD_NODE_SIZE) //如果小于ZL_AST_CHILD_NODE_SIZE,就对childnum里的基本子节点进行扫描，否则就将extchilds扩展子节点里的节点进行扫描。
				nodenum = nodes[tmpstack.nodenum].childs.childnum[tmpstack.curchild];
			else
				nodenum = nodes[tmpstack.nodenum].childs.extchilds[tmpstack.curchild -
																   ZL_AST_CHILD_NODE_SIZE];
			compile->AST_TreeScanStackList.stacks[compile->AST_TreeScanStackList.count - 1].curchild++; //将当前扫描的节点索引加一，这样下次就可以扫描下一个子节点。
			compile->push_AST_TreeScanStack(VM_ARG,nodenum); //将当前扫描的子节点压入栈，这样就和之前压入栈的节点在堆栈中构成了一个节点路径。
			continue; //continue后会跳到do...while开头，然后pop_AST_TreeScanStack就会将刚压入栈的子节点弹出来。
		}
		else if(nodes[tmpstack.nodenum].toktype == ZL_TK_ID)
		{
			compile->SymInsertHashTableForClassMember(VM_ARG,tmpstack.nodenum,classmember_index++,classid,cls_stmt_classid);
		}
		else if(nodes[tmpstack.nodenum].toktype == ZL_TK_CLASS_STATEMENT)
		{
			cls_stmt_classid = 0;
		}

		if(tmpstack.next != 0 && compile->AST_TreeScanStackList.count > 1) //如果当前节点有next兄弟节点且不是根节点的情况下，就将当前节点弹出，并将next节点压入栈
		{
			compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE);
			compile->push_AST_TreeScanStack(VM_ARG,tmpstack.next);
			continue; //continue后会跳到do...while开头，然后pop_AST_TreeScanStack就会将刚压入栈的子节点弹出来。
		}
		else
			compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE); //当子节点和next节点都处理完了，就将节点从堆栈中弹出。
	}while(compile->AST_TreeScanStackList.count > 0); //如果堆栈中还有元素，说明还有节点没处理完，只有当堆栈里的元素个数为0时则表示所有AST里的节点都处理完了，就可以跳出循环返回了。
}

/*
	根据节点号查找类ID信息
*/
ZL_INT zengl_SymLookupClass(ZL_VOID * VM_ARG,ZL_INT nodenum)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZL_CHAR * name = compile->TokenStringPoolGetPtr(VM_ARG,nodes[nodenum].strindex);
	ZL_INT h = compile->hash(VM_ARG,name) + ZL_SYM_HASH_SIZE * ZL_HASH_TYPE_CLASS_TABLE;
	ZL_INT tmpindex = compile->HashTable[h];
	while(tmpindex != 0 && compile->SymClassTable.classTable[tmpindex].isvalid == ZL_TRUE &&
		  ZENGL_SYS_STRCMP(name,compile->TokenStringPoolGetPtr(VM_ARG,compile->SymClassTable.classTable[tmpindex].nameIndex)) != 0)
		tmpindex = compile->SymClassTable.classTable[tmpindex].next;
	if(tmpindex == 0)
	{
		compile->parser_curnode = nodenum;
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_CLASS_NAME_OF_CLS_STMT_NOT_EXISTS,name,name);
	}
	else if(compile->SymClassTable.classTable[tmpindex].isvalid == ZL_FALSE)
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_CLASS_TABLE_FIND_NOT_VALID_INDEX_WHEN_LOOKUP);
	return compile->SymClassTable.classTable[tmpindex].classid;
}

/*
	根据类名查找类ID信息
*/
ZL_INT zengl_SymLookupClassByName(ZL_VOID * VM_ARG,ZL_CHAR * name)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZL_INT h = compile->hash(VM_ARG,name) + ZL_SYM_HASH_SIZE * ZL_HASH_TYPE_CLASS_TABLE;
	ZL_INT tmpindex = compile->HashTable[h];
	while(tmpindex != 0 && compile->SymClassTable.classTable[tmpindex].isvalid == ZL_TRUE &&
		  ZENGL_SYS_STRCMP(name,compile->TokenStringPoolGetPtr(VM_ARG,compile->SymClassTable.classTable[tmpindex].nameIndex)) != 0)
		tmpindex = compile->SymClassTable.classTable[tmpindex].next;
	if(tmpindex == 0)
		return 0;
	else if(compile->SymClassTable.classTable[tmpindex].isvalid == ZL_FALSE)
		return -1;
	return compile->SymClassTable.classTable[tmpindex].classid;
}

/*
	从SymClassMemberTable中查找parent_classid对应的类的成员nodenum的信息
*/
ZL_INT zengl_SymLookupClassMember(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT parent_classid)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_AST_NODE_TYPE * node = nodes + nodenum;
	ZL_CHAR * name = compile->TokenStringPoolGetPtr(VM_ARG,nodes[nodenum].strindex);
	ZL_INT h = compile->hash(VM_ARG,name) + ZL_SYM_HASH_SIZE * ZL_HASH_TYPE_CLASSMEMBER_TABLE;
	ZL_INT tmpindex = compile->HashTable[h];

	while(tmpindex != 0 && compile->SymClassMemberTable.members[tmpindex].isvalid == ZL_TRUE &&
		 (compile->SymClassMemberTable.members[tmpindex].classid != parent_classid || 
		  ZENGL_SYS_STRCMP(name,compile->TokenStringPoolGetPtr(VM_ARG,compile->SymClassMemberTable.members[tmpindex].nameIndex)) != 0))
		tmpindex = compile->SymClassMemberTable.members[tmpindex].next;
	if(tmpindex == 0)
	{
		compile->parser_curnode = nodenum;
		compile->parser_errorExit(VM_ARG,ZL_ERR_SYNTAX_INVALID_CLASS_MEMBER);
	}
	else if(compile->SymClassMemberTable.members[tmpindex].isvalid == ZL_FALSE)
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_CLASSMEMBER_TABLE_FIND_NOT_VALID_INDEX);
	else
	{
		return tmpindex;
	}
	return -1;
}

/*
	将nodenum类成员所在的节点加入到哈希表和类成员符号表所在的动态数组中
*/
ZL_INT zengl_SymInsertHashTableForClassMember(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT index,ZL_INT classid,ZL_INT cls_stmt_classid)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_AST_NODE_TYPE * node = nodes + nodenum;
	ZL_CHAR * name = compile->TokenStringPoolGetPtr(VM_ARG,nodes[nodenum].strindex);
	ZL_INT h = compile->hash(VM_ARG,name) + ZL_SYM_HASH_SIZE * ZL_HASH_TYPE_CLASSMEMBER_TABLE;
	ZL_INT tmpindex = compile->HashTable[h];

	while(tmpindex != 0 && compile->SymClassMemberTable.members[tmpindex].isvalid == ZL_TRUE &&
		 (compile->SymClassMemberTable.members[tmpindex].classid != classid || 
		  ZENGL_SYS_STRCMP(name,compile->TokenStringPoolGetPtr(VM_ARG,compile->SymClassMemberTable.members[tmpindex].nameIndex)) != 0))
		tmpindex = compile->SymClassMemberTable.members[tmpindex].next;
	if(tmpindex == 0)
	{
		tmpindex = compile->HashTable[h];
		compile->HashTable[h] = compile->SymInsertClassMemberTable(VM_ARG,nodes[nodenum].strindex,index,classid,cls_stmt_classid,nodenum);
		compile->SymClassMemberTable.members[compile->HashTable[h]].next = tmpindex;
		return compile->HashTable[h];
	}
	else if(compile->SymClassMemberTable.members[tmpindex].isvalid == ZL_FALSE)
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_CLASSMEMBER_TABLE_FIND_NOT_VALID_INDEX);
	else
	{
		compile->parser_curnode = nodenum;
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_CLASSMEMBER_HAS_BEEN_DEFINED,
			name,compile->TokenStringPoolGetPtr(VM_ARG,compile->SymClassTable.classTable[classid].nameIndex),
			nodes[compile->SymClassMemberTable.members[tmpindex].nodenum].line_no,
			nodes[compile->SymClassMemberTable.members[tmpindex].nodenum].col_no,
			nodes[compile->SymClassMemberTable.members[tmpindex].nodenum].filename,
			name);
	}
	return -1;
}

/*
	将类成员名，类成员在类结构中的索引值，所属类ID等信息加入类成员符号表中
*/
ZL_INT zengl_SymInsertClassMemberTable(ZL_VOID * VM_ARG,ZL_INT nameIndex,ZL_INT index,ZL_INT classid,ZL_INT cls_stmt_classid,ZL_INT nodenum)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZL_INT tmpindex;

	if(!compile->SymClassMemberTable.isInit)
		compile->SymInitClassMemberTable(VM_ARG);
	if(compile->SymClassMemberTable.count == compile->SymClassMemberTable.size)
	{
		compile->SymClassMemberTable.size += ZL_SYM_CLASSMEMBER_TABLE_SIZE;
		compile->SymClassMemberTable.members = compile->memReAlloc(VM_ARG,compile->SymClassMemberTable.members,
															compile->SymClassMemberTable.size * sizeof(ZENGL_SYM_CLASSMEMBER_TABLE_MEMBER));
		ZENGL_SYS_MEM_SET(compile->SymClassMemberTable.members + (compile->SymClassMemberTable.size - ZL_SYM_CLASSMEMBER_TABLE_SIZE),0,
						ZL_SYM_CLASSMEMBER_TABLE_SIZE * sizeof(ZENGL_SYM_CLASSMEMBER_TABLE_MEMBER));
	}
	tmpindex = compile->SymClassMemberTable.count;
	if(compile->SymClassMemberTable.members[tmpindex].isvalid == ZL_FALSE)
	{
		compile->SymClassMemberTable.members[tmpindex].nameIndex = nameIndex;
		compile->SymClassMemberTable.members[tmpindex].index = index;
		compile->SymClassMemberTable.members[tmpindex].nodenum = nodenum;
		compile->SymClassMemberTable.members[tmpindex].classid = classid;
		compile->SymClassMemberTable.members[tmpindex].cls_stmt_classid = cls_stmt_classid;
		compile->SymClassMemberTable.members[tmpindex].next = 0;
		compile->SymClassMemberTable.members[tmpindex].isvalid = ZL_TRUE;
		compile->SymClassMemberTable.count++;
		return tmpindex;
	}
	else
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_CLASSMEMBER_TABLE_NO_NOT_VALID_INDEX);
	return -1;
}

/*
	初始化类成员符号表对应的动态数组
*/
ZL_VOID zengl_SymInitClassMemberTable(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	if(compile->SymClassMemberTable.isInit)
		return;
	compile->SymClassMemberTable.count = 1; //从1索引开始才是有效的索引，小于等于0都属于无效索引，方便程序做判断。
	compile->SymClassMemberTable.size = ZL_SYM_CLASSMEMBER_TABLE_SIZE;
	compile->SymClassMemberTable.members = compile->memAlloc(VM_ARG,compile->SymClassMemberTable.size * sizeof(ZENGL_SYM_CLASSMEMBER_TABLE_MEMBER));
	if(compile->SymClassMemberTable.members == ZL_NULL)
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_CLASSMEMBER_TABLE_MALLOC_FAILED);
	ZENGL_SYS_MEM_SET(compile->SymClassMemberTable.members,0,compile->SymClassMemberTable.size * sizeof(ZENGL_SYM_CLASSMEMBER_TABLE_MEMBER));
	compile->SymClassMemberTable.isInit = ZL_TRUE;
}

/**
	通过节点索引来查找该节点的变量标示符在自定义的虚拟内存中的内存地址，并返回内存地址的字符串格式，例如：内存地址1则返回“(1)”	
*/
ZENGL_RUN_INST_OP_DATA zengl_SymLookupID(ZL_VOID * VM_ARG,ZL_INT nodenum)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_AST_NODE_TYPE * node = nodes + nodenum;
	ZL_CHAR * name = compile->TokenStringPoolGetPtr(VM_ARG,nodes[nodenum].strindex);
	ZL_INT h;
	ZL_INT tmpindex;
	ZL_BOOL is_local_global = ZL_FALSE;
	ZENGL_RUN_INST_OP_DATA ret_data;
	
	if(!compile->gencode_struct.is_inFun)
	{
FindGlobalSymLoc:
		h = compile->hash(VM_ARG,name) + ZL_SYM_HASH_SIZE * ZL_HASH_TYPE_SYM_TABLE;
		tmpindex = compile->HashTable[h];
		while(tmpindex != 0 && compile->SymGlobalTable.sym[tmpindex].isvalid == ZL_TRUE &&
			  ZENGL_SYS_STRCMP(name,compile->TokenStringPoolGetPtr(VM_ARG,compile->SymGlobalTable.sym[tmpindex].nameIndex)) != 0)
			tmpindex = compile->SymGlobalTable.sym[tmpindex].next;
		if(is_local_global == ZL_TRUE && tmpindex == 0)
		{
			tmpindex = compile->SymInsertHashTableForGlobal(VM_ARG,nodenum); //如果是fun函数定义里通过global声明的新的全局变量，就用SymInsertHashTableForGlobal向全局变量HASH表中插入一个新的全局变量。
			goto ReturnGlobalSymInfo;
		}
		else if(tmpindex == 0)
		{
			compile->parser_curnode = nodenum;
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYM_CAN_NOT_FIND_ID_MEMLOC,name,name);
		}
		else if(compile->SymGlobalTable.sym[tmpindex].isvalid == ZL_FALSE)
			compile->exit(VM_ARG,ZL_ERR_CP_SYM_GLOBAL_TABLE_FIND_NOT_VALID_INDEX_WHEN_LOOKUP);
ReturnGlobalSymInfo:
		ret_data.type = ZL_R_DT_MEM;
		ret_data.val.mem = compile->SymGlobalTable.sym[tmpindex].memloc;
		return ret_data;
	}
	else
	{
		h = compile->hash(VM_ARG,name) + ZL_SYM_HASH_SIZE * ZL_HASH_TYPE_LOCAL_TABLE;
		tmpindex = compile->HashTable[h];
		while(tmpindex != 0 && compile->SymLocalTable.local[tmpindex].isvalid == ZL_TRUE && 
			(ZENGL_SYS_STRCMP(name,compile->TokenStringPoolGetPtr(VM_ARG,compile->SymLocalTable.local[tmpindex].nameIndex)) != 0 || 
			compile->SymLocalTable.local[tmpindex].funid != compile->SymFunTable.global_funid))
		{
			tmpindex = compile->SymLocalTable.local[tmpindex].next;
		}
		if(tmpindex == 0)
		{
			compile->parser_curnode = nodenum;
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYM_CAN_NOT_FIND_LOCAL_ID_MEMLOC,name,name);
		}
		else if(compile->SymLocalTable.local[tmpindex].isvalid == ZL_FALSE)
			compile->exit(VM_ARG,ZL_ERR_CP_SYM_LOCAL_TABLE_INVALID_INDEX);
		else if(compile->SymLocalTable.local[tmpindex].funid != compile->SymFunTable.global_funid)
			compile->exit(VM_ARG,ZL_ERR_CP_SYM_LOCAL_ID_INVALID_FUN_ID);
		switch(compile->SymLocalTable.local[tmpindex].type)
		{
		case ZL_SYM_ENUM_LOCAL_TYPE_ARG:
			ret_data.type = ZL_R_DT_ARGMEM;
			ret_data.val.mem = compile->SymLocalTable.local[tmpindex].localid;
			break;
		case ZL_SYM_ENUM_LOCAL_TYPE_LOCAL:
			ret_data.type = ZL_R_DT_LOCMEM;
			ret_data.val.mem = compile->SymLocalTable.local[tmpindex].localid;
			break;
		case ZL_SYM_ENUM_LOCAL_TYPE_GLOBAL:
			is_local_global = ZL_TRUE;
			goto FindGlobalSymLoc;
			break;
		default:
			compile->exit(VM_ARG,ZL_ERR_CP_SYM_LOCAL_ID_INVALID_TYPE);
			break;
		}
		return ret_data;
	}
	return ret_data;
}

/**
	为nodenum节点设置classid类id	
*/
ZL_VOID zengl_SymInsertClassID(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT classid)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_AST_NODE_TYPE * node = nodes + nodenum;
	ZL_CHAR * name = compile->TokenStringPoolGetPtr(VM_ARG,nodes[nodenum].strindex);
	ZL_INT h;
	ZL_INT tmpindex;
	ZL_BOOL is_local_global = ZL_FALSE;
	
	if(!compile->gencode_struct.is_inFun)
	{
FindGlobalSymLoc:
		h = compile->hash(VM_ARG,name) + ZL_SYM_HASH_SIZE * ZL_HASH_TYPE_SYM_TABLE;
		tmpindex = compile->HashTable[h];
		while(tmpindex != 0 && compile->SymGlobalTable.sym[tmpindex].isvalid == ZL_TRUE &&
			  ZENGL_SYS_STRCMP(name,compile->TokenStringPoolGetPtr(VM_ARG,compile->SymGlobalTable.sym[tmpindex].nameIndex)) != 0)
			tmpindex = compile->SymGlobalTable.sym[tmpindex].next;
		if(is_local_global == ZL_TRUE && tmpindex == 0)
		{
			tmpindex = compile->SymInsertHashTableForGlobal(VM_ARG,nodenum); //如果是fun函数定义里通过global声明的新的全局变量，就用SymInsertHashTableForGlobal向全局变量HASH表中插入一个新的全局变量。
			goto ReturnGlobalSymInfo;
		}
		else if(tmpindex == 0)
		{
			compile->parser_curnode = nodenum;
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYM_CAN_NOT_FIND_ID_MEMLOC,name,name);
		}
		else if(compile->SymGlobalTable.sym[tmpindex].isvalid == ZL_FALSE)
			compile->exit(VM_ARG,ZL_ERR_CP_SYM_GLOBAL_TABLE_FIND_NOT_VALID_INDEX_WHEN_LOOKUP);
ReturnGlobalSymInfo:
		compile->SymGlobalTable.sym[tmpindex].classid = classid;
		return ;
	}
	else
	{
		h = compile->hash(VM_ARG,name) + ZL_SYM_HASH_SIZE * ZL_HASH_TYPE_LOCAL_TABLE;
		tmpindex = compile->HashTable[h];
		while(tmpindex != 0 && compile->SymLocalTable.local[tmpindex].isvalid == ZL_TRUE && 
			(ZENGL_SYS_STRCMP(name,compile->TokenStringPoolGetPtr(VM_ARG,compile->SymLocalTable.local[tmpindex].nameIndex)) != 0 || 
			compile->SymLocalTable.local[tmpindex].funid != compile->SymFunTable.global_funid))
		{
			tmpindex = compile->SymLocalTable.local[tmpindex].next;
		}
		if(tmpindex == 0)
		{
			compile->parser_curnode = nodenum;
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYM_CAN_NOT_FIND_LOCAL_ID_MEMLOC,name,name);
		}
		else if(compile->SymLocalTable.local[tmpindex].isvalid == ZL_FALSE)
			compile->exit(VM_ARG,ZL_ERR_CP_SYM_LOCAL_TABLE_INVALID_INDEX);
		else if(compile->SymLocalTable.local[tmpindex].funid != compile->SymFunTable.global_funid)
			compile->exit(VM_ARG,ZL_ERR_CP_SYM_LOCAL_ID_INVALID_FUN_ID);
		switch(compile->SymLocalTable.local[tmpindex].type)
		{
		case ZL_SYM_ENUM_LOCAL_TYPE_ARG:
		case ZL_SYM_ENUM_LOCAL_TYPE_LOCAL:
			compile->SymLocalTable.local[tmpindex].classid = classid;
			break;
		case ZL_SYM_ENUM_LOCAL_TYPE_GLOBAL:
			is_local_global = ZL_TRUE;
			goto FindGlobalSymLoc;
			break;
		default:
			compile->exit(VM_ARG,ZL_ERR_CP_SYM_LOCAL_ID_INVALID_TYPE);
			break;
		}
		return ;
	}
	return ;
}

/*
    查找nodenum对应节点的classid值，主要用于生成点运算符的汇编指令时
*/
ZL_INT zengl_SymLookupID_ForDot(ZL_VOID * VM_ARG,ZL_INT nodenum)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_AST_NODE_TYPE * node = nodes + nodenum;
	ZL_CHAR * name = compile->TokenStringPoolGetPtr(VM_ARG,nodes[nodenum].strindex);
	ZL_INT h;
	ZL_INT tmpindex;
	
	if(!compile->gencode_struct.is_inFun)
	{
FindGlobalSymLoc:
		h = compile->hash(VM_ARG,name) + ZL_SYM_HASH_SIZE * ZL_HASH_TYPE_SYM_TABLE;
		tmpindex = compile->HashTable[h];
		while(tmpindex != 0 && compile->SymGlobalTable.sym[tmpindex].isvalid == ZL_TRUE &&
			  ZENGL_SYS_STRCMP(name,compile->TokenStringPoolGetPtr(VM_ARG,compile->SymGlobalTable.sym[tmpindex].nameIndex)) != 0)
			tmpindex = compile->SymGlobalTable.sym[tmpindex].next;
		if(tmpindex == 0)
		{
			compile->parser_curnode = nodenum;
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYM_CAN_NOT_FIND_ID_MEMLOC,name,name);
		}
		else if(compile->SymGlobalTable.sym[tmpindex].isvalid == ZL_FALSE)
			compile->exit(VM_ARG,ZL_ERR_CP_SYM_GLOBAL_TABLE_FIND_NOT_VALID_INDEX_WHEN_LOOKUP);
		else
		{
			compile->memDataForDot.type = ZL_R_DT_MEM;
			compile->memDataForDot.val.mem = compile->SymGlobalTable.sym[tmpindex].memloc;
			return compile->SymGlobalTable.sym[tmpindex].classid;
		}
	}
	else
	{
		h = compile->hash(VM_ARG,name) + ZL_SYM_HASH_SIZE * ZL_HASH_TYPE_LOCAL_TABLE;
		tmpindex = compile->HashTable[h];
		while(tmpindex != 0 && compile->SymLocalTable.local[tmpindex].isvalid == ZL_TRUE && 
			(ZENGL_SYS_STRCMP(name,compile->TokenStringPoolGetPtr(VM_ARG,compile->SymLocalTable.local[tmpindex].nameIndex)) != 0 || 
			compile->SymLocalTable.local[tmpindex].funid != compile->SymFunTable.global_funid))
		{
			tmpindex = compile->SymLocalTable.local[tmpindex].next;
		}
		if(tmpindex == 0)
		{
			compile->parser_curnode = nodenum;
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYM_CAN_NOT_FIND_LOCAL_ID_MEMLOC,name,name);
		}
		else if(compile->SymLocalTable.local[tmpindex].isvalid == ZL_FALSE)
			compile->exit(VM_ARG,ZL_ERR_CP_SYM_LOCAL_TABLE_INVALID_INDEX);
		else if(compile->SymLocalTable.local[tmpindex].funid != compile->SymFunTable.global_funid)
			compile->exit(VM_ARG,ZL_ERR_CP_SYM_LOCAL_ID_INVALID_FUN_ID);
		switch(compile->SymLocalTable.local[tmpindex].type)
		{
		case ZL_SYM_ENUM_LOCAL_TYPE_ARG:
			compile->memDataForDot.type = ZL_R_DT_ARGMEM;
			compile->memDataForDot.val.mem = compile->SymLocalTable.local[tmpindex].localid;
			return compile->SymLocalTable.local[tmpindex].classid;
			break;
		case ZL_SYM_ENUM_LOCAL_TYPE_LOCAL:
			compile->memDataForDot.type = ZL_R_DT_LOCMEM;
			compile->memDataForDot.val.mem = compile->SymLocalTable.local[tmpindex].localid;
			return compile->SymLocalTable.local[tmpindex].classid;
			break;
		case ZL_SYM_ENUM_LOCAL_TYPE_GLOBAL:
			goto FindGlobalSymLoc;
			break;
		default:
			compile->exit(VM_ARG,ZL_ERR_CP_SYM_LOCAL_ID_INVALID_TYPE);
			break;
		}
		return 0;
	}
	return 0;
}

/**
	通过变量名称查找变量标示符在自定义的虚拟内存中的内存地址，主要用于对外的API接口中	
*/
ZL_INT zengl_SymLookupID_ByName(ZL_VOID * VM_ARG,ZL_CHAR * name)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZL_INT h;
	ZL_INT tmpindex;
	h = compile->hash(VM_ARG,name) + ZL_SYM_HASH_SIZE * ZL_HASH_TYPE_SYM_TABLE;
	tmpindex = compile->HashTable[h];
	while(tmpindex != 0 && compile->SymGlobalTable.sym[tmpindex].isvalid == ZL_TRUE &&
		  ZENGL_SYS_STRCMP(name,compile->TokenStringPoolGetPtr(VM_ARG,compile->SymGlobalTable.sym[tmpindex].nameIndex)) != 0)
		tmpindex = compile->SymGlobalTable.sym[tmpindex].next;
	if(tmpindex == 0)
		return -1;
	else if(compile->SymGlobalTable.sym[tmpindex].isvalid == ZL_FALSE)
		return -2;
	return compile->SymGlobalTable.sym[tmpindex].memloc;
}

/*
	根据节点号查找函数ID信息
*/
ZL_INT zengl_SymLookupFun(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT classid)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_AST_NODE_TYPE * node = nodes + nodenum;
	ZL_CHAR * name = compile->TokenStringPoolGetPtr(VM_ARG,nodes[nodenum].strindex);
	ZL_INT h = compile->hash(VM_ARG,name) + ZL_SYM_HASH_SIZE * ZL_HASH_TYPE_FUN_TABLE;
	ZL_INT tmpindex = compile->HashTable[h];

	while(tmpindex != 0 && compile->SymFunTable.funs[tmpindex].isvalid == ZL_TRUE &&
		 (classid != compile->SymFunTable.funs[tmpindex].classid ||
		  ZENGL_SYS_STRCMP(name,compile->TokenStringPoolGetPtr(VM_ARG,compile->SymFunTable.funs[tmpindex].nameIndex)) != 0)) //循环查找同名函数或类函数，如果同名则产生语法错误
		tmpindex = compile->SymFunTable.funs[tmpindex].next;
	if(tmpindex == 0)
		return 0; //如果没找到，说明不是用户自定义的函数，那么有可能是模块里的函数。
	else if(compile->SymFunTable.funs[tmpindex].isvalid == ZL_FALSE)
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_FUN_TABLE_FIND_NOT_VALID_INDEX);
	return compile->SymFunTable.funs[tmpindex].funid; //函数ID从1开始，且等于funs数组对应的索引值
}

/*
	根据函数名称查找函数ID信息
*/
ZL_INT zengl_SymLookupFunByName(ZL_VOID * VM_ARG,ZL_CHAR * name,ZL_INT classid)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZL_INT h;
	ZL_INT tmpindex;
	if(name == ZL_NULL)
		return 0;
	h = compile->hash(VM_ARG,name) + ZL_SYM_HASH_SIZE * ZL_HASH_TYPE_FUN_TABLE;
	tmpindex = compile->HashTable[h];
	while(tmpindex != 0 && compile->SymFunTable.funs[tmpindex].isvalid == ZL_TRUE &&
		 (classid != compile->SymFunTable.funs[tmpindex].classid ||
		  ZENGL_SYS_STRCMP(name,compile->TokenStringPoolGetPtr(VM_ARG,compile->SymFunTable.funs[tmpindex].nameIndex)) != 0)) //循环查找同名函数或类函数，如果同名则产生语法错误
		tmpindex = compile->SymFunTable.funs[tmpindex].next;
	if(tmpindex == 0)
		return 0; //如果没找到，说明不是用户自定义的函数，那么有可能是模块里的函数。
	else if(compile->SymFunTable.funs[tmpindex].isvalid == ZL_FALSE)
		return -1;
	return compile->SymFunTable.funs[tmpindex].funid; //函数ID从1开始，且等于funs数组对应的索引值
}

/*
	使用AST扫描堆栈来扫描语法树中函数的参数
*/
ZL_VOID zengl_SymScanFunArg(ZL_VOID * VM_ARG,ZL_INT nodenum)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_AST_SCAN_STACK_TYPE tmpstack;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZL_INT * chnum;
	ZENGL_RUN_INST_OP_DATA_TYPE tmpInstType = ZL_R_DT_NONE;
	ZL_DOUBLE tmpInstData = 0;
	if(compile->AST_TreeScanStackList.count != 0)
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_AST_TREE_SCAN_STACK_NOT_EMPTY);
	if(nodenum == -1) //如果函数参数列表是空的则返回
		return;
	compile->push_AST_TreeScanStack(VM_ARG,nodenum); //先将节点压入栈，形成扫描节点路径的起始点。
	do{
		tmpstack = compile->pop_AST_TreeScanStack(VM_ARG,ZL_FALSE); //返回前面压入栈的节点信息。参数ZL_FALSE表示只返回信息，暂不将节点从堆栈中删除。
		if(tmpstack.nodenum < 0) //如-1之类的空节点则跳过
		{
			compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE);
			continue;
		}
		if(tmpstack.curchild == 0) //curchild为0时，表示还没开始扫描子节点，就对当前节点进行处理。
		{
			nodenum = tmpstack.nodenum;
			switch(nodes[nodenum].toktype)
			{
			case ZL_TK_ID:
				if(compile->SymInsertHashTableForLocal(VM_ARG,nodenum,ZL_SYM_ENUM_LOCAL_TYPE_ARG) == ZL_TRUE) //如果是脚本函数的参数，就将其加入到SymLocalTable局部变量符号表动态数组中，并将其标示为ZL_SYM_ENUM_LOCAL_TYPE_ARG
					compile->gencode_struct.localID++;
				break;
			case ZL_TK_COMMA:
				break;
			case ZL_TK_ASSIGN:
				chnum = nodes[nodenum].childs.childnum;
				switch(nodes[chnum[1]].toktype)
				{
				case ZL_TK_NUM:
					tmpInstType = ZL_R_DT_NUM;
					tmpInstData = (ZL_DOUBLE)ZENGL_SYS_STR_TO_NUM(compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[1]].strindex));
					break;
				case ZL_TK_FLOAT:
					tmpInstType = ZL_R_DT_FLOAT;
					tmpInstData = ZENGL_SYS_STR_TO_FLOAT(compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[1]].strindex));
					break;
				case ZL_TK_STR:
					tmpInstType = ZL_R_DT_STR;
					tmpInstData = (ZL_DOUBLE)((ZL_LONG)compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[1]].strindex));
					break;
				default:
					compile->parser_curnode = nodenum;
					compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_FUN_ARGLIST_INVALID_TOKEN);
					break;
				}
				run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
					ZL_R_IT_ARG_SET , ZL_R_DT_ARGMEM , compile->gencode_struct.localID,
					tmpInstType , tmpInstData); //对应汇编指令 类似 "ARG_SET arg(%d) 123" 当某个参数没有被赋值时，设置的默认值
				if(compile->SymInsertHashTableForLocal(VM_ARG,chnum[0],ZL_SYM_ENUM_LOCAL_TYPE_ARG) == ZL_TRUE)
					compile->gencode_struct.localID++;
				break;
			default:
				//compile->parser_curnode = nodenum;
				//compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_FUN_ARGLIST_INVALID_TOKEN);
				break;
			}
		}
		if(tmpstack.curchild < tmpstack.childcnt) //如果curchild小于childcnt子节点数，就说明子节点没处理完，则继续处理子节点信息。
		{
			if(tmpstack.curchild < ZL_AST_CHILD_NODE_SIZE) //根据curchild索引判断是基本子节点还是扩展子节点。
				nodenum = nodes[tmpstack.nodenum].childs.childnum[tmpstack.curchild];
			else
				nodenum = nodes[tmpstack.nodenum].childs.extchilds[tmpstack.curchild -
																   ZL_AST_CHILD_NODE_SIZE];
			compile->AST_TreeScanStackList.stacks[compile->AST_TreeScanStackList.count - 1].curchild++; //将curchild加一，下次就会处理下一个子节点信息。
			compile->push_AST_TreeScanStack(VM_ARG,nodenum); //将子节点压入栈
			continue; //continue后会跳到do...while开头，然后pop_TreeStack，从而根据前面压入栈的子节点信息进行处理。
		}
		else
			compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE); //如果没有子节点或者子节点也已扫描完，则弹出当前节点。
	}while(compile->AST_TreeScanStackList.count > 0); //如果堆栈中还有元素，说明还有节点没处理完，只有当堆栈里的元素个数为0时则表示所有AST里的节点都处理完了，就可以跳出循环返回了。
}

/*
	使用AST扫描堆栈来扫描语法树中函数的global关键字声明的全局变量
*/
ZL_VOID zengl_SymScanFunGlobal(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT global_nodenum)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_AST_SCAN_STACK_TYPE tmpstack;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZL_INT globalnum;
	ZENGL_RUN_INST_OP_DATA_TYPE tmpInstType = ZL_R_DT_NONE;
	ZL_DOUBLE tmpInstData = 0;
	globalnum = compile->AST_TreeScanStackList.count; //因为global关键字在fun内部，在扫描局部变量的时候会间接的扫描到global的全局变量，所以global对应的起始堆栈计数不一定为0，可以大于0，因为之前压入的可能是局部变量相关的扫描节点信息。
	if(nodenum == -1)
	{
		compile->parser_curnode = global_nodenum;
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_SYM_GLOBAL_MUST_HAVE_CHILD_NODE);
	}
	compile->push_AST_TreeScanStack(VM_ARG,nodenum); //先将节点压入栈，形成扫描节点路径的起始点。
	do{
		tmpstack = compile->pop_AST_TreeScanStack(VM_ARG,ZL_FALSE); //返回前面压入栈的节点信息。参数ZL_FALSE表示只返回信息，暂不将节点从堆栈中删除。
		if(tmpstack.nodenum < 0) //如-1之类的空节点则跳过
		{
			compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE);
			continue;
		}
		if(tmpstack.curchild == 0) //curchild为0时，表示还没开始扫描子节点，就对当前节点进行处理。
		{
			nodenum = tmpstack.nodenum;
			switch(nodes[nodenum].toktype)
			{
			case ZL_TK_ID:
				compile->SymInsertHashTableForLocal(VM_ARG,nodenum,ZL_SYM_ENUM_LOCAL_TYPE_GLOBAL); //如果是脚本函数中global声明的全局变量，就将其加入到SymLocalTable局部变量符号表动态数组中，并将其标示为ZL_SYM_ENUM_LOCAL_TYPE_GLOBAL
				break;
			case ZL_TK_COMMA:
				break;
			default:
				compile->exit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_TOKEN,
					nodes[nodenum].line_no,
					nodes[nodenum].col_no,
					nodes[nodenum].filename,
					compile->getTokenStr(VM_ARG,nodes,nodenum));
				break;
			}
		}
		if(tmpstack.curchild < tmpstack.childcnt) //如果curchild小于childcnt子节点数，就说明子节点没处理完，则继续处理子节点信息。
		{
			if(tmpstack.curchild < ZL_AST_CHILD_NODE_SIZE) //根据curchild索引判断是基本子节点还是扩展子节点。
				nodenum = nodes[tmpstack.nodenum].childs.childnum[tmpstack.curchild];
			else
				nodenum = nodes[tmpstack.nodenum].childs.extchilds[tmpstack.curchild -
																   ZL_AST_CHILD_NODE_SIZE];
			compile->AST_TreeScanStackList.stacks[compile->AST_TreeScanStackList.count - 1].curchild++; //将curchild加一，下次就会处理下一个子节点信息。
			compile->push_AST_TreeScanStack(VM_ARG,nodenum); //将子节点压入栈
			continue; //continue后会跳到do...while开头，然后pop_TreeStack，从而根据前面压入栈的子节点信息进行处理。
		}
		else
			compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE); //如果没有子节点或者子节点也已扫描完，则弹出当前节点。
	}while(compile->AST_TreeScanStackList.count > globalnum); //扫描堆栈的计数器值等于globalnum初始计数值时，则说明扫描处理完毕并跳出循环。
}

/*扫描类声明语句里声明的变量*/
ZL_VOID zengl_SymScanClassStatement(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT cls_statement_nodenum)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_SCAN_STACK_TYPE tmpstack;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZL_INT classid = compile->SymLookupClass(VM_ARG,cls_statement_nodenum);
	if(compile->AST_TreeScanStackList.count != 0)
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_AST_TREE_SCAN_STACK_NOT_EMPTY);
	if(nodenum == -1)
	{
		compile->parser_curnode = cls_statement_nodenum;
		compile->parser_errorExit(VM_ARG,ZL_ERR_SYNTAX_SYM_CLASS_STATEMENT_MUST_HAVE_CHILD_NODE);
	}
	compile->push_AST_TreeScanStack(VM_ARG,nodenum); //先将节点压入栈，形成扫描节点路径的起始点
	do{
		tmpstack = compile->pop_AST_TreeScanStack(VM_ARG,ZL_FALSE); //返回前面压入栈的节点信息。参数ZL_FALSE表示只返回信息，暂不将节点从堆栈中删除
		if(tmpstack.nodenum < 0) //如-1之类的空节点则跳过
		{
			compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE);
			continue;
		}
		if(tmpstack.curchild == 0) //curchild为0时，表示还没开始扫描子节点，就对当前节点进行处理。
		{
			nodenum = tmpstack.nodenum;
			switch(nodes[nodenum].toktype)
			{
			case ZL_TK_ID:
				compile->SymInsertClassID(VM_ARG,nodenum,classid);
				break;
			case ZL_TK_COMMA:
				break;
			default:
				compile->exit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_TOKEN,
					nodes[nodenum].line_no,
					nodes[nodenum].col_no,
					nodes[nodenum].filename,
					compile->getTokenStr(VM_ARG,nodes,nodenum));
				break;
			}
		}
		if(tmpstack.curchild < tmpstack.childcnt) //如果curchild小于childcnt子节点数，就说明子节点没处理完，则继续处理子节点信息。
		{
			if(tmpstack.curchild < ZL_AST_CHILD_NODE_SIZE) //根据curchild索引判断是基本子节点还是扩展子节点。
				nodenum = nodes[tmpstack.nodenum].childs.childnum[tmpstack.curchild];
			else
				nodenum = nodes[tmpstack.nodenum].childs.extchilds[tmpstack.curchild -
																   ZL_AST_CHILD_NODE_SIZE];
			compile->AST_TreeScanStackList.stacks[compile->AST_TreeScanStackList.count - 1].curchild++; //将curchild加一，下次就会处理下一个子节点信息。
			compile->push_AST_TreeScanStack(VM_ARG,nodenum); //将子节点压入栈
			continue; //continue后会跳到do...while开头，然后pop_TreeStack，从而根据前面压入栈的子节点信息进行处理。
		}
		else
			compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE); //如果没有子节点或者子节点也已扫描完，则弹出当前节点。
	}while(compile->AST_TreeScanStackList.count > 0); //如果堆栈中还有元素，说明还有节点没处理完，只有当堆栈里的元素个数为0时则表示所有AST里的节点都处理完了，就可以跳出循环返回了
}

/*
    扫描类引用时的节点，将类成员转为数组的索引压入栈中。
	如test.test2[3].name.val
	如果test2在test类中的索引为1，name在test2类里的索引为2,val在name类的索引为3则
	表达式相当于test[1,3,2,3]，第一个1代表test2成员,第二个3是test2[3]里的索引3
	第三个2代表name，第四个3代表val
	然后就按数组的方式将1,3,2,3依次压入栈作为test数组的索引值，所以test类本质上就是
	一个数组。 
	
	类引用是采用点运算符来连接的，所以该函数采用了堆栈扫描法
	先在点运算符的第一个子节点中查找类id，然后使用该类id找到第二个子节点即类成员的
	索引，接着将索引PUSH压入栈。
*/
ZL_VOID zengl_SymScanDotForClass(ZL_VOID * VM_ARG,ZL_INT nodenum)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_AST_SCAN_STACK_TYPE tmpstack;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZL_INT classid = 0;
	ZL_INT start_scan_num = 0;
	ZL_BOOL initScan = ZL_TRUE;
	if(compile->AST_TreeScanStackList.count < 0)
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_AST_TREE_SCAN_STACK_INVALID_COUNT);
	else
		start_scan_num = compile->AST_TreeScanStackList.count; //像obj.key[i % obj.keylen]这样的表达式，当扫描obj.key数组元素时，会递归进去扫描obj.keylen，所以AST_TreeScanStackList.count不一定为0，可以大于0
	if(nodenum == -1)
	{
		return;
	}
	compile->push_AST_TreeScanStack(VM_ARG,nodenum); //先将节点压入栈，形成扫描节点路径的起始点
	do{
		tmpstack = compile->pop_AST_TreeScanStack(VM_ARG,ZL_FALSE); //返回前面压入栈的节点信息。参数ZL_FALSE表示只返回信息，暂不将节点从堆栈中删除
		if(tmpstack.nodenum < 0) //如-1之类的空节点则跳过
		{
			compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE);
			continue;
		}
		if(tmpstack.curchild == 0) //curchild为0时，表示还没开始扫描子节点，就对当前节点进行处理。
		{
			nodenum = tmpstack.nodenum;
			switch(nodes[nodenum].toktype)
			{
			case ZL_TK_ID:
			case ZL_TK_ARRAY_ITEM:
				if(classid == 0) //第一次查找时，如test.test2[3].name.val当扫描test时classid就为0
				{
					if(initScan != ZL_TRUE)
					{
						compile->parser_curnode = nodenum;
						compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYM_SCAN_DOT_FOR_CLASS_ID_ZERO_WHEN_NOT_INIT);
					}
					else
						initScan = ZL_FALSE;
					classid = compile->SymLookupID_ForDot(VM_ARG,nodenum); //找到根类的类id
					if(classid == 0)
					{
						compile->parser_curnode = nodenum;
						compile->parser_errorExit(VM_ARG,ZL_ERR_SYNTAX_SYM_CAN_NOT_FIND_CLASS_INFO_FOR_ID);
					}
					if(nodes[nodenum].toktype == ZL_TK_ARRAY_ITEM)
					{
						ZL_INT * chnum = nodes[nodenum].childs.childnum;
						if(nodes[chnum[0]].tokcategory == ZL_TKCG_OP_FACTOR || 
							ZENGL_AST_ISTOKEXPRESS(chnum[0]))
						{
							compile->AsmGCStackPush(VM_ARG,(ZL_INT)ZL_ASM_AI_OP_NONE,ZL_ASM_STACK_ENUM_ARRAY_ITEM_OP_TYPE);
							compile->AsmGenCodes(VM_ARG,chnum[0]);
							compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_ARRAY_ITEM_OP_TYPE,ZL_TRUE);
						}
						else if(chnum[0] != -1)
						{
							compile->exit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_TOKEN,
								nodes[chnum[0]].line_no,
								nodes[chnum[0]].col_no,
								nodes[chnum[0]].filename,
								compile->getTokenStr(VM_ARG,nodes,chnum[0]));
						}
						compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE);
						continue;
					}
				} //if(classid == 0)
				else
				{
					ZL_INT classMemberIndex = compile->SymLookupClassMember(VM_ARG,nodenum,classid);
					run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
						ZL_R_IT_PUSH,ZL_R_DT_NONE,0,
						ZL_R_DT_NUM,compile->SymClassMemberTable.members[classMemberIndex].index); //对应汇编指令 类似 "PUSH %d" 取得类成员的索引，并压入栈
					//classid = compile->SymClassMemberTable.members[classMemberIndex].classid; //使用该类成员的类id，为获取后面的成员的索引做准备
					classid = compile->SymClassMemberTable.members[classMemberIndex].cls_stmt_classid; //应该使用声明该成员时所使用的类ID信息，而非该成员所在的类结构，因为该成员的成员是声明类里的成员！
					if(nodes[nodenum].toktype == ZL_TK_ARRAY_ITEM) //如果该类成员是一个数组元素如上例：test.test2[3].name.val中test2就是一个数组，则将数组里的索引3也压入栈，该操作在AsmGenCodes函数里自动完成
					{
						ZL_INT * chnum = nodes[nodenum].childs.childnum;
						if(nodes[chnum[0]].tokcategory == ZL_TKCG_OP_FACTOR || 
							ZENGL_AST_ISTOKEXPRESS(chnum[0]))
						{
							compile->AsmGCStackPush(VM_ARG,(ZL_INT)ZL_ASM_AI_OP_NONE,ZL_ASM_STACK_ENUM_ARRAY_ITEM_OP_TYPE);
							compile->AsmGenCodes(VM_ARG,chnum[0]);
							compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_ARRAY_ITEM_OP_TYPE,ZL_TRUE);
						}
						else if(chnum[0] != -1)
						{
							compile->exit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_TOKEN,
								nodes[chnum[0]].line_no,
								nodes[chnum[0]].col_no,
								nodes[chnum[0]].filename,
								compile->getTokenStr(VM_ARG,nodes,chnum[0]));
						}
						compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE);
						continue;
					}
				}
				break; //case ZL_TK_ARRAY_ITEM: case ZL_TK_ID:
			case ZL_TK_DOT:
				break;
			default:
				compile->exit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_TOKEN,
							nodes[nodenum].line_no,
							nodes[nodenum].col_no,
							nodes[nodenum].filename,
							compile->getTokenStr(VM_ARG,nodes,nodenum));
				break;
			} //switch(nodes[nodenum].toktype)
		} //if(tmpstack.curchild == 0)
		if(tmpstack.curchild < tmpstack.childcnt) //如果curchild小于childcnt子节点数，就说明子节点没处理完，则继续处理子节点信息。
		{
			if(tmpstack.curchild < ZL_AST_CHILD_NODE_SIZE) //根据curchild索引判断是基本子节点还是扩展子节点。
				nodenum = nodes[tmpstack.nodenum].childs.childnum[tmpstack.curchild];
			else
				nodenum = nodes[tmpstack.nodenum].childs.extchilds[tmpstack.curchild -
																   ZL_AST_CHILD_NODE_SIZE];
			compile->AST_TreeScanStackList.stacks[compile->AST_TreeScanStackList.count - 1].curchild++; //将curchild加一，下次就会处理下一个子节点信息。
			compile->push_AST_TreeScanStack(VM_ARG,nodenum); //将子节点压入栈
			continue; //continue后会跳到do...while开头，然后pop_TreeStack，从而根据前面压入栈的子节点信息进行处理。
		}
		else
			compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE); //如果没有子节点或者子节点也已扫描完，则弹出当前节点。
	}while(compile->AST_TreeScanStackList.count > start_scan_num); //如果堆栈中还有元素，说明还有节点没处理完，只有当堆栈里的元素个数为start_scan_num时则表示所有当前AST里的节点都处理完了，就可以跳出循环返回了
}

/*
	使用AST扫描堆栈来扫描语法树中use关键字声明的模块名
*/
ZL_VOID zengl_SymScanUseRsv(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_INT use_nodenum)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_AST_SCAN_STACK_TYPE tmpstack;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_RUN_INST_OP_DATA inst_op_data;
	if(compile->AST_TreeScanStackList.count != 0)
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_AST_TREE_SCAN_STACK_NOT_EMPTY);
	if(nodenum == -1)
	{
		compile->parser_curnode = use_nodenum;
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_SYM_USE_MUST_HAVE_CHILD_NODE);
	}
	compile->push_AST_TreeScanStack(VM_ARG,nodenum); //先将节点压入栈，形成扫描节点路径的起始点。
	do{
		tmpstack = compile->pop_AST_TreeScanStack(VM_ARG,ZL_FALSE); //返回前面压入栈的节点信息。参数ZL_FALSE表示只返回信息，暂不将节点从堆栈中删除。
		if(tmpstack.curchild == 0) //curchild为0时，表示还没开始扫描子节点，就对当前节点进行处理。
		{
			nodenum = tmpstack.nodenum;
			switch(nodes[nodenum].toktype)
			{
			case ZL_TK_ID:
				inst_op_data.val.num = (ZL_LONG)compile->TokenStringPoolGetPtr(VM_ARG,nodes[nodenum].strindex);
				run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
					ZL_R_IT_USE,ZL_R_DT_NONE,0,
					ZL_R_DT_STR,inst_op_data.val.num); //对应汇编指令 "USE %s"
				break;
			case ZL_TK_COMMA:
				break;
			default:
				compile->exit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_TOKEN,
					nodes[nodenum].line_no,
					nodes[nodenum].col_no,
					nodes[nodenum].filename,
					compile->getTokenStr(VM_ARG,nodes,nodenum));
				break;
			}
		}
		if(tmpstack.curchild < tmpstack.childcnt) //如果curchild小于childcnt子节点数，就说明子节点没处理完，则继续处理子节点信息。
		{
			if(tmpstack.curchild < ZL_AST_CHILD_NODE_SIZE) //根据curchild索引判断是基本子节点还是扩展子节点。
				nodenum = nodes[tmpstack.nodenum].childs.childnum[tmpstack.curchild];
			else
				nodenum = nodes[tmpstack.nodenum].childs.extchilds[tmpstack.curchild -
																   ZL_AST_CHILD_NODE_SIZE];
			compile->AST_TreeScanStackList.stacks[compile->AST_TreeScanStackList.count - 1].curchild++; //将curchild加一，下次就会处理下一个子节点信息。
			compile->push_AST_TreeScanStack(VM_ARG,nodenum); //将子节点压入栈
			continue; //continue后会跳到do...while开头，然后pop_TreeStack，从而根据前面压入栈的子节点信息进行处理。
		}
		else
			compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE); //如果没有子节点或者子节点也已扫描完，则弹出当前节点。
	}while(compile->AST_TreeScanStackList.count > 0); //扫描堆栈的计数器值等于0初始计数值时，则说明扫描处理完毕并跳出循环。
}

/*
	使用AST扫描堆栈来扫描语法树中函数的局部变量
*/
ZL_VOID zengl_SymScanFunLocal(ZL_VOID * VM_ARG,ZL_INT nodenum)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_AST_SCAN_STACK_TYPE tmpstack;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZL_INT * chnum,childcnt;
	if(compile->AST_TreeScanStackList.count != 0)
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_AST_TREE_SCAN_STACK_NOT_EMPTY);
	if(nodenum == -1) //如果函数体是空的则返回
		return;
	compile->push_AST_TreeScanStack(VM_ARG,nodenum); //先将节点压入栈，形成扫描节点路径的起始点。
	do{
		tmpstack = compile->pop_AST_TreeScanStack(VM_ARG,ZL_FALSE); //返回前面压入栈的节点信息。参数ZL_FALSE表示只返回信息，暂不将节点从堆栈中删除。
		if(tmpstack.nodenum < 0) //如-1之类的空节点则跳过
		{
			compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE);
			continue;
		}
		if(tmpstack.curchild == 0) //curchild为0时，表示还没开始扫描子节点，就对当前节点进行处理。
		{
			nodenum = tmpstack.nodenum;
			switch(nodes[nodenum].toktype)
			{
			case ZL_TK_ID:
			case ZL_TK_ARRAY_ITEM:
				if(nodes[nodenum].parentnode != ZL_TK_DOT) //变量，数组元素，且不是以类成员的形式出现的时候，就可以加入局部符号表
				{
					if(compile->SymInsertHashTableForLocal(VM_ARG,nodenum,ZL_SYM_ENUM_LOCAL_TYPE_LOCAL) == ZL_TRUE) //如果是脚本函数的局部变量，就将其加入到SymLocalTable局部变量符号表动态数组中，并将其标示为ZL_SYM_ENUM_LOCAL_TYPE_LOCAL
					{
						run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
								ZL_R_IT_PUSH_LOC , ZL_R_DT_NONE , 0,
								ZL_R_DT_NONE , 0); //对应汇编指令 "PUSH_LOC" 扫描到的局部变量会输出一个PUSH_LOC指令从而当虚拟机运行时会在其堆栈中分配一个栈空间。并将运行时类型设为NONE未初始化状态
						compile->gencode_struct.localID++;
					}
				}
				break;
			case ZL_TK_RESERVE:
				if(nodes[nodenum].reserve == ZL_RSV_GLOBAL)
				{
					childcnt = nodes[nodenum].childs.count;
					chnum = nodes[nodenum].childs.childnum;
					if(childcnt == 1)
					{
						compile->SymScanFunGlobal(VM_ARG,chnum[0],nodenum); //如果是global关键字，就将global后面的全局变量通过SymScanFunGlobal扫描到SymLocalTable中。
						compile->AST_TreeScanStackList.stacks[compile->AST_TreeScanStackList.count - 1].curchild++;
					}
					else
					{
						compile->parser_curnode = nodenum;
						compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_ONE_CHILD);
					}
				}
				break;
			}
		}
		if(tmpstack.curchild < tmpstack.childcnt) //如果curchild小于childcnt子节点数，就说明子节点没处理完，则继续处理子节点信息。
		{
			if(tmpstack.curchild < ZL_AST_CHILD_NODE_SIZE) //根据curchild索引判断是基本子节点还是扩展子节点。
				nodenum = nodes[tmpstack.nodenum].childs.childnum[tmpstack.curchild];
			else
				nodenum = nodes[tmpstack.nodenum].childs.extchilds[tmpstack.curchild -
																   ZL_AST_CHILD_NODE_SIZE];
			compile->AST_TreeScanStackList.stacks[compile->AST_TreeScanStackList.count - 1].curchild++; //将curchild加一，下次就会处理下一个子节点信息。
			compile->push_AST_TreeScanStack(VM_ARG,nodenum); //将子节点压入栈
			continue; //continue后会跳到do...while开头，然后pop_TreeStack，从而根据前面压入栈的子节点信息进行处理。
		}
		else if(tmpstack.next != 0) //对next兄弟节点进行扫描
		{
			compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE);
			compile->push_AST_TreeScanStack(VM_ARG,tmpstack.next);
			continue;
		}
		else
			compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE); //如果没有子节点或者子节点也已扫描完，则弹出当前节点。
	}while(compile->AST_TreeScanStackList.count > 0); //如果堆栈中还有元素，说明还有节点没处理完，只有当堆栈里的元素个数为0时则表示所有AST里的节点都处理完了，就可以跳出循环返回了。
}

/*
	将局部变量和参数插入到局部变量符号表中，并将符号表动态数组的索引加入到哈希表中
*/
ZL_BOOL zengl_SymInsertHashTableForLocal(ZL_VOID * VM_ARG,ZL_INT nodenum,ZENGL_SYM_ENUM_LOCAL_TYPE type)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_AST_NODE_TYPE * node = nodes + nodenum;
	ZL_CHAR * name = compile->TokenStringPoolGetPtr(VM_ARG,nodes[nodenum].strindex);
	ZL_INT h = compile->hash(VM_ARG,name) + ZL_SYM_HASH_SIZE * ZL_HASH_TYPE_LOCAL_TABLE;
	ZL_INT tmpindex = compile->HashTable[h];

	while(tmpindex != 0 && compile->SymLocalTable.local[tmpindex].isvalid == ZL_TRUE && 
		(ZENGL_SYS_STRCMP(name,compile->TokenStringPoolGetPtr(VM_ARG,compile->SymLocalTable.local[tmpindex].nameIndex)) != 0 || 
		compile->SymLocalTable.local[tmpindex].funid != compile->SymFunTable.global_funid))
	{
		tmpindex = compile->SymLocalTable.local[tmpindex].next;
	}
	if(tmpindex == 0)
	{
		tmpindex = compile->HashTable[h];
		compile->HashTable[h] = compile->SymInsertLocalTable(VM_ARG,nodes[nodenum].strindex,type);
		compile->SymLocalTable.local[compile->HashTable[h]].next = tmpindex;
		tmpindex = compile->HashTable[h];
		return ZL_TRUE;
	}
	else if(compile->SymLocalTable.local[tmpindex].isvalid == ZL_FALSE)
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_LOCAL_TABLE_INVALID_INDEX);
	return ZL_FALSE;
}

/*
	将局部变量名和局部变量的类型插入到SymLocalTable动态数组中。
*/
ZL_INT zengl_SymInsertLocalTable(ZL_VOID * VM_ARG,ZL_INT nameIndex,ZENGL_SYM_ENUM_LOCAL_TYPE type)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZL_INT tmpindex;
	if(!compile->SymLocalTable.isInit)
		compile->SymInitLocalTable(VM_ARG);
	if(compile->SymLocalTable.count == compile->SymLocalTable.size)
	{
		compile->SymLocalTable.size += ZL_SYM_LOCAL_TABLE_SIZE;
		compile->SymLocalTable.local = compile->memReAlloc(VM_ARG,compile->SymLocalTable.local,
			compile->SymLocalTable.size * sizeof(ZENGL_SYM_LOCAL_TABLE_MEMBER));
		ZENGL_SYS_MEM_SET(compile->SymLocalTable.local + (compile->SymLocalTable.size - ZL_SYM_LOCAL_TABLE_SIZE),0,
			ZL_SYM_LOCAL_TABLE_SIZE * sizeof(ZENGL_SYM_LOCAL_TABLE_MEMBER));
	}
	tmpindex = compile->SymLocalTable.count;
	if(compile->SymLocalTable.local[tmpindex].isvalid == ZL_FALSE)
	{
		compile->SymLocalTable.local[tmpindex].nameIndex = nameIndex;
		compile->SymLocalTable.local[tmpindex].funid = compile->SymFunTable.global_funid;
		compile->SymLocalTable.local[tmpindex].localid = compile->gencode_struct.localID;
		compile->SymLocalTable.local[tmpindex].type = type;
		compile->SymLocalTable.local[tmpindex].next = 0;
		compile->SymLocalTable.local[tmpindex].isvalid = ZL_TRUE;
		compile->SymLocalTable.count++;
		return tmpindex;
	}
	else
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_LOCAL_TABLE_CAN_NOT_FIND_NOT_VALID_INDEX_WHEN_INSERT);
	return -1;
}

/*
	初始化SymLocalTable局部符号表对应的动态数组
*/
ZL_VOID zengl_SymInitLocalTable(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	if(compile->SymLocalTable.isInit)
		return;
	compile->SymLocalTable.count = 1;
	compile->SymLocalTable.size = ZL_SYM_LOCAL_TABLE_SIZE;
	compile->SymLocalTable.local = compile->memAlloc(VM_ARG,compile->SymLocalTable.size * sizeof(ZENGL_SYM_LOCAL_TABLE_MEMBER));
	if(compile->SymLocalTable.local == ZL_NULL)
		compile->exit(VM_ARG,ZL_ERR_CP_SYM_LOCAL_TABLE_MALLOC_FAILED);
	ZENGL_SYS_MEM_SET(compile->SymLocalTable.local,0,compile->SymLocalTable.size * sizeof(ZENGL_SYM_LOCAL_TABLE_MEMBER));
	compile->SymLocalTable.isInit = ZL_TRUE;
}

/*
	打印符号表，将符号表里的每个变量的内存地址，变量名，以及这些变量标示符在源文件里出现的位置打印显示出来。
	以及所有的函数和类信息。
*/
ZL_VOID zengl_SymPrintTables(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZL_INT i;
	ZL_INT linecol;
	ZL_CLOCK_T start_time = ZENGL_SYS_TIME_CLOCK();
	ZL_CLOCK_T end_time;

	for(i=1;i<compile->SymGlobalTable.count;i++)
	{
		if(compile->SymGlobalTable.sym[i].isvalid)
		{
			compile->info(VM_ARG,"id:%d %s",compile->SymGlobalTable.sym[i].memloc,
						compile->TokenStringPoolGetPtr(VM_ARG,compile->SymGlobalTable.sym[i].nameIndex));
			linecol = compile->SymGlobalTable.sym[i].linecols;
			if(linecol == 0)
				compile->exit(VM_ARG,ZL_ERR_CP_SYM_GLOBAL_TABLE_INDEX_HAVE_NO_LINECOL_WHEN_PRINT,i,i);
			while(linecol)
			{
				compile->info(VM_ARG," [%d %d %s]",compile->LineCols.lines[linecol].lineno,
							compile->LineCols.lines[linecol].colno,compile->LineCols.lines[linecol].filename);
				linecol = compile->LineCols.lines[linecol].next;
			}
			compile->info(VM_ARG,"\n");
		}
	}
	for(i=1;i<compile->SymClassTable.count;i++)
	{
		if(compile->SymClassTable.classTable[i].isvalid)
		{
			compile->info(VM_ARG,"class:%d %s <nodenum:%d>",compile->SymClassTable.classTable[i].classid,
						compile->TokenStringPoolGetPtr(VM_ARG,compile->SymClassTable.classTable[i].nameIndex),
						compile->SymClassTable.classTable[i].nodenum);
			linecol = compile->SymClassTable.classTable[i].linecols;
			if(linecol == 0)
				compile->exit(VM_ARG,ZL_ERR_CP_SYM_CLASS_TABLE_INDEX_HAVE_NO_LINECOL_WHEN_PRINT,i,i);
			while(linecol)
			{
				compile->info(VM_ARG," [%d %d %s]",compile->LineCols.lines[linecol].lineno,
							compile->LineCols.lines[linecol].colno,compile->LineCols.lines[linecol].filename);
				linecol = compile->LineCols.lines[linecol].next;
			}
			compile->info(VM_ARG,"\n");
		}
	}
	for(i=1;i<compile->SymClassMemberTable.count;i++)
	{
		if(compile->SymClassMemberTable.members[i].isvalid)
		{
			compile->info(VM_ARG,"classmember:%d %s",
				compile->SymClassMemberTable.members[i].index,
				compile->TokenStringPoolGetPtr(VM_ARG,compile->SymClassTable.classTable[compile->SymClassMemberTable.members[i].classid].nameIndex));
			if(compile->SymClassMemberTable.members[i].cls_stmt_classid > 0)
			{
				compile->info(VM_ARG," %s",
					compile->TokenStringPoolGetPtr(VM_ARG,compile->SymClassTable.classTable[compile->SymClassMemberTable.members[i].cls_stmt_classid].nameIndex)
					);
			}
			compile->info(VM_ARG," %s",compile->TokenStringPoolGetPtr(VM_ARG,compile->SymClassMemberTable.members[i].nameIndex));
			compile->info(VM_ARG," [%d %d %s]",
				compile->AST_nodes.nodes[compile->SymClassMemberTable.members[i].nodenum].line_no,
				compile->AST_nodes.nodes[compile->SymClassMemberTable.members[i].nodenum].col_no,
				compile->AST_nodes.nodes[compile->SymClassMemberTable.members[i].nodenum].filename
				);
			compile->info(VM_ARG,"\n");
		}
	}
	for(i=1;i<compile->SymFunTable.count;i++)
	{
		if(compile->SymFunTable.funs[i].isvalid)
		{
			if(compile->SymFunTable.funs[i].classid > 0)
				compile->info(VM_ARG,"classfun:<class:%d %s> ",compile->SymFunTable.funs[i].classid,
							compile->TokenStringPoolGetPtr(VM_ARG,compile->SymClassTable.classTable[compile->SymFunTable.funs[i].classid].nameIndex));
			compile->info(VM_ARG,"fun:%d %s",compile->SymFunTable.funs[i].funid,
							compile->TokenStringPoolGetPtr(VM_ARG,compile->SymFunTable.funs[i].nameIndex));
			linecol = compile->SymFunTable.funs[i].linecols;
			if(linecol == 0)
				compile->exit(VM_ARG,ZL_ERR_CP_SYM_FUN_TABLE_INDEX_HAVE_NO_LINECOL_WHEN_PRINT,i,i);
			while(linecol)
			{
				compile->info(VM_ARG," [%d %d %s]",compile->LineCols.lines[linecol].lineno,
							compile->LineCols.lines[linecol].colno,compile->LineCols.lines[linecol].filename);
				linecol = compile->LineCols.lines[linecol].next;
			}
			compile->info(VM_ARG,"\n");
		}
	}
	end_time = ZENGL_SYS_TIME_CLOCK();
	compile->total_print_time += end_time - start_time;
}
