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

#ifndef _ZENGL_LOCALS_H_
#define _ZENGL_LOCALS_H_

/*各种错误的枚举定义*/
typedef enum _ZENGL_ERRORNO{
	ZL_NO_ERR_SUCCESS, //没有错误
	ZL_ERR_FILE_CAN_NOT_OPEN, //无法打开文件
	ZL_ERR_FILE_CAN_NOT_GETS, //无法获取文件内容
	ZL_ERR_CP_MTSTR_MALLOC_FAILED, //编译器在makeTokenStr函数中分配内存失败
	ZL_ERR_CP_MP_MALLOC_FAILED, //编译器为内存池指针分配内存失败
	ZL_ERR_CP_MP_MALLOC_NO_NULL_POINT, //编译器无法在内存池中找到NULL指针
	ZL_ERR_CP_MP_REALLOC_FAILED, //编译器为内存池指针重新分配内存失败
	ZL_ERR_CP_INVALID_OCTAL, //无效的八进制数
	ZL_ERR_CP_INVALID_HEX, //无效的十六进制数
	ZL_ERR_CP_INVALID_STR_NO_ENDCHAR, //字符串没有用于结束的单引号或双引号
	ZL_ERR_CP_MTSFS_MALLOC_FAILED, //编译器在makeTokenStrForString函数中分配内存失败
	ZL_ERR_CP_GT_INVALID_OR, //无效的逻辑或运算符
	ZL_ERR_CP_GT_INVALID_MULTI_COMMENT, //无效的多行注释
	ZL_ERR_CP_DEF_STR_POOL_MALLOC_FAILED, //编译器为def_StringPool常量宏字符串池分配内存初始化失败
	ZL_ERR_CP_DEF_STR_POOL_I_OUT_OF_BOUND, //编译器将宏相关的字符串信息添加到def_StringPool字符串池时超出字符串数组范围
	ZL_ERR_CP_DEF_TABLE_MALLOC_FAILED, //编译器为def_table宏定义动态数组分配内存初始化失败
	ZL_ERR_CP_LINECOL_MALLOC_FAILED, //编译器为LineCols行列号动态数组分配内存初始化失败
	ZL_ERR_CP_DEF_TABLE_NO_NONVALID_INDEX, //编译器异常：添加宏定义时，无法在动态数组中找到没被占用的索引
	ZL_ERR_CP_DEF_TABLE_INVALID_INDEX, //编译器异常：在函数insert_HashTableForDef中，tmpindex不是有效的索引
	ZL_ERR_CP_DEF_TABLE_SAME_DEF_FOUND, //语法错误：存在相同的def宏定义，请根据行列号进行检查
	ZL_ERR_CP_DEF_MUST_WITH_ID, //语法错误：def关键字后面必须是有效的id标识符
	ZL_ERR_CP_DEF_NO_END_SEMI, //语法错误：def定义宏必须以分号结束
	ZL_ERR_CP_DEF_INVALID_VALUE_TYPE, //语法错误：def定义的宏对应的值必须是数字，浮点数，或字符串
	ZL_ERR_CP_INC_FILENAME_TOO_LONG, //语法错误：文件路径信息太长
	ZL_ERR_CP_INC_FILENAME_TOO_LONG_WHEN_MERGE, //语法错误：合并后的文件路径信息太长
	ZL_ERR_CP_INC_FILESTACKLIST_MALLOC_FAILED, //编译器异常：为FileStackList文件加载堆栈分配内存初始化失败
	ZL_ERR_CP_INC_NO_END_SEMI, //语法错误：inc语句必须以分号结束
	ZL_ERR_CP_INC_MUST_WITH_STRING, //语法错误：inc关键字后面必须是字符串类型的文件名或文件相对路径
	ZL_ERR_CP_DEF_TABLE_INVALID_INDEX_WHEN_LOOKUP, //编译器异常：查找宏定义时，无效的索引值
	ZL_ERR_CP_DEF_TABLE_TK_STR_REALLOC_FAILED_WHEN_REPLACE, //编译器异常：在进行宏替换时重新分配内存失败
	ZL_ERR_CP_INC_FILESTACKLIST_OVERFLOW, //编译器异常：文件加载堆栈发生溢出
	ZL_ERR_CP_INC_FILESTACKLIST_INVALID_COUNT, //编译器异常：FileStackList文件堆栈的元素个数必须大于等于0
	ZL_ERR_CP_PARSER_AST_MALLOC_FAILED, //编译器异常：AST抽象语法树分配内存初始化失败
	ZL_ERR_CP_TK_STR_POOL_MALLOC_FAILED, //编译器异常：token字符串池分配内存初始化失败
	ZL_ERR_CP_TK_STR_POOL_OUT_OF_BOUNDS, //编译器异常：Token_StringPool缓存在添加字符串信息时发生严重错误：返回的索引超过缓存范围
	ZL_ERR_CP_PARSER_ERROR_EXIT, //parser err exit 在zengl_parser_errorExit函数中末尾连接的错误信息。
	ZL_ERR_CP_PARSER_CURNODE_OUT_OF_END, //语法分析错误：语法树当前扫描节点parser_curnode超出语法树范围
	ZL_ERR_CP_PARSER_OPLVL_STACKLIST_MALLOC_FAILED, //编译器异常：优先级比较堆栈分配内存初始化失败
	ZL_ERR_CP_SYNTAX_ID_INVALID_NEXT_NODE, //语法错误：id变量标识符后面必须是操作运算符或者左括号或者左中括号或分号
	ZL_ERR_CP_SYNTAX_INVALID_NEXT_NODE, //语法错误：无法识别的节点
	ZL_ERR_CP_SYNTAX_EXPRESS_OP_INVALID_NEXT_NODE, //语法错误：表达式操作运算符后面必须是变量，数字，字符串等操作因子或者加加减减或者取反运算符或左括号
	ZL_ERR_CP_SYNTAX_START_INVALID_TOKEN, //语法错误：开头无效的token
	ZL_ERR_CP_AST_SCAN_STACK_MALLOC_FAILED, //编译器异常：语法树的扫描堆栈分配内存初始化失败
	ZL_ERR_CP_AST_SCAN_STACK_HAS_NOTHING, //编译器异常：语法树的扫描堆栈为空
	ZL_ERR_CP_SYNTAX_NUM_FLOAT_STR_INVALID_NEXT_NODE, //语法错误：数字,浮点数,字符串后面必须是除了赋值和点之外的操作运算符或者分号
	ZL_ERR_CP_SYNTAX_INVALID_TOKEN, //语法错误：未知的Token，不能识别的符号
	ZL_ERR_CP_SYNTAX_ADDRESS_INVALID_NEXT_NODE, //语法错误：引用运算符后面必须是变量之类标识符
	ZL_ERR_CP_SYNTAX_PPMM_INVALID_NEXT_NODE, //语法错误：加加减减运算符后面遇到无效的符号
	ZL_ERR_CP_SYNTAX_LBRACKET_INVALID_NEXT_NODE, //语法错误：左括号后面必须是id变量标识符 ....
	ZL_ERR_CP_SYNTAX_RBRACKET_INVALID_NEXT_NODE, //语法错误：右括号后面必须是除了赋值，点，取反，引用之外的操作运算符或者分号
	ZL_ERR_CP_SYNTAX_BRACKET_EMPTY, //语法错误：普通的括号表达式不能为空，除非是函数的参数括号，因为函数可以没有参数
	ZL_ERR_CP_SYNTAX_RBRACKET_NO_LBRACKET, //语法错误：右括号没有对应的左括号
	ZL_ERR_CP_SYNTAX_LBRACKET_NO_RBRACKET, //语法错误：左括号没有对应的右括号
	ZL_ERR_CP_SYNTAX_PARSER_EXPRESS_UNKNOWN_STATE, //语法错误：在parser生成语法树的express函数中遇到无效的状态机，可能是还无法解析的token
	ZL_ERR_CP_SYNTAX_LMBRACKET_INVALID_NEXT_NODE, //语法错误：右中括号后面必须是id变量标识符，浮点数，数字，字符串或加加减减 .....
	ZL_ERR_CP_SYNTAX_INVALID_ARRAY_ITEM, //语法错误：无效的数组元素格式
	ZL_ERR_CP_SYNTAX_RMBRACKET_NO_LMBRACKET, //语法错误：右中括号没有对应的左括号
	ZL_ERR_CP_SYNTAX_LMBRACKET_NO_RMBRACKET, //语法错误：左中括号没有对应的右中括号
	ZL_ERR_CP_SYNTAX_RMBRACKET_INVALID_NEXT_NODE, //语法错误：右中括号后面必须是除了取反，引用之外的操作运算符或者右括号或者右中括号或者分号
	ZL_ERR_CP_SYNTAX_COMMA_INVALID_NEXT_NODE, //语法错误：逗号运算符后面必须是id变量标识符 .....
	ZL_ERR_CP_SYNTAX_DOT_INVALID_NEXT_NODE, //语法错误：点运算符后面必须是id变量标识符
	ZL_ERR_CP_SYNTAX_COLON_INVALID_NEXT_NODE, //语法错误：冒号运算符后面必须是id变量标识符 .....
	ZL_ERR_CP_SYNTAX_QUESTION_MARK_INVALID_NEXT_NODE, //语法错误：问号运算符后面必须是id变量标识符......
	ZL_ERR_CP_SYNTAX_QUESTION_NO_COLON_RIGHT, //语法错误：问号右侧没有相匹配的冒号
	ZL_ERR_CP_CHECK_QUESTION_VALID_FUNC_INVALID_TOKEN, //编译异常：zengl_CheckQstColonValid只能针对问号运算符进行检测
	ZL_ERR_CP_SYNTAX_COLON_NO_QUESTION_LEFT, //语法错误：冒号左侧没有相匹配的问号
	ZL_ERR_CP_SYNTAX_IF_NO_LBRACKET, //语法错误：if判断表达式没有左括号
	ZL_ERR_CP_DETECT_SET_RBRACKET_TO_SEMI_FUNC_INVALID_START_TOKEN, //编译异常：zengl_detectSetRBracketToSemi函数必须从左括号的token开始检测
	ZL_ERR_CP_SYNTAX_ELIF_MUST_BEFORE_ELSE, //语法错误：elif必须用在else之前
	ZL_ERR_CP_SYNTAX_ELSE_MORE_THAN_ONE, //语法错误：一个if结构里只允许有一个else
	ZL_ERR_CP_SYNTAX_UNKNOWN_KEYWORD_INIF, //语法错误：在if结构中遇到无效的关键字
	ZL_ERR_CP_SYNTAX_UNKNOWN_KEYWORD, //语法错误：无效的关键字
	ZL_ERR_CP_SYNTAX_IF_NO_ENDIF, //语法错误：if或内嵌的if语句没有匹配的endif
	ZL_ERR_CP_SYNTAX_FOR_NO_ENDFOR, //语法错误：for没有对应的endfor
	ZL_ERR_CP_SYNTAX_FOR_NO_LBRACKET, //语法错误：for条件判断表达式没有左括号
	ZL_ERR_CP_SYNTAX_FOR_STMT_MUST_HAVE_THREE_EXP, //语法错误：for条件判断表达式必须由三个子表达式组成
	ZL_ERR_CP_SYNTAX_ITS_NOT_ENDFOR, //语法错误：for必须以endfor结束
	ZL_ERR_CP_SYNTAX_FUN_CONTAIN_ANOTHER_FUN, //语法错误：一个fun函数中不可以包含另一个fun的定义
	ZL_ERR_CP_SYNTAX_FUN_NO_ENDFUN, //语法错误：fun没有对应的endfun
	ZL_ERR_CP_SYNTAX_FUN_NO_LBRACKET, //语法错误：fun函数参数没有左括号
	ZL_ERR_CP_SYNTAX_FUN_ARGLIST_INVALID_TOKEN, //语法错误：fun函数参数列表遇到无效的token
	ZL_ERR_CP_SYNTAX_FUN_NO_NAME, //语法错误：fun函数定义没有函数名
	ZL_ERR_CP_SYNTAX_ITS_NOT_ENDFUN, //语法错误：fun必须以endfun结束
	ZL_ERR_CP_SYNTAX_INVALID_EXP_AFTER_GLOBAL, //语法错误：global关键字后面是无效的表达式
	ZL_ERR_CP_SYNTAX_INVALID_EXP_AFTER_USE,	 //语法错误：use关键字后面是无效的表达式
	ZL_ERR_CP_SYNTAX_INVALID_BREAK_EXP, //语法错误：无效的break语句
	ZL_ERR_CP_SYNTAX_INVALID_CONTINUE_EXP, //语法错误：无效的continue语句
	ZL_ERR_CP_SYNTAX_SWITCH_NO_ENDSWT, //语法错误：switch没有对应的endswt或者endswitch
	ZL_ERR_CP_SYNTAX_SWITCH_NO_LBRACKET, //语法错误：switch条件判断表达式没有左括号
	ZL_ERR_CP_SYNTAX_CASE_MUST_WITH_NUM_FLOAT_STR, //语法错误：switch里的case后面必须是数字，浮点数或字符串
	ZL_ERR_CP_SYNTAX_CASE_MUST_END_WITH_COLON, //语法错误：switch里的case后面接数字，浮点数，字符串，并且必须以冒号结束，不可以接表达式
	ZL_ERR_CP_SYNTAX_CASE_MUST_BEFORE_DEFAULT, //语法错误：case必须用在default之前
	ZL_ERR_CP_SYNTAX_DEFAULT_USE_MORE_THAN_ONE, //语法错误：switch结构中只能有一个default
	ZL_ERR_CP_SYNTAX_DEFAULT_MUST_END_WITH_COLON, //语法错误：switch里的default后面必须直接以冒号结束
	ZL_ERR_CP_SYNTAX_UNKNOWN_KEYWORD_INSWITCH, //语法错误：在switch结构中遇到无效的关键字
	ZL_ERR_CP_SYNTAX_INVALID_MEMBER_INCLASS, //语法错误：class结构中包含无效的成员
	ZL_ERR_CP_SYNTAX_CLASS_NO_ENDCLS,  //语法错误：class没有匹配的endclass或endcls
	ZL_ERR_CP_SYNTAX_CLASS_NO_CLASS_NAME, //语法错误：class类定义中没有类名，类名必须紧跟在class关键字后面
	ZL_ERR_CP_SYNTAX_ITS_NOT_ENDCLS,	//语法错误：class必须以endcls或endclass结束
	ZL_ERR_CP_SYNTAX_WHILE_NO_ENDWHILE, //语法错误：while或内嵌的while没有对应的endwhile
	ZL_ERR_CP_SYNTAX_WHILE_NO_LBRACKET,	//语法错误：while条件判断表达式没有左括号
	ZL_ERR_CP_SYNTAX_ITS_NOT_ENDWHILE,	//语法错误：while必须以endwhile结束
	ZL_ERR_CP_SYNTAX_DO_NO_DOWHILE,		//语法错误：do没有对应的dowhile
	ZL_ERR_CP_SYNTAX_ITS_NOT_DOWHILE,	//语法错误：do必须以dowhile结束
	ZL_ERR_CP_SYNTAX_DOWHILE_NO_LBRACKET,	//语法错误：dowhile条件判断表达式没有左括号
	ZL_ERR_CP_SYNTAX_INVALID_EXP_AFTER_CLASS_STATEMENT,	//语法错误：无效的类声明语句
	ZL_ERR_CP_SYNTAX_KEYWORD_DETECT_UNEXPECT_KEYWORD,		//语法错误：%s检测到不期望的关键字%s
	ZL_ERR_CP_SYNTAX_UNEXPECT_ENDKEYWORD,	//语法错误：不期望的结束关键字%s
	ZL_ERR_CP_SYNTAX_UNEXPECT_LOC_OF_USE,	//语法错误：use关键字出现在不期望的位置
	ZL_ERR_CP_SYNTAX_UNEXPECT_LOC_OF_GLOBAL,//语法错误：global关键字出现在不期望的位置
	ZL_ERR_CP_SYNTAX_UNEXPECT_LOC_OF_RETURN,//语法错误：return关键字出现在不期望的位置
	ZL_ERR_CP_SYNTAX_UNEXPECT_LOC_OF_BREAK_CONTINUE,	//语法错误：break或continue必须用在while,do...dowhile,for,switch结构中
	ZL_ERR_CP_SYNTAX_INVALID_CLASS_FUNCALL,	//语法错误：调用类函数格式错误
	ZL_ERR_CP_SYM_GLOBAL_TABLE_MALLOC_FAILED, //编译器变异：在zengl_SymInitGlobalTable函数中为SymGlobalTable全局变量符号表分配内存初始化失败
	ZL_ERR_CP_SYM_GLOBAL_TABLE_NO_NOT_VALID_INDEX,		//编译器异常：zengl_SymInsertGlobalTable函数无法在全局变量符号表中找到没被占用的索引
	ZL_ERR_CP_SYM_GLOBAL_TABLE_FIND_NOT_VALID_INDEX,	//编译器异常：zengl_SymInsertHashTableForGlobal函数在SymGlobalTable符号表中找到的tmpindex是无效的
	ZL_ERR_CP_SYM_GLOBAL_TABLE_INDEX_LINECOL_NOT_INIT,	//编译器异常：tmpindex对应的符号的行列号没有初始化
	ZL_ERR_CP_SYM_GLOBAL_TABLE_INDEX_HAVE_NO_LINECOL_WHEN_PRINT,	//编译器异常：SymGlobalTable中的[%d]索引对应的符号没有行列号信息
	ZL_ERR_CP_SYM_CLASS_TABLE_MALLOC_FAILED, //编译器异常：在zengl_SymInitClassTable函数中为SymClassTable类符号表对应的动态数组分配内存初始化失败
	ZL_ERR_CP_SYM_CLASS_TABLE_NO_NOT_VALID_INDEX,		//编译器异常：zengl_SymInsertClassTable函数无法在类符号表中找到没被占用的索引
	ZL_ERR_CP_SYM_CLASS_TABLE_FIND_NOT_VALID_INDEX,		//编译器异常：zengl_SymInsertHashTableForClass函数在SymClassTable类符号表中找到的tmpindex是无效的
	ZL_ERR_CP_SYM_CLASS_HAS_BEEN_DEFINED,	//%s类已经被定义过了
	ZL_ERR_CP_SYM_CLASS_TABLE_INDEX_HAVE_NO_LINECOL_WHEN_PRINT,		//编译器异常：SymClassTable类符号表中的[%d]索引对应的类定义中没有行列号信息
	ZL_ERR_CP_SYM_FUN_TABLE_MALLOC_FAILED,	//编译器异常：在zengl_SymInitFunTable函数中为SymFunTable函数符号表分配内存初始化失败
	ZL_ERR_CP_SYM_FUN_TABLE_NO_NOT_VALID_INDEX,			//编译器异常：zengl_SymInsertFunTable函数无法在函数符号表中找到没被占用的索引
	ZL_ERR_CP_SYM_FUN_TABLE_FIND_NOT_VALID_INDEX,		//编译器异常：zengl_SymInsertHashTableForFun函数在SymFunTable函数表中找到的tmpindex是无效的
	ZL_ERR_CP_SYM_FUN_HAS_BEEN_DEFINED,		//%s脚本函数已经被定义过了
	ZL_ERR_CP_SYM_FUN_TABLE_INDEX_HAVE_NO_LINECOL_WHEN_PRINT,		//编译器异常：SymFunTable函数符号表中的[%d]索引对应的函数定义中没有行列号信息
	ZL_ERR_CP_SYNTAX_CLASS_NAME_OF_CLS_STMT_NOT_EXISTS,	//语法错误：类名'%s'不存在
	ZL_ERR_CP_SYM_CLASS_TABLE_FIND_NOT_VALID_INDEX_WHEN_LOOKUP,		//编译器异常：zengl_SymLookupClass函数在SymClassTable类符号表中找到的tmpindex是无效的索引
	ZL_ERR_CP_SYM_CLASSMEMBER_TABLE_MALLOC_FAILED,		//编译器异常：在zengl_SymInitClassMemberTable为SymClassMemberTable类成员符号表动态数组分配内存初始化失败
	ZL_ERR_CP_SYM_CLASSMEMBER_TABLE_NO_NOT_VALID_INDEX,	//编译器异常：zengl_SymInsertClassMemberTable函数无法在类成员符号表中找到没被占用的索引
	ZL_ERR_CP_SYM_CLASSMEMBER_TABLE_FIND_NOT_VALID_INDEX,	//编译器异常：zengl_SymInsertHashTableForClassMember函数在SymClassMemberTable类成员符号表中找到的tmpindex是无效的
	ZL_ERR_CP_SYNTAX_CLASSMEMBER_HAS_BEEN_DEFINED,		//%s类成员已经被定义过了
	ZL_ERR_CP_SYNTAX_ASM_ASSIGN_MUST_HAVE_TWO_CHILD,	//语法错误：赋值节点必须有两个子节点
	ZL_ERR_CP_SYM_CAN_NOT_FIND_ID_MEMLOC,	//编译器异常：无法找到'%s'变量的内存地址
	ZL_ERR_CP_SYM_GLOBAL_TABLE_FIND_NOT_VALID_INDEX_WHEN_LOOKUP,	//编译器异常：zengl_SymLookupID函数在全局符号表中找到的tmpindex是无效的索引
	ZL_ERR_CP_SYNTAX_ASM_ASSIGN_SECOND_NODE_IS_INVALID_TYPE,		//语法错误：赋值运算符第二个子节点既不是变量或数字或浮点数或字符串，也不是加减乘除等表达式
	ZL_ERR_CP_SYNTAX_ASM_ASSIGN_FIRST_NODE_IS_INVALID_TYPE,			//语法错误：赋值语句的第一个子节点不是有效的结构
	ZL_ERR_RUN_ERROR_EXIT_WHEN_IN_COMPILE,	// run err exit when in compile 
	ZL_ERR_RUN_MEM_POOL_MALLOC_FAILED,	//解释器异常：为解释器内存池分配内存初始化失败
	ZL_ERR_RUN_MEM_ALLOC_FUNC_FAILED,	//解释器异常：memAlloc函数分配内存失败
	ZL_ERR_RUN_MEM_ALLOC_FUNC_CAN_NOT_FIND_NULL_POINT,	//解释器异常：memAlloc函数无法在内存池中找到NULL指针，也就无法分配新的指针
	ZL_ERR_RUN_MEM_FREE_POOL_MALLOC_FAILED,	//解释器异常：为解释器的内存释放池分配内存初始化失败
	ZL_ERR_RUN_INST_LIST_MALLOC_FAILED,		//解释器异常：为解释器的汇编指令集分配内存初始化失败
	ZL_ERR_RUN_MEM_RE_ALLOC_INVALID_POINT,	//解释器异常：memReAlloc时，需要调整的指针在内存池中无效
	ZL_ERR_RUN_MEM_RE_ALLOC_FUNC_FAILED,	//解释器异常：memReAlloc函数重新分配内存失败
	ZL_ERR_RUN_INST_INVALID_PC,	//解释器异常：指令的PC值和PC寄存器里的值不一致
	ZL_ERR_RUN_INST_DATA_STRING_POOL_MALLOC_FAILED,	//解释器异常：在zenglrun_initInstDataStringPool函数中为指令操作数字符串池分配内存初始化失败
	ZL_ERR_RUN_INST_DATA_STR_POOL_ADD_I_OUT_OF_BOUNDS, //解释器异常：当指令操作数字符串池添加字符串时索引i超出范围
	ZL_ERR_RUN_VMEM_LIST_MALLOC_FAILED,		//解释器异常：为全局虚拟内存动态数组分配内存初始化失败
	ZL_ERR_RUN_MEM_RE_USE_PTR_MALLOC_FAILED,	//解释器异常：重利用内存池指针时，调整分配系统内存失败
	ZL_ERR_RUN_INVALID_VMEM_TYPE,			//解释器异常：无效的虚拟内存类型
	ZL_ERR_RUN_INVALID_INST_SRC_TYPE,		//解释器异常：指令中源操作数类型无效
	ZL_ERR_RUN_INVALID_INST_DEST_TYPE,		//解释器异常：指令中目标操作数类型无效
	ZL_ERR_RUN_INVALID_INST_TYPE,			//解释器异常：无效的汇编指令
	ZL_ERR_CP_SYNTAX_ASM_INVALID_TOKEN_CAN_NOT_GEN_CODE, //语法错误：无效的token，该token目前无法生成对应的汇编指令
	ZL_ERR_CP_SYNTAX_ASM_PRINT_MUST_HAVE_ONE_CHILD,		//语法错误：打印节点必须包含一个子节点
	ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_TWO_CHILDS, //语法错误：当前节点必须包含两个子节点
	ZL_ERR_CP_SYNTAX_ASM_INVALID_CHILD_NODE_TYPE,	//语法错误：遇到无效的子节点类型
	ZL_ERR_RUN_VSTACK_LIST_MALLOC_FAILED,	//解释器异常：为虚拟堆栈动态数组分配内存初始化失败
	ZL_ERR_RUN_VSTACK_LIST_OVERFLOW,		//解释器异常：虚拟堆栈反向溢出，栈顶计数器小于0，当前PC寄存器值：%d
	ZL_ERR_RUN_MOD_BY_ZERO,		//解释器运行时错误：取余运算时，发生除零错误
	ZL_ERR_RUN_DIVIDE_BY_ZERO,	//解释器运行时错误：除法运算时，发生除零错误
	ZL_ERR_RUN_SOURCE_CODE_INFO,//source code info: %s %d:%d <'%s'>
	ZL_ERR_CP_ASM_GCSTACK_LIST_MALLOC_FAILED,		//编译器异常：汇编堆栈动态数组分配内存初始化失败
	ZL_ERR_CP_LD_ADDR_LIST_MALLOC_FAILED,	//编译器异常：链接地址动态数组分配内存初始化失败
	ZL_ERR_CP_LD_INDEX_ADDR_MUST_UNIQUE,	//编译器异常：链接地址必须唯一，一个伪地址对应一个汇编代码位置
	ZL_ERR_CP_LD_INVALID_ADDR_INDEX,		//编译器异常：无效的伪地址:adr%d
	ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_ONE_CHILD,	//语法错误：当前节点必须有且只有一个子节点
	ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_AT_LEAST_TWO_CHILDS, //语法错误：当前节点必须至少有两个子节点
	ZL_ERR_CP_SYNTAX_ASM_INVALID_OP_FACTOR,	//语法错误：无效的操作因子
	ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_FOUR_CHILDS,	//语法错误：当前节点必须有且只有四个子节点
	ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_THREE_CHILDS,	//语法错误：当前节点必须有且只有三个子节点
	ZL_ERR_CP_SYNTAX_ASM_FUN_CAN_NOT_DEFINED_IN_OTHER_FUN,		//语法错误：在一个函数体内不能再定义另一个函数
	ZL_ERR_CP_SYM_AST_TREE_SCAN_STACK_NOT_EMPTY,	//编译器异常：AST扫描堆栈不为空
	ZL_ERR_CP_SYM_LOCAL_TABLE_MALLOC_FAILED,	//编译器异常：局部变量符号表动态数组分配内存初始化失败
	ZL_ERR_CP_SYM_LOCAL_TABLE_CAN_NOT_FIND_NOT_VALID_INDEX_WHEN_INSERT, //编译器异常：局部符号表无法找到没被占用的索引，无法完成符号插入操作
	ZL_ERR_CP_SYM_LOCAL_TABLE_INVALID_INDEX,	//编译器异常：局部符号表遇到无效索引
	ZL_ERR_CP_SYNTAX_SYM_GLOBAL_MUST_HAVE_CHILD_NODE,	//语法错误：global后面不可以为空节点即空语句
	ZL_ERR_CP_SYM_CAN_NOT_FIND_LOCAL_ID_MEMLOC,	//编译器异常：无法找到'%s'变量的内存地址
	ZL_ERR_CP_SYM_LOCAL_ID_INVALID_FUN_ID,		//编译器异常：局部变量符号信息对应的函数ID无效
	ZL_ERR_CP_SYM_LOCAL_ID_INVALID_TYPE,		//编译器异常：无效的局部符号类型
	ZL_ERR_RUN_MAIN_INVALID_FUN_RET_ADDRESS,	//解释器运行时错误：函数的汇编返回地址必须是整数
	ZL_ERR_RUN_MAIN_FUN_VSTACK_STRUCT_EXCEPTION,//解释器运行时错误：函数的虚拟栈结构异常
	ZL_ERR_CP_SYNTAX_SYM_USE_MUST_HAVE_CHILD_NODE,		//语法错误：use关键字后面不可以为空节点即空语句
	ZL_ERR_RUN_MODULE_TALBE_MALLOC_FAILED,		//解释器异常：模块动态数组分配内存初始化失败
	ZL_ERR_RUN_MODULE_TALBE_NO_NOT_VALID_INDEX,	//解释器异常：模块动态数组中无法找到没被占用的索引
	ZL_ERR_RUN_MODULE_TABLE_FIND_INVALID_INDEX,	//解释器异常：模块动态数组中遇到无效的索引
	ZL_ERR_RUN_MODULE_HAS_BEEN_SET_HANDLE_BEFORE,		//解释器运行时错误：模块'%s'已经被设置过处理句柄了
	ZL_ERR_RUN_MOD_FUN_TABLE_MALLOC_FAILED,		//解释器异常：模块函数动态数组分配内存初始化失败
	ZL_ERR_RUN_MOD_FUN_TABLE_NO_NOT_VALID_INDEX,//解释器异常：模块函数动态数组中无法找到没被占用的索引
	ZL_ERR_RUN_MOD_FUN_TABLE_FIND_INVALID_INDEX,//解释器异常：模块函数动态数组中遇到无效的索引
	ZL_ERR_RUN_MOD_FUN_HAS_BEEN_SET_HANDLE_IN_OTHER_MOD_BEFORE, //解释器运行时错误：模块函数'%s'已经在'%s'模块中设置过处理句柄了
	ZL_ERR_RUN_MODULE_IS_INVALID,				//解释器运行时错误：模块'%s'无效，可能原因是用户没有定义该模块的Use处理句柄，或者处理句柄无效
	ZL_ERR_RUN_FUNCTION_IS_INVALID,				//解释器运行时错误：函数'%s'无效
	ZL_ERR_VM_API_GET_FUN_ARG_ARGNUM_IS_INVALID,//解释器运行时错误：GetFunArg...接口的argnum参数无效，不可小于1，也不可大于参数总数
	ZL_ERR_VM_API_USER_DEFINED_ERROR,			//user defined error 用户自定义的错误信息
	ZL_ERR_VM_API_INVALID_FUNCTION_NAME,		//解释器运行时错误：Call接口中的函数名无效
	ZL_ERR_VM_API_INVALID_SCRIPT_NAME,			//解释器运行时错误：API接口中的脚本名无效
	ZL_ERR_VM_API_VM_MUST_RESET,				//解释器运行时错误：API接口中的虚拟机之前被用过了，请使用Reset接口进行重置
	ZL_ERR_VM_API_GET_VALUE_AS_MUST_IN_DEBUG_MODE,		//解释器运行时错误：GetValueAs...之类的接口只能用在DEBUG调试模式下
	ZL_ERR_VM_API_GET_VALUE_AS_VALUE_NAME_INVALID,		//解释器运行时错误：GetValueAs...之类的接口中的全局变量名无效，可能不存在于脚本中
	ZL_ERR_VM_API_SYM_GLOBAL_TABLE_INVALID_INDEX,		//解释器运行时错误：通过接口查找变量名时，全局符号表中发现无效索引
	ZL_ERR_VM_API_GET_VALUE_AS_VALUE_RUNTYPE_INVALID,	//解释器运行时错误：GetValueAs...之类的接口查找到的变量的运行时类型无效，只能是整数，浮点数，字符串类型
	ZL_ERR_RUN_INVALID_RUNTYPE_IN_GEN_BY_RUNTYPE_MACRO, //解释器运行时错误：ZENGL_RUN_VMEM_OP_GET_BY_RUNTYPE宏遇到无效的运行时类型
	ZL_ERR_RUN_MEM_BLOCK_MALLOC_FAILED,			//解释器运行时错误：为内存块分配内存初始化失败
	ZL_ERR_RUN_MEM_BLOCK_INVALID_INDEX,			//解释器运行时错误：对数组，类成员等内存块进行设置时，访问索引值无效，可能索引值小于0或者超出内存块范围
	ZL_ERR_RUN_MEM_BLOCK_VSTACK_STRUCT_EXCEPTION,		//解释器运行时错误：内存块的虚拟栈结构异常
	ZL_ERR_RUN_ARRAY_CLASS_ITEM_IS_NOT_INIT,	//解释器运行时错误：数组或类元素不存在，多半是没有被初始化或者索引值小于0，请使用array函数或直接赋值等方式进行初始化
	ZL_ERR_VM_API_SET_RET_VAL_INVALID_TYPE,		//解释器运行时错误：SetRetVal接口遇到无效的值类型
	ZL_ERR_SYNTAX_SYM_CLASS_STATEMENT_MUST_HAVE_CHILD_NODE,	//语法错误：类声明语句后面不可以为空节点即空语句
	ZL_ERR_CP_SYM_SCAN_DOT_FOR_CLASS_ID_ZERO_WHEN_NOT_INIT,	//编译器异常：ScanDotForClass在非初始扫描状态下遇到类ID为0
	ZL_ERR_SYNTAX_SYM_CAN_NOT_FIND_CLASS_INFO_FOR_ID,		//语法错误：该变量不是一个类变量，请使用类声明语句进行声明
	ZL_ERR_SYNTAX_INVALID_CLASS_MEMBER,			//语法错误：无效的类成员
	ZL_ERR_CP_SYNTAX_ASM_INVALID_RESERVE,				//语法错误：无效的关键字
	ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_AT_LEAST_ONE_CHILD, //语法错误：当前节点必须至少有一个子节点
	ZL_ERR_CP_ASM_CASE_JMP_TABLE_MALLOC_FAILED,	//编译器异常：switch...case结构跳转表分配内存初始化失败
	ZL_ERR_CP_ASM_CASE_JMP_TABLE_REALLOC_FAILED,//编译器异常：switch...case结构跳转表重新分配内存失败
	ZL_ERR_CP_SYNTAX_ASM_SWITCH_FIND_SAME_CASE_VAL,		//语法错误：switch结构中发现相同的case值
	ZL_ERR_CP_ASM_CASE_JMP_TABLE_CASEADDR_CAN_NOT_BE_ZERO,			//编译器异常：switch...case结构跳转表中case的目标代码位置不能为0
	ZL_ERR_RUN_SWITCH_HAVE_NO_LONG_INST_BELOW,	//解释器异常：SWITCH指令后面必须是LONG指令
	ZL_ERR_VM_API_INVALID_CLASS_NAME_WHEN_CALL,	//解释器运行时错误：Call接口中的类名无效
	ZL_ERR_VM_API_INVALID_CLASS_TABLE_INDEX_WHEN_CALL,	//解释器运行时异常：Call接口调用时找到无效的类索引
	ZL_ERR_VM_API_INVALID_MEMBLOCK_ARG_IN_CREATE_MEM_BLOCK,			//解释器运行时错误：CreateMemBlock接口中memblock参数无效，不可为空指针
	ZL_ERR_VM_API_INVALID_SIZE_ARG_IN_CREATE_MEM_BLOCK,	//解释器运行时错误：CreateMemBlock接口中size参数无效，size必须大于等于0
	ZL_ERR_VM_API_INVALID_MEMBLOCK_ARG_IN_SET_RETVAL,	//解释器运行时错误：SetRetValAsMemBlock接口中memblock参数无效，不可为空指针
	ZL_ERR_VM_API_INVALID_ARGNUM_ARG_IN_SET_FUN_ARG,	//解释器运行时错误：SetFunArg接口中argnum参数无效
	ZL_ERR_VM_API_INVALID_MEMBLOCK_ARG_IN_GET_MEM_BLOCK_SIZE,		//解释器运行时错误：GetMemBlockSize接口中memblock参数无效，不可为空指针
}ZENGL_ERRORNO;
/*各种错误的枚举定义结束*/

