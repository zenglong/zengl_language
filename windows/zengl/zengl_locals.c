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

/*Ϊ�˷������ԵĹ��ʻ�����������ڲ����п��ܵĴ���ʱ������ַ���ͳһ������һ�������У��������ű��е������*/
#ifdef ZL_LANG_EN_WITH_CH
ZL_CONST ZL_CHAR * ZL_Error_String[] = {
	"no error success (û�д���)", //ZL_NO_ERR_SUCCESS
	"\n err: can't open file : \"%s\" (�޷����ļ�)\n", //ZL_ERR_FILE_CAN_NOT_OPEN
	"\n err: can't read \"%s\" file content.(�޷���ȡ�ļ�����)\n", //ZL_ERR_FILE_CAN_NOT_GETS
	"\n err: compile makeTokenStr alloc failure. (��������makeTokenStr�����з����ڴ�ʧ��)\n ", //ZL_ERR_CP_MTSTR_MALLOC_FAILED
	"\n err: compile memAlloc failure when try to malloc ! (������Ϊ�ڴ��ָ������ڴ�ʧ��)\n", //ZL_ERR_CP_MP_MALLOC_FAILED
	"\n err: compile memAlloc can't find null point ! (�������޷����ڴ�����ҵ�NULLָ�룬Ҳ���޷������µ�ָ����ڴ�)\n", //ZL_ERR_CP_MP_MALLOC_NO_NULL_POINT
	"\n err: compile memReAlloc failure when try to realloc ! (������Ϊ�ڴ��ָ�����·����ڴ�ʧ��)\n", //ZL_ERR_CP_MP_REALLOC_FAILED
	"\n syntax error: %d:%d <'%s'> invalid Octal number (�﷨������Ч�İ˽��������˽�������0e��ͷ������������ֵ��0��7֮�����)\n", //ZL_ERR_CP_INVALID_OCTAL
	"\n syntax error: %d:%d <'%s'> invalid Hex number (�﷨������Ч��ʮ�����ƣ�ʮ����������0x��ͷ������������ֵ��0��9��a��f��A��F֮�����)\n", //ZL_ERR_CP_INVALID_HEX
	"\n syntax error: %d:%d <'%s'> string %s have no end close char [%c] (�﷨�����ַ���û�����ڽ����ĵ����Ż�˫����)\n", //ZL_ERR_CP_INVALID_STR_NO_ENDCHAR
	"\n err: makeTokenStrForString alloc failure. (��������makeTokenStrForString�����з����ڴ�ʧ��)\n", //ZL_ERR_CP_MTSFS_MALLOC_FAILED
	"\n syntax error: %d:%d <'%s'> multicomment %s have no end close [*/] (�﷨���󣺶���ע��û����ȷ�Ľ�����)\n", //ZL_ERR_CP_GT_INVALID_MULTI_COMMENT
	"\n err: def_StringPool alloc failure. (�������쳣��Ϊdef_StringPool�������ַ����ط����ڴ��ʼ��ʧ��)\n", //ZL_ERR_CP_DEF_STR_POOL_MALLOC_FAILED
	"\n err: def_StringPool add str err i out of bounds (�������쳣��������ص��ַ�����Ϣ��ӵ�def_StringPool�ַ�����ʱ�����ַ������鷶Χ)\n", //ZL_ERR_CP_DEF_STR_POOL_I_OUT_OF_BOUND
	"\n err: def_table alloc failure. (�������쳣��Ϊdef_table�궨�嶯̬��������ڴ��ʼ��ʧ��)\n", //ZL_ERR_CP_DEF_TABLE_MALLOC_FAILED
	"\n err: compile LineCols alloc failure (�������쳣��LineCols���кŶ�̬��������ڴ��ʼ��ʧ��)\n", //ZL_ERR_CP_LINECOL_MALLOC_FAILED
	"\n err: compile can't find non-valid index in def_table (�������쳣����Ӻ궨��ʱ���޷��ڶ�̬�������ҵ�û��ռ�õ�����)\n", //ZL_ERR_CP_DEF_TABLE_NO_NONVALID_INDEX
	"\n err: def_table's tmpindex is not valid when insert_HashTableForDef (�������쳣���ں���insert_HashTableForDef�У�tmpindex������Ч������)\n", //ZL_ERR_CP_DEF_TABLE_INVALID_INDEX
	"\n syntax error: the same def '%s' in %d:%d <'%s'> and %d:%d <'%s'> (�﷨���󣺴�����ͬ��def�� '%s' ���壬��������кŽ��м��)\n", //ZL_ERR_CP_DEF_TABLE_SAME_DEF_FOUND
	"\n syntax error: def must with id type! line:%d col:%d <'%s'> (�﷨����def�ؼ��ֺ����������Ч��id��ʶ��)\n", //ZL_ERR_CP_DEF_MUST_WITH_ID
	"\n syntax error: def can not with self! line:%d col:%d <'%s'> (�﷨����def�ؼ��ֺ��治����ʹ��self��Ϊ������)\n", //ZL_ERR_CP_DEF_CAN_NOT_WITH_SELF
	"\n syntax error: can not find the value of '%s' or no def lookup handle line:%d col:%d <'%s'> (�﷨�����޷���ѯ��def�ؼ��ֺ����'%s'����Ӧ��ֵ������û�ж�����ص�def��ѯ����)\n", //ZL_ERR_CP_DEF_LOOKUP_NOT_FOUND
	"\n syntax error: def must end with semi! line:%d col:%d <'%s'> (�﷨����def���������ԷֺŽ���)\n", //ZL_ERR_CP_DEF_NO_END_SEMI
	"\n syntax error: def const must be int num,float num or string! line:%d col:%d <'%s'> (�﷨����def����ĺ��Ӧ��ֵ���������֣������������ַ���)\n", //ZL_ERR_CP_DEF_INVALID_VALUE_TYPE
	"\n syntax error: filename <'%s'> is too long (�﷨�����ļ�·����Ϣ̫��)\n", //ZL_ERR_CP_INC_FILENAME_TOO_LONG
	"\n syntax error: filename <'%s'> + <'%s'> is too long (�﷨���󣺺ϲ�����ļ�·����Ϣ̫��)\n", //ZL_ERR_CP_INC_FILENAME_TOO_LONG_WHEN_MERGE
	"\n err: compile FileStackList alloc failure (�������쳣��ΪFileStackList�ļ����ض�ջ�����ڴ��ʼ��ʧ��)\n", //ZL_ERR_CP_INC_FILESTACKLIST_MALLOC_FAILED
	"\n syntax error: inc must end with semi! line:%d col:%d <'%s'> (�﷨����inc�������ԷֺŽ���)\n", //ZL_ERR_CP_INC_NO_END_SEMI
	"\n syntax error: inc must with string! line:%d col:%d <'%s'> (�﷨����inc�ؼ��ֺ���������ַ������͵��ļ������ļ����·��)\n", //ZL_ERR_CP_INC_MUST_WITH_STRING
	"\n err: compile def_table's tmpindex is not valid in lookupDefTable (�������쳣�����Һ궨��ʱ����Ч������ֵ)\n", //ZL_ERR_CP_DEF_TABLE_INVALID_INDEX_WHEN_LOOKUP
	"\n err: compile tokenInfo realloc failure when in ReplaceDefConst. (�������쳣���ڽ��к��滻ʱ���·����ڴ�ʧ��)\n", //ZL_ERR_CP_DEF_TABLE_TK_STR_REALLOC_FAILED_WHEN_REPLACE
	"\n err: compile FileStackList overflow (�������쳣���ļ����ض�ջ�������)\n", //ZL_ERR_CP_INC_FILESTACKLIST_OVERFLOW
	"\n err: compile FileStackList count must >= 0 (�������쳣��FileStackList�ļ���ջ��Ԫ�ظ���������ڵ���0)\n", //ZL_ERR_CP_INC_FILESTACKLIST_INVALID_COUNT
	"\n err: compile AST_nodes alloc failure! (�������쳣��AST�����﷨�������ڴ��ʼ��ʧ��)\n", //ZL_ERR_CP_PARSER_AST_MALLOC_FAILED
	"\n err: compile Token_StringPool alloc failure (�������쳣��token�ַ����ط����ڴ��ʼ��ʧ��)\n", //ZL_ERR_CP_TK_STR_POOL_MALLOC_FAILED
	"\n err: compile Token_StringPool add string error i out of bounds (�������쳣��Token_StringPool����������ַ�����Ϣʱ�������ش��󣺷��ص������������淶Χ)\n", //ZL_ERR_CP_TK_STR_POOL_OUT_OF_BOUNDS
	" parser err exit \n", //ZL_ERR_CP_PARSER_ERROR_EXIT
	"\n syntax parser error: parser_curnode out of end (�﷨���������﷨����ǰɨ��ڵ�parser_curnode�����﷨����Χ)", //ZL_ERR_CP_PARSER_CURNODE_OUT_OF_END
	"\n err: compile parser oplevel stacklist alloc failure (�������쳣�����ȼ��Ƚ϶�ջ�����ڴ��ʼ��ʧ��)\n", //ZL_ERR_CP_PARSER_OPLVL_STACKLIST_MALLOC_FAILED
	"\n syntax error: id must with operate(except '!' or '&') or left bracket or left middle bracket or ')' or ']' or semi (�﷨����id������ʶ����������ǲ��������(����ȡ�������ú͸��������)���������Ż����������Ż������Ż��������Ż�ֺ�)\n", //ZL_ERR_CP_SYNTAX_ID_INVALID_NEXT_NODE
	"\n syntax error: invalid next node (�﷨���󣺵�ǰ�ڵ��������Ч�ڵ㣬������ȱ�ٷֺŵ��µ�)\n", //ZL_ERR_CP_SYNTAX_INVALID_NEXT_NODE
	"\n syntax error: +,-,*,/, .... must with id or float or num or string or ++,-- or ! or '(' , if token is '=' can also with '&' (�﷨���󣺱��ʽ�����������������Ǳ��������֣��ַ����Ȳ������ӻ��߼ӼӼ�������ȡ��������򸺺Ż������ţ�����Ǹ�ֵ��䣬���滹�ɽ����������)\n", //ZL_ERR_CP_SYNTAX_EXPRESS_OP_INVALID_NEXT_NODE
	"\n syntax error: can't start with %s (�﷨���󣺿�ͷ��������'%s')\n", //ZL_ERR_CP_SYNTAX_START_INVALID_TOKEN
	"\n err: compile AST_TreeScanStackList alloc failure (�������쳣���﷨����ɨ���ջ�����ڴ��ʼ��ʧ��)\n", //ZL_ERR_CP_AST_SCAN_STACK_MALLOC_FAILED
	"\n err: compile AST_TreeScanStackList have no element (�������쳣���﷨����ɨ���ջΪ��)\n", //ZL_ERR_CP_AST_SCAN_STACK_HAS_NOTHING
	"\n syntax error: num,float,string must with operate(except assign or dot or '!' or '&') or ')' or ']' or semi (�﷨��������,������,�ַ�����������ǳ��˸�ֵ���㣬ȡ�������ã�����֮��Ĳ�����������������Ż����������Ż��߷ֺ�)\n", //ZL_ERR_CP_SYNTAX_NUM_FLOAT_STR_INVALID_NEXT_NODE
	"\n syntax error: %d:%d <'%s'> invalid token '%s' (�﷨������Ч��Token)\n", //ZL_ERR_CP_SYNTAX_INVALID_TOKEN
	"\n syntax error: '&' must with id (�﷨���������������������Ǳ���֮���ʶ��)\n", //ZL_ERR_CP_SYNTAX_ADDRESS_INVALID_NEXT_NODE
	"\n syntax error: invalid token after ++,-- (�﷨���󣺼ӼӼ������������������Ч�ķ��ţ����ӼӼ��������ʱ������ֻ���ǼӼӼ������������ӣ�ȡ�������Ż������ţ����Ҳ�ʱ��ֻ���ǳ��˸�ֵ���㣬ȡ�������ã����������֮��Ĳ�����������������Ż����������Ż��߷ֺ�)\n", //ZL_ERR_CP_SYNTAX_PPMM_INVALID_NEXT_NODE
	"\n syntax error: '(' must with id or float or num or string or ++,-- or ! or '(' or ')' or '&' (�﷨���������ź��������id������ʶ���������������֣��ַ�����ӼӼ�������ȡ����������򸺺ţ����������ţ����������ţ������Խ����������)\n", //ZL_ERR_CP_SYNTAX_LBRACKET_INVALID_NEXT_NODE
	"\n syntax error: ')' must with operate(except assign or dot or '!' or '&') or ')' or ']' or semi (�﷨���������ź�������ǳ��˸�ֵ���㣬ȡ�������ţ�����֮��Ĳ�����������������Ż����������Ż��߷ֺ�)\n", //ZL_ERR_CP_SYNTAX_RBRACKET_INVALID_NEXT_NODE
	"\n syntax error: bracket can't empty except funcall (�﷨������ͨ�����ű��ʽ����Ϊ�գ������Ǻ����Ĳ������ţ���Ϊ��������û�в���)\n", //ZL_ERR_CP_SYNTAX_BRACKET_EMPTY
	"\n syntax error: ')' have no '(' (�﷨����������û�ж�Ӧ��������)\n", //ZL_ERR_CP_SYNTAX_RBRACKET_NO_LBRACKET
	"\n syntax error: '(' have no ')' (�﷨����������û�ж�Ӧ��������)\n", //ZL_ERR_CP_SYNTAX_LBRACKET_NO_RBRACKET
	"\n syntax error: invalid token in parser express (�﷨���󣺱��ʽ�е�token��Ч����token�������ڱ��ʽ)\n", //ZL_ERR_CP_SYNTAX_PARSER_EXPRESS_INVALID_TOKEN
	"\n syntax error: unknown state in parser express , may be can't analysis token (�﷨������parser�����﷨����express������������Ч��״̬���������ǻ��޷�������token)\n", //ZL_ERR_CP_SYNTAX_PARSER_EXPRESS_UNKNOWN_STATE
	"\n syntax error: '[' must with id or float or num or string or ++,-- or ! or '(' or ']' (�﷨�����������ź��������id������ʶ���������������֣��ַ�����ӼӼ�������ȡ����������򸺺ţ����������ţ�������������)\n", //ZL_ERR_CP_SYNTAX_LMBRACKET_INVALID_NEXT_NODE
	"\n syntax error: invalid array item (�﷨������Ч������Ԫ�ظ�ʽ������Ԫ�ر�����test[a]�����ı�����ʶ�����������żӱ��ʽ���������ŵĸ�ʽ�����б��ʽ����Ϊ��)\n", //ZL_ERR_CP_SYNTAX_INVALID_ARRAY_ITEM
	"\n syntax error: ']' have no '[' (�﷨������������û�ж�Ӧ��������)\n", //ZL_ERR_CP_SYNTAX_RMBRACKET_NO_LMBRACKET
	"\n syntax error: '[' have no ']' (�﷨������������û�ж�Ӧ����������)\n", //ZL_ERR_CP_SYNTAX_LMBRACKET_NO_RMBRACKET
	"\n syntax error: ']' must with operate(except '!' or '&') or ')' or ']' or semi (�﷨�����������ź�������ǳ���ȡ�������ţ�����֮��Ĳ�����������������Ż����������Ż��߷ֺ�)\n", //ZL_ERR_CP_SYNTAX_RMBRACKET_INVALID_NEXT_NODE
	"\n syntax error: ',' must with id or float or num or string or ++,-- or ! or '(' or '&' (�﷨���󣺶�����������������id������ʶ���������������֣��ַ�����ӼӼ�������ȡ������������������ţ��������������)\n", //ZL_ERR_CP_SYNTAX_COMMA_INVALID_NEXT_NODE
	"\n syntax error: '.' must with id (�﷨���󣺵���������������id������ʶ��)\n", //ZL_ERR_CP_SYNTAX_DOT_INVALID_NEXT_NODE
	"\n syntax error: ':' must with id or float or num or string or ++,-- or ! or '(' or '&' (�﷨����ð����������������id������ʶ���������������֣��ַ�����ӼӼ�������ȡ����������򸺺ţ����������ţ��������������)\n", //ZL_ERR_CP_SYNTAX_COLON_INVALID_NEXT_NODE
	"\n syntax error: '?' must with id or float or num or string or ++,-- or ! or '(' or '&' (�﷨�����ʺ���������������id������ʶ���������������֣��ַ�����ӼӼ�������ȡ����������򸺺ţ����������ţ��������������)\n", //ZL_ERR_CP_SYNTAX_QUESTION_MARK_INVALID_NEXT_NODE
	"\n syntax error: '?' have no match colon in the right (�﷨�����ʺ��Ҳ�û����ƥ���ð�ţ�һ���ʺű����һ��ð�����Ӧ)\n", //ZL_ERR_CP_SYNTAX_QUESTION_NO_COLON_RIGHT
	"\n err: compile parser err, zengl_CheckQstColonValid function can only detect question token (�����쳣��zengl_CheckQstColonValidֻ������ʺ���������м��)\n", //ZL_ERR_CP_CHECK_QUESTION_VALID_FUNC_INVALID_TOKEN
	"\n syntax error: ':' have no match '?' in the left (�﷨����ð�����û����ƥ����ʺţ�һ��ð�ű����һ���ʺ����Ӧ)\n", //ZL_ERR_CP_SYNTAX_COLON_NO_QUESTION_LEFT
	"\n syntax error: if or elif statement with no left bracket (�﷨����if��elif�жϱ��ʽû��������)\n", //ZL_ERR_CP_SYNTAX_IF_NO_LBRACKET
	"\n err: compile parser err, zengl_detectSetRBracketToSemi function must start with left bracket token (�����쳣��zengl_detectSetRBracketToSemi��������������ŵ�token��ʼ���)\n", //ZL_ERR_CP_DETECT_SET_RBRACKET_TO_SEMI_FUNC_INVALID_START_TOKEN
	"\n syntax error: elif must use before else (�﷨����elif��������else֮ǰ)\n", //ZL_ERR_CP_SYNTAX_ELIF_MUST_BEFORE_ELSE
	"\n syntax error: else use more than one (�﷨����һ��if�ṹ��ֻ������һ��else���ô������ԭ�������֮ǰ�Ѿ��ù�һ��else��)\n", //ZL_ERR_CP_SYNTAX_ELSE_MORE_THAN_ONE
	"\n syntax error: unknown keyword in if structure (�﷨������if�ṹ��������Ч�Ĺؼ���)\n", //ZL_ERR_CP_SYNTAX_UNKNOWN_KEYWORD_INIF
	"\n syntax error: unknown keyword or the keyword can't use in start (�﷨������Ч�Ĺؼ��֣������Ǹùؼ��ֲ������ڿ�ͷ)\n", //ZL_ERR_CP_SYNTAX_UNKNOWN_KEYWORD
	"\n syntax error: if or inner if have no match endif (�﷨����if����Ƕ��if���û��ƥ���endif)\n", //ZL_ERR_CP_SYNTAX_IF_NO_ENDIF
	"\n syntax error: for have no match endfor (�﷨����forû�ж�Ӧ��endfor)\n", //ZL_ERR_CP_SYNTAX_FOR_NO_ENDFOR
	"\n syntax error: for statement with no left bracket (�﷨����for�����жϱ��ʽû��������)\n", //ZL_ERR_CP_SYNTAX_FOR_NO_LBRACKET
	"\n syntax error: for statement must have thress express (�﷨����for�����жϱ��ʽ�����������ӱ��ʽ�������for(i=1;i<=3;i++)��������ʽ���������ʽ����Ϊ�գ����ǲ�����ȱ���м�������ֺţ��˴���Ҳ������ȱ��for�����жϱ��ʽ�����������)\n", //ZL_ERR_CP_SYNTAX_FOR_STMT_MUST_HAVE_THREE_EXP
	"\n syntax error: it's not endfor (�﷨����for������endfor����)\n", //ZL_ERR_CP_SYNTAX_ITS_NOT_ENDFOR
	"\n syntax error: fun...endfun can't contain another fun (�﷨����һ��fun�����в����԰�����һ��fun�Ķ���)\n", //ZL_ERR_CP_SYNTAX_FUN_CONTAIN_ANOTHER_FUN
	"\n syntax error: fun have no match endfun (�﷨����funû�ж�Ӧ��endfun)\n", //ZL_ERR_CP_SYNTAX_FUN_NO_ENDFUN
	"\n syntax error: fun arg list with no left bracket (�﷨����fun��������û��������)\n", //ZL_ERR_CP_SYNTAX_FUN_NO_LBRACKET
	"\n syntax error: fun arg list invalid token (�﷨����fun���������б�������Ч��token)\n", //ZL_ERR_CP_SYNTAX_FUN_ARGLIST_INVALID_TOKEN
	"\n syntax error: fun have no fun name (�﷨����fun��������û�к�����)\n", //ZL_ERR_CP_SYNTAX_FUN_NO_NAME
	"\n syntax error: it's not endfun (�﷨����fun������endfun����)\n", //ZL_ERR_CP_SYNTAX_ITS_NOT_ENDFUN
	"\n syntax error: invalid express after print reserve (�﷨����print�ؼ��ֺ���ı��ʽ��Ч)\n", // ZL_ERR_CP_SYNTAX_INVALID_EXP_AFTER_PRINT
	"\n syntax error: invalid express after global reserve (�﷨����global�ؼ��ֺ�������Ч�ı��ʽ��global����ı��ʽֻ�ܰ���������ʶ���Ͷ��ţ����ԷֺŽ���)\n", //ZL_ERR_CP_SYNTAX_INVALID_EXP_AFTER_GLOBAL
	"\n syntax error: invalid express after use reserve (�﷨����use�ؼ��ֺ�������Ч�ı��ʽ��use����ı��ʽֻ�ܰ���������ʶ���Ͷ��ţ����ԷֺŽ���)\n", //ZL_ERR_CP_SYNTAX_INVALID_EXP_AFTER_USE
	"\n syntax error: invalid break express (�﷨������Ч��break��䣬break����Ӧ��ֱ���ԷֺŽ���)\n", //ZL_ERR_CP_SYNTAX_INVALID_BREAK_EXP
	"\n syntax error: invalid continue express (�﷨������Ч��continue��䣬continue����Ӧ��ֱ���ԷֺŽ���)\n", //ZL_ERR_CP_SYNTAX_INVALID_CONTINUE_EXP
	"\n syntax error: switch have no match endswt or endswitch (�﷨����switchû�ж�Ӧ��endswt����endswitch)\n", //ZL_ERR_CP_SYNTAX_SWITCH_NO_ENDSWT
	"\n syntax error: switch statement with no left bracket (�﷨����switch�����жϱ��ʽû��������)\n", //ZL_ERR_CP_SYNTAX_SWITCH_NO_LBRACKET
	"\n syntax error: switch statement case must with num or float or string (�﷨����switch���case������������֣����������ַ���)\n", //ZL_ERR_CP_SYNTAX_CASE_MUST_WITH_NUM_FLOAT_STR
	"\n syntax error: switch statement case must end with colon (�﷨����switch���case��������֣����������ַ��������ұ�����ð�Ž����������Խӱ��ʽ)\n", //ZL_ERR_CP_SYNTAX_CASE_MUST_END_WITH_COLON
	"\n syntax error: switch statement case must before default (�﷨����case��������default֮ǰ)\n", //ZL_ERR_CP_SYNTAX_CASE_MUST_BEFORE_DEFAULT
	"\n syntax error: switch statement default used more than once (�﷨����switch�ṹ��ֻ����һ��default)\n", //ZL_ERR_CP_SYNTAX_DEFAULT_USE_MORE_THAN_ONE
	"\n syntax error: switch statement default must end with colon (�﷨����switch���default�������ֱ����ð�Ž���)\n", //ZL_ERR_CP_SYNTAX_DEFAULT_MUST_END_WITH_COLON
	"\n syntax error: unknown keyword in switch structure (�﷨������switch�ṹ��������Ч�Ĺؼ���)\n", //ZL_ERR_CP_SYNTAX_UNKNOWN_KEYWORD_INSWITCH
	"\n syntax error: invalid member in class structure (�﷨����class�ṹ�а�����Ч�ĳ�Ա)\n", //ZL_ERR_CP_SYNTAX_INVALID_MEMBER_INCLASS
	"\n syntax error: class have no match endclass or endcls (�﷨����classû��ƥ���endclass��endcls)\n", //ZL_ERR_CP_SYNTAX_CLASS_NO_ENDCLS
	"\n syntax error: no class name in class define (�﷨����class�ඨ����û���������������������class�ؼ��ֺ���)\n", //ZL_ERR_CP_SYNTAX_CLASS_NO_CLASS_NAME
	"\n syntax error: it's not endcls or endclass (�﷨����class������endcls��endclass����)\n", //ZL_ERR_CP_SYNTAX_ITS_NOT_ENDCLS
	"\n syntax error: while or inner while have no match endwhile (�﷨����while����Ƕ��whileû�ж�Ӧ��endwhile)\n", //ZL_ERR_CP_SYNTAX_WHILE_NO_ENDWHILE
	"\n syntax error: while with no left bracket (�﷨����while�����жϱ��ʽû��������)\n", //ZL_ERR_CP_SYNTAX_WHILE_NO_LBRACKET
	"\n syntax error: it's not endwhile (�﷨����while������endwhile����)\n", //ZL_ERR_CP_SYNTAX_ITS_NOT_ENDWHILE
	"\n syntax error: do have no match dowhile (�﷨����doû�ж�Ӧ��dowhile)\n", //ZL_ERR_CP_SYNTAX_DO_NO_DOWHILE
	"\n syntax error: it's not dowhile (�﷨����do������dowhile����)\n", //ZL_ERR_CP_SYNTAX_ITS_NOT_DOWHILE
	"\n syntax error: dowhile with no left bracket (�﷨����dowhile�����жϱ��ʽû��������)\n", //ZL_ERR_CP_SYNTAX_DOWHILE_NO_LBRACKET
	"\n syntax error: invalid express after class statement (�﷨������Ч����������䣬�����������ֻ�ܰ���������ʶ���Ͷ��ţ����ԷֺŽ���)\n", //ZL_ERR_CP_SYNTAX_INVALID_EXP_AFTER_CLASS_STATEMENT
	"\n syntax error: line:%d,col:%d <'%s'> %s detect unexpect keyword %s (�﷨����%s��⵽�������Ĺؼ���%s)\n", //ZL_ERR_CP_SYNTAX_KEYWORD_DETECT_UNEXPECT_KEYWORD
	"\n syntax error: unexpect end keyword %s (�﷨���󣺲������Ľ����ؼ���%s)\n", //ZL_ERR_CP_SYNTAX_UNEXPECT_ENDKEYWORD
	"\n syntax error: unexpect location of use (�﷨����use�ؼ��ֳ����ڲ�������λ�ã�useֻ������ȫ�ַ�Χ�ڣ��Ҳ��ܳ�����if֮��Ŀ��ƽṹ��fun������)\n", //ZL_ERR_CP_SYNTAX_UNEXPECT_LOC_OF_USE
	"\n syntax error: unexpect location of global (�﷨����global�ؼ��ֳ����ڲ�������λ�ã����ں�����ʹ��global����global��Ҫ����if֮��Ŀ��ƽṹ��)\n", //ZL_ERR_CP_SYNTAX_UNEXPECT_LOC_OF_GLOBAL
	"\n syntax error: unexpect location of return (�﷨����return�ؼ��ֳ����ڲ�������λ�ã������ں�����)\n", //ZL_ERR_CP_SYNTAX_UNEXPECT_LOC_OF_RETURN
	"\n syntax error: unexpect location of break or continue (�﷨����break��continue��������while,do...dowhile,for,switch�ṹ��)\n", //ZL_ERR_CP_SYNTAX_UNEXPECT_LOC_OF_BREAK_CONTINUE
	"\n syntax error: invalid class function call (�﷨���󣺵����ຯ����ʽ����)\n", //ZL_ERR_CP_SYNTAX_INVALID_CLASS_FUNCALL
	"\n err: compile symbol err , SymGlobalTable alloc failure in zengl_SymInitGlobalTable (�������쳣����zengl_SymInitGlobalTable������ΪSymGlobalTableȫ�ֱ������ű�����ڴ��ʼ��ʧ��)\n", //ZL_ERR_CP_SYM_GLOBAL_TABLE_MALLOC_FAILED
	"\n err: compile symbol err , zengl_SymInsertGlobalTable function can't find non-valid index in SymGlobalTable (�������쳣��zengl_SymInsertGlobalTable�����޷���ȫ�ֱ������ű����ҵ�û��ռ�õ�����)\n", //ZL_ERR_CP_SYM_GLOBAL_TABLE_NO_NOT_VALID_INDEX
	"\n err: compile symbol err , zengl_SymInsertHashTableForGlobal function SymGlobalTable's tmpindex is not valid (�������쳣��zengl_SymInsertHashTableForGlobal������SymGlobalTable���ű����ҵ���tmpindex����Ч��)\n", //ZL_ERR_CP_SYM_GLOBAL_TABLE_FIND_NOT_VALID_INDEX
	"\n err: compile symbol err , zengl_SymInsertHashTableForGlobal function SymGlobalTable's tmpindex's linecols not init (�������쳣��tmpindex��Ӧ�ķ��ŵ����к�û�г�ʼ��)\n", //ZL_ERR_CP_SYM_GLOBAL_TABLE_INDEX_LINECOL_NOT_INIT
	"\n err: compile symbol err , zengl_SymPrintTables function print SymGlobalTable[%d] has no linecols (�������쳣��SymGlobalTable�е�[%d]������Ӧ�ķ���û�����к���Ϣ)\n", //ZL_ERR_CP_SYM_GLOBAL_TABLE_INDEX_HAVE_NO_LINECOL_WHEN_PRINT
	"\n err: compile symbol err , SymClassTable alloc failure in zengl_SymInitClassTable (�������쳣����zengl_SymInitClassTable������ΪSymClassTable����ű��Ӧ�Ķ�̬��������ڴ��ʼ��ʧ��)\n", //ZL_ERR_CP_SYM_CLASS_TABLE_MALLOC_FAILED
	"\n err: compile symbol err , zengl_SymInsertClassTable function can't find non-valid index in SymClassTable (�������쳣��zengl_SymInsertClassTable�����޷�������ű����ҵ�û��ռ�õ�����)\n",//ZL_ERR_CP_SYM_CLASS_TABLE_NO_NOT_VALID_INDEX
	"\n err: compile symbol err , zengl_SymInsertHashTableForClass function SymClassTable's tmpindex is not valid (�������쳣��zengl_SymInsertHashTableForClass������SymClassTable����ű����ҵ���tmpindex����Ч��)\n",	//ZL_ERR_CP_SYM_CLASS_TABLE_FIND_NOT_VALID_INDEX
	"\n syntax error: class %s has been defined in line:%d,col:%d <'%s'> (%s���Ѿ����������)\n", //ZL_ERR_CP_SYM_CLASS_HAS_BEEN_DEFINED
	"\n err: compile symbol err , zengl_SymPrintTables function print SymClassTable[%d] has no linecols (�������쳣��SymClassTable����ű��е�[%d]������Ӧ���ඨ����û�����к���Ϣ)\n", //ZL_ERR_CP_SYM_CLASS_TABLE_INDEX_HAVE_NO_LINECOL_WHEN_PRINT
	"\n err: compile symbol err , SymFunTable alloc failure in zengl_SymInitFunTable (�������쳣����zengl_SymInitFunTable������ΪSymFunTable�������ű�����ڴ��ʼ��ʧ��)\n", //ZL_ERR_CP_SYM_FUN_TABLE_MALLOC_FAILED
	"\n err: compile symbol err , zengl_SymInsertFunTable function can't find non-valid index in SymFunTable (�������쳣��zengl_SymInsertFunTable�����޷��ں������ű����ҵ�û��ռ�õ�����)\n", //ZL_ERR_CP_SYM_FUN_TABLE_NO_NOT_VALID_INDEX
	"\n err: compile symbol err , zengl_SymInsertHashTableForFun function SymFunTable's tmpindex is not valid (�������쳣��zengl_SymInsertHashTableForFun������SymFunTable���������ҵ���tmpindex����Ч��)\n", //ZL_ERR_CP_SYM_FUN_TABLE_FIND_NOT_VALID_INDEX
	"\n syntax error: fun %s has been defined in line:%d,col:%d <'%s'> (%s�ű������Ѿ����������)\n", //ZL_ERR_CP_SYM_FUN_HAS_BEEN_DEFINED
	"\n err: compile symbol err , zengl_SymPrintTables function print SymFunTable[%d] has no linecols (�������쳣��SymFunTable�������ű��е�[%d]������Ӧ�ĺ���������û�����к���Ϣ)\n", //ZL_ERR_CP_SYM_FUN_TABLE_INDEX_HAVE_NO_LINECOL_WHEN_PRINT
	"\n syntax error: class name '%s' does not exists (�﷨��������'%s'������)\n", //ZL_ERR_CP_SYNTAX_CLASS_NAME_OF_CLS_STMT_NOT_EXISTS
	"\n syntax error: self must be use in class and can not use to define class name (�﷨����self��������class�ṹ�У��Ҳ������ڶ���class��������)\n", //ZL_ERR_CP_SYNTAX_SELF_MUST_BE_USE_IN_CLASS
	"\n syntax error: class name can not use in debug express (�﷨���󣺲����ڵ��Ա��ʽ��ʹ������)\n", //ZL_ERR_CP_SYNTAX_CLASS_NAME_CAN_NOT_USE_IN_DEBUG
	"\n syntax error: script function call can not use in debug express (�﷨���󣺲����ڵ��Ա��ʽ�е����û��Զ���Ľű�����)\n", //ZL_ERR_CP_SYNTAX_SCRIPT_FUN_CALL_CAN_NOT_USE_IN_DEBUG
	"\n err: compile symbol err , zengl_SymLookupClass function SymClassTable's tmpindex is not valid (�������쳣��zengl_SymLookupClass������SymClassTable����ű����ҵ���tmpindex����Ч������)\n", //ZL_ERR_CP_SYM_CLASS_TABLE_FIND_NOT_VALID_INDEX_WHEN_LOOKUP
	"\n err: compile symbol err , SymClassMemberTable alloc failure in zengl_SymInitClassMemberTable (�������쳣����zengl_SymInitClassMemberTableΪSymClassMemberTable���Ա���ű�̬��������ڴ��ʼ��ʧ��)\n", //ZL_ERR_CP_SYM_CLASSMEMBER_TABLE_MALLOC_FAILED
	"\n err: compile symbol err , zengl_SymInsertClassMemberTable function can't find non-valid index in SymClassMemberTable (�������쳣��zengl_SymInsertClassMemberTable�����޷������Ա���ű����ҵ�û��ռ�õ�����)\n", //ZL_ERR_CP_SYM_CLASSMEMBER_TABLE_NO_NOT_VALID_INDEX
	"\n err: compile symbol err , zengl_SymInsertHashTableForClassMember function SymClassMemberTable's tmpindex is not valid (�������쳣��zengl_SymInsertHashTableForClassMember������SymClassMemberTable���Ա���ű����ҵ���tmpindex����Ч��)\n", //ZL_ERR_CP_SYM_CLASSMEMBER_TABLE_FIND_NOT_VALID_INDEX
	"\n syntax error: class member '%s' of class '%s' has been defined in line:%d,col:%d <'%s'> (%s���Ա�Ѿ����������)\n", //ZL_ERR_CP_SYNTAX_CLASSMEMBER_HAS_BEEN_DEFINED
	"\n syntax error: assign node must have 2 child-nodes (�﷨���󣺸�ֵ�ڵ�����������ӽڵ�)\n", //ZL_ERR_CP_SYNTAX_ASM_ASSIGN_MUST_HAVE_TWO_CHILD
	"\n err: compile symbol err , can't find '%s' memloc in SymGlobalTable (�������쳣���޷��ҵ�'%s'�������ڴ��ַ)\n", //ZL_ERR_CP_SYM_CAN_NOT_FIND_ID_MEMLOC
	"\n err: compile symbol err , zengl_SymLookupID function SymGlobalTable's tmpindex is not valid (�������쳣��zengl_SymLookupID������ȫ�ַ��ű����ҵ���tmpindex����Ч������)\n", //ZL_ERR_CP_SYM_GLOBAL_TABLE_FIND_NOT_VALID_INDEX_WHEN_LOOKUP
	"\n syntax error: assign second child node is not id or num float or sring or express (�﷨���󣺸�ֵ������ڶ����ӽڵ�Ȳ��Ǳ��������ֻ򸡵������ַ�����Ҳ���ǼӼ��˳��ȱ��ʽ)\n", //ZL_ERR_CP_SYNTAX_ASM_ASSIGN_SECOND_NODE_IS_INVALID_TYPE
	"\n syntax error: assign first child node is not valid struct (�﷨���󣺸�ֵ���ĵ�һ���ӽڵ㲻����Ч�Ľṹ)\n", //ZL_ERR_CP_SYNTAX_ASM_ASSIGN_FIRST_NODE_IS_INVALID_TYPE
	" run err exit when in compile \n", //ZL_ERR_RUN_ERROR_EXIT_WHEN_IN_COMPILE
	"\n err: run func err , mempool alloc failure when in zenglrun_init (�������쳣��Ϊ�������ڴ�ط����ڴ��ʼ��ʧ��)\n", //ZL_ERR_RUN_MEM_POOL_MALLOC_FAILED
	"\n err: run func err , memAlloc function failed (�������쳣��memAlloc���������ڴ�ʧ��)\n", //ZL_ERR_RUN_MEM_ALLOC_FUNC_FAILED
	"\n err: run func err , memAlloc function can't find null point ! (�������쳣��memAlloc�����޷����ڴ�����ҵ�NULLָ�룬Ҳ���޷������µ�ָ��)\n", //ZL_ERR_RUN_MEM_ALLOC_FUNC_CAN_NOT_FIND_NULL_POINT
	"\n err: run func err , memfreepool alloc failure when in zenglrun_init (�������쳣��Ϊ���������ڴ��ͷųط����ڴ��ʼ��ʧ��)\n", //ZL_ERR_RUN_MEM_FREE_POOL_MALLOC_FAILED
	"\n err: run main err , inst_list alloc failure when in zenglrun_initInstList (�������쳣��Ϊ�������Ļ��ָ������ڴ��ʼ��ʧ��)\n", //ZL_ERR_RUN_INST_LIST_MALLOC_FAILED
	"\n err: run func err , memReAlloc the point is not valid (�������쳣��memReAllocʱ����Ҫ������ָ�����ڴ������Ч)\n", //ZL_ERR_RUN_MEM_RE_ALLOC_INVALID_POINT
	"\n err: run func err , memReAlloc function failed (�������쳣��memReAlloc�������·����ڴ�ʧ��)\n", //ZL_ERR_RUN_MEM_RE_ALLOC_FUNC_FAILED
	"\n err: run main err , inst are not valid or pc:%d != PC:%d (�������쳣��ָ���PCֵ��PC�Ĵ������ֵ��һ��)\n", //ZL_ERR_RUN_INST_INVALID_PC
	"\n err: run func err , InstData_StringPool alloc failure in zenglrun_initInstDataStringPool(�������쳣����zenglrun_initInstDataStringPool������Ϊָ��������ַ����ط����ڴ��ʼ��ʧ��)\n", //ZL_ERR_RUN_INST_DATA_STRING_POOL_MALLOC_FAILED
	"\n err: run func err , i out of bounds when InstData_StringPool add string (�������쳣����ָ��������ַ���������ַ���ʱ����i������Χ)\n", //ZL_ERR_RUN_INST_DATA_STR_POOL_ADD_I_OUT_OF_BOUNDS
	"\n err: run main err , vmem_list alloc failure in zenglrun_initVMemList (�������쳣��Ϊȫ�������ڴ涯̬��������ڴ��ʼ��ʧ��)\n", //ZL_ERR_RUN_VMEM_LIST_MALLOC_FAILED
	"\n err: run err , hash array alloc memory failure (�������쳣��Ϊ�����ڴ��Ĺ�ϣ�������ϵͳ�ڴ�ʧ��)\n", //ZL_ERR_RUN_HASH_ARRAY_MEM_ALLOC_FAILED
	"\n err: run func err , zengl_memReUsePtr alloc failure (�������쳣���������ڴ��ָ��ʱ����������ϵͳ�ڴ�ʧ��)\n", //ZL_ERR_RUN_MEM_RE_USE_PTR_MALLOC_FAILED
	"\n err: run main err , invalid virtual mem type , pc=%d (�������쳣����Ч�������ڴ�����)\n", //ZL_ERR_RUN_INVALID_VMEM_TYPE
	"\n err: run main err , invalid inst src data type , pc=%d (�������쳣��ָ����Դ������������Ч)\n", //ZL_ERR_RUN_INVALID_INST_SRC_TYPE
	"\n err: run main err , invalid inst dest data type , pc=%d (�������쳣��ָ����Ŀ�������������Ч)\n", //ZL_ERR_RUN_INVALID_INST_DEST_TYPE
	"\n err: run main err , invalid inst type , pc=%d (�������쳣����Ч�Ļ��ָ��)\n", //ZL_ERR_RUN_INVALID_INST_TYPE
	"\n syntax error: invalid token can't generate assemble codes (�﷨������Ч��token����tokenĿǰ�޷����ɶ�Ӧ�Ļ��ָ��)\n", //ZL_ERR_CP_SYNTAX_ASM_INVALID_TOKEN_CAN_NOT_GEN_CODE
	"\n syntax error: print node must have 1 child-node (�﷨���󣺴�ӡ�ڵ�������һ���ӽڵ�)\n", //ZL_ERR_CP_SYNTAX_ASM_PRINT_MUST_HAVE_ONE_CHILD
	"\n syntax error: current node must have 2 child-node (�﷨���󣺵�ǰ�ڵ������������ӽڵ�)\n", //ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_TWO_CHILDS
	"\n syatax error: invalid child node type [%s line:%d,col:%d <'%s'>] (�﷨����������Ч���ӽڵ�����)\n", //ZL_ERR_CP_SYNTAX_ASM_INVALID_CHILD_NODE_TYPE
	"\n err: run main err , vstack_list alloc failure in zenglrun_initVStackList (�������쳣��Ϊ�����ջ��̬��������ڴ��ʼ��ʧ��)\n", //ZL_ERR_RUN_VSTACK_LIST_MALLOC_FAILED
	"\n err: run main err , vstack_list overflow vstack_list count less then zero pc:%d (�������쳣�������ջ���������ջ��������С��0����ǰPC�Ĵ���ֵ��%d)\n", //ZL_ERR_RUN_VSTACK_LIST_OVERFLOW
	"\n err: run main err , modulo by zero , pc=%d (����������ʱ����ȡ������ʱ�������������)\n", //ZL_ERR_RUN_MOD_BY_ZERO
	"\n err: run main err , division by zero , pc=%d (����������ʱ���󣺳�������ʱ�������������)\n", //ZL_ERR_RUN_DIVIDE_BY_ZERO
	"\n source code info: [ %s ] %d:%d <'%s'>\n", //ZL_ERR_RUN_SOURCE_CODE_INFO
	"\n err: compile assemble err , AsmGCStackList alloc failure in zengl_AsmGCStackInit (�������쳣������ջ��̬��������ڴ��ʼ��ʧ��)\n", //ZL_ERR_CP_ASM_GCSTACK_LIST_MALLOC_FAILED
	"\n err: compile assemble err , AsmGCLoopStackList alloc failure in zengl_AsmGCLoopStackInit or zengl_AsmGCLoopStackValsInit (�������쳣�����ģ���ջ��̬��������ڴ��ʼ��ʧ��)\n", //ZL_ERR_CP_ASM_GC_LOOP_STACK_LIST_MALLOC_FAILED
	"\n err: compile ld err , LDAddrList alloc failure in zengl_LDAddrListInit (�������쳣�����ӵ�ַ��̬��������ڴ��ʼ��ʧ��)\n", //ZL_ERR_CP_LD_ADDR_LIST_MALLOC_FAILED
	"\n err: compile ld err , LDAddrList index loc must unique pc:%d (�������쳣�����ӵ�ַ����Ψһ��һ��α��ַ��Ӧһ��������λ��)\n", //ZL_ERR_CP_LD_INDEX_ADDR_MUST_UNIQUE
	"\n err: compile ld err , invalid LDAddrList index : adr%d (�������쳣����Ч��α��ַ:adr%d)\n", //ZL_ERR_CP_LD_INVALID_ADDR_INDEX
	"\n syntax error: current node must have 1 child-node (�﷨���󣺵�ǰ�ڵ��������ֻ��һ���ӽڵ�)\n", //ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_ONE_CHILD
	"\n syntax error: current node must have at least 2 child-node (�﷨���󣺵�ǰ�ڵ���������������ӽڵ�)\n", //ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_AT_LEAST_TWO_CHILDS
	"\n syntax error: invalid op factor (�﷨������Ч�Ĳ�������)\n", //ZL_ERR_CP_SYNTAX_ASM_INVALID_OP_FACTOR
	"\n syntax error: current node must have 4 child-nodes (�﷨���󣺵�ǰ�ڵ��������ֻ���ĸ��ӽڵ�)\n", //ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_FOUR_CHILDS
	"\n syntax error: current node must have 3 child-nodes (�﷨���󣺵�ǰ�ڵ��������ֻ�������ӽڵ�)\n", //ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_THREE_CHILDS
	"\n syntax error: fun can't be defined in other fun (�﷨������һ���������ڲ����ٶ�����һ������)\n",//ZL_ERR_CP_SYNTAX_ASM_FUN_CAN_NOT_DEFINED_IN_OTHER_FUN
	"\n err: compile symbol err , AST_TreeScanStackList invalid count (�������쳣��ASTɨ���ջ���Ԫ�ظ���С��0)\n", //ZL_ERR_CP_SYM_AST_TREE_SCAN_STACK_INVALID_COUNT
	"\n err: compile symbol err , AST_TreeScanStackList are not empty (�������쳣��ASTɨ���ջ��Ϊ��)\n", //ZL_ERR_CP_SYM_AST_TREE_SCAN_STACK_NOT_EMPTY
	"\n err: compile symbol err , SymLocalTable alloc failure (�������쳣���ֲ��������ű�̬��������ڴ��ʼ��ʧ��)\n", //ZL_ERR_CP_SYM_LOCAL_TABLE_MALLOC_FAILED
	"\n err: compile symbol err , SymLocalTable can't find non-valid index when in zengl_SymInsertLocalTable (�������쳣���ֲ����ű��޷��ҵ�û��ռ�õ��������޷���ɷ��Ų������)\n", //ZL_ERR_CP_SYM_LOCAL_TABLE_CAN_NOT_FIND_NOT_VALID_INDEX_WHEN_INSERT
	"\n err: compile symbol err , SymLocalTable invalid index (�������쳣���ֲ����ű�������Ч����)\n", //ZL_ERR_CP_SYM_LOCAL_TABLE_INVALID_INDEX
	"\n syntax error: fun global reserve must have child node! (�﷨����global���治����Ϊ�սڵ㼴�����)\n", //ZL_ERR_CP_SYNTAX_SYM_GLOBAL_MUST_HAVE_CHILD_NODE
	"\n err: compile symbol err , can't find '%s' memloc in SymLocalTable (�������쳣���޷��ҵ�'%s'�������ڴ��ַ)\n", //ZL_ERR_CP_SYM_CAN_NOT_FIND_LOCAL_ID_MEMLOC
	"\n err: compile symbol err , invalid funid in zengl_SymLookupID (�������쳣���ֲ�����������Ϣ��Ӧ�ĺ���ID��Ч)\n", //ZL_ERR_CP_SYM_LOCAL_ID_INVALID_FUN_ID
	"\n err: compile symbol err , invalid local type in zengl_SymLookupID (�������쳣����Ч�ľֲ���������)\n", //ZL_ERR_CP_SYM_LOCAL_ID_INVALID_TYPE
	"\n err: compile symbol err , SymSelfClassTable alloc failure (�������쳣��self�ڵ��Ӧ������Ϣ��̬�������ʧ��)\n", //ZL_ERR_CP_SYM_SELF_CLASS_TABLE_MALLOC_FAILED
	"\n err: run main err , fun return address must int pc=%d (����������ʱ���󣺺����Ļ�෵�ص�ַ����������)\n", //ZL_ERR_RUN_MAIN_INVALID_FUN_RET_ADDRESS
	"\n err: run main err , fun virtual stack struct exception pc=%d (����������ʱ���󣺺���������ջ�ṹ�쳣)\n", //ZL_ERR_RUN_MAIN_FUN_VSTACK_STRUCT_EXCEPTION
	"\n syntax error: use reserve must have child node! (�﷨����use�ؼ��ֺ��治����Ϊ�սڵ㼴�����)\n", //ZL_ERR_CP_SYNTAX_SYM_USE_MUST_HAVE_CHILD_NODE
	"\n err: run func err , moduleTable alloc failure when in zenglrun_initModuleTable (�������쳣��ģ�鶯̬��������ڴ��ʼ��ʧ��)\n", //ZL_ERR_RUN_MODULE_TALBE_MALLOC_FAILED
	"\n err: run func err , moduleTable can't find non-valid index (�������쳣��ģ�鶯̬�������޷��ҵ�û��ռ�õ�����)\n", //ZL_ERR_RUN_MODULE_TALBE_NO_NOT_VALID_INDEX
	"\n err: VM Api err , moduleTable find invalid index (�������쳣��ģ�鶯̬������������Ч������)\n", //ZL_ERR_RUN_MODULE_TABLE_FIND_INVALID_INDEX
	"\n err: VM Api err , module '%s' has been set handle before (����������ʱ����ģ��'%s'�Ѿ������ù���������)\n", //ZL_ERR_RUN_MODULE_HAS_BEEN_SET_HANDLE_BEFORE
	"\n err: run func err , ModFunTable alloc failure when in zenglrun_initModFunTable (�������쳣��ģ�麯����̬��������ڴ��ʼ��ʧ��)\n", //ZL_ERR_RUN_MOD_FUN_TABLE_MALLOC_FAILED
	"\n err: run func err , ModFunTable can't find non-valid index (�������쳣��ģ�麯����̬�������޷��ҵ�û��ռ�õ�����)\n", //ZL_ERR_RUN_MOD_FUN_TABLE_NO_NOT_VALID_INDEX
	"\n err: VM Api err , ModFunTable find invalid index (�������쳣��ģ�麯����̬������������Ч������)\n", //ZL_ERR_RUN_MOD_FUN_TABLE_FIND_INVALID_INDEX
	"\n err: VM Api err , function '%s' has been set handle in module '%s' before (����������ʱ����ģ�麯��'%s'�Ѿ���'%s'ģ�������ù���������)\n", //ZL_ERR_RUN_MOD_FUN_HAS_BEEN_SET_HANDLE_IN_OTHER_MOD_BEFORE
	"\n err: run func err , module '%s' is invalid pc=%d (����������ʱ����ģ��'%s'��Ч������ԭ�����û�û�ж����ģ���Use������)\n", //ZL_ERR_RUN_MODULE_IS_INVALID
	"\n err: run func err , function '%s' is invalid pc=%d (����������ʱ���󣺺���'%s'��Ч)\n", //ZL_ERR_RUN_FUNCTION_IS_INVALID
	"\n err: VM Api err , zenglApi_GetFunArg...'s argnum is invalid pc=%d (����������ʱ����GetFunArg...�ӿڵ�argnum������Ч������С��1��Ҳ���ɴ��ڲ�������)\n", //ZL_ERR_VM_API_GET_FUN_ARG_ARGNUM_IS_INVALID
	"\n user defined error: ", //ZL_ERR_VM_API_USER_DEFINED_ERROR
	"\n err: VM Api err , invalid function name when in zenglApi_Call(����������ʱ����Call�ӿ��еĺ�������Ч)\n", //ZL_ERR_VM_API_INVALID_FUNCTION_NAME
	"\n err: VM Api err , invalid script name when (����������ʱ����API�ӿ��еĽű�����Ч)\n", //ZL_ERR_VM_API_INVALID_SCRIPT_NAME
	"\n err: VM Api err , VM has been used before please reset it (����������ʱ����API�ӿ��е������֮ǰ���ù��ˣ���ʹ��Reset�ӿڽ�������)\n", //ZL_ERR_VM_API_VM_MUST_RESET
	"\n err: VM Api err , GetValueAs... can only use in debug mode (����������ʱ����GetValueAs...֮��Ľӿ�ֻ������DEBUG����ģʽ��)\n", //ZL_ERR_VM_API_GET_VALUE_AS_MUST_IN_DEBUG_MODE
	"\n err: VM Api err , GetValueAs... valueName is invalid (����������ʱ����GetValueAs...֮��Ľӿ��е�ȫ�ֱ�������Ч�����ܲ������ڽű���)\n", //ZL_ERR_VM_API_GET_VALUE_AS_VALUE_NAME_INVALID
	"\n err: VM Api err , SymGlobalTable invalid index (����������ʱ����ͨ���ӿڲ��ұ�����ʱ��ȫ�ַ��ű��з�����Ч����)\n", //ZL_ERR_VM_API_SYM_GLOBAL_TABLE_INVALID_INDEX
	"\n err: VM Api err , GetValueAs... value's runtype is invalid (����������ʱ����GetValueAs...֮��Ľӿڲ��ҵ��ı���������ʱ������Ч��ֻ�������������������ַ�������)\n", //ZL_ERR_VM_API_GET_VALUE_AS_VALUE_RUNTYPE_INVALID
	"\n err: run main err , invalid runtype in ZENGL_RUN_VMEM_OP_GET_BY_RUNTYPE (����������ʱ����ZENGL_RUN_VMEM_OP_GET_BY_RUNTYPE��������Ч������ʱ����)\n", //ZL_ERR_RUN_INVALID_RUNTYPE_IN_GEN_BY_RUNTYPE_MACRO
	"\n err: run main err , memblock alloc failure (����������ʱ����Ϊ�ڴ������ڴ��ʼ��ʧ��)\n", //ZL_ERR_RUN_MEM_BLOCK_MALLOC_FAILED
	"\n err: run main err , memblock invalid index , maybe index less then zero or too big (����������ʱ���󣺶����飬���Ա���ڴ���������ʱ����������ֵ��Ч����������ֵС��0���߳����ڴ�鷶Χ)\n", //ZL_ERR_RUN_MEM_BLOCK_INVALID_INDEX
	"\n err: run main err , memblock virtual stack struct exception pc=%d (����������ʱ�����ڴ�������ջ�ṹ�쳣)\n", //ZL_ERR_RUN_MEM_BLOCK_VSTACK_STRUCT_EXCEPTION
	"\n err: run main err , array or class item is not init (����������ʱ�����������Ԫ�ز����ڣ������û�б���ʼ����������ֵС��0����ʹ��array������ֱ�Ӹ�ֵ�ȷ�ʽ���г�ʼ��)\n", //ZL_ERR_RUN_ARRAY_CLASS_ITEM_IS_NOT_INIT
	"\n err: VM Api err , SetRetVal invalid type (����������ʱ����SetRetVal�ӿ�������Ч��ֵ����)\n", //ZL_ERR_VM_API_SET_RET_VAL_INVALID_TYPE
	"\n syntax error: ClassStatement must have child node (�﷨���������������治����Ϊ�սڵ㼴�����)n", //ZL_ERR_SYNTAX_SYM_CLASS_STATEMENT_MUST_HAVE_CHILD_NODE
	"\n err: compile symbol err , ScanDotForClass classid is 0 but not in init (�������쳣��ScanDotForClass�ڷǳ�ʼɨ��״̬��������IDΪ0)\n", //ZL_ERR_CP_SYM_SCAN_DOT_FOR_CLASS_ID_ZERO_WHEN_NOT_INIT
	"\n syntax error: value is not a class value (�﷨���󣺸ñ�������һ�����������ʹ������������������)\n", //ZL_ERR_SYNTAX_SYM_CAN_NOT_FIND_CLASS_INFO_FOR_ID
	"\n syntax error: invalid class member (�﷨������Ч�����Ա)\n", //ZL_ERR_SYNTAX_INVALID_CLASS_MEMBER
	"\n syntax error: invalid reserve (�﷨������Ч�Ĺؼ���)\n", //ZL_ERR_CP_SYNTAX_ASM_INVALID_RESERVE
	"\n syntax error: current node must have at least 1 child-node (�﷨���󣺵�ǰ�ڵ����������һ���ӽڵ�)\n", //ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_AT_LEAST_ONE_CHILD
	"\n err: compile assemble err , case jmp table alloc failure (�������쳣��switch...case�ṹ��ת������ڴ��ʼ��ʧ��)\n", //ZL_ERR_CP_ASM_CASE_JMP_TABLE_MALLOC_FAILED
	"\n err: compile assemble err , case jmp table realloc failure (�������쳣��switch...case�ṹ��ת�����·����ڴ�ʧ��)\n", //ZL_ERR_CP_ASM_CASE_JMP_TABLE_REALLOC_FAILED
	"\n syntax error: switch have same case val (�﷨����switch�ṹ�з�����ͬ��caseֵ)\n", //ZL_ERR_CP_SYNTAX_ASM_SWITCH_FIND_SAME_CASE_VAL
	"\n err: compile assemble err , case jmp table caseAddr can't be 0 (�������쳣��switch...case�ṹ��ת����case��Ŀ�����λ�ò���Ϊ0)\n", //ZL_ERR_CP_ASM_CASE_JMP_TABLE_CASEADDR_CAN_NOT_BE_ZERO
	"\n err: run main err , SWITCH inst have no LONG inst below (�������쳣��SWITCHָ����������LONGָ��)\n", //ZL_ERR_RUN_SWITCH_HAVE_NO_LONG_INST_BELOW
	"\n err: VM Api err , invalid class name when in zenglApi_Call(����������ʱ����Call�ӿ��е�������Ч)\n", //ZL_ERR_VM_API_INVALID_CLASS_NAME_WHEN_CALL
	"\n err: VM Api err , class table find invalid index when in zenglApi_Call(����������ʱ�쳣��Call�ӿڵ���ʱ�ҵ���Ч��������)", //ZL_ERR_VM_API_INVALID_CLASS_TABLE_INDEX_WHEN_CALL
	"\n err: VM Api err , invalid memblock arg in zenglApi_CreateMemBlock (����������ʱ����CreateMemBlock�ӿ���memblock������Ч������Ϊ��ָ��)\n", //ZL_ERR_VM_API_INVALID_MEMBLOCK_ARG_IN_CREATE_MEM_BLOCK
	"\n err: VM Api err , invalid size arg in zenglApi_CreateMemBlock (����������ʱ����CreateMemBlock�ӿ���size������Ч��size������ڵ���0)\n", //ZL_ERR_VM_API_INVALID_SIZE_ARG_IN_CREATE_MEM_BLOCK
	"\n err: VM Api err , invalid memblock arg in zenglApi_SetRetValAsMemBlock (����������ʱ����SetRetValAsMemBlock�ӿ���memblock������Ч������Ϊ��ָ��)\n", //ZL_ERR_VM_API_INVALID_MEMBLOCK_ARG_IN_SET_RETVAL
	"\n err: VM Api err , invalid argnum arg in zenglApi_SetFunArg (����������ʱ����SetFunArg�ӿ���argnum������Ч)\n", //ZL_ERR_VM_API_INVALID_ARGNUM_ARG_IN_SET_FUN_ARG
	"\n err: VM Api err , invalid memblock arg in zenglApi_GetMemBlockSize (����������ʱ����GetMemBlockSize�ӿ���memblock������Ч������Ϊ��ָ��)\n", //ZL_ERR_VM_API_INVALID_MEMBLOCK_ARG_IN_GET_MEM_BLOCK_SIZE
	"\n err: VM Api err , invalid memblock arg in zenglApi_GetMemBlockNNCount (����������ʱ����GetMemBlockNNCount�ӿ���memblock������Ч������Ϊ��ָ��)\n", //ZL_ERR_VM_API_INVALID_MEMBLOCK_ARG_IN_GET_MEM_BLOCK_NNCOUNT
	"\n err: VM Api err , init extra data table failure (����������ʱ�����û��������ݶ�̬��������ڴ��ʼ��ʧ��)\n", //ZL_ERR_VM_API_INIT_EXTRA_DATA_TABLE_FAILED
	"\n err: VM Api err , extra data table can't find not-valid index (����������ʱ���󣺶������ݱ����޷��ҵ�û��ռ�õ��������޷������ӹ���)\n", //ZL_ERR_VM_API_CAN_NOT_FIND_NOT_VALID_INDEX_IN_EXTRA_DATA_TABLE
	"\n err: VM Api err , extra data name or point is null (����������ʱ���󣺶����������ƻ��Ӧ��ָ��Ϊ��)\n", //ZL_ERR_VM_API_INVALID_EXTRA_DATA_NAME_OR_POINT
	"\n err: VM Api err , extra data table invalid index (����������ʱ���󣺶������ݶ�̬�����з�����Ч������)\n", //ZL_ERR_VM_API_EXTRA_DATA_TABLE_INVALID_INDEX
	"\n err: VM Api err , extra data name '%s' exist (����������ʱ���󣺶�����������'%s'�Ѿ�����)\n", //ZL_ERR_VM_API_EXTRA_DATA_NAME_EXIST
	"\n err: VM Api err , extra data name '%s' is invalid (����������ʱ���󣺶�����������'%s'��Ч)\n",//ZL_ERR_VM_API_EXTRA_DATA_NAME_INVALID
	"\n syntax error: if statement have no express (�﷨����if���������жϱ��ʽΪ��)\n", //ZL_ERR_CP_SYNTAX_ASM_IF_HAVE_NO_EXPRESS
	"\n err: VM Api err , zengl api '%s' invalid call position (����������ʱ����'%s'��API�ӿڵĵ���λ�ò���)\n", //ZL_ERR_VM_API_INVALID_CALL_POSITION
	"\n err: VM Api err , invalid zengl VM (����������ʱ����API�ӿ��з��ʵ�����Ч�����������ͨ��zenglApi_Open������Ч�������)\n", //ZL_ERR_VM_API_INVALID_ZLVM
	"\n err: VM Api err , invalid second arg type in zenglApi_Push (����������ʱ����zenglApi_Push�ӿڵĵڶ�����������Ч�Ĳ�������)\n", //ZL_ERR_VM_API_INVALID_ARG_TYPE_WHEN_PUSH
	"\n err: VM Api err , invalid last arg type in zenglApi_SetMemBlock (����������ʱ����zenglApi_SetMemBlock�ӿ������һ�����������ʹ���)\n", //ZL_ERR_VM_API_INVALID_RETVAL_TYPE_WHEN_SETMEMBLOCK
	"\n err: VM Api err , invalid src arg in zenglApi_AllocMemForString (����������ʱ����zenglApi_AllocMemForStringԴ�ַ���ָ�������Ч)\n", //ZL_ERR_VM_API_INVALID_SRC_WHEN_ALLOCMEM_FOR_STRING
	"\n err: VM Api err , invalid size arg in zenglApi_AllocMem (����������ʱ����zenglApi_AllocMem�е�size������Ч)\n", //ZL_ERR_VM_API_INVALID_SIZE_WHEN_ALLOCMEM
	"\n err: VM Api err , invalid ptr arg in zenglApi_FreeMem (����������ʱ����zenglApi_FreeMem�е�ptrָ�������Ч)\n", //ZL_ERR_VM_API_INVALID_PTR_WHEN_FREEMEM
	"\n err: VM Api err , invalid script string in zenglApi_RunStr (����������ʱ����zenglApi_RunStr�ӿ��е��ַ����ű���Ч)\n", //ZL_ERR_VM_API_INVALID_SCRIPT_STR_WHEN_RUNSTR
	"\n err: VM Api err , DeubugVM open failed in %s (����������ʱ����%s�е�DeubugVM����ʧ��)\n", //ZL_ERR_VM_API_DEBUGVM_OPEN_FAILED
	"\n err: VM Api err , invalid debug_str in %s (����������ʱ����%s�е�debug_str�����ַ�����Ч)\n", //ZL_ERR_VM_API_INVALID_DEBUG_STR
	"DebugErr:%s", //ZL_ERR_VM_API_DEBUG_ERR
	"\n err: VM Api err , debug break point init failed (����������ʱ���󣺵������ϵ��ʼ��ʧ��)\n", //ZL_ERR_VM_API_INIT_DEBUG_BREAK_FAILED
	"\n err: VM Api err , debug break point set failed (����������ʱ���󣺵��������öϵ�ʧ��)\n", //ZL_ERR_VM_API_SET_DEBUG_BREAK_FAILED
	"\n err: VM Api err , invalid break point location (��Ч�Ķϵ�λ��)\n", //ZL_ERR_VM_API_INVALID_BREAK_LOCATION
	"\n err: VM Api err , invalid args in %s (���������нű�����׷��ʱ���ṩ��API�ӿ�'%s'�Ĳ�����Ч)\n", //ZL_ERR_VM_API_DEBUG_TRACE_INVALID_ARGS
	"\n err: VM Api err , invalid ptr arg in zenglApi_ReAllocMem (����������ʱ����zenglApi_ReAllocMem�е�ptrָ�������Ч)\n", //ZL_ERR_VM_API_INVALID_PTR_WHEN_REALLOC
	"\n err: VM Api err , filename <'%s'> is too long (����������ʱ�����ļ�·����Ϣ̫��)\n", //ZL_ERR_VM_API_FILENAME_TOO_LONG
	"\n err: VM Api err , filename <'%s'> + <'%s'> is too long (����������ʱ���󣺺ϲ�����ļ�·����Ϣ̫��)\n", //ZL_ERR_VM_API_FILENAME_TOO_LONG_WHEN_MERGE
	"\n err: VM Api err , invalid args in %s (����������ʱ�����ṩ��API�ӿ�'%s'�Ĳ�����Ч)\n", //ZL_ERR_VM_API_INVALID_ARG
	"\n err: VM Api err , zengl cache api '%s' search mempool point failed (����������ʱ����API����ӿ�'%s'�����ڴ��ָ��ʧ��)\n", //ZL_ERR_VM_API_CACHE_SEARCH_MEMPOOL_POINT_FAILED
	"\n err: VM Api err , zengl cache api '%s' get invalid cache data (����������ʱ����API����ӿ�'%s'��ȡ���Ļ���������Ч)\n", //ZL_ERR_VM_API_CACHE_INVALID_CACHE_DATA
};
#endif

