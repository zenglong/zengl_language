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
	该文件为AST语法解析程式。
*/

#include "zengl_global.h"

/**
	将token加入AST抽象语法树
*/
ZL_VOID zengl_ASTAddNode(ZL_VOID * VM_ARG,ZENGL_TOKENTYPE token)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZL_INT i;
	ZENGL_PARSER_OP_LEVEL_STACK_TYPE last_stack;
	ZENGL_RESERVE_TYPE stack_compare_rsv = ZL_RSV_START_NONE;
	ZL_CHAR * detect_from_keyword = ZL_NULL;

	if(!compile->AST_nodes.isInit)
		compile->initAST(VM_ARG);
	if(token == ZL_TK_ERROR || token == ZL_TK_ENDFILE)
		return;
	if(compile->AST_nodes.count == compile->AST_nodes.size)
	{
		compile->AST_nodes.size += ZL_AST_SIZE; //ZL_AST_SIZE为数组的初始大小，也是每次扩容时要增加的大小，定义在global.h中，默认为200
		compile->AST_nodes.nodes = compile->memReAlloc(VM_ARG,compile->AST_nodes.nodes,
														compile->AST_nodes.size * sizeof(ZENGL_AST_NODE_TYPE));
		ZENGL_SYS_MEM_SET(compile->AST_nodes.nodes + (compile->AST_nodes.size - ZL_AST_SIZE),0,
							ZL_AST_SIZE * sizeof(ZENGL_AST_NODE_TYPE)); //将扩容后的新内存空间用0填充。
	}
	if(compile->AST_nodes.nodes[compile->AST_nodes.count].isvalid == ZL_FALSE)
	{
		compile->AST_nodes.nodes[compile->AST_nodes.count].toktype = token;
		if(token == ZL_TK_ID || token == ZL_TK_NUM || token == ZL_TK_FLOAT || token == ZL_TK_STR) //如果是标示符，数字，浮点数，字符串就将该token的字符串信息加入到token字符串池缓存中。
		{
			compile->AST_nodes.nodes[compile->AST_nodes.count].strindex = compile->TokenStringPoolAddStr(VM_ARG,compile->tokenInfo.str); //在AST的对应token节点中的strindex字段存放了字符串信息在token字符串池缓存中的索引。
		}

		compile->AST_nodes.nodes[compile->AST_nodes.count].isvalid = ZL_TRUE;
		switch(compile->AST_nodes.nodes[compile->AST_nodes.count].toktype)
		{
		case ZL_TK_GREAT:
		case ZL_TK_LESS:
		case ZL_TK_EQUAL:
		case ZL_TK_NOT_EQ:
		case ZL_TK_GREAT_EQ:
		case ZL_TK_LESS_EQ:
			compile->AST_nodes.nodes[compile->AST_nodes.count].tokcategory = ZL_TKCG_OP_RELATION;
			compile->AST_nodes.nodes[compile->AST_nodes.count].tok_op_level = ZL_OP_LEVEL_RELATION;
			break;
		case ZL_TK_AND:
		case ZL_TK_OR:
			compile->AST_nodes.nodes[compile->AST_nodes.count].tokcategory = ZL_TKCG_OP_LOGIC;
			compile->AST_nodes.nodes[compile->AST_nodes.count].tok_op_level = ZL_OP_LEVEL_LOGIC;
			break;
		case ZL_TK_REVERSE:
		case ZL_TK_ADDRESS:
			compile->AST_nodes.nodes[compile->AST_nodes.count].tokcategory = ZL_TKCG_OP_LOGIC;
			compile->AST_nodes.nodes[compile->AST_nodes.count].tok_op_level = ZL_OP_LEVEL_REVERSE;
			break;
		case ZL_TK_PLUS:
		case ZL_TK_MINIS:
			compile->AST_nodes.nodes[compile->AST_nodes.count].tokcategory = ZL_TKCG_OP_PLUS_MINIS;
			compile->AST_nodes.nodes[compile->AST_nodes.count].tok_op_level = ZL_OP_LEVEL_PLUS_MINIS;
			break;
		case ZL_TK_PLUS_PLUS:
		case ZL_TK_MINIS_MINIS:
			compile->AST_nodes.nodes[compile->AST_nodes.count].tokcategory = ZL_TKCG_OP_PP_MM;
			compile->AST_nodes.nodes[compile->AST_nodes.count].tok_op_level = ZL_OP_LEVEL_PP_MM;
			break;
		case ZL_TK_MOD: //取余和乘除具有相同的优先级。
		case ZL_TK_TIMES:
		case ZL_TK_DIVIDE:
			compile->AST_nodes.nodes[compile->AST_nodes.count].tokcategory = ZL_TKCG_OP_TIM_DIV;
			compile->AST_nodes.nodes[compile->AST_nodes.count].tok_op_level = ZL_OP_LEVEL_TIM_DIV;
			break;
		case ZL_TK_PLUS_ASSIGN:
		case ZL_TK_MINIS_ASSIGN:
		case ZL_TK_MOD_ASSIGN:
		case ZL_TK_TIMES_ASSIGN:
		case ZL_TK_DIVIDE_ASSIGN:
		case ZL_TK_ASSIGN:
			compile->AST_nodes.nodes[compile->AST_nodes.count].tokcategory = ZL_TKCG_OP_ASSIGN;
			compile->AST_nodes.nodes[compile->AST_nodes.count].tok_op_level = ZL_OP_LEVEL_ASSIGN;
			break;
		case ZL_TK_COMMA:
			compile->AST_nodes.nodes[compile->AST_nodes.count].tokcategory = ZL_TKCG_OP_COMMA;
			compile->AST_nodes.nodes[compile->AST_nodes.count].tok_op_level = ZL_OP_LEVEL_COMMA;
			break;
		case ZL_TK_QUESTION_MARK:
		case ZL_TK_COLON:
			compile->AST_nodes.nodes[compile->AST_nodes.count].tokcategory = ZL_TKCG_OP_QUESTION;
			compile->AST_nodes.nodes[compile->AST_nodes.count].tok_op_level = ZL_OP_LEVEL_QUESTION;
			break;
		case ZL_TK_ID:
		case ZL_TK_NUM:
		case ZL_TK_FLOAT:
		case ZL_TK_STR:
			compile->AST_nodes.nodes[compile->AST_nodes.count].tokcategory = ZL_TKCG_OP_FACTOR;
			compile->AST_nodes.nodes[compile->AST_nodes.count].tok_op_level = ZL_OP_LEVEL_START;
			break;
		case ZL_TK_DOT:
			compile->AST_nodes.nodes[compile->AST_nodes.count].tokcategory = ZL_TKCG_OP_DOT;
			compile->AST_nodes.nodes[compile->AST_nodes.count].tok_op_level = ZL_OP_LEVEL_DOT;
			break;
		default:
			compile->AST_nodes.nodes[compile->AST_nodes.count].tokcategory = ZL_TKCG_OP_START;
			compile->AST_nodes.nodes[compile->AST_nodes.count].tok_op_level = ZL_OP_LEVEL_START;
			break;
		}
		compile->AST_nodes.nodes[compile->AST_nodes.count].line_no = compile->tokenInfo.start_line;
		compile->AST_nodes.nodes[compile->AST_nodes.count].col_no = compile->tokenInfo.start_col;
		compile->AST_nodes.nodes[compile->AST_nodes.count].filename = compile->tokenInfo.filename;
		compile->AST_nodes.nodes[compile->AST_nodes.count].reserve = compile->tokenInfo.reserve;
		compile->AST_nodes.nodes[compile->AST_nodes.count].childs.size = ZL_AST_CHILD_NODE_SIZE; //将添加的AST节点的默认子节点数组进行初始化。默认子节点数组可以存放3个子节点信息，3个以上的部分存放在extchilds扩展子节点部分
		for(i=0;i<compile->AST_nodes.nodes[compile->AST_nodes.count].childs.size;i++)
			compile->AST_nodes.nodes[compile->AST_nodes.count].childs.childnum[i] = -1;
		compile->AST_nodes.nodes[compile->AST_nodes.count].parentnode = -1;
		compile->AST_nodes.count++;
		switch(compile->AST_nodes.nodes[compile->AST_nodes.count-1].reserve) //判断if,while,fun之类的结构是否完整，例如if是否有endif相匹配等
		{
		case ZL_RSV_IF:
		case ZL_RSV_SWITCH:
		case ZL_RSV_WHILE:
		case ZL_RSV_DO:
		case ZL_RSV_FOR:
			compile->opLevel_push_stack(VM_ARG,compile->AST_nodes.count-1);
			break;
		case ZL_RSV_FUN:
			last_stack = compile->opLevel_pop_stack(VM_ARG,-1);
			if(last_stack.nodenum != -1 &&
			   compile->AST_nodes.nodes[last_stack.nodenum].reserve != ZL_RSV_CLASS)
			{
				compile->parser_curnode = last_stack.nodenum;
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_KEYWORD_DETECT_UNEXPECT_KEYWORD,
						compile->AST_nodes.nodes[compile->AST_nodes.count-1].line_no,
						compile->AST_nodes.nodes[compile->AST_nodes.count-1].col_no,
						compile->AST_nodes.nodes[compile->AST_nodes.count-1].filename,
						compile->getTokenStr(VM_ARG,compile->AST_nodes.nodes,compile->AST_nodes.count-1),
						compile->getTokenStr(VM_ARG,compile->AST_nodes.nodes,compile->parser_curnode),
						compile->getTokenStr(VM_ARG,compile->AST_nodes.nodes,compile->AST_nodes.count-1),
						compile->getTokenStr(VM_ARG,compile->AST_nodes.nodes,compile->parser_curnode)
						);
			}
			compile->opLevel_push_stack(VM_ARG,compile->AST_nodes.count-1);
			break;
		case ZL_RSV_CLASS:
			last_stack = compile->opLevel_pop_stack(VM_ARG,-1);
			if(last_stack.nodenum != -1)
			{
				compile->parser_curnode = last_stack.nodenum;
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_KEYWORD_DETECT_UNEXPECT_KEYWORD,
						compile->AST_nodes.nodes[compile->AST_nodes.count-1].line_no,
						compile->AST_nodes.nodes[compile->AST_nodes.count-1].col_no,
						compile->AST_nodes.nodes[compile->AST_nodes.count-1].filename,
						compile->getTokenStr(VM_ARG,compile->AST_nodes.nodes,compile->AST_nodes.count-1),
						compile->getTokenStr(VM_ARG,compile->AST_nodes.nodes,compile->parser_curnode),
						compile->getTokenStr(VM_ARG,compile->AST_nodes.nodes,compile->AST_nodes.count-1),
						compile->getTokenStr(VM_ARG,compile->AST_nodes.nodes,compile->parser_curnode)
						);
			}
			compile->opLevel_push_stack(VM_ARG,compile->AST_nodes.count-1);
			break;
		case ZL_RSV_ENDIF:
		case ZL_RSV_ENDSWT:
		case ZL_RSV_ENDWHILE:
		case ZL_RSV_DOWHILE:
		case ZL_RSV_ENDFOR:
		case ZL_RSV_ENDFUN:
		case ZL_RSV_ENDCLS:
			switch(compile->AST_nodes.nodes[compile->AST_nodes.count-1].reserve)
			{
			case ZL_RSV_ENDIF:
				stack_compare_rsv = ZL_RSV_IF;
				break;
			case ZL_RSV_ENDSWT:
				detect_from_keyword = "endswt or endswitch";
				stack_compare_rsv = ZL_RSV_SWITCH;
				break;
			case ZL_RSV_ENDWHILE:
				stack_compare_rsv = ZL_RSV_WHILE;
				break;
			case ZL_RSV_DOWHILE:
				stack_compare_rsv = ZL_RSV_DO;
				break;
			case ZL_RSV_ENDFOR:
				stack_compare_rsv = ZL_RSV_FOR;
				break;
			case ZL_RSV_ENDFUN:
				stack_compare_rsv = ZL_RSV_FUN;
				break;
			case ZL_RSV_ENDCLS:
				detect_from_keyword = "endcls or endclass";
				stack_compare_rsv = ZL_RSV_CLASS;
				break;
			} //switch
			last_stack = compile->opLevel_pop_stack(VM_ARG,-1);
			if(last_stack.nodenum == -1 || 
			   compile->AST_nodes.nodes[last_stack.nodenum].reserve != stack_compare_rsv)
			{
				compile->parser_curnode = last_stack.nodenum;
				if(detect_from_keyword == ZL_NULL)
					detect_from_keyword = compile->getTokenStr(VM_ARG,compile->AST_nodes.nodes,compile->AST_nodes.count-1);

				if(compile->parser_curnode == -1)
				{
					compile->parser_curnode = compile->AST_nodes.count-1;
					compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_UNEXPECT_ENDKEYWORD,
						compile->getTokenStr(VM_ARG,compile->AST_nodes.nodes,compile->AST_nodes.count-1),
						compile->getTokenStr(VM_ARG,compile->AST_nodes.nodes,compile->AST_nodes.count-1)
						);
				}
				else
					compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_KEYWORD_DETECT_UNEXPECT_KEYWORD,
						compile->AST_nodes.nodes[compile->AST_nodes.count-1].line_no,
						compile->AST_nodes.nodes[compile->AST_nodes.count-1].col_no,
						compile->AST_nodes.nodes[compile->AST_nodes.count-1].filename,
						detect_from_keyword,
						compile->getTokenStr(VM_ARG,compile->AST_nodes.nodes,compile->parser_curnode),
						detect_from_keyword,
						compile->getTokenStr(VM_ARG,compile->AST_nodes.nodes,compile->parser_curnode)
						);
			}
			else
				compile->parser_opLevel_stackList.count > 0 ? compile->parser_opLevel_stackList.count-- : ZL_TRUE ;
			break;
		case ZL_RSV_USE:
			last_stack = compile->opLevel_pop_stack(VM_ARG,-1);
			if(last_stack.nodenum != -1)
			{
				compile->parser_curnode = compile->AST_nodes.count-1;
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_UNEXPECT_LOC_OF_USE);
			}
			break;
		case ZL_RSV_GLOBAL:
			last_stack = compile->opLevel_pop_stack(VM_ARG,-1);
			if(compile->AST_nodes.nodes[last_stack.nodenum].reserve != ZL_RSV_FUN)
			{
				compile->parser_curnode = compile->AST_nodes.count-1;
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_UNEXPECT_LOC_OF_GLOBAL);
			}
			break;
		case ZL_RSV_RETURN:
			for(i=compile->parser_opLevel_stackList.count-1;i>=0;i--)
			{
				switch(compile->AST_nodes.nodes[compile->parser_opLevel_stackList.stacks[i].nodenum].reserve)
				{
				case ZL_RSV_FUN:
					return;
					break;
				}
			}
			compile->parser_curnode = compile->AST_nodes.count-1;
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_UNEXPECT_LOC_OF_RETURN);
			break;
		case ZL_RSV_BREAK:
		case ZL_RSV_CONTINUE:
			for(i=compile->parser_opLevel_stackList.count-1;i>=0;i--)
			{
				switch(compile->AST_nodes.nodes[compile->parser_opLevel_stackList.stacks[i].nodenum].reserve)
				{
				case ZL_RSV_FOR:
				case ZL_RSV_WHILE:
				case ZL_RSV_DO:
				case ZL_RSV_SWITCH:
					return;
					break;
				}
			}
			compile->parser_curnode = compile->AST_nodes.count-1;
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_UNEXPECT_LOC_OF_BREAK_CONTINUE);
			break;
		}	//switch(compile->AST_nodes.nodes[compile->AST_nodes.count-1].reserve)
	}	//if(compile->AST_nodes.nodes[compile->AST_nodes.count].isvalid == ZL_FALSE)
}