/*各种关键字的枚举定义*/
typedef enum _ZENGL_RESERVE_TYPE{
	ZL_RSV_START_NONE,
	ZL_RSV_PRINT,
	ZL_RSV_IF,
	ZL_RSV_ELIF,
	ZL_RSV_ELSE,
	ZL_RSV_ENDIF,
	ZL_RSV_FOR,
	ZL_RSV_ENDFOR,
	ZL_RSV_FUN,
	ZL_RSV_ENDFUN,
	ZL_RSV_GLOBAL,
	ZL_RSV_RETURN,
	ZL_RSV_USE,
	ZL_RSV_BREAK,
	ZL_RSV_CONTINUE,
	ZL_RSV_DEF,
	ZL_RSV_INC,
	ZL_RSV_SWITCH,
	ZL_RSV_CASE,
	ZL_RSV_DEFAULT,
	ZL_RSV_ENDSWT,
	ZL_RSV_CLASS,
	ZL_RSV_ENDCLS,
	ZL_RSV_WHILE,
	ZL_RSV_ENDWHILE,
	ZL_RSV_DO,
	ZL_RSV_DOWHILE,
	ZL_RSV_ENDSWITCH,
	ZL_RSV_ENDCLASS
}ZENGL_RESERVE_TYPE;
/*各种关键字的枚举定义结束*/

#endif/* _ZENGL_LOCALS_H_ */