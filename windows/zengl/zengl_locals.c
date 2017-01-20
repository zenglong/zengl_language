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

#include "zengl_locals.h"
#include "zengl_global.h"

/*为了方便语言的国际化，将虚拟机内部所有可能的错误时输出的字符串统一定义在一个数组中，不包含脚本中的输出！*/
#ifdef ZL_LANG_EN_WITH_CH
ZL_CONST ZL_CHAR * ZL_Error_String[] = {
	"no error success (没有错误)", //ZL_NO_ERR_SUCCESS
	"\n err: can't open file : \"%s\" (无法打开文件)\n", //ZL_ERR_FILE_CAN_NOT_OPEN
	"\n err: can't read \"%s\" file content.(无法读取文件内容)\n", //ZL_ERR_FILE_CAN_NOT_GETS
	"\n err: compile makeTokenStr alloc failure. (编译器在makeTokenStr函数中分配内存失败)\n ", //ZL_ERR_CP_MTSTR_MALLOC_FAILED
	"\n err: compile memAlloc failure when try to malloc ! (编译器为内存池指针分配内存失败)\n", //ZL_ERR_CP_MP_MALLOC_FAILED
	"\n err: compile memAlloc can't find null point ! (编译器无法在内存池中找到NULL指针，也就无法分配新的指针和内存)\n", //ZL_ERR_CP_MP_MALLOC_NO_NULL_POINT
	"\n err: compile memReAlloc failure when try to realloc ! (编译器为内存池指针重新分配内存失败)\n", //ZL_ERR_CP_MP_REALLOC_FAILED
	"\n syntax error: %d:%d <'%s'> invalid Octal number (语法错误：无效的八进制数，八进制是以0e开头，并且其后的数值是0到7之间的数)\n", //ZL_ERR_CP_INVALID_OCTAL
	"\n syntax error: %d:%d <'%s'> invalid Hex number (语法错误：无效的十六进制，十六进制是以0x开头，并且其后的数值是0到9或a到f或A到F之间的数)\n", //ZL_ERR_CP_INVALID_HEX
	"\n syntax error: %d:%d <'%s'> string %s have no end close char [%c] (语法错误：字符串没有用于结束的单引号或双引号)\n", //ZL_ERR_CP_INVALID_STR_NO_ENDCHAR
	"\n err: makeTokenStrForString alloc failure. (编译器在makeTokenStrForString函数中分配内存失败)\n", //ZL_ERR_CP_MTSFS_MALLOC_FAILED
	"\n syntax error: %d:%d <'%s'> multicomment %s have no end close [*/] (语法错误：多行注释没有正确的结束符)\n", //ZL_ERR_CP_GT_INVALID_MULTI_COMMENT
	"\n err: def_StringPool alloc failure. (编译器异常：为def_StringPool常量宏字符串池分配内存初始化失败)\n", //ZL_ERR_CP_DEF_STR_POOL_MALLOC_FAILED
	"\n err: def_StringPool add str err i out of bounds (编译器异常：将宏相关的字符串信息添加到def_StringPool字符串池时超出字符串数组范围)\n", //ZL_ERR_CP_DEF_STR_POOL_I_OUT_OF_BOUND
	"\n err: def_table alloc failure. (编译器异常：为def_table宏定义动态数组分配内存初始化失败)\n", //ZL_ERR_CP_DEF_TABLE_MALLOC_FAILED
	"\n err: compile LineCols alloc failure (编译器异常：LineCols行列号动态数组分配内存初始化失败)\n", //ZL_ERR_CP_LINECOL_MALLOC_FAILED
	"\n err: compile can't find non-valid index in def_table (编译器异常：添加宏定义时，无法在动态数组中找到没被占用的索引)\n", //ZL_ERR_CP_DEF_TABLE_NO_NONVALID_INDEX
	"\n err: def_table's tmpindex is not valid when insert_HashTableForDef (编译器异常：在函数insert_HashTableForDef中，tmpindex不是有效的索引)\n", //ZL_ERR_CP_DEF_TABLE_INVALID_INDEX
	"\n syntax error: the same def '%s' in %d:%d <'%s'> and %d:%d <'%s'> (语法错误：存在相同的def宏 '%s' 定义，请根据行列号进行检查)\n", //ZL_ERR_CP_DEF_TABLE_SAME_DEF_FOUND
	"\n syntax error: def must with id type! line:%d col:%d <'%s'> (语法错误：def关键字后面必须是有效的id标识符)\n", //ZL_ERR_CP_DEF_MUST_WITH_ID
	"\n syntax error: def must end with semi! line:%d col:%d <'%s'> (语法错误：def定义宏必须以分号结束)\n", //ZL_ERR_CP_DEF_NO_END_SEMI
	"\n syntax error: def const must be int num,float num or string! line:%d col:%d <'%s'> (语法错误：def定义的宏对应的值必须是数字，浮点数，或字符串)\n", //ZL_ERR_CP_DEF_INVALID_VALUE_TYPE
	"\n syntax error: filename <'%s'> is too long (语法错误：文件路径信息太长)\n", //ZL_ERR_CP_INC_FILENAME_TOO_LONG
	"\n syntax error: filename <'%s'> + <'%s'> is too long (语法错误：合并后的文件路径信息太长)\n", //ZL_ERR_CP_INC_FILENAME_TOO_LONG_WHEN_MERGE
	"\n err: compile FileStackList alloc failure (编译器异常：为FileStackList文件加载堆栈分配内存初始化失败)\n", //ZL_ERR_CP_INC_FILESTACKLIST_MALLOC_FAILED
	"\n syntax error: inc must end with semi! line:%d col:%d <'%s'> (语法错误：inc语句必须以分号结束)\n", //ZL_ERR_CP_INC_NO_END_SEMI
	"\n syntax error: inc must with string! line:%d col:%d <'%s'> (语法错误：inc关键字后面必须是字符串类型的文件名或文件相对路径)\n", //ZL_ERR_CP_INC_MUST_WITH_STRING
	"\n err: compile def_table's tmpindex is not valid in lookupDefTable (编译器异常：查找宏定义时，无效的索引值)\n", //ZL_ERR_CP_DEF_TABLE_INVALID_INDEX_WHEN_LOOKUP
	"\n err: compile tokenInfo realloc failure when in ReplaceDefConst. (编译器异常：在进行宏替换时重新分配内存失败)\n", //ZL_ERR_CP_DEF_TABLE_TK_STR_REALLOC_FAILED_WHEN_REPLACE
	"\n err: compile FileStackList overflow (编译器异常：文件加载堆栈发生溢出)\n", //ZL_ERR_CP_INC_FILESTACKLIST_OVERFLOW
	"\n err: compile FileStackList count must >= 0 (编译器异常：FileStackList文件堆栈的元素个数必须大于等于0)\n", //ZL_ERR_CP_INC_FILESTACKLIST_INVALID_COUNT
	"\n err: compile AST_nodes alloc failure! (编译器异常：AST抽象语法树分配内存初始化失败)\n", //ZL_ERR_CP_PARSER_AST_MALLOC_FAILED
	"\n err: compile Token_StringPool alloc failure (编译器异常：token字符串池分配内存初始化失败)\n", //ZL_ERR_CP_TK_STR_POOL_MALLOC_FAILED
	"\n err: compile Token_StringPool add string error i out of bounds (编译器异常：Token_StringPool缓存在添加字符串信息时发生严重错误：返回的索引超过缓存范围)\n", //ZL_ERR_CP_TK_STR_POOL_OUT_OF_BOUNDS
	" parser err exit \n", //ZL_ERR_CP_PARSER_ERROR_EXIT
	"\n syntax parser error: parser_curnode out of end (语法分析错误：语法树当前扫描节点parser_curnode超出语法树范围)", //ZL_ERR_CP_PARSER_CURNODE_OUT_OF_END
	"\n err: compile parser oplevel stacklist alloc failure (编译器异常：优先级比较堆栈分配内存初始化失败)\n", //ZL_ERR_CP_PARSER_OPLVL_STACKLIST_MALLOC_FAILED
	"\n syntax error: id must with operate(except '!' or '&') or left bracket or left middle bracket or ')' or ']' or semi (语法错误：id变量标识符后面必须是操作运算符(除了取反，引用和负号运算符)或者左括号或者左中括号或右括号或右中括号或分号)\n", //ZL_ERR_CP_SYNTAX_ID_INVALID_NEXT_NODE
	"\n syntax error: invalid next node (语法错误：当前节点后面是无效节点，可能是缺少分号导致的)\n", //ZL_ERR_CP_SYNTAX_INVALID_NEXT_NODE
	"\n syntax error: +,-,*,/, .... must with id or float or num or string or ++,-- or ! or '(' , if token is '=' can also with '&' (语法错误：表达式操作运算符后面必须是变量，数字，字符串等操作因子或者加加减减或者取反运算符或负号或左括号，如果是赋值语句，后面还可接引用运算符)\n", //ZL_ERR_CP_SYNTAX_EXPRESS_OP_INVALID_NEXT_NODE
	"\n syntax error: can't start with %s (语法错误：开头不可以用'%s')\n", //ZL_ERR_CP_SYNTAX_START_INVALID_TOKEN
	"\n err: compile AST_TreeScanStackList alloc failure (编译器异常：语法树的扫描堆栈分配内存初始化失败)\n", //ZL_ERR_CP_AST_SCAN_STACK_MALLOC_FAILED
	"\n err: compile AST_TreeScanStackList have no element (编译器异常：语法树的扫描堆栈为空)\n", //ZL_ERR_CP_AST_SCAN_STACK_HAS_NOTHING
	"\n syntax error: num,float,string must with operate(except assign or dot or '!' or '&') or ')' or ']' or semi (语法错误：数字,浮点数,字符串后面必须是除了赋值，点，取反，引用，负号之外的操作运算符或者右括号或者右中括号或者分号)\n", //ZL_ERR_CP_SYNTAX_NUM_FLOAT_STR_INVALID_NEXT_NODE
	"\n syntax error: %d:%d <'%s'> invalid token '%s' (语法错误：无效的Token)\n", //ZL_ERR_CP_SYNTAX_INVALID_TOKEN
	"\n syntax error: '&' must with id (语法错误：引用运算符后面必须是变量之类标识符)\n", //ZL_ERR_CP_SYNTAX_ADDRESS_INVALID_NEXT_NODE
	"\n syntax error: invalid token after ++,-- (语法错误：加加减减运算符后面遇到无效的符号，当加加减减在左侧时，后面只能是加加减减，操作因子，取反，负号或左括号，在右侧时，只能是除了赋值，点，取反，引用，负号运算符之外的操作运算符或者右括号或者右中括号或者分号)\n", //ZL_ERR_CP_SYNTAX_PPMM_INVALID_NEXT_NODE
	"\n syntax error: '(' must with id or float or num or string or ++,-- or ! or '(' or ')' or '&' (语法错误：左括号后面必须是id变量标识符，浮点数，数字，字符串或加加减减，或取反运算符，或负号，或者左括号，或者右括号，还可以接引用运算符)\n", //ZL_ERR_CP_SYNTAX_LBRACKET_INVALID_NEXT_NODE
	"\n syntax error: ')' must with operate(except assign or dot or '!' or '&') or ')' or ']' or semi (语法错误：右括号后面必须是除了赋值，点，取反，负号，引用之外的操作运算符或者右括号或者右中括号或者分号)\n", //ZL_ERR_CP_SYNTAX_RBRACKET_INVALID_NEXT_NODE
	"\n syntax error: bracket can't empty except funcall (语法错误：普通的括号表达式不能为空，除非是函数的参数括号，因为函数可以没有参数)\n", //ZL_ERR_CP_SYNTAX_BRACKET_EMPTY
	"\n syntax error: ')' have no '(' (语法错误：右括号没有对应的左括号)\n", //ZL_ERR_CP_SYNTAX_RBRACKET_NO_LBRACKET
	"\n syntax error: '(' have no ')' (语法错误：左括号没有对应的右括号)\n", //ZL_ERR_CP_SYNTAX_LBRACKET_NO_RBRACKET
	"\n syntax error: unknown state in parser express , may be can't analysis token (语法错误：在parser生成语法树的express函数中遇到无效的状态机，可能是还无法解析的token)\n", //ZL_ERR_CP_SYNTAX_PARSER_EXPRESS_UNKNOWN_STATE
	"\n syntax error: '[' must with id or float or num or string or ++,-- or ! or '(' or ']' (语法错误：左中括号后面必须是id变量标识符，浮点数，数字，字符串或加加减减，或取反运算符，或负号，或者左括号，或者右中括号)\n", //ZL_ERR_CP_SYNTAX_LMBRACKET_INVALID_NEXT_NODE
	"\n syntax error: invalid array item (语法错误：无效的数组元素格式，数组元素必须是test[a]这样的变量标识符加左中括号加表达式加右中括号的格式，其中表达式可以为空)\n", //ZL_ERR_CP_SYNTAX_INVALID_ARRAY_ITEM
	"\n syntax error: ']' have no '[' (语法错误：右中括号没有对应的左括号)\n", //ZL_ERR_CP_SYNTAX_RMBRACKET_NO_LMBRACKET
	"\n syntax error: '[' have no ']' (语法错误：左中括号没有对应的右中括号)\n", //ZL_ERR_CP_SYNTAX_LMBRACKET_NO_RMBRACKET
	"\n syntax error: ']' must with operate(except '!' or '&') or ')' or ']' or semi (语法错误：右中括号后面必须是除了取反，负号，引用之外的操作运算符或者右括号或者右中括号或者分号)\n", //ZL_ERR_CP_SYNTAX_RMBRACKET_INVALID_NEXT_NODE
	"\n syntax error: ',' must with id or float or num or string or ++,-- or ! or '(' or '&' (语法错误：逗号运算符后面必须是id变量标识符，浮点数，数字，字符串或加加减减，或取反运算符，或者左括号，或者引用运算符)\n", //ZL_ERR_CP_SYNTAX_COMMA_INVALID_NEXT_NODE
	"\n syntax error: '.' must with id (语法错误：点运算符后面必须是id变量标识符)\n", //ZL_ERR_CP_SYNTAX_DOT_INVALID_NEXT_NODE
	"\n syntax error: ':' must with id or float or num or string or ++,-- or ! or '(' or '&' (语法错误：冒号运算符后面必须是id变量标识符，浮点数，数字，字符串或加加减减，或取反运算符，或负号，或者左括号，或者引用运算符)\n", //ZL_ERR_CP_SYNTAX_COLON_INVALID_NEXT_NODE
	"\n syntax error: '?' must with id or float or num or string or ++,-- or ! or '(' or '&' (语法错误：问号运算符后面必须是id变量标识符，浮点数，数字，字符串或加加减减，或取反运算符，或负号，或者左括号，或者引用运算符)\n", //ZL_ERR_CP_SYNTAX_QUESTION_MARK_INVALID_NEXT_NODE
	"\n syntax error: '?' have no match colon in the right (语法错误：问号右侧没有相匹配的冒号，一个问号必须和一个冒号相对应)\n", //ZL_ERR_CP_SYNTAX_QUESTION_NO_COLON_RIGHT
	"\n err: compile parser err, zengl_CheckQstColonValid function can only detect question token (编译异常：zengl_CheckQstColonValid只能针对问号运算符进行检测)\n", //ZL_ERR_CP_CHECK_QUESTION_VALID_FUNC_INVALID_TOKEN
	"\n syntax error: ':' have no match '?' in the left (语法错误：冒号左侧没有相匹配的问号，一个冒号必须和一个问号相对应)\n", //ZL_ERR_CP_SYNTAX_COLON_NO_QUESTION_LEFT
	"\n syntax error: if or elif statement with no left bracket (语法错误：if或elif判断表达式没有左括号)\n", //ZL_ERR_CP_SYNTAX_IF_NO_LBRACKET
	"\n err: compile parser err, zengl_detectSetRBracketToSemi function must start with left bracket token (编译异常：zengl_detectSetRBracketToSemi函数必须从左括号的token开始检测)\n", //ZL_ERR_CP_DETECT_SET_RBRACKET_TO_SEMI_FUNC_INVALID_START_TOKEN
	"\n syntax error: elif must use before else (语法错误：elif必须用在else之前)\n", //ZL_ERR_CP_SYNTAX_ELIF_MUST_BEFORE_ELSE
	"\n syntax error: else use more than one (语法错误：一个if结构里只允许有一个else，该错误出现原因可能是之前已经用过一个else了)\n", //ZL_ERR_CP_SYNTAX_ELSE_MORE_THAN_ONE
	"\n syntax error: unknown keyword in if structure (语法错误：在if结构中遇到无效的关键字)\n", //ZL_ERR_CP_SYNTAX_UNKNOWN_KEYWORD_INIF
	"\n syntax error: unknown keyword or the keyword can't use in start (语法错误：无效的关键字，或者是该关键字不能用在开头)\n", //ZL_ERR_CP_SYNTAX_UNKNOWN_KEYWORD
	"\n syntax error: if or inner if have no match endif (语法错误：if或内嵌的if语句没有匹配的endif)\n", //ZL_ERR_CP_SYNTAX_IF_NO_ENDIF
	"\n syntax error: for have no match endfor (语法错误：for没有对应的endfor)\n", //ZL_ERR_CP_SYNTAX_FOR_NO_ENDFOR
	"\n syntax error: for statement with no left bracket (语法错误：for条件判断表达式没有左括号)\n", //ZL_ERR_CP_SYNTAX_FOR_NO_LBRACKET
	"\n syntax error: for statement must have thress express (语法错误：for条件判断表达式必须由三个子表达式组成例如for(i=1;i<=3;i++)这样的形式，三个表达式可以为空，但是不可以缺少中间的两个分号，此错误也可能是缺少for条件判断表达式的右括号造成)\n", //ZL_ERR_CP_SYNTAX_FOR_STMT_MUST_HAVE_THREE_EXP
	"\n syntax error: it's not endfor (语法错误：for必须以endfor结束)\n", //ZL_ERR_CP_SYNTAX_ITS_NOT_ENDFOR
	"\n syntax error: fun...endfun can't contain another fun (语法错误：一个fun函数中不可以包含另一个fun的定义)\n", //ZL_ERR_CP_SYNTAX_FUN_CONTAIN_ANOTHER_FUN
	"\n syntax error: fun have no match endfun (语法错误：fun没有对应的endfun)\n", //ZL_ERR_CP_SYNTAX_FUN_NO_ENDFUN
	"\n syntax error: fun arg list with no left bracket (语法错误：fun函数参数没有左括号)\n", //ZL_ERR_CP_SYNTAX_FUN_NO_LBRACKET
	"\n syntax error: fun arg list invalid token (语法错误：fun函数参数列表遇到无效的token)\n", //ZL_ERR_CP_SYNTAX_FUN_ARGLIST_INVALID_TOKEN
	"\n syntax error: fun have no fun name (语法错误：fun函数定义没有函数名)\n", //ZL_ERR_CP_SYNTAX_FUN_NO_NAME
	"\n syntax error: it's not endfun (语法错误：fun必须以endfun结束)\n", //ZL_ERR_CP_SYNTAX_ITS_NOT_ENDFUN
	"\n syntax error: invalid express after global reserve (语法错误：global关键字后面是无效的表达式，global后面的表达式只能包含变量标识符和逗号，并以分号结束)\n", //ZL_ERR_CP_SYNTAX_INVALID_EXP_AFTER_GLOBAL
	"\n syntax error: invalid express after use reserve (语法错误：use关键字后面是无效的表达式，use后面的表达式只能包含变量标识符和逗号，并以分号结束)\n", //ZL_ERR_CP_SYNTAX_INVALID_EXP_AFTER_USE
	"\n syntax error: invalid break express (语法错误：无效的break语句，break后面应该直接以分号结束)\n", //ZL_ERR_CP_SYNTAX_INVALID_BREAK_EXP
	"\n syntax error: invalid continue express (语法错误：无效的continue语句，continue后面应该直接以分号结束)\n", //ZL_ERR_CP_SYNTAX_INVALID_CONTINUE_EXP
	"\n syntax error: switch have no match endswt or endswitch (语法错误：switch没有对应的endswt或者endswitch)\n", //ZL_ERR_CP_SYNTAX_SWITCH_NO_ENDSWT
	"\n syntax error: switch statement with no left bracket (语法错误：switch条件判断表达式没有左括号)\n", //ZL_ERR_CP_SYNTAX_SWITCH_NO_LBRACKET
	"\n syntax error: switch statement case must with num or float or string (语法错误：switch里的case后面必须是数字，浮点数或字符串)\n", //ZL_ERR_CP_SYNTAX_CASE_MUST_WITH_NUM_FLOAT_STR
	"\n syntax error: switch statement case must end with colon (语法错误：switch里的case后面接数字，浮点数，字符串，并且必须以冒号结束，不可以接表达式)\n", //ZL_ERR_CP_SYNTAX_CASE_MUST_END_WITH_COLON
	"\n syntax error: switch statement case must before default (语法错误：case必须用在default之前)\n", //ZL_ERR_CP_SYNTAX_CASE_MUST_BEFORE_DEFAULT
	"\n syntax error: switch statement default used more than once (语法错误：switch结构中只能有一个default)\n", //ZL_ERR_CP_SYNTAX_DEFAULT_USE_MORE_THAN_ONE
	"\n syntax error: switch statement default must end with colon (语法错误：switch里的default后面必须直接以冒号结束)\n", //ZL_ERR_CP_SYNTAX_DEFAULT_MUST_END_WITH_COLON
	"\n syntax error: unknown keyword in switch structure (语法错误：在switch结构中遇到无效的关键字)\n", //ZL_ERR_CP_SYNTAX_UNKNOWN_KEYWORD_INSWITCH
	"\n syntax error: invalid member in class structure (语法错误：class结构中包含无效的成员)\n", //ZL_ERR_CP_SYNTAX_INVALID_MEMBER_INCLASS
	"\n syntax error: class have no match endclass or endcls (语法错误：class没有匹配的endclass或endcls)\n", //ZL_ERR_CP_SYNTAX_CLASS_NO_ENDCLS
	"\n syntax error: no class name in class define (语法错误：class类定义中没有类名，类名必须紧跟在class关键字后面)\n", //ZL_ERR_CP_SYNTAX_CLASS_NO_CLASS_NAME
	"\n syntax error: it's not endcls or endclass (语法错误：class必须以endcls或endclass结束)\n", //ZL_ERR_CP_SYNTAX_ITS_NOT_ENDCLS
	"\n syntax error: while or inner while have no match endwhile (语法错误：while或内嵌的while没有对应的endwhile)\n", //ZL_ERR_CP_SYNTAX_WHILE_NO_ENDWHILE
	"\n syntax error: while with no left bracket (语法错误：while条件判断表达式没有左括号)\n", //ZL_ERR_CP_SYNTAX_WHILE_NO_LBRACKET
	"\n syntax error: it's not endwhile (语法错误：while必须以endwhile结束)\n", //ZL_ERR_CP_SYNTAX_ITS_NOT_ENDWHILE
	"\n syntax error: do have no match dowhile (语法错误：do没有对应的dowhile)\n", //ZL_ERR_CP_SYNTAX_DO_NO_DOWHILE
	"\n syntax error: it's not dowhile (语法错误：do必须以dowhile结束)\n", //ZL_ERR_CP_SYNTAX_ITS_NOT_DOWHILE
	"\n syntax error: dowhile with no left bracket (语法错误：dowhile条件判断表达式没有左括号)\n", //ZL_ERR_CP_SYNTAX_DOWHILE_NO_LBRACKET
	"\n syntax error: invalid express after class statement (语法错误：无效的类声明语句，类声明语句中只能包含变量标识符和逗号，并以分号结束)\n", //ZL_ERR_CP_SYNTAX_INVALID_EXP_AFTER_CLASS_STATEMENT
	"\n syntax error: line:%d,col:%d <'%s'> %s detect unexpect keyword %s (语法错误：%s检测到不期望的关键字%s)\n", //ZL_ERR_CP_SYNTAX_KEYWORD_DETECT_UNEXPECT_KEYWORD
	"\n syntax error: unexpect end keyword %s (语法错误：不期望的结束关键字%s)\n", //ZL_ERR_CP_SYNTAX_UNEXPECT_ENDKEYWORD
	"\n syntax error: unexpect location of use (语法错误：use关键字出现在不期望的位置，use只能用在全局范围内，且不能出现在if之类的控制结构或fun函数中)\n", //ZL_ERR_CP_SYNTAX_UNEXPECT_LOC_OF_USE
	"\n syntax error: unexpect location of global (语法错误：global关键字出现在不期望的位置，请在函数中使用global，且global不要放在if之类的控制结构中)\n", //ZL_ERR_CP_SYNTAX_UNEXPECT_LOC_OF_GLOBAL
	"\n syntax error: unexpect location of return (语法错误：return关键字出现在不期望的位置，请用在函数中)\n", //ZL_ERR_CP_SYNTAX_UNEXPECT_LOC_OF_RETURN
	"\n syntax error: unexpect location of break or continue (语法错误：break或continue必须用在while,do...dowhile,for,switch结构中)\n", //ZL_ERR_CP_SYNTAX_UNEXPECT_LOC_OF_BREAK_CONTINUE
	"\n syntax error: invalid class function call (语法错误：调用类函数格式错误)\n", //ZL_ERR_CP_SYNTAX_INVALID_CLASS_FUNCALL
	"\n err: compile symbol err , SymGlobalTable alloc failure in zengl_SymInitGlobalTable (编译器异常：在zengl_SymInitGlobalTable函数中为SymGlobalTable全局变量符号表分配内存初始化失败)\n", //ZL_ERR_CP_SYM_GLOBAL_TABLE_MALLOC_FAILED
	"\n err: compile symbol err , zengl_SymInsertGlobalTable function can't find non-valid index in SymGlobalTable (编译器异常：zengl_SymInsertGlobalTable函数无法在全局变量符号表中找到没被占用的索引)\n", //ZL_ERR_CP_SYM_GLOBAL_TABLE_NO_NOT_VALID_INDEX
	"\n err: compile symbol err , zengl_SymInsertHashTableForGlobal function SymGlobalTable's tmpindex is not valid (编译器异常：zengl_SymInsertHashTableForGlobal函数在SymGlobalTable符号表中找到的tmpindex是无效的)\n", //ZL_ERR_CP_SYM_GLOBAL_TABLE_FIND_NOT_VALID_INDEX
	"\n err: compile symbol err , zengl_SymInsertHashTableForGlobal function SymGlobalTable's tmpindex's linecols not init (编译器异常：tmpindex对应的符号的行列号没有初始化)\n", //ZL_ERR_CP_SYM_GLOBAL_TABLE_INDEX_LINECOL_NOT_INIT
	"\n err: compile symbol err , zengl_SymPrintTables function print SymGlobalTable[%d] has no linecols (编译器异常：SymGlobalTable中的[%d]索引对应的符号没有行列号信息)\n", //ZL_ERR_CP_SYM_GLOBAL_TABLE_INDEX_HAVE_NO_LINECOL_WHEN_PRINT
	"\n err: compile symbol err , SymClassTable alloc failure in zengl_SymInitClassTable (编译器异常：在zengl_SymInitClassTable函数中为SymClassTable类符号表对应的动态数组分配内存初始化失败)\n", //ZL_ERR_CP_SYM_CLASS_TABLE_MALLOC_FAILED
	"\n err: compile symbol err , zengl_SymInsertClassTable function can't find non-valid index in SymClassTable (编译器异常：zengl_SymInsertClassTable函数无法在类符号表中找到没被占用的索引)\n",//ZL_ERR_CP_SYM_CLASS_TABLE_NO_NOT_VALID_INDEX
	"\n err: compile symbol err , zengl_SymInsertHashTableForClass function SymClassTable's tmpindex is not valid (编译器异常：zengl_SymInsertHashTableForClass函数在SymClassTable类符号表中找到的tmpindex是无效的)\n",	//ZL_ERR_CP_SYM_CLASS_TABLE_FIND_NOT_VALID_INDEX
	"\n syntax error: class %s has been defined in line:%d,col:%d <'%s'> (%s类已经被定义过了)\n", //ZL_ERR_CP_SYM_CLASS_HAS_BEEN_DEFINED
	"\n err: compile symbol err , zengl_SymPrintTables function print SymClassTable[%d] has no linecols (编译器异常：SymClassTable类符号表中的[%d]索引对应的类定义中没有行列号信息)\n", //ZL_ERR_CP_SYM_CLASS_TABLE_INDEX_HAVE_NO_LINECOL_WHEN_PRINT
	"\n err: compile symbol err , SymFunTable alloc failure in zengl_SymInitFunTable (编译器异常：在zengl_SymInitFunTable函数中为SymFunTable函数符号表分配内存初始化失败)\n", //ZL_ERR_CP_SYM_FUN_TABLE_MALLOC_FAILED
	"\n err: compile symbol err , zengl_SymInsertFunTable function can't find non-valid index in SymFunTable (编译器异常：zengl_SymInsertFunTable函数无法在函数符号表中找到没被占用的索引)\n", //ZL_ERR_CP_SYM_FUN_TABLE_NO_NOT_VALID_INDEX
	"\n err: compile symbol err , zengl_SymInsertHashTableForFun function SymFunTable's tmpindex is not valid (编译器异常：zengl_SymInsertHashTableForFun函数在SymFunTable函数表中找到的tmpindex是无效的)\n", //ZL_ERR_CP_SYM_FUN_TABLE_FIND_NOT_VALID_INDEX
	"\n syntax error: fun %s has been defined in line:%d,col:%d <'%s'> (%s脚本函数已经被定义过了)\n", //ZL_ERR_CP_SYM_FUN_HAS_BEEN_DEFINED
	"\n err: compile symbol err , zengl_SymPrintTables function print SymFunTable[%d] has no linecols (编译器异常：SymFunTable函数符号表中的[%d]索引对应的函数定义中没有行列号信息)\n", //ZL_ERR_CP_SYM_FUN_TABLE_INDEX_HAVE_NO_LINECOL_WHEN_PRINT
	"\n syntax error: class name '%s' does not exists (语法错误：类名'%s'不存在)\n", //ZL_ERR_CP_SYNTAX_CLASS_NAME_OF_CLS_STMT_NOT_EXISTS
	"\n err: compile symbol err , zengl_SymLookupClass function SymClassTable's tmpindex is not valid (编译器异常：zengl_SymLookupClass函数在SymClassTable类符号表中找到的tmpindex是无效的索引)\n", //ZL_ERR_CP_SYM_CLASS_TABLE_FIND_NOT_VALID_INDEX_WHEN_LOOKUP
	"\n err: compile symbol err , SymClassMemberTable alloc failure in zengl_SymInitClassMemberTable (编译器异常：在zengl_SymInitClassMemberTable为SymClassMemberTable类成员符号表动态数组分配内存初始化失败)\n", //ZL_ERR_CP_SYM_CLASSMEMBER_TABLE_MALLOC_FAILED
	"\n err: compile symbol err , zengl_SymInsertClassMemberTable function can't find non-valid index in SymClassMemberTable (编译器异常：zengl_SymInsertClassMemberTable函数无法在类成员符号表中找到没被占用的索引)\n", //ZL_ERR_CP_SYM_CLASSMEMBER_TABLE_NO_NOT_VALID_INDEX
	"\n err: compile symbol err , zengl_SymInsertHashTableForClassMember function SymClassMemberTable's tmpindex is not valid (编译器异常：zengl_SymInsertHashTableForClassMember函数在SymClassMemberTable类成员符号表中找到的tmpindex是无效的)\n", //ZL_ERR_CP_SYM_CLASSMEMBER_TABLE_FIND_NOT_VALID_INDEX
	"\n syntax error: class member '%s' of class '%s' has been defined in line:%d,col:%d <'%s'> (%s类成员已经被定义过了)\n", //ZL_ERR_CP_SYNTAX_CLASSMEMBER_HAS_BEEN_DEFINED
	"\n syntax error: assign node must have 2 child-nodes (语法错误：赋值节点必须有两个子节点)\n", //ZL_ERR_CP_SYNTAX_ASM_ASSIGN_MUST_HAVE_TWO_CHILD
	"\n err: compile symbol err , can't find '%s' memloc in SymGlobalTable (编译器异常：无法找到'%s'变量的内存地址)\n", //ZL_ERR_CP_SYM_CAN_NOT_FIND_ID_MEMLOC
	"\n err: compile symbol err , zengl_SymLookupID function SymGlobalTable's tmpindex is not valid (编译器异常：zengl_SymLookupID函数在全局符号表中找到的tmpindex是无效的索引)\n", //ZL_ERR_CP_SYM_GLOBAL_TABLE_FIND_NOT_VALID_INDEX_WHEN_LOOKUP
	"\n syntax error: assign second child node is not id or num float or sring or express (语法错误：赋值运算符第二个子节点既不是变量或数字或浮点数或字符串，也不是加减乘除等表达式)\n", //ZL_ERR_CP_SYNTAX_ASM_ASSIGN_SECOND_NODE_IS_INVALID_TYPE
	"\n syntax error: assign first child node is not valid struct (语法错误：赋值语句的第一个子节点不是有效的结构)\n", //ZL_ERR_CP_SYNTAX_ASM_ASSIGN_FIRST_NODE_IS_INVALID_TYPE
	" run err exit when in compile \n", //ZL_ERR_RUN_ERROR_EXIT_WHEN_IN_COMPILE
	"\n err: run func err , mempool alloc failure when in zenglrun_init (解释器异常：为解释器内存池分配内存初始化失败)\n", //ZL_ERR_RUN_MEM_POOL_MALLOC_FAILED
	"\n err: run func err , memAlloc function failed (解释器异常：memAlloc函数分配内存失败)\n", //ZL_ERR_RUN_MEM_ALLOC_FUNC_FAILED
	"\n err: run func err , memAlloc function can't find null point ! (解释器异常：memAlloc函数无法在内存池中找到NULL指针，也就无法分配新的指针)\n", //ZL_ERR_RUN_MEM_ALLOC_FUNC_CAN_NOT_FIND_NULL_POINT
	"\n err: run func err , memfreepool alloc failure when in zenglrun_init (解释器异常：为解释器的内存释放池分配内存初始化失败)\n", //ZL_ERR_RUN_MEM_FREE_POOL_MALLOC_FAILED
	"\n err: run main err , inst_list alloc failure when in zenglrun_initInstList (解释器异常：为解释器的汇编指令集分配内存初始化失败)\n", //ZL_ERR_RUN_INST_LIST_MALLOC_FAILED
	"\n err: run func err , memReAlloc the point is not valid (解释器异常：memReAlloc时，需要调整的指针在内存池中无效)\n", //ZL_ERR_RUN_MEM_RE_ALLOC_INVALID_POINT
	"\n err: run func err , memReAlloc function failed (解释器异常：memReAlloc函数重新分配内存失败)\n", //ZL_ERR_RUN_MEM_RE_ALLOC_FUNC_FAILED
	"\n err: run main err , inst are not valid or pc:%d != PC:%d (解释器异常：指令的PC值和PC寄存器里的值不一致)\n", //ZL_ERR_RUN_INST_INVALID_PC
	"\n err: run func err , InstData_StringPool alloc failure in zenglrun_initInstDataStringPool(解释器异常：在zenglrun_initInstDataStringPool函数中为指令操作数字符串池分配内存初始化失败)\n", //ZL_ERR_RUN_INST_DATA_STRING_POOL_MALLOC_FAILED
	"\n err: run func err , i out of bounds when InstData_StringPool add string (解释器异常：当指令操作数字符串池添加字符串时索引i超出范围)\n", //ZL_ERR_RUN_INST_DATA_STR_POOL_ADD_I_OUT_OF_BOUNDS
	"\n err: run main err , vmem_list alloc failure in zenglrun_initVMemList (解释器异常：为全局虚拟内存动态数组分配内存初始化失败)\n", //ZL_ERR_RUN_VMEM_LIST_MALLOC_FAILED
	"\n err: run func err , zengl_memReUsePtr alloc failure (解释器异常：重利用内存池指针时，调整分配系统内存失败)\n", //ZL_ERR_RUN_MEM_RE_USE_PTR_MALLOC_FAILED
	"\n err: run main err , invalid virtual mem type , pc=%d (解释器异常：无效的虚拟内存类型)\n", //ZL_ERR_RUN_INVALID_VMEM_TYPE
	"\n err: run main err , invalid inst src data type , pc=%d (解释器异常：指令中源操作数类型无效)\n", //ZL_ERR_RUN_INVALID_INST_SRC_TYPE
	"\n err: run main err , invalid inst dest data type , pc=%d (解释器异常：指令中目标操作数类型无效)\n", //ZL_ERR_RUN_INVALID_INST_DEST_TYPE
	"\n err: run main err , invalid inst type , pc=%d (解释器异常：无效的汇编指令)\n", //ZL_ERR_RUN_INVALID_INST_TYPE
	"\n syntax error: invalid token can't generate assemble codes (语法错误：无效的token，该token目前无法生成对应的汇编指令)\n", //ZL_ERR_CP_SYNTAX_ASM_INVALID_TOKEN_CAN_NOT_GEN_CODE
	"\n syntax error: print node must have 1 child-node (语法错误：打印节点必须包含一个子节点)\n", //ZL_ERR_CP_SYNTAX_ASM_PRINT_MUST_HAVE_ONE_CHILD
	"\n syntax error: current node must have 2 child-node (语法错误：当前节点必须包含两个子节点)\n", //ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_TWO_CHILDS
	"\n syatax error: invalid child node type [%s line:%d,col:%d <'%s'>] (语法错误：遇到无效的子节点类型)\n", //ZL_ERR_CP_SYNTAX_ASM_INVALID_CHILD_NODE_TYPE
	"\n err: run main err , vstack_list alloc failure in zenglrun_initVStackList (解释器异常：为虚拟堆栈动态数组分配内存初始化失败)\n", //ZL_ERR_RUN_VSTACK_LIST_MALLOC_FAILED
	"\n err: run main err , vstack_list overflow vstack_list count less then zero pc:%d (解释器异常：虚拟堆栈反向溢出，栈顶计数器小于0，当前PC寄存器值：%d)\n", //ZL_ERR_RUN_VSTACK_LIST_OVERFLOW
	"\n err: run main err , modulo by zero , pc=%d (解释器运行时错误：取余运算时，发生除零错误)\n", //ZL_ERR_RUN_MOD_BY_ZERO
	"\n err: run main err , division by zero , pc=%d (解释器运行时错误：除法运算时，发生除零错误)\n", //ZL_ERR_RUN_DIVIDE_BY_ZERO
	"\n source code info: [ %s ] %d:%d <'%s'>\n", //ZL_ERR_RUN_SOURCE_CODE_INFO
	"\n err: compile assemble err , AsmGCStackList alloc failure in zengl_AsmGCStackInit (编译器异常：汇编堆栈动态数组分配内存初始化失败)\n", //ZL_ERR_CP_ASM_GCSTACK_LIST_MALLOC_FAILED
	"\n err: compile assemble err , AsmGCLoopStackList alloc failure in zengl_AsmGCLoopStackInit (编译器异常：汇编模拟堆栈动态数组分配内存初始化失败)\n", //ZL_ERR_CP_ASM_GC_LOOP_STACK_LIST_MALLOC_FAILED
	"\n err: compile ld err , LDAddrList alloc failure in zengl_LDAddrListInit (编译器异常：链接地址动态数组分配内存初始化失败)\n", //ZL_ERR_CP_LD_ADDR_LIST_MALLOC_FAILED
	"\n err: compile ld err , LDAddrList index loc must unique pc:%d (编译器异常：链接地址必须唯一，一个伪地址对应一个汇编代码位置)\n", //ZL_ERR_CP_LD_INDEX_ADDR_MUST_UNIQUE
	"\n err: compile ld err , invalid LDAddrList index : adr%d (编译器异常：无效的伪地址:adr%d)\n", //ZL_ERR_CP_LD_INVALID_ADDR_INDEX
	"\n syntax error: current node must have 1 child-node (语法错误：当前节点必须有且只有一个子节点)\n", //ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_ONE_CHILD
	"\n syntax error: current node must have at least 2 child-node (语法错误：当前节点必须至少有两个子节点)\n", //ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_AT_LEAST_TWO_CHILDS
	"\n syntax error: invalid op factor (语法错误：无效的操作因子)\n", //ZL_ERR_CP_SYNTAX_ASM_INVALID_OP_FACTOR
	"\n syntax error: current node must have 4 child-nodes (语法错误：当前节点必须有且只有四个子节点)\n", //ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_FOUR_CHILDS
	"\n syntax error: current node must have 3 child-nodes (语法错误：当前节点必须有且只有三个子节点)\n", //ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_THREE_CHILDS
	"\n syntax error: fun can't be defined in other fun (语法错误：在一个函数体内不能再定义另一个函数)\n",//ZL_ERR_CP_SYNTAX_ASM_FUN_CAN_NOT_DEFINED_IN_OTHER_FUN
	"\n err: compile symbol err , AST_TreeScanStackList invalid count (编译器异常：AST扫描堆栈里的元素个数小于0)\n", //ZL_ERR_CP_SYM_AST_TREE_SCAN_STACK_INVALID_COUNT
	"\n err: compile symbol err , AST_TreeScanStackList are not empty (编译器异常：AST扫描堆栈不为空)\n", //ZL_ERR_CP_SYM_AST_TREE_SCAN_STACK_NOT_EMPTY
	"\n err: compile symbol err , SymLocalTable alloc failure (编译器异常：局部变量符号表动态数组分配内存初始化失败)\n", //ZL_ERR_CP_SYM_LOCAL_TABLE_MALLOC_FAILED
	"\n err: compile symbol err , SymLocalTable can't find non-valid index when in zengl_SymInsertLocalTable (编译器异常：局部符号表无法找到没被占用的索引，无法完成符号插入操作)\n", //ZL_ERR_CP_SYM_LOCAL_TABLE_CAN_NOT_FIND_NOT_VALID_INDEX_WHEN_INSERT
	"\n err: compile symbol err , SymLocalTable invalid index (编译器异常：局部符号表遇到无效索引)\n", //ZL_ERR_CP_SYM_LOCAL_TABLE_INVALID_INDEX
	"\n syntax error: fun global reserve must have child node! (语法错误：global后面不可以为空节点即空语句)\n", //ZL_ERR_CP_SYNTAX_SYM_GLOBAL_MUST_HAVE_CHILD_NODE
	"\n err: compile symbol err , can't find '%s' memloc in SymLocalTable (编译器异常：无法找到'%s'变量的内存地址)\n", //ZL_ERR_CP_SYM_CAN_NOT_FIND_LOCAL_ID_MEMLOC
	"\n err: compile symbol err , invalid funid in zengl_SymLookupID (编译器异常：局部变量符号信息对应的函数ID无效)\n", //ZL_ERR_CP_SYM_LOCAL_ID_INVALID_FUN_ID
	"\n err: compile symbol err , invalid local type in zengl_SymLookupID (编译器异常：无效的局部符号类型)\n", //ZL_ERR_CP_SYM_LOCAL_ID_INVALID_TYPE
	"\n err: run main err , fun return address must int pc=%d (解释器运行时错误：函数的汇编返回地址必须是整数)\n", //ZL_ERR_RUN_MAIN_INVALID_FUN_RET_ADDRESS
	"\n err: run main err , fun virtual stack struct exception pc=%d (解释器运行时错误：函数的虚拟栈结构异常)\n", //ZL_ERR_RUN_MAIN_FUN_VSTACK_STRUCT_EXCEPTION
	"\n syntax error: use reserve must have child node! (语法错误：use关键字后面不可以为空节点即空语句)\n", //ZL_ERR_CP_SYNTAX_SYM_USE_MUST_HAVE_CHILD_NODE
	"\n err: run func err , moduleTable alloc failure when in zenglrun_initModuleTable (解释器异常：模块动态数组分配内存初始化失败)\n", //ZL_ERR_RUN_MODULE_TALBE_MALLOC_FAILED
	"\n err: run func err , moduleTable can't find non-valid index (解释器异常：模块动态数组中无法找到没被占用的索引)\n", //ZL_ERR_RUN_MODULE_TALBE_NO_NOT_VALID_INDEX
	"\n err: VM Api err , moduleTable find invalid index (解释器异常：模块动态数组中遇到无效的索引)\n", //ZL_ERR_RUN_MODULE_TABLE_FIND_INVALID_INDEX
	"\n err: VM Api err , module '%s' has been set handle before (解释器运行时错误：模块'%s'已经被设置过处理句柄了)\n", //ZL_ERR_RUN_MODULE_HAS_BEEN_SET_HANDLE_BEFORE
	"\n err: run func err , ModFunTable alloc failure when in zenglrun_initModFunTable (解释器异常：模块函数动态数组分配内存初始化失败)\n", //ZL_ERR_RUN_MOD_FUN_TABLE_MALLOC_FAILED
	"\n err: run func err , ModFunTable can't find non-valid index (解释器异常：模块函数动态数组中无法找到没被占用的索引)\n", //ZL_ERR_RUN_MOD_FUN_TABLE_NO_NOT_VALID_INDEX
	"\n err: VM Api err , ModFunTable find invalid index (解释器异常：模块函数动态数组中遇到无效的索引)\n", //ZL_ERR_RUN_MOD_FUN_TABLE_FIND_INVALID_INDEX
	"\n err: VM Api err , function '%s' has been set handle in module '%s' before (解释器运行时错误：模块函数'%s'已经在'%s'模块中设置过处理句柄了)\n", //ZL_ERR_RUN_MOD_FUN_HAS_BEEN_SET_HANDLE_IN_OTHER_MOD_BEFORE
	"\n err: run func err , module '%s' is invalid pc=%d (解释器运行时错误：模块'%s'无效，可能原因是用户没有定义该模块的Use处理句柄)\n", //ZL_ERR_RUN_MODULE_IS_INVALID
	"\n err: run func err , function '%s' is invalid pc=%d (解释器运行时错误：函数'%s'无效)\n", //ZL_ERR_RUN_FUNCTION_IS_INVALID
	"\n err: VM Api err , zenglApi_GetFunArg...'s argnum is invalid pc=%d (解释器运行时错误：GetFunArg...接口的argnum参数无效，不可小于1，也不可大于参数总数)\n", //ZL_ERR_VM_API_GET_FUN_ARG_ARGNUM_IS_INVALID
	"\n user defined error: ", //ZL_ERR_VM_API_USER_DEFINED_ERROR
	"\n err: VM Api err , invalid function name when in zenglApi_Call(解释器运行时错误：Call接口中的函数名无效)\n", //ZL_ERR_VM_API_INVALID_FUNCTION_NAME
	"\n err: VM Api err , invalid script name when (解释器运行时错误：API接口中的脚本名无效)\n", //ZL_ERR_VM_API_INVALID_SCRIPT_NAME
	"\n err: VM Api err , VM has been used before please reset it (解释器运行时错误：API接口中的虚拟机之前被用过了，请使用Reset接口进行重置)\n", //ZL_ERR_VM_API_VM_MUST_RESET
	"\n err: VM Api err , GetValueAs... can only use in debug mode (解释器运行时错误：GetValueAs...之类的接口只能用在DEBUG调试模式下)\n", //ZL_ERR_VM_API_GET_VALUE_AS_MUST_IN_DEBUG_MODE
	"\n err: VM Api err , GetValueAs... valueName is invalid (解释器运行时错误：GetValueAs...之类的接口中的全局变量名无效，可能不存在于脚本中)\n", //ZL_ERR_VM_API_GET_VALUE_AS_VALUE_NAME_INVALID
	"\n err: VM Api err , SymGlobalTable invalid index (解释器运行时错误：通过接口查找变量名时，全局符号表中发现无效索引)\n", //ZL_ERR_VM_API_SYM_GLOBAL_TABLE_INVALID_INDEX
	"\n err: VM Api err , GetValueAs... value's runtype is invalid (解释器运行时错误：GetValueAs...之类的接口查找到的变量的运行时类型无效，只能是整数，浮点数，字符串类型)\n", //ZL_ERR_VM_API_GET_VALUE_AS_VALUE_RUNTYPE_INVALID
	"\n err: run main err , invalid runtype in ZENGL_RUN_VMEM_OP_GET_BY_RUNTYPE (解释器运行时错误：ZENGL_RUN_VMEM_OP_GET_BY_RUNTYPE宏遇到无效的运行时类型)\n", //ZL_ERR_RUN_INVALID_RUNTYPE_IN_GEN_BY_RUNTYPE_MACRO
	"\n err: run main err , memblock alloc failure (解释器运行时错误：为内存块分配内存初始化失败)\n", //ZL_ERR_RUN_MEM_BLOCK_MALLOC_FAILED
	"\n err: run main err , memblock invalid index , maybe index less then zero or too big (解释器运行时错误：对数组，类成员等内存块进行设置时，访问索引值无效，可能索引值小于0或者超出内存块范围)\n", //ZL_ERR_RUN_MEM_BLOCK_INVALID_INDEX
	"\n err: run main err , memblock virtual stack struct exception pc=%d (解释器运行时错误：内存块的虚拟栈结构异常)\n", //ZL_ERR_RUN_MEM_BLOCK_VSTACK_STRUCT_EXCEPTION
	"\n err: run main err , array or class item is not init (解释器运行时错误：数组或类元素不存在，多半是没有被初始化或者索引值小于0，请使用array函数或直接赋值等方式进行初始化)\n", //ZL_ERR_RUN_ARRAY_CLASS_ITEM_IS_NOT_INIT
	"\n err: VM Api err , SetRetVal invalid type (解释器运行时错误：SetRetVal接口遇到无效的值类型)\n", //ZL_ERR_VM_API_SET_RET_VAL_INVALID_TYPE
	"\n syntax error: ClassStatement must have child node (语法错误：类声明语句后面不可以为空节点即空语句)n", //ZL_ERR_SYNTAX_SYM_CLASS_STATEMENT_MUST_HAVE_CHILD_NODE
	"\n err: compile symbol err , ScanDotForClass classid is 0 but not in init (编译器异常：ScanDotForClass在非初始扫描状态下遇到类ID为0)\n", //ZL_ERR_CP_SYM_SCAN_DOT_FOR_CLASS_ID_ZERO_WHEN_NOT_INIT
	"\n syntax error: value is not a class value (语法错误：该变量不是一个类变量，请使用类声明语句进行声明)\n", //ZL_ERR_SYNTAX_SYM_CAN_NOT_FIND_CLASS_INFO_FOR_ID
	"\n syntax error: invalid class member (语法错误：无效的类成员)\n", //ZL_ERR_SYNTAX_INVALID_CLASS_MEMBER
	"\n syntax error: invalid reserve (语法错误：无效的关键字)\n", //ZL_ERR_CP_SYNTAX_ASM_INVALID_RESERVE
	"\n syntax error: current node must have at least 1 child-node (语法错误：当前节点必须至少有一个子节点)\n", //ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_AT_LEAST_ONE_CHILD
	"\n err: compile assemble err , case jmp table alloc failure (编译器异常：switch...case结构跳转表分配内存初始化失败)\n", //ZL_ERR_CP_ASM_CASE_JMP_TABLE_MALLOC_FAILED
	"\n err: compile assemble err , case jmp table realloc failure (编译器异常：switch...case结构跳转表重新分配内存失败)\n", //ZL_ERR_CP_ASM_CASE_JMP_TABLE_REALLOC_FAILED
	"\n syntax error: switch have same case val (语法错误：switch结构中发现相同的case值)\n", //ZL_ERR_CP_SYNTAX_ASM_SWITCH_FIND_SAME_CASE_VAL
	"\n err: compile assemble err , case jmp table caseAddr can't be 0 (编译器异常：switch...case结构跳转表中case的目标代码位置不能为0)\n", //ZL_ERR_CP_ASM_CASE_JMP_TABLE_CASEADDR_CAN_NOT_BE_ZERO
	"\n err: run main err , SWITCH inst have no LONG inst below (解释器异常：SWITCH指令后面必须是LONG指令)\n", //ZL_ERR_RUN_SWITCH_HAVE_NO_LONG_INST_BELOW
	"\n err: VM Api err , invalid class name when in zenglApi_Call(解释器运行时错误：Call接口中的类名无效)\n", //ZL_ERR_VM_API_INVALID_CLASS_NAME_WHEN_CALL
	"\n err: VM Api err , class table find invalid index when in zenglApi_Call(解释器运行时异常：Call接口调用时找到无效的类索引)", //ZL_ERR_VM_API_INVALID_CLASS_TABLE_INDEX_WHEN_CALL
	"\n err: VM Api err , invalid memblock arg in zenglApi_CreateMemBlock (解释器运行时错误：CreateMemBlock接口中memblock参数无效，不可为空指针)\n", //ZL_ERR_VM_API_INVALID_MEMBLOCK_ARG_IN_CREATE_MEM_BLOCK
	"\n err: VM Api err , invalid size arg in zenglApi_CreateMemBlock (解释器运行时错误：CreateMemBlock接口中size参数无效，size必须大于等于0)\n", //ZL_ERR_VM_API_INVALID_SIZE_ARG_IN_CREATE_MEM_BLOCK
	"\n err: VM Api err , invalid memblock arg in zenglApi_SetRetValAsMemBlock (解释器运行时错误：SetRetValAsMemBlock接口中memblock参数无效，不可为空指针)\n", //ZL_ERR_VM_API_INVALID_MEMBLOCK_ARG_IN_SET_RETVAL
	"\n err: VM Api err , invalid argnum arg in zenglApi_SetFunArg (解释器运行时错误：SetFunArg接口中argnum参数无效)\n", //ZL_ERR_VM_API_INVALID_ARGNUM_ARG_IN_SET_FUN_ARG
	"\n err: VM Api err , invalid memblock arg in zenglApi_GetMemBlockSize (解释器运行时错误：GetMemBlockSize接口中memblock参数无效，不可为空指针)\n", //ZL_ERR_VM_API_INVALID_MEMBLOCK_ARG_IN_GET_MEM_BLOCK_SIZE
	"\n err: VM Api err , init extra data table failure (解释器运行时错误：用户额外数据动态数组分配内存初始化失败)\n", //ZL_ERR_VM_API_INIT_EXTRA_DATA_TABLE_FAILED
	"\n err: VM Api err , extra data table can't find not-valid index (解释器运行时错误：额外数据表中无法找到没被占用的索引，无法完成添加工作)\n", //ZL_ERR_VM_API_CAN_NOT_FIND_NOT_VALID_INDEX_IN_EXTRA_DATA_TABLE
	"\n err: VM Api err , extra data name or point is null (解释器运行时错误：额外数据名称或对应的指针为空)\n", //ZL_ERR_VM_API_INVALID_EXTRA_DATA_NAME_OR_POINT
	"\n err: VM Api err , extra data table invalid index (解释器运行时错误：额外数据动态数组中发现无效的索引)\n", //ZL_ERR_VM_API_EXTRA_DATA_TABLE_INVALID_INDEX
	"\n err: VM Api err , extra data name '%s' exist (解释器运行时错误：额外数据名称'%s'已经存在)\n", //ZL_ERR_VM_API_EXTRA_DATA_NAME_EXIST
	"\n err: VM Api err , extra data name '%s' is invalid (解释器运行时错误：额外数据名称'%s'无效)\n",//ZL_ERR_VM_API_EXTRA_DATA_NAME_INVALID
	"\n syntax error: if statement have no express (语法错误：if语句的条件判断表达式为空)\n", //ZL_ERR_CP_SYNTAX_ASM_IF_HAVE_NO_EXPRESS
	"\n err: VM Api err , zengl api '%s' invalid call position (解释器运行时错误：'%s'该API接口的调用位置不对)\n", //ZL_ERR_VM_API_INVALID_CALL_POSITION
	"\n err: VM Api err , invalid zengl VM (解释器运行时错误：API接口中访问的是无效的虚拟机，请通过zenglApi_Open生成有效的虚拟机)\n", //ZL_ERR_VM_API_INVALID_ZLVM
	"\n err: VM Api err , invalid second arg type in zenglApi_Push (解释器运行时错误：zenglApi_Push接口的第二个参数是无效的参数类型)\n", //ZL_ERR_VM_API_INVALID_ARG_TYPE_WHEN_PUSH
	"\n err: VM Api err , invalid last arg type in zenglApi_SetMemBlock (解释器运行时错误：zenglApi_SetMemBlock接口中最后一个参数的类型错误)\n", //ZL_ERR_VM_API_INVALID_RETVAL_TYPE_WHEN_SETMEMBLOCK
	"\n err: VM Api err , invalid src arg in zenglApi_AllocMemForString (解释器运行时错误：zenglApi_AllocMemForString源字符串指针参数无效)\n", //ZL_ERR_VM_API_INVALID_SRC_WHEN_ALLOCMEM_FOR_STRING
	"\n err: VM Api err , invalid size arg in zenglApi_AllocMem (解释器运行时错误：zenglApi_AllocMem中的size参数无效)\n", //ZL_ERR_VM_API_INVALID_SIZE_WHEN_ALLOCMEM
	"\n err: VM Api err , invalid ptr arg in zenglApi_FreeMem (解释器运行时错误：zenglApi_FreeMem中的ptr指针参数无效)\n", //ZL_ERR_VM_API_INVALID_PTR_WHEN_FREEMEM
	"\n err: VM Api err , invalid script string in zenglApi_RunStr (解释器运行时错误：zenglApi_RunStr接口中的字符串脚本无效)\n", //ZL_ERR_VM_API_INVALID_SCRIPT_STR_WHEN_RUNSTR
	"\n err: VM Api err , DeubugVM open failed in %s (解释器运行时错误：%s中的DeubugVM分配失败)\n", //ZL_ERR_VM_API_DEBUGVM_OPEN_FAILED
	"\n err: VM Api err , invalid debug_str in %s (解释器运行时错误：%s中的debug_str调试字符串无效)\n", //ZL_ERR_VM_API_INVALID_DEBUG_STR
	"DebugErr:%s", //ZL_ERR_VM_API_DEBUG_ERR
	"\n err: VM Api err , debug break point init failed (解释器运行时错误：调试器断点初始化失败)\n", //ZL_ERR_VM_API_INIT_DEBUG_BREAK_FAILED
	"\n err: VM Api err , debug break point set failed (解释器运行时错误：调试器设置断点失败)\n", //ZL_ERR_VM_API_SET_DEBUG_BREAK_FAILED
	"\n err: VM Api err , invalid break point location (无效的断点位置)\n", //ZL_ERR_VM_API_INVALID_BREAK_LOCATION
	"\n err: VM Api err , invalid args in %s (调试器进行脚本函数追踪时，提供给API接口'%s'的参数无效)\n", //ZL_ERR_VM_API_DEBUG_TRACE_INVALID_ARGS
	"\n err: VM Api err , invalid ptr arg in zenglApi_ReAllocMem (解释器运行时错误：zenglApi_ReAllocMem中的ptr指针参数无效)\n", //ZL_ERR_VM_API_INVALID_PTR_WHEN_REALLOC
	"\n err: VM Api err , filename <'%s'> is too long (解释器运行时错误：文件路径信息太长)\n", //ZL_ERR_VM_API_FILENAME_TOO_LONG
	"\n err: VM Api err , filename <'%s'> + <'%s'> is too long (解释器运行时错误：合并后的文件路径信息太长)\n", //ZL_ERR_VM_API_FILENAME_TOO_LONG_WHEN_MERGE
	"\n err: VM Api err , invalid args in %s (解释器运行时错误：提供给API接口'%s'的参数无效)\n", //ZL_ERR_VM_API_INVALID_ARG
};
#endif

/*各种关键字(保留字)定义*/
ZL_CONST ZL_CHAR * ZL_Reserves_String[] = {
	ZL_NULL,    //ZL_RSV_START_NONE 普通的非关键字的token的初始值
	"print",    //ZL_RSV_PRINT
	"if",	    //ZL_RSV_IF
	"elif",     //ZL_RSV_ELIF
	"else",     //ZL_RSV_ELSE
	"endif",    //ZL_RSV_ENDIF
	"for",		//ZL_RSV_FOR
	"endfor",	//ZL_RSV_ENDFOR
	"fun",		//ZL_RSV_FUN
	"endfun",	//ZL_RSV_ENDFUN
	"global",	//ZL_RSV_GLOBAL
	"return",	//ZL_RSV_RETURN
	"use",		//ZL_RSV_USE
	"break",	//ZL_RSV_BREAK
	"continue",	//ZL_RSV_CONTINUE
	"def",		//ZL_RSV_DEF
	"inc",		//ZL_RSV_INC
	"switch",	//ZL_RSV_SWITCH
	"case",		//ZL_RSV_CASE
	"default",	//ZL_RSV_DEFAULT
	"endswt",	//ZL_RSV_ENDSWT
	"class",	//ZL_RSV_CLASS
	"endcls",	//ZL_RSV_ENDCLS
	"while",	//ZL_RSV_WHILE
	"endwhile",	//ZL_RSV_ENDWHILE
	"do",		//ZL_RSV_DO
	"dowhile",	//ZL_RSV_DOWHILE
	"endswitch",//ZL_RSV_ENDSWITCH 该关键字等价于 endswt
	"endclass",	//ZL_RSV_ENDCLASS  该关键字等价于 endcls
	ZL_NULL
};

