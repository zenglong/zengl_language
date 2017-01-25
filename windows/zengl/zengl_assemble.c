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
	该文件为汇编输出的处理文件。
*/

#include "zengl_global.h"

static ZL_VOID zengl_AsmGCLoopStackInit(ZL_VOID * VM_ARG); // 汇编模拟堆栈的初始化

// zengl_AsmGenCode_Fun函数用于生成fun...endfun代码块对应的汇编指令
static ZENGL_STATES zengl_AsmGenCode_Fun(ZL_VOID * VM_ARG, ZENGL_STATES state, ZENGL_ASM_LOOP_STACK_TYPE ** loopStackTopArg);

// zengl_AsmGenCode_Comma函数用于逗号运算符的汇编指令
static ZENGL_STATES zengl_AsmGenCode_Comma(ZL_VOID * VM_ARG, ZENGL_STATES state, ZENGL_ASM_LOOP_STACK_TYPE ** loopStackTopArg);

// 生成与操作因子相关的汇编指令
static ZENGL_STATES zengl_AsmGenCode_Factor(ZL_VOID * VM_ARG, ZENGL_STATES state, ZENGL_ASM_LOOP_STACK_TYPE ** loopStackTopArg);

// 生成赋值运算符的汇编指令
static ZENGL_STATES zengl_AsmGenCode_Assign(ZL_VOID * VM_ARG, ZENGL_STATES state, ZENGL_ASM_LOOP_STACK_TYPE ** loopStackTopArg);

// 生成大多数双目运算符，如：按位运算符，逻辑运算符，比较运算符，加减乘除取余等运算符的汇编指令
static ZENGL_STATES zengl_AsmGenCode_ForTwo(ZL_VOID * VM_ARG, ZENGL_STATES state, ZENGL_ASM_LOOP_STACK_TYPE ** loopStackTopArg);

// 生成负号运算符的汇编指令
static ZENGL_STATES zengl_AsmGenCode_Negative(ZL_VOID * VM_ARG, ZENGL_STATES state, ZENGL_ASM_LOOP_STACK_TYPE ** loopStackTopArg);

// 生成 按位取反运算符 或者 取反运算符 的汇编指令
static ZENGL_STATES zengl_AsmGenCode_Reverse(ZL_VOID * VM_ARG, ZENGL_STATES state, ZENGL_ASM_LOOP_STACK_TYPE ** loopStackTopArg);

// 生成 引用运算符的处理 的汇编指令
static ZENGL_STATES zengl_AsmGenCode_Address(ZL_VOID * VM_ARG, ZENGL_STATES state, ZENGL_ASM_LOOP_STACK_TYPE ** loopStackTopArg);

//zengl_AsmGCElif函数用于生成elif代码块对应的汇编指令
static ZENGL_STATES zengl_AsmGCElif(ZL_VOID * VM_ARG,ZENGL_AST_CHILD_NODE_TYPE * ifchnum,ZL_INT num, ZENGL_ASM_LOOP_STACK_TYPE ** loopStackTopArg);

// 生成 if-elif-else 控制语句的汇编指令
static ZENGL_STATES zengl_AsmGenCode_If_Elif_Else(ZL_VOID * VM_ARG, ZENGL_STATES state, ZENGL_ASM_LOOP_STACK_TYPE ** loopStackTopArg);

// 生成 ++和--运算符 的汇编指令
static ZENGL_STATES zengl_AsmGenCode_PP_MM(ZL_VOID * VM_ARG, ZENGL_STATES state, ZENGL_ASM_LOOP_STACK_TYPE ** loopStackTopArg);

// 生成 打印语句 的汇编指令
static ZENGL_STATES zengl_AsmGenCode_Print(ZL_VOID * VM_ARG, ZENGL_STATES state, ZENGL_ASM_LOOP_STACK_TYPE ** loopStackTopArg);

// 生成 for...endfor 循环控制语句的汇编指令
static ZENGL_STATES zengl_AsmGenCode_For(ZL_VOID * VM_ARG, ZENGL_STATES state, ZENGL_ASM_LOOP_STACK_TYPE ** loopStackTopArg);

/* 生成 函数调用 相关的汇编指令 */
static ZENGL_STATES zengl_AsmGenCode_Funcall(ZL_VOID * VM_ARG, ZENGL_STATES state, ZENGL_ASM_LOOP_STACK_TYPE ** loopStackTopArg);

// 生成return语句的汇编指令
static ZENGL_STATES zengl_AsmGenCode_Return(ZL_VOID * VM_ARG, ZENGL_STATES state, ZENGL_ASM_LOOP_STACK_TYPE ** loopStackTopArg);

// 生成数组元素的汇编指令
static ZENGL_STATES zengl_AsmGenCode_ArrayItem(ZL_VOID * VM_ARG, ZENGL_STATES state, ZENGL_ASM_LOOP_STACK_TYPE ** loopStackTopArg);

/* 生成class语句的汇编指令 */
static ZENGL_STATES zengl_AsmGenCode_Class(ZL_VOID * VM_ARG, ZENGL_STATES state, ZENGL_ASM_LOOP_STACK_TYPE ** loopStackTopArg);

/* 生成通过点运算符访问类成员的汇编指令 */
static ZENGL_STATES zengl_AsmGenCode_Dot(ZL_VOID * VM_ARG, ZENGL_STATES state, ZENGL_ASM_LOOP_STACK_TYPE ** loopStackTopArg);

/*
 *	break语句的汇编代码生成，break如果在for里则跳出for，如果在switch里则跳出switch
 *	如果在while结构里则跳出while，如果在do...dowhile里则跳出do...dowhile
 */
static ZL_VOID zengl_AsmGCBreak_Codes(ZL_VOID * VM_ARG,ZL_INT nodenum);

/*
 *	continue语句的汇编代码生成，判断是在for还是在while还是在do...dowhile里，然后
 *	根据判断跳到对应结构的开头
 */
static ZL_VOID zengl_AsmGCContinue_Codes(ZL_VOID * VM_ARG,ZL_INT nodenum);

/*
	组建汇编代码的主程式
*/
ZL_VOID zengl_buildAsmCode(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZL_INT tmpNodeNum = compile->AST_nodes.rootnode;
	ZL_INT count = 0;
	compile->gencode_struct.pc = 0;
	if(!(compile->AST_nodes.isInit == ZL_FALSE || compile->AST_nodes.count <= 0)) //如果语法树没初始化或为空，则表示里面没有任何token (可能是一个空的字符串脚本或空的脚本文件) 则只输出END指令
	{
		while(tmpNodeNum !=0 || count == 0)
		{
			compile->AsmGenCodes(VM_ARG,tmpNodeNum);
			tmpNodeNum = nodes[tmpNodeNum].nextnode;
			count++;
		}
	}
	run->AddInst(VM_ARG,compile->gencode_struct.pc++,compile->AST_nodes.count - 1,
				ZL_R_IT_END,ZL_R_DT_NONE,0,
				ZL_R_DT_NONE,0); //对应汇编指令 "END"
}

