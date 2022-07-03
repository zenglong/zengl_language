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
	���ļ�ΪAST�﷨������ʽ��
*/

#include "zengl_global.h"

/**
	��token����AST�����﷨��
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
		compile->AST_nodes.size += ZL_AST_SIZE; //ZL_AST_SIZEΪ����ĳ�ʼ��С��Ҳ��ÿ������ʱҪ���ӵĴ�С��������global.h�У�Ĭ��Ϊ200
		compile->AST_nodes.nodes = compile->memReAlloc(VM_ARG,compile->AST_nodes.nodes,
														compile->AST_nodes.size * sizeof(ZENGL_AST_NODE_TYPE));
		ZENGL_SYS_MEM_SET(compile->AST_nodes.nodes + (compile->AST_nodes.size - ZL_AST_SIZE),0,
							ZL_AST_SIZE * sizeof(ZENGL_AST_NODE_TYPE)); //�����ݺ�����ڴ�ռ���0��䡣
	}
	if(compile->AST_nodes.nodes[compile->AST_nodes.count].isvalid == ZL_FALSE)
	{
		compile->AST_nodes.nodes[compile->AST_nodes.count].toktype = token;
		if(token == ZL_TK_ID || token == ZL_TK_NUM || token == ZL_TK_FLOAT || token == ZL_TK_STR) //����Ǳ�ʾ�������֣����������ַ����ͽ���token���ַ�����Ϣ���뵽token�ַ����ػ����С�
		{
			compile->AST_nodes.nodes[compile->AST_nodes.count].strindex = compile->TokenStringPoolAddStr(VM_ARG,compile->tokenInfo.str); //��AST�Ķ�Ӧtoken�ڵ��е�strindex�ֶδ�����ַ�����Ϣ��token�ַ����ػ����е�������
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
		case ZL_TK_PLUS:
		case ZL_TK_MINIS:
			compile->AST_nodes.nodes[compile->AST_nodes.count].tokcategory = ZL_TKCG_OP_PLUS_MINIS;
			compile->AST_nodes.nodes[compile->AST_nodes.count].tok_op_level = ZL_OP_LEVEL_PLUS_MINIS;
			if(compile->AST_nodes.nodes[compile->AST_nodes.count].toktype == ZL_TK_PLUS ||
			   !compile->CheckIsNegative(VM_ARG))
			{
				break;
			}
			compile->AST_nodes.nodes[compile->AST_nodes.count].toktype = ZL_TK_NEGATIVE; //������תΪ���ŵ�Ŀ���������ʹ�ú������REVERSEһ�������ȼ�
		case ZL_TK_BIT_REVERSE:
		case ZL_TK_REVERSE:
		case ZL_TK_ADDRESS:
			compile->AST_nodes.nodes[compile->AST_nodes.count].tokcategory = ZL_TKCG_OP_LOGIC;
			compile->AST_nodes.nodes[compile->AST_nodes.count].tok_op_level = ZL_OP_LEVEL_REVERSE;
			if(compile->AST_nodes.nodes[compile->AST_nodes.count].toktype == ZL_TK_ADDRESS)
			{
				if(compile->CheckIsBitAnd(VM_ARG) == ZL_TRUE) //���"&"�ǰ�λ������������޸�ΪZL_TK_BIT_AND��token
				{
					compile->AST_nodes.nodes[compile->AST_nodes.count].toktype = ZL_TK_BIT_AND;
					compile->AST_nodes.nodes[compile->AST_nodes.count].tokcategory = ZL_TKCG_OP_BITS;
					compile->AST_nodes.nodes[compile->AST_nodes.count].tok_op_level = ZL_OP_LEVEL_BITS;
				}
			}
			break;
		case ZL_TK_BIT_LEFT:
		case ZL_TK_BIT_RIGHT:
		case ZL_TK_BIT_XOR:
		case ZL_TK_BIT_OR:
			compile->AST_nodes.nodes[compile->AST_nodes.count].tokcategory = ZL_TKCG_OP_BITS;
			compile->AST_nodes.nodes[compile->AST_nodes.count].tok_op_level = ZL_OP_LEVEL_BITS;
			break;
		case ZL_TK_PLUS_PLUS:
		case ZL_TK_MINIS_MINIS:
			compile->AST_nodes.nodes[compile->AST_nodes.count].tokcategory = ZL_TKCG_OP_PP_MM;
			compile->AST_nodes.nodes[compile->AST_nodes.count].tok_op_level = ZL_OP_LEVEL_PP_MM;
			break;
		case ZL_TK_MOD: //ȡ��ͳ˳�������ͬ�����ȼ���
		case ZL_TK_TIMES:
		case ZL_TK_DIVIDE:
			compile->AST_nodes.nodes[compile->AST_nodes.count].tokcategory = ZL_TKCG_OP_TIM_DIV;
			compile->AST_nodes.nodes[compile->AST_nodes.count].tok_op_level = ZL_OP_LEVEL_TIM_DIV;
			break;
		case ZL_TK_BIT_LEFT_ASSIGN:
		case ZL_TK_BIT_RIGHT_ASSIGN:
		case ZL_TK_BIT_XOR_ASSIGN:
		case ZL_TK_BIT_OR_ASSIGN:
		case ZL_TK_BIT_AND_ASSIGN:
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
			if(compile->SymIsSelfToken(VM_ARG, ZL_NULL)) {
				compile->SymAddNodeNumToSelfClassTable(VM_ARG, compile->AST_nodes.count);
			}
			/* no break */
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
		compile->AST_nodes.nodes[compile->AST_nodes.count].childs.size = ZL_AST_CHILD_NODE_SIZE; //����ӵ�AST�ڵ��Ĭ���ӽڵ�������г�ʼ����Ĭ���ӽڵ�������Դ��3���ӽڵ���Ϣ��3�����ϵĲ��ִ����extchilds��չ�ӽڵ㲿��
		for(i=0;i<compile->AST_nodes.nodes[compile->AST_nodes.count].childs.size;i++)
			compile->AST_nodes.nodes[compile->AST_nodes.count].childs.childnum[i] = -1;
		compile->AST_nodes.nodes[compile->AST_nodes.count].parentnode = -1;
		compile->AST_nodes.count++;
		switch(compile->AST_nodes.nodes[compile->AST_nodes.count-1].reserve) //�ж�if,while,fun֮��Ľṹ�Ƿ�����������if�Ƿ���endif��ƥ���
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
			compile->SymSelfClassTable.cur_class_nodenum = (compile->AST_nodes.count - 1);
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
				compile->SymSelfClassTable.cur_class_nodenum = -1;
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
	��ʼ��AST�����﷨���Ķ�̬���顣
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

/*��token�ַ����ض�̬��������ַ����������ظ��ַ����ڻ����е�������Ϣ*/
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
			compile->Token_StringPool.count + len + 1 > compile->Token_StringPool.size) //������װ��ʱ���Ͷ�token�ַ����ؽ������ݡ�
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

/*��ʼ��token�ַ����صĶ�̬����*/
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

/*��ӡtoken����Ϣ*/
ZL_VOID zengl_printNode(ZL_VOID * VM_ARG , ZL_INT nodenum)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_AST_NODE_TYPE * node = nodes + nodenum;

	compile->info(VM_ARG,"nodenum:%d ",nodenum);
	if(nodenum == -1) //����ڵ����-1˵����NULL�սڵ㣬��ӡ��NULL nodeȻ�󷵻ء�
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
		case ZL_TK_ARRAY_ITEM: //����Ԫ��
			compile->info(VM_ARG,"array token: %s " , compile->getTokenStr(VM_ARG,nodes,nodenum));
			break;
		case ZL_TK_ADDRESS: //����
			compile->info(VM_ARG,"address token: %s " , compile->getTokenStr(VM_ARG,nodes,nodenum));
			break;
		case ZL_TK_RESERVE:
			compile->info(VM_ARG,"reserve token: %s ", compile->getTokenStr(VM_ARG,nodes,nodenum));
			break;
		case ZL_TK_BIT_REVERSE:
			compile->info(VM_ARG,"bit reserve token: %s ", compile->getTokenStr(VM_ARG,nodes,nodenum));
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
		case ZL_TK_NEGATIVE:
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
		case ZL_TK_BIT_AND:
		case ZL_TK_BIT_AND_ASSIGN:
		case ZL_TK_BIT_OR:
		case ZL_TK_BIT_OR_ASSIGN:
		case ZL_TK_BIT_XOR:
		case ZL_TK_BIT_XOR_ASSIGN:
		case ZL_TK_BIT_RIGHT:
		case ZL_TK_BIT_RIGHT_ASSIGN:
		case ZL_TK_BIT_LEFT:
		case ZL_TK_BIT_LEFT_ASSIGN:
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