/*���ֹؼ���(������)����*/
ZL_CONST ZL_CHAR * ZL_Reserves_String[] = {
	ZL_NULL,    //ZL_RSV_START_NONE ��ͨ�ķǹؼ��ֵ�token�ĳ�ʼֵ
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
	"endswitch",//ZL_RSV_ENDSWITCH �ùؼ��ֵȼ��� endswt
	"endclass",	//ZL_RSV_ENDCLASS  �ùؼ��ֵȼ��� endcls
	ZL_NULL
};

/*���в���������������ַ�����Ϣ*/
ZL_CONST ZL_CHAR * ZL_Token_Operate_String[] = {
	ZL_NULL, //ZL_TK_START_NONE,	��ʼֵ������Ӧ�κ�token
	ZL_NULL, //ZL_TK_ID,			����֮��ı�ʶ��token ���ڲ������ӣ������ڲ�����������ַ�����Ϣ�����token�ַ������У���������ΪZL_NULL
	ZL_NULL, //ZL_TK_RESERVE,		�ؼ���token �ؼ�����Ϣ����������ZL_Reserves_String�����У������ڲ������������������ΪZL_NULL
	ZL_NULL, //ZL_TK_NUM,			��123֮������ֵ�token ���ڲ������ӣ������ڲ�����������ַ�����Ϣ�����token�ַ������У���������ΪZL_NULL
	ZL_NULL, //ZL_TK_FLOAT,			��3.14159֮��ĸ����� ���ڲ������ӣ������ڲ�����������ַ�����Ϣ�����token�ַ������У���������ΪZL_NULL
	ZL_NULL, //ZL_TK_STR,			�ַ���token ���ڲ������ӣ������ڲ�����������ַ�����Ϣ�����token�ַ������У���������ΪZL_NULL
	"=",	 //ZL_TK_ASSIGN,		��ֵ�����token
	"==",	 //ZL_TK_EQUAL,			"=="���������token
	">=",	 //ZL_TK_GREAT_EQ,		">="���ڵ��������token
	">",	 //ZL_TK_GREAT,			">"���������token
	"<=",	 //ZL_TK_LESS_EQ,		"<="С�ڵ��������token
	"<",	 //ZL_TK_LESS,			"<"С�������token
	"!=",	 //ZL_TK_NOT_EQ,		"!="�����������token
	"!",	 //ZL_TK_REVERSE,		"!"ȡ�������token
	"&&",	 //ZL_TK_AND,			"&&"�߼��������token
	"&",	 //ZL_TK_ADDRESS,		"&"���������token
	"||",	 //ZL_TK_OR,			"||"�߼��������token
	"++",	 //ZL_TK_PLUS_PLUS,		++�����token
	"+=",	 //ZL_TK_PLUS_ASSIGN,	+=�����token
	"+",	 //ZL_TK_PLUS,			�ӷ������token
	"--",	 //ZL_TK_MINIS_MINIS,	--�����token
	"-=",	 //ZL_TK_MINIS_ASSIGN,	-=�����token
	"-",	 //ZL_TK_MINIS,			���������token
	"-",	 //ZL_TK_NEGATIVE,		���ŵ�Ŀ�����token
	"*=",	 //ZL_TK_TIMES_ASSIGN,	*=�����token
	"*",	 //ZL_TK_TIMES,			�˷������token
	"/=",	 //ZL_TK_DIVIDE_ASSIGN,	"/=" ����ֵtoken
	"/",	 //ZL_TK_DIVIDE,		���������token
	"%=",	 //ZL_TK_MOD_ASSIGN,	"%=" ȡ�ำֵ�����token
	"%",	 //ZL_TK_MOD,			ȡ�������token
	"(",	 //ZL_TK_LBRACKET,		������token
	")",	 //ZL_TK_RBRACKET,		������token
	"[",	 //ZL_TK_LMBRACKET,		��������token
	"]",	 //ZL_TK_RMBRACKET,		��������token
	ZL_NULL, //ZL_TK_ENDFILE,		�ļ�������token
	";",	 //ZL_TK_SEMI,			�ֺ�token
	",",	 //ZL_TK_COMMA,			����token
	":",	 //ZL_TK_COLON,			ð��token
	"?",	 //ZL_TK_QUESTION_MARK,	�ʺ�token
	".",	 //ZL_TK_DOT,			�������
	"&",	 //ZL_TK_BIT_AND,		"&"��λ��˫Ŀ�����token
	"&=",	 //ZL_TK_BIT_AND_ASSIGN, &=�����token
	"|",	 //ZL_TK_BIT_OR,		"|"��λ��˫Ŀ�����token
	"|=",	 //ZL_TK_BIT_OR_ASSIGN, |=�����token
	"^",	 //ZL_TK_BIT_XOR,		"^"��λ��������token
	"^=",	 //ZL_TK_BIT_XOR_ASSIGN, ^=�����token
	">>",	 //ZL_TK_BIT_RIGHT,		">>"���������token
	">>=",	 //ZL_TK_BIT_RIGHT_ASSIGN, ">>="���Ƹ�ֵ�����token
	"<<",	 //ZL_TK_BIT_LEFT,		"<<"���������token
	"<<=",	 //ZL_TK_BIT_LEFT_ASSIGN,  "<<="���Ƹ�ֵ�����token
	"~",	 //ZL_TK_BIT_REVERSE,	"~"��λȡ�������token
	(ZL_CHAR *)(-1L)
};

/*�������и���ָ���ڴ�ӡ���ʱ����ʾ�ַ���*/
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
	"RETURN",			//ZL_R_IT_RETURN
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

/*�������и��ּĴ����ڴ�ӡ���ʱ����ʾ�ַ���*/
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