/*
	该函数根据AST抽象语法树的节点索引将某节点转为汇编代码。
	参数nodenum为节点在AST语法树动态数组里的节点索引。
*/
ZL_VOID zengl_AsmGenCodes(ZL_VOID * VM_ARG,ZL_INT nodenum)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_RUN_INST_OP_DATA inst_op_data;
	ZL_INT * chnum = ZL_NULL,* extnum = ZL_NULL,i;
	ZL_INT tmpch;
	//ZL_INT tmptype;
	ZENGL_STATES state;
	//compile->gencode_struct.state = ZL_ST_START;
	ZENGL_ASM_LOOP_STACK_TYPE * loopStackTop = ZL_NULL;
	zengl_AsmGCLoopStackPush(VM_ARG, nodenum, ZL_ST_START);
	state = ZL_ST_START;
	while(state != ZL_ST_DOWN)
	{
		switch(state)
		{
		case ZL_ST_START:
			loopStackTop = & compile->AsmGCLoopStackList.stacks[compile->AsmGCLoopStackList.count - 1];
			nodenum = loopStackTop->nodenum;
			switch(nodes[nodenum].toktype)	//采用switch...case效率更高些。
			{
			case ZL_TK_ASSIGN: //如果当前节点是赋值符号，就进入ZL_ST_INASSIGN状态机。
				state = ZL_ST_INASSIGN;
				break;
			case ZL_TK_COMMA: //判断是否是逗号运算符
				state = ZL_ST_ASM_CODE_INCOMMA;
				break;
			case ZL_TK_PLUS: //+,-,+=,-=运算符进入ZL_ST_ASM_CODE_INPLUS_MINIS
			case ZL_TK_MINIS:
			case ZL_TK_PLUS_ASSIGN:
			case ZL_TK_MINIS_ASSIGN:
				state = ZL_ST_ASM_CODE_INPLUS_MINIS;
				break;
			case ZL_TK_MOD:
			case ZL_TK_TIMES:
			case ZL_TK_DIVIDE:
			case ZL_TK_MOD_ASSIGN:
			case ZL_TK_TIMES_ASSIGN:
			case ZL_TK_DIVIDE_ASSIGN:
				state = ZL_ST_ASM_CODE_INTIME_DIVIDE;
				break;
			case ZL_TK_GREAT: //大于，小于，等于，不等于，大于等于，小于等于运算符则进入ZL_ST_ASM_CODE_INRELATION状态
			case ZL_TK_LESS:
			case ZL_TK_EQUAL:
			case ZL_TK_NOT_EQ:
			case ZL_TK_GREAT_EQ:
			case ZL_TK_LESS_EQ:
				state = ZL_ST_ASM_CODE_INRELATION;
				break;
			case ZL_TK_AND: //逻辑与，逻辑或进入ZL_ST_ASM_CODE_INAND_OR状态
			case ZL_TK_OR:
				state = ZL_ST_ASM_CODE_INAND_OR;
				break;
			case ZL_TK_NEGATIVE: //负号单目运算符
				state = ZL_ST_ASM_CODE_INNEGATIVE;
				break;
			case ZL_TK_BIT_REVERSE: //按位取反运算符
				state = ZL_ST_ASM_CODE_INBIT_REVERSE;
				break;
			case ZL_TK_REVERSE: //取反运算符
				state = ZL_ST_ASM_CODE_INREVERSE;
				break;
			case ZL_TK_ADDRESS: //引用运算符。
				state = ZL_ST_ASM_CODE_INADDRESS;
				break;
			case ZL_TK_FUNCALL: //函数调用
				state = ZL_ST_ASM_CODE_INFUNCALL;
				break;
			case ZL_TK_ARRAY_ITEM: //数组中的某元素。
				state = ZL_ST_ASM_CODE_INARRAY_ITEM;
				break;
			case ZL_TK_CLASS_STATEMENT: //类的声明，假设Poker是一个class类，那么Poker test; 这条语句就是类的声明，在该声明中声明了一个Poker类型的test变量，这个声明中的Poker就是CLASS_STATEMENT
				state = ZL_ST_ASM_CODE_INCLASS_STATEMENT;
				break;
			case ZL_TK_DOT: //点运算符，如test.name这种类成员的引用就是用的点运算符
				state = ZL_ST_ASM_CODE_INDOT;
				break;
			case ZL_TK_COLON: //判断是否是... ? ... : ... 结构中的冒号
				state = ZL_ST_ASM_CODE_INCOLON;
				break;
			case ZL_TK_QUESTION_MARK: //判断是否是... ? ... : ... 结构中的问号
				state = ZL_ST_ASM_CODE_INQUESTION;
				break;
			case ZL_TK_BIT_AND: //按位与，或，异或，&= 等双目位运算符
			case ZL_TK_BIT_AND_ASSIGN:
			case ZL_TK_BIT_OR:
			case ZL_TK_BIT_OR_ASSIGN:
			case ZL_TK_BIT_XOR:
			case ZL_TK_BIT_XOR_ASSIGN:
			case ZL_TK_BIT_RIGHT:
			case ZL_TK_BIT_RIGHT_ASSIGN:
			case ZL_TK_BIT_LEFT:
			case ZL_TK_BIT_LEFT_ASSIGN:
				state = ZL_ST_ASM_CODE_INBITS;
				break;
			default:
				if(nodes[nodenum].tokcategory == ZL_TKCG_OP_PP_MM) //加加，减减运算符
					state = ZL_ST_ASM_CODE_IN_PP_MM;
				else if(nodes[nodenum].tokcategory == ZL_TKCG_OP_FACTOR) //id变量，num数字，float浮点数，str字符串的操作因子。
				{
					switch(nodes[nodenum].toktype)
					{
					case ZL_TK_ID:
						state = ZL_ST_INID;
						break;
					case ZL_TK_NUM:
						state = ZL_ST_INNUM;
						break;
					case ZL_TK_FLOAT:
						state = ZL_ST_INFLOAT;
						break;
					case ZL_TK_STR:
						state = ZL_ST_INSTR;
						break;
					default:
						compile->parser_curnode = nodenum;
						compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_INVALID_OP_FACTOR);
						break;
					}
				}
				else if(nodes[nodenum].toktype == ZL_TK_RESERVE) //各种关键字（保留字）
				{
					switch(nodes[nodenum].reserve)
					{
					case ZL_RSV_PRINT: //打印语句
						state = ZL_ST_ASM_CODE_INPRINT;
						break;
					case ZL_RSV_IF: //if语句
						state = ZL_ST_ASM_CODE_INIF;
						break;
					case ZL_RSV_FOR: //for语句
						state = ZL_ST_ASM_CODE_INFOR;
						break;
					case ZL_RSV_FUN: //fun语句
						state = ZL_ST_ASM_CODE_INFUN;
						break;
					case ZL_RSV_CLASS: //class语句
						state = ZL_ST_ASM_CODE_INCLASS;
						break;
					case ZL_RSV_USE: //use语句
						state = ZL_ST_ASM_CODE_INUSE;
						break;
					case ZL_RSV_BREAK: //break语句
						state = ZL_ST_ASM_CODE_INBREAK;
						break;
					case ZL_RSV_CONTINUE: //continue语句
						state = ZL_ST_ASM_CODE_INCONTINUE;
						break;
					case ZL_RSV_GLOBAL: //global语句
						state = ZL_ST_ASM_CODE_INGLOBAL;
						break;
					case ZL_RSV_RETURN: //return语句
						state = ZL_ST_ASM_CODE_INRETURN;
						break;
					case ZL_RSV_SWITCH: //switch...case语句，就进入ZL_ST_ASM_CODE_INSWITCH状态
						state = ZL_ST_ASM_CODE_INSWITCH;
						break;
					case ZL_RSV_WHILE: //while...endwhile循环结构
						state = ZL_ST_ASM_CODE_INWHILE;
						break;
					case ZL_RSV_DO: //do...dowhile循环结构
						state = ZL_ST_ASM_CODE_INDOWHILE;
						break;
					default:
						compile->parser_curnode = nodenum;
						compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_INVALID_RESERVE);
						break;
					}
				}
				else
				{
					compile->parser_curnode = nodenum;
					compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_INVALID_TOKEN_CAN_NOT_GEN_CODE);
				}
				break;
			}	//switch(nodes[nodenum].toktype)
			break;	//case ZL_ST_START:
		case ZL_ST_INID:
		case ZL_ST_INNUM:
		case ZL_ST_INFLOAT:
		case ZL_ST_INSTR: 
			state = zengl_AsmGenCode_Factor(VM_ARG, state, &loopStackTop);
			break;
		case ZL_ST_INASSIGN: //赋值运算符的汇编指令输出
			state = zengl_AsmGenCode_Assign(VM_ARG, state, &loopStackTop);
			break;
		case ZL_ST_ASM_CODE_INBITS: //按位与，或，异或等双目位运算符
		case ZL_ST_ASM_CODE_INRELATION:	//大于等于之类的关系比较运算符
		case ZL_ST_ASM_CODE_INAND_OR:	//与或运算符
		case ZL_ST_ASM_CODE_INTIME_DIVIDE://乘除取余运算符汇编输出
		case ZL_ST_ASM_CODE_INPLUS_MINIS: //加减运算符汇编输出
			state = zengl_AsmGenCode_ForTwo(VM_ARG, state, &loopStackTop);
			break;
		case ZL_ST_ASM_CODE_INNEGATIVE: //负号单目运算符的汇编输出
			state = zengl_AsmGenCode_Negative(VM_ARG, state, &loopStackTop);
			break;
		case ZL_ST_ASM_CODE_INCOMMA: //逗号运算符和c语言的逗号运算符作用是一样的，都是从左往右，依次执行表达式，并以最后一个表达式的结果作为返回结果。
			state = zengl_AsmGenCode_Comma(VM_ARG, state, &loopStackTop);
			break;
		case ZL_ST_ASM_CODE_INBIT_REVERSE: //按位取反运算符的处理
		case ZL_ST_ASM_CODE_INREVERSE: //取反运算符的处理
			state = zengl_AsmGenCode_Reverse(VM_ARG, state, &loopStackTop);
			break;
		case ZL_ST_ASM_CODE_INADDRESS: //引用运算符的处理
			state = zengl_AsmGenCode_Address(VM_ARG, state, &loopStackTop);
			break;
		case ZL_ST_ASM_CODE_IN_PP_MM: //++和--运算符，这两个运算符的作用也和C语言的一样，就是对变量或表达式结果加一或减一，当++在前面时，先加一再取值，当++在后面时，先取值后加一，--同理
			state = zengl_AsmGenCode_PP_MM(VM_ARG, state, &loopStackTop);
			break;
		case ZL_ST_ASM_CODE_INPRINT: //打印语句的汇编代码输出
			state = zengl_AsmGenCode_Print(VM_ARG, state, &loopStackTop);
			break;
		case ZL_ST_ASM_CODE_INIF: //if-elif-else 控制语句的汇编代码输出
			state = zengl_AsmGenCode_If_Elif_Else(VM_ARG, state, &loopStackTop);
			break;
		case ZL_ST_ASM_CODE_INELIF:
			state = zengl_AsmGCElif(VM_ARG, ZL_NULL, 0, &loopStackTop);
			break;
		case ZL_ST_ASM_CODE_INFOR: //for...endfor..循环控制语句汇编代码输出
			state = zengl_AsmGenCode_For(VM_ARG, state, &loopStackTop);
			break;
		case ZL_ST_ASM_CODE_INFUN: //fun...endfun 函数汇编代码生成
			state = zengl_AsmGenCode_Fun(VM_ARG, state, &loopStackTop);
			break;
		case ZL_ST_ASM_CODE_INFUNCALL: //如果是函数调用，就输出相应的汇编代码。
			state = zengl_AsmGenCode_Funcall(VM_ARG, state, &loopStackTop);
			break;
		case ZL_ST_ASM_CODE_INARRAY_ITEM: //生成数组元素的汇编代码。例如test[a,b+1,c] 这是一个三维的数组，会以此将a,b+1,c的值压入栈，ARRAY_ITEM寄存器指向第一个a的栈位置，由a得到第一维，由b+1的值得到第二维，由c得到第三维，所以test[a,b+1,c]类似php的test[a][b+1][c]，当然a,b+1和c都是索引而非php的数组键名。
			state = zengl_AsmGenCode_ArrayItem(VM_ARG, state, &loopStackTop);
			break;
		case ZL_ST_ASM_CODE_INUSE: //use语句的汇编代码生成
			if(nodes[nodenum].childs.count == 1)
			{
				chnum = nodes[nodenum].childs.childnum;
				compile->SymScanUseRsv(VM_ARG,chnum[0],nodenum);
				loopStackTop = ZL_NULL;
				state = zengl_AsmGCLoopStackFinishTop(VM_ARG, nodenum);
			}
			else
			{
				compile->parser_curnode = nodenum;
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_ONE_CHILD);
			}
			break; //case ZL_ST_ASM_CODE_INUSE: //use语句的汇编代码生成
		case ZL_ST_ASM_CODE_INBREAK: //break关键字的汇编输出
			//compile->AsmGCBreak_Codes(VM_ARG,nodenum);
			zengl_AsmGCBreak_Codes(VM_ARG,nodenum);
			loopStackTop = ZL_NULL;
			state = zengl_AsmGCLoopStackFinishTop(VM_ARG, nodenum);
			break;
		case ZL_ST_ASM_CODE_INCONTINUE: //continue关键字的汇编输出
			//compile->AsmGCContinue_Codes(VM_ARG,nodenum);
			zengl_AsmGCContinue_Codes(VM_ARG,nodenum);
			loopStackTop = ZL_NULL;
			state = zengl_AsmGCLoopStackFinishTop(VM_ARG, nodenum);
			break;
		case ZL_ST_ASM_CODE_INGLOBAL: //global语句汇编输出
			if(compile->gencode_struct.is_inFun == ZL_TRUE) { //判断global关键字是否使用在函数里，因为global在前面的ScanFunArg_Global函数里进行过扫描，所以这里就将state设为down，直接返回。
				//state = ZL_ST_DOWN;
				loopStackTop = ZL_NULL;
				state = zengl_AsmGCLoopStackFinishTop(VM_ARG, nodenum);
			}
			else
			{
				compile->parser_curnode = nodenum;
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_UNEXPECT_LOC_OF_GLOBAL);
			}
			break;
		case ZL_ST_ASM_CODE_INRETURN: //return语句汇编输出
			state = zengl_AsmGenCode_Return(VM_ARG, state, &loopStackTop);
			break;
		case ZL_ST_ASM_CODE_INCLASS: //class语句的汇编输出
			state = zengl_AsmGenCode_Class(VM_ARG, state, &loopStackTop);
			break;
		case ZL_ST_ASM_CODE_INCLASS_STATEMENT: //例如假设Poker是一个类，那么类的声明Poker test; 就会将Poker类的classid加入到test变量所在的全局或局部哈希表的classid成员中
			if(nodes[nodenum].childs.count == 1)
			{
				chnum = nodes[nodenum].childs.childnum;
				compile->SymScanClassStatement(VM_ARG,chnum[0],nodenum);
				//state = ZL_ST_DOWN;
				loopStackTop = ZL_NULL;
				state = zengl_AsmGCLoopStackFinishTop(VM_ARG, nodenum);
			}
			else
			{
				compile->parser_curnode = nodenum;
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_ONE_CHILD);
			}
			break; //case ZL_ST_ASM_CODE_INCLASS_STATEMENT:
		case ZL_ST_ASM_CODE_INDOT: //通过点运算符访问类的成员
			state = zengl_AsmGenCode_Dot(VM_ARG, state, &loopStackTop);
			break;
		case ZL_ST_ASM_CODE_INDOT_SYM_SCAN:
			state = compile->SymScanDotForClass(VM_ARG, 0, &loopStackTop);
			break;
		case ZL_ST_ASM_CODE_INSWITCH: //switch...case结构汇编输出
			if(nodes[nodenum].childs.count >= 1)
			{
				ZL_BOOL hasminmax,hasdefault;
				ZL_INT max,min,num,j;
				ZENGL_ASM_CASE_JMP_TABLE casejmptable = {0}; //switch...case的跳转表
				chnum = nodes[nodenum].childs.childnum;
				extnum = nodes[nodenum].childs.extchilds;
				switch(nodes[chnum[0]].toktype) //先将要比较的值赋值给AX
				{
				case ZL_TK_ID:
					inst_op_data = compile->SymLookupID(VM_ARG,chnum[0]);
					run->AddInst(VM_ARG,compile->gencode_struct.pc++,chnum[0],
									ZL_R_IT_MOV,ZL_R_DT_REG,ZL_R_RT_AX,
									inst_op_data.type,inst_op_data.val.mem); //对应汇编指令 类似 "MOV AX (%d)"
					break;
				case ZL_TK_NUM:
					inst_op_data.val.num = ZENGL_SYS_STR_TO_LONG_NUM(compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[0]].strindex));
					run->AddInst(VM_ARG,compile->gencode_struct.pc++,chnum[0],
									ZL_R_IT_MOV,ZL_R_DT_REG,ZL_R_RT_AX,
									ZL_R_DT_NUM,inst_op_data.val.num); //对应汇编指令 类似 "MOV AX 123"
					break;
				case ZL_TK_FLOAT:
					inst_op_data.val.floatnum = ZENGL_SYS_STR_TO_FLOAT(compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[0]].strindex));
					run->AddInst(VM_ARG,compile->gencode_struct.pc++,chnum[0],
									ZL_R_IT_MOV,ZL_R_DT_REG,ZL_R_RT_AX,
									ZL_R_DT_FLOAT,inst_op_data.val.floatnum); //对应汇编指令 类似 "MOV AX 3.1415926"
					break;
				case ZL_TK_STR:
					inst_op_data.val.num = (ZL_LONG)compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[0]].strindex);
					run->AddInst(VM_ARG,compile->gencode_struct.pc++,chnum[0],
									ZL_R_IT_MOV,ZL_R_DT_REG,ZL_R_RT_AX,
									ZL_R_DT_STR,inst_op_data.val.num); //对应汇编指令 类似 [MOV AX "hello world"]
					break;
				default:
					if(ZENGL_AST_ISTOKEXPRESS(chnum[0])) //表达式的结果默认就是存放在AX中的，所以直接AsmGenCodes生成表达式的汇编代码即可
						compile->AsmGenCodes(VM_ARG,chnum[0]);
					else
					{
						compile->parser_curnode = nodenum;
						compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_INVALID_CHILD_NODE_TYPE,
							compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[0]].strindex),
							nodes[chnum[0]].line_no,
							nodes[chnum[0]].col_no,
							nodes[chnum[0]].filename);
					}
					break;
				} //switch(nodes[chnum[0]].toktype)
				compile->AsmGCStackPush(VM_ARG,compile->AsmGCAddrNum++,ZL_ASM_STACK_ENUM_SWITCH_TABLE);
				compile->AsmGCStackPush(VM_ARG,compile->AsmGCAddrNum++,ZL_ASM_STACK_ENUM_SWITCH_DEFAULT);
				compile->AsmGCStackPush(VM_ARG,compile->AsmGCAddrNum++,ZL_ASM_STACK_ENUM_SWITCH_END);
				compile->AsmScanCaseMinMax(VM_ARG,nodenum,&hasminmax,&min,&max,&hasdefault,&casejmptable);
				if(hasdefault == ZL_TRUE) //如果有default则输出SWITCH adr%d(跳转表地址) adr%d(default汇编地址)的汇编格式
					run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
						ZL_R_IT_SWITCH,ZL_R_DT_LDADDR,compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_SWITCH_TABLE,ZL_FALSE),
						ZL_R_DT_LDADDR,compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_SWITCH_DEFAULT,ZL_FALSE));
				else //如果没有default则输出SWITCH adr%d(跳转表地址) adr%d(switch结构的结束位置)的汇编格式
					run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
						ZL_R_IT_SWITCH,ZL_R_DT_LDADDR,compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_SWITCH_TABLE,ZL_FALSE),
						ZL_R_DT_LDADDR,compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_SWITCH_END,ZL_FALSE));
				run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
						ZL_R_IT_LONG,ZL_R_DT_NUM,casejmptable.count,
						ZL_R_DT_NUM,casejmptable.count); //对应汇编指令 类似 "LONG %d %d" LONG后面的两个操作数都是当前switch...case跳转表中的case总数
				if(hasminmax == ZL_TRUE)
					run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
						ZL_R_IT_LONG,ZL_R_DT_NUM,min,
						ZL_R_DT_NUM,max); //对应汇编指令 类似 "LONG %d %d" LONG后面分别是case中的最小值和最大值
				i = 1;
				j = 0;
				while(i < nodes[nodenum].childs.count)
				{
					if(i >= ZL_AST_CHILD_NODE_SIZE) //如果超过基本子节点数则使用扩展子节点
						tmpch = extnum[i - ZL_AST_CHILD_NODE_SIZE];
					else
						tmpch = chnum[i];
					if(nodes[tmpch].reserve == ZL_RSV_CASE) //生成case块部分的执行代码
					{
						casejmptable.member[j++].caseAddr = compile->gencode_struct.pc; //将本case块的执行代码的汇编位置设置到跳转表的caseAddr字段
						i++;
						if(i < nodes[nodenum].childs.count)
						{
							if(i >= ZL_AST_CHILD_NODE_SIZE) //如果超过基本子节点数则使用扩展子节点
								tmpch = extnum[i - ZL_AST_CHILD_NODE_SIZE];
							else
								tmpch = chnum[i];
							switch(nodes[tmpch].reserve)
							{
							case ZL_RSV_CASE:
							case ZL_RSV_DEFAULT:
								break;
							default:
								while(tmpch > 0) //循环生成本case块的汇编执行代码
								{
									compile->AsmGenCodes(VM_ARG,tmpch);
									tmpch = nodes[tmpch].nextnode;
								}
								i++;
								break;
							}
						}
						if(i >= nodes[nodenum].childs.count)
							run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
								ZL_R_IT_JMP,ZL_R_DT_NONE,0,
								ZL_R_DT_LDADDR,compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_SWITCH_END,ZL_FALSE)); //对应汇编指令 类似 "JMP adr%d" 如果当前是最后一个case，且case后无default则JMP跳转到switch...case结束位置，在switch...case结束位置前存放的是跳转表，所以这里必须JMP进行跳转
					} //if(nodes[tmpch].reserve == ZL_RSV_CASE) 
					else if(nodes[tmpch].reserve == ZL_RSV_DEFAULT) //生成default块部分的执行代码
					{
						compile->LDAddrListSet(VM_ARG,compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_SWITCH_DEFAULT,ZL_FALSE),compile->gencode_struct.pc);
						i++;
						if(i < nodes[nodenum].childs.count)
						{
							if(i >= ZL_AST_CHILD_NODE_SIZE) //如果超过基本子节点数则使用扩展子节点
								tmpch = extnum[i - ZL_AST_CHILD_NODE_SIZE];
							else
								tmpch = chnum[i];
							while(tmpch > 0) //生成default块部分的汇编代码
							{
								compile->AsmGenCodes(VM_ARG,tmpch);
								tmpch = nodes[tmpch].nextnode;
							}
						}
						run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
								ZL_R_IT_JMP,ZL_R_DT_NONE,0,
								ZL_R_DT_LDADDR,compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_SWITCH_END,ZL_FALSE)); //对应汇编指令 类似 "JMP adr%d" JMP跳转到switch...case的结束位置
						break;
					} //else if(nodes[tmpch].reserve == ZL_RSV_DEFAULT)
				} //while(i < nodes[nodenum].childs.count)
				if(casejmptable.count > 0)
					compile->AsmSortCaseJmpTable(VM_ARG,&casejmptable,nodenum); //将switch...case的跳转表按从小到大的顺序进行排序，为下面生成跳转表的汇编代码做准备
				compile->LDAddrListSet(VM_ARG,compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_SWITCH_TABLE,ZL_FALSE),compile->gencode_struct.pc); 
				if(hasminmax == ZL_TRUE) //如果有case最小值和最大值则说明switch...case中有case节点，就有必要生成跳转表的汇编代码
				{
					num = casejmptable.count;
					for(i=0;i<num;i++)	//循环生成跳转表的汇编代码
					{
						if(casejmptable.member[i].caseAddr != 0)
							run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
								ZL_R_IT_LONG,ZL_R_DT_NUM,casejmptable.member[i].caseAddr,
								ZL_R_DT_NUM,casejmptable.member[i].caseNum); //对应汇编指令 类似 "LONG %d %d" LONG后面的分别是跳转表中存放的case的执行代码的位置和case的比较数
						else
							compile->exit(VM_ARG,ZL_ERR_CP_ASM_CASE_JMP_TABLE_CASEADDR_CAN_NOT_BE_ZERO);
					}
				}
				compile->LDAddrListSet(VM_ARG,compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_SWITCH_END,ZL_FALSE),compile->gencode_struct.pc); //将switch...case的结束位置加入到链接器中
				if(hasminmax == ZL_TRUE && casejmptable.member != ZL_NULL)
					compile->memFreeOnce(VM_ARG,casejmptable.member); //释放掉当前switch...case的跳转表
				compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_SWITCH_TABLE,ZL_TRUE);
				compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_SWITCH_DEFAULT,ZL_TRUE);
				compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_SWITCH_END,ZL_TRUE);
				state = ZL_ST_DOWN;
			} //if(nodes[nodenum].childs.count >= 1)
			else
			{
				compile->parser_curnode = nodenum;
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_AT_LEAST_ONE_CHILD);
			}
			break; //case ZL_ST_ASM_CODE_INSWITCH:
		case ZL_ST_ASM_CODE_INWHILE: //while...endwhile循环结构汇编代码生成
			if(nodes[nodenum].childs.count == 2)
			{
				chnum = nodes[nodenum].childs.childnum;
				compile->AsmGCStackPush(VM_ARG,compile->AsmGCAddrNum++,ZL_ASM_STACK_ENUM_WHILE_ADDR);
				compile->AsmGCStackPush(VM_ARG,compile->AsmGCAddrNum++,ZL_ASM_STACK_ENUM_WHILE_END);
				compile->LDAddrListSet(VM_ARG,compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_WHILE_ADDR,ZL_FALSE),compile->gencode_struct.pc); //将开头条件判断的汇编位置加入链接动态数组中
				if(chnum[0] != -1 && ZENGL_AST_ISTOKEXPRESS(chnum[0])) //先生成while括号里的条件判断表达式的汇编码
				{
					compile->AsmGenCodes(VM_ARG,chnum[0]);
					run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
							ZL_R_IT_JE,ZL_R_DT_NONE,0,
							ZL_R_DT_LDADDR,compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_WHILE_END,ZL_FALSE)); //对应汇编指令 类似 "JE adr%d" 根据条件判断是否需要跳出循环
				}
				i = chnum[1];
				while(i > 0) //循环生成while...endwhile里的代码块的汇编码
				{
					compile->AsmGenCodes(VM_ARG,i);
					i = nodes[i].nextnode;
				}
				run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
						ZL_R_IT_JMP,ZL_R_DT_NONE,0,
						ZL_R_DT_LDADDR,compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_WHILE_ADDR,ZL_FALSE)); //对应汇编指令 类似 "JMP adr%d" 循环体执行完后，跳到开头进行条件判断
				compile->LDAddrListSet(VM_ARG,compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_WHILE_END,ZL_FALSE),compile->gencode_struct.pc); //将while结束位置加入到链接动态数组中
				compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_WHILE_ADDR,ZL_TRUE);
				compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_WHILE_END,ZL_TRUE);
				state = ZL_ST_DOWN;
			}
			else
			{
				compile->parser_curnode = nodenum;
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_TWO_CHILDS);
			}
			break; //case ZL_ST_ASM_CODE_INWHILE: //while...endwhile循环结构汇编代码生成
		case ZL_ST_ASM_CODE_INDOWHILE: //do...dowhile循环结构的汇编代码生成
			if(nodes[nodenum].childs.count == 2)
			{
				chnum = nodes[nodenum].childs.childnum;
				compile->AsmGCStackPush(VM_ARG,compile->AsmGCAddrNum++,ZL_ASM_STACK_ENUM_DO_ADDR);
				compile->AsmGCStackPush(VM_ARG,compile->AsmGCAddrNum++,ZL_ASM_STACK_ENUM_DO_CONTINUE);
				compile->AsmGCStackPush(VM_ARG,compile->AsmGCAddrNum++,ZL_ASM_STACK_ENUM_DO_END);
				compile->LDAddrListSet(VM_ARG,compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_DO_ADDR,ZL_FALSE),compile->gencode_struct.pc); //将do...dowhile结构开头汇编位置加入到链接动态数组中
				i = chnum[0];
				while(i > 0) //循环生成do...dowhile里的代码块的汇编码
				{
					compile->AsmGenCodes(VM_ARG,i);
					i = nodes[i].nextnode;
				}
				compile->LDAddrListSet(VM_ARG,compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_DO_CONTINUE,ZL_FALSE),compile->gencode_struct.pc); //将continue需要跳转的位置加入到链接动态数组中
				if(chnum[1] != -1 && ZENGL_AST_ISTOKEXPRESS(chnum[1])) //生成dowhile括号里条件判断表达式的汇编代码
				{
					compile->AsmGenCodes(VM_ARG,chnum[1]);
					run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
						ZL_R_IT_JNE,ZL_R_DT_NONE,0,
						ZL_R_DT_LDADDR,compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_DO_ADDR,ZL_FALSE)); //对应汇编指令 类似 "JNE adr%d"
				}
				else
					run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
						ZL_R_IT_JMP,ZL_R_DT_NONE,0,
						ZL_R_DT_LDADDR,compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_DO_ADDR,ZL_FALSE)); //如果条件判断语句是空的，则JMP无限循环，此时只有break语句才可以跳出循环
				compile->LDAddrListSet(VM_ARG,compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_DO_END,ZL_FALSE),compile->gencode_struct.pc); //将do...dowhile的结束位置加入到链接器的动态数组中
				compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_DO_ADDR,ZL_TRUE);
				compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_DO_CONTINUE,ZL_TRUE);
				compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_DO_END,ZL_TRUE);
				state = ZL_ST_DOWN;
			}
			else
			{
				compile->parser_curnode = nodenum;
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_TWO_CHILDS);
			}
			break; //case ZL_ST_ASM_CODE_INDOWHILE: //do...dowhile循环结构的汇编代码生成
		case ZL_ST_ASM_CODE_INCOLON: //如果是 ... ? ... : ... 中的冒号运算符
			if(nodes[nodenum].childs.count == 2)
			{
				compile->AsmGCStackPush(VM_ARG,compile->AsmGCAddrNum++,ZL_ASM_STACK_ENUM_QUESTION_END);
				chnum = nodes[nodenum].childs.childnum;
				if(nodes[chnum[0]].toktype == ZL_TK_QUESTION_MARK)
					compile->AsmGenCodes(VM_ARG,chnum[0]);
				else
				{
					compile->parser_curnode = nodenum;
					compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_INVALID_CHILD_NODE_TYPE,
						compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[0]].strindex),
						nodes[chnum[0]].line_no,
						nodes[chnum[0]].col_no,
						nodes[chnum[0]].filename);
				}
				if(nodes[chnum[1]].tokcategory == ZL_TKCG_OP_FACTOR || ZENGL_AST_ISTOKEXPRESS(chnum[1]))
					compile->AsmGenCodes(VM_ARG,chnum[1]);
				else
				{
					compile->parser_curnode = nodenum;
					compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_INVALID_CHILD_NODE_TYPE,
						compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[1]].strindex),
						nodes[chnum[1]].line_no,
						nodes[chnum[1]].col_no,
						nodes[chnum[1]].filename);
				}
				/*设置... ? ... : ...整个表达式结束的跳转汇编地址，并将ZL_ASM_STACK_ENUM_QUESTION_END从栈中弹出，
				因为如果问号前的表达式为TRUE的时候在执行完问号后的表达式后，就应当结束执行，例如a > b ? test(a) : test(b);如果a>b则执行完test(a)就应当结束，
				而不应该继续执行test(b)了*/
				compile->LDAddrListSet(VM_ARG,compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_QUESTION_END,ZL_TRUE),compile->gencode_struct.pc);
				state = ZL_ST_DOWN;
			}
			else
			{
				compile->parser_curnode = nodenum;
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_TWO_CHILDS);
			}
			break; //case ZL_ST_ASM_CODE_INCOLON: 
		case ZL_ST_ASM_CODE_INQUESTION: //如果是 ... ? ... : ... 中的问号运算符
			if(nodes[nodes[nodenum].parentnode].toktype != ZL_TK_COLON)
			{
				compile->parser_curnode = nodenum;
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_QUESTION_NO_COLON_RIGHT);
			}
			if(nodes[nodenum].childs.count == 2) //判断节点是否有两个子节点
			{
				compile->AsmGCStackPush(VM_ARG,compile->AsmGCAddrNum++,ZL_ASM_STACK_ENUM_QUESTION_ADDR);
				chnum = nodes[nodenum].childs.childnum;  //获取子节点的索引数组
				if(nodes[chnum[0]].tokcategory == ZL_TKCG_OP_FACTOR || ZENGL_AST_ISTOKEXPRESS(chnum[0]))
					compile->AsmGenCodes(VM_ARG,chnum[0]);
				else
				{
					compile->parser_curnode = nodenum;
					compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_INVALID_CHILD_NODE_TYPE,
						compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[0]].strindex),
						nodes[chnum[0]].line_no,
						nodes[chnum[0]].col_no,
						nodes[chnum[0]].filename);
				}
				run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
						ZL_R_IT_JE,ZL_R_DT_NONE,0,
						ZL_R_DT_LDADDR,compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_QUESTION_ADDR,ZL_FALSE)); //对应汇编指令 类似 "JE adr%d" 如果判断结果为FALSE(所有等于0或空字符串都代表FALSE)，则跳转执行第二个表达式
				if(nodes[chnum[1]].tokcategory == ZL_TKCG_OP_FACTOR || ZENGL_AST_ISTOKEXPRESS(chnum[1]))
					compile->AsmGenCodes(VM_ARG,chnum[1]);
				else
				{
					compile->parser_curnode = nodenum;
					compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_INVALID_CHILD_NODE_TYPE,
						compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[1]].strindex),
						nodes[chnum[1]].line_no,
						nodes[chnum[1]].col_no,
						nodes[chnum[1]].filename);
				}
				run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
						ZL_R_IT_JMP,ZL_R_DT_NONE,0,
						ZL_R_DT_LDADDR,compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_QUESTION_END,ZL_FALSE)); //对应汇编指令 类似 "JMP adr%d" 
				compile->LDAddrListSet(VM_ARG,compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_QUESTION_ADDR,ZL_TRUE),compile->gencode_struct.pc); //设置问号右边表达式结尾处的跳转汇编地址，并将ZL_ASM_STACK_ENUM_QUESTION_ADDR从栈中弹出
				state = ZL_ST_DOWN;
			}
			else
			{
				compile->parser_curnode = nodenum;
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_TWO_CHILDS);
			}
			break; //case ZL_ST_ASM_CODE_INQUESTION:
		}	//switch(state)
	}	//while(state != ZL_ST_DOWN)
} //ZL_VOID zengl_AsmGenCodes(ZL_VOID * VM_ARG,ZL_INT nodenum)

/*
	将数字压入汇编堆栈
*/
ZL_INT zengl_AsmGCStackPush(ZL_VOID * VM_ARG,ZL_INT num,ZENGL_ASM_STACK_ENUM type)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZL_INT i,index=-1;
	if(!compile->AsmGCStackList.isInit)
		compile->AsmGCStackInit(VM_ARG);
	i=0;
find_index:
	for(;i < compile->AsmGCStackList.size - 1;i++)
	{
		if(compile->AsmGCStackList.stacks[i].isvalid == ZL_FALSE)
		{
			index = i;
			break;
		}
	}
	if(index == -1)
	{
		compile->AsmGCStackList.size += ZL_ASM_STACK_LIST_SIZE;
		compile->AsmGCStackList.stacks = compile->memReAlloc(VM_ARG,compile->AsmGCStackList.stacks,compile->AsmGCStackList.size * sizeof(ZENGL_ASM_STACK_TYPE));
		ZENGL_SYS_MEM_SET(compile->AsmGCStackList.stacks + (compile->AsmGCStackList.size - ZL_ASM_STACK_LIST_SIZE),0,
			ZL_ASM_STACK_LIST_SIZE * sizeof(ZENGL_ASM_STACK_TYPE));
		i = compile->AsmGCStackList.size - ZL_ASM_STACK_LIST_SIZE;
		goto find_index;
	}
	compile->AsmGCStackList.stacks[index].addrnum = num;
	compile->AsmGCStackList.stacks[index].addrtype = type;
	compile->AsmGCStackList.stacks[index].before_index = compile->AsmGCStackList.ends[type];
	compile->AsmGCStackList.stacks[index].isvalid = ZL_TRUE;
	compile->AsmGCStackList.ends[type] = index;
	compile->AsmGCStackList.count++;
	return num;
}

/*
	弹出汇编栈
*/
ZL_INT zengl_AsmGCStackPop(ZL_VOID * VM_ARG,ZENGL_ASM_STACK_ENUM type,ZL_BOOL isremove)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZL_INT end=-1,retnum;
	if(!compile->AsmGCStackList.isInit) //在压栈和弹出栈时需要判断是否初始化，如果没有初始化就要进行初始化。
		compile->AsmGCStackInit(VM_ARG);
	if(compile->AsmGCStackList.count <= 0)
	{
		return -1;
	}
	end = compile->AsmGCStackList.ends[type];
	if(end == -1)
		return -1;
	if(compile->AsmGCStackList.stacks[end].isvalid == ZL_TRUE && compile->AsmGCStackList.stacks[end].addrtype == type)
	{
		if(isremove == ZL_TRUE)
		{
			retnum = compile->AsmGCStackList.stacks[end].addrnum;
			compile->AsmGCStackList.ends[type] = compile->AsmGCStackList.stacks[end].before_index;
			compile->AsmGCStackList.stacks[end].isvalid = ZL_FALSE;
			compile->AsmGCStackList.count--;
			return retnum;
		}
		else
			return compile->AsmGCStackList.stacks[end].addrnum;
	}
	return -1;
}

/*
	汇编堆栈初始化
*/
ZL_VOID zengl_AsmGCStackInit(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZL_INT i;
	if(compile->AsmGCStackList.isInit)
		return;
	compile->AsmGCStackList.count = 0;
	compile->AsmGCStackList.size = ZL_ASM_STACK_LIST_SIZE;
	compile->AsmGCStackList.stacks = compile->memAlloc(VM_ARG,compile->AsmGCStackList.size * sizeof(ZENGL_ASM_STACK_TYPE));
	if(compile->AsmGCStackList.stacks == ZL_NULL)
		compile->exit(VM_ARG,ZL_ERR_CP_ASM_GCSTACK_LIST_MALLOC_FAILED);
	ZENGL_SYS_MEM_SET(compile->AsmGCStackList.stacks, 0 , compile->AsmGCStackList.size * sizeof(ZENGL_ASM_STACK_TYPE));
	for(i=0;i<ZL_ASM_ADDR_TYPE_NUM;i++)
		compile->AsmGCStackList.ends[i] = -1;
	compile->AsmGCStackList.isInit = ZL_TRUE;
}

// 将信息压入模拟堆栈
ZL_VOID zengl_AsmGCLoopStackPush(ZL_VOID * VM_ARG, ZL_INT nodenum, ZENGL_STATES state)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	if(!compile->AsmGCLoopStackList.isInit) {
		zengl_AsmGCLoopStackInit(VM_ARG);
	}
	if(compile->AsmGCLoopStackList.count == compile->AsmGCLoopStackList.size) {
		compile->AsmGCLoopStackList.size += ZL_ASM_LOOP_STACK_LIST_SIZE;
		compile->AsmGCLoopStackList.stacks = compile->memReAlloc(VM_ARG,compile->AsmGCLoopStackList.stacks,
																	compile->AsmGCLoopStackList.size * sizeof(ZENGL_ASM_LOOP_STACK_TYPE));
		ZENGL_SYS_MEM_SET(compile->AsmGCLoopStackList.stacks + (compile->AsmGCLoopStackList.size - ZL_ASM_LOOP_STACK_LIST_SIZE),0,
						ZL_ASM_LOOP_STACK_LIST_SIZE * sizeof(ZENGL_ASM_LOOP_STACK_TYPE));
	}	
	compile->AsmGCLoopStackList.stacks[compile->AsmGCLoopStackList.count].nodenum = nodenum;
	compile->AsmGCLoopStackList.stacks[compile->AsmGCLoopStackList.count].orig_nodenum = nodenum;
	compile->AsmGCLoopStackList.stacks[compile->AsmGCLoopStackList.count].state = state;
	compile->AsmGCLoopStackList.stacks[compile->AsmGCLoopStackList.count].stackValIndex = -1; // -1表示无效的索引，也就是没有在stackVals中存储额外数据
	compile->AsmGCLoopStackList.stacks[compile->AsmGCLoopStackList.count].stackValCnt = 0;
	compile->AsmGCLoopStackList.count++;
}