/*���ݽڵ�Ż�ȡtoken��Ӧ���ַ�����Ϣ*/
ZL_CHAR * zengl_getTokenStr(ZL_VOID * VM_ARG , ZENGL_AST_NODE_TYPE * nodes,ZL_INT nodenum)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	if(nodenum < 0) //��Ϊ-1�սڵ�ʱ��
		return ZL_NULL;
	if(ZENGL_AST_ISTOKCATEX(nodenum,ZL_TKCG_OP_FACTOR)) //����Ǳ�ʾ���Ȳ������ӣ��ʹ�token�ַ����ػ����л�ȡ�ַ�����Ϣ�� 
	{
		return compile->TokenStringPoolGetPtr(VM_ARG,nodes[nodenum].strindex);
	}
	else if(ZENGL_AST_ISTOKTYPEX(nodenum,ZL_TK_RESERVE)) //�����if,else֮��Ĺؼ��֣��ʹ�reserveString�ؼ��������л�ȡ�ַ�����Ϣ��
	{
		return (ZL_CHAR *)compile->reserveString[nodes[nodenum].reserve];
	}
	else //����Ǳ��ʽ�����֮��ģ��ʹ�TokenOperateString�����л�ȡ�ַ�����Ϣ��
	{
		if(nodes[nodenum].toktype < compile->TokenOperateStringCount)
			return (ZL_CHAR *)compile->TokenOperateString[nodes[nodenum].toktype];
		else
			return ZL_NULL;
	}
}

/*��������index��token�ַ����ػ����еõ���Ӧ���ַ�����Ϣ*/
ZL_CHAR * zengl_TokenStringPoolGetPtr(ZL_VOID * VM_ARG , ZL_INT index)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	if(index < 2 || index >  compile->Token_StringPool.count - 1)
		return ZL_NULL;
	return compile->Token_StringPool.ptr + index;
}

/*
	��ӡ��AST�����﷨�������нڵ���Ϣ���������µ�ASTɨ���ջ�ķ������б���ɨ�衣
*/
ZL_VOID zengl_printASTnodes(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_SCAN_STACK_TYPE tmpstack;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZL_INT nodenum;
	ZL_CLOCK_T start_time = ZENGL_SYS_TIME_CLOCK();
	ZL_CLOCK_T end_time;

	compile->push_AST_TreeScanStack(VM_ARG,compile->AST_nodes.rootnode); //��AST���ڵ�ѹ��ջ����ʾ�Ӹ��ڵ㿪ʼ����AST������нڵ���Ϣȫ��������ӡ�����
	do{
		tmpstack = compile->pop_AST_TreeScanStack(VM_ARG,ZL_FALSE); //����ǰ��ѹ��ջ�Ľڵ���Ϣ������ZL_FALSE��ʾֻ������Ϣ���ݲ����ڵ�Ӷ�ջ��ɾ����
		if(tmpstack.curchild == 0) //curchild��ʾ��ǰ����ɨ����ӽڵ�������0��ʾ��û��ʼɨ���ӽڵ㣬�ͽ���ǰ�ڵ���Ϣ��ӡ�����
			compile->printNode(VM_ARG,tmpstack.nodenum);
		if(tmpstack.childcnt > 0) //������صĽڵ�������ӽڵ㣬���ӽڵ���Ϣ��ӡ������
		{
			if(tmpstack.curchild < tmpstack.childcnt) //��curchildС��childcnt�ӽڵ���ʱ��˵���ӽڵ㻹ûȫ����ӡ�꣬�ͼ�����ӡ�������ӽڵ���Ϣ��
			{
				if(tmpstack.curchild == 0) //����ʾ��ǰ�ڵ�һ���ж��ٸ��ӽڵ㡣
					compile->info(VM_ARG,"(%d) %s has %d childs: \n",tmpstack.nodenum,
									compile->getTokenStr(VM_ARG,nodes,tmpstack.nodenum),tmpstack.childcnt);
				if(tmpstack.curchild < ZL_AST_CHILD_NODE_SIZE) //���С��ZL_AST_CHILD_NODE_SIZE,�ͽ�childnum��Ļ����ӽڵ���Ϣ��ӡ����������ͽ�extchilds��չ�ӽڵ���Ľڵ���Ϣ��ӡ������
					nodenum = nodes[tmpstack.nodenum].childs.childnum[tmpstack.curchild];
				else
					nodenum = nodes[tmpstack.nodenum].childs.extchilds[tmpstack.curchild -
																	   ZL_AST_CHILD_NODE_SIZE];
				compile->info(VM_ARG,"(%d) %s (%d) child is ",tmpstack.nodenum,
								compile->getTokenStr(VM_ARG,nodes,tmpstack.nodenum),tmpstack.curchild);
				compile->AST_TreeScanStackList.stacks[compile->AST_TreeScanStackList.count - 1].curchild++; //����ǰɨ��Ľڵ�������һ�������´ξͿ���ɨ����һ���ӽڵ㡣
				compile->push_AST_TreeScanStack(VM_ARG,nodenum); //����ǰɨ����ӽڵ�ѹ��ջ�������ͺ�֮ǰѹ��ջ�Ľڵ��ڶ�ջ�й�����һ���ڵ�·����
				continue; //continue�������do...while��ͷ��Ȼ��pop_AST_TreeScanStack��printNode�ͻὫ��ѹ��ջ���ӽڵ���Ϣ��ӡ������
			}
			else
				compile->info(VM_ARG,"(%d) %s childs end \n",tmpstack.nodenum,
								compile->getTokenStr(VM_ARG,nodes,tmpstack.nodenum),tmpstack.childcnt); //�ӽڵ�ɨ����ϡ�
		}
		if(tmpstack.next != 0) //�����ǰ�ڵ���next�ֵܽڵ㣬�ͽ���ǰ�ڵ㵯��������next�ڵ�ѹ��ջ
		{
			compile->info(VM_ARG,"(%d) %s has nextnode: ",tmpstack.nodenum,
								compile->getTokenStr(VM_ARG,nodes,tmpstack.nodenum));
			compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE);
			compile->push_AST_TreeScanStack(VM_ARG,tmpstack.next);
			continue; //continue�������do...while��ͷ��Ȼ��pop_AST_TreeScanStack��printNode�ͻὫ��ѹ��ջ��next�ڵ���Ϣ��ӡ������
		}
		else
			compile->pop_AST_TreeScanStack(VM_ARG,ZL_TRUE); //���ӽڵ��next�ڵ���Ϣ����ӡ���ˣ��ͽ��ڵ�Ӷ�ջ�е�����
	}while(compile->AST_TreeScanStackList.count > 0); //�����ջ�л���Ԫ�أ�˵�����нڵ���Ϣû��ӡ�ֻ꣬�е���ջ���Ԫ�ظ���Ϊ0ʱ���ʾ����AST��Ľڵ���Ϣ����ӡ���ˣ��Ϳ�������ѭ�������ˡ�
	end_time = ZENGL_SYS_TIME_CLOCK();
	compile->total_print_time += end_time - start_time;
}

/*
	ASTɨ���ջ��ѹջ��������ջɨ��AST��ԭ����ʵ���ǽ�ɨ����Ľڵ�·�����浽��ջ�У�������ɨ�赽�ײ�ʱ���Ϳ���ͨ��zengl_pop_AST_TreeScanStack���ڵ�·�����ε������Ӷ�����
	��ԭʼ��㣬����ԭʼ������һ��ɨ��㿪ʼ�������д��ϵ��£������ҵ�ɨ�裬ѭ����ȥ��ֱ��������Ҫɨ���AST����ȫ��ɨ����Ϊֹ��
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
	ASTɨ���ջ�ĵ���ջ����������remove�ж��Ƿ���Ҫ�ڷ��ؽڵ���Ϣ��ͬʱ���ڵ�Ӷ�ջ��ɾ����
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
	��ʼ��ASTɨ���ջAST_TreeScanStackList��ԭ����ASTɨ����ͨ���ݹ麯�����õķ��������ַ�����Ȼ�������򵥣����ǹ���ĵݹ�ᵼ�¶�ջ�������corrupt���ƻ���
	���ö�̬����ģ���ջ�ķ�����һ����ȵݹ���ÿ���С����һ���治���ƻ����̵Ķ�ջ�ռ䡣
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
	�ú���ͨ��ѭ���������������AST
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
	����������print 'hello'֮�������AST�﷨����statement�����express���������汾���﷨��������������������б��ʽ��AST
*/
ZL_INT zengl_statement(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZL_INT p, child_exp_no;
	
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
			child_exp_no = compile->express(VM_ARG);
			if(child_exp_no < 0) { // ��child_exp_noС��0ʱ��˵��print������һ����Ч�ı��ʽ������ print ; �����������Ч����䣬��ʱ�᷵����Ӧ���﷨����
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_EXP_AFTER_PRINT);
			}
			else
				compile->ASTAddNodeChild(VM_ARG,p,child_exp_no);
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
	�ؼ��������Լ�飬����if�ṹ������endif��ƥ���
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
	use��global��break��continue���������������﷨���
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
	if-elif-else��������AST�����﷨�������ɺ�����
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
		case ZL_ST_PARSER_STMT_INIF: //����if,elif,else�Ĵ�����AST
			compile->ASTAddNodeChild(VM_ARG,orig_p,compile->if_stmt_sequence(VM_ARG));
			compile->parser_addcurnode(VM_ARG);
			if(nodes[compile->parser_curnode].toktype == ZL_TK_RESERVE)
			{
				switch(nodes[compile->parser_curnode].reserve)
				{
				case ZL_RSV_ENDIF: //����endif��ʾif���ƽṹ����
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
	if-elif-else-endif������������б��ʽ��AST�����﷨��������
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
		 nodes[compile->parser_curnode+1].reserve == ZL_RSV_ENDIF)) //���ֱ������elif,else,endif��˵��������û����䣬ֱ�ӷ���root,root���ʼ��Ϊ��-1�������൱�ڷ���-1�����սڵ㡣
		return root;
	do
	{
		if((tmp = compile->statement(VM_ARG)) == -1) //����ǿսڵ����� ;; �����ֺ���һ���������м����һ������䣬��ʱֱ��continue��������䣬��������������﷨�����ɡ�
			continue;
		if(p == -1)
			p = tmp;
		else
		{
			nodes[p].nextnode = tmp;  //ͨ��nextnode���������������ӳ�����
			p = tmp;
		}
		if(root == -1) //��������ĵ�һ�����Ϊ���ڵ㡣
			root = tmp;

		if(compile->parser_curnode+1 > compile->AST_nodes.count - 1 ||
			!ZENGL_AST_ISTOK_VALIDX(compile->parser_curnode+1))
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_PARSER_CURNODE_OUT_OF_END);

	}while(!(nodes[compile->parser_curnode+1].toktype == ZL_TK_RESERVE &&
			(nodes[compile->parser_curnode+1].reserve == ZL_RSV_ELIF ||
			 nodes[compile->parser_curnode+1].reserve == ZL_RSV_ELSE ||
			 nodes[compile->parser_curnode+1].reserve == ZL_RSV_ENDIF)));
	return root;  //���ص�һ�����ʽ��AST�Ķ������ڵ㡣
}

/*
	for...endfor��������AST�����﷨�������ɺ�����
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
		case ZL_ST_PARSER_STMT_INFOR: //����for...endfor�Ĵ�����AST
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
	for...endfor������������б��ʽ��AST�����﷨��������
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
		(nodes[compile->parser_curnode+1].reserve == ZL_RSV_ENDFOR)) //���ֱ������endfor��˵��������û����䣬ֱ�ӷ���root,root���ʼ��Ϊ��-1�������൱�ڷ���-1�����սڵ㡣
		return root;
	do
	{
		if((tmp = compile->statement(VM_ARG)) == -1) //����ǿսڵ����� ;; �����ֺ���һ���������м����һ������䣬��ʱֱ��continue��������䣬��������������﷨�����ɡ�
			continue;
		if(p == -1)
			p = tmp;
		else
		{
			nodes[p].nextnode = tmp;  //ͨ��nextnode���������������ӳ�����
			p = tmp;
		}
		if(root == -1) //��������ĵ�һ�����Ϊ���ڵ㡣
			root = tmp;

		if(compile->parser_curnode+1 > compile->AST_nodes.count - 1 ||
			!ZENGL_AST_ISTOK_VALIDX(compile->parser_curnode+1))
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_PARSER_CURNODE_OUT_OF_END);

	}while(!(nodes[compile->parser_curnode+1].toktype == ZL_TK_RESERVE &&
			(nodes[compile->parser_curnode+1].reserve == ZL_RSV_ENDFOR)));
	return root;  //���ص�һ�����ʽ��AST�Ķ������ڵ㡣
}

/*
	fun...endfun��AST�����﷨�������ɺ�����
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
				compile->ASTAddNodeChild(VM_ARG,p,compile->parser_curnode); //���������ڵ���뵽fun�ڵ㡣

			compile->parser_addcurnode(VM_ARG);
			if(!ZENGL_AST_ISTOKTYPEX(compile->parser_curnode,ZL_TK_LBRACKET))
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_FUN_NO_LBRACKET);
			else
				origRBracketNodeNum = compile->detectSetRBracketToSemi(VM_ARG);
			compile->ASTAddNodeChild(VM_ARG,p,compile->express(VM_ARG)); //�õ�fun�Ĳ����б��AST
			nodes[origRBracketNodeNum].toktype = ZL_TK_RBRACKET;
			state = ZL_ST_PARSER_STMT_INFUN;
			break;
		case ZL_ST_PARSER_STMT_INFUN: //����fun...endfun�Ĵ�����AST
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
	fun...endfun������������б��ʽ��AST�����﷨��������
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
		(nodes[compile->parser_curnode+1].reserve == ZL_RSV_ENDFUN)) //���ֱ������endfun��˵��������û����䣬ֱ�ӷ���root,root���ʼ��Ϊ��-1�������൱�ڷ���-1�����սڵ㡣
		return root;
	do
	{
		if((tmp = compile->statement(VM_ARG)) == -1) //����ǿսڵ����� ;; �����ֺ���һ���������м����һ������䣬��ʱֱ��continue��������䣬��������������﷨�����ɡ�
			continue;
		if(p == -1)
			p = tmp;
		else
		{
			nodes[p].nextnode = tmp;  //ͨ��nextnode���������������ӳ�����
			p = tmp;
		}
		if(root == -1) //��������ĵ�һ�����Ϊ���ڵ㡣
			root = tmp;

		if(compile->parser_curnode+1 > compile->AST_nodes.count - 1 ||
			!ZENGL_AST_ISTOK_VALIDX(compile->parser_curnode+1))
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_PARSER_CURNODE_OUT_OF_END);

	}while(!(nodes[compile->parser_curnode+1].toktype == ZL_TK_RESERVE &&
			(nodes[compile->parser_curnode+1].reserve == ZL_RSV_ENDFUN)));
	return root;  //���ص�һ�����ʽ��AST�Ķ������ڵ㡣
}

/*
    switch...case�ṹ��AST���ɡ�
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
				case ZL_RSV_ENDSWT: //����endswt��endswitch��ʾswitch���ƽṹ����
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
						compile->ASTAddNodeChild(VM_ARG,tmp,compile->parser_curnode); //��case��������ֻ򸡵������ַ����ڵ���뵽case���ӽڵ�
					else
						compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_CASE_MUST_WITH_NUM_FLOAT_STR); //�����﷨����switch���case������������֣����������ַ���
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
	switch...case������������б��ʽ��AST�����﷨��������
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
		 nodes[compile->parser_curnode+1].reserve == ZL_RSV_ENDSWT)) //���ֱ������case,default,endswt��endswitch��˵��������û����䣬ֱ�ӷ���root,root���ʼ��Ϊ��-1�������൱�ڷ���-1�����սڵ㡣
		return root;
	do
	{
		if((tmp = compile->statement(VM_ARG)) == -1) //����ǿսڵ����� ;; �����ֺ���һ���������м����һ������䣬��ʱֱ��continue��������䣬��������������﷨�����ɡ�
			continue;
		if(p == -1)
			p = tmp;
		else
		{
			nodes[p].nextnode = tmp;  //ͨ��nextnode���������������ӳ�����
			p = tmp;
		}
		if(root == -1) //��������ĵ�һ�����Ϊ���ڵ㡣
			root = tmp;

		if(compile->parser_curnode+1 > compile->AST_nodes.count - 1 ||
			!ZENGL_AST_ISTOK_VALIDX(compile->parser_curnode+1))
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_PARSER_CURNODE_OUT_OF_END);

	}while(!(nodes[compile->parser_curnode+1].toktype == ZL_TK_RESERVE &&
			(nodes[compile->parser_curnode+1].reserve == ZL_RSV_CASE ||
			 nodes[compile->parser_curnode+1].reserve == ZL_RSV_DEFAULT ||
			 nodes[compile->parser_curnode+1].reserve == ZL_RSV_ENDSWT)));
	return root;  //���ص�һ�����ʽ��AST�Ķ������ڵ㡣
}

/*
    class...endcls��endclass�ṹ��AST���ɡ�
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
	class...endcls��endclass������������б��ʽ��AST�����﷨��������
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
		(nodes[compile->parser_curnode+1].reserve == ZL_RSV_ENDCLS)) //���ֱ������endcls��endclass��˵��������û����䣬ֱ�ӷ���root,root���ʼ��Ϊ��-1�������൱�ڷ���-1�����սڵ㡣
		return root;
	do
	{
		if((tmp = compile->statement(VM_ARG)) == -1) //����ǿսڵ����� ;; �����ֺ���һ���������м����һ������䣬��ʱֱ��continue��������䣬��������������﷨�����ɡ�
			continue;
		if(p == -1)
			p = tmp;
		else
		{
			nodes[p].nextnode = tmp;  //ͨ��nextnode���������������ӳ�����
			p = tmp;
		}
		if(root == -1) //��������ĵ�һ�����Ϊ���ڵ㡣
			root = tmp;

		switch(nodes[tmp].toktype) //�����﷨��飬�ж����Ա�Ƿ��Ǳ�����ʶ�����������������෽��(�ຯ��)
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
	return root;  //���ص�һ�����ʽ��AST�Ķ������ڵ㡣
}

/*
    while...endwhile�ṹ��AST���ɡ�
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
	while...endwhile������������б��ʽ��AST�����﷨��������
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
		(nodes[compile->parser_curnode+1].reserve == ZL_RSV_ENDWHILE)) //���ֱ������endwhile��˵��������û����䣬ֱ�ӷ���root,root���ʼ��Ϊ��-1�������൱�ڷ���-1�����սڵ㡣
		return root;
	do
	{
		if((tmp = compile->statement(VM_ARG)) == -1) //����ǿսڵ����� ;; �����ֺ���һ���������м����һ������䣬��ʱֱ��continue��������䣬��������������﷨�����ɡ�
			continue;
		if(p == -1)
			p = tmp;
		else
		{
			nodes[p].nextnode = tmp;  //ͨ��nextnode���������������ӳ�����
			p = tmp;
		}
		if(root == -1) //��������ĵ�һ�����Ϊ���ڵ㡣
			root = tmp;

		if(compile->parser_curnode+1 > compile->AST_nodes.count - 1 ||
			!ZENGL_AST_ISTOK_VALIDX(compile->parser_curnode+1))
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_PARSER_CURNODE_OUT_OF_END);

	}while(!(nodes[compile->parser_curnode+1].toktype == ZL_TK_RESERVE &&
			(nodes[compile->parser_curnode+1].reserve == ZL_RSV_ENDWHILE)));
	return root;  //���ص�һ�����ʽ��AST�Ķ������ڵ㡣
}

/*
    do...dowhile�ṹ��AST���ɡ�
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
	do...dowhile������������б��ʽ��AST�����﷨��������
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
		(nodes[compile->parser_curnode+1].reserve == ZL_RSV_DOWHILE)) //���ֱ������dowhile��˵��������û����䣬ֱ�ӷ���root,root���ʼ��Ϊ��-1�������൱�ڷ���-1�����սڵ㡣
		return root;
	do
	{
		if((tmp = compile->statement(VM_ARG)) == -1) //����ǿսڵ����� ;; �����ֺ���һ���������м����һ������䣬��ʱֱ��continue��������䣬��������������﷨�����ɡ�
			continue;
		if(p == -1)
			p = tmp;
		else
		{
			nodes[p].nextnode = tmp;  //ͨ��nextnode���������������ӳ�����
			p = tmp;
		}
		if(root == -1) //��������ĵ�һ�����Ϊ���ڵ㡣
			root = tmp;

		if(compile->parser_curnode+1 > compile->AST_nodes.count - 1 ||
			!ZENGL_AST_ISTOK_VALIDX(compile->parser_curnode+1))
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_PARSER_CURNODE_OUT_OF_END);

	}while(!(nodes[compile->parser_curnode+1].toktype == ZL_TK_RESERVE &&
			(nodes[compile->parser_curnode+1].reserve == ZL_RSV_DOWHILE)));
	return root;  //���ص�һ�����ʽ��AST�Ķ������ڵ㡣
}

/*
	��if֮��Ĺؼ����Ҳ����������ʱתΪ�ֺţ������Ϳ��Եõ�if�����е������жϱ��ʽ��AST
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
						compile->parser_curnode--; //ʹ������Ϣָ��for
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
					compile->parser_curnode--; //ʹ������Ϣָ��for
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
							compile->parser_curnode = tmpNodeNum+1; //ʹ������Ϣָ����Ч��token
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
							compile->parser_curnode = tmpNodeNum+1; //ʹ������Ϣָ����Ч��token
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
							compile->parser_curnode = tmpNodeNum+1; //ʹ������Ϣָ����Ч��token
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
						case ZL_TK_NEGATIVE:
							break;
						default:
							compile->parser_curnode = tmpNodeNum+1; //ʹ������Ϣָ����Ч��token
							compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_FUN_ARGLIST_INVALID_TOKEN);
							break;
						}
					}
					break;
				case ZL_TK_NEGATIVE:
					if((tmpNodeNum + 1) <= compile->AST_nodes.count - 1)
					{
						switch(nodes[tmpNodeNum+1].toktype)
						{
						case ZL_TK_NUM:
						case ZL_TK_FLOAT:
							break;
						default:
							compile->parser_curnode = tmpNodeNum+1; //ʹ������Ϣָ����Ч��token
							compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_FUN_ARGLIST_INVALID_TOKEN);
							break;
						}
					}
					break;
				default:
					compile->parser_curnode = tmpNodeNum; //ʹ������Ϣָ����Ч��token
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
	�������汾���﷨�������������Ǳ�������������ĵĲ��֣�����һ���汾�������㷨���˵�����
	���ô�״̬�������ȼ���ջ�ķ�ʽ���ȵڶ����汾�Ŀɶ���ǿ�ܶ࣬����ά������չ��
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
			case ZL_TK_NEGATIVE:
				compile->exp_struct.state = ZL_ST_PARSER_INNEGATIVE;
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
			case ZL_TK_BIT_AND:
				compile->exp_struct.state = ZL_ST_PARSER_INBIT_AND;
				break;
			case ZL_TK_BIT_AND_ASSIGN:
				compile->exp_struct.state = ZL_ST_PARSER_INBIT_AND_ASSIGN;
				break;
			case ZL_TK_BIT_OR:
				compile->exp_struct.state = ZL_ST_PARSER_INBIT_OR;
				break;
			case ZL_TK_BIT_OR_ASSIGN:
				compile->exp_struct.state = ZL_ST_PARSER_INBIT_OR_ASSIGN;
				break;
			case ZL_TK_BIT_XOR:
				compile->exp_struct.state = ZL_ST_PARSER_INBIT_XOR;
				break;
			case ZL_TK_BIT_XOR_ASSIGN:
				compile->exp_struct.state = ZL_ST_PARSER_INBIT_XOR_ASSIGN;
				break;
			case ZL_TK_BIT_RIGHT:
				compile->exp_struct.state = ZL_ST_PARSER_INBIT_RIGHT;
				break;
			case ZL_TK_BIT_RIGHT_ASSIGN:
				compile->exp_struct.state = ZL_ST_PARSER_INBIT_RIGHT_ASSIGN;
				break;
			case ZL_TK_BIT_LEFT:
				compile->exp_struct.state = ZL_ST_PARSER_INBIT_LEFT;
				break;
			case ZL_TK_BIT_LEFT_ASSIGN:
				compile->exp_struct.state = ZL_ST_PARSER_INBIT_LEFT_ASSIGN;
				break;
			case ZL_TK_BIT_REVERSE:
				compile->exp_struct.state = ZL_ST_PARSER_INBIT_REVERSE;
				break;
			default: // �˴����û��defaultĬ�ϴ���Ļ����ͻ���ֽ������ʽʱ������ĳЩtoken����������޷���λ�﷨����ľ���λ�ú�ԭ�򣬻����ܵ��´�����﷨����
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_PARSER_EXPRESS_INVALID_TOKEN);
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
		case ZL_ST_PARSER_INBIT_LEFT_ASSIGN:
		case ZL_ST_PARSER_INBIT_RIGHT_ASSIGN:
		case ZL_ST_PARSER_INBIT_XOR_ASSIGN:
		case ZL_ST_PARSER_INBIT_OR_ASSIGN:
		case ZL_ST_PARSER_INBIT_AND_ASSIGN:
		case ZL_ST_PARSER_INPLUS_ASSIGN:
		case ZL_ST_PARSER_INMINIS_ASSIGN:
		case ZL_ST_PARSER_INTIMES_ASSIGN:
		case ZL_ST_PARSER_INDIVIDE_ASSIGN:
		case ZL_ST_PARSER_INMOD_ASSIGN:
		case ZL_ST_INASSIGN:
			compile->detectCurnodeSyntax(VM_ARG);
			compile->OpLevelForAssign(VM_ARG);
			break;
		case ZL_ST_PARSER_INBIT_LEFT:
		case ZL_ST_PARSER_INBIT_RIGHT:
		case ZL_ST_PARSER_INBIT_XOR:
		case ZL_ST_PARSER_INBIT_OR:
		case ZL_ST_PARSER_INBIT_AND:
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
		case ZL_ST_PARSER_INBIT_REVERSE:
		case ZL_ST_PARSER_INNEGATIVE:
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
	����parser_curnode��ǰ�﷨�����ڵ�Ľڵ�ţ��������������Ч�ķ�ֹparser_curnode�����
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

/*�﷨����ʱ���õĴ�ӡ���������Ϣ�ĺ���*/
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
	compile->printNode(VM_ARG,compile->parser_curnode);  //��ӡ����ڵ����Ϣ
	compile->exit(VM_ARG,ZL_ERR_CP_PARSER_ERROR_EXIT);
}

/**
	��ĳ�ڵ�ѹ��ջ������Ҫ�Ƚ����ȼ�ʱ���ٵ���ջ
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
	�����ѹ��ջ�Ľڵ㵯����
*/
ZENGL_PARSER_OP_LEVEL_STACK_TYPE zengl_opLevel_pop_stack(ZL_VOID * VM_ARG,ZL_INT index)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_PARSER_OP_LEVEL_STACK_TYPE retstack;
	if(compile->parser_opLevel_stackList.count<=0 ||
		compile->parser_opLevel_stackList.count + index <0)
	{
		retstack.tokcategory = ZL_TKCG_OP_START; //ZL_TKCG_OP_START��ZL_OP_LEVEL_START�Ƕ�Ӧö�����еĵ�һ��ֵ�����Ա�ʾ��͵����ȼ���
		retstack.tok_op_level = ZL_OP_LEVEL_START;
		retstack.nodenum = -1;
		return retstack;
	}
	retstack = compile->parser_opLevel_stackList.stacks[compile->parser_opLevel_stackList.count + index];
	return retstack;
}

/**
	��ʼ�����ȼ���ջ��̬����
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

/*���ݵ�ǰcurnode�ڵ��curnode+1�ڵ��������ж��Ƿ����﷨����*/
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
			nextNodeTkType != ZL_TK_BIT_REVERSE &&
			nextNodeTkType != ZL_TK_REVERSE && 
			nextNodeTkType != ZL_TK_ADDRESS && 
			nextNodeTkType != ZL_TK_NEGATIVE &&
			(nextNodeTKCG == ZL_TKCG_OP_COMMA ||
			 nextNodeTKCG == ZL_TKCG_OP_PLUS_MINIS ||
			 nextNodeTKCG == ZL_TKCG_OP_TIM_DIV ||
			 nextNodeTKCG == ZL_TKCG_OP_BITS ||
			 nextNodeTKCG == ZL_TKCG_OP_RELATION ||
			 nextNodeTKCG == ZL_TKCG_OP_LOGIC ||
			 nextNodeTKCG == ZL_TKCG_OP_PP_MM ||
			 nextNodeTKCG == ZL_TKCG_OP_QUESTION ||
			 nextNodeTkType == ZL_TK_RBRACKET ||
			 nextNodeTkType == ZL_TK_RMBRACKET ||
			 nextNodeTkType == ZL_TK_SEMI)) //����,������,�ַ�����������ǳ��˸�ֵ���㣬ȡ�������ã�����֮��Ĳ�����������������Ż����������Ż��߷ֺ�
			return;
		else if(nextNodeNum != -1)
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_NUM_FLOAT_STR_INVALID_NEXT_NODE);
		else
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_NEXT_NODE);
		break;
	case ZL_ST_INID:
		if((ZENGL_AST_ISTOKCATEXOP(nextNodeNum) && nextNodeTkType != ZL_TK_BIT_REVERSE && nextNodeTkType != ZL_TK_REVERSE && nextNodeTkType != ZL_TK_ADDRESS && nextNodeTkType != ZL_TK_NEGATIVE) || 
			(ZENGL_AST_ISTOK_VALIDX(nextNodeNum) && 
			(nextNodeTkType == ZL_TK_LBRACKET ||
			 nextNodeTkType == ZL_TK_LMBRACKET ||
			 nextNodeTkType == ZL_TK_RBRACKET ||
			 nextNodeTkType == ZL_TK_RMBRACKET ||
			 nextNodeTkType == ZL_TK_SEMI))) //������ʶ����������ǲ��������(����ȡ�������ú͸��������)���������Ż����������Ż������Ż��������Ż�ֺ�
			return;
		else if(nextNodeNum != -1)
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ID_INVALID_NEXT_NODE);
		else
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_NEXT_NODE);
		break;
	case ZL_ST_PARSER_INBIT_REVERSE:
	case ZL_ST_PARSER_INBIT_LEFT:
	case ZL_ST_PARSER_INBIT_RIGHT:
	case ZL_ST_PARSER_INBIT_XOR:
	case ZL_ST_PARSER_INBIT_OR:
	case ZL_ST_PARSER_INBIT_AND:
	case ZL_ST_PARSER_INEQUAL:
	case ZL_ST_PARSER_INGREAT_EQ:
	case ZL_ST_INGREAT:
	case ZL_ST_PARSER_INLESS_EQ:
	case ZL_ST_INLESS:
	case ZL_ST_INNOT_EQ:
	case ZL_ST_PARSER_INREVERSE:
	case ZL_ST_INAND:
	case ZL_ST_INOR:
	case ZL_ST_PARSER_INBIT_LEFT_ASSIGN:
	case ZL_ST_PARSER_INBIT_RIGHT_ASSIGN:
	case ZL_ST_PARSER_INBIT_XOR_ASSIGN:
	case ZL_ST_PARSER_INBIT_OR_ASSIGN:
	case ZL_ST_PARSER_INBIT_AND_ASSIGN:
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
	case ZL_ST_PARSER_INNEGATIVE:
		if(ZENGL_AST_ISTOK_VALIDX(nextNodeNum) && 
			(nextNodeTKCG == ZL_TKCG_OP_FACTOR ||
			 nextNodeTKCG == ZL_TKCG_OP_PP_MM ||
			 nextNodeTkType == ZL_TK_BIT_REVERSE ||
			 nextNodeTkType == ZL_TK_REVERSE ||
			 nextNodeTkType == ZL_TK_NEGATIVE ||
			 nextNodeTkType == ZL_TK_LBRACKET)) //���ʽ�����������������Ǳ��������֣��ַ����Ȳ������ӻ��߼ӼӼ�������ȡ��������򸺺Ż�������
			 return;
		else if(ZENGL_AST_ISTOK_VALIDX(nextNodeNum) && 
				compile->exp_struct.state == ZL_ST_INASSIGN &&
				nextNodeTkType == ZL_TK_ADDRESS) //����Ǹ�ֵ��䣬�������ں�������������
			return;
		else if(nextNodeNum != -1)
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_EXPRESS_OP_INVALID_NEXT_NODE);
		else
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_NEXT_NODE);
		break;
	case ZL_ST_PARSER_INADDRESS:
		if(ZENGL_AST_ISTOK_VALIDX(nextNodeNum) && 
			(nextNodeTkType == ZL_TK_ID)) //�����������������Ǳ���֮���ʶ��
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
			 nextNodeTkType == ZL_TK_BIT_REVERSE ||
			 nextNodeTkType == ZL_TK_REVERSE ||
			 nextNodeTkType == ZL_TK_NEGATIVE ||
			 nextNodeTkType == ZL_TK_LBRACKET)) //���ӼӼ��������ʱ������ֻ���ǼӼӼ������������ӣ�ȡ�������Ż�������
			 return;
		else if(ZENGL_AST_ISTOK_VALIDX(nextNodeNum) && 
				nodes[compile->parser_curnode].leftOrRight == ZL_OP_POS_IN_RIGHT && 
				nextNodeTkType != ZL_TK_BIT_REVERSE &&
				nextNodeTkType != ZL_TK_REVERSE && 
				nextNodeTkType != ZL_TK_ADDRESS && 
				nextNodeTkType != ZL_TK_NEGATIVE && 
				(nextNodeTKCG == ZL_TKCG_OP_COMMA ||
				 nextNodeTKCG == ZL_TKCG_OP_PLUS_MINIS ||
				 nextNodeTKCG == ZL_TKCG_OP_TIM_DIV ||
				 nextNodeTKCG == ZL_TKCG_OP_BITS ||
				 nextNodeTKCG == ZL_TKCG_OP_RELATION ||
				 nextNodeTKCG == ZL_TKCG_OP_LOGIC ||
				 nextNodeTKCG == ZL_TKCG_OP_PP_MM ||
				 nextNodeTKCG == ZL_TKCG_OP_QUESTION ||
				 nextNodeTkType == ZL_TK_RBRACKET ||
				 nextNodeTkType == ZL_TK_RMBRACKET ||
				 nextNodeTkType == ZL_TK_SEMI)) //���Ҳ�ʱ��ֻ���ǳ��˸�ֵ���㣬ȡ�������ã����������֮��Ĳ�����������������Ż����������Ż�ֺ�
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
			 nextNodeTkType == ZL_TK_BIT_REVERSE ||
			 nextNodeTkType == ZL_TK_REVERSE ||
			 nextNodeTkType == ZL_TK_NEGATIVE ||
			 nextNodeTkType == ZL_TK_LBRACKET ||
			 nextNodeTkType == ZL_TK_RBRACKET ||
			 nextNodeTkType == ZL_TK_ADDRESS)) //�����ź�����﷨����
			return;
		else if(nextNodeNum != -1)
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_LBRACKET_INVALID_NEXT_NODE);
		else
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_NEXT_NODE);
		break;
	case ZL_ST_PARSER_INLMBRACKET:
		if(compile->parser_opLevel_stackList.count <= 0) //�������Ų����Է��ڿ�ͷ
		{
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_START_INVALID_TOKEN,
									compile->getTokenStr(VM_ARG,nodes,compile->parser_curnode),
									compile->getTokenStr(VM_ARG,nodes,compile->parser_curnode));
		}
		else if(ZENGL_AST_ISTOK_VALIDX(nextNodeNum) && 
			(nextNodeTKCG == ZL_TKCG_OP_FACTOR ||
			 nextNodeTKCG == ZL_TKCG_OP_PP_MM ||
			 nextNodeTkType == ZL_TK_BIT_REVERSE ||
			 nextNodeTkType == ZL_TK_REVERSE ||
			 nextNodeTkType == ZL_TK_NEGATIVE ||
			 nextNodeTkType == ZL_TK_LBRACKET ||
			 nextNodeTkType == ZL_TK_RMBRACKET)) //�������ź�����﷨����
		    return;
		else if(nextNodeNum != -1)
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_LMBRACKET_INVALID_NEXT_NODE);
		else
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_NEXT_NODE);
		break;
	case ZL_ST_PARSER_INRBRACKET:
		if(ZENGL_AST_ISTOK_VALIDX(nextNodeNum) && 
			nextNodeTkType != ZL_TK_BIT_REVERSE &&
			nextNodeTkType != ZL_TK_REVERSE && 
			nextNodeTkType != ZL_TK_ADDRESS && 
			nextNodeTkType != ZL_TK_NEGATIVE &&
			(nextNodeTKCG == ZL_TKCG_OP_COMMA ||
			 nextNodeTKCG == ZL_TKCG_OP_PLUS_MINIS ||
			 nextNodeTKCG == ZL_TKCG_OP_TIM_DIV ||
			 nextNodeTKCG == ZL_TKCG_OP_BITS ||
			 nextNodeTKCG == ZL_TKCG_OP_RELATION ||
			 nextNodeTKCG == ZL_TKCG_OP_LOGIC ||
			 nextNodeTKCG == ZL_TKCG_OP_PP_MM ||
			 nextNodeTKCG == ZL_TKCG_OP_QUESTION ||
			 nextNodeTkType == ZL_TK_RBRACKET ||
			 nextNodeTkType == ZL_TK_RMBRACKET ||
			 nextNodeTkType == ZL_TK_SEMI)) //�����ź�������ǳ��˸�ֵ���㣬ȡ�������ţ����������֮��Ĳ�����������������Ż����������Ż�ֺ�
			return;
		else if(nextNodeNum != -1)
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_RBRACKET_INVALID_NEXT_NODE);
		else
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_NEXT_NODE);
		break;
	case ZL_ST_PARSER_INRMBRACKET:
		if(ZENGL_AST_ISTOK_VALIDX(nextNodeNum) && 
			nextNodeTkType != ZL_TK_BIT_REVERSE && 
			nextNodeTkType != ZL_TK_REVERSE && 
			nextNodeTkType != ZL_TK_ADDRESS && 
			nextNodeTkType != ZL_TK_NEGATIVE &&
			(nextNodeTKCG == ZL_TKCG_OP_COMMA ||
			 nextNodeTKCG == ZL_TKCG_OP_PLUS_MINIS ||
			 nextNodeTKCG == ZL_TKCG_OP_TIM_DIV ||
			 nextNodeTKCG == ZL_TKCG_OP_BITS ||
			 nextNodeTKCG == ZL_TKCG_OP_RELATION ||
			 nextNodeTKCG == ZL_TKCG_OP_LOGIC ||
			 nextNodeTKCG == ZL_TKCG_OP_PP_MM ||
			 nextNodeTKCG == ZL_TKCG_OP_QUESTION ||
			 nextNodeTKCG == ZL_TKCG_OP_ASSIGN ||
			 nextNodeTKCG == ZL_TKCG_OP_DOT ||
			 nextNodeTkType == ZL_TK_RBRACKET ||
			 nextNodeTkType == ZL_TK_RMBRACKET ||
			 nextNodeTkType == ZL_TK_SEMI)) //�������ź�������ǳ���ȡ�������ţ����������֮��Ĳ�����������������Ż����������Ż�ֺ�
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
			 nextNodeTkType == ZL_TK_BIT_REVERSE ||
			 nextNodeTkType == ZL_TK_REVERSE ||
			 nextNodeTkType == ZL_TK_NEGATIVE ||
			 nextNodeTkType == ZL_TK_LBRACKET ||
			 nextNodeTkType == ZL_TK_ADDRESS)) //����,�ʺ�,ð�ź�����﷨����
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
	ʹ�����ȼ���ջ����Ӽ��˳���˫Ŀ�����
*/
ZL_VOID zengl_OpLevelForTwo(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_PARSER_OP_LEVEL_STACK_TYPE last_stack;
	ZENGL_PARSER_OP_LEVEL_STACK_TYPE before_stack;
	if(compile->parser_opLevel_stackList.count <= 0 && compile->exp_struct.state != ZL_ST_INMINIS) //ֻ�м��ſ����Ը��ŵ���ʽ���ڱ��ʽ�Ŀ�ͷ
	{
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_START_INVALID_TOKEN,
								compile->getTokenStr(VM_ARG,nodes,compile->parser_curnode),
								compile->getTokenStr(VM_ARG,nodes,compile->parser_curnode));
	}
	while(ZL_TRUE) //ѭ�����ж�ջ���ȼ��ıȽ�
	{
		last_stack = compile->opLevel_pop_stack(VM_ARG,-1);
		before_stack = compile->opLevel_pop_stack(VM_ARG,-2);
		if(before_stack.tok_op_level >= nodes[compile->parser_curnode].tok_op_level) //ͨ�� ">=" ʵ�ִ����ҵĽ�Ϲ�����Ϊ�����ȼ���ȵ�����£�����Ȼ�ȡ�ӽڵ�
		{
			compile->ASTAddNodeChild(VM_ARG,before_stack.nodenum,last_stack.nodenum);
			compile->parser_opLevel_stackList.count > 0 ? compile->parser_opLevel_stackList.count-- : ZL_TRUE ;
		}
		else
		{
			compile->ASTAddNodeChild(VM_ARG,compile->parser_curnode,last_stack.nodenum);
			compile->parser_opLevel_stackList.count > 0 ? compile->parser_opLevel_stackList.count-- : ZL_TRUE ;
			compile->opLevel_push_stack(VM_ARG,compile->parser_curnode);
			compile->exp_struct.state = ZL_ST_START; //��ǰ״̬�����ȼ�������ϣ�׼����ȡ��һ��״̬��
			break;
		}
	}
}

/*
	ʹ�����ȼ���ջ����ֵ����˫Ŀ�����
*/
ZL_VOID zengl_OpLevelForAssign(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_PARSER_OP_LEVEL_STACK_TYPE last_stack;
	ZENGL_PARSER_OP_LEVEL_STACK_TYPE before_stack;
	if(compile->parser_opLevel_stackList.count <= 0) //��ֵ��䲻���Է��ڱ��ʽ��ͷ
	{
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_START_INVALID_TOKEN,
								compile->getTokenStr(VM_ARG,nodes,compile->parser_curnode),
								compile->getTokenStr(VM_ARG,nodes,compile->parser_curnode));
	}
	while(ZL_TRUE) //ѭ�����ж�ջ���ȼ��ıȽ�
	{
		last_stack = compile->opLevel_pop_stack(VM_ARG,-1);
		before_stack = compile->opLevel_pop_stack(VM_ARG,-2);
		if(before_stack.tok_op_level > ZL_OP_LEVEL_ASSIGN_WHEN_IN_RIGHT) //Ŀǰֻ�е���������Ҳำֵ�����ȼ��ߣ�ͨ�� ">" ʵ�ִ��ҵ���Ľ�ϣ���Ϊ�����ȼ���ȵ�����£��Ҳ��Ȼ�ȡ�ӽڵ�
		{
			compile->ASTAddNodeChild(VM_ARG,before_stack.nodenum,last_stack.nodenum);
			compile->parser_opLevel_stackList.count > 0 ? compile->parser_opLevel_stackList.count-- : ZL_TRUE ;
		}
		else
		{
			compile->ASTAddNodeChild(VM_ARG,compile->parser_curnode,last_stack.nodenum);
			compile->parser_opLevel_stackList.count > 0 ? compile->parser_opLevel_stackList.count-- : ZL_TRUE ;
			compile->opLevel_push_stack(VM_ARG,compile->parser_curnode);
			compile->exp_struct.state = ZL_ST_START; //��ǰ״̬�����ȼ�������ϣ�׼����ȡ��һ��״̬��
			break;
		}
	}
}