/**
	初始化AST抽象语法树的动态数组。
*/
ZL_VOID zengl_initAST(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	if(compile->AST_nodes.isInit)
		return;
	compile->AST_nodes.size = ZL_AST_SIZE;
	compile->AST_nodes.count = 0;
	compile->AST_nodes.nodes = compile->memAlloc(VM_ARG,compile->AST_nodes.size * sizeof(ZENGL_AST_NODE_TYPE));
	if(compile->AST_nodes.nodes == ZL_NULL)
		compile->exit(VM_ARG,ZL_ERR_CP_PARSER_AST_MALLOC_FAILED);
	ZENGL_SYS_MEM_SET(compile->AST_nodes.nodes , 0, compile->AST_nodes.size * sizeof(ZENGL_AST_NODE_TYPE));
	compile->AST_nodes.rootnode = -1;
	compile->AST_nodes.isInit = ZL_TRUE;
}

/*向token字符串池动态缓存添加字符串，并返回该字符串在缓存中的索引信息*/
ZL_INT zengl_TokenStringPoolAddStr(ZL_VOID * VM_ARG , ZL_CHAR * src)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZL_INT len;
	ZL_INT i,j;
	if(!compile->Token_StringPool.isInit)
		compile->initTokenStringPool(VM_ARG);
	if(src == ZL_NULL)
		return -1;
	len = ZENGL_SYS_STRLEN(src);
	if(compile->Token_StringPool.count == compile->Token_StringPool.size ||
			compile->Token_StringPool.count + len + 1 > compile->Token_StringPool.size) //当缓存装满时，就对token字符串池进行扩容。
	{
		do{
			compile->Token_StringPool.size += ZL_TK_STRING_POOL_SIZE;
		}
		while(compile->Token_StringPool.count + len + 1 > compile->Token_StringPool.size);

		compile->Token_StringPool.ptr  = compile->memReAlloc(VM_ARG,compile->Token_StringPool.ptr,
															compile->Token_StringPool.size * sizeof(ZL_CHAR));
		ZENGL_SYS_MEM_SET(compile->Token_StringPool.ptr + (compile->Token_StringPool.size - ZL_TK_STRING_POOL_SIZE),0,
							ZL_TK_STRING_POOL_SIZE * sizeof(ZL_CHAR));
	}
	for(i=compile->Token_StringPool.count,j=0;
		i<compile->Token_StringPool.size && j<len;i++,j++)
	{
		compile->Token_StringPool.ptr[i] = src[j];
	}
	if(i >= compile->Token_StringPool.size)
		compile->exit(VM_ARG , ZL_ERR_CP_TK_STR_POOL_OUT_OF_BOUNDS);
	else
		compile->Token_StringPool.ptr[i] = ZL_STRNULL;
	i = compile->Token_StringPool.count;
	compile->Token_StringPool.count += len +1;
	return i;
}

/*初始化token字符串池的动态缓存*/
ZL_VOID zengl_initTokenStringPool(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	if(compile->Token_StringPool.isInit)
		return;
	compile->Token_StringPool.count = 2;
	compile->Token_StringPool.size = ZL_TK_STRING_POOL_SIZE;
	compile->Token_StringPool.ptr = compile->memAlloc(VM_ARG,compile->Token_StringPool.size * sizeof(ZL_CHAR));
	if(compile->Token_StringPool.ptr == ZL_NULL)
		compile->exit(VM_ARG,ZL_ERR_CP_TK_STR_POOL_MALLOC_FAILED);
	ZENGL_SYS_MEM_SET(compile->Token_StringPool.ptr, 0 , compile->Token_StringPool.size * sizeof(ZL_CHAR));
	compile->Token_StringPool.isInit = ZL_TRUE;
}

/*打印token的信息*/
ZL_VOID zengl_printNode(ZL_VOID * VM_ARG , ZL_INT nodenum)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_AST_NODE_TYPE * node = nodes + nodenum;

	compile->info(VM_ARG,"nodenum:%d ",nodenum);
	if(nodenum == -1) //如果节点号是-1说明是NULL空节点，打印出NULL node然后返回。
	{
		compile->info(VM_ARG,"NULL node \n");
		return;
	}
	switch(node->toktype){
		case ZL_TK_ID:
			compile->info(VM_ARG,"identifier token: %s " , compile->getTokenStr(VM_ARG,nodes,nodenum));
			break;
		case ZL_TK_FUNCALL:
			compile->info(VM_ARG,"funcall token: %s " , compile->getTokenStr(VM_ARG,nodes,nodenum));
			break;
		case ZL_TK_ARRAY_ITEM: //数组元素
			compile->info(VM_ARG,"array token: %s " , compile->getTokenStr(VM_ARG,nodes,nodenum));
			break;
		case ZL_TK_ADDRESS: //引用
			compile->info(VM_ARG,"address token: %s " , compile->getTokenStr(VM_ARG,nodes,nodenum));
			break;
		case ZL_TK_RESERVE:
			compile->info(VM_ARG,"reserve token: %s ", compile->getTokenStr(VM_ARG,nodes,nodenum));
			break;
		case ZL_TK_NUM:
			compile->info(VM_ARG,"number token: %s ",compile->getTokenStr(VM_ARG,nodes,nodenum));
			break;
		case ZL_TK_FLOAT:
			compile->info(VM_ARG,"float token: %s ",compile->getTokenStr(VM_ARG,nodes,nodenum));
			break;
		case ZL_TK_STR:
			compile->info(VM_ARG,"string token: '%s' ",compile->getTokenStr(VM_ARG,nodes,nodenum));
			break;
		case ZL_TK_CLASS_STATEMENT:
			compile->info(VM_ARG,"class statement token: '%s' ",compile->getTokenStr(VM_ARG,nodes,nodenum));
			break;
		case ZL_TK_PLUS:
		case ZL_TK_MINIS:
		case ZL_TK_MOD:
		case ZL_TK_TIMES:
		case ZL_TK_DIVIDE:
		case ZL_TK_PLUS_ASSIGN:
		case ZL_TK_MINIS_ASSIGN:
		case ZL_TK_MOD_ASSIGN:
		case ZL_TK_TIMES_ASSIGN:
		case ZL_TK_DIVIDE_ASSIGN:
		case ZL_TK_GREAT:
		case ZL_TK_LESS:
		case ZL_TK_EQUAL:
		case ZL_TK_NOT_EQ:
		case ZL_TK_GREAT_EQ:
		case ZL_TK_LESS_EQ:
		case ZL_TK_AND:
		case ZL_TK_OR:
		case ZL_TK_REVERSE:
		case ZL_TK_ASSIGN:
		case ZL_TK_COMMA:
		case ZL_TK_QUESTION_MARK:
		case ZL_TK_COLON:
		case ZL_TK_PLUS_PLUS:
		case ZL_TK_MINIS_MINIS:
		case ZL_TK_DOT:
			compile->info(VM_ARG,"ops token: %s ",compile->getTokenStr(VM_ARG,nodes,nodenum));
			break;
		case ZL_TK_LBRACKET:
			compile->info(VM_ARG,"left bracket token: %s ",compile->getTokenStr(VM_ARG,nodes,nodenum));
			break;
		case ZL_TK_LMBRACKET:
			compile->info(VM_ARG,"left middle bracket token: %s ",compile->getTokenStr(VM_ARG,nodes,nodenum));
			break;
		case ZL_TK_RBRACKET:
			compile->info(VM_ARG,"right bracket token: %s ",compile->getTokenStr(VM_ARG,nodes,nodenum));
			break;
		case ZL_TK_RMBRACKET:
			compile->info(VM_ARG,"right middle bracket token: %s ",compile->getTokenStr(VM_ARG,nodes,nodenum));
			break;
		case ZL_TK_SEMI:
			compile->info(VM_ARG,"statement end mark token %s ",compile->getTokenStr(VM_ARG,nodes,nodenum));
			break;
		default:
			compile->info(VM_ARG,"error unknown token: %s, tokentype: %d ",
					compile->getTokenStr(VM_ARG,nodes,nodenum),node->toktype);
			break;
	}
	compile->info(VM_ARG,"line:%d,col:%d <'%s'>\n",node->line_no,node->col_no,node->filename);
}

/*根据节点号获取token对应的字符串信息*/
ZL_CHAR * zengl_getTokenStr(ZL_VOID * VM_ARG , ZENGL_AST_NODE_TYPE * nodes,ZL_INT nodenum)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	if(nodenum < 0) //当为-1空节点时。
		return ZL_NULL;
	if(ZENGL_AST_ISTOKCATEX(nodenum,ZL_TKCG_OP_FACTOR)) //如果是标示符等操作因子，就从token字符串池缓存中获取字符串信息。 
	{
		return compile->TokenStringPoolGetPtr(VM_ARG,nodes[nodenum].strindex);
	}
	else if(ZENGL_AST_ISTOKTYPEX(nodenum,ZL_TK_RESERVE)) //如果是if,else之类的关键字，就从reserveString关键字数组中获取字符串信息。
	{
		return (ZL_CHAR *)compile->reserveString[nodes[nodenum].reserve];
	}
	else //如果是表达式运算符之类的，就从TokenOperateString数组中获取字符串信息。
	{
		if(nodes[nodenum].toktype < compile->TokenOperateStringCount)
			return (ZL_CHAR *)compile->TokenOperateString[nodes[nodenum].toktype];
		else
			return ZL_NULL;
	}
}

/*根据索引index从token字符串池缓存中得到对应的字符串信息*/
ZL_CHAR * zengl_TokenStringPoolGetPtr(ZL_VOID * VM_ARG , ZL_INT index)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	if(index < 2 || index >  compile->Token_StringPool.count - 1)
		return ZL_NULL;
	return compile->Token_StringPool.ptr + index;
}

/*
	打印出AST抽象语法树的所有节点信息。采用了新的AST扫描堆栈的方法进行遍历扫描。
*/
ZL_VOID zengl_printASTnodes(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_SCAN_STACK_TYPE tmpstack;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZL_INT nodenum;
	compile->push_AST_TreeScanStack(VM_ARG,compile->AST_nodes.rootnode); //将AST根节点压入栈，表示从根节点开始，将AST里的所有节点信息全部遍历打印输出。
	do{
		tmpstack = compile->pop_AST_TreeScanStack(VM_ARG,ZL_FALSE); //返回前面压入栈的节点信息。参数ZL_FALSE表示只返回信息，暂不将节点从堆栈中删除。
		if(tmpstack.curchild == 0) //curchild表示当前正在扫描的子节点索引，0表示还没开始扫描子节点，就将当前节点信息打印输出。
			compile->printNode(VM_ARG,tmpstack.nodenum);
		if(tmpstack.childcnt > 0) //如果返回的节点里包含子节点，则将子节点信息打印出来。
		{
			if(tmpstack.curchild < tmpstack.childcnt) //当curchild小于childcnt子节点数时，说明子节点还没全部打印完，就继续打印其他的子节点信息。
			{
				if(tmpstack.curchild == 0) //先显示当前节点一共有多少个子节点。
					compile->info(VM_ARG,"(%d) %s has %d childs: \n",tmpstack.nodenum,
									compile->getTokenStr(VM_ARG,nodes,tmpstack.nodenum),tmpstack.childcnt);
				if(tmpstack.curchild < ZL_AST_CHILD_NODE_SIZE) //如果小于ZL_AST_CHILD_NODE_SIZE,就将childnum里的基本子节点信息打印出来，否则就将extchilds扩展子节点里的节点信息打印出来。
					nodenum = nodes[tmpstack.nodenum].childs.childnum[tmpstack.curchild];
				else
					nodenum = nodes[tmpstack.nodenum].childs.extchilds[tmpstack.curchild -
																	   ZL_AST_CHILD_NODE_SIZE];
				compile->info(VM_ARG,"(%d) %s (%d) child is ",tmpstack.nodenum,
								compile->getTokenStr(VM_ARG,nodes,tmpstack.nodenum),tmpstack.curchild);
				compile->AST_TreeScanStackList.stacks[compile->AST_TreeScanStackList.count - 1].curchild++; //将当前扫描的节点索引加一，这样下次就可以扫描下一个子节点。
				compile->push_AST_TreeScanStack(VM_ARG,nodenum); //将当前扫描的子节点压入栈，这样就和之前压入栈的节点在堆栈中构成了一个节点路径。
				continue; //continue后会跳到do...while开头，然后pop_AST_TreeScanStack和printNode就会将刚压入栈的子节点信息打印出来。
			}
			else
				compile->info(VM_ARG,"(%d) %s childs end \n",tmpstack.nodenum,
								compile->getTokenStr(VM_ARG,nodes,tmpstack.nodenum),tmpstack.childcnt); //子节点扫描完毕。
		}
		if(tmpstack.next != 0) //如果当前节点有next兄弟节点，就将当前节点弹出，并将next节点压入栈
		{
			compile->info(VM_ARG,"(%d) %s has nextnode: ",tmpstack.nodenum,
								compile->getTokenStr(VM_ARG,nodes,tmpstack.nodenum));
			compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE);
			compile->push_AST_TreeScanStack(VM_ARG,tmpstack.next);
			continue; //continue后会跳到do...while开头，然后pop_AST_TreeScanStack和printNode就会将刚压入栈的next节点信息打印出来。
		}
		else
			compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE); //当子节点和next节点信息都打印完了，就将节点从堆栈中弹出。
	}while(compile->AST_TreeScanStackList.count > 0); //如果堆栈中还有元素，说明还有节点信息没打印完，只有当堆栈里的元素个数为0时则表示所有AST里的节点信息都打印完了，就可以跳出循环返回了。
}

/*
	AST扫描堆栈的压栈函数。堆栈扫描AST的原理其实就是将扫描过的节点路径保存到堆栈中，这样当扫描到底部时，就可以通过zengl_pop_AST_TreeScanStack将节点路径依次弹出，从而返回
	到原始起点，并从原始起点的下一个扫描点开始继续进行从上到下，从左到右的扫描，循环下去，直到将所需要扫描的AST区域全部扫描完为止。
*/
ZL_VOID zengl_push_AST_TreeScanStack(ZL_VOID * VM_ARG , ZL_INT nodenum)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	if(!compile->AST_TreeScanStackList.isInit)
		compile->init_AST_TreeScanStack(VM_ARG);
	if(compile->AST_TreeScanStackList.count == compile->AST_TreeScanStackList.size)
	{
		compile->AST_TreeScanStackList.size += ZL_AST_SCAN_STACKLIST_SIZE;
		compile->AST_TreeScanStackList.stacks = compile->memReAlloc(VM_ARG,compile->AST_TreeScanStackList.stacks,
																	compile->AST_TreeScanStackList.size * sizeof(ZENGL_AST_SCAN_STACK_TYPE));
		ZENGL_SYS_MEM_SET(compile->AST_TreeScanStackList.stacks + (compile->AST_TreeScanStackList.size - ZL_AST_SCAN_STACKLIST_SIZE),0,
								ZL_AST_SCAN_STACKLIST_SIZE * sizeof(ZENGL_AST_SCAN_STACK_TYPE));
	}
	if(nodenum < 0)
		compile->AST_TreeScanStackList.stacks[compile->AST_TreeScanStackList.count].nodenum = -1;
	else
	{
		compile->AST_TreeScanStackList.stacks[compile->AST_TreeScanStackList.count].nodenum = nodenum;
		compile->AST_TreeScanStackList.stacks[compile->AST_TreeScanStackList.count].childcnt = nodes[nodenum].childs.count;
		compile->AST_TreeScanStackList.stacks[compile->AST_TreeScanStackList.count].next = nodes[nodenum].nextnode;
		compile->AST_TreeScanStackList.stacks[compile->AST_TreeScanStackList.count].curchild = 0;
	}
	compile->AST_TreeScanStackList.count++;
}

/*
	AST扫描堆栈的弹出栈函数，参数remove判断是否需要在返回节点信息的同时将节点从堆栈中删除。
*/
ZENGL_AST_SCAN_STACK_TYPE zengl_pop_AST_TreeScanStack(ZL_VOID * VM_ARG,ZL_BOOL remove)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	if(compile->AST_TreeScanStackList.count <= 0)
		compile->exit(VM_ARG,ZL_ERR_CP_AST_SCAN_STACK_HAS_NOTHING);
	else if(remove == ZL_TRUE)
		return compile->AST_TreeScanStackList.stacks[--compile->AST_TreeScanStackList.count];
	else
		return compile->AST_TreeScanStackList.stacks[compile->AST_TreeScanStackList.count-1];
}

/*
	初始化AST扫描堆栈AST_TreeScanStackList，原来的AST扫描是通过递归函数调用的方法，这种方法虽然用起来简单，但是过多的递归会导致堆栈溢出或者corrupt被破坏。
	采用动态数组模拟堆栈的方法，一方面比递归调用开销小，另一方面不会破坏进程的堆栈空间。
*/
ZL_VOID zengl_init_AST_TreeScanStack(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	if(compile->AST_TreeScanStackList.isInit)
		return;
	compile->AST_TreeScanStackList.size = ZL_AST_SCAN_STACKLIST_SIZE;
	compile->AST_TreeScanStackList.count = 0;
	compile->AST_TreeScanStackList.stacks = compile->memAlloc(VM_ARG,compile->AST_TreeScanStackList.size * sizeof(ZENGL_AST_SCAN_STACK_TYPE));
	if(compile->AST_TreeScanStackList.stacks == ZL_NULL)
		compile->exit(VM_ARG,ZL_ERR_CP_AST_SCAN_STACK_MALLOC_FAILED);
	ZENGL_SYS_MEM_SET(compile->AST_TreeScanStackList.stacks,0,compile->AST_TreeScanStackList.size * sizeof(ZENGL_AST_SCAN_STACK_TYPE));
	compile->AST_TreeScanStackList.isInit = ZL_TRUE;
}

/**
	该函数通过循环来构建所有语句AST
*/
ZL_VOID zengl_buildAST(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZL_INT p = -1,tmp = -1;
	ZL_BOOL tmpinitRootNode = ZL_FALSE;
	while(compile->parser_curnode < compile->AST_nodes.count-1)
	{
		if(!tmpinitRootNode)
		{
			tmp = compile->statement(VM_ARG);
			if(tmp == -1)
				continue;
			compile->AST_nodes.rootnode = tmp;
			p = compile->AST_nodes.rootnode;
			tmpinitRootNode = ZL_TRUE;
		}
		else
		{
			tmp = compile->statement(VM_ARG);
			if(tmp == -1)
				continue;
			compile->AST_nodes.nodes[p].nextnode = tmp;
			p = compile->AST_nodes.nodes[p].nextnode;
		}
	}
	if(!tmpinitRootNode)
		compile->AST_nodes.rootnode = -1;
}

/**
	生成语句比如print 'hello'之类的语句的AST语法树，statement会调用express（第三个版本的语法分析函数）来生成语句中表达式的AST
*/
ZL_INT zengl_statement(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZL_INT p;
	
	if(compile->parser_curnode+1 > compile->AST_nodes.count - 1 ||
		!ZENGL_AST_ISTOK_VALIDX(compile->parser_curnode+1))
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_PARSER_CURNODE_OUT_OF_END);

	else if(nodes[compile->parser_curnode+1].toktype == ZL_TK_RESERVE)
	{
		compile->parser_addcurnode(VM_ARG);
		switch(nodes[compile->parser_curnode].reserve)
		{
		case ZL_RSV_PRINT:
			p = compile->parser_curnode;
			compile->ASTAddNodeChild(VM_ARG,p,compile->express(VM_ARG));
			break;
		case ZL_RSV_IF:
			p = compile->parser_curnode;
			compile->if_stmt(VM_ARG,p);
			break;
		case ZL_RSV_FOR:
			p = compile->parser_curnode;
			compile->for_stmt(VM_ARG,p);
			break;
		case ZL_RSV_FUN:
			p = compile->parser_curnode;
			compile->fun_stmt(VM_ARG,p);
			break;
		case ZL_RSV_GLOBAL:
			compile->TokenSyntaxDetect(VM_ARG);
			p = compile->parser_curnode;
			compile->ASTAddNodeChild(VM_ARG,p,compile->express(VM_ARG));
			break;
		case ZL_RSV_RETURN:
			p = compile->parser_curnode;
			compile->ASTAddNodeChild(VM_ARG,p,compile->express(VM_ARG));
			break;
		case ZL_RSV_USE:
			compile->TokenSyntaxDetect(VM_ARG);
			p = compile->parser_curnode;
			compile->ASTAddNodeChild(VM_ARG,p,compile->express(VM_ARG));
			break;
		case ZL_RSV_BREAK:
		case ZL_RSV_CONTINUE:
			compile->TokenSyntaxDetect(VM_ARG);
			p = compile->parser_curnode;
			compile->ASTAddNodeChild(VM_ARG,p,compile->express(VM_ARG));
			break;
		case ZL_RSV_SWITCH:
			p = compile->parser_curnode;
			compile->switch_stmt(VM_ARG,p);
			break;
		case ZL_RSV_CLASS:
			p = compile->parser_curnode;
			compile->class_stmt(VM_ARG,p);
			break;
		case ZL_RSV_WHILE:
			p = compile->parser_curnode;
			compile->while_stmt(VM_ARG,p);
			break;
		case ZL_RSV_DO:
			p = compile->parser_curnode;
			compile->do_stmt(VM_ARG,p);
			break;
		default:
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_UNKNOWN_KEYWORD);
			break;
		}
	}
	else if(nodes[compile->parser_curnode+1].toktype == ZL_TK_ID &&
			compile->parser_curnode+2 <= compile->AST_nodes.count - 1 &&
			ZENGL_AST_ISTOKTYPEX(compile->parser_curnode+2,ZL_TK_ID))
	{
		compile->parser_addcurnode(VM_ARG);
		p = compile->parser_curnode;
		nodes[p].toktype = ZL_TK_CLASS_STATEMENT;
		compile->TokenSyntaxDetect(VM_ARG);
		compile->ASTAddNodeChild(VM_ARG,p,compile->express(VM_ARG));
	}
	else
	{
		p = compile->express(VM_ARG);
	}
	return p;
}

/*
	关键词完整性检查，例如if结构必须有endif相匹配等
*/
ZL_VOID zengl_KeywordCompleteDetect(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZL_INT first_stack_nodenum;
	if(!compile->parser_opLevel_stackList.isInit)
		return;
	else
		first_stack_nodenum = compile->parser_opLevel_stackList.stacks[0].nodenum;

	if(compile->parser_opLevel_stackList.count > 0)
	{
		switch(nodes[first_stack_nodenum].reserve)
		{
		case ZL_RSV_IF:
			compile->parser_curnode = first_stack_nodenum;
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_IF_NO_ENDIF);
			break;
		case ZL_RSV_SWITCH:
			compile->parser_curnode = first_stack_nodenum;
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_SWITCH_NO_ENDSWT);
			break;
		case ZL_RSV_WHILE:
			compile->parser_curnode = first_stack_nodenum;
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_WHILE_NO_ENDWHILE);
			break;
		case ZL_RSV_DO:
			compile->parser_curnode = first_stack_nodenum;
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_DO_NO_DOWHILE);
			break;
		case ZL_RSV_FOR:
			compile->parser_curnode = first_stack_nodenum;
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_FOR_NO_ENDFOR);
			break;
		case ZL_RSV_FUN:
			compile->parser_curnode = first_stack_nodenum;
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_FUN_NO_ENDFUN);
			break;
		case ZL_RSV_CLASS:
			compile->parser_curnode = first_stack_nodenum;
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_CLASS_NO_ENDCLS);
			break;
		}
	}
	else
		compile->parser_opLevel_stackList.count = 0;
}

/*
	use，global，break，continue，类声明等语句的语法检查
*/
ZL_VOID zengl_TokenSyntaxDetect(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZL_INT count = 0;
	ZL_INT tmpNodeNum = compile->parser_curnode;
	switch(nodes[tmpNodeNum].reserve)
	{
	case ZL_RSV_USE:
	case ZL_RSV_GLOBAL:
		while((++tmpNodeNum) <= compile->AST_nodes.count - 1)
		{
			switch(nodes[tmpNodeNum].toktype)
			{
			case ZL_TK_ID:
			case ZL_TK_COMMA:
				break;
			case ZL_TK_SEMI:
				return;
				break;
			default:
				if(nodes[compile->parser_curnode].reserve == ZL_RSV_GLOBAL)
				{
					//compile->parser_curnode = tmpNodeNum;
					compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_EXP_AFTER_GLOBAL);
				}
				else
				{
					//compile->parser_curnode = tmpNodeNum;
					compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_EXP_AFTER_USE);
				}
				break;
			}
		}
		if(nodes[compile->parser_curnode].reserve == ZL_RSV_GLOBAL)
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_EXP_AFTER_GLOBAL);
		else
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_EXP_AFTER_USE);
		break;
	case ZL_RSV_BREAK:
	case ZL_RSV_CONTINUE:
		while((++tmpNodeNum) <= compile->AST_nodes.count - 1)
		{
			switch(nodes[tmpNodeNum].toktype)
			{
			case ZL_TK_SEMI:
				return;
				break;
			default:
				if(nodes[compile->parser_curnode].reserve == ZL_RSV_BREAK)
				{
					//compile->parser_curnode = tmpNodeNum;
					compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_BREAK_EXP);
				}
				else
				{
					//compile->parser_curnode = tmpNodeNum;
					compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_CONTINUE_EXP);
				}
				break;
			}
		}
		if(nodes[compile->parser_curnode].reserve == ZL_RSV_BREAK)
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_BREAK_EXP);
		else
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_CONTINUE_EXP);
		break;
	default:
		if(nodes[tmpNodeNum].toktype == ZL_TK_CLASS_STATEMENT)
		{
			while((++tmpNodeNum) <= compile->AST_nodes.count - 1)
			{
				switch(nodes[tmpNodeNum].toktype)
				{
				case ZL_TK_ID:
				case ZL_TK_COMMA:
					break;
				case ZL_TK_SEMI:
					return;
					break;
				default:
					//compile->parser_curnode = tmpNodeNum;
					compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_EXP_AFTER_CLASS_STATEMENT);
					break;
				}
			}
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_EXP_AFTER_CLASS_STATEMENT);
		}	//if(nodes[tmpNodeNum].toktype == ZL_TK_CLASS_STATEMENT)
		break;
	}
}

/*
	if-elif-else控制语句的AST抽象语法树的生成函数。
*/
ZL_VOID zengl_if_stmt(ZL_VOID * VM_ARG,ZL_INT p)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_STATES state = ZL_ST_START;
	ZL_INT origRBracketNodeNum;
	ZL_INT orig_p = p;
	ZL_BOOL final = ZL_FALSE;

	while(state != ZL_ST_DOWN)
	{
		switch(state)
		{
		case ZL_ST_START:
			if(compile->parser_curnode+1 > compile->AST_nodes.count - 1)
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_PARSER_CURNODE_OUT_OF_END);
			else if(!ZENGL_AST_ISTOK_VALIDX(compile->parser_curnode+1) ||
					nodes[compile->parser_curnode + 1].toktype != ZL_TK_LBRACKET)
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_IF_NO_LBRACKET);
			else
			{
				compile->parser_addcurnode(VM_ARG);
				origRBracketNodeNum = compile->detectSetRBracketToSemi(VM_ARG);
				compile->ASTAddNodeChild(VM_ARG,p,compile->express(VM_ARG));
				nodes[origRBracketNodeNum].toktype = ZL_TK_RBRACKET;
				state = ZL_ST_PARSER_STMT_INIF;
			}
			break;
		case ZL_ST_PARSER_STMT_INIF: //生成if,elif,else的代码块的AST
			compile->ASTAddNodeChild(VM_ARG,orig_p,compile->if_stmt_sequence(VM_ARG));
			compile->parser_addcurnode(VM_ARG);
			if(nodes[compile->parser_curnode].toktype == ZL_TK_RESERVE)
			{
				switch(nodes[compile->parser_curnode].reserve)
				{
				case ZL_RSV_ENDIF: //遇到endif表示if控制结构结束
					state = ZL_ST_DOWN;
					break;
				case ZL_RSV_ELIF:
					if(final == ZL_TRUE)
						compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ELIF_MUST_BEFORE_ELSE);
					compile->ASTAddNodeChild(VM_ARG,orig_p,compile->parser_curnode);
					p = compile->parser_curnode;
					state = ZL_ST_START;
					break;
				case ZL_RSV_ELSE:
					if(final == ZL_TRUE)
						compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ELSE_MORE_THAN_ONE);
					compile->ASTAddNodeChild(VM_ARG,orig_p,compile->parser_curnode);
					final = ZL_TRUE;
					break;
				default:
					compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_UNKNOWN_KEYWORD_INIF);
					break;
				}
			}
			break;
		}//switch(state)
	}//while(state != ZL_ST_DOWN)
}

/*
	if-elif-else-endif语句代码块中所有表达式的AST抽象语法树的生成
*/
ZL_INT zengl_if_stmt_sequence(ZL_VOID * VM_ARG)
{	 
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZL_INT root = -1,p = -1,tmp = -1;

	if(compile->parser_curnode+1 > compile->AST_nodes.count - 1 ||
		!ZENGL_AST_ISTOK_VALIDX(compile->parser_curnode+1))
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_PARSER_CURNODE_OUT_OF_END);

	if(nodes[compile->parser_curnode+1].toktype == ZL_TK_RESERVE &&
		(nodes[compile->parser_curnode+1].reserve == ZL_RSV_ELIF ||
		 nodes[compile->parser_curnode+1].reserve == ZL_RSV_ELSE ||
		 nodes[compile->parser_curnode+1].reserve == ZL_RSV_ENDIF)) //如果直接遇到elif,else,endif，说明语句块中没有语句，直接返回root,root在最开始设为了-1，所以相当于返回-1，即空节点。
		return root;
	do
	{
		if((tmp = compile->statement(VM_ARG)) == -1) //如果是空节点例如 ;; 两个分号在一起的情况，中间就是一个空语句，此时直接continue跳过空语句，继续下面的语句的语法树生成。
			continue;
		if(p == -1)
			p = tmp;
		else
		{
			nodes[p].nextnode = tmp;  //通过nextnode将语句块里的语句连接成链表。
			p = tmp;
		}
		if(root == -1) //设置语句块的第一条语句为根节点。
			root = tmp;

		if(compile->parser_curnode+1 > compile->AST_nodes.count - 1 ||
			!ZENGL_AST_ISTOK_VALIDX(compile->parser_curnode+1))
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_PARSER_CURNODE_OUT_OF_END);

	}while(!(nodes[compile->parser_curnode+1].toktype == ZL_TK_RESERVE &&
			(nodes[compile->parser_curnode+1].reserve == ZL_RSV_ELIF ||
			 nodes[compile->parser_curnode+1].reserve == ZL_RSV_ELSE ||
			 nodes[compile->parser_curnode+1].reserve == ZL_RSV_ENDIF)));
	return root;  //返回第一条表达式的AST的顶级父节点。
}

/*
	for...endfor控制语句的AST抽象语法树的生成函数。
*/
ZL_VOID zengl_for_stmt(ZL_VOID * VM_ARG,ZL_INT p)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_STATES state = ZL_ST_START;
	ZL_INT origRBracketNodeNum = -1;

	while(state != ZL_ST_DOWN)
	{
		switch(state)
		{
		case ZL_ST_START:
			if(compile->parser_curnode+1 > compile->AST_nodes.count - 1)
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_PARSER_CURNODE_OUT_OF_END);
			else if(!ZENGL_AST_ISTOK_VALIDX(compile->parser_curnode+1) ||
					nodes[compile->parser_curnode + 1].toktype != ZL_TK_LBRACKET)
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_FOR_NO_LBRACKET);
			else
			{
				compile->parser_addcurnode(VM_ARG);
				origRBracketNodeNum = compile->detectSetRBracketToSemi(VM_ARG);
				compile->ASTAddNodeChild(VM_ARG,p,compile->express(VM_ARG));
				compile->ASTAddNodeChild(VM_ARG,p,compile->express(VM_ARG));
				compile->ASTAddNodeChild(VM_ARG,p,compile->express(VM_ARG));
				nodes[origRBracketNodeNum].toktype = ZL_TK_RBRACKET;
				state = ZL_ST_PARSER_STMT_INFOR;
			}
			break;
		case ZL_ST_PARSER_STMT_INFOR: //生成for...endfor的代码块的AST
			compile->ASTAddNodeChild(VM_ARG,p,compile->for_stmt_sequence(VM_ARG));
			compile->parser_addcurnode(VM_ARG);
			if(nodes[compile->parser_curnode].toktype == ZL_TK_RESERVE &&
				nodes[compile->parser_curnode].reserve == ZL_RSV_ENDFOR)
			{
				state = ZL_ST_DOWN;
			}
			else
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ITS_NOT_ENDFOR);
			break;
		}//switch(state)
	}//while(state != ZL_ST_DOWN)
}