// 当模拟栈顶的元素生成完所需的汇编指令后，就将栈顶弹出，如果栈内还有元素的话，就返回栈顶元素的状态，继续模拟栈配合循环来生成新的栈顶元素的汇编指令，否则返回ZL_ST_DOWN表示栈内元素全处理完毕
ZENGL_STATES zengl_AsmGCLoopStackFinishTop(ZL_VOID * VM_ARG, ZL_INT nodenum)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZL_INT tmptype;
	tmptype = compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_IS_IN_ARRAYITEM_OR_FUNCALL,ZL_FALSE);
	switch(tmptype)
	{
	case ZL_TK_FUNCALL: //如果是在函数调用中，那么函数调用里的每个参数包括表达式的值都必须压入栈中，这样函数才能从栈中找到需要的参数。
		{
			ZL_INT parent = nodes[nodenum].parentnode;
			switch(nodes[parent].toktype)
			{
			case ZL_TK_COMMA:
			case ZL_TK_FUNCALL:
				if(nodes[nodenum].toktype != ZL_TK_COMMA)
				{
					run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
						ZL_R_IT_PUSH,ZL_R_DT_NONE,0,
						ZL_R_DT_REG,ZL_R_RT_AX); //对应汇编指令 "PUSH AX" 
				}
				break;
			}
		}
		break;
	case ZL_TK_ARRAY_ITEM: //如果是在生成数组元素里的索引表达式时，就输出PUSH AX将表达式的值压栈作为数组的索引值，如test[a+b]语句，a+b执行完后就会PUSH AX将a+b的结果压入栈，并作为test数组的索引值。
		{
			ZL_INT parent = nodes[nodenum].parentnode;
			switch(nodes[parent].toktype)
			{
			case ZL_TK_COMMA:
			case ZL_TK_ARRAY_ITEM:
				if(nodes[nodenum].toktype != ZL_TK_COMMA)
				{
					run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
						ZL_R_IT_PUSH,ZL_R_DT_NONE,0,
						ZL_R_DT_REG,ZL_R_RT_AX); //对应汇编指令 "PUSH AX" 
				}
				break;
			}
		}
		break;
	} //switch(tmptype)
	compile->AsmGCLoopStackList.count--;
	if(compile->AsmGCLoopStackList.count > 0) {
		return compile->AsmGCLoopStackList.stacks[compile->AsmGCLoopStackList.count - 1].state;
	}
	return ZL_ST_DOWN;
}

// 当模拟栈顶的元素生成完所需的汇编指令后，就将栈顶弹出，这是简化的版本，没有针对数组或函数调用的PUSH AX操作，主要用于elif生成汇编指令以及扫描点运算符时使用的。
ZENGL_STATES zengl_AsmGCLoopStackFinishTopSimple(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	compile->AsmGCLoopStackList.count--;
	if(compile->AsmGCLoopStackList.count > 0) {
		return compile->AsmGCLoopStackList.stacks[compile->AsmGCLoopStackList.count - 1].state;
	}
	return ZL_ST_DOWN;
}

// 汇编模拟堆栈的初始化
static ZL_VOID zengl_AsmGCLoopStackInit(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	//ZL_INT i;
	if(compile->AsmGCLoopStackList.isInit)
		return;
	compile->AsmGCLoopStackList.count = 0;
	compile->AsmGCLoopStackList.size = ZL_ASM_LOOP_STACK_LIST_SIZE;
	compile->AsmGCLoopStackList.stacks = compile->memAlloc(VM_ARG,compile->AsmGCLoopStackList.size * sizeof(ZENGL_ASM_LOOP_STACK_TYPE));
	if(compile->AsmGCLoopStackList.stacks == ZL_NULL)
		compile->exit(VM_ARG,ZL_ERR_CP_ASM_GC_LOOP_STACK_LIST_MALLOC_FAILED);
	ZENGL_SYS_MEM_SET(compile->AsmGCLoopStackList.stacks, 0 , compile->AsmGCLoopStackList.size * sizeof(ZENGL_ASM_LOOP_STACK_TYPE));
	compile->AsmGCLoopStackList.isInit = ZL_TRUE;
}

static ZL_VOID zengl_AsmGCLoopStackValsInit(ZL_VOID * VM_ARG)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	if(compile->AsmGCLoopStackList.isInitStackVals)
		return;
	compile->AsmGCLoopStackList.stackValCount = 0;
	compile->AsmGCLoopStackList.stackValSize = ZL_ASM_LOOP_STACK_LIST_SIZE; // 初始化和动态扩容的大小为20个字节
	compile->AsmGCLoopStackList.stackVals = compile->memAlloc(VM_ARG,compile->AsmGCLoopStackList.stackValSize);
	if(compile->AsmGCLoopStackList.stackVals == ZL_NULL)
		compile->exit(VM_ARG,ZL_ERR_CP_ASM_GC_LOOP_STACK_LIST_MALLOC_FAILED);
	ZENGL_SYS_MEM_SET(compile->AsmGCLoopStackList.stackVals, 0 , compile->AsmGCLoopStackList.stackValSize);
	compile->AsmGCLoopStackList.isInitStackVals = ZL_TRUE;
}

// 将汇编输出函数中所需使用的一些局部变量保存到模拟堆栈中
ZL_BYTE * zengl_AsmGCLoopStackValsPush(ZL_VOID * VM_ARG, ZENGL_ASM_LOOP_STACK_TYPE * loopStackTop, ZL_INT count)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZL_INT total_count, orig_count;
	ZL_BYTE * return_point;
	if(count <= 0 || loopStackTop == ZL_NULL)
		return ZL_NULL;
	if(!compile->AsmGCLoopStackList.isInitStackVals)
		zengl_AsmGCLoopStackValsInit(VM_ARG);
	total_count = compile->AsmGCLoopStackList.stackValCount + count;
	if(total_count > compile->AsmGCLoopStackList.stackValSize)
	{
		ZL_INT orig_size = compile->AsmGCLoopStackList.stackValSize;
		ZL_INT increase_size;
		do{
			compile->AsmGCLoopStackList.stackValSize += ZL_ASM_LOOP_STACK_LIST_SIZE; // 以字节为单位
		}while(total_count > compile->AsmGCLoopStackList.stackValSize);
		increase_size = compile->AsmGCLoopStackList.stackValSize - orig_size;
		compile->AsmGCLoopStackList.stackVals = compile->memReAlloc(VM_ARG,compile->AsmGCLoopStackList.stackVals, compile->AsmGCLoopStackList.stackValSize);
		ZENGL_SYS_MEM_SET(compile->AsmGCLoopStackList.stackVals + orig_size, 0, increase_size);
	}
	orig_count = compile->AsmGCLoopStackList.stackValCount;
	compile->AsmGCLoopStackList.stackValCount += count;
	return_point = & compile->AsmGCLoopStackList.stackVals[orig_count];
	loopStackTop->stackValIndex = orig_count;
	loopStackTop->stackValCnt = count;
	return return_point;
}

// 将保存局部变量和其他额外数据的模拟栈的栈顶弹出
ZL_VOID zengl_AsmGCLoopStackValsPop(ZL_VOID * VM_ARG, ZENGL_ASM_LOOP_STACK_TYPE * loopStackTop)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	if(loopStackTop == ZL_NULL)
		return;
	if(!compile->AsmGCLoopStackList.isInitStackVals)
		return;
	compile->AsmGCLoopStackList.stackValCount -= loopStackTop->stackValCnt;
	if(compile->AsmGCLoopStackList.stackValCount < 0)
		compile->AsmGCLoopStackList.stackValCount = 0;
	return;
}

// zengl_AsmGenCode_Fun函数用于生成fun...endfun代码块对应的汇编指令
static ZENGL_STATES zengl_AsmGenCode_Fun(ZL_VOID * VM_ARG, ZENGL_STATES state, ZENGL_ASM_LOOP_STACK_TYPE ** loopStackTopArg)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_ASM_LOOP_STACK_TYPE * loopStackTop = (*loopStackTopArg);
	ZL_INT nodenum;
	ZL_INT orig_nodenum;
	ZL_INT i;
	int * chnum = ZL_NULL;
	if(loopStackTop == ZL_NULL) {
		loopStackTop = & compile->AsmGCLoopStackList.stacks[compile->AsmGCLoopStackList.count - 1];
	}
	nodenum = loopStackTop->nodenum;
	orig_nodenum = loopStackTop->orig_nodenum;
	(*loopStackTopArg) = ZL_NULL;

	if(nodenum != orig_nodenum)
	{
		i = nodenum;
		nodenum = orig_nodenum;
		goto finish_child;
	}
	
	if(nodes[nodenum].childs.count == 3)
	{
		ZL_INT tmpFunid;	//存放当前函数的函数ID的临时变量
		ZL_INT tmpFunEnd = compile->AsmGCAddrNum++;		//存放当前函数结束位置的伪地址值
		ZL_INT tmpFunAddr = compile->AsmGCAddrNum++;	//存放当前函数入口位置的伪地址值
		ZL_INT tmpFunArgPC; //存放FUNARG指令汇编代码位置的临时变量
		ZL_INT tmpClassID = 0; //存放类函数ID信息
		loopStackTop->state = state;

		if(compile->gencode_struct.is_inFun)
		{
			compile->parser_curnode = nodenum;
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_FUN_CAN_NOT_DEFINED_IN_OTHER_FUN);
		}
		compile->gencode_struct.is_inFun = ZL_TRUE;
		chnum = nodes[nodenum].childs.childnum;
		if(compile->AsmGCIsInClass == ZL_TRUE) //类函数时，先获取类ID信息
		{
			tmpClassID = compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_FUN_CLASSID,ZL_FALSE);
			if(tmpClassID < 0)
				tmpClassID = 0;
		}
		compile->SymFunTable.global_funid = tmpFunid = compile->SymLookupFun(VM_ARG,chnum[0],tmpClassID); //fun第一个子节点是函数名
		run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
				ZL_R_IT_JMP,ZL_R_DT_NONE,0,
				ZL_R_DT_LDADDR,tmpFunEnd); //对应汇编指令 类似 "JMP adr%d" 跳转到函数结束位置，这样非函数调用的情况下就不会执行函数体
		compile->LDAddrListSet(VM_ARG,tmpFunAddr,compile->gencode_struct.pc); //设置当前函数的入口地址对应的实际汇编位置
		compile->SymFunTable.funs[tmpFunid].LDAdr = tmpFunAddr; //设置函数的入口伪地址
		tmpFunArgPC = compile->gencode_struct.pc;
		run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
				ZL_R_IT_FUNARG,ZL_R_DT_NONE,0,
				ZL_R_DT_NUM , 0); //对应汇编指令 "FUNARG %d" 下面会对FUNARG的源操作数进行设置，得到具体参数数目，FUNARG会根据参数数目，在运行时在虚拟堆栈中分配对应数量的内存空间用以存放所有的参数。
		compile->gencode_struct.localID = 0;
		compile->SymScanFunArg(VM_ARG,chnum[1]); //根据第二个子节点扫描函数的参数。
		run->inst_list.insts[tmpFunArgPC].src.val.num = compile->gencode_struct.localID;
		compile->gencode_struct.localID = 0;
		compile->SymScanFunLocal(VM_ARG,chnum[2]); //根据第三个子节点扫描函数体内的局部变量和global关键字修饰的全局变量(SymScanFunLocal在扫描时如遇到global关键字就会调用SymScanFunGlobal来处理全局变量)，并对每个局部变量生成一个PUSH_LOC的汇编代码，这样就为每个局部变量分配了一个栈空间。
		i = chnum[2];
		compile->AsmGCStackPush(VM_ARG,0,ZL_ASM_STACK_ENUM_FUN_CLASSID); //防止外层的类ID信息影响到fun函数体内部的funcall函数调用，没有这条代码，则fun里的普通的funcall也都变成类函数调用了！
		loopStackTop->extData[0] = tmpFunEnd;
		while(i > 0) //循环生成fun函数体里的每个表达式的汇编代码
		{
			//compile->AsmGenCodes(VM_ARG,i);
			zengl_AsmGCLoopStackPush(VM_ARG, i, ZL_ST_START);
			if(nodes[i].nextnode > 0)
				loopStackTop->nodenum = nodes[i].nextnode;
			else
				loopStackTop->nodenum = -1;
			return ZL_ST_START;
finish_child:
			;
		}
		compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_FUN_CLASSID,ZL_TRUE); //将前面压入的0的classid弹出去
		run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
				ZL_R_IT_RET,ZL_R_DT_NONE,0,
				ZL_R_DT_NONE,0); //对应汇编指令 "RET"
		tmpFunEnd = loopStackTop->extData[0];
		compile->LDAddrListSet(VM_ARG,tmpFunEnd,compile->gencode_struct.pc); //设置函数的结束地址
		compile->gencode_struct.is_inFun = ZL_FALSE;
		//state = ZL_ST_DOWN;
	}
	else
	{
		compile->parser_curnode = nodenum;
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_THREE_CHILDS);
	}
	return zengl_AsmGCLoopStackFinishTop(VM_ARG, orig_nodenum);
}

// zengl_AsmGenCode_Comma函数用于逗号运算符的汇编指令
static ZENGL_STATES zengl_AsmGenCode_Comma(ZL_VOID * VM_ARG, ZENGL_STATES state, ZENGL_ASM_LOOP_STACK_TYPE ** loopStackTopArg)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_ASM_LOOP_STACK_TYPE * loopStackTop = (*loopStackTopArg);
	ZL_INT nodenum;
	ZL_INT orig_nodenum;
	int * chnum = ZL_NULL;
	if(loopStackTop == ZL_NULL) {
		loopStackTop = & compile->AsmGCLoopStackList.stacks[compile->AsmGCLoopStackList.count - 1];
	}
	nodenum = loopStackTop->nodenum;
	orig_nodenum = loopStackTop->orig_nodenum;
	(*loopStackTopArg) = ZL_NULL;

	chnum = nodes[orig_nodenum].childs.childnum;
	if(nodenum == orig_nodenum) {
		if(nodes[nodenum].childs.count != 2) {
			compile->parser_curnode = nodenum;
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_TWO_CHILDS);
		}
		loopStackTop->state = state;
		if(nodes[chnum[0]].tokcategory == ZL_TKCG_OP_FACTOR || ZENGL_AST_ISTOKEXPRESS(chnum[0])) {
			zengl_AsmGCLoopStackPush(VM_ARG, chnum[0], ZL_ST_START);
			loopStackTop->nodenum = chnum[1];
			return ZL_ST_START;
		}
		else {
			compile->exit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_TOKEN,
				nodes[chnum[0]].line_no,
				nodes[chnum[0]].col_no,
				nodes[chnum[0]].filename,
				compile->getTokenStr(VM_ARG,nodes,chnum[0]));
		}
	}
	else if(nodenum == chnum[1]) {
		if(nodes[chnum[1]].tokcategory == ZL_TKCG_OP_FACTOR || ZENGL_AST_ISTOKEXPRESS(chnum[1])) {
			zengl_AsmGCLoopStackPush(VM_ARG, chnum[1], ZL_ST_START);
			loopStackTop->nodenum = -1;
			return ZL_ST_START;
		}
		else {
			compile->exit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_TOKEN,
				nodes[chnum[1]].line_no,
				nodes[chnum[1]].col_no,
				nodes[chnum[1]].filename,
				compile->getTokenStr(VM_ARG,nodes,chnum[1]));
		}
	}
	return zengl_AsmGCLoopStackFinishTop(VM_ARG, orig_nodenum);
}

// 生成与操作因子相关的汇编指令
static ZENGL_STATES zengl_AsmGenCode_Factor(ZL_VOID * VM_ARG, ZENGL_STATES state, ZENGL_ASM_LOOP_STACK_TYPE ** loopStackTopArg)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_ASM_LOOP_STACK_TYPE * loopStackTop = (*loopStackTopArg);
	ZL_INT nodenum;
	ZL_INT orig_nodenum;
	ZENGL_RUN_INST_OP_DATA inst_op_data;
	if(loopStackTop == ZL_NULL) {
		loopStackTop = & compile->AsmGCLoopStackList.stacks[compile->AsmGCLoopStackList.count - 1];
	}
	nodenum = loopStackTop->nodenum;
	orig_nodenum = loopStackTop->orig_nodenum;
	(*loopStackTopArg) = ZL_NULL;

	switch(state)
	{
	case ZL_ST_INID: // 将变量的值存入AX寄存器，以便函数调用等能够通过 PUSH AX 指令将变量的值压入栈，从而供函数等使用
		inst_op_data = compile->SymLookupID(VM_ARG,nodenum);
		run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
						ZL_R_IT_MOV,ZL_R_DT_REG,ZL_R_RT_AX,
						inst_op_data.type,inst_op_data.val.mem); //对应的汇编指令 类似 "MOV AX (%d)"
		break;
	case ZL_ST_INNUM: //解释同上
		inst_op_data.val.num = ZENGL_SYS_STR_TO_LONG_NUM(compile->TokenStringPoolGetPtr(VM_ARG,nodes[nodenum].strindex));
		run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
						ZL_R_IT_MOV,ZL_R_DT_REG,ZL_R_RT_AX,
						ZL_R_DT_NUM,inst_op_data.val.num); //对应的汇编指令 类似 "MOV AX 123"
		break;
	case ZL_ST_INFLOAT: //解释同上
		inst_op_data.val.floatnum = ZENGL_SYS_STR_TO_FLOAT(compile->TokenStringPoolGetPtr(VM_ARG,nodes[nodenum].strindex));
		run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
						ZL_R_IT_MOV,ZL_R_DT_REG,ZL_R_RT_AX,
						ZL_R_DT_FLOAT,inst_op_data.val.floatnum); //对应的汇编指令 类似 "MOV AX 3.1415926"
		break;
	case ZL_ST_INSTR: //解释同上
		inst_op_data.val.num = (ZL_LONG)compile->TokenStringPoolGetPtr(VM_ARG,nodes[nodenum].strindex);
		run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
						ZL_R_IT_MOV,ZL_R_DT_REG,ZL_R_RT_AX,
						ZL_R_DT_STR,inst_op_data.val.num); //对应的汇编指令 类似 [MOV AX "hello world"]
		break;
	}
	return zengl_AsmGCLoopStackFinishTop(VM_ARG, orig_nodenum);
}

// 生成赋值运算符的第一个子节点的汇编指令，加赋值、减赋值等运算符在生成加减等汇编指令后，也会调用此函数
static ZENGL_STATES zengl_AsmGenCode_Assign_FirstChildNode(ZL_VOID * VM_ARG, ZENGL_AST_NODE_TYPE * nodes, ZL_INT assign_nodenum, 
														   ZENGL_ASM_LOOP_STACK_TYPE * loopStackTop)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_RUN_INST_OP_DATA inst_op_data;
	ZL_INT * chnum = ZL_NULL;
	if(assign_nodenum < 0)
	{
		goto finish;
	}
	chnum = nodes[assign_nodenum].childs.childnum; //获取子节点的索引数组
	switch(nodes[chnum[0]].toktype)
	{
	case ZL_TK_ID: //判断赋值运算符第一个子节点是否是变量标示符
		inst_op_data = compile->SymLookupID(VM_ARG,chnum[0]);
		run->AddInst(VM_ARG, compile->gencode_struct.pc++, chnum[0],
					ZL_R_IT_MOV , inst_op_data.type , inst_op_data.val.mem,
					ZL_R_DT_REG , ZL_R_RT_AX); //对应汇编指令 类似 "MOV (%d) AX"
		return ZL_ST_DOWN;
		break;
	case ZL_TK_ARRAY_ITEM: //判断第一个子节点是否是数组元素或者是类的成员
	case ZL_TK_DOT:
		compile->AsmGCStackPush(VM_ARG,(ZL_INT)ZL_ASM_AI_OP_IN_MOV,ZL_ASM_STACK_ENUM_ARRAY_ITEM_OP_TYPE); //因为数组元素内部还可以嵌入别的数组元素如test[test2[0]]这样的形式，所以通过压栈的方式可以防止嵌套时，内部的数组元素影响外部的数组元素的相关判断
		run->AddInst(VM_ARG, compile->gencode_struct.pc++, chnum[0],
					ZL_R_IT_PUSH , ZL_R_DT_NONE , 0,
					ZL_R_DT_REG , ZL_R_RT_AX); //对应汇编指令 "PUSH AX" 下面的AsmGenCodes生成的表达式的结果默认也是存放在AX中的，所以这里先将AX压栈保存
		zengl_AsmGCLoopStackPush(VM_ARG, chnum[0], ZL_ST_START); //生成数组元素或类成员的汇编指令
		loopStackTop->nodenum = -1;
		return ZL_ST_START;
finish:
		compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_ARRAY_ITEM_OP_TYPE,ZL_TRUE); //将ZL_ASM_STACK_ENUM_ARRAY_ITEM_OP_TYPE弹出栈
		return ZL_ST_DOWN;
		break;
	default:
		compile->parser_curnode = chnum[0];
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_ASSIGN_FIRST_NODE_IS_INVALID_TYPE);
		break;
	}
}

// 生成赋值运算符的汇编指令
static ZENGL_STATES zengl_AsmGenCode_Assign(ZL_VOID * VM_ARG, ZENGL_STATES state, ZENGL_ASM_LOOP_STACK_TYPE ** loopStackTopArg)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_ASM_LOOP_STACK_TYPE * loopStackTop = (*loopStackTopArg);
	ZL_INT nodenum;
	ZL_INT orig_nodenum;
	ZL_INT * chnum = ZL_NULL;
	ZENGL_RUN_INST_OP_DATA inst_op_data;
	if(loopStackTop == ZL_NULL) {
		loopStackTop = & compile->AsmGCLoopStackList.stacks[compile->AsmGCLoopStackList.count - 1];
	}
	nodenum = loopStackTop->nodenum;
	orig_nodenum = loopStackTop->orig_nodenum;
	(*loopStackTopArg) = ZL_NULL;

	chnum = nodes[orig_nodenum].childs.childnum; //获取子节点的索引数组
	if(nodenum == chnum[0])
	{
		nodenum = orig_nodenum;
		goto second_finish;
	}
	else if(nodenum == -1)
	{
		zengl_AsmGenCode_Assign_FirstChildNode(VM_ARG, nodes, -1, loopStackTop);
		goto end;
	}
	
	if(nodes[nodenum].childs.count != 2) 
	{
		compile->parser_curnode = nodenum;
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_ASSIGN_MUST_HAVE_TWO_CHILD);
	}
	
	loopStackTop->state = state;
	switch(nodes[chnum[1]].toktype) //如果第二个子节点为ID变量标示符，就输出MOV AX (变量内存地址) ， 索引0表示第一个子节点，1则是第二个子节点，将该变量内存空间的值赋给AX寄存器
	{
	case ZL_TK_ID:
		inst_op_data = compile->SymLookupID(VM_ARG,chnum[1]);
		run->AddInst(VM_ARG,compile->gencode_struct.pc++,chnum[1],
						ZL_R_IT_MOV,ZL_R_DT_REG,ZL_R_RT_AX,
						inst_op_data.type,inst_op_data.val.mem); //对应汇编指令 类似 "MOV AX (%d)"
		break;
	case ZL_TK_NUM:
		inst_op_data.val.num = ZENGL_SYS_STR_TO_LONG_NUM(compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[1]].strindex));
		run->AddInst(VM_ARG,compile->gencode_struct.pc++,chnum[1],
						ZL_R_IT_MOV,ZL_R_DT_REG,ZL_R_RT_AX,
						ZL_R_DT_NUM,inst_op_data.val.num); //对应汇编指令 类似 "MOV AX 123"
		break;
	case ZL_TK_FLOAT:
		inst_op_data.val.floatnum = ZENGL_SYS_STR_TO_FLOAT(compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[1]].strindex));
		run->AddInst(VM_ARG,compile->gencode_struct.pc++,chnum[1],
						ZL_R_IT_MOV,ZL_R_DT_REG,ZL_R_RT_AX,
						ZL_R_DT_FLOAT,inst_op_data.val.floatnum); //对应汇编指令 类似 "MOV AX 3.1415926"
		break;
	case ZL_TK_STR:
		inst_op_data.val.num = (ZL_LONG)compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[1]].strindex);
		run->AddInst(VM_ARG,compile->gencode_struct.pc++,chnum[1],
						ZL_R_IT_MOV,ZL_R_DT_REG,ZL_R_RT_AX,
						ZL_R_DT_STR,inst_op_data.val.num); //对应汇编指令 类似 [MOV AX "hello world"]
		break;
	default:
		if(ZENGL_AST_ISTOKEXPRESS(chnum[1]))
		{
			zengl_AsmGCLoopStackPush(VM_ARG, chnum[1], ZL_ST_START); // 将chnum[1]压入栈，返回后会对chnum[1]对应的节点执行生成汇编指令的操作
			loopStackTop->nodenum = chnum[0]; // 将当前循环模拟栈的nodenum设置为chnum[0]，下次进入本函数时，就会对该节点进行处理
			return ZL_ST_START;
second_finish:
			;
		}
		else
		{
			compile->parser_curnode = chnum[1];
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_ASSIGN_SECOND_NODE_IS_INVALID_TYPE);
		}
		break;
	} //switch(nodes[chnum[1]].toktype)
	
	state = zengl_AsmGenCode_Assign_FirstChildNode(VM_ARG, nodes, nodenum, loopStackTop);
	if(state == ZL_ST_START)
		return state;