/*
	ʹ�����ȼ���ջ����ֺŽ�����
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
	ʹ�����ȼ���ջ����ӼӼ��������
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
		while(ZL_TRUE) //ѭ�����ж�ջ���ȼ��ıȽ�
		{
			last_stack = compile->opLevel_pop_stack(VM_ARG,-1);
			before_stack = compile->opLevel_pop_stack(VM_ARG,-2);
			if(before_stack.tok_op_level >= nodes[compile->parser_curnode].tok_op_level) //ͨ�� ">=" ʵ�ִ����ҵĽ�Ϲ�����Ϊ�����ȼ���ȵ�����£�����Ȼ�ȡ�ӽڵ�
			{
				compile->ASTAddNodeChild(VM_ARG,before_stack.nodenum,last_stack.nodenum);
				compile->parser_opLevel_stackList.count > 0 ? compile->parser_opLevel_stackList.count-- : ZL_TRUE ;
			}
			else
			{
				compile->ASTAddNodeChild(VM_ARG,compile->parser_curnode,last_stack.nodenum);
				compile->parser_opLevel_stackList.count > 0 ? compile->parser_opLevel_stackList.count-- : ZL_TRUE ;
				compile->opLevel_push_stack(VM_ARG,compile->parser_curnode);
				compile->exp_struct.state = ZL_ST_START; //��ǰ״̬�����ȼ�������ϣ�׼����ȡ��һ��״̬��
				break;
			}
		}
	}
}

/*
	��ȡ�ӼӼ�������������Ҳ��λ�ñ�ʶ��Ϊ�˽������ȼ��ıȽϣ�����֪������໹���Ҳ࣬ͬʱ�����Ҳ�ĺ���Ҳ��һ��
	�������ȼӼӼ�����ȡֵ���Ҳ������ȡֵ��ӼӼ���
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
	ʹ�����ȼ���ջ����������
*/
ZL_VOID zengl_OpLevelForRBRACKET(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_PARSER_OP_LEVEL_STACK_TYPE last_stack;
	ZENGL_PARSER_OP_LEVEL_STACK_TYPE before_stack;
	ZENGL_PARSER_OP_LEVEL_STACK_TYPE before_before_stack;
	if(compile->parser_opLevel_stackList.count <= 0) //�����Ų����Է��ڱ��ʽ�Ŀ�ͷ
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
				if(before_stack.nodenum >= 0 && nodes[before_stack.nodenum].toktype == ZL_TK_ID) //���磺test();�����Ŀղ�����������
				{
					compile->ASTAddNodeChild(VM_ARG,before_stack.nodenum,-1);
					nodes[before_stack.nodenum].toktype = ZL_TK_FUNCALL;
					/*if(ZENGL_AST_ISTOKTYPEX(before_stack.nodenum-1,ZL_TK_DOT) &&
					   (ZENGL_AST_ISTOKTYPEX(before_stack.nodenum-2,ZL_TK_RMBRACKET) ||
					    ZENGL_AST_ISTOKTYPEX(before_stack.nodenum-3,ZL_TK_DOT))) //�ж��Ƿ�����Ч���ຯ��
					{
						compile->parser_curnode = before_stack.nodenum;
						compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_CLASS_FUNCALL);	
					}*/
					compile->parser_opLevel_stackList.count > 0 ? compile->parser_opLevel_stackList.count-- : ZL_TRUE ;
					compile->exp_struct.state = ZL_ST_START; //��ǰ״̬�����ȼ�������ϣ�׼����ȡ��һ��״̬��
					break;
				}
				else
					compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_BRACKET_EMPTY); //��ͨ�����ű��ʽ����Ϊ�գ������Ǻ����Ĳ������ţ���Ϊ��������û�в���
			}
			else if(nodes[before_stack.nodenum].toktype == ZL_TK_LBRACKET)
			{
				if(before_before_stack.nodenum >=0 && nodes[before_before_stack.nodenum].toktype == ZL_TK_ID) //����max(a,b);�����ĺ���������ʽ
				{
					compile->ASTAddNodeChild(VM_ARG,before_before_stack.nodenum,last_stack.nodenum);
					compile->parser_opLevel_stackList.count > 0 ? compile->parser_opLevel_stackList.count-- : ZL_TRUE ;
					compile->parser_opLevel_stackList.count > 0 ? compile->parser_opLevel_stackList.count-- : ZL_TRUE ;
					nodes[before_before_stack.nodenum].toktype = ZL_TK_FUNCALL;
					/*if(ZENGL_AST_ISTOKTYPEX(before_before_stack.nodenum-1,ZL_TK_DOT) &&
					   (ZENGL_AST_ISTOKTYPEX(before_before_stack.nodenum-2,ZL_TK_RMBRACKET) ||
					    ZENGL_AST_ISTOKTYPEX(before_before_stack.nodenum-3,ZL_TK_DOT))) //�ж��Ƿ�����Ч���ຯ��
					{
						compile->parser_curnode = before_before_stack.nodenum;
						compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_CLASS_FUNCALL);	
					}*/
				}
				else //���������ͨ�����ű��ʽ�����Ǻ��������� 2 + (3 - 4);��������ű��ʽ
				{
					compile->parser_opLevel_stackList.count > 0 ? compile->parser_opLevel_stackList.count-- : ZL_TRUE ;
					compile->parser_opLevel_stackList.count > 0 ? compile->parser_opLevel_stackList.count-- : ZL_TRUE ;
					compile->opLevel_push_stack(VM_ARG,last_stack.nodenum);
				}
				compile->exp_struct.state = ZL_ST_START; //��ǰ״̬�����ȼ�������ϣ�׼����ȡ��һ��״̬��
				break;
			}
			else //���û���������ţ��򹹽������ڲ��ı��ʽ�ṹ��
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
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_RBRACKET_NO_LBRACKET); //û�ж�Ӧ����������ƥ��
		}
		else
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_RBRACKET_NO_LBRACKET);
	}
}

/*
	ʹ�����ȼ���ջ������������
*/
ZL_VOID zengl_OpLevelForRMBRACKET(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_PARSER_OP_LEVEL_STACK_TYPE last_stack;
	ZENGL_PARSER_OP_LEVEL_STACK_TYPE before_stack;
	ZENGL_PARSER_OP_LEVEL_STACK_TYPE before_before_stack;
	if(compile->parser_opLevel_stackList.count <= 0) //�������Ų����Է��ڱ��ʽ�Ŀ�ͷ
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
				if(before_stack.nodenum >= 0 && nodes[before_stack.nodenum].toktype == ZL_TK_ID) //���磺test[] = 2;����������Ԫ����ʽ
				{
					compile->ASTAddNodeChild(VM_ARG,before_stack.nodenum,-1);
					nodes[before_stack.nodenum].toktype = ZL_TK_ARRAY_ITEM;
					compile->parser_opLevel_stackList.count > 0 ? compile->parser_opLevel_stackList.count-- : ZL_TRUE ;
					compile->exp_struct.state = ZL_ST_START; //��ǰ״̬�����ȼ�������ϣ�׼����ȡ��һ��״̬��
					break;
				}
				else
					compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_ARRAY_ITEM); //��Ч������Ԫ�ظ�ʽ
			}
			else if(nodes[before_stack.nodenum].toktype == ZL_TK_LMBRACKET)
			{
				if(before_before_stack.nodenum >=0 && nodes[before_before_stack.nodenum].toktype == ZL_TK_ID) //����test[a,b];����������Ԫ����ʽ
				{
					compile->ASTAddNodeChild(VM_ARG,before_before_stack.nodenum,last_stack.nodenum);
					compile->parser_opLevel_stackList.count > 0 ? compile->parser_opLevel_stackList.count-- : ZL_TRUE ;
					compile->parser_opLevel_stackList.count > 0 ? compile->parser_opLevel_stackList.count-- : ZL_TRUE ;
					nodes[before_before_stack.nodenum].toktype = ZL_TK_ARRAY_ITEM;
					compile->exp_struct.state = ZL_ST_START; //��ǰ״̬�����ȼ�������ϣ�׼����ȡ��һ��״̬��
					break;
				}
				else //[a,b]������û�б�����ʶ������Ч�����ʽ
					compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_ARRAY_ITEM); //��Ч������Ԫ�ظ�ʽ
			}
			else //���û�����������ţ��򹹽��������ڲ��ı��ʽ�ṹ��
			{
				compile->ASTAddNodeChild(VM_ARG,before_stack.nodenum,last_stack.nodenum);
				compile->parser_opLevel_stackList.count > 0 ? compile->parser_opLevel_stackList.count-- : ZL_TRUE ;
			}
		}
		else if(compile->parser_opLevel_stackList.count == 1)
		{
			if(nodes[last_stack.nodenum].toktype == ZL_TK_LMBRACKET)
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_ARRAY_ITEM); //��Ч������Ԫ�ظ�ʽ
			else
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_RMBRACKET_NO_LMBRACKET); //û�ж�Ӧ������������ƥ��
		}
		else 
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_RMBRACKET_NO_LMBRACKET); //û�ж�Ӧ������������ƥ��
	}
}