/*
	for...endfor语句代码块中所有表达式的AST抽象语法树的生成
*/
ZL_INT zengl_for_stmt_sequence(ZL_VOID * VM_ARG)
{	 
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZL_INT root = -1,p = -1,tmp = -1;

	if(compile->parser_curnode+1 > compile->AST_nodes.count - 1 ||
		!ZENGL_AST_ISTOK_VALIDX(compile->parser_curnode+1))
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_PARSER_CURNODE_OUT_OF_END);

	if(nodes[compile->parser_curnode+1].toktype == ZL_TK_RESERVE &&
		(nodes[compile->parser_curnode+1].reserve == ZL_RSV_ENDFOR)) //如果直接遇到endfor，说明语句块中没有语句，直接返回root,root在最开始设为了-1，所以相当于返回-1，即空节点。
		return root;
	do
	{
		if((tmp = compile->statement(VM_ARG)) == -1) //如果是空节点例如 ;; 两个分号在一起的情况，中间就是一个空语句，此时直接continue跳过空语句，继续下面的语句的语法树生成。
			continue;
		if(p == -1)
			p = tmp;
		else
		{
			nodes[p].nextnode = tmp;  //通过nextnode将语句块里的语句连接成链表。
			p = tmp;
		}
		if(root == -1) //设置语句块的第一条语句为根节点。
			root = tmp;

		if(compile->parser_curnode+1 > compile->AST_nodes.count - 1 ||
			!ZENGL_AST_ISTOK_VALIDX(compile->parser_curnode+1))
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_PARSER_CURNODE_OUT_OF_END);

	}while(!(nodes[compile->parser_curnode+1].toktype == ZL_TK_RESERVE &&
			(nodes[compile->parser_curnode+1].reserve == ZL_RSV_ENDFOR)));
	return root;  //返回第一条表达式的AST的顶级父节点。
}

/*
	fun...endfun的AST抽象语法树的生成函数。
*/
ZL_VOID zengl_fun_stmt(ZL_VOID * VM_ARG,ZL_INT p)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_STATES state = ZL_ST_START;
	ZL_INT origRBracketNodeNum = -1;

	while(state != ZL_ST_DOWN)
	{
		switch(state)
		{
		case ZL_ST_START:
			compile->parser_addcurnode(VM_ARG);
			if(!ZENGL_AST_ISTOKTYPEX(compile->parser_curnode,ZL_TK_ID))
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_FUN_NO_NAME);
			else
				compile->ASTAddNodeChild(VM_ARG,p,compile->parser_curnode); //将函数名节点加入到fun节点。

			compile->parser_addcurnode(VM_ARG);
			if(!ZENGL_AST_ISTOKTYPEX(compile->parser_curnode,ZL_TK_LBRACKET))
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_FUN_NO_LBRACKET);
			else
				origRBracketNodeNum = compile->detectSetRBracketToSemi(VM_ARG);
			compile->ASTAddNodeChild(VM_ARG,p,compile->express(VM_ARG)); //得到fun的参数列表的AST
			nodes[origRBracketNodeNum].toktype = ZL_TK_RBRACKET;
			state = ZL_ST_PARSER_STMT_INFUN;
			break;
		case ZL_ST_PARSER_STMT_INFUN: //生成fun...endfun的代码块的AST
			compile->ASTAddNodeChild(VM_ARG,p,compile->fun_stmt_sequence(VM_ARG));
			compile->parser_addcurnode(VM_ARG);
			if(nodes[compile->parser_curnode].toktype == ZL_TK_RESERVE &&
				nodes[compile->parser_curnode].reserve == ZL_RSV_ENDFUN)
			{
				state = ZL_ST_DOWN;
			}
			else
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ITS_NOT_ENDFUN);
			break;
		}//switch(state)
	}//while(state != ZL_ST_DOWN)
}

/*
	fun...endfun语句代码块中所有表达式的AST抽象语法树的生成
*/
ZL_INT zengl_fun_stmt_sequence(ZL_VOID * VM_ARG)
{	 
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZL_INT root = -1,p = -1,tmp = -1;

	if(compile->parser_curnode+1 > compile->AST_nodes.count - 1 ||
		!ZENGL_AST_ISTOK_VALIDX(compile->parser_curnode+1))
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_PARSER_CURNODE_OUT_OF_END);

	if(nodes[compile->parser_curnode+1].toktype == ZL_TK_RESERVE &&
		(nodes[compile->parser_curnode+1].reserve == ZL_RSV_ENDFUN)) //如果直接遇到endfun，说明语句块中没有语句，直接返回root,root在最开始设为了-1，所以相当于返回-1，即空节点。
		return root;
	do
	{
		if((tmp = compile->statement(VM_ARG)) == -1) //如果是空节点例如 ;; 两个分号在一起的情况，中间就是一个空语句，此时直接continue跳过空语句，继续下面的语句的语法树生成。
			continue;
		if(p == -1)
			p = tmp;
		else
		{
			nodes[p].nextnode = tmp;  //通过nextnode将语句块里的语句连接成链表。
			p = tmp;
		}
		if(root == -1) //设置语句块的第一条语句为根节点。
			root = tmp;

		if(compile->parser_curnode+1 > compile->AST_nodes.count - 1 ||
			!ZENGL_AST_ISTOK_VALIDX(compile->parser_curnode+1))
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_PARSER_CURNODE_OUT_OF_END);

	}while(!(nodes[compile->parser_curnode+1].toktype == ZL_TK_RESERVE &&
			(nodes[compile->parser_curnode+1].reserve == ZL_RSV_ENDFUN)));
	return root;  //返回第一条表达式的AST的顶级父节点。
}

/*
    switch...case结构的AST生成。
*/
ZL_VOID zengl_switch_stmt(ZL_VOID * VM_ARG,ZL_INT p)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_STATES state = ZL_ST_START;
	ZL_INT origRBracketNodeNum = -1;
	ZL_INT tmp = -1;
	ZL_BOOL final = ZL_FALSE;

	while(state != ZL_ST_DOWN)
	{
		switch(state)
		{
		case ZL_ST_START:
			compile->parser_addcurnode(VM_ARG);
			if(!ZENGL_AST_ISTOKTYPEX(compile->parser_curnode,ZL_TK_LBRACKET))
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_SWITCH_NO_LBRACKET);
			else
				origRBracketNodeNum = compile->detectSetRBracketToSemi(VM_ARG);
			compile->ASTAddNodeChild(VM_ARG,p,compile->express(VM_ARG));
			nodes[origRBracketNodeNum].toktype = ZL_TK_RBRACKET;
			state = ZL_ST_PARSER_STMT_INSWITCH;
			break;
		case ZL_ST_PARSER_STMT_INSWITCH:
			compile->parser_addcurnode(VM_ARG);
			if(nodes[compile->parser_curnode].toktype == ZL_TK_RESERVE)
			{
				switch(nodes[compile->parser_curnode].reserve)
				{
				case ZL_RSV_ENDSWT: //遇到endswt或endswitch表示switch控制结构结束
					state = ZL_ST_DOWN;
					break;
				case ZL_RSV_CASE:
					if(final == ZL_TRUE)
						compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_CASE_MUST_BEFORE_DEFAULT);
					compile->ASTAddNodeChild(VM_ARG,p,compile->parser_curnode);
					tmp = compile->parser_curnode;
					compile->parser_addcurnode(VM_ARG);
					if(ZENGL_AST_ISTOK_VALIDX(compile->parser_curnode) &&
						(nodes[compile->parser_curnode].toktype == ZL_TK_NUM ||
						 nodes[compile->parser_curnode].toktype == ZL_TK_FLOAT ||
						 nodes[compile->parser_curnode].toktype == ZL_TK_STR))
						compile->ASTAddNodeChild(VM_ARG,tmp,compile->parser_curnode); //将case后面的数字或浮点数或字符串节点加入到case的子节点
					else
						compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_CASE_MUST_WITH_NUM_FLOAT_STR); //否则语法错误：switch里的case后面必须是数字，浮点数或字符串
					compile->parser_addcurnode(VM_ARG);
					if(!ZENGL_AST_ISTOKTYPEX(compile->parser_curnode,ZL_TK_COLON))
						compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_CASE_MUST_END_WITH_COLON);
					if((tmp = compile->switch_stmt_sequence(VM_ARG)) != -1)
						compile->ASTAddNodeChild(VM_ARG,p,tmp);
					break;
				case ZL_RSV_DEFAULT:
					if(final == ZL_TRUE)
						compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_DEFAULT_USE_MORE_THAN_ONE);
					compile->ASTAddNodeChild(VM_ARG,p,compile->parser_curnode);
					compile->parser_addcurnode(VM_ARG);
					if(!ZENGL_AST_ISTOKTYPEX(compile->parser_curnode,ZL_TK_COLON))
						compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_DEFAULT_MUST_END_WITH_COLON);
					if((tmp = compile->switch_stmt_sequence(VM_ARG)) != -1)
						compile->ASTAddNodeChild(VM_ARG,p,tmp);
					final = ZL_TRUE;
					break;
				default:
					compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_UNKNOWN_KEYWORD_INSWITCH);
					break;
				}//switch(nodes[compile->parser_curnode].reserve)
			}
			break;
		}//switch(state)
	}//while(state != ZL_ST_DOWN)
}

/*
	switch...case语句代码块中所有表达式的AST抽象语法树的生成
*/
ZL_INT zengl_switch_stmt_sequence(ZL_VOID * VM_ARG)
{	 
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZL_INT root = -1,p = -1,tmp = -1;

	if(compile->parser_curnode+1 > compile->AST_nodes.count - 1 ||
		!ZENGL_AST_ISTOK_VALIDX(compile->parser_curnode+1))
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_PARSER_CURNODE_OUT_OF_END);

	if(nodes[compile->parser_curnode+1].toktype == ZL_TK_RESERVE &&
		(nodes[compile->parser_curnode+1].reserve == ZL_RSV_CASE ||
		 nodes[compile->parser_curnode+1].reserve == ZL_RSV_DEFAULT ||
		 nodes[compile->parser_curnode+1].reserve == ZL_RSV_ENDSWT)) //如果直接遇到case,default,endswt或endswitch，说明语句块中没有语句，直接返回root,root在最开始设为了-1，所以相当于返回-1，即空节点。
		return root;
	do
	{
		if((tmp = compile->statement(VM_ARG)) == -1) //如果是空节点例如 ;; 两个分号在一起的情况，中间就是一个空语句，此时直接continue跳过空语句，继续下面的语句的语法树生成。
			continue;
		if(p == -1)
			p = tmp;
		else
		{
			nodes[p].nextnode = tmp;  //通过nextnode将语句块里的语句连接成链表。
			p = tmp;
		}
		if(root == -1) //设置语句块的第一条语句为根节点。
			root = tmp;

		if(compile->parser_curnode+1 > compile->AST_nodes.count - 1 ||
			!ZENGL_AST_ISTOK_VALIDX(compile->parser_curnode+1))
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_PARSER_CURNODE_OUT_OF_END);

	}while(!(nodes[compile->parser_curnode+1].toktype == ZL_TK_RESERVE &&
			(nodes[compile->parser_curnode+1].reserve == ZL_RSV_CASE ||
			 nodes[compile->parser_curnode+1].reserve == ZL_RSV_DEFAULT ||
			 nodes[compile->parser_curnode+1].reserve == ZL_RSV_ENDSWT)));
	return root;  //返回第一条表达式的AST的顶级父节点。
}

/*
    class...endcls或endclass结构的AST生成。
*/
ZL_VOID zengl_class_stmt(ZL_VOID * VM_ARG,ZL_INT p)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_STATES state = ZL_ST_START;

	while(state != ZL_ST_DOWN)
	{
		switch(state)
		{
		case ZL_ST_START:
			compile->parser_addcurnode(VM_ARG);
			if(!ZENGL_AST_ISTOKTYPEX(compile->parser_curnode,ZL_TK_ID))
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_CLASS_NO_CLASS_NAME);
			compile->ASTAddNodeChild(VM_ARG,p,compile->parser_curnode);
			state = ZL_ST_PARSER_STMT_INCLASS;
			break;
		case ZL_ST_PARSER_STMT_INCLASS:
			compile->ASTAddNodeChild(VM_ARG,p,compile->class_stmt_sequence(VM_ARG));
			compile->parser_addcurnode(VM_ARG);
			if(nodes[compile->parser_curnode].toktype == ZL_TK_RESERVE &&
			   nodes[compile->parser_curnode].reserve == ZL_RSV_ENDCLS)
			{
				state = ZL_ST_DOWN;
			}
			else
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ITS_NOT_ENDCLS);
			break;
		}//switch(state)
	}//while(state != ZL_ST_DOWN)
}

/*
	class...endcls或endclass语句代码块中所有表达式的AST抽象语法树的生成
*/
ZL_INT zengl_class_stmt_sequence(ZL_VOID * VM_ARG)
{	 
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZL_INT root = -1,p = -1,tmp = -1;

	if(compile->parser_curnode+1 > compile->AST_nodes.count - 1 ||
		!ZENGL_AST_ISTOK_VALIDX(compile->parser_curnode+1))
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_PARSER_CURNODE_OUT_OF_END);

	if(nodes[compile->parser_curnode+1].toktype == ZL_TK_RESERVE &&
		(nodes[compile->parser_curnode+1].reserve == ZL_RSV_ENDCLS)) //如果直接遇到endcls或endclass，说明语句块中没有语句，直接返回root,root在最开始设为了-1，所以相当于返回-1，即空节点。
		return root;
	do
	{
		if((tmp = compile->statement(VM_ARG)) == -1) //如果是空节点例如 ;; 两个分号在一起的情况，中间就是一个空语句，此时直接continue跳过空语句，继续下面的语句的语法树生成。
			continue;
		if(p == -1)
			p = tmp;
		else
		{
			nodes[p].nextnode = tmp;  //通过nextnode将语句块里的语句连接成链表。
			p = tmp;
		}
		if(root == -1) //设置语句块的第一条语句为根节点。
			root = tmp;

		switch(nodes[tmp].toktype) //进行语法检查，判断类成员是否是变量标识符，类声明语句或者类方法(类函数)
		{
		case ZL_TK_ID:
		case ZL_TK_CLASS_STATEMENT:
			break;
		default:
			if(nodes[tmp].toktype == ZL_TK_RESERVE &&
			   nodes[tmp].reserve == ZL_RSV_FUN)
			{
				break;
			}
			else
			{
				compile->parser_curnode = tmp;
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_MEMBER_INCLASS);
			}
			break;
		}

		if(compile->parser_curnode+1 > compile->AST_nodes.count - 1 ||
			!ZENGL_AST_ISTOK_VALIDX(compile->parser_curnode+1))
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_PARSER_CURNODE_OUT_OF_END);

	}while(!(nodes[compile->parser_curnode+1].toktype == ZL_TK_RESERVE &&
			(nodes[compile->parser_curnode+1].reserve == ZL_RSV_ENDCLS)));
	return root;  //返回第一条表达式的AST的顶级父节点。
}

/*
    while...endwhile结构的AST生成。
*/
ZL_VOID zengl_while_stmt(ZL_VOID * VM_ARG,ZL_INT p)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_STATES state = ZL_ST_START;
	ZL_INT origRBracketNodeNum = -1;

	while(state != ZL_ST_DOWN)
	{
		switch(state)
		{
		case ZL_ST_START:
			compile->parser_addcurnode(VM_ARG);
			if(!ZENGL_AST_ISTOKTYPEX(compile->parser_curnode,ZL_TK_LBRACKET))
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_WHILE_NO_LBRACKET);
			origRBracketNodeNum = compile->detectSetRBracketToSemi(VM_ARG);
			compile->ASTAddNodeChild(VM_ARG,p,compile->express(VM_ARG));
			nodes[origRBracketNodeNum].toktype = ZL_TK_RBRACKET;
			state = ZL_ST_PARSER_STMT_INWHILE;
			break;
		case ZL_ST_PARSER_STMT_INWHILE:
			compile->ASTAddNodeChild(VM_ARG,p,compile->while_stmt_sequence(VM_ARG));
			compile->parser_addcurnode(VM_ARG);
			if(nodes[compile->parser_curnode].toktype == ZL_TK_RESERVE &&
				nodes[compile->parser_curnode].reserve == ZL_RSV_ENDWHILE)
			{
				state = ZL_ST_DOWN;
			}
			else
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ITS_NOT_ENDWHILE);
			break;
		}//switch(state)
	}//while(state != ZL_ST_DOWN)
}

/*
	while...endwhile语句代码块中所有表达式的AST抽象语法树的生成
*/
ZL_INT zengl_while_stmt_sequence(ZL_VOID * VM_ARG)
{	 
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZL_INT root = -1,p = -1,tmp = -1;

	if(compile->parser_curnode+1 > compile->AST_nodes.count - 1 ||
		!ZENGL_AST_ISTOK_VALIDX(compile->parser_curnode+1))
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_PARSER_CURNODE_OUT_OF_END);

	if(nodes[compile->parser_curnode+1].toktype == ZL_TK_RESERVE &&
		(nodes[compile->parser_curnode+1].reserve == ZL_RSV_ENDWHILE)) //如果直接遇到endwhile，说明语句块中没有语句，直接返回root,root在最开始设为了-1，所以相当于返回-1，即空节点。
		return root;
	do
	{
		if((tmp = compile->statement(VM_ARG)) == -1) //如果是空节点例如 ;; 两个分号在一起的情况，中间就是一个空语句，此时直接continue跳过空语句，继续下面的语句的语法树生成。
			continue;
		if(p == -1)
			p = tmp;
		else
		{
			nodes[p].nextnode = tmp;  //通过nextnode将语句块里的语句连接成链表。
			p = tmp;
		}
		if(root == -1) //设置语句块的第一条语句为根节点。
			root = tmp;

		if(compile->parser_curnode+1 > compile->AST_nodes.count - 1 ||
			!ZENGL_AST_ISTOK_VALIDX(compile->parser_curnode+1))
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_PARSER_CURNODE_OUT_OF_END);

	}while(!(nodes[compile->parser_curnode+1].toktype == ZL_TK_RESERVE &&
			(nodes[compile->parser_curnode+1].reserve == ZL_RSV_ENDWHILE)));
	return root;  //返回第一条表达式的AST的顶级父节点。
}

/*
    do...dowhile结构的AST生成。
*/
ZL_VOID zengl_do_stmt(ZL_VOID * VM_ARG,ZL_INT p)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_STATES state = ZL_ST_START;
	ZL_INT origRBracketNodeNum = -1;

	while(state != ZL_ST_DOWN)
	{
		switch(state)
		{
		case ZL_ST_START:
			compile->ASTAddNodeChild(VM_ARG,p,compile->do_stmt_sequence(VM_ARG));
			compile->parser_addcurnode(VM_ARG);
			if(nodes[compile->parser_curnode].toktype == ZL_TK_RESERVE &&
				nodes[compile->parser_curnode].reserve == ZL_RSV_DOWHILE)
			{
				state = ZL_ST_PARSER_STMT_INDOWHILE;
			}
			else
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ITS_NOT_DOWHILE);
			break;
		case ZL_ST_PARSER_STMT_INDOWHILE:
			compile->parser_addcurnode(VM_ARG);
			if(!ZENGL_AST_ISTOKTYPEX(compile->parser_curnode,ZL_TK_LBRACKET))
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_DOWHILE_NO_LBRACKET);
			origRBracketNodeNum = compile->detectSetRBracketToSemi(VM_ARG);
			compile->ASTAddNodeChild(VM_ARG,p,compile->express(VM_ARG));
			nodes[origRBracketNodeNum].toktype = ZL_TK_RBRACKET;
			state = ZL_ST_DOWN;
			break;
		}	//switch(state)
	}	//while(state != ZL_ST_DOWN)
}

/*
	do...dowhile语句代码块中所有表达式的AST抽象语法树的生成
*/
ZL_INT zengl_do_stmt_sequence(ZL_VOID * VM_ARG)
{	 
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZL_INT root = -1,p = -1,tmp = -1;

	if(compile->parser_curnode+1 > compile->AST_nodes.count - 1 ||
		!ZENGL_AST_ISTOK_VALIDX(compile->parser_curnode+1))
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_PARSER_CURNODE_OUT_OF_END);

	if(nodes[compile->parser_curnode+1].toktype == ZL_TK_RESERVE &&
		(nodes[compile->parser_curnode+1].reserve == ZL_RSV_DOWHILE)) //如果直接遇到dowhile，说明语句块中没有语句，直接返回root,root在最开始设为了-1，所以相当于返回-1，即空节点。
		return root;
	do
	{
		if((tmp = compile->statement(VM_ARG)) == -1) //如果是空节点例如 ;; 两个分号在一起的情况，中间就是一个空语句，此时直接continue跳过空语句，继续下面的语句的语法树生成。
			continue;
		if(p == -1)
			p = tmp;
		else
		{
			nodes[p].nextnode = tmp;  //通过nextnode将语句块里的语句连接成链表。
			p = tmp;
		}
		if(root == -1) //设置语句块的第一条语句为根节点。
			root = tmp;

		if(compile->parser_curnode+1 > compile->AST_nodes.count - 1 ||
			!ZENGL_AST_ISTOK_VALIDX(compile->parser_curnode+1))
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_PARSER_CURNODE_OUT_OF_END);

	}while(!(nodes[compile->parser_curnode+1].toktype == ZL_TK_RESERVE &&
			(nodes[compile->parser_curnode+1].reserve == ZL_RSV_DOWHILE)));
	return root;  //返回第一条表达式的AST的顶级父节点。
}

/*
	将if之类的关键字右侧的右括号临时转为分号，这样就可以得到if括号中的条件判断表达式的AST
*/
ZL_INT zengl_detectSetRBracketToSemi(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZL_INT bracket_count = 0;
	ZL_INT tmpNodeNum = compile->parser_curnode;
	ZL_BOOL isinFor = ZL_FALSE;
	ZL_INT forSemiCount = 0;

	if(nodes[tmpNodeNum].toktype != ZL_TK_LBRACKET)
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_DETECT_SET_RBRACKET_TO_SEMI_FUNC_INVALID_START_TOKEN);

	switch(nodes[tmpNodeNum - 1].reserve)
	{
	case ZL_RSV_IF:
	case ZL_RSV_ELIF:
	case ZL_RSV_SWITCH:
	case ZL_RSV_WHILE:
	case ZL_RSV_DOWHILE:
	case ZL_RSV_FOR:
		if(nodes[tmpNodeNum - 1].reserve == ZL_RSV_FOR)
			isinFor = ZL_TRUE;

		while((++tmpNodeNum) <= compile->AST_nodes.count - 1)
		{
			switch(nodes[tmpNodeNum].toktype)
			{
			case ZL_TK_LBRACKET:
				bracket_count++;
				break;
			case ZL_TK_RBRACKET:
				if(bracket_count == 0)
				{
					if(isinFor && forSemiCount != 2)
					{
						compile->parser_curnode--; //使错误信息指向for
						compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_FOR_STMT_MUST_HAVE_THREE_EXP);
					}
					nodes[tmpNodeNum].toktype = ZL_TK_SEMI;
					return tmpNodeNum;
				}
				else if(bracket_count > 0)
					bracket_count--;
				break;
			case ZL_TK_SEMI:
				if(isinFor == ZL_FALSE)
					compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_LBRACKET_NO_RBRACKET);
				else if((++forSemiCount) >= 3)
				{
					compile->parser_curnode--; //使错误信息指向for
					compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_FOR_STMT_MUST_HAVE_THREE_EXP);
				}
				break;
			}
		}
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_LBRACKET_NO_RBRACKET);
		break;
	default:
		if(nodes[tmpNodeNum - 2].reserve == ZL_RSV_FUN)
		{
			while((++tmpNodeNum) <= compile->AST_nodes.count - 1)
			{
				switch(nodes[tmpNodeNum].toktype)
				{
				case ZL_TK_RBRACKET:
					nodes[tmpNodeNum].toktype = ZL_TK_SEMI;
					return tmpNodeNum;
				case ZL_TK_ID:
					if((tmpNodeNum + 1) <= compile->AST_nodes.count - 1)
					{
						switch(nodes[tmpNodeNum+1].toktype)
						{
						case ZL_TK_COMMA:
						case ZL_TK_RBRACKET:
						case ZL_TK_ASSIGN:
							break;
						default:
							compile->parser_curnode = tmpNodeNum+1; //使错误信息指向无效的token
							compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_FUN_ARGLIST_INVALID_TOKEN);
							break;
						}
					}
					break;
				case ZL_TK_NUM:
				case ZL_TK_FLOAT:
				case ZL_TK_STR:
					if((tmpNodeNum + 1) <= compile->AST_nodes.count - 1)
					{
						switch(nodes[tmpNodeNum+1].toktype)
						{
						case ZL_TK_COMMA:
						case ZL_TK_RBRACKET:
							break;
						default:
							compile->parser_curnode = tmpNodeNum+1; //使错误信息指向无效的token
							compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_FUN_ARGLIST_INVALID_TOKEN);
							break;
						}
					}
					break;
				case ZL_TK_COMMA:
					if((tmpNodeNum + 1) <= compile->AST_nodes.count - 1)
					{
						switch(nodes[tmpNodeNum+1].toktype)
						{
						case ZL_TK_ID:
							break;
						default:
							compile->parser_curnode = tmpNodeNum+1; //使错误信息指向无效的token
							compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_FUN_ARGLIST_INVALID_TOKEN);
							break;
						}
					}
					break;
				case ZL_TK_ASSIGN:
					if((tmpNodeNum + 1) <= compile->AST_nodes.count - 1)
					{
						switch(nodes[tmpNodeNum+1].toktype)
						{
						case ZL_TK_NUM:
						case ZL_TK_FLOAT:
						case ZL_TK_STR:
							break;
						default:
							compile->parser_curnode = tmpNodeNum+1; //使错误信息指向无效的token
							compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_FUN_ARGLIST_INVALID_TOKEN);
							break;
						}
					}
					break;
				default:
					compile->parser_curnode = tmpNodeNum; //使错误信息指向无效的token
					compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_FUN_ARGLIST_INVALID_TOKEN);
					break;
				}	//switch ...
			}	//while ...
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_LBRACKET_NO_RBRACKET);
		}	//if(nodes[tmpNodeNum - 2].reserve == ZL_RSV_FUN)
		break;	//default
	}	//switch(nodes[tmpNodeNum - 1].reserve)
	return -1;
}

/**
	第三个版本的语法分析函数。这是编译引擎里最核心的部分，在上一个版本基础上算法做了调整，
	采用纯状态机加优先级堆栈的方式，比第二个版本的可读性强很多，方便维护和扩展。
*/
ZL_INT zengl_express(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	compile->exp_struct.state = ZL_ST_START;
	compile->exp_struct.p = -1;
	compile->exp_struct.tmpcount = 0;
	compile->exp_struct.tmpnode = -1;
	//compile->exp_struct.express_count++;
	while(compile->exp_struct.state!= ZL_ST_DOWN)
	{
		switch(compile->exp_struct.state)
		{
		case ZL_ST_START:
			compile->parser_addcurnode(VM_ARG);
			switch(nodes[compile->parser_curnode].toktype)
			{
			case ZL_TK_ID:
				compile->exp_struct.state = ZL_ST_INID;
				break;
			case ZL_TK_NUM:
				compile->exp_struct.state = ZL_ST_INNUM;
				break;
			case ZL_TK_FLOAT:
				compile->exp_struct.state = ZL_ST_INFLOAT;
				break;
			case ZL_TK_STR:
				compile->exp_struct.state = ZL_ST_INSTR;
				break;
			case ZL_TK_ASSIGN:
				compile->exp_struct.state = ZL_ST_INASSIGN;
				break;
			case ZL_TK_EQUAL:
				compile->exp_struct.state = ZL_ST_PARSER_INEQUAL;
				break;
			case ZL_TK_GREAT_EQ:
				compile->exp_struct.state = ZL_ST_PARSER_INGREAT_EQ;
				break;
			case ZL_TK_GREAT:
				compile->exp_struct.state = ZL_ST_INGREAT;
				break;
			case ZL_TK_LESS_EQ:
				compile->exp_struct.state = ZL_ST_PARSER_INLESS_EQ;
				break;
			case ZL_TK_LESS:
				compile->exp_struct.state = ZL_ST_INLESS;
				break;
			case ZL_TK_NOT_EQ:
				compile->exp_struct.state = ZL_ST_INNOT_EQ;
				break;
			case ZL_TK_REVERSE:
				compile->exp_struct.state = ZL_ST_PARSER_INREVERSE;
				break;
			case ZL_TK_AND:
				compile->exp_struct.state = ZL_ST_INAND;
				break;
			case ZL_TK_ADDRESS:
				compile->exp_struct.state = ZL_ST_PARSER_INADDRESS;
				break;
			case ZL_TK_OR:
				compile->exp_struct.state = ZL_ST_INOR;
				break;
			case ZL_TK_PLUS_PLUS:
				compile->exp_struct.state = ZL_ST_PARSER_INPLUS_PLUS;
				break;
			case ZL_TK_PLUS_ASSIGN:
				compile->exp_struct.state = ZL_ST_PARSER_INPLUS_ASSIGN;
				break;
			case ZL_TK_PLUS:
				compile->exp_struct.state = ZL_ST_INPLUS;
				break;
			case ZL_TK_MINIS_MINIS:
				compile->exp_struct.state = ZL_ST_PARSER_INMINIS_MINIS;
				break;
			case ZL_TK_MINIS_ASSIGN:
				compile->exp_struct.state = ZL_ST_PARSER_INMINIS_ASSIGN;
				break;
			case ZL_TK_MINIS:
				compile->exp_struct.state = ZL_ST_INMINIS;
				break;
			case ZL_TK_TIMES_ASSIGN:
				compile->exp_struct.state = ZL_ST_PARSER_INTIMES_ASSIGN;
				break;
			case ZL_TK_TIMES:
				compile->exp_struct.state = ZL_ST_INTIMES;
				break;
			case ZL_TK_DIVIDE_ASSIGN:
				compile->exp_struct.state = ZL_ST_PARSER_INDIVIDE_ASSIGN;
				break;
			case ZL_TK_DIVIDE:
				compile->exp_struct.state = ZL_ST_INDIVIDE;
				break;
			case ZL_TK_MOD_ASSIGN:
				compile->exp_struct.state = ZL_ST_PARSER_INMOD_ASSIGN;
				break;
			case ZL_TK_MOD:
				compile->exp_struct.state = ZL_ST_INMOD;
				break;
			case ZL_TK_LBRACKET:
				compile->exp_struct.state = ZL_ST_PARSER_INLBRACKET;
				break;
			case ZL_TK_RBRACKET:
				compile->exp_struct.state = ZL_ST_PARSER_INRBRACKET;
				break;
			case ZL_TK_LMBRACKET:
				compile->exp_struct.state = ZL_ST_PARSER_INLMBRACKET;
				break;
			case ZL_TK_RMBRACKET:
				compile->exp_struct.state = ZL_ST_PARSER_INRMBRACKET;
				break;
			case ZL_TK_SEMI:
				compile->exp_struct.state = ZL_ST_PARSER_INSEMI;
				break;
			case ZL_TK_COMMA:
				compile->exp_struct.state = ZL_ST_PARSER_INCOMMA;
				break;
			case ZL_TK_COLON:
				compile->exp_struct.state = ZL_ST_PARSER_INCOLON;
				break;
			case ZL_TK_QUESTION_MARK:
				compile->exp_struct.state = ZL_ST_PARSER_INQUESTION_MARK;
				break;
			case ZL_TK_DOT:
				compile->exp_struct.state = ZL_ST_PARSER_INDOT;
				break;
			}
			break;
		case ZL_ST_INNUM:
		case ZL_ST_INFLOAT:
		case ZL_ST_INSTR:
		case ZL_ST_INID:
			compile->detectCurnodeSyntax(VM_ARG);
			compile->opLevel_push_stack(VM_ARG,compile->parser_curnode);
			compile->exp_struct.state = ZL_ST_START;
			break;
		case ZL_ST_PARSER_INPLUS_ASSIGN:
		case ZL_ST_PARSER_INMINIS_ASSIGN:
		case ZL_ST_PARSER_INTIMES_ASSIGN:
		case ZL_ST_PARSER_INDIVIDE_ASSIGN:
		case ZL_ST_PARSER_INMOD_ASSIGN:
		case ZL_ST_INASSIGN:
			compile->detectCurnodeSyntax(VM_ARG);
			compile->OpLevelForAssign(VM_ARG);
			break;
		case ZL_ST_PARSER_INEQUAL:
		case ZL_ST_PARSER_INGREAT_EQ:
		case ZL_ST_INGREAT:
		case ZL_ST_PARSER_INLESS_EQ:
		case ZL_ST_INLESS:
		case ZL_ST_INNOT_EQ:
		case ZL_ST_INAND:
		case ZL_ST_INOR:
		case ZL_ST_INPLUS:
		case ZL_ST_INMINIS:
		case ZL_ST_INTIMES:
		case ZL_ST_INDIVIDE:
		case ZL_ST_INMOD:
		case ZL_ST_PARSER_INCOMMA:
		case ZL_ST_PARSER_INDOT:
			compile->detectCurnodeSyntax(VM_ARG);
			compile->OpLevelForTwo(VM_ARG);
			break;
		case ZL_ST_PARSER_INADDRESS:
		case ZL_ST_PARSER_INREVERSE:
			compile->detectCurnodeSyntax(VM_ARG);
			compile->opLevel_push_stack(VM_ARG,compile->parser_curnode);
			compile->exp_struct.state = ZL_ST_START;
			break;
		case ZL_ST_PARSER_INPLUS_PLUS:
		case ZL_ST_PARSER_INMINIS_MINIS:
			compile->OpLevelForPPMM(VM_ARG);
			break;
		case ZL_ST_PARSER_INLMBRACKET:
		case ZL_ST_PARSER_INLBRACKET:
			compile->detectCurnodeSyntax(VM_ARG);
			compile->opLevel_push_stack(VM_ARG,compile->parser_curnode);
			compile->exp_struct.state = ZL_ST_START;
			break;
		case ZL_ST_PARSER_INRBRACKET:
			compile->detectCurnodeSyntax(VM_ARG);
			compile->OpLevelForRBRACKET(VM_ARG);
			break;
		case ZL_ST_PARSER_INRMBRACKET:
			compile->detectCurnodeSyntax(VM_ARG);
			compile->OpLevelForRMBRACKET(VM_ARG);
			break;
		case ZL_ST_PARSER_INCOLON:
			compile->detectCurnodeSyntax(VM_ARG);
			compile->OpLevelForColon(VM_ARG);
			break;
		case ZL_ST_PARSER_INQUESTION_MARK:
			compile->detectCurnodeSyntax(VM_ARG);
			compile->OpLevelForQuestion(VM_ARG);
			break;
		case ZL_ST_PARSER_INSEMI:
			compile->OpLevelForSEMI(VM_ARG);
			compile->exp_struct.state = ZL_ST_DOWN;
			break;
		default:
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_PARSER_EXPRESS_UNKNOWN_STATE);
			break;
		}//switch(compile->exp_struct.state)
	}//while(compile->exp_struct.state!= ZL_ST_DOWN)
	return compile->exp_struct.p;
}

/**
	增加parser_curnode当前语法分析节点的节点号，这个函数可以有效的防止parser_curnode溢出。
*/
ZL_VOID zengl_parser_addcurnode(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	compile->parser_curnode++;
	if(compile->parser_curnode > compile->AST_nodes.count-1)
	{
		compile->parser_curnode--;
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_PARSER_CURNODE_OUT_OF_END);
	}
}

/*语法分析时调用的打印输出错误信息的函数*/
ZL_VOID zengl_parser_errorExit(ZL_VOID * VM_ARG,ZENGL_ERRORNO errorno, ...)  
{
	ZENGL_VM_TYPE * VM = (ZENGL_VM_TYPE *)VM_ARG;
	ZENGL_COMPILE_TYPE * compile = &VM->compile;
	ZENGL_SYS_ARG_LIST arg;
	ZENGL_SYS_ARG_START(arg,errorno);
	VM->errorno = errorno;
	compile->makeInfoString(VM_ARG,&compile->errorFullString, VM->errorString[VM->errorno] , arg);
	compile->makeInfoString(VM_ARG,&compile->errorFullString, ": " , arg);
	ZENGL_SYS_ARG_END(arg);
	compile->printNode(VM_ARG,compile->parser_curnode);  //打印出错节点的信息
	compile->exit(VM_ARG,ZL_ERR_CP_PARSER_ERROR_EXIT);
}

/**
	将某节点压入栈，当需要比较优先级时，再弹出栈
*/
ZL_VOID zengl_opLevel_push_stack(ZL_VOID * VM_ARG,ZL_INT nodenum)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	if(!compile->parser_opLevel_stackList.isInit)
		compile->initOpLevelStackList(VM_ARG);
	if(compile->parser_opLevel_stackList.count == compile->parser_opLevel_stackList.size)
	{
		compile->parser_opLevel_stackList.size += ZL_OPLVL_STACKLIST_SIZE;
		compile->parser_opLevel_stackList.stacks = compile->memReAlloc(VM_ARG,compile->parser_opLevel_stackList.stacks,
																	   compile->parser_opLevel_stackList.size * sizeof(ZENGL_PARSER_OP_LEVEL_STACK_TYPE));
		ZENGL_SYS_MEM_SET(compile->parser_opLevel_stackList.stacks + (compile->parser_opLevel_stackList.size - ZL_OPLVL_STACKLIST_SIZE),0,
								ZL_OPLVL_STACKLIST_SIZE * sizeof(ZENGL_PARSER_OP_LEVEL_STACK_TYPE));
	}
	if(nodenum < 0)
	{
		compile->parser_opLevel_stackList.stacks[compile->parser_opLevel_stackList.count].tok_op_level = ZL_OP_LEVEL_START;
		compile->parser_opLevel_stackList.stacks[compile->parser_opLevel_stackList.count].tokcategory = ZL_TKCG_OP_START;
		compile->parser_opLevel_stackList.stacks[compile->parser_opLevel_stackList.count].nodenum = -1;
	}
	else
	{
		compile->parser_opLevel_stackList.stacks[compile->parser_opLevel_stackList.count].tok_op_level = nodes[nodenum].tok_op_level;
		compile->parser_opLevel_stackList.stacks[compile->parser_opLevel_stackList.count].tokcategory = nodes[nodenum].tokcategory;
		compile->parser_opLevel_stackList.stacks[compile->parser_opLevel_stackList.count].nodenum = nodenum;
	}
	compile->parser_opLevel_stackList.count++;
}

/**
	将最后压入栈的节点弹出。
*/
ZENGL_PARSER_OP_LEVEL_STACK_TYPE zengl_opLevel_pop_stack(ZL_VOID * VM_ARG,ZL_INT index)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_PARSER_OP_LEVEL_STACK_TYPE retstack;
	if(compile->parser_opLevel_stackList.count<=0 ||
		compile->parser_opLevel_stackList.count + index <0)
	{
		retstack.tokcategory = ZL_TKCG_OP_START; //ZL_TKCG_OP_START和ZL_OP_LEVEL_START是对应枚举体中的第一个值，所以表示最低的优先级。
		retstack.tok_op_level = ZL_OP_LEVEL_START;
		retstack.nodenum = -1;
		return retstack;
	}
	retstack = compile->parser_opLevel_stackList.stacks[compile->parser_opLevel_stackList.count + index];
	return retstack;
}

/**
	初始化优先级堆栈动态数组
*/
ZL_VOID zengl_initOpLevelStackList(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	if(compile->parser_opLevel_stackList.isInit)
		return;
	compile->parser_opLevel_stackList.count = 0;
	compile->parser_opLevel_stackList.size = ZL_OPLVL_STACKLIST_SIZE;
	compile->parser_opLevel_stackList.stacks = compile->memAlloc( VM_ARG , compile->parser_opLevel_stackList.size * sizeof(ZENGL_PARSER_OP_LEVEL_STACK_TYPE));
	if(compile->parser_opLevel_stackList.stacks == ZL_NULL)
		compile->exit(VM_ARG,ZL_ERR_CP_PARSER_OPLVL_STACKLIST_MALLOC_FAILED);
	ZENGL_SYS_MEM_SET(compile->parser_opLevel_stackList.stacks , 0, compile->parser_opLevel_stackList.size * sizeof(ZENGL_PARSER_OP_LEVEL_STACK_TYPE));
	compile->parser_opLevel_stackList.isInit = ZL_TRUE;
}

/*根据当前curnode节点和curnode+1节点来初步判断是否有语法错误*/
ZL_VOID zengl_detectCurnodeSyntax(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_TOKENTYPE nextNodeTkType;
	ZENGL_TOKEN_CATEGORY nextNodeTKCG;
	ZL_INT nextNodeNum;
	if(compile->parser_curnode >=0 && compile->parser_curnode < compile->AST_nodes.count-1)
	{
		nextNodeTkType = nodes[compile->parser_curnode + 1].toktype;
		nextNodeTKCG = nodes[compile->parser_curnode + 1].tokcategory;
		nextNodeNum = compile->parser_curnode + 1;
	}
	else
	{
		nextNodeTkType = ZL_TK_START_NONE;
		nextNodeTKCG = ZL_TKCG_OP_START;
		nextNodeNum = -1;
	}
	switch(compile->exp_struct.state)
	{
	case ZL_ST_INNUM:
	case ZL_ST_INFLOAT:
	case ZL_ST_INSTR:
		if(ZENGL_AST_ISTOK_VALIDX(nextNodeNum) && 
			nextNodeTkType != ZL_TK_REVERSE && 
			nextNodeTkType != ZL_TK_ADDRESS && 
			(nextNodeTKCG == ZL_TKCG_OP_COMMA ||
			 nextNodeTKCG == ZL_TKCG_OP_PLUS_MINIS ||
			 nextNodeTKCG == ZL_TKCG_OP_TIM_DIV ||
			 nextNodeTKCG == ZL_TKCG_OP_RELATION ||
			 nextNodeTKCG == ZL_TKCG_OP_LOGIC ||
			 nextNodeTKCG == ZL_TKCG_OP_PP_MM ||
			 nextNodeTKCG == ZL_TKCG_OP_QUESTION ||
			 nextNodeTkType == ZL_TK_RBRACKET ||
			 nextNodeTkType == ZL_TK_RMBRACKET ||
			 nextNodeTkType == ZL_TK_SEMI)) //数字,浮点数,字符串后面必须是除了赋值，点，取反，引用之外的操作运算符或者右括号或者右中括号或者分号
			return;
		else if(nextNodeNum != -1)
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_NUM_FLOAT_STR_INVALID_NEXT_NODE);
		else
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_NEXT_NODE);
		break;
	case ZL_ST_INID:
		if((ZENGL_AST_ISTOKCATEXOP(nextNodeNum) && nextNodeTkType != ZL_TK_REVERSE && nextNodeTkType != ZL_TK_ADDRESS) || 
			(ZENGL_AST_ISTOK_VALIDX(nextNodeNum) && 
			(nextNodeTkType == ZL_TK_LBRACKET ||
			 nextNodeTkType == ZL_TK_LMBRACKET ||
			 nextNodeTkType == ZL_TK_RBRACKET ||
			 nextNodeTkType == ZL_TK_RMBRACKET ||
			 nextNodeTkType == ZL_TK_SEMI))) //变量标识符后面必须是操作运算符(除了取反和引用运算符)或者左括号或者左中括号或右括号或右中括号或分号
			return;
		else if(nextNodeNum != -1)
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ID_INVALID_NEXT_NODE);
		else
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_NEXT_NODE);
		break;
	case ZL_ST_PARSER_INEQUAL:
	case ZL_ST_PARSER_INGREAT_EQ:
	case ZL_ST_INGREAT:
	case ZL_ST_PARSER_INLESS_EQ:
	case ZL_ST_INLESS:
	case ZL_ST_INNOT_EQ:
	case ZL_ST_PARSER_INREVERSE:
	case ZL_ST_INAND:
	case ZL_ST_INOR:
	case ZL_ST_PARSER_INPLUS_ASSIGN:
	case ZL_ST_PARSER_INMINIS_ASSIGN:
	case ZL_ST_PARSER_INTIMES_ASSIGN:
	case ZL_ST_PARSER_INDIVIDE_ASSIGN:
	case ZL_ST_PARSER_INMOD_ASSIGN:
	case ZL_ST_INASSIGN:
	case ZL_ST_INPLUS:
	case ZL_ST_INMINIS:
	case ZL_ST_INTIMES:
	case ZL_ST_INDIVIDE:
	case ZL_ST_INMOD:
		if(ZENGL_AST_ISTOK_VALIDX(nextNodeNum) && 
			(nextNodeTKCG == ZL_TKCG_OP_FACTOR ||
			 nextNodeTKCG == ZL_TKCG_OP_PP_MM ||
			 nextNodeTkType == ZL_TK_REVERSE ||
			 nextNodeTkType == ZL_TK_LBRACKET)) //表达式操作运算符后面必须是变量，数字，字符串等操作因子或者加加减减或者取反运算符或左括号
			 return;
		else if(ZENGL_AST_ISTOK_VALIDX(nextNodeNum) && 
				compile->exp_struct.state == ZL_ST_INASSIGN &&
				nextNodeTkType == ZL_TK_ADDRESS) //如果是赋值语句，还可以在后面接引用运算符
			return;
		else if(nextNodeNum != -1)
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_EXPRESS_OP_INVALID_NEXT_NODE);
		else
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_NEXT_NODE);
		break;
	case ZL_ST_PARSER_INADDRESS:
		if(ZENGL_AST_ISTOK_VALIDX(nextNodeNum) && 
			(nextNodeTkType == ZL_TK_ID)) //引用运算符后面必须是变量之类标识符
			return;
		else if(nextNodeNum != -1)
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ADDRESS_INVALID_NEXT_NODE);
		else
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_NEXT_NODE);
		break;
	case ZL_ST_PARSER_INPLUS_PLUS:
	case ZL_ST_PARSER_INMINIS_MINIS:
		if(ZENGL_AST_ISTOK_VALIDX(nextNodeNum) && 
			nodes[compile->parser_curnode].leftOrRight == ZL_OP_POS_IN_LEFT && 
			(nextNodeTKCG == ZL_TKCG_OP_PP_MM || 
			 nextNodeTKCG == ZL_TKCG_OP_FACTOR ||
			 nextNodeTkType == ZL_TK_REVERSE ||
			 nextNodeTkType == ZL_TK_LBRACKET)) //当加加减减在左侧时，后面只能是加加减减，操作因子，取反或左括号
			 return;
		else if(ZENGL_AST_ISTOK_VALIDX(nextNodeNum) && 
				nodes[compile->parser_curnode].leftOrRight == ZL_OP_POS_IN_RIGHT && 
				nextNodeTkType != ZL_TK_REVERSE && 
				nextNodeTkType != ZL_TK_ADDRESS && 
				(nextNodeTKCG == ZL_TKCG_OP_COMMA ||
				 nextNodeTKCG == ZL_TKCG_OP_PLUS_MINIS ||
				 nextNodeTKCG == ZL_TKCG_OP_TIM_DIV ||
				 nextNodeTKCG == ZL_TKCG_OP_RELATION ||
				 nextNodeTKCG == ZL_TKCG_OP_LOGIC ||
				 nextNodeTKCG == ZL_TKCG_OP_PP_MM ||
				 nextNodeTKCG == ZL_TKCG_OP_QUESTION ||
				 nextNodeTkType == ZL_TK_RBRACKET ||
				 nextNodeTkType == ZL_TK_RMBRACKET ||
				 nextNodeTkType == ZL_TK_SEMI)) //在右侧时，只能是除了赋值，点，取反，引用运算符之外的操作运算符或者右括号或者右中括号或分号
			 return;
		else if(nextNodeNum != -1)
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_PPMM_INVALID_NEXT_NODE);
		else
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_NEXT_NODE);
		break;
	case ZL_ST_PARSER_INLBRACKET:
		if(ZENGL_AST_ISTOK_VALIDX(nextNodeNum) && 
			(nextNodeTKCG == ZL_TKCG_OP_FACTOR ||
			 nextNodeTKCG == ZL_TKCG_OP_PP_MM ||
			 nextNodeTkType == ZL_TK_REVERSE ||
			 nextNodeTkType == ZL_TK_LBRACKET ||
			 nextNodeTkType == ZL_TK_RBRACKET ||
			 nextNodeTkType == ZL_TK_ADDRESS)) //左括号后面的语法条件
			return;
		else if(nextNodeNum != -1)
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_LBRACKET_INVALID_NEXT_NODE);
		else
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_NEXT_NODE);
		break;
	case ZL_ST_PARSER_INLMBRACKET:
		if(compile->parser_opLevel_stackList.count <= 0) //左中括号不可以放在开头
		{
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_START_INVALID_TOKEN,
									compile->getTokenStr(VM_ARG,nodes,compile->parser_curnode),
									compile->getTokenStr(VM_ARG,nodes,compile->parser_curnode));
		}
		else if(ZENGL_AST_ISTOK_VALIDX(nextNodeNum) && 
			(nextNodeTKCG == ZL_TKCG_OP_FACTOR ||
			 nextNodeTKCG == ZL_TKCG_OP_PP_MM ||
			 nextNodeTkType == ZL_TK_REVERSE ||
			 nextNodeTkType == ZL_TK_LBRACKET ||
			 nextNodeTkType == ZL_TK_RMBRACKET)) //左中括号后面的语法条件
		    return;
		else if(nextNodeNum != -1)
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_LMBRACKET_INVALID_NEXT_NODE);
		else
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_NEXT_NODE);
		break;
	case ZL_ST_PARSER_INRBRACKET:
		if(ZENGL_AST_ISTOK_VALIDX(nextNodeNum) && 
			nextNodeTkType != ZL_TK_REVERSE && 
			nextNodeTkType != ZL_TK_ADDRESS && 
			(nextNodeTKCG == ZL_TKCG_OP_COMMA ||
			 nextNodeTKCG == ZL_TKCG_OP_PLUS_MINIS ||
			 nextNodeTKCG == ZL_TKCG_OP_TIM_DIV ||
			 nextNodeTKCG == ZL_TKCG_OP_RELATION ||
			 nextNodeTKCG == ZL_TKCG_OP_LOGIC ||
			 nextNodeTKCG == ZL_TKCG_OP_PP_MM ||
			 nextNodeTKCG == ZL_TKCG_OP_QUESTION ||
			 nextNodeTkType == ZL_TK_RBRACKET ||
			 nextNodeTkType == ZL_TK_RMBRACKET ||
			 nextNodeTkType == ZL_TK_SEMI)) //右括号后面必须是除了赋值，点，取反，引用运算符之外的操作运算符或者右括号或者右中括号或分号
			return;
		else if(nextNodeNum != -1)
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_RBRACKET_INVALID_NEXT_NODE);
		else
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_NEXT_NODE);
		break;
	case ZL_ST_PARSER_INRMBRACKET:
		if(ZENGL_AST_ISTOK_VALIDX(nextNodeNum) && 
			nextNodeTkType != ZL_TK_REVERSE && 
			nextNodeTkType != ZL_TK_ADDRESS && 
			(nextNodeTKCG == ZL_TKCG_OP_COMMA ||
			 nextNodeTKCG == ZL_TKCG_OP_PLUS_MINIS ||
			 nextNodeTKCG == ZL_TKCG_OP_TIM_DIV ||
			 nextNodeTKCG == ZL_TKCG_OP_RELATION ||
			 nextNodeTKCG == ZL_TKCG_OP_LOGIC ||
			 nextNodeTKCG == ZL_TKCG_OP_PP_MM ||
			 nextNodeTKCG == ZL_TKCG_OP_QUESTION ||
			 nextNodeTKCG == ZL_TKCG_OP_ASSIGN ||
			 nextNodeTKCG == ZL_TKCG_OP_DOT ||
			 nextNodeTkType == ZL_TK_RBRACKET ||
			 nextNodeTkType == ZL_TK_RMBRACKET ||
			 nextNodeTkType == ZL_TK_SEMI)) //右中括号后面必须是除了取反，引用运算符之外的操作运算符或者右括号或者右中括号或分号
		    return;
		else if(nextNodeNum != -1)
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_RMBRACKET_INVALID_NEXT_NODE);
		else
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_NEXT_NODE);
		break;
	case ZL_ST_PARSER_INCOLON:
	case ZL_ST_PARSER_INQUESTION_MARK:
	case ZL_ST_PARSER_INCOMMA:
		if(ZENGL_AST_ISTOK_VALIDX(nextNodeNum) && 
			(nextNodeTKCG == ZL_TKCG_OP_FACTOR ||
			 nextNodeTKCG == ZL_TKCG_OP_PP_MM ||
			 nextNodeTkType == ZL_TK_REVERSE ||
			 nextNodeTkType == ZL_TK_LBRACKET ||
			 nextNodeTkType == ZL_TK_ADDRESS)) //逗号,问号,冒号后面的语法条件
			return;
		else if(nextNodeNum != -1)
		{
			if(compile->exp_struct.state == ZL_ST_PARSER_INCOLON)
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_COLON_INVALID_NEXT_NODE);
			else if(compile->exp_struct.state == ZL_ST_PARSER_INQUESTION_MARK)
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_QUESTION_MARK_INVALID_NEXT_NODE);
			else
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_COMMA_INVALID_NEXT_NODE);
		}
		else
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_NEXT_NODE);
		break;
	case ZL_ST_PARSER_INDOT:
		if(ZENGL_AST_ISTOK_VALIDX(nextNodeNum) && 
			(nextNodeTkType == ZL_TK_ID))
			return;
		else if(nextNodeNum != -1)
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_DOT_INVALID_NEXT_NODE);
		else
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_NEXT_NODE);
		break;
	}
}

/*
	使用优先级堆栈处理加减乘除等双目运算符
*/
ZL_VOID zengl_OpLevelForTwo(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_PARSER_OP_LEVEL_STACK_TYPE last_stack;
	ZENGL_PARSER_OP_LEVEL_STACK_TYPE before_stack;
	if(compile->parser_opLevel_stackList.count <= 0 && compile->exp_struct.state != ZL_ST_INMINIS) //只有减号可以以负号的形式放在表达式的开头
	{
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_START_INVALID_TOKEN,
								compile->getTokenStr(VM_ARG,nodes,compile->parser_curnode),
								compile->getTokenStr(VM_ARG,nodes,compile->parser_curnode));
	}
	while(ZL_TRUE) //循环进行堆栈优先级的比较
	{
		last_stack = compile->opLevel_pop_stack(VM_ARG,-1);
		before_stack = compile->opLevel_pop_stack(VM_ARG,-2);
		if(before_stack.tok_op_level >= nodes[compile->parser_curnode].tok_op_level) //通过 ">=" 实现从左到右的结合规则，因为在优先级相等的情况下，左侧先获取子节点
		{
			compile->ASTAddNodeChild(VM_ARG,before_stack.nodenum,last_stack.nodenum);
			compile->parser_opLevel_stackList.count > 0 ? compile->parser_opLevel_stackList.count-- : ZL_TRUE ;
		}
		else
		{
			compile->ASTAddNodeChild(VM_ARG,compile->parser_curnode,last_stack.nodenum);
			compile->parser_opLevel_stackList.count > 0 ? compile->parser_opLevel_stackList.count-- : ZL_TRUE ;
			compile->opLevel_push_stack(VM_ARG,compile->parser_curnode);
			compile->exp_struct.state = ZL_ST_START; //当前状态机优先级处理完毕，准备获取下一个状态机
			break;
		}
	}
}

/*
	使用优先级堆栈处理赋值语句的双目运算符
*/
ZL_VOID zengl_OpLevelForAssign(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_PARSER_OP_LEVEL_STACK_TYPE last_stack;
	ZENGL_PARSER_OP_LEVEL_STACK_TYPE before_stack;
	if(compile->parser_opLevel_stackList.count <= 0) //赋值语句不可以放在表达式开头
	{
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_START_INVALID_TOKEN,
								compile->getTokenStr(VM_ARG,nodes,compile->parser_curnode),
								compile->getTokenStr(VM_ARG,nodes,compile->parser_curnode));
	}
	while(ZL_TRUE) //循环进行堆栈优先级的比较
	{
		last_stack = compile->opLevel_pop_stack(VM_ARG,-1);
		before_stack = compile->opLevel_pop_stack(VM_ARG,-2);
		if(before_stack.tok_op_level > ZL_OP_LEVEL_ASSIGN_WHEN_IN_RIGHT) //目前只有点运算符比右侧赋值符优先级高，通过 ">" 实现从右到左的结合，因为在优先级相等的情况下，右侧先获取子节点
		{
			compile->ASTAddNodeChild(VM_ARG,before_stack.nodenum,last_stack.nodenum);
			compile->parser_opLevel_stackList.count > 0 ? compile->parser_opLevel_stackList.count-- : ZL_TRUE ;
		}
		else
		{
			compile->ASTAddNodeChild(VM_ARG,compile->parser_curnode,last_stack.nodenum);
			compile->parser_opLevel_stackList.count > 0 ? compile->parser_opLevel_stackList.count-- : ZL_TRUE ;
			compile->opLevel_push_stack(VM_ARG,compile->parser_curnode);
			compile->exp_struct.state = ZL_ST_START; //当前状态机优先级处理完毕，准备获取下一个状态机
			break;
		}
	}
}

/*
	使用优先级堆栈处理分号结束符
*/
ZL_VOID zengl_OpLevelForSEMI(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_PARSER_OP_LEVEL_STACK_TYPE last_stack;
	ZENGL_PARSER_OP_LEVEL_STACK_TYPE before_stack;
	if(compile->parser_opLevel_stackList.count <= 0)
	{
		return;
	}
	while(ZL_TRUE)
	{
		last_stack = compile->opLevel_pop_stack(VM_ARG,-1);
		before_stack = compile->opLevel_pop_stack(VM_ARG,-2);
		if(compile->parser_opLevel_stackList.count > 1)
		{
			compile->ASTAddNodeChild(VM_ARG,before_stack.nodenum,last_stack.nodenum);
			compile->parser_opLevel_stackList.count > 0 ? compile->parser_opLevel_stackList.count-- : ZL_TRUE ;
		}
		else if(compile->parser_opLevel_stackList.count == 1)
		{
			compile->exp_struct.p = last_stack.nodenum;
			compile->parser_opLevel_stackList.count > 0 ? compile->parser_opLevel_stackList.count-- : ZL_TRUE ;
			break;
		}
		else
			break;
	}
}

/*
	使用优先级堆栈处理加加减减运算符
*/
ZL_VOID zengl_OpLevelForPPMM(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_PARSER_OP_LEVEL_STACK_TYPE last_stack;
	ZENGL_PARSER_OP_LEVEL_STACK_TYPE before_stack;
	
	if(nodes[compile->parser_curnode].leftOrRight == ZL_OP_POS_START_NONE)
		compile->PPMM_GET_POS(VM_ARG);

	compile->detectCurnodeSyntax(VM_ARG);
	if(nodes[compile->parser_curnode].leftOrRight == ZL_OP_POS_IN_LEFT)
	{
		compile->opLevel_push_stack(VM_ARG,compile->parser_curnode);
		compile->exp_struct.state = ZL_ST_START;
	}
	else
	{
		while(ZL_TRUE) //循环进行堆栈优先级的比较
		{
			last_stack = compile->opLevel_pop_stack(VM_ARG,-1);
			before_stack = compile->opLevel_pop_stack(VM_ARG,-2);
			if(before_stack.tok_op_level >= nodes[compile->parser_curnode].tok_op_level) //通过 ">=" 实现从左到右的结合规则，因为在优先级相等的情况下，左侧先获取子节点
			{
				compile->ASTAddNodeChild(VM_ARG,before_stack.nodenum,last_stack.nodenum);
				compile->parser_opLevel_stackList.count > 0 ? compile->parser_opLevel_stackList.count-- : ZL_TRUE ;
			}
			else
			{
				compile->ASTAddNodeChild(VM_ARG,compile->parser_curnode,last_stack.nodenum);
				compile->parser_opLevel_stackList.count > 0 ? compile->parser_opLevel_stackList.count-- : ZL_TRUE ;
				compile->opLevel_push_stack(VM_ARG,compile->parser_curnode);
				compile->exp_struct.state = ZL_ST_START; //当前状态机优先级处理完毕，准备获取下一个状态机
				break;
			}
		}
	}
}

/*
	获取加加减减运算符的左右侧的位置标识，为了进行优先级的比较，必须知道是左侧还是右侧，同时左侧和右侧的含义也不一样
	左侧的是先加加减减后取值，右侧的是先取值后加加减减
*/
ZL_VOID zengl_PPMM_GET_POS(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZL_INT tmpNodeNum = compile->parser_curnode;

	while((--tmpNodeNum) >= 0)
	{
		if(nodes[tmpNodeNum].tokcategory == ZL_TKCG_OP_PP_MM)
			continue;
		else if(nodes[tmpNodeNum].tokcategory == ZL_TKCG_OP_FACTOR ||
				nodes[tmpNodeNum].toktype == ZL_TK_RBRACKET ||
				nodes[tmpNodeNum].toktype == ZL_TK_RMBRACKET)
		{
			nodes[compile->parser_curnode].leftOrRight = ZL_OP_POS_IN_RIGHT;
			break;
		}
		else
		{
			nodes[compile->parser_curnode].leftOrRight = ZL_OP_POS_IN_LEFT;
			break;
		}
	}
	if(tmpNodeNum < 0)
		nodes[compile->parser_curnode].leftOrRight = ZL_OP_POS_IN_LEFT;

	tmpNodeNum = compile->parser_curnode;
	while((++tmpNodeNum) <= compile->AST_nodes.count - 1)
	{
		if(nodes[tmpNodeNum].tokcategory == ZL_TKCG_OP_PP_MM)
			nodes[tmpNodeNum].leftOrRight = nodes[compile->parser_curnode].leftOrRight;
		else if(nodes[tmpNodeNum].toktype == ZL_TK_REVERSE)
			continue;
		else
			break;
	}
}