end:
	return zengl_AsmGCLoopStackFinishTop(VM_ARG, orig_nodenum);
}

#define AGC_FOR_TWO_FINISH_SECOND_CHILD -2

// 生成大多数双目运算符，如：按位运算符，逻辑运算符，比较运算符，加减乘除取余等运算符的汇编指令
static ZENGL_STATES zengl_AsmGenCode_ForTwo(ZL_VOID * VM_ARG, ZENGL_STATES state, ZENGL_ASM_LOOP_STACK_TYPE ** loopStackTopArg)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_ASM_LOOP_STACK_TYPE * loopStackTop = (*loopStackTopArg);
	ZL_INT nodenum;
	ZL_INT orig_nodenum;
	ZL_INT * chnum = ZL_NULL;
	ZENGL_RUN_INST_OP_DATA inst_op_data;
	if(loopStackTop == ZL_NULL) {
		loopStackTop = & compile->AsmGCLoopStackList.stacks[compile->AsmGCLoopStackList.count - 1];
	}
	nodenum = loopStackTop->nodenum;
	orig_nodenum = loopStackTop->orig_nodenum;
	(*loopStackTopArg) = ZL_NULL;

	chnum = nodes[orig_nodenum].childs.childnum; //获取子节点的索引数组
	if(nodenum == chnum[1])
	{
		nodenum = orig_nodenum;
		goto first_finish;
	}
	else if(nodenum == AGC_FOR_TWO_FINISH_SECOND_CHILD)
	{
		nodenum = orig_nodenum;
		goto second_finish;
	}
	else if(nodenum == -1) {
		nodenum = orig_nodenum;
		zengl_AsmGenCode_Assign_FirstChildNode(VM_ARG, nodes, -1, loopStackTop);
		goto end;
	}

	if(nodes[nodenum].childs.count != 2) 
	{
		compile->parser_curnode = nodenum;
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_TWO_CHILDS);
	}

	loopStackTop->state = state;
	if(nodes[nodenum].toktype == ZL_TK_AND) //AND运算符的子节点中可能还有AND，即逻辑与内部会嵌入别的逻辑与运算，所以也需要压栈
		compile->AsmGCStackPush(VM_ARG,compile->AsmGCAddrNum++,ZL_ASM_STACK_ENUM_AND_ADDR);
	else if(nodes[nodenum].toktype == ZL_TK_OR) //OR运算符的子节点中可能还有OR，即逻辑或内部会嵌入别的逻辑或运算，所以也需要压栈
		compile->AsmGCStackPush(VM_ARG,compile->AsmGCAddrNum++,ZL_ASM_STACK_ENUM_OR_ADDR);

	switch(nodes[chnum[0]].toktype) //以下为对第一个子节点的判断并输出相应的汇编指令，结果储存在AX寄存器中
	{
	case ZL_TK_ID:
		inst_op_data = compile->SymLookupID(VM_ARG,chnum[0]);
		run->AddInst(VM_ARG,compile->gencode_struct.pc++,chnum[0],
						ZL_R_IT_MOV,ZL_R_DT_REG,ZL_R_RT_AX,
						inst_op_data.type,inst_op_data.val.mem); //对应汇编指令 类似 "MOV AX (%d)"
		break;
	case ZL_TK_NUM:
		inst_op_data.val.num = ZENGL_SYS_STR_TO_LONG_NUM(compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[0]].strindex));
		run->AddInst(VM_ARG,compile->gencode_struct.pc++,chnum[0],
						ZL_R_IT_MOV,ZL_R_DT_REG,ZL_R_RT_AX,
						ZL_R_DT_NUM,inst_op_data.val.num); //对应汇编指令 类似 "MOV AX 123"
		break;
	case ZL_TK_FLOAT:
		inst_op_data.val.floatnum = ZENGL_SYS_STR_TO_FLOAT(compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[0]].strindex));
		run->AddInst(VM_ARG,compile->gencode_struct.pc++,chnum[0],
						ZL_R_IT_MOV,ZL_R_DT_REG,ZL_R_RT_AX,
						ZL_R_DT_FLOAT,inst_op_data.val.floatnum); //对应汇编指令 类似 "MOV AX 3.1415926"
		break;
	case ZL_TK_STR:
		inst_op_data.val.num = (ZL_LONG)compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[0]].strindex);
		run->AddInst(VM_ARG,compile->gencode_struct.pc++,chnum[0],
						ZL_R_IT_MOV,ZL_R_DT_REG,ZL_R_RT_AX,
						ZL_R_DT_STR,inst_op_data.val.num); //对应汇编指令 类似 [MOV AX "hello world"]
		break;
	default:
		if(ZENGL_AST_ISTOKEXPRESS(chnum[0]))
		{
			zengl_AsmGCLoopStackPush(VM_ARG, chnum[0], ZL_ST_START); // 将chnum[0]压入栈，返回后会对chnum[0]对应的节点执行生成汇编指令的操作
			loopStackTop->nodenum = chnum[1]; // 将当前循环模拟栈的nodenum设置为chnum[1]，下次进入本函数时，就会对该节点进行处理
			return ZL_ST_START;
first_finish:
			;
		}
		else
		{
			compile->parser_curnode = nodenum;
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_INVALID_CHILD_NODE_TYPE,
				compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[0]].strindex),
				nodes[chnum[0]].line_no,
				nodes[chnum[0]].col_no,
				nodes[chnum[0]].filename);
		}
		break;
	} //switch(nodes[chnum[0]].toktype)

	if(nodes[nodenum].toktype == ZL_TK_AND)
	{
		run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
			ZL_R_IT_JE,ZL_R_DT_NONE,0,
			ZL_R_DT_LDADDR,compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_AND_ADDR,ZL_FALSE)); //对应汇编指令 "JE adr%d" 其中%d为整数类型的伪地址值
	}
	else if(nodes[nodenum].toktype == ZL_TK_OR)
	{
		run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
			ZL_R_IT_JNE,ZL_R_DT_NONE,0,
			ZL_R_DT_LDADDR,compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_OR_ADDR,ZL_FALSE)); //对应汇编指令 "JNE adr%d" 其中%d为整数类型的伪地址值
	}

	switch(nodes[chnum[1]].toktype) //以下为对第二个子节点的判断并输出相应的汇编代码，结果储存在BX寄存器中
	{
	case ZL_TK_ID:
		inst_op_data = compile->SymLookupID(VM_ARG,chnum[1]);
		run->AddInst(VM_ARG,compile->gencode_struct.pc++,chnum[1],
						ZL_R_IT_MOV,ZL_R_DT_REG,ZL_R_RT_BX,
						inst_op_data.type,inst_op_data.val.mem); //对应汇编指令 类似 "MOV BX (%d)"
		break;
	case ZL_TK_NUM:
		inst_op_data.val.num = ZENGL_SYS_STR_TO_LONG_NUM(compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[1]].strindex));
		run->AddInst(VM_ARG,compile->gencode_struct.pc++,chnum[1],
						ZL_R_IT_MOV,ZL_R_DT_REG,ZL_R_RT_BX,
						ZL_R_DT_NUM,inst_op_data.val.num); //对应汇编指令 类似 "MOV BX 123"
		break;
	case ZL_TK_FLOAT:
		inst_op_data.val.floatnum = ZENGL_SYS_STR_TO_FLOAT(compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[1]].strindex));
		run->AddInst(VM_ARG,compile->gencode_struct.pc++,chnum[1],
						ZL_R_IT_MOV,ZL_R_DT_REG,ZL_R_RT_BX,
						ZL_R_DT_FLOAT,inst_op_data.val.floatnum); //对应汇编指令 类似 "MOV BX 3.1415926"
		break;
	case ZL_TK_STR:
		inst_op_data.val.num = (ZL_LONG)compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[1]].strindex);
		run->AddInst(VM_ARG,compile->gencode_struct.pc++,chnum[1],
						ZL_R_IT_MOV,ZL_R_DT_REG,ZL_R_RT_BX,
						ZL_R_DT_STR,inst_op_data.val.num); //对应汇编指令 类似 [MOV BX "hello world"]
		break;
	default:
		if(ZENGL_AST_ISTOKEXPRESS(chnum[1]))
		{
			run->AddInst(VM_ARG, compile->gencode_struct.pc++, chnum[1],
					ZL_R_IT_PUSH , ZL_R_DT_NONE , 0,
					ZL_R_DT_REG , ZL_R_RT_AX); //对应汇编指令 "PUSH AX" 先将前面的AX寄存器里的值压入栈，下面第二个节点的汇编指令会将新值覆盖到AX中
			zengl_AsmGCLoopStackPush(VM_ARG, chnum[1], ZL_ST_START); // 将chnum[1]压入栈，返回后会对chnum[1]对应的节点执行生成汇编指令的操作
			loopStackTop->nodenum = AGC_FOR_TWO_FINISH_SECOND_CHILD; // 将当前循环模拟栈的nodenum设置为-2，下次再进入本函数时，就表示第二个子节点处理完
			return ZL_ST_START;
second_finish:
			run->AddInst(VM_ARG, compile->gencode_struct.pc++, chnum[1],
					ZL_R_IT_MOV , ZL_R_DT_REG , ZL_R_RT_BX,
					ZL_R_DT_REG , ZL_R_RT_AX); //对应汇编指令 "MOV BX AX" 将AX里存放的表达式的结果转存到BX寄存器中。
			run->AddInst(VM_ARG, compile->gencode_struct.pc++, chnum[1],
					ZL_R_IT_POP , ZL_R_DT_NONE , 0,
					ZL_R_DT_REG , ZL_R_RT_AX); //对应汇编指令 "POP AX" 从虚拟栈中弹出AX值，下面就会利用AX和BX的值进行加减乘除运算。
		}
		else
		{
			compile->parser_curnode = nodenum;
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_INVALID_CHILD_NODE_TYPE,
				compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[1]].strindex),
				nodes[chnum[1]].line_no,
				nodes[chnum[1]].col_no,
				nodes[chnum[1]].filename);
		}
		break;
	} // switch(nodes[chnum[1]].toktype)

	switch(nodes[nodenum].toktype) // 根据运算符节点的类型输出不同的汇编指令
	{
	case ZL_TK_PLUS_ASSIGN:
	case ZL_TK_PLUS:
		run->AddInst(VM_ARG, compile->gencode_struct.pc++, nodenum,
				ZL_R_IT_PLUS , ZL_R_DT_NONE , 0,
				ZL_R_DT_NONE , 0); //对应汇编指令 "PLUS" 加法汇编码，PLUS指令会将AX 和 BX寄存器的值相加，结果存放在AX中
		if(nodes[nodenum].toktype == ZL_TK_PLUS_ASSIGN) //如果是+=运算符，在输出加法汇编后，再跳到assign生成赋值语句的汇编
		{
			state = zengl_AsmGenCode_Assign_FirstChildNode(VM_ARG, nodes, nodenum, loopStackTop);
			if(state == ZL_ST_START)
				return state;
		}
		break;
	case ZL_TK_MINIS_ASSIGN:
	case ZL_TK_MINIS:
		run->AddInst(VM_ARG, compile->gencode_struct.pc++, nodenum,
				ZL_R_IT_MINIS , ZL_R_DT_NONE , 0,
				ZL_R_DT_NONE , 0); //对应汇编指令 "MINIS" 减法汇编码，MINIS指令会将AX 和 BX寄存器的值相减，结果存放在AX中
		if(nodes[nodenum].toktype == ZL_TK_MINIS_ASSIGN)
		{
			state = zengl_AsmGenCode_Assign_FirstChildNode(VM_ARG, nodes, nodenum, loopStackTop);
			if(state == ZL_ST_START)
				return state;
		}
		break;
	case ZL_TK_MOD_ASSIGN:
	case ZL_TK_MOD: //取余运算符。输出MOD汇编指令
		run->AddInst(VM_ARG, compile->gencode_struct.pc++, nodenum,
				ZL_R_IT_MOD , ZL_R_DT_NONE , 0,
				ZL_R_DT_NONE , 0); //对应汇编指令 "MOD" 取余汇编码
		if(nodes[nodenum].toktype == ZL_TK_MOD_ASSIGN)
		{
			state = zengl_AsmGenCode_Assign_FirstChildNode(VM_ARG, nodes, nodenum, loopStackTop);
			if(state == ZL_ST_START)
				return state;
		}
		break;
	case ZL_TK_TIMES_ASSIGN:
	case ZL_TK_TIMES:
		run->AddInst(VM_ARG, compile->gencode_struct.pc++, nodenum,
				ZL_R_IT_TIMES , ZL_R_DT_NONE , 0,
				ZL_R_DT_NONE , 0); //对应汇编指令 "TIMES" 乘法汇编码 TIMES指令会将AX 和 BX寄存器的值相乘，结果存放在AX中
		if(nodes[nodenum].toktype == ZL_TK_TIMES_ASSIGN)
		{
			state = zengl_AsmGenCode_Assign_FirstChildNode(VM_ARG, nodes, nodenum, loopStackTop);
			if(state == ZL_ST_START)
				return state;
		}
		break;
	case ZL_TK_DIVIDE_ASSIGN:
	case ZL_TK_DIVIDE:
		run->AddInst(VM_ARG, compile->gencode_struct.pc++, nodenum,
				ZL_R_IT_DIVIDE , ZL_R_DT_NONE , 0,
				ZL_R_DT_NONE , 0); //对应汇编指令 "DIVIDE" 除法汇编码 DIVIDE指令会将AX 和 BX寄存器的值相除，结果存放在AX中
		if(nodes[nodenum].toktype == ZL_TK_DIVIDE_ASSIGN)
		{
			state = zengl_AsmGenCode_Assign_FirstChildNode(VM_ARG, nodes, nodenum, loopStackTop);
			if(state == ZL_ST_START)
				return state;
		}
		break;
	case ZL_TK_BIT_AND_ASSIGN:
	case ZL_TK_BIT_AND:
		run->AddInst(VM_ARG, compile->gencode_struct.pc++, nodenum,
				ZL_R_IT_BIT_AND , ZL_R_DT_NONE , 0,
				ZL_R_DT_NONE , 0); //对应汇编指令 "BIT_AND" 按位与汇编码 BIT_AND指令会将AX 和 BX寄存器的值进行按位与运算，结果存放在AX中
		if(nodes[nodenum].toktype == ZL_TK_BIT_AND_ASSIGN)
		{
			state = zengl_AsmGenCode_Assign_FirstChildNode(VM_ARG, nodes, nodenum, loopStackTop);
			if(state == ZL_ST_START)
				return state;
		}
		break;
	case ZL_TK_BIT_OR_ASSIGN:
	case ZL_TK_BIT_OR:
		run->AddInst(VM_ARG, compile->gencode_struct.pc++, nodenum,
				ZL_R_IT_BIT_OR , ZL_R_DT_NONE , 0,
				ZL_R_DT_NONE , 0); //对应汇编指令 "BIT_OR" 按位或汇编码 BIT_OR指令会将AX 和 BX寄存器的值进行按位或运算，结果存放在AX中
		if(nodes[nodenum].toktype == ZL_TK_BIT_OR_ASSIGN)
		{
			state = zengl_AsmGenCode_Assign_FirstChildNode(VM_ARG, nodes, nodenum, loopStackTop);
			if(state == ZL_ST_START)
				return state;
		}
		break;
	case ZL_TK_BIT_XOR_ASSIGN:
	case ZL_TK_BIT_XOR:
		run->AddInst(VM_ARG, compile->gencode_struct.pc++, nodenum,
				ZL_R_IT_BIT_XOR , ZL_R_DT_NONE , 0,
				ZL_R_DT_NONE , 0); //对应汇编指令 "BIT_XOR" 按位异或汇编码 BIT_XOR指令会将AX 和 BX寄存器的值进行按位异或运算，结果存放在AX中
		if(nodes[nodenum].toktype == ZL_TK_BIT_XOR_ASSIGN)
		{
			state = zengl_AsmGenCode_Assign_FirstChildNode(VM_ARG, nodes, nodenum, loopStackTop);
			if(state == ZL_ST_START)
				return state;
		}
		break;
	case ZL_TK_BIT_RIGHT_ASSIGN:
	case ZL_TK_BIT_RIGHT:
		run->AddInst(VM_ARG, compile->gencode_struct.pc++, nodenum,
				ZL_R_IT_BIT_RIGHT , ZL_R_DT_NONE , 0,
				ZL_R_DT_NONE , 0); //对应汇编指令 "BIT_RIGHT" 右移汇编码 BIT_RIGHT指令会将AX里的值根据BX里的值进行>>右移运算，结果存放在AX中
		if(nodes[nodenum].toktype == ZL_TK_BIT_RIGHT_ASSIGN)
		{
			state = zengl_AsmGenCode_Assign_FirstChildNode(VM_ARG, nodes, nodenum, loopStackTop);
			if(state == ZL_ST_START)
				return state;
		}
		break;
	case ZL_TK_BIT_LEFT_ASSIGN:
	case ZL_TK_BIT_LEFT:
		run->AddInst(VM_ARG, compile->gencode_struct.pc++, nodenum,
				ZL_R_IT_BIT_LEFT , ZL_R_DT_NONE , 0,
				ZL_R_DT_NONE , 0); //对应汇编指令 "BIT_LEFT" 左移汇编码 BIT_LEFT指令会将AX里的值根据BX里的值进行<<左移运算，结果存放在AX中
		if(nodes[nodenum].toktype == ZL_TK_BIT_LEFT_ASSIGN)
		{
			state = zengl_AsmGenCode_Assign_FirstChildNode(VM_ARG, nodes, nodenum, loopStackTop);
			if(state == ZL_ST_START)
				return state;
		}
		break;
	case ZL_TK_GREAT:
		run->AddInst(VM_ARG, compile->gencode_struct.pc++, nodenum,
				ZL_R_IT_GREAT , ZL_R_DT_NONE , 0,
				ZL_R_DT_NONE , 0); //对应汇编指令 "GREAT" 大于运算符汇编码，GREAT指令会将AX和BX进行大小比较，结果通常以0或1的值存放在AX中。0表示FALSE，1表示TRUE
		break;
	case ZL_TK_LESS:
		run->AddInst(VM_ARG, compile->gencode_struct.pc++, nodenum,
				ZL_R_IT_LESS , ZL_R_DT_NONE , 0,
				ZL_R_DT_NONE , 0); //对应汇编指令 "LESS" 小于运算符汇编码，LESS指令会将AX和BX进行大小比较，结果通常以0或1的值存放在AX中。
		break;
	case ZL_TK_EQUAL:
		run->AddInst(VM_ARG, compile->gencode_struct.pc++, nodenum,
				ZL_R_IT_EQUAL , ZL_R_DT_NONE , 0,
				ZL_R_DT_NONE , 0); //对应汇编指令 "EQUAL" 等于运算符汇编码
		break;
	case ZL_TK_NOT_EQ:
		run->AddInst(VM_ARG, compile->gencode_struct.pc++, nodenum,
				ZL_R_IT_NOT_EQ , ZL_R_DT_NONE , 0,
				ZL_R_DT_NONE , 0); //对应汇编指令 "NOT_EQ" 不等于运算符汇编码
		break;
	case ZL_TK_GREAT_EQ:
		run->AddInst(VM_ARG, compile->gencode_struct.pc++, nodenum,
				ZL_R_IT_GREAT_EQ , ZL_R_DT_NONE , 0,
				ZL_R_DT_NONE , 0); //对应汇编指令 "GREAT_EQ" 大于等于汇编码
		break;
	case ZL_TK_LESS_EQ:
		run->AddInst(VM_ARG, compile->gencode_struct.pc++, nodenum,
				ZL_R_IT_LESS_EQ , ZL_R_DT_NONE , 0,
				ZL_R_DT_NONE , 0); //对应汇编指令 "LESS_EQ" 小于等于汇编码
		break;
	case ZL_TK_AND:
		run->AddInst(VM_ARG, compile->gencode_struct.pc++, nodenum,
				ZL_R_IT_AND , ZL_R_DT_NONE , 0,
				ZL_R_DT_NONE , 0); //对应汇编指令 "AND" 与运算符汇编码，与运算一般用于条件判断，相等于PHP和C的&&运算符
		break;
	case ZL_TK_OR:
		run->AddInst(VM_ARG, compile->gencode_struct.pc++, nodenum,
				ZL_R_IT_OR , ZL_R_DT_NONE , 0,
				ZL_R_DT_NONE , 0); //对应汇编指令 "OR" 或运算符汇编码，相当于PHP和C的||运算符
		break;
	} //switch(nodes[nodenum].toktype) //根据运算符节点的类型输出不同的汇编指令
	if(nodes[nodenum].toktype == ZL_TK_AND) //设置and逻辑与的跳转汇编地址，并将ZL_ASM_STACK_ENUM_AND_ADDR从栈中弹出
	{
		compile->LDAddrListSet(VM_ARG,compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_AND_ADDR,ZL_TRUE),compile->gencode_struct.pc);
	}
	else if(nodes[nodenum].toktype == ZL_TK_OR) //设置or逻辑或的跳转汇编地址，并将ZL_ASM_STACK_ENUM_OR_ADDR从栈中弹出
	{
		compile->LDAddrListSet(VM_ARG,compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_OR_ADDR,ZL_TRUE),compile->gencode_struct.pc);
	}

end:
	return zengl_AsmGCLoopStackFinishTop(VM_ARG, orig_nodenum);
}