/*
	ʹ�����ȼ���ջ�����ʺ������
*/
ZL_VOID zengl_OpLevelForQuestion(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_PARSER_OP_LEVEL_STACK_TYPE last_stack;
	ZENGL_PARSER_OP_LEVEL_STACK_TYPE before_stack;
	if(compile->parser_opLevel_stackList.count <= 0) //�ʺŲ����Է��ڱ��ʽ�Ŀ�ͷ
	{
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_START_INVALID_TOKEN,
								compile->getTokenStr(VM_ARG,nodes,compile->parser_curnode),
								compile->getTokenStr(VM_ARG,nodes,compile->parser_curnode));
	}
	compile->CheckQstColonValid(VM_ARG);
	while(ZL_TRUE) //ѭ�����ж�ջ���ȼ��ıȽ�
	{
		last_stack = compile->opLevel_pop_stack(VM_ARG,-1);
		before_stack = compile->opLevel_pop_stack(VM_ARG,-2);
		if(before_stack.tok_op_level >= nodes[compile->parser_curnode].tok_op_level) //ͨ�� ">=" ʵ�ִ����ҵĽ�Ϲ�����Ϊ�����ȼ���ȵ�����£�����Ȼ�ȡ�ӽڵ�
		{
			compile->ASTAddNodeChild(VM_ARG,before_stack.nodenum,last_stack.nodenum);
			compile->parser_opLevel_stackList.count > 0 ? compile->parser_opLevel_stackList.count-- : ZL_TRUE ;
		}
		else
		{
			compile->ASTAddNodeChild(VM_ARG,compile->parser_curnode,last_stack.nodenum);
			compile->parser_opLevel_stackList.count > 0 ? compile->parser_opLevel_stackList.count-- : ZL_TRUE ;
			compile->opLevel_push_stack(VM_ARG,compile->parser_curnode);
			compile->exp_struct.state = ZL_ST_START; //��ǰ״̬�����ȼ�������ϣ�׼����ȡ��һ��״̬��
			break;
		}
	}
}

/*
	ʹ�����ȼ���ջ����ð�������
*/
ZL_VOID zengl_OpLevelForColon(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_PARSER_OP_LEVEL_STACK_TYPE last_stack;
	ZENGL_PARSER_OP_LEVEL_STACK_TYPE before_stack;
	ZL_BOOL isinMiddle = ZL_TRUE; //�жϸ�ֵ����Ƿ���?...:֮��

	if(compile->parser_opLevel_stackList.count <= 0) //ð�Ų����Է��ڱ��ʽ�Ŀ�ͷ
	{
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_START_INVALID_TOKEN,
								compile->getTokenStr(VM_ARG,nodes,compile->parser_curnode),
								compile->getTokenStr(VM_ARG,nodes,compile->parser_curnode));
	}
	else if(nodes[compile->parser_curnode].isvalid_qst_colon != ZL_TRUE)
	{
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_COLON_NO_QUESTION_LEFT);
	}
	while(ZL_TRUE) //ѭ�����ж�ջ���ȼ��ıȽ�
	{
		last_stack = compile->opLevel_pop_stack(VM_ARG,-1);
		before_stack = compile->opLevel_pop_stack(VM_ARG,-2);
		if(before_stack.tok_op_level >= nodes[compile->parser_curnode].tok_op_level || 
			(isinMiddle == ZL_TRUE && before_stack.tokcategory == ZL_TKCG_OP_ASSIGN)) //ͨ�� ">=" ʵ�ִ����ҵĽ�Ϲ�����Ϊ�����ȼ���ȵ�����£�����Ȼ�ȡ�ӽڵ㣬���⸳ֵ�����..?...:�м�ʱ���ȼ����Ҳ��ð�Ÿߣ������Ͳ���ҪΪ..?..:�м�ĸ�ֵ�����������ˡ�
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
			compile->exp_struct.state = ZL_ST_START; //��ǰ״̬�����ȼ�������ϣ�׼����ȡ��һ��״̬��
			break;
		}
	}
}

/*
	����ʺ�ð���Ƿ�һһƥ��
*/
ZL_VOID zengl_CheckQstColonValid(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZL_INT tmpNodeNum = compile->parser_curnode;
	ZL_INT bracket_count = 0; //��������ƥ�������
	ZL_INT mbracket_count = 0; //����������ƥ�������

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

/*��child��Ӧ�Ľڵ���뵽parent�ڵ���ӽڵ��С�*/
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
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_LMBRACKET_NO_RMBRACKET); //��Ч������Ԫ�ظ�ʽ �˴���û�ж�Ӧ����������
	}

	if(childs->count < ZL_AST_CHILD_NODE_SIZE)
	{
		if(childs->childnum[childs->count] == -1)
			childs->childnum[childs->count++] = child;
		if(child >= 0 && child <= compile->AST_nodes.count-1) //���û�������жϣ���ô��childΪ-1ʱ��Խ���ˣ����BUG���Ѳ�ģ�����Ϊ��������Խ���ʱ������㣬������������ϡ��Źֵĵط���ʾ��Ҫ�����ڴ����ݶϵ㣬�������Բ����ҳ�����
			nodes[child].parentnode = parent; //����child�ڵ��parentnode�ֶ�Ϊparent.
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

/*�����жϵ�ǰ�ļ����Ƿ��Ǹ���*/
ZL_BOOL zengl_CheckIsNegative(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	if(nodes[compile->AST_nodes.count].toktype != ZL_TK_MINIS)
		return ZL_FALSE;
	else if(compile->AST_nodes.count == 0)
		return ZL_TRUE;
	switch(nodes[compile->AST_nodes.count - 1].toktype)
	{
	case ZL_TK_SEMI:
	case ZL_TK_LBRACKET:
	case ZL_TK_LMBRACKET:
		return ZL_TRUE;
		break;
	case ZL_TK_RESERVE:
		switch(nodes[compile->AST_nodes.count - 1].reserve)
		{
		case ZL_RSV_RETURN:
		case ZL_RSV_PRINT:
			return ZL_TRUE;
			break;
		}
		break;
	default:
		switch(nodes[compile->AST_nodes.count - 1].tokcategory)
		{
		case ZL_TKCG_OP_PLUS_MINIS:
		case ZL_TKCG_OP_ASSIGN:
		case ZL_TKCG_OP_TIM_DIV:
		case ZL_TKCG_OP_RELATION:
		case ZL_TKCG_OP_LOGIC:
		case ZL_TKCG_OP_COMMA:
		case ZL_TKCG_OP_QUESTION:
		case ZL_TKCG_OP_PP_MM:
			return ZL_TRUE;
			break;
		}
		break;
	}
	return ZL_FALSE;
}

/*�����жϵ�ǰ��"&"�����Ƿ��ǰ�λ�������*/
ZL_BOOL zengl_CheckIsBitAnd(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	if(nodes[compile->AST_nodes.count].toktype != ZL_TK_ADDRESS)
		return ZL_FALSE;
	else if(compile->AST_nodes.count == 0)
		return ZL_FALSE;
	switch(nodes[compile->AST_nodes.count - 1].toktype)
	{
	case ZL_TK_RBRACKET:
	case ZL_TK_RMBRACKET:
	case ZL_TK_PLUS_PLUS:
	case ZL_TK_MINIS_MINIS:
		return ZL_TRUE;
		break;
	default:
		switch(nodes[compile->AST_nodes.count - 1].tokcategory)
		{
		case ZL_TKCG_OP_FACTOR:
			return ZL_TRUE;
			break;
		}
		break;
	}
	return ZL_FALSE;
}