/*
	使用优先级堆栈处理右括号
*/
ZL_VOID zengl_OpLevelForRBRACKET(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_PARSER_OP_LEVEL_STACK_TYPE last_stack;
	ZENGL_PARSER_OP_LEVEL_STACK_TYPE before_stack;
	ZENGL_PARSER_OP_LEVEL_STACK_TYPE before_before_stack;
	if(compile->parser_opLevel_stackList.count <= 0) //右括号不可以放在表达式的开头
	{
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_START_INVALID_TOKEN,
								compile->getTokenStr(VM_ARG,nodes,compile->parser_curnode),
								compile->getTokenStr(VM_ARG,nodes,compile->parser_curnode));
	}
	while(ZL_TRUE)
	{
		last_stack = compile->opLevel_pop_stack(VM_ARG,-1);
		before_stack = compile->opLevel_pop_stack(VM_ARG,-2);
		before_before_stack = compile->opLevel_pop_stack(VM_ARG,-3);
		if(compile->parser_opLevel_stackList.count > 1)
		{
			if(nodes[last_stack.nodenum].toktype == ZL_TK_LBRACKET)
			{
				if(before_stack.nodenum >= 0 && nodes[before_stack.nodenum].toktype == ZL_TK_ID) //例如：test();这样的空参数函数调用
				{
					compile->ASTAddNodeChild(VM_ARG,before_stack.nodenum,-1);
					nodes[before_stack.nodenum].toktype = ZL_TK_FUNCALL;
					/*if(ZENGL_AST_ISTOKTYPEX(before_stack.nodenum-1,ZL_TK_DOT) &&
					   (ZENGL_AST_ISTOKTYPEX(before_stack.nodenum-2,ZL_TK_RMBRACKET) ||
					    ZENGL_AST_ISTOKTYPEX(before_stack.nodenum-3,ZL_TK_DOT))) //判断是否是有效的类函数
					{
						compile->parser_curnode = before_stack.nodenum;
						compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_CLASS_FUNCALL);	
					}*/
					compile->parser_opLevel_stackList.count > 0 ? compile->parser_opLevel_stackList.count-- : ZL_TRUE ;
					compile->exp_struct.state = ZL_ST_START; //当前状态机优先级处理完毕，准备获取下一个状态机
					break;
				}
				else
					compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_BRACKET_EMPTY); //普通的括号表达式不能为空，除非是函数的参数括号，因为函数可以没有参数
			}
			else if(nodes[before_stack.nodenum].toktype == ZL_TK_LBRACKET)
			{
				if(before_before_stack.nodenum >=0 && nodes[before_before_stack.nodenum].toktype == ZL_TK_ID) //例如max(a,b);这样的函数调用形式
				{
					compile->ASTAddNodeChild(VM_ARG,before_before_stack.nodenum,last_stack.nodenum);
					compile->parser_opLevel_stackList.count > 0 ? compile->parser_opLevel_stackList.count-- : ZL_TRUE ;
					compile->parser_opLevel_stackList.count > 0 ? compile->parser_opLevel_stackList.count-- : ZL_TRUE ;
					nodes[before_before_stack.nodenum].toktype = ZL_TK_FUNCALL;
					/*if(ZENGL_AST_ISTOKTYPEX(before_before_stack.nodenum-1,ZL_TK_DOT) &&
					   (ZENGL_AST_ISTOKTYPEX(before_before_stack.nodenum-2,ZL_TK_RMBRACKET) ||
					    ZENGL_AST_ISTOKTYPEX(before_before_stack.nodenum-3,ZL_TK_DOT))) //判断是否是有效的类函数
					{
						compile->parser_curnode = before_before_stack.nodenum;
						compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_CLASS_FUNCALL);	
					}*/
				}
				else //否则就是普通的括号表达式，而非函数调用如 2 + (3 - 4);这类的括号表达式
				{
					compile->parser_opLevel_stackList.count > 0 ? compile->parser_opLevel_stackList.count-- : ZL_TRUE ;
					compile->parser_opLevel_stackList.count > 0 ? compile->parser_opLevel_stackList.count-- : ZL_TRUE ;
					compile->opLevel_push_stack(VM_ARG,last_stack.nodenum);
				}
				compile->exp_struct.state = ZL_ST_START; //当前状态机优先级处理完毕，准备获取下一个状态机
				break;
			}
			else //如果没碰到左括号，则构建括号内部的表达式结构。
			{
				compile->ASTAddNodeChild(VM_ARG,before_stack.nodenum,last_stack.nodenum);
				compile->parser_opLevel_stackList.count > 0 ? compile->parser_opLevel_stackList.count-- : ZL_TRUE ;
			}
		}
		else if(compile->parser_opLevel_stackList.count == 1)
		{
			if(nodes[last_stack.nodenum].toktype == ZL_TK_LBRACKET)
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_BRACKET_EMPTY);
			else
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_RBRACKET_NO_LBRACKET); //没有对应的左括号相匹配
		}
		else
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_RBRACKET_NO_LBRACKET);
	}
}

/*
	使用优先级堆栈处理右中括号
*/
ZL_VOID zengl_OpLevelForRMBRACKET(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_PARSER_OP_LEVEL_STACK_TYPE last_stack;
	ZENGL_PARSER_OP_LEVEL_STACK_TYPE before_stack;
	ZENGL_PARSER_OP_LEVEL_STACK_TYPE before_before_stack;
	if(compile->parser_opLevel_stackList.count <= 0) //右中括号不可以放在表达式的开头
	{
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_START_INVALID_TOKEN,
								compile->getTokenStr(VM_ARG,nodes,compile->parser_curnode),
								compile->getTokenStr(VM_ARG,nodes,compile->parser_curnode));
	}
	while(ZL_TRUE)
	{
		last_stack = compile->opLevel_pop_stack(VM_ARG,-1);
		before_stack = compile->opLevel_pop_stack(VM_ARG,-2);
		before_before_stack = compile->opLevel_pop_stack(VM_ARG,-3);
		if(compile->parser_opLevel_stackList.count > 1)
		{
			if(nodes[last_stack.nodenum].toktype == ZL_TK_LMBRACKET)
			{
				if(before_stack.nodenum >= 0 && nodes[before_stack.nodenum].toktype == ZL_TK_ID) //例如：test[] = 2;这样的数组元素形式
				{
					compile->ASTAddNodeChild(VM_ARG,before_stack.nodenum,-1);
					nodes[before_stack.nodenum].toktype = ZL_TK_ARRAY_ITEM;
					compile->parser_opLevel_stackList.count > 0 ? compile->parser_opLevel_stackList.count-- : ZL_TRUE ;
					compile->exp_struct.state = ZL_ST_START; //当前状态机优先级处理完毕，准备获取下一个状态机
					break;
				}
				else
					compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_ARRAY_ITEM); //无效的数组元素格式
			}
			else if(nodes[before_stack.nodenum].toktype == ZL_TK_LMBRACKET)
			{
				if(before_before_stack.nodenum >=0 && nodes[before_before_stack.nodenum].toktype == ZL_TK_ID) //例如test[a,b];这样的数组元素形式
				{
					compile->ASTAddNodeChild(VM_ARG,before_before_stack.nodenum,last_stack.nodenum);
					compile->parser_opLevel_stackList.count > 0 ? compile->parser_opLevel_stackList.count-- : ZL_TRUE ;
					compile->parser_opLevel_stackList.count > 0 ? compile->parser_opLevel_stackList.count-- : ZL_TRUE ;
					nodes[before_before_stack.nodenum].toktype = ZL_TK_ARRAY_ITEM;
					compile->exp_struct.state = ZL_ST_START; //当前状态机优先级处理完毕，准备获取下一个状态机
					break;
				}
				else //[a,b]这样的没有变量标识符的无效数组格式
					compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_ARRAY_ITEM); //无效的数组元素格式
			}
			else //如果没碰到左中括号，则构建中括号内部的表达式结构。
			{
				compile->ASTAddNodeChild(VM_ARG,before_stack.nodenum,last_stack.nodenum);
				compile->parser_opLevel_stackList.count > 0 ? compile->parser_opLevel_stackList.count-- : ZL_TRUE ;
			}
		}
		else if(compile->parser_opLevel_stackList.count == 1)
		{
			if(nodes[last_stack.nodenum].toktype == ZL_TK_LMBRACKET)
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_ARRAY_ITEM); //无效的数组元素格式
			else
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_RMBRACKET_NO_LMBRACKET); //没有对应的左中括号相匹配
		}
		else 
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_RMBRACKET_NO_LMBRACKET); //没有对应的左中括号相匹配
	}
}

/*
	使用优先级堆栈处理问号运算符
*/
ZL_VOID zengl_OpLevelForQuestion(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_PARSER_OP_LEVEL_STACK_TYPE last_stack;
	ZENGL_PARSER_OP_LEVEL_STACK_TYPE before_stack;
	if(compile->parser_opLevel_stackList.count <= 0) //问号不可以放在表达式的开头
	{
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_START_INVALID_TOKEN,
								compile->getTokenStr(VM_ARG,nodes,compile->parser_curnode),
								compile->getTokenStr(VM_ARG,nodes,compile->parser_curnode));
	}
	compile->CheckQstColonValid(VM_ARG);
	while(ZL_TRUE) //循环进行堆栈优先级的比较
	{
		last_stack = compile->opLevel_pop_stack(VM_ARG,-1);
		before_stack = compile->opLevel_pop_stack(VM_ARG,-2);
		if(before_stack.tok_op_level >= nodes[compile->parser_curnode].tok_op_level) //通过 ">=" 实现从左到右的结合规则，因为在优先级相等的情况下，左侧先获取子节点
		{
			compile->ASTAddNodeChild(VM_ARG,before_stack.nodenum,last_stack.nodenum);
			compile->parser_opLevel_stackList.count > 0 ? compile->parser_opLevel_stackList.count-- : ZL_TRUE ;
		}
		else
		{
			compile->ASTAddNodeChild(VM_ARG,compile->parser_curnode,last_stack.nodenum);
			compile->parser_opLevel_stackList.count > 0 ? compile->parser_opLevel_stackList.count-- : ZL_TRUE ;
			compile->opLevel_push_stack(VM_ARG,compile->parser_curnode);
			compile->exp_struct.state = ZL_ST_START; //当前状态机优先级处理完毕，准备获取下一个状态机
			break;
		}
	}
}

/*
	使用优先级堆栈处理冒号运算符
*/
ZL_VOID zengl_OpLevelForColon(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_PARSER_OP_LEVEL_STACK_TYPE last_stack;
	ZENGL_PARSER_OP_LEVEL_STACK_TYPE before_stack;
	ZL_BOOL isinMiddle = ZL_TRUE; //判断赋值语句是否在?...:之间

	if(compile->parser_opLevel_stackList.count <= 0) //冒号不可以放在表达式的开头
	{
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_START_INVALID_TOKEN,
								compile->getTokenStr(VM_ARG,nodes,compile->parser_curnode),
								compile->getTokenStr(VM_ARG,nodes,compile->parser_curnode));
	}
	else if(nodes[compile->parser_curnode].isvalid_qst_colon != ZL_TRUE)
	{
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_COLON_NO_QUESTION_LEFT);
	}
	while(ZL_TRUE) //循环进行堆栈优先级的比较
	{
		last_stack = compile->opLevel_pop_stack(VM_ARG,-1);
		before_stack = compile->opLevel_pop_stack(VM_ARG,-2);
		if(before_stack.tok_op_level >= nodes[compile->parser_curnode].tok_op_level || 
			(isinMiddle == ZL_TRUE && before_stack.tokcategory == ZL_TKCG_OP_ASSIGN)) //通过 ">=" 实现从左到右的结合规则，因为在优先级相等的情况下，左侧先获取子节点，另外赋值语句在..?...:中间时优先级比右侧的冒号高，这样就不需要为..?..:中间的赋值语句添加括号了。
		{
			if(nodes[before_stack.nodenum].toktype == ZL_TK_QUESTION_MARK)
				isinMiddle = ZL_FALSE;
			compile->ASTAddNodeChild(VM_ARG,before_stack.nodenum,last_stack.nodenum);
			compile->parser_opLevel_stackList.count > 0 ? compile->parser_opLevel_stackList.count-- : ZL_TRUE ;
		}
		else
		{
			compile->ASTAddNodeChild(VM_ARG,compile->parser_curnode,last_stack.nodenum);
			compile->parser_opLevel_stackList.count > 0 ? compile->parser_opLevel_stackList.count-- : ZL_TRUE ;
			compile->opLevel_push_stack(VM_ARG,compile->parser_curnode);
			compile->exp_struct.state = ZL_ST_START; //当前状态机优先级处理完毕，准备获取下一个状态机
			break;
		}
	}
}

/*
	检测问号冒号是否一一匹配
*/
ZL_VOID zengl_CheckQstColonValid(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZL_INT tmpNodeNum = compile->parser_curnode;
	ZL_INT bracket_count = 0; //左右括号匹配计数器
	ZL_INT mbracket_count = 0; //左右中括号匹配计数器

	if(nodes[compile->parser_curnode].toktype != ZL_TK_QUESTION_MARK)
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_CHECK_QUESTION_VALID_FUNC_INVALID_TOKEN);

	while((++tmpNodeNum) <= compile->AST_nodes.count - 1)
	{
		switch(nodes[tmpNodeNum].toktype)
		{
		case ZL_TK_LBRACKET:
			bracket_count++;
			break;
		case ZL_TK_LMBRACKET:
			mbracket_count++;
			break;
		case ZL_TK_RBRACKET:
			if((--bracket_count) < 0)
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_QUESTION_NO_COLON_RIGHT);
			break;
		case ZL_TK_RMBRACKET:
			if((--mbracket_count) < 0)
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_QUESTION_NO_COLON_RIGHT);
			break;
		case ZL_TK_SEMI:
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_QUESTION_NO_COLON_RIGHT);
			break;
		case ZL_TK_QUESTION_MARK:
			if(bracket_count == 0 && mbracket_count == 0)
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_QUESTION_NO_COLON_RIGHT);
			break;
		case ZL_TK_COLON:
			if(bracket_count == 0 && mbracket_count == 0)
			{
				nodes[compile->parser_curnode].isvalid_qst_colon = nodes[tmpNodeNum].isvalid_qst_colon = ZL_TRUE;
				return;
			}
			break;
		}
	}
	compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_QUESTION_NO_COLON_RIGHT);
}

/*将child对应的节点加入到parent节点的子节点中。*/
ZL_VOID zengl_ASTAddNodeChild(ZL_VOID * VM_ARG,ZL_INT parent,ZL_INT child)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_CHILD_NODE_TYPE * childs  = &(compile->AST_nodes.nodes[parent].childs);
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;

	if(nodes[parent].toktype == ZL_TK_LBRACKET)
	{
		compile->parser_curnode = parent;
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_LBRACKET_NO_RBRACKET);
	}
	else if(nodes[parent].toktype == ZL_TK_LMBRACKET)
	{
		compile->parser_curnode = parent;
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_LMBRACKET_NO_RMBRACKET); //无效的数组元素格式 此处是没有对应的右中括号
	}

	if(childs->count < ZL_AST_CHILD_NODE_SIZE)
	{
		if(childs->childnum[childs->count] == -1)
			childs->childnum[childs->count++] = child;
		if(child >= 0 && child <= compile->AST_nodes.count-1) //如果没有这条判断，那么当child为-1时就越界了，这个BUG很难查的！！因为他不会在越界的时候告诉你，而是在其他的稀奇古怪的地方提示，要设置内存数据断点，反复调试才能找出来。
			nodes[child].parentnode = parent; //设置child节点的parentnode字段为parent.
		return;
	}
	else if(childs->count == childs->size)
	{
		if(childs->size == ZL_AST_CHILD_NODE_SIZE)
		{
			childs->size += ZL_AST_CHILD_NODE_SIZE;
			childs->extchilds = compile->memAlloc(VM_ARG,(childs->size - ZL_AST_CHILD_NODE_SIZE) * sizeof(ZL_INT));
			ZENGL_SYS_MEM_SET(childs->extchilds,-1,(childs->size - ZL_AST_CHILD_NODE_SIZE) * sizeof(ZL_INT));
		}
		else if(childs->size > ZL_AST_CHILD_NODE_SIZE)
		{
			childs->size += ZL_AST_CHILD_NODE_SIZE;
			childs->extchilds = compile->memReAlloc(VM_ARG,childs->extchilds ,
													(childs->size - ZL_AST_CHILD_NODE_SIZE) * sizeof(ZL_INT));
			ZENGL_SYS_MEM_SET(childs->extchilds + (childs->size - ZL_AST_CHILD_NODE_SIZE * 2),-1 ,
							ZL_AST_CHILD_NODE_SIZE * sizeof(ZL_INT));
		}
	}
	if(childs->extchilds[childs->count - ZL_AST_CHILD_NODE_SIZE] == -1)
	{
		childs->extchilds[childs->count - ZL_AST_CHILD_NODE_SIZE ] = child;
		childs->count++;
	}
	if(child >= 0 && child <= compile->AST_nodes.count-1)
		nodes[child].parentnode = parent;
}