// 生成负号运算符的汇编指令
static ZENGL_STATES zengl_AsmGenCode_Negative(ZL_VOID * VM_ARG, ZENGL_STATES state, ZENGL_ASM_LOOP_STACK_TYPE ** loopStackTopArg)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_ASM_LOOP_STACK_TYPE * loopStackTop = (*loopStackTopArg);
	ZL_INT nodenum;
	ZL_INT orig_nodenum;
	ZL_INT * chnum;
	ZENGL_RUN_INST_OP_DATA inst_op_data;
	if(loopStackTop == ZL_NULL) {
		loopStackTop = & compile->AsmGCLoopStackList.stacks[compile->AsmGCLoopStackList.count - 1];
	}
	nodenum = loopStackTop->nodenum;
	orig_nodenum = loopStackTop->orig_nodenum;
	(*loopStackTopArg) = ZL_NULL;

	chnum = nodes[orig_nodenum].childs.childnum; //获取子节点的索引数组
	if(nodenum == -1){
		nodenum = orig_nodenum;
		goto finish;
	}

	loopStackTop->state = state;
	if(nodes[nodenum].childs.count == 1)
	{
		run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
					ZL_R_IT_MOV,ZL_R_DT_REG,ZL_R_RT_AX,
					ZL_R_DT_NUM,0); //对应汇编指令 "MOV AX 0" 将0作为AX，这样就是0减另一个数，结果就是该数的负数了。
		switch(nodes[chnum[0]].toktype) //以下为对第一个子节点的判断并输出相应的汇编代码，结果储存在BX寄存器中。
		{
		case ZL_TK_ID:
			inst_op_data = compile->SymLookupID(VM_ARG,chnum[0]);
			run->AddInst(VM_ARG,compile->gencode_struct.pc++,chnum[0],
							ZL_R_IT_MOV,ZL_R_DT_REG,ZL_R_RT_BX,
							inst_op_data.type,inst_op_data.val.mem); //对应汇编指令 类似 "MOV BX (%d)"
			break;
		case ZL_TK_NUM:
			inst_op_data.val.num = ZENGL_SYS_STR_TO_LONG_NUM(compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[0]].strindex));
			run->AddInst(VM_ARG,compile->gencode_struct.pc++,chnum[0],
							ZL_R_IT_MOV,ZL_R_DT_REG,ZL_R_RT_BX,
							ZL_R_DT_NUM,inst_op_data.val.num); //对应汇编指令 类似 "MOV BX 123"
			break;
		case ZL_TK_FLOAT:
			inst_op_data.val.floatnum = ZENGL_SYS_STR_TO_FLOAT(compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[0]].strindex));
			run->AddInst(VM_ARG,compile->gencode_struct.pc++,chnum[0],
							ZL_R_IT_MOV,ZL_R_DT_REG,ZL_R_RT_BX,
							ZL_R_DT_FLOAT,inst_op_data.val.floatnum); //对应汇编指令 类似 "MOV BX 3.1415926"
			break;
		case ZL_TK_STR:
			inst_op_data.val.num = (ZL_LONG)compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[0]].strindex);
			run->AddInst(VM_ARG,compile->gencode_struct.pc++,chnum[0],
							ZL_R_IT_MOV,ZL_R_DT_REG,ZL_R_RT_BX,
							ZL_R_DT_STR,inst_op_data.val.num); //对应汇编指令 类似 [MOV BX "hello world"]
			break;
		default:
			if(ZENGL_AST_ISTOKEXPRESS(chnum[0]))
			{
				run->AddInst(VM_ARG,compile->gencode_struct.pc++,chnum[0],
						ZL_R_IT_PUSH , ZL_R_DT_NONE , 0,
						ZL_R_DT_REG , ZL_R_RT_AX); //对应汇编指令 "PUSH AX"
				//compile->AsmGenCodes(VM_ARG,chnum[0]);
				zengl_AsmGCLoopStackPush(VM_ARG, chnum[0], ZL_ST_START); // 将chnum[0]压入栈，返回后会对chnum[0]对应的节点执行生成汇编指令的操作
				loopStackTop->nodenum = -1;
				return ZL_ST_START;
finish:
				run->AddInst(VM_ARG,compile->gencode_struct.pc++,chnum[0],
						ZL_R_IT_MOV , ZL_R_DT_REG , ZL_R_RT_BX,
						ZL_R_DT_REG , ZL_R_RT_AX); //对应汇编指令 "MOV BX AX"
				run->AddInst(VM_ARG,compile->gencode_struct.pc++,chnum[0],
						ZL_R_IT_POP , ZL_R_DT_NONE , 0,
						ZL_R_DT_REG , ZL_R_RT_AX); //对应汇编指令 "POP AX"
			}
			else
			{
				compile->parser_curnode = nodenum;
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_INVALID_CHILD_NODE_TYPE,
					compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[0]].strindex),
					nodes[chnum[0]].line_no,
					nodes[chnum[0]].col_no,
					nodes[chnum[0]].filename);
			}
			break;
		} //switch(nodes[chnum[0]].toktype)
		run->AddInst(VM_ARG, compile->gencode_struct.pc++, nodenum,
				ZL_R_IT_MINIS , ZL_R_DT_NONE , 0,
				ZL_R_DT_NONE , 0); //对应汇编指令 "MINIS" 
		return zengl_AsmGCLoopStackFinishTop(VM_ARG, orig_nodenum);
	}
	else
	{
		compile->parser_curnode = nodenum;
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_ONE_CHILD);
	}
}

// 生成 按位取反运算符 或者 取反运算符 的汇编指令
static ZENGL_STATES zengl_AsmGenCode_Reverse(ZL_VOID * VM_ARG, ZENGL_STATES state, ZENGL_ASM_LOOP_STACK_TYPE ** loopStackTopArg)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_ASM_LOOP_STACK_TYPE * loopStackTop = (*loopStackTopArg);
	ZL_INT nodenum;
	ZL_INT orig_nodenum;
	ZL_INT * chnum;
	ZENGL_RUN_INST_OP_DATA inst_op_data;
	if(loopStackTop == ZL_NULL) {
		loopStackTop = & compile->AsmGCLoopStackList.stacks[compile->AsmGCLoopStackList.count - 1];
	}
	nodenum = loopStackTop->nodenum;
	orig_nodenum = loopStackTop->orig_nodenum;
	(*loopStackTopArg) = ZL_NULL;

	chnum = nodes[orig_nodenum].childs.childnum; //获取子节点的索引数组
	if(nodenum == -1)
	{
		nodenum = orig_nodenum;
		goto finish;
	}
	if(nodes[nodenum].childs.count == 1)
	{
		loopStackTop->state = state;
		switch(nodes[chnum[0]].toktype) //以下为对第一个子节点的判断并输出相应的汇编代码，结果储存在AX寄存器中。
		{
		case ZL_TK_ID:
			inst_op_data = compile->SymLookupID(VM_ARG,chnum[0]);
			run->AddInst(VM_ARG,compile->gencode_struct.pc++,chnum[0],
							ZL_R_IT_MOV,ZL_R_DT_REG,ZL_R_RT_AX,
							inst_op_data.type,inst_op_data.val.mem); //对应汇编指令 类似 "MOV AX (%d)"
			break;
		case ZL_TK_NUM:
			inst_op_data.val.num = ZENGL_SYS_STR_TO_LONG_NUM(compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[0]].strindex));
			run->AddInst(VM_ARG,compile->gencode_struct.pc++,chnum[0],
							ZL_R_IT_MOV,ZL_R_DT_REG,ZL_R_RT_AX,
							ZL_R_DT_NUM,inst_op_data.val.num); //对应汇编指令 类似 "MOV AX 123"
			break;
		case ZL_TK_FLOAT:
			inst_op_data.val.floatnum = ZENGL_SYS_STR_TO_FLOAT(compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[0]].strindex));
			run->AddInst(VM_ARG,compile->gencode_struct.pc++,chnum[0],
							ZL_R_IT_MOV,ZL_R_DT_REG,ZL_R_RT_AX,
							ZL_R_DT_FLOAT,inst_op_data.val.floatnum); //对应汇编指令 类似 "MOV AX 3.1415926"
			break;
		case ZL_TK_STR:
			inst_op_data.val.num = (ZL_LONG)compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[0]].strindex);
			run->AddInst(VM_ARG,compile->gencode_struct.pc++,chnum[0],
							ZL_R_IT_MOV,ZL_R_DT_REG,ZL_R_RT_AX,
							ZL_R_DT_STR,inst_op_data.val.num); //对应汇编指令 类似 [MOV AX "hello world"]
			break;
		default:
			if(ZENGL_AST_ISTOKEXPRESS(chnum[0])) 
			{
				//compile->AsmGenCodes(VM_ARG,chnum[0]);
				zengl_AsmGCLoopStackPush(VM_ARG, chnum[0], ZL_ST_START); // 将chnum[0]压入栈，返回后会对chnum[0]对应的节点执行生成汇编指令的操作
				loopStackTop->nodenum = -1;
				return ZL_ST_START;
finish:
				;
			}
			else
			{
				compile->parser_curnode = nodenum;
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_INVALID_CHILD_NODE_TYPE,
					compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[0]].strindex),
					nodes[chnum[0]].line_no,
					nodes[chnum[0]].col_no,
					nodes[chnum[0]].filename);
			}
			break;
		} //switch(nodes[chnum[0]].toktype)
		if(state == ZL_ST_ASM_CODE_INREVERSE)
		{
			run->AddInst(VM_ARG,compile->gencode_struct.pc++,chnum[0],
							ZL_R_IT_REVERSE,ZL_R_DT_NONE , 0,
							ZL_R_DT_NONE , 0); //对应汇编指令 "REVERSE" 取反运算符的汇编码，会将AX里的值取反，结果存放于AX中。
		}
		else
		{
			run->AddInst(VM_ARG,compile->gencode_struct.pc++,chnum[0],
							ZL_R_IT_BIT_REVERSE,ZL_R_DT_NONE , 0,
							ZL_R_DT_NONE , 0); //对应汇编指令 "BIT_REVERSE" 按位取反运算符的汇编码，会将AX里的值进行按位取反，结果存放于AX中。
		}
		//state = ZL_ST_DOWN; 
		return zengl_AsmGCLoopStackFinishTop(VM_ARG, orig_nodenum);
	}
	else
	{
		compile->parser_curnode = nodenum;
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_ONE_CHILD);
	}
}

// 生成 引用运算符的处理 的汇编指令
static ZENGL_STATES zengl_AsmGenCode_Address(ZL_VOID * VM_ARG, ZENGL_STATES state, ZENGL_ASM_LOOP_STACK_TYPE ** loopStackTopArg)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_ASM_LOOP_STACK_TYPE * loopStackTop = (*loopStackTopArg);
	ZL_INT nodenum;
	ZL_INT orig_nodenum;
	ZL_INT * chnum;
	ZENGL_RUN_INST_OP_DATA inst_op_data;
	if(loopStackTop == ZL_NULL) {
		loopStackTop = & compile->AsmGCLoopStackList.stacks[compile->AsmGCLoopStackList.count - 1];
	}
	nodenum = loopStackTop->nodenum;
	orig_nodenum = loopStackTop->orig_nodenum;
	(*loopStackTopArg) = ZL_NULL;

	chnum = nodes[orig_nodenum].childs.childnum; //获取子节点的索引数组
	if(nodenum == -1)
	{
		nodenum = orig_nodenum;
		goto finish;
	}
	if(nodes[nodenum].childs.count == 1)
	{
		//chnum = nodes[nodenum].childs.childnum;
		loopStackTop->state = state;
		switch(nodes[chnum[0]].toktype)
		{
		case ZL_TK_ID:
			inst_op_data = compile->SymLookupID(VM_ARG,chnum[0]);
			run->AddInst(VM_ARG,compile->gencode_struct.pc++,chnum[0],
						ZL_R_IT_ADDR ,ZL_R_DT_NONE , 0,
						inst_op_data.type,inst_op_data.val.mem); //对应汇编指令 类似 "ADDR (%d)"
			break;
		case ZL_TK_ARRAY_ITEM:
		case ZL_TK_DOT:
			compile->AsmGCStackPush(VM_ARG,(ZL_INT)ZL_ASM_AI_OP_IN_ADDR,ZL_ASM_STACK_ENUM_ARRAY_ITEM_OP_TYPE);
			//compile->AsmGenCodes(VM_ARG,chnum[0]);
			zengl_AsmGCLoopStackPush(VM_ARG, chnum[0], ZL_ST_START); // 将chnum[0]压入栈，返回后会对chnum[0]对应的节点执行生成汇编指令的操作
			loopStackTop->nodenum = -1;
			return ZL_ST_START;
finish:
			compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_ARRAY_ITEM_OP_TYPE,ZL_TRUE);
			break;
		default:
			compile->parser_curnode = nodenum;
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_INVALID_CHILD_NODE_TYPE,
				compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[0]].strindex),
				nodes[chnum[0]].line_no,
				nodes[chnum[0]].col_no,
				nodes[chnum[0]].filename);
			break;
		}
		//state = ZL_ST_DOWN;
		return zengl_AsmGCLoopStackFinishTop(VM_ARG, orig_nodenum);
	}
	else
	{
		compile->parser_curnode = nodenum;
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_ONE_CHILD);
	}
}

typedef enum _ZENGL_ASMGC_IF_STATUS{
	ZL_ASMGC_IF_STATUS_START,
	ZL_ASMGC_IF_STATUS_FINISH_FIRST_EXPRESS,
	ZL_ASMGC_IF_STATUS_LOOP_IN_FIRST_BLOCK,
	ZL_ASMGC_IF_STATUS_FINISH_ELIF,
	ZL_ASMGC_ELSE_STATUS_LOOP_IN_BLOCK
} ZENGL_ASMGC_IF_STATUS;

typedef enum _ZENGL_ASMGC_ELIF_STATUS{
	ZL_ASMGC_ELIF_STATUS_START,
	ZL_ASMGC_ELIF_STATUS_FINISH_EXPRESS,
	ZL_ASMGC_ELIF_STATUS_LOOP_IN_BLOCK
} ZENGL_ASMGC_ELIF_STATUS;

typedef struct _ZENGL_ASMGC_IF_STACK_VAL{
	ZL_INT LastNodeNum;
	ZL_INT i;
	ZL_INT tmpch;
	ZENGL_ASMGC_IF_STATUS status;
} ZENGL_ASMGC_IF_STACK_VAL;

typedef struct _ZENGL_ASMGC_ELIF_STACK_VAL{
	ZL_INT * chnum;
	ZENGL_AST_CHILD_NODE_TYPE * ifchnum;
	ZL_INT nodenum;
	ZL_INT num;
	ZL_INT lastNodeNum;
	ZENGL_ASMGC_ELIF_STATUS status;
} ZENGL_ASMGC_ELIF_STACK_VAL;

//zengl_AsmGCElif函数用于生成elif代码块对应的汇编指令
static ZENGL_STATES zengl_AsmGCElif(ZL_VOID * VM_ARG,ZENGL_AST_CHILD_NODE_TYPE * ifchnum,ZL_INT num, ZENGL_ASM_LOOP_STACK_TYPE ** loopStackTopArg)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_ASM_LOOP_STACK_TYPE * loopStackTop = (*loopStackTopArg);
	ZENGL_ASMGC_ELIF_STACK_VAL * stackVal;
	if(loopStackTop != ZL_NULL) // 不为NULL，说明是从if进来的，就将自己压入模拟栈，下次循环时，就直接进入当前函数
	{
		ZL_INT nodenum;
		ZENGL_ASM_LOOP_STACK_TYPE * if_loopStackTop;
		ZENGL_ASMGC_IF_STACK_VAL * if_loopStackTop_stackVal;
		if(num < ZL_AST_CHILD_NODE_SIZE)
			nodenum = ifchnum->childnum[num];
		else
			nodenum = ifchnum->extchilds[num - ZL_AST_CHILD_NODE_SIZE];
		zengl_AsmGCLoopStackPush(VM_ARG, nodenum, ZL_ST_ASM_CODE_INELIF);
		if_loopStackTop = loopStackTop;
		loopStackTop = & compile->AsmGCLoopStackList.stacks[compile->AsmGCLoopStackList.count - 1];
		stackVal = (ZENGL_ASMGC_ELIF_STACK_VAL *)zengl_AsmGCLoopStackValsPush(VM_ARG, loopStackTop, sizeof(ZENGL_ASMGC_ELIF_STACK_VAL));
		stackVal->ifchnum = ifchnum;
		stackVal->num = num;
		stackVal->nodenum = nodenum;
		if_loopStackTop_stackVal = (ZENGL_ASMGC_IF_STACK_VAL *)(& compile->AsmGCLoopStackList.stackVals[if_loopStackTop->stackValIndex]);
		if_loopStackTop_stackVal->status = ZL_ASMGC_IF_STATUS_FINISH_ELIF;
		stackVal->status = ZL_ASMGC_ELIF_STATUS_START;
	}
	else // 否则就是直接从汇编输出的主循环直接进来的
	{
		loopStackTop = & compile->AsmGCLoopStackList.stacks[compile->AsmGCLoopStackList.count - 1];
		stackVal = (ZENGL_ASMGC_ELIF_STACK_VAL *)(& compile->AsmGCLoopStackList.stackVals[loopStackTop->stackValIndex]);
		ifchnum = stackVal->ifchnum;
		num = stackVal->num;
	}

	//ZL_INT * chnum;
	//ZL_INT nodenum;
	//ZL_INT lastNodeNum;
	switch(stackVal->status)
	{
	case ZL_ASMGC_ELIF_STATUS_START:
		break;
	case ZL_ASMGC_ELIF_STATUS_FINISH_EXPRESS:
		goto finish_express;
	case ZL_ASMGC_ELIF_STATUS_LOOP_IN_BLOCK:
		goto loop_in_block;
	}

	if(nodes[stackVal->nodenum].childs.count == 1)  //生成elif对应的判断表达式的汇编指令
	{
		stackVal->chnum = nodes[stackVal->nodenum].childs.childnum;
		if(stackVal->chnum[0] == -1)
		{
			compile->parser_curnode = stackVal->nodenum;
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_IF_HAVE_NO_EXPRESS);
		}
		else if(nodes[stackVal->chnum[0]].tokcategory == ZL_TKCG_OP_FACTOR || ZENGL_AST_ISTOKEXPRESS(stackVal->chnum[0])) //单纯的变量，数字，字符串之类的操作因子或者表达式都可以作为if...elif的判断表达式
		{
			//compile->AsmGenCodes(VM_ARG,chnum[0]); // TODO
			zengl_AsmGCLoopStackPush(VM_ARG, stackVal->chnum[0], ZL_ST_START); // 将stackVal->chnum[0]压入栈，返回后会对stackVal->chnum[0]对应的节点执行生成汇编指令的操作
			stackVal->status = ZL_ASMGC_ELIF_STATUS_FINISH_EXPRESS; return ZL_ST_START;
finish_express:
			run->AddInst(VM_ARG,compile->gencode_struct.pc++,stackVal->nodenum,
				ZL_R_IT_JE,ZL_R_DT_NONE,0,
				ZL_R_DT_LDADDR,compile->AsmGCStackPush(VM_ARG,compile->AsmGCAddrNum++,ZL_ASM_STACK_ENUM_IF_ADDR)); //对应汇编指令 "JE adr%d"
		}
		else
		{
			compile->parser_curnode = stackVal->nodenum;
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_INVALID_CHILD_NODE_TYPE,
				compile->TokenStringPoolGetPtr(VM_ARG,nodes[stackVal->chnum[0]].strindex),
				nodes[stackVal->chnum[0]].line_no,
				nodes[stackVal->chnum[0]].col_no,
				nodes[stackVal->chnum[0]].filename);
		}
	}
	else
	{
		compile->parser_curnode = stackVal->nodenum;
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_ONE_CHILD);
	}
	if(num + 1 < ZL_AST_CHILD_NODE_SIZE)
		stackVal->nodenum = ifchnum->childnum[num + 1];
	else
		stackVal->nodenum = ifchnum->extchilds[num + 1 - ZL_AST_CHILD_NODE_SIZE];
	while(stackVal->nodenum > 0)  //循环生成elif下面的代码块的汇编指令
	{
		stackVal->lastNodeNum = stackVal->nodenum;
		//compile->AsmGenCodes(VM_ARG,nodenum); // TODO
		zengl_AsmGCLoopStackPush(VM_ARG, stackVal->nodenum, ZL_ST_START); // 将stackVal->nodenum压入栈，返回后会对stackVal->nodenum对应的节点执行生成汇编指令的操作
		stackVal->status = ZL_ASMGC_ELIF_STATUS_LOOP_IN_BLOCK; return ZL_ST_START;
loop_in_block:
		stackVal->nodenum = nodes[stackVal->nodenum].nextnode;
	}
	run->AddInst(VM_ARG,compile->gencode_struct.pc++,stackVal->lastNodeNum,
			ZL_R_IT_JMP,ZL_R_DT_NONE,0,
			ZL_R_DT_LDADDR,compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_IF_END,ZL_FALSE)); //对应汇编指令 "JMP adr%d" elif代码块执行完毕后，JMP无条件跳转到if-elif-else的结束位置
	compile->LDAddrListSet(VM_ARG,compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_IF_ADDR,ZL_TRUE),compile->gencode_struct.pc); //elif判断表达式为false时的跳转位置
	//return ZL_ST_DOWN;
	zengl_AsmGCLoopStackValsPop(VM_ARG,loopStackTop);
	return zengl_AsmGCLoopStackFinishTopSimple(VM_ARG);
}