/*所有操作类型运算符的字符串信息*/
ZL_CONST ZL_CHAR * ZL_Token_Operate_String[] = {
	ZL_NULL, //ZL_TK_START_NONE,	初始值，不对应任何token
	ZL_NULL, //ZL_TK_ID,			变量之类的标识符token 属于操作因子，不属于操作运算符，字符串信息存放在token字符串池中，所以这里为ZL_NULL
	ZL_NULL, //ZL_TK_RESERVE,		关键字token 关键字信息存放在上面的ZL_Reserves_String数组中，不属于操作运算符，所以这里为ZL_NULL
	ZL_NULL, //ZL_TK_NUM,			如123之类的数字的token 属于操作因子，不属于操作运算符，字符串信息存放在token字符串池中，所以这里为ZL_NULL
	ZL_NULL, //ZL_TK_FLOAT,			如3.14159之类的浮点数 属于操作因子，不属于操作运算符，字符串信息存放在token字符串池中，所以这里为ZL_NULL
	ZL_NULL, //ZL_TK_STR,			字符串token 属于操作因子，不属于操作运算符，字符串信息存放在token字符串池中，所以这里为ZL_NULL
	"=",	 //ZL_TK_ASSIGN,		赋值运算符token
	"==",	 //ZL_TK_EQUAL,			"=="等于运算符token
	">=",	 //ZL_TK_GREAT_EQ,		">="大于等于运算符token
	">",	 //ZL_TK_GREAT,			">"大于运算符token
	"<=",	 //ZL_TK_LESS_EQ,		"<="小于等于运算符token
	"<",	 //ZL_TK_LESS,			"<"小于运算符token
	"!=",	 //ZL_TK_NOT_EQ,		"!="不等于运算符token
	"!",	 //ZL_TK_REVERSE,		"!"取反运算符token
	"&&",	 //ZL_TK_AND,			"&&"逻辑且运算符token
	"&",	 //ZL_TK_ADDRESS,		"&"引用运算符token
	"||",	 //ZL_TK_OR,			"||"逻辑或运算符token
	"++",	 //ZL_TK_PLUS_PLUS,		++运算符token
	"+=",	 //ZL_TK_PLUS_ASSIGN,	+=运算符token
	"+",	 //ZL_TK_PLUS,			加法运算符token
	"--",	 //ZL_TK_MINIS_MINIS,	--运算符token
	"-=",	 //ZL_TK_MINIS_ASSIGN,	-=运算符token
	"-",	 //ZL_TK_MINIS,			减法运算符token
	"-",	 //ZL_TK_NEGATIVE,		负号单目运算符token
	"*=",	 //ZL_TK_TIMES_ASSIGN,	*=运算符token
	"*",	 //ZL_TK_TIMES,			乘法运算符token
	"/=",	 //ZL_TK_DIVIDE_ASSIGN,	"/=" 除赋值token
	"/",	 //ZL_TK_DIVIDE,		除法运算符token
	"%=",	 //ZL_TK_MOD_ASSIGN,	"%=" 取余赋值运算符token
	"%",	 //ZL_TK_MOD,			取余运算符token
	"(",	 //ZL_TK_LBRACKET,		左括号token
	")",	 //ZL_TK_RBRACKET,		右括号token
	"[",	 //ZL_TK_LMBRACKET,		左中括号token
	"]",	 //ZL_TK_RMBRACKET,		右中括号token
	ZL_NULL, //ZL_TK_ENDFILE,		文件结束符token
	";",	 //ZL_TK_SEMI,			分号token
	",",	 //ZL_TK_COMMA,			逗号token
	":",	 //ZL_TK_COLON,			冒号token
	"?",	 //ZL_TK_QUESTION_MARK,	问号token
	".",	 //ZL_TK_DOT,			点运算符
	"&",	 //ZL_TK_BIT_AND,		"&"按位与双目运算符token
	"&=",	 //ZL_TK_BIT_AND_ASSIGN, &=运算符token
	"|",	 //ZL_TK_BIT_OR,		"|"按位或双目运算符token
	"|=",	 //ZL_TK_BIT_OR_ASSIGN, |=运算符token
	"^",	 //ZL_TK_BIT_XOR,		"^"按位异或运算符token
	"^=",	 //ZL_TK_BIT_XOR_ASSIGN, ^=运算符token
	">>",	 //ZL_TK_BIT_RIGHT,		">>"右移运算符token
	">>=",	 //ZL_TK_BIT_RIGHT_ASSIGN, ">>="右移赋值运算符token
	"<<",	 //ZL_TK_BIT_LEFT,		"<<"左移运算符token
	"<<=",	 //ZL_TK_BIT_LEFT_ASSIGN,  "<<="左移赋值运算符token
	"~",	 //ZL_TK_BIT_REVERSE,	"~"按位取反运算符token
	(ZL_CHAR *)(-1L)
};

/*解释器中各种指令在打印输出时的显示字符串*/
ZL_CONST ZL_CHAR * ZL_Run_Inst_Type_String[] = {
	ZL_NULL,			//ZL_R_IT_NONE
	"MOV",				//ZL_R_IT_MOV
	"PRINT",			//ZL_R_IT_PRINT
	"PLUS",				//ZL_R_IT_PLUS
	"MINIS",			//ZL_R_IT_MINIS
	"MOD",				//ZL_R_IT_MOD
	"TIMES",			//ZL_R_IT_TIMES
	"DIVIDE",			//ZL_R_IT_DIVIDE
	"PUSH",				//ZL_R_IT_PUSH
	"POP",				//ZL_R_IT_POP
	"JE",				//ZL_R_IT_JE
	"JNE",				//ZL_R_IT_JNE
	"JMP",				//ZL_R_IT_JMP
	"GREAT",			//ZL_R_IT_GREAT
	"LESS",				//ZL_R_IT_LESS
	"EQUAL",			//ZL_R_IT_EQUAL
	"NOT_EQ",			//ZL_R_IT_NOT_EQ
	"GREAT_EQ",			//ZL_R_IT_GREAT_EQ
	"LESS_EQ",			//ZL_R_IT_LESS_EQ
	"AND",				//ZL_R_IT_AND
	"OR",				//ZL_R_IT_OR
	"REVERSE",			//ZL_R_IT_REVERSE
	"ADDGET",			//ZL_R_IT_ADDGET
	"MINIS_GET",		//ZL_R_IT_MINIS_GET
	"GETADD",			//ZL_R_IT_GETADD
	"GET_MINIS",		//ZL_R_IT_GET_MINIS
	"ADDONE",			//ZL_R_IT_ADDONE
	"MINIS_ONE",		//ZL_R_IT_MINIS_ONE
	"FUNARG",			//ZL_R_IT_FUNARG
	"ARG_SET",			//ZL_R_IT_ARG_SET
	"PUSH_LOC",			//ZL_R_IT_PUSH_LOC
	"RET",				//ZL_R_IT_RET
	"RESET",			//ZL_R_IT_RESET
	"CALL",				//ZL_R_IT_CALL
	"USE",				//ZL_R_IT_USE
	"ADDR",				//ZL_R_IT_ADDR
	"SET_ARRAY",		//ZL_R_IT_SET_ARRAY
	"GET_ARRAY_ADDR",	//ZL_R_IT_GET_ARRAY_ADDR
	"ADDGET_ARRAY",		//ZL_R_IT_ADDGET_ARRAY
	"MINISGET_ARRAY",	//ZL_R_IT_MINISGET_ARRAY
	"GETADD_ARRAY",		//ZL_R_IT_GETADD_ARRAY
	"GETMINIS_ARRAY",	//ZL_R_IT_GETMINIS_ARRAY
	"GET_ARRAY",		//ZL_R_IT_GET_ARRAY
	"SWITCH",			//ZL_R_IT_SWITCH
	"LONG",				//ZL_R_IT_LONG
	"BIT_AND",			//ZL_R_IT_BIT_AND
	"BIT_OR",			//ZL_R_IT_BIT_OR
	"BIT_XOR",			//ZL_R_IT_BIT_XOR
	"BIT_RIGHT",		//ZL_R_IT_BIT_RIGHT
	"BIT_LEFT",			//ZL_R_IT_BIT_LEFT
	"BIT_REVERSE",		//ZL_R_IT_BIT_REVERSE
	"BREAK",			//ZL_R_IT_BREAK
	"SINGLE_BREAK",		//ZL_R_IT_SINGLE_BREAK
	"END",				//ZL_R_IT_END
};

/*解释器中各种寄存器在打印输出时的显示字符串*/
ZL_CONST ZL_CHAR * ZL_Run_Register_Type_String[] = {
	ZL_NULL,	//ZL_R_RT_NONE
	"AX",		//ZL_R_RT_AX
	"BX",		//ZL_R_RT_BX
	"ARG",		//ZL_R_RT_ARG
	"LOC",		//ZL_R_RT_LOC
	"ARGTMP",	//ZL_R_RT_ARGTMP
	"ARRAY_ITEM",//ZL_R_RT_ARRAY_ITEM
	"DEBUG",	//ZL_R_RT_DEBUG
};