// 生成 if-elif-else 控制语句的汇编指令
static ZENGL_STATES zengl_AsmGenCode_If_Elif_Else(ZL_VOID * VM_ARG, ZENGL_STATES state, ZENGL_ASM_LOOP_STACK_TYPE ** loopStackTopArg)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_ASM_LOOP_STACK_TYPE * loopStackTop = (*loopStackTopArg);
	ZL_INT nodenum;
	ZL_INT orig_nodenum;
	ZENGL_STATES elif_return_state;
	ZL_INT * chnum;
	ZENGL_ASMGC_IF_STACK_VAL * stackVal;
	if(loopStackTop == ZL_NULL) {
		loopStackTop = & compile->AsmGCLoopStackList.stacks[compile->AsmGCLoopStackList.count - 1];
	}
	nodenum = loopStackTop->nodenum;
	orig_nodenum = loopStackTop->orig_nodenum;
	(*loopStackTopArg) = ZL_NULL;
	if(loopStackTop->stackValIndex < 0) {
		stackVal = (ZENGL_ASMGC_IF_STACK_VAL *)zengl_AsmGCLoopStackValsPush(VM_ARG, loopStackTop, sizeof(ZENGL_ASMGC_IF_STACK_VAL));
		stackVal->status = ZL_ASMGC_IF_STATUS_START;
	}
	else
		stackVal = (ZENGL_ASMGC_IF_STACK_VAL *)(& compile->AsmGCLoopStackList.stackVals[loopStackTop->stackValIndex]);
	
	chnum = nodes[orig_nodenum].childs.childnum;
	switch(stackVal->status)
	{
	case ZL_ASMGC_IF_STATUS_START:
		loopStackTop->state = state;
		break;
	case ZL_ASMGC_IF_STATUS_FINISH_FIRST_EXPRESS:
		goto finish_first_express;
	case ZL_ASMGC_IF_STATUS_LOOP_IN_FIRST_BLOCK:
		goto loop_in_first_block;
	case ZL_ASMGC_IF_STATUS_FINISH_ELIF:
		goto finish_elif;
	case ZL_ASMGC_ELSE_STATUS_LOOP_IN_BLOCK:
		goto else_loop_in_block;
	}

	if(nodes[nodenum].childs.count >= 2)
	{
		if(chnum[0] == -1)
		{
			compile->parser_curnode = nodenum;
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_IF_HAVE_NO_EXPRESS);
		}
		else if(nodes[chnum[0]].tokcategory == ZL_TKCG_OP_FACTOR || ZENGL_AST_ISTOKEXPRESS(chnum[0])) //操作因子或表达式都可以作为判断表达式。
		{
			//compile->AsmGenCodes(VM_ARG,chnum[0]); //生成if第一个判断表达式里的汇编代码
			zengl_AsmGCLoopStackPush(VM_ARG, chnum[0], ZL_ST_START); // 将chnum[0]压入栈，返回后会对chnum[0]对应的节点执行生成汇编指令的操作
			stackVal->status = ZL_ASMGC_IF_STATUS_FINISH_FIRST_EXPRESS; return ZL_ST_START;
finish_first_express:
			run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
				ZL_R_IT_JE,ZL_R_DT_NONE,0,
				ZL_R_DT_LDADDR,compile->AsmGCStackPush(VM_ARG,compile->AsmGCAddrNum++,ZL_ASM_STACK_ENUM_IF_ADDR)); //对应汇编指令 "JE adr%d" 根据判断表达式的结果判断是跳转到elif或else或结尾处，还是继续执行
			stackVal->i = chnum[1];
			while(stackVal->i > 0)  //循环生成if第一个执行块里的所有语句的汇编代码。
			{
				stackVal->LastNodeNum = stackVal->i;
				//compile->AsmGenCodes(VM_ARG,i);
				zengl_AsmGCLoopStackPush(VM_ARG, stackVal->i, ZL_ST_START); // 将 stackVal->i 压入栈，返回后会对 stackVal->i 对应的节点执行生成汇编指令的操作
				stackVal->status = ZL_ASMGC_IF_STATUS_LOOP_IN_FIRST_BLOCK; return ZL_ST_START;
loop_in_first_block:
				stackVal->i = nodes[stackVal->i].nextnode;
			}
			run->AddInst(VM_ARG,compile->gencode_struct.pc++,stackVal->LastNodeNum,
				ZL_R_IT_JMP,ZL_R_DT_NONE,0,
				ZL_R_DT_LDADDR,compile->AsmGCStackPush(VM_ARG,compile->AsmGCAddrNum++,ZL_ASM_STACK_ENUM_IF_END)); //对应汇编指令 "JMP adr%d" 运行完执行块里的代码后，就JMP无条件跳转到if-elif-else语句的结束位置。
			compile->LDAddrListSet(VM_ARG,compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_IF_ADDR,ZL_TRUE),compile->gencode_struct.pc); //记录下IF_ADDR 这个标示符跳转地址对应的汇编指令的位置。如果第一个判断为false就跳转到这里。
		}
		else
		{
			compile->parser_curnode = nodenum;
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_INVALID_CHILD_NODE_TYPE,
				compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[0]].strindex),
				nodes[chnum[0]].line_no,
				nodes[chnum[0]].col_no,
				nodes[chnum[0]].filename);
		}
		stackVal->i = 2;
		while(stackVal->i < nodes[nodenum].childs.count)
		{
			if(stackVal->i >= ZL_AST_CHILD_NODE_SIZE) //判断是否需要查询扩展子节点，默认超过3个的子节点都存放在扩展子节点里。
			{
				stackVal->tmpch = nodes[nodenum].childs.extchilds[stackVal->i - ZL_AST_CHILD_NODE_SIZE];
			}
			else
				stackVal->tmpch = chnum[stackVal->i];
			if(nodes[stackVal->tmpch].reserve == ZL_RSV_ELIF) //如果是elif就通过AsmGCElif函数来生成elif部分的汇编指令
			{
				elif_return_state = zengl_AsmGCElif(VM_ARG,&nodes[nodenum].childs,stackVal->i,&loopStackTop);
				if(elif_return_state == ZL_ST_START)
					return elif_return_state;
finish_elif:
				stackVal->i +=2;
			}
			else if(nodes[stackVal->tmpch].reserve == ZL_RSV_ELSE) //如果是else,就循环将else下面的语句生成对应的汇编指令。
			{
				if(stackVal->i + 1 < ZL_AST_CHILD_NODE_SIZE)
					stackVal->i = chnum[stackVal->i + 1];
				else
					stackVal->i = nodes[nodenum].childs.extchilds[stackVal->i + 1 - ZL_AST_CHILD_NODE_SIZE];
				while(stackVal->i > 0)
				{
					//compile->AsmGenCodes(VM_ARG,i); // TODO
					zengl_AsmGCLoopStackPush(VM_ARG, stackVal->i, ZL_ST_START); // 将 stackVal->i 压入栈，返回后会对 stackVal->i 对应的节点执行生成汇编指令的操作
					stackVal->status = ZL_ASMGC_ELSE_STATUS_LOOP_IN_BLOCK; return ZL_ST_START;
else_loop_in_block:
					stackVal->i = nodes[stackVal->i].nextnode;
				}
				break;
			}
		}
		compile->LDAddrListSet(VM_ARG,compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_IF_END,ZL_TRUE),compile->gencode_struct.pc); //设置if结构结束位置的汇编位置。
		//state = ZL_ST_DOWN; // TODO
		zengl_AsmGCLoopStackValsPop(VM_ARG,loopStackTop);
		return zengl_AsmGCLoopStackFinishTop(VM_ARG, orig_nodenum);
	}
	else
	{
		compile->parser_curnode = nodenum;
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_AT_LEAST_TWO_CHILDS);
	}
}

typedef enum _ZENGL_ASMGC_PPMM_STATUS{
	ZENGL_ASMGC_PPMM_STATUS_START,
	ZENGL_ASMGC_PPMM_STATUS_FINISH_LEFT_PP,
	ZENGL_ASMGC_PPMM_STATUS_FINISH_LEFT_MM,
	ZENGL_ASMGC_PPMM_STATUS_FINISH_RIGHT_PP,
	ZENGL_ASMGC_PPMM_STATUS_FINISH_RIGHT_MM,
	ZENGL_ASMGC_PPMM_STATUS_FINISH_EXPRESS
} ZENGL_ASMGC_PPMM_STATUS;

typedef struct _ZENGL_ASMGC_PPMM_STACK_VAL{
	ZENGL_ASMGC_PPMM_STATUS status;
} ZENGL_ASMGC_PPMM_STACK_VAL;

// 生成 ++和--运算符 的汇编指令
static ZENGL_STATES zengl_AsmGenCode_PP_MM(ZL_VOID * VM_ARG, ZENGL_STATES state, ZENGL_ASM_LOOP_STACK_TYPE ** loopStackTopArg)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_ASM_LOOP_STACK_TYPE * loopStackTop = (*loopStackTopArg);
	ZL_INT nodenum;
	ZL_INT orig_nodenum;
	ZL_INT * chnum;
	ZENGL_RUN_INST_OP_DATA inst_op_data;
	ZENGL_ASMGC_PPMM_STACK_VAL * stackVal;
	if(loopStackTop == ZL_NULL) {
		loopStackTop = & compile->AsmGCLoopStackList.stacks[compile->AsmGCLoopStackList.count - 1];
	}
	nodenum = loopStackTop->nodenum;
	orig_nodenum = loopStackTop->orig_nodenum;
	(*loopStackTopArg) = ZL_NULL;
	if(loopStackTop->stackValIndex < 0) {
		stackVal = (ZENGL_ASMGC_PPMM_STACK_VAL *)zengl_AsmGCLoopStackValsPush(VM_ARG, loopStackTop, sizeof(ZENGL_ASMGC_PPMM_STACK_VAL));
		stackVal->status = ZENGL_ASMGC_PPMM_STATUS_START;
	}
	else
		stackVal = (ZENGL_ASMGC_PPMM_STACK_VAL *)(& compile->AsmGCLoopStackList.stackVals[loopStackTop->stackValIndex]);

	chnum = nodes[orig_nodenum].childs.childnum;
	switch(stackVal->status)
	{
	case ZENGL_ASMGC_PPMM_STATUS_START:
		loopStackTop->state = state;
		break;
	case ZENGL_ASMGC_PPMM_STATUS_FINISH_LEFT_PP:
		goto finish_left_pp;
	case ZENGL_ASMGC_PPMM_STATUS_FINISH_LEFT_MM:
		goto finish_left_mm;
	case ZENGL_ASMGC_PPMM_STATUS_FINISH_RIGHT_PP:
		goto finish_right_pp;
	case ZENGL_ASMGC_PPMM_STATUS_FINISH_RIGHT_MM:
		goto finish_right_mm;
	case ZENGL_ASMGC_PPMM_STATUS_FINISH_EXPRESS:
		goto finish_express;
	}

	if(nodes[nodenum].childs.count == 1)
	{
		switch(nodes[chnum[0]].toktype)
		{
		case ZL_TK_ID:
			switch(nodes[nodenum].leftOrRight)
			{
			case ZL_OP_POS_IN_LEFT: //加加减减在左侧，执行ADDGET,MINIS_GET即先加减，再取值。
				switch(nodes[nodenum].toktype)
				{
				case ZL_TK_PLUS_PLUS:
					inst_op_data = compile->SymLookupID(VM_ARG,chnum[0]);
					run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
							ZL_R_IT_ADDGET, ZL_R_DT_NONE , 0,
							inst_op_data.type,inst_op_data.val.mem); //对应汇编指令 类似 "ADDGET (%d)"
					break;
				case ZL_TK_MINIS_MINIS:
					inst_op_data = compile->SymLookupID(VM_ARG,chnum[0]);
					run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
							ZL_R_IT_MINIS_GET, ZL_R_DT_NONE , 0,
							inst_op_data.type,inst_op_data.val.mem); //对应汇编指令 类似 "MINIS_GET (%d)"
					break;
				}
				break;
			case ZL_OP_POS_IN_RIGHT: //GETADD,GET_MINIS,先取值后加减
				switch(nodes[nodenum].toktype)
				{
				case ZL_TK_PLUS_PLUS:
					inst_op_data = compile->SymLookupID(VM_ARG,chnum[0]);
					run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
							ZL_R_IT_GETADD, ZL_R_DT_NONE , 0,
							inst_op_data.type,inst_op_data.val.mem); //对应汇编指令 类似 "GETADD (%d)"
					break;
				case ZL_TK_MINIS_MINIS:
					inst_op_data = compile->SymLookupID(VM_ARG,chnum[0]);
					run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
							ZL_R_IT_GET_MINIS, ZL_R_DT_NONE , 0,
							inst_op_data.type,inst_op_data.val.mem); //对应汇编指令 类似 "GET_MINIS (%d)"
					break;
				}
				break;
			}
			break; //case ZL_TK_ID:
		case ZL_TK_ARRAY_ITEM: //加加减减遇到数组元素或者类的成员时，就在AsmGenCodes里生成数组元素的汇编代码，并且在生成过程中会根据ZL_ASM_AI_OP_IN_ADDGET之类的类型生成对应的代码。
		case ZL_TK_DOT:
			switch(nodes[nodenum].leftOrRight)
			{
			case ZL_OP_POS_IN_LEFT: //加加减减在左侧
				switch(nodes[nodenum].toktype)
				{
				case ZL_TK_PLUS_PLUS:
					compile->AsmGCStackPush(VM_ARG,(ZL_INT)ZL_ASM_AI_OP_IN_ADDGET,ZL_ASM_STACK_ENUM_ARRAY_ITEM_OP_TYPE);
					//compile->AsmGenCodes(VM_ARG,chnum[0]); // TODO
					zengl_AsmGCLoopStackPush(VM_ARG, chnum[0], ZL_ST_START); // 将chnum[0]压入栈，返回后会对chnum[0]对应的节点执行生成汇编指令的操作
					stackVal->status = ZENGL_ASMGC_PPMM_STATUS_FINISH_LEFT_PP; return ZL_ST_START;
finish_left_pp:
					compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_ARRAY_ITEM_OP_TYPE,ZL_TRUE);
					break;
				case ZL_TK_MINIS_MINIS:
					compile->AsmGCStackPush(VM_ARG,(ZL_INT)ZL_ASM_AI_OP_IN_MINISGET,ZL_ASM_STACK_ENUM_ARRAY_ITEM_OP_TYPE);
					//compile->AsmGenCodes(VM_ARG,chnum[0]); // TODO
					zengl_AsmGCLoopStackPush(VM_ARG, chnum[0], ZL_ST_START); // 将chnum[0]压入栈，返回后会对chnum[0]对应的节点执行生成汇编指令的操作
					stackVal->status = ZENGL_ASMGC_PPMM_STATUS_FINISH_LEFT_MM; return ZL_ST_START;
finish_left_mm:
					compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_ARRAY_ITEM_OP_TYPE,ZL_TRUE);
					break;
				}
				break; //case ZL_OP_POS_IN_LEFT: //加加减减在左侧
			case ZL_OP_POS_IN_RIGHT: //GETADD,GET_MINIS,先取值后加减 
				switch(nodes[nodenum].toktype)
				{
				case ZL_TK_PLUS_PLUS:
					compile->AsmGCStackPush(VM_ARG,(ZL_INT)ZL_ASM_AI_OP_IN_GETADD,ZL_ASM_STACK_ENUM_ARRAY_ITEM_OP_TYPE);
					//compile->AsmGenCodes(VM_ARG,chnum[0]); // TODO
					zengl_AsmGCLoopStackPush(VM_ARG, chnum[0], ZL_ST_START); // 将chnum[0]压入栈，返回后会对chnum[0]对应的节点执行生成汇编指令的操作
					stackVal->status = ZENGL_ASMGC_PPMM_STATUS_FINISH_RIGHT_PP; return ZL_ST_START;
finish_right_pp:
					compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_ARRAY_ITEM_OP_TYPE,ZL_TRUE);
					break;
				case ZL_TK_MINIS_MINIS:
					compile->AsmGCStackPush(VM_ARG,(ZL_INT)ZL_ASM_AI_OP_IN_GETMINIS,ZL_ASM_STACK_ENUM_ARRAY_ITEM_OP_TYPE);
					//compile->AsmGenCodes(VM_ARG,chnum[0]); // TODO
					zengl_AsmGCLoopStackPush(VM_ARG, chnum[0], ZL_ST_START); // 将chnum[0]压入栈，返回后会对chnum[0]对应的节点执行生成汇编指令的操作
					stackVal->status = ZENGL_ASMGC_PPMM_STATUS_FINISH_RIGHT_MM; return ZL_ST_START;
finish_right_mm:
					compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_ARRAY_ITEM_OP_TYPE,ZL_TRUE);
					break;
				}
				break;
			}
			break; //case ZL_TK_DOT: case ZL_TK_ARRAY_ITEM: //加加减减遇到数组元素或者类的成员时的处理
		case ZL_TK_NUM: //如果直接对整数使用++,--则直接对结果进行加一，减一。
			inst_op_data.val.num = ZENGL_SYS_STR_TO_LONG_NUM(compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[0]].strindex));
			run->AddInst(VM_ARG,compile->gencode_struct.pc++,chnum[0],
							ZL_R_IT_MOV,ZL_R_DT_REG,ZL_R_RT_AX,
							ZL_R_DT_NUM,inst_op_data.val.num); //对应汇编指令 类似 "MOV AX 123"
			switch(nodes[nodenum].toktype)
			{
			case ZL_TK_PLUS_PLUS:
				run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
						ZL_R_IT_ADDONE, ZL_R_DT_NONE , 0,
						ZL_R_DT_NONE , 0); //对应汇编指令 "ADDONE"
				break;
			case ZL_TK_MINIS_MINIS:
				run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
						ZL_R_IT_MINIS_ONE, ZL_R_DT_NONE , 0,
						ZL_R_DT_NONE , 0); //对应汇编指令 "MINIS_ONE"
				break;
			}
			break; //case ZL_TK_NUM:
		case ZL_TK_FLOAT: //如果直接对浮点数使用++,--则直接对结果进行加一，减一。
			inst_op_data.val.floatnum = ZENGL_SYS_STR_TO_FLOAT(compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[0]].strindex));
			run->AddInst(VM_ARG,compile->gencode_struct.pc++,chnum[0],
							ZL_R_IT_MOV,ZL_R_DT_REG,ZL_R_RT_AX,
							ZL_R_DT_FLOAT,inst_op_data.val.floatnum); //对应汇编指令 类似 "MOV AX 3.1415926"
			switch(nodes[nodenum].toktype)
			{
			case ZL_TK_PLUS_PLUS:
				run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
						ZL_R_IT_ADDONE, ZL_R_DT_NONE , 0,
						ZL_R_DT_NONE , 0); //对应汇编指令 "ADDONE"
				break;
			case ZL_TK_MINIS_MINIS:
				run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
						ZL_R_IT_MINIS_ONE, ZL_R_DT_NONE , 0,
						ZL_R_DT_NONE , 0); //对应汇编指令 "MINIS_ONE"
				break;
			}
			break; //case ZL_TK_FLOAT:
		case ZL_TK_STR:
			inst_op_data.val.num = (ZL_LONG)compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[0]].strindex);
			run->AddInst(VM_ARG,compile->gencode_struct.pc++,chnum[0],
							ZL_R_IT_MOV,ZL_R_DT_REG,ZL_R_RT_AX,
							ZL_R_DT_STR,inst_op_data.val.num); //对应汇编指令 类似 [MOV AX "hello world"]
			switch(nodes[nodenum].toktype)
			{
			case ZL_TK_PLUS_PLUS:
				run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
						ZL_R_IT_ADDONE, ZL_R_DT_NONE , 0,
						ZL_R_DT_NONE , 0); //对应汇编指令 "ADDONE"
				break;
			case ZL_TK_MINIS_MINIS:
				run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
						ZL_R_IT_MINIS_ONE, ZL_R_DT_NONE , 0,
						ZL_R_DT_NONE , 0); //对应汇编指令 "MINIS_ONE"
				break;
			}
			break; //case ZL_TK_STR:
		default:
			if(ZENGL_AST_ISTOKEXPRESS(chnum[0])) //直接对表达式结果加一，减一
			{
				//compile->AsmGenCodes(VM_ARG,chnum[0]); // TODO
				zengl_AsmGCLoopStackPush(VM_ARG, chnum[0], ZL_ST_START); // 将chnum[0]压入栈，返回后会对chnum[0]对应的节点执行生成汇编指令的操作
				stackVal->status = ZENGL_ASMGC_PPMM_STATUS_FINISH_EXPRESS; return ZL_ST_START;
finish_express:
				switch(nodes[nodenum].toktype)
				{
				case ZL_TK_PLUS_PLUS:
					run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
							ZL_R_IT_ADDONE, ZL_R_DT_NONE , 0,
							ZL_R_DT_NONE , 0); //对应汇编指令 "ADDONE"
					break;
				case ZL_TK_MINIS_MINIS:
					run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
							ZL_R_IT_MINIS_ONE, ZL_R_DT_NONE , 0,
							ZL_R_DT_NONE , 0); //对应汇编指令 "MINIS_ONE"
					break;
				}
			}
			else
			{
				compile->parser_curnode = nodenum;
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_INVALID_CHILD_NODE_TYPE,
					compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[0]].strindex),
					nodes[chnum[0]].line_no,
					nodes[chnum[0]].col_no,
					nodes[chnum[0]].filename);
			}
			break; //default:
		} //switch(nodes[chnum[0]].toktype)
		//state = ZL_ST_DOWN; // TODO
		zengl_AsmGCLoopStackValsPop(VM_ARG,loopStackTop);
		return zengl_AsmGCLoopStackFinishTop(VM_ARG, orig_nodenum);
	}
	else
	{
		compile->parser_curnode = nodenum;
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_ONE_CHILD);
	}
}

// 生成 打印语句 的汇编指令
static ZENGL_STATES zengl_AsmGenCode_Print(ZL_VOID * VM_ARG, ZENGL_STATES state, ZENGL_ASM_LOOP_STACK_TYPE ** loopStackTopArg)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_ASM_LOOP_STACK_TYPE * loopStackTop = (*loopStackTopArg);
	ZL_INT nodenum;
	ZL_INT orig_nodenum;
	ZL_INT * chnum;
	ZENGL_RUN_INST_OP_DATA inst_op_data;
	if(loopStackTop == ZL_NULL) {
		loopStackTop = & compile->AsmGCLoopStackList.stacks[compile->AsmGCLoopStackList.count - 1];
	}
	nodenum = loopStackTop->nodenum;
	orig_nodenum = loopStackTop->orig_nodenum;
	(*loopStackTopArg) = ZL_NULL;

	chnum = nodes[orig_nodenum].childs.childnum;
	if(nodenum == -1)
	{
		nodenum = orig_nodenum;
		goto finish_express;
	}

	if(nodes[nodenum].childs.count == 1)
	{
		loopStackTop->state = state;
		switch(nodes[chnum[0]].toktype)
		{
		case ZL_TK_ID: //打印变量等标示符
			inst_op_data = compile->SymLookupID(VM_ARG,chnum[0]);
			run->AddInst(VM_ARG, compile->gencode_struct.pc++, nodenum,
						ZL_R_IT_PRINT , ZL_R_DT_NONE , 0,
						inst_op_data.type , inst_op_data.val.mem); //对应汇编指令 类似 "PRINT (%d)"
			break;
		case ZL_TK_NUM:
			inst_op_data.val.num = ZENGL_SYS_STR_TO_LONG_NUM(compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[0]].strindex));
			run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
						ZL_R_IT_PRINT , ZL_R_DT_NONE , 0,
						ZL_R_DT_NUM,inst_op_data.val.num); //对应汇编指令 类似 "PRINT 123"
			break;
		case ZL_TK_FLOAT:
			inst_op_data.val.floatnum = ZENGL_SYS_STR_TO_FLOAT(compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[0]].strindex));
			run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
						ZL_R_IT_PRINT , ZL_R_DT_NONE , 0,
						ZL_R_DT_FLOAT,inst_op_data.val.floatnum); //对应汇编指令 类似 "PRINT 3.1415926"
			break;
		case ZL_TK_STR:
			inst_op_data.val.num = (ZL_LONG)compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[0]].strindex);
			run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
						ZL_R_IT_PRINT , ZL_R_DT_NONE , 0,
						ZL_R_DT_STR,inst_op_data.val.num); //对应汇编指令 类似 [PRINT "hello world"]
			break;
		default:
			if(ZENGL_AST_ISTOKEXPRESS(chnum[0]))
			{
				//compile->AsmGenCodes(VM_ARG,chnum[0]); // TODO
				zengl_AsmGCLoopStackPush(VM_ARG, chnum[0], ZL_ST_START); // 将chnum[0]压入栈，返回后会对chnum[0]对应的节点执行生成汇编指令的操作
				loopStackTop->nodenum = -1; return ZL_ST_START;
finish_express:
				run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
						ZL_R_IT_PRINT , ZL_R_DT_NONE , 0,
						ZL_R_DT_REG,ZL_R_RT_AX); //对应汇编指令 "PRINT AX"
			}
			else
			{
				compile->parser_curnode = chnum[0];
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_INVALID_TOKEN_CAN_NOT_GEN_CODE);
			}
			break;
		} //switch(nodes[chnum[0]].toktype)
		//state = ZL_ST_DOWN; // TODO
		return zengl_AsmGCLoopStackFinishTop(VM_ARG, orig_nodenum);
	}
	else
	{
		compile->parser_curnode = nodenum;
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_PRINT_MUST_HAVE_ONE_CHILD);
	}
}

typedef enum _ZENGL_ASMGC_FOR_STATUS{
	ZENGL_ASMGC_FOR_STATUS_START,
	ZENGL_ASMGC_FOR_STATUS_FINISH_FIRST_EXPRESS,
	ZENGL_ASMGC_FOR_STATUS_FINISH_SECOND_EXPRESS,
	ZENGL_ASMGC_FOR_STATUS_LOOP_IN_BLOCK,
	ZENGL_ASMGC_FOR_STATUS_FINISH_THIRD_EXPRESS
} ZENGL_ASMGC_FOR_STATUS;

typedef struct _ZENGL_ASMGC_FOR_STACK_VAL{
	ZL_INT * chnum;
	ZL_INT * extnum;
	ZL_INT i;
	ZENGL_ASMGC_FOR_STATUS status;
} ZENGL_ASMGC_FOR_STACK_VAL;

// 生成 for...endfor 循环控制语句的汇编指令
static ZENGL_STATES zengl_AsmGenCode_For(ZL_VOID * VM_ARG, ZENGL_STATES state, ZENGL_ASM_LOOP_STACK_TYPE ** loopStackTopArg)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_ASM_LOOP_STACK_TYPE * loopStackTop = (*loopStackTopArg);
	ZL_INT nodenum;
	ZL_INT orig_nodenum;
	ZENGL_ASMGC_FOR_STACK_VAL * stackVal;
	if(loopStackTop == ZL_NULL) {
		loopStackTop = & compile->AsmGCLoopStackList.stacks[compile->AsmGCLoopStackList.count - 1];
	}
	nodenum = loopStackTop->nodenum;
	orig_nodenum = loopStackTop->orig_nodenum;
	(*loopStackTopArg) = ZL_NULL;
	if(loopStackTop->stackValIndex < 0) {
		stackVal = (ZENGL_ASMGC_FOR_STACK_VAL *)zengl_AsmGCLoopStackValsPush(VM_ARG, loopStackTop, sizeof(ZENGL_ASMGC_FOR_STACK_VAL));
		stackVal->status = ZENGL_ASMGC_FOR_STATUS_START;
	}
	else
		stackVal = (ZENGL_ASMGC_FOR_STACK_VAL *)(& compile->AsmGCLoopStackList.stackVals[loopStackTop->stackValIndex]);

	switch(stackVal->status)
	{
	case ZENGL_ASMGC_FOR_STATUS_START:
		loopStackTop->state = state;
		break;
	case ZENGL_ASMGC_FOR_STATUS_FINISH_FIRST_EXPRESS:
		goto finish_first_express;
	case ZENGL_ASMGC_FOR_STATUS_FINISH_SECOND_EXPRESS:
		goto finish_second_express;
	case ZENGL_ASMGC_FOR_STATUS_LOOP_IN_BLOCK:
		goto loop_in_block;
	case ZENGL_ASMGC_FOR_STATUS_FINISH_THIRD_EXPRESS:
		goto finish_third_express;
	}

	if(nodes[nodenum].childs.count == 4)
	{
		stackVal->chnum = nodes[nodenum].childs.childnum; //chnum包含最开始的3个子节点。
		stackVal->extnum = nodes[nodenum].childs.extchilds; //extnum包含超过3个的扩展子节点。
		compile->AsmGCStackPush(VM_ARG,compile->AsmGCAddrNum++,ZL_ASM_STACK_ENUM_FOR_ADDR); //因为for内部还可以嵌套其他的for语句，所以需要将和for有关的地址进行压栈操作。
		compile->AsmGCStackPush(VM_ARG,compile->AsmGCAddrNum++,ZL_ASM_STACK_ENUM_FOR_END);
		compile->AsmGCStackPush(VM_ARG,compile->AsmGCAddrNum++,ZL_ASM_STACK_ENUM_FOR_CONTINUE);
		if(stackVal->chnum[0] != -1 && ZENGL_AST_ISTOKEXPRESS(stackVal->chnum[0])) //例如for(i=1;i<3;i++)其中的i=1初始化部分就是第一个子节点。
		{
			//compile->AsmGenCodes(VM_ARG,chnum[0]); // TODO //生成第一个子节点对应的表达式的汇编代码。
			zengl_AsmGCLoopStackPush(VM_ARG,stackVal->chnum[0], ZL_ST_START); // 将stackVal->chnum[0]压入栈，返回后会对stackVal->chnum[0]对应的节点执行生成汇编指令的操作
			stackVal->status = ZENGL_ASMGC_FOR_STATUS_FINISH_FIRST_EXPRESS; return ZL_ST_START;
finish_first_express:
			;
		}
		compile->LDAddrListSet(VM_ARG,compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_FOR_ADDR,ZL_FALSE),compile->gencode_struct.pc); //记录下第二个比较子节点的开始汇编代码位置。循环体在执行到结束时会跳转到这里进行判断是否需要继续循环。
		if(stackVal->chnum[1] != -1 && ZENGL_AST_ISTOKEXPRESS(stackVal->chnum[1]))
		{
			// compile->AsmGenCodes(VM_ARG,chnum[1]); // TODO
			zengl_AsmGCLoopStackPush(VM_ARG,stackVal->chnum[1], ZL_ST_START); // 将stackVal->chnum[1]压入栈，返回后会对stackVal->chnum[1]对应的节点执行生成汇编指令的操作
			stackVal->status = ZENGL_ASMGC_FOR_STATUS_FINISH_SECOND_EXPRESS; return ZL_ST_START;
finish_second_express:
			run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
				ZL_R_IT_JE,ZL_R_DT_NONE,0,
				ZL_R_DT_LDADDR,compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_FOR_END,ZL_FALSE)); //对应汇编指令 类似 "JE adr%d" 如果判断为false就跳过循环。
		}
		stackVal->i = stackVal->extnum[0]; //扩展子节点部分就是for...endfor之间的循环体部分的代码。
		while(stackVal->i > 0) //如果节点号小于0，说明是条空语句。
		{
			//compile->AsmGenCodes(VM_ARG,i); // TODO //循环生成for...endfor之间的所有语句对应的汇编代码。
			zengl_AsmGCLoopStackPush(VM_ARG,stackVal->i, ZL_ST_START); // 将stackVal->i压入栈，返回后会对stackVal->i对应的节点执行生成汇编指令的操作
			stackVal->status = ZENGL_ASMGC_FOR_STATUS_LOOP_IN_BLOCK; return ZL_ST_START;
loop_in_block:
			stackVal->i = nodes[stackVal->i].nextnode;
		}
		compile->LDAddrListSet(VM_ARG,compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_FOR_CONTINUE,ZL_FALSE),compile->gencode_struct.pc); //循环体的末尾就是continue将要跳转的位置。
		if(stackVal->chnum[2] != -1 && ZENGL_AST_ISTOKEXPRESS(stackVal->chnum[2])) //第三个子节点就是上例中的i++，循环体执行完后需要执行的代码。
		{
			//compile->AsmGenCodes(VM_ARG,chnum[2]); // TODO
			zengl_AsmGCLoopStackPush(VM_ARG,stackVal->chnum[2], ZL_ST_START); // 将stackVal->chnum[2]压入栈，返回后会对stackVal->chnum[2]对应的节点执行生成汇编指令的操作
			stackVal->status = ZENGL_ASMGC_FOR_STATUS_FINISH_THIRD_EXPRESS; return ZL_ST_START;
finish_third_express:
			;
		}
		run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
				ZL_R_IT_JMP,ZL_R_DT_NONE,0,
				ZL_R_DT_LDADDR,compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_FOR_ADDR,ZL_FALSE)); //对应汇编指令 类似 "JMP adr%d" 执行完第三个子节点的代码后，跳转到第二个子节点的代码处进行判断是否需要继续循环。
		compile->LDAddrListSet(VM_ARG,compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_FOR_END,ZL_FALSE),compile->gencode_struct.pc); //for...endfor结束位置
		compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_FOR_ADDR,ZL_TRUE); //生成完for的汇编代码后，将for相关的地址弹出栈。 
		compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_FOR_END,ZL_TRUE);
		compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_FOR_CONTINUE,ZL_TRUE); 
		//state = ZL_ST_DOWN; // TODO
		zengl_AsmGCLoopStackValsPop(VM_ARG,loopStackTop);
		return zengl_AsmGCLoopStackFinishTop(VM_ARG, orig_nodenum);
	}
	else
	{
		compile->parser_curnode = nodenum;
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_FOUR_CHILDS);
	}
}

typedef enum _ZENGL_ASMGC_FUNCALL_STATUS{
	ZENGL_ASMGC_FUNCALL_STATUS_START,
	ZENGL_ASMGC_FUNCALL_STATUS_FINISH_PARAMS
} ZENGL_ASMGC_FUNCALL_STATUS;

typedef struct _ZENGL_ASMGC_FUNCALL_STACK_VAL{
	ZENGL_ASMGC_FUNCALL_STATUS status;
} ZENGL_ASMGC_FUNCALL_STACK_VAL;

/* 生成 函数调用 相关的汇编指令 */
static ZENGL_STATES zengl_AsmGenCode_Funcall(ZL_VOID * VM_ARG, ZENGL_STATES state, ZENGL_ASM_LOOP_STACK_TYPE ** loopStackTopArg)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_ASM_LOOP_STACK_TYPE * loopStackTop = (*loopStackTopArg);
	ZL_INT nodenum;
	ZL_INT orig_nodenum;
	ZL_INT * chnum;
	ZENGL_ASMGC_FUNCALL_STACK_VAL * stackVal;
	ZENGL_RUN_INST_OP_DATA inst_op_data;
	if(loopStackTop == ZL_NULL) {
		loopStackTop = & compile->AsmGCLoopStackList.stacks[compile->AsmGCLoopStackList.count - 1];
	}
	nodenum = loopStackTop->nodenum;
	orig_nodenum = loopStackTop->orig_nodenum;
	(*loopStackTopArg) = ZL_NULL;
	if(loopStackTop->stackValIndex < 0) {
		stackVal = (ZENGL_ASMGC_FUNCALL_STACK_VAL *)zengl_AsmGCLoopStackValsPush(VM_ARG, loopStackTop, sizeof(ZENGL_ASMGC_FUNCALL_STACK_VAL));
		stackVal->status = ZENGL_ASMGC_FUNCALL_STATUS_START;
	}
	else
		stackVal = (ZENGL_ASMGC_FUNCALL_STACK_VAL *)(& compile->AsmGCLoopStackList.stackVals[loopStackTop->stackValIndex]);

	chnum = nodes[orig_nodenum].childs.childnum;
	switch(stackVal->status)
	{
	case ZENGL_ASMGC_FUNCALL_STATUS_START:
		loopStackTop->state = state;
		break;
	case ZENGL_ASMGC_FUNCALL_STATUS_FINISH_PARAMS:
		goto finish_params;
	}

	if(nodes[nodenum].childs.count == 1)
	{
		ZL_INT tmpReturnPC,tmpFunID,tmpClassID; //将函数调用返回时要执行的下一条代码的汇编代码位置
		compile->AsmGCStackPush(VM_ARG,(ZL_INT)ZL_TK_FUNCALL,ZL_ASM_STACK_ENUM_IS_IN_ARRAYITEM_OR_FUNCALL); //例如test(a+b)语句，a+b生成代码后，因为在FUNCALL函数调用里，所以会PUSH将a+b的值压入栈，从而成为test函数的参数
		run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
			ZL_R_IT_PUSH,ZL_R_DT_NONE,0,
			ZL_R_DT_REG,ZL_R_RT_ARG); //对应汇编指令 "PUSH ARG" 将当前的ARG参数寄存器压入栈。
		run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
			ZL_R_IT_PUSH,ZL_R_DT_NONE,0,
			ZL_R_DT_REG,ZL_R_RT_LOC); //对应汇编指令 "PUSH LOC" 将当前的LOC局部变量寄存器压入栈。
		run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
			ZL_R_IT_PUSH,ZL_R_DT_NONE,0,
			ZL_R_DT_REG,ZL_R_RT_ARGTMP); //对应汇编指令 "PUSH ARGTMP" 将当前的ARGTMP临时参数寄存器也压入栈。
		run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
			ZL_R_IT_RESET,ZL_R_DT_NONE,0,
			ZL_R_DT_REG,ZL_R_RT_ARGTMP); //对应汇编指令 "RESET ARGTMP" ARGTMP临时的ARG参数寄存器，如果在这一步就直接RESET ARG的话，下面在PUSH 参数时，因为参数可能是个表达式，而表达式里很可能要用到当前的ARG参数寄存器，所以先RESET ARGTMP寄存器，让ARGTMP寄存器记录下当前的虚拟堆栈中位置，等参数生成完后，再将ARGTMP 赋值给ARG寄存器。
		//chnum = nodes[nodenum].childs.childnum;
		if(chnum[0] == -1) //如果函数调用的参数为空如test()语句，则子节点为-1，此时跳过代码的生成
			;
		else if(nodes[chnum[0]].tokcategory == ZL_TKCG_OP_FACTOR || ZENGL_AST_ISTOKEXPRESS(chnum[0])) //函数调用的参数部分要么是单个的标识符之类的操作因子，要么是逗号分隔开的表达式，不论是操作因子还是表达式，都可以将参数节点压入栈，返回到汇编输出主循环后，都会生成相应的汇编指令，并将结果PUSH到AX中，以作为函数调用的参数。
		{
			compile->AsmGCStackPush(VM_ARG,0,ZL_ASM_STACK_ENUM_FUN_CLASSID);
			//compile->AsmGenCodes(VM_ARG,chnum[0]); // TODO
			zengl_AsmGCLoopStackPush(VM_ARG,chnum[0], ZL_ST_START); // 将chnum[0]压入栈，返回后会对chnum[0]对应的节点执行生成汇编指令的操作
			stackVal->status = ZENGL_ASMGC_FUNCALL_STATUS_FINISH_PARAMS; return ZL_ST_START;
finish_params:
			compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_FUN_CLASSID,ZL_TRUE);
		}
		else
			compile->exit(VM_ARG,ZL_ERR_CP_SYNTAX_INVALID_TOKEN,
				nodes[chnum[0]].line_no,
				nodes[chnum[0]].col_no,
				nodes[chnum[0]].filename,
				compile->getTokenStr(VM_ARG,nodes,chnum[0]));
		tmpReturnPC = compile->gencode_struct.pc + 4;
		run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
			ZL_R_IT_PUSH,ZL_R_DT_NONE,0,
			ZL_R_DT_NUM,tmpReturnPC); //对应汇编指令 类似 "PUSH %d" 将函数调用返回时要执行的下一条代码的汇编代码位置压入栈中。
		run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
			ZL_R_IT_MOV,ZL_R_DT_REG,ZL_R_RT_ARG,
			ZL_R_DT_REG,ZL_R_RT_ARGTMP); //对应汇编指令 "MOV ARG ARGTMP" 将ARGTMP赋值给ARG寄存器。
		run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
			ZL_R_IT_RESET,ZL_R_DT_NONE,0,
			ZL_R_DT_REG,ZL_R_RT_LOC); //对应汇编指令 "RESET LOC" 将当前虚拟堆栈的位置值通过RESET指令赋值给LOC寄存器。
		tmpClassID = compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_FUN_CLASSID,ZL_FALSE);
		if(tmpClassID < 0)
			tmpClassID = 0;
		if((tmpFunID = compile->SymLookupFun(VM_ARG,nodenum,tmpClassID)) == 0)
		{
			inst_op_data.val.num = (ZL_LONG)compile->TokenStringPoolGetPtr(VM_ARG,nodes[nodenum].strindex);
			run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
				ZL_R_IT_CALL,ZL_R_DT_NONE,0,
				ZL_R_DT_STR,inst_op_data.val.num); //对应汇编指令 "CALL %s" //如果在当前的用户自定义脚本函数哈希表中没有找到函数的信息，就说明该函数不是用户自定义的函数，而是use关键字引入的模块里的函数，所以就输出CALL "函数名"这种汇编格式，这样虚拟机解释器在运行时就会在use引入的模块中查找并调用函数。
		}
		else
			run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
				ZL_R_IT_JMP,ZL_R_DT_NONE,0,
				ZL_R_DT_LDFUNID,tmpFunID); //对应汇编指令 类似 "JMP funid%d" 如果找到函数的信息，说明是用户自定义的脚本函数，则跳转到目标函数的可执行代码入口处。这里使用的是函数ID，在链接替换时，先由函数ID得到函数伪地址，再由伪地址得到真实汇编位置，这样函数就可以定义在脚本的任意合法位置，如果直接使用伪地址，那么函数就只能定义在函数调用之前
		compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_IS_IN_ARRAYITEM_OR_FUNCALL,ZL_TRUE); //FUNCALL生成完毕，可以将之前的压栈弹出。
		//state = ZL_ST_DOWN; // TODO
		zengl_AsmGCLoopStackValsPop(VM_ARG,loopStackTop);
		return zengl_AsmGCLoopStackFinishTop(VM_ARG, orig_nodenum);
	}
	else
	{
		compile->parser_curnode = nodenum;
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_ONE_CHILD);
	}
}

// 生成return语句的汇编指令
static ZENGL_STATES zengl_AsmGenCode_Return(ZL_VOID * VM_ARG, ZENGL_STATES state, ZENGL_ASM_LOOP_STACK_TYPE ** loopStackTopArg)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_ASM_LOOP_STACK_TYPE * loopStackTop = (*loopStackTopArg);
	ZL_INT nodenum;
	ZL_INT orig_nodenum;
	ZL_INT * chnum;
	ZENGL_RUN_INST_OP_DATA inst_op_data;
	if(loopStackTop == ZL_NULL) {
		loopStackTop = & compile->AsmGCLoopStackList.stacks[compile->AsmGCLoopStackList.count - 1];
	}
	nodenum = loopStackTop->nodenum;
	orig_nodenum = loopStackTop->orig_nodenum;
	(*loopStackTopArg) = ZL_NULL;

	chnum = nodes[orig_nodenum].childs.childnum;
	if(nodenum == -2)
	{
		nodenum = orig_nodenum;
		goto finish_express;
	}

	if(compile->gencode_struct.is_inFun != ZL_TRUE)
	{
		compile->parser_curnode = nodenum;
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_UNEXPECT_LOC_OF_RETURN);
	}
	if(nodes[nodenum].childs.count == 1) //return关键字后面跟的是要返回的值，如果是变量标识符或数字或字符串，就直接将变量,数字,字符串的值赋值给AX寄存器作为返回值，如果是表达式就计算出表达式的值，因为表达式的结果默认就在AX中，所以可以直接作为返回值。最后以RET指令跳出脚本函数。
	{
		loopStackTop->state = state;
		//chnum = nodes[nodenum].childs.childnum;
		if(chnum[0] == -1) //当子节点为-1，如return;语句，return后没有表达式，所以子节点为-1，就需要在此处跳过，否则下面数组访问-1的索引就会出现内存错误。
			;
		else
		{
			switch(nodes[chnum[0]].toktype) //以下为对第一个子节点的判断并输出相应的汇编代码，结果储存在AX寄存器中。
			{
			case ZL_TK_ID:
				inst_op_data = compile->SymLookupID(VM_ARG,chnum[0]);
				run->AddInst(VM_ARG,compile->gencode_struct.pc++,chnum[0],
								ZL_R_IT_MOV,ZL_R_DT_REG,ZL_R_RT_AX,
								inst_op_data.type,inst_op_data.val.mem); //对应汇编指令 类似 "MOV AX (%d)"
				break;
			case ZL_TK_NUM:
				inst_op_data.val.num = ZENGL_SYS_STR_TO_LONG_NUM(compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[0]].strindex));
				run->AddInst(VM_ARG,compile->gencode_struct.pc++,chnum[0],
								ZL_R_IT_MOV,ZL_R_DT_REG,ZL_R_RT_AX,
								ZL_R_DT_NUM,inst_op_data.val.num); //对应汇编指令 类似 "MOV AX 123"
				break;
			case ZL_TK_FLOAT:
				inst_op_data.val.floatnum = ZENGL_SYS_STR_TO_FLOAT(compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[0]].strindex));
				run->AddInst(VM_ARG,compile->gencode_struct.pc++,chnum[0],
								ZL_R_IT_MOV,ZL_R_DT_REG,ZL_R_RT_AX,
								ZL_R_DT_FLOAT,inst_op_data.val.floatnum); //对应汇编指令 类似 "MOV AX 3.1415926"
				break;
			case ZL_TK_STR:
				inst_op_data.val.num = (ZL_LONG)compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[0]].strindex);
				run->AddInst(VM_ARG,compile->gencode_struct.pc++,chnum[0],
								ZL_R_IT_MOV,ZL_R_DT_REG,ZL_R_RT_AX,
								ZL_R_DT_STR,inst_op_data.val.num); //对应汇编指令 类似 [MOV AX "hello world"]
				break;
			default:
				if(ZENGL_AST_ISTOKEXPRESS(chnum[0]))
				{
					//compile->AsmGenCodes(VM_ARG,chnum[0]); // TODO
					zengl_AsmGCLoopStackPush(VM_ARG, chnum[0], ZL_ST_START); // 将chnum[0]压入栈，返回后会对chnum[0]对应的节点执行生成汇编指令的操作
					loopStackTop->nodenum = -2; return ZL_ST_START;
finish_express:
					;
				}
				else if(chnum[0] != -1)
				{
					compile->parser_curnode = nodenum;
					compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_INVALID_CHILD_NODE_TYPE,
						compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[0]].strindex),
						nodes[chnum[0]].line_no,
						nodes[chnum[0]].col_no,
						nodes[chnum[0]].filename);
				}
				break;
			} //switch(nodes[chnum[0]].toktype)
		} //else
		run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
				ZL_R_IT_RET,ZL_R_DT_NONE,0,
				ZL_R_DT_NONE,0); //对应汇编指令 "RET"
		//state = ZL_ST_DOWN; // TODO
		return zengl_AsmGCLoopStackFinishTop(VM_ARG, orig_nodenum);
	}
	else
	{
		compile->parser_curnode = nodenum;
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_ONE_CHILD);
	}
}

// 生成数组元素的汇编指令
static ZENGL_STATES zengl_AsmGenCode_ArrayItem(ZL_VOID * VM_ARG, ZENGL_STATES state, ZENGL_ASM_LOOP_STACK_TYPE ** loopStackTopArg)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_ASM_LOOP_STACK_TYPE * loopStackTop = (*loopStackTopArg);
	ZL_INT nodenum;
	ZL_INT orig_nodenum;
	ZL_INT * chnum;
	ZENGL_RUN_INST_OP_DATA inst_op_data;
	if(loopStackTop == ZL_NULL) {
		loopStackTop = & compile->AsmGCLoopStackList.stacks[compile->AsmGCLoopStackList.count - 1];
	}
	nodenum = loopStackTop->nodenum;
	orig_nodenum = loopStackTop->orig_nodenum;
	(*loopStackTopArg) = ZL_NULL;

	chnum = nodes[orig_nodenum].childs.childnum;
	if(nodenum == -2)
	{
		nodenum = orig_nodenum;
		goto finish_express;
	}

	if(nodes[nodenum].childs.count == 1)
	{
		ZENGL_ASM_ARRAY_ITEM_OP_TYPE array_item;
		loopStackTop->state = state;
		compile->AsmGCStackPush(VM_ARG,(ZL_INT)ZL_TK_ARRAY_ITEM,ZL_ASM_STACK_ENUM_IS_IN_ARRAYITEM_OR_FUNCALL); //将ZL_TK_ARRAY_ITEM压入栈，用于内部嵌套其他数组元素，同时在下面生成子节点的表达式的汇编指令后还会输出PUSH AX，这样数组元素里的表达式的值就可以作为数组索引(索引存储在栈中)。
		run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
				ZL_R_IT_PUSH,ZL_R_DT_NONE,0,
				ZL_R_DT_REG,ZL_R_RT_ARRAY_ITEM); //对应汇编指令 "PUSH ARRAY_ITEM" 先将原来的ARRAY_ITEM寄存器压入栈。
		run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
				ZL_R_IT_RESET,ZL_R_DT_NONE,0,
				ZL_R_DT_REG,ZL_R_RT_ARRAY_ITEM); //对应汇编指令 "RESET ARRAY_ITEM" 设置当前的ARRAY_ITEM寄存器为当前的栈顶。
		chnum = nodes[nodenum].childs.childnum;
		if(chnum[0] == -1) //等于-1时表示空节点，如test[]
			;
		else if(nodes[chnum[0]].tokcategory == ZL_TKCG_OP_FACTOR || ZENGL_AST_ISTOKEXPRESS(chnum[0]))
		{
			compile->AsmGCStackPush(VM_ARG,(ZL_INT)ZL_ASM_AI_OP_NONE,ZL_ASM_STACK_ENUM_ARRAY_ITEM_OP_TYPE); //防止在生成嵌套数组元素的汇编指令时，受到外层数组元素的ARRAY_ITEM_OP_TYPE的影响
			//compile->AsmGenCodes(VM_ARG,chnum[0]); // TODO
			zengl_AsmGCLoopStackPush(VM_ARG, chnum[0], ZL_ST_START); // 将chnum[0]压入栈，返回后会对chnum[0]对应的节点执行生成汇编指令的操作
			loopStackTop->nodenum = -2; return ZL_ST_START;
finish_express:
			compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_ARRAY_ITEM_OP_TYPE,ZL_TRUE);
		}
		else
		{
			compile->parser_curnode = nodenum;
			compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_INVALID_CHILD_NODE_TYPE,
				compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[0]].strindex),
				nodes[chnum[0]].line_no,
				nodes[chnum[0]].col_no,
				nodes[chnum[0]].filename);
		}
		array_item = compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_ARRAY_ITEM_OP_TYPE,ZL_FALSE);
		switch(array_item)
		{
		case ZL_ASM_AI_OP_IN_MOV: //如果是类似test[0] = 5这样的赋值语句，就输出SET_ARRAY指令，该指令会用AX里的值来设置数组里的元素。
			inst_op_data = compile->SymLookupID(VM_ARG,nodenum);
			run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
						ZL_R_IT_SET_ARRAY,ZL_R_DT_NONE,0,
						inst_op_data.type,inst_op_data.val.mem); //对应汇编指令 类似 "SET_ARRAY (%d)"
			break;
		case ZL_ASM_AI_OP_IN_ADDR: //如果是类似 &test[0] 这样的引用数组元素的语句，就输出GET_ARRAY_ADDR指令，该指令会得到数组元素的引用信息。
			inst_op_data = compile->SymLookupID(VM_ARG,nodenum);
			run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
						ZL_R_IT_GET_ARRAY_ADDR,ZL_R_DT_NONE,0,
						inst_op_data.type,inst_op_data.val.mem); //对应汇编指令 类似 "GET_ARRAY_ADDR (%d)"
			break;
		case ZL_ASM_AI_OP_IN_ADDGET: //如果是类似 ++test[0] 这样的先加加后取值的语句，就输出ADDGET_ARRAY指令，将数组元素进行加一操作再返回值。
			inst_op_data = compile->SymLookupID(VM_ARG,nodenum);
			run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
						ZL_R_IT_ADDGET_ARRAY,ZL_R_DT_NONE,0,
						inst_op_data.type,inst_op_data.val.mem); //对应汇编指令 类似 "ADDGET_ARRAY (%d)"
			break;
		case ZL_ASM_AI_OP_IN_MINISGET: //--test[0]之类的语句
			inst_op_data = compile->SymLookupID(VM_ARG,nodenum);
			run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
						ZL_R_IT_MINISGET_ARRAY,ZL_R_DT_NONE,0,
						inst_op_data.type,inst_op_data.val.mem); //对应汇编指令 类似 "MINISGET_ARRAY (%d)"
			break;
		case ZL_ASM_AI_OP_IN_GETADD: //test[0]++之类的语句
			inst_op_data = compile->SymLookupID(VM_ARG,nodenum);
			run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
						ZL_R_IT_GETADD_ARRAY,ZL_R_DT_NONE,0,
						inst_op_data.type,inst_op_data.val.mem); //对应汇编指令 类似 "GETADD_ARRAY (%d)"
			break;
		case ZL_ASM_AI_OP_IN_GETMINIS: //test[0]--之类的语句
			inst_op_data = compile->SymLookupID(VM_ARG,nodenum);
			run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
						ZL_R_IT_GETMINIS_ARRAY,ZL_R_DT_NONE,0,
						inst_op_data.type,inst_op_data.val.mem); //对应汇编指令 类似 "GETMINIS_ARRAY (%d)"
			break;
		default: //a = test[0]之类的获取数组元素的值的语句。
			inst_op_data = compile->SymLookupID(VM_ARG,nodenum);
			run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
						ZL_R_IT_GET_ARRAY,ZL_R_DT_NONE,0,
						inst_op_data.type,inst_op_data.val.mem); //对应汇编指令 类似 "GET_ARRAY (%d)"
			break;
		} //switch(array_item)
		compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_IS_IN_ARRAYITEM_OR_FUNCALL,ZL_TRUE); //输出完数组元素的汇编指令后，就可以弹出之前的压栈了。
		//state = ZL_ST_DOWN; // TODO
		return zengl_AsmGCLoopStackFinishTop(VM_ARG, orig_nodenum);
	}
	else
	{
		compile->parser_curnode = nodenum;
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_ONE_CHILD);
	}
}

typedef enum _ZENGL_ASMGC_CLASS_STATUS{
	ZENGL_ASMGC_CLASS_STATUS_START,
	ZENGL_ASMGC_CLASS_STATUS_FINISH_FUN
} ZENGL_ASMGC_CLASS_STATUS;

typedef struct _ZENGL_ASMGC_CLASS_STACK_VAL{
	ZL_INT tmpNodeNum;
	ZENGL_ASMGC_CLASS_STATUS status;
} ZENGL_ASMGC_CLASS_STACK_VAL;

/* 生成class语句的汇编指令 */
static ZENGL_STATES zengl_AsmGenCode_Class(ZL_VOID * VM_ARG, ZENGL_STATES state, ZENGL_ASM_LOOP_STACK_TYPE ** loopStackTopArg)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_ASM_LOOP_STACK_TYPE * loopStackTop = (*loopStackTopArg);
	ZL_INT nodenum;
	ZL_INT orig_nodenum;
	ZL_INT * chnum;
	ZENGL_ASMGC_CLASS_STACK_VAL * stackVal;
	if(loopStackTop == ZL_NULL) {
		loopStackTop = & compile->AsmGCLoopStackList.stacks[compile->AsmGCLoopStackList.count - 1];
	}
	nodenum = loopStackTop->nodenum;
	orig_nodenum = loopStackTop->orig_nodenum;
	(*loopStackTopArg) = ZL_NULL;
	if(loopStackTop->stackValIndex < 0) {
		stackVal = (ZENGL_ASMGC_CLASS_STACK_VAL *)zengl_AsmGCLoopStackValsPush(VM_ARG, loopStackTop, sizeof(ZENGL_ASMGC_CLASS_STACK_VAL));
		stackVal->status = ZENGL_ASMGC_CLASS_STATUS_START;
	}
	else
		stackVal = (ZENGL_ASMGC_CLASS_STACK_VAL *)(& compile->AsmGCLoopStackList.stackVals[loopStackTop->stackValIndex]);

	chnum = nodes[orig_nodenum].childs.childnum;
	switch(stackVal->status)
	{
	case ZENGL_ASMGC_CLASS_STATUS_START:
		loopStackTop->state = state;
		break;
	case ZENGL_ASMGC_CLASS_STATUS_FINISH_FUN:
		goto finish_fun;
	}

	if(nodes[nodenum].childs.count == 2)
	{
		ZL_INT classid;
		//chnum = nodes[nodenum].childs.childnum;
		compile->AsmGCIsInClass = ZL_TRUE;
		classid = compile->SymLookupClass(VM_ARG,chnum[0]);
		compile->AsmGCStackPush(VM_ARG,classid,ZL_ASM_STACK_ENUM_FUN_CLASSID);
		stackVal->tmpNodeNum = chnum[1];
		while(stackVal->tmpNodeNum !=0)
		{
			switch(nodes[stackVal->tmpNodeNum].reserve)
			{
			case ZL_RSV_FUN:
				//compile->AsmGenCodes(VM_ARG,tmpNodeNum); // TODO
				zengl_AsmGCLoopStackPush(VM_ARG, stackVal->tmpNodeNum, ZL_ST_START); // 将stackVal->tmpNodeNum压入栈，返回后会对stackVal->tmpNodeNum对应的节点执行生成汇编指令的操作
				stackVal->status = ZENGL_ASMGC_CLASS_STATUS_FINISH_FUN; return ZL_ST_START;
finish_fun:
				break;
			}
			stackVal->tmpNodeNum = nodes[stackVal->tmpNodeNum].nextnode;
		}
		compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_FUN_CLASSID,ZL_TRUE);
		compile->AsmGCIsInClass = ZL_FALSE;
	}
	//state = ZL_ST_DOWN; // TODO
	zengl_AsmGCLoopStackValsPop(VM_ARG,loopStackTop);
	return zengl_AsmGCLoopStackFinishTop(VM_ARG, orig_nodenum);
}

/* 生成通过点运算符访问类成员的汇编指令 */
static ZENGL_STATES zengl_AsmGenCode_Dot(ZL_VOID * VM_ARG, ZENGL_STATES state, ZENGL_ASM_LOOP_STACK_TYPE ** loopStackTopArg)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZENGL_ASM_LOOP_STACK_TYPE * loopStackTop = (*loopStackTopArg);
	ZL_INT nodenum;
	ZL_INT orig_nodenum;
	ZL_INT * chnum;
	ZENGL_ASMGC_DOT_STACK_VAL * stackVal;
	if(loopStackTop == ZL_NULL) {
		loopStackTop = & compile->AsmGCLoopStackList.stacks[compile->AsmGCLoopStackList.count - 1];
	}
	nodenum = loopStackTop->nodenum;
	orig_nodenum = loopStackTop->orig_nodenum;
	(*loopStackTopArg) = ZL_NULL;
	if(loopStackTop->stackValIndex < 0) {
		stackVal = (ZENGL_ASMGC_DOT_STACK_VAL *)zengl_AsmGCLoopStackValsPush(VM_ARG, loopStackTop, sizeof(ZENGL_ASMGC_DOT_STACK_VAL));
		stackVal->status = ZENGL_ASMGC_DOT_STATUS_START;
	}
	else
		stackVal = (ZENGL_ASMGC_DOT_STACK_VAL *)(& compile->AsmGCLoopStackList.stackVals[loopStackTop->stackValIndex]);

	chnum = nodes[orig_nodenum].childs.childnum;
	switch(stackVal->status)
	{
	case ZENGL_ASMGC_DOT_STATUS_START:
		loopStackTop->state = state;
		break;
	case ZENGL_ASMGC_DOT_STATUS_FINISH_SCAN_DOT:
		goto finish_scan_dot;
	case ZENGL_ASMGC_DOT_STATUS_FINISH_FUNCALL:
		goto finish_funcall;
	}

	if(nodes[nodenum].childs.count == 2)
	{
		if(nodes[chnum[1]].toktype != ZL_TK_FUNCALL)
		{
			ZENGL_STATES ret_scandot;
			ZENGL_ASM_ARRAY_ITEM_OP_TYPE array_item;
			compile->AsmGCStackPush(VM_ARG,(ZL_INT)ZL_TK_ARRAY_ITEM,ZL_ASM_STACK_ENUM_IS_IN_ARRAYITEM_OR_FUNCALL); //将ZL_TK_ARRAY_ITEM压入栈，用于内部嵌套其他数组元素，同时在下面AsmGenCodes生成表达式后还会输出PUSH AX，这样数组元素里的表达式的值就可以作为数组索引。
			run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
					ZL_R_IT_PUSH,ZL_R_DT_NONE,0,
					ZL_R_DT_REG,ZL_R_RT_ARRAY_ITEM); //对应汇编指令 "PUSH ARRAY_ITEM" 先将原来的ARRAY_ITEM寄存器压入栈。
			run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
					ZL_R_IT_RESET,ZL_R_DT_NONE,0,
					ZL_R_DT_REG,ZL_R_RT_ARRAY_ITEM); //对应汇编指令 "RESET ARRAY_ITEM" 设置当前的ARRAY_ITEM寄存器为当前的栈顶。
			ret_scandot = compile->SymScanDotForClass(VM_ARG,nodenum,&loopStackTop);
			if(ret_scandot == ZL_ST_START)
				return ret_scandot;
finish_scan_dot:
			array_item = compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_ARRAY_ITEM_OP_TYPE,ZL_FALSE);
			switch(array_item)
			{
			case ZL_ASM_AI_OP_IN_MOV: //如果是类似test[0] = 5这样的赋值语句，就输出SET_ARRAY指令，该指令会用AX里的值来设置数组里的元素。
				run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
							ZL_R_IT_SET_ARRAY,ZL_R_DT_NONE,0,
							compile->memDataForDot.type,compile->memDataForDot.val.mem); //对应汇编指令 类似 "SET_ARRAY (%d)"
				break;
			case ZL_ASM_AI_OP_IN_ADDR: //如果是类似 &test[0] 这样的引用数组元素的语句，就输出GET_ARRAY_ADDR指令，该指令会得到数组元素的引用信息。
				run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
							ZL_R_IT_GET_ARRAY_ADDR,ZL_R_DT_NONE,0,
							compile->memDataForDot.type,compile->memDataForDot.val.mem); //对应汇编指令 类似 "GET_ARRAY_ADDR (%d)"
				break;
			case ZL_ASM_AI_OP_IN_ADDGET: //如果是类似 ++test[0] 这样的先加加后取值的语句，就输出ADDGET_ARRAY指令，将数组元素进行加一操作再返回值。
				run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
							ZL_R_IT_ADDGET_ARRAY,ZL_R_DT_NONE,0,
							compile->memDataForDot.type,compile->memDataForDot.val.mem); //对应汇编指令 类似 "ADDGET_ARRAY (%d)"
				break;
			case ZL_ASM_AI_OP_IN_MINISGET: //--test[0]之类的语句
				run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
							ZL_R_IT_MINISGET_ARRAY,ZL_R_DT_NONE,0,
							compile->memDataForDot.type,compile->memDataForDot.val.mem); ///对应汇编指令 类似 "MINISGET_ARRAY (%d)"
				break;
			case ZL_ASM_AI_OP_IN_GETADD: //test[0]++之类的语句
				run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
							ZL_R_IT_GETADD_ARRAY,ZL_R_DT_NONE,0,
							compile->memDataForDot.type,compile->memDataForDot.val.mem); ///对应汇编指令 类似 "GETADD_ARRAY (%d)"
				break;
			case ZL_ASM_AI_OP_IN_GETMINIS: //test[0]--之类的语句
				run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
							ZL_R_IT_GETMINIS_ARRAY,ZL_R_DT_NONE,0,
							compile->memDataForDot.type,compile->memDataForDot.val.mem); ///对应汇编指令 类似 "GETMINIS_ARRAY (%d)"
				break;
			default: //a = test[0]之类的获取数组元素的值的语句。
				run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
							ZL_R_IT_GET_ARRAY,ZL_R_DT_NONE,0,
							compile->memDataForDot.type,compile->memDataForDot.val.mem); ///对应汇编指令 类似 "GET_ARRAY (%d)"
				break;
			}
			compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_IS_IN_ARRAYITEM_OR_FUNCALL,ZL_TRUE); //输出完数组元素的汇编指令后，就可以弹出之前的压栈了。
		} //if(nodes[chnum[1]].toktype != ZL_TK_FUNCALL)
		else
		{
			ZL_INT classid = 0;
			if(nodes[chnum[0]].toktype != ZL_TK_ID)
			{
				compile->parser_curnode = nodenum;
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_INVALID_CHILD_NODE_TYPE,
					compile->TokenStringPoolGetPtr(VM_ARG,nodes[chnum[0]].strindex),
					nodes[chnum[0]].line_no,
					nodes[chnum[0]].col_no,
					nodes[chnum[0]].filename);
			}
			classid = compile->SymLookupClass(VM_ARG,chnum[0]);
			compile->AsmGCStackPush(VM_ARG,classid,ZL_ASM_STACK_ENUM_FUN_CLASSID);
			//compile->AsmGenCodes(VM_ARG,chnum[1]); // TODO
			zengl_AsmGCLoopStackPush(VM_ARG, chnum[1], ZL_ST_START); // 将chnum[1]压入栈，返回后会对chnum[1]对应的节点执行生成汇编指令的操作
			stackVal->status = ZENGL_ASMGC_DOT_STATUS_FINISH_FUNCALL; return ZL_ST_START;
finish_funcall:
			compile->AsmGCStackPop(VM_ARG,ZL_ASM_STACK_ENUM_FUN_CLASSID,ZL_TRUE);
		}
		//state = ZL_ST_DOWN; // TODO
		zengl_AsmGCLoopStackValsPop(VM_ARG,loopStackTop);
		return zengl_AsmGCLoopStackFinishTop(VM_ARG, orig_nodenum);
	} //if(nodes[nodenum].childs.count == 2)
	else
	{
		compile->parser_curnode = nodenum;
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_TWO_CHILDS);
	}
}

/*
 *	break语句的汇编代码生成，break如果在for里则跳出for，如果在switch里则跳出switch
 *	如果在while结构里则跳出while，如果在do...dowhile里则跳出do...dowhile
 */
static ZL_VOID zengl_AsmGCBreak_Codes(ZL_VOID * VM_ARG,ZL_INT nodenum)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZL_INT asm_ends[4] = {ZL_ASM_STACK_ENUM_FOR_END,ZL_ASM_STACK_ENUM_SWITCH_END,ZL_ASM_STACK_ENUM_WHILE_END,ZL_ASM_STACK_ENUM_DO_END};
	ZL_INT i,maxend,adrnum;
	for(i=1,maxend=0;i<4;i++)
	{
		if(compile->AsmGCStackList.ends[asm_ends[maxend]] < compile->AsmGCStackList.ends[asm_ends[i]])
			maxend = i;
	}
	adrnum = compile->AsmGCStackPop(VM_ARG,asm_ends[maxend],ZL_FALSE);
	if(adrnum == -1)
	{
		compile->parser_curnode = nodenum;
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_UNEXPECT_LOC_OF_BREAK_CONTINUE);
	}
	else
		run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
			ZL_R_IT_JMP,ZL_R_DT_NONE,0,
			ZL_R_DT_LDADDR,adrnum); //对应汇编指令 "JMP adr%d" 跳转到for，switch，while，do...dowhile结尾处
}

/*
 *	continue语句的汇编代码生成，判断是在for还是在while还是在do...dowhile里，然后
 *	根据判断跳到对应结构的开头
 */
static ZL_VOID zengl_AsmGCContinue_Codes(ZL_VOID * VM_ARG,ZL_INT nodenum)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_RUN_TYPE * run = &((ZENGL_VM_TYPE *)VM_ARG)->run;
	ZL_INT asm_ends[3] = {ZL_ASM_STACK_ENUM_FOR_CONTINUE,ZL_ASM_STACK_ENUM_WHILE_ADDR,ZL_ASM_STACK_ENUM_DO_CONTINUE};
	ZL_INT i,maxend,adrnum;
	for(i=1,maxend=0;i<3;i++)
	{
		if(compile->AsmGCStackList.ends[asm_ends[maxend]] < compile->AsmGCStackList.ends[asm_ends[i]])
			maxend = i;
	}
	adrnum = compile->AsmGCStackPop(VM_ARG,asm_ends[maxend],ZL_FALSE);
	if(adrnum == -1)
	{
		compile->parser_curnode = nodenum;
		compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_UNEXPECT_LOC_OF_BREAK_CONTINUE);
	}
	else
		run->AddInst(VM_ARG,compile->gencode_struct.pc++,nodenum,
			ZL_R_IT_JMP,ZL_R_DT_NONE,0,
			ZL_R_DT_LDADDR,adrnum); //对应汇编指令 "JMP adr%d" 跳转到for，while，do...dowhile的continue对应位置
}

/*
    扫描switch...case ，找出其中的case的最大值，最小值，以及判断是否有default默认节点。
	参数nodenum为switch节点的节点号。
	参数hasminmax用于判断是否有最大和最小值。
	参数minarg用于保存case中的最小值。
	参数maxarg用于保存case中的最大值。
	参数hasdefault用于判断是否有default节点。
	参数table是switch...case的跳转表。
*/
ZL_VOID zengl_AsmScanCaseMinMax(ZL_VOID * VM_ARG,ZL_INT nodenum,ZL_BOOL * hasminmax,ZL_INT * minarg,ZL_INT * maxarg,ZL_BOOL * hasdefault,
								ZENGL_ASM_CASE_JMP_TABLE * table)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZL_INT * chnum = ZL_NULL,* extnum = ZL_NULL,i;
	ZL_LONG num,min=0,max=0;
	ZENGL_AST_CHILD_NODE_TYPE * childs = &nodes[nodenum].childs;
	(*hasminmax) = ZL_FALSE;
	(*hasdefault) = ZL_FALSE;
	if(childs->count == 1)
		return;
	chnum = childs->childnum;
	extnum = childs->extchilds;
	for(i=1;i<nodes[nodenum].childs.count;i++)
	{
		if(i < ZL_AST_CHILD_NODE_SIZE)
			num = chnum[i];
		else
			num = extnum[i - ZL_AST_CHILD_NODE_SIZE];
		switch(nodes[num].reserve)
		{
		case ZL_RSV_CASE:
			num = compile->GetNodeInt(VM_ARG,nodes[num].childs.childnum[0]);
			compile->AsmAddCaseJmpTable(VM_ARG,table,num);
			if(i == 1)
				min = max = num;
			else
			{
				min = num < min ? num : min;
				max = num > max ? num : max;
			}
			(*hasminmax) = ZL_TRUE;
			break;
		case ZL_RSV_DEFAULT:
			(*hasdefault) = ZL_TRUE;
			break;
		}
	}
	(*minarg) = min;
	(*maxarg) = max;
}

/*返回节点的字符串信息的整数形式*/
ZL_LONG zengl_GetNodeInt(ZL_VOID * VM_ARG,ZL_INT nodenum)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	switch(nodes[nodenum].toktype)
	{
	case ZL_TK_NUM:
	case ZL_TK_FLOAT:
	case ZL_TK_STR:
		return ZENGL_SYS_STR_TO_LONG_NUM(compile->TokenStringPoolGetPtr(VM_ARG,nodes[nodenum].strindex));
		break;
	}
	return 0;
}

/*将case对应的比较数字添加到跳转表中*/
ZL_VOID zengl_AsmAddCaseJmpTable(ZL_VOID * VM_ARG,ZENGL_ASM_CASE_JMP_TABLE * table,ZL_LONG num)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	if(!table->isInit)
		compile->AsmInitCaseJmpTable(VM_ARG,table);
	if(table->count == table->size)
	{
		table->size += ZL_ASM_CASE_JMP_TABLE_SIZE;
		table->member = compile->memReAlloc(VM_ARG,table->member,table->size * sizeof(ZENGL_ASM_CASE_JMP_TABLE_MEMBER));
		if(table->member == ZL_NULL)
			compile->exit(VM_ARG,ZL_ERR_CP_ASM_CASE_JMP_TABLE_REALLOC_FAILED);
		ZENGL_SYS_MEM_SET(table->member + (table->size - ZL_ASM_CASE_JMP_TABLE_SIZE),0,
			ZL_ASM_CASE_JMP_TABLE_SIZE * sizeof(ZENGL_ASM_CASE_JMP_TABLE_MEMBER));
	}
	table->member[table->count].caseNum = num;
	table->count++;
}

/*初始化switch case的跳转表*/
ZL_VOID zengl_AsmInitCaseJmpTable(ZL_VOID * VM_ARG,ZENGL_ASM_CASE_JMP_TABLE * table)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	if(table->isInit)
		return;
	table->count = 0;
	table->size = ZL_ASM_CASE_JMP_TABLE_SIZE;
	table->member = compile->memAlloc(VM_ARG,table->size * sizeof(ZENGL_ASM_CASE_JMP_TABLE_MEMBER));
	if(table->member == ZL_NULL)
		compile->exit(VM_ARG,ZL_ERR_CP_ASM_CASE_JMP_TABLE_MALLOC_FAILED);
	ZENGL_SYS_MEM_SET(table->member,0,table->size * sizeof(ZENGL_ASM_CASE_JMP_TABLE_MEMBER));
	table->isInit = ZL_TRUE;
}

/*将switch...case跳转表进行从小到大的排序*/
ZL_VOID zengl_AsmSortCaseJmpTable(ZL_VOID * VM_ARG,ZENGL_ASM_CASE_JMP_TABLE * table,ZL_INT nodenum)
{
	ZENGL_COMPILE_TYPE * compile = &((ZENGL_VM_TYPE *)VM_ARG)->compile;
	ZENGL_AST_NODE_TYPE * nodes = compile->AST_nodes.nodes;
	ZL_INT i,j,k;
	ZENGL_ASM_CASE_JMP_TABLE_MEMBER temp;
	for(i=0;i<table->count-1;i++)
	{
		k = i;
		for(j=i+1;j<table->count;j++)
		{
			if(table->member[k].caseNum > table->member[j].caseNum)
				k = j;
			else if(table->member[k].caseNum == table->member[j].caseNum)
			{
				compile->parser_curnode = nodenum;
				compile->parser_errorExit(VM_ARG,ZL_ERR_CP_SYNTAX_ASM_SWITCH_FIND_SAME_CASE_VAL);
			}
		}
		if(k != i)
		{
			temp = table->member[i];
			table->member[i] = table->member[k];
			table->member[k] = temp;
		}
	}
}
