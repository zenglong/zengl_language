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

/*���ִ����ö�ٶ���*/
typedef enum _ZENGL_ERRORNO{
	ZL_NO_ERR_SUCCESS, //û�д���
	ZL_ERR_FILE_CAN_NOT_OPEN, //�޷����ļ�
	ZL_ERR_FILE_CAN_NOT_GETS, //�޷���ȡ�ļ�����
	ZL_ERR_CP_MTSTR_MALLOC_FAILED, //��������makeTokenStr�����з����ڴ�ʧ��
	ZL_ERR_CP_MP_MALLOC_FAILED, //������Ϊ�ڴ��ָ������ڴ�ʧ��
	ZL_ERR_CP_MP_MALLOC_NO_NULL_POINT, //�������޷����ڴ�����ҵ�NULLָ��
	ZL_ERR_CP_MP_REALLOC_FAILED, //������Ϊ�ڴ��ָ�����·����ڴ�ʧ��
	ZL_ERR_CP_INVALID_OCTAL, //��Ч�İ˽�����
	ZL_ERR_CP_INVALID_HEX, //��Ч��ʮ��������
	ZL_ERR_CP_INVALID_STR_NO_ENDCHAR, //�ַ���û�����ڽ����ĵ����Ż�˫����
	ZL_ERR_CP_MTSFS_MALLOC_FAILED, //��������makeTokenStrForString�����з����ڴ�ʧ��
	ZL_ERR_CP_GT_INVALID_MULTI_COMMENT, //��Ч�Ķ���ע��
	ZL_ERR_CP_DEF_STR_POOL_MALLOC_FAILED, //������Ϊdef_StringPool�������ַ����ط����ڴ��ʼ��ʧ��
	ZL_ERR_CP_DEF_STR_POOL_I_OUT_OF_BOUND, //������������ص��ַ�����Ϣ��ӵ�def_StringPool�ַ�����ʱ�����ַ������鷶Χ
	ZL_ERR_CP_DEF_TABLE_MALLOC_FAILED, //������Ϊdef_table�궨�嶯̬��������ڴ��ʼ��ʧ��
	ZL_ERR_CP_LINECOL_MALLOC_FAILED, //������ΪLineCols���кŶ�̬��������ڴ��ʼ��ʧ��
	ZL_ERR_CP_DEF_TABLE_NO_NONVALID_INDEX, //�������쳣����Ӻ궨��ʱ���޷��ڶ�̬�������ҵ�û��ռ�õ�����
	ZL_ERR_CP_DEF_TABLE_INVALID_INDEX, //�������쳣���ں���insert_HashTableForDef�У�tmpindex������Ч������
	ZL_ERR_CP_DEF_TABLE_SAME_DEF_FOUND, //�﷨���󣺴�����ͬ��def�� '%s' ���壬��������кŽ��м��
	ZL_ERR_CP_DEF_MUST_WITH_ID, //�﷨����def�ؼ��ֺ����������Ч��id��ʶ��
	ZL_ERR_CP_DEF_NO_END_SEMI, //�﷨����def���������ԷֺŽ���
	ZL_ERR_CP_DEF_INVALID_VALUE_TYPE, //�﷨����def����ĺ��Ӧ��ֵ���������֣������������ַ���
	ZL_ERR_CP_INC_FILENAME_TOO_LONG, //�﷨�����ļ�·����Ϣ̫��
	ZL_ERR_CP_INC_FILENAME_TOO_LONG_WHEN_MERGE, //�﷨���󣺺ϲ�����ļ�·����Ϣ̫��
	ZL_ERR_CP_INC_FILESTACKLIST_MALLOC_FAILED, //�������쳣��ΪFileStackList�ļ����ض�ջ�����ڴ��ʼ��ʧ��
	ZL_ERR_CP_INC_NO_END_SEMI, //�﷨����inc�������ԷֺŽ���
	ZL_ERR_CP_INC_MUST_WITH_STRING, //�﷨����inc�ؼ��ֺ���������ַ������͵��ļ������ļ����·��
	ZL_ERR_CP_DEF_TABLE_INVALID_INDEX_WHEN_LOOKUP, //�������쳣�����Һ궨��ʱ����Ч������ֵ
	ZL_ERR_CP_DEF_TABLE_TK_STR_REALLOC_FAILED_WHEN_REPLACE, //�������쳣���ڽ��к��滻ʱ���·����ڴ�ʧ��
	ZL_ERR_CP_INC_FILESTACKLIST_OVERFLOW, //�������쳣���ļ����ض�ջ�������
	ZL_ERR_CP_INC_FILESTACKLIST_INVALID_COUNT, //�������쳣��FileStackList�ļ���ջ��Ԫ�ظ���������ڵ���0
	ZL_ERR_CP_PARSER_AST_MALLOC_FAILED, //�������쳣��AST�����﷨�������ڴ��ʼ��ʧ��
	ZL_ERR_CP_TK_STR_POOL_MALLOC_FAILED, //�������쳣��token�ַ����ط����ڴ��ʼ��ʧ��
	ZL_ERR_CP_TK_STR_POOL_OUT_OF_BOUNDS, //�������쳣��Token_StringPool����������ַ�����Ϣʱ�������ش��󣺷��ص������������淶Χ
	ZL_ERR_CP_PARSER_ERROR_EXIT, //parser err exit ��zengl_parser_errorExit������ĩβ���ӵĴ�����Ϣ��
	ZL_ERR_CP_PARSER_CURNODE_OUT_OF_END, //�﷨���������﷨����ǰɨ��ڵ�parser_curnode�����﷨����Χ
	ZL_ERR_CP_PARSER_OPLVL_STACKLIST_MALLOC_FAILED, //�������쳣�����ȼ��Ƚ϶�ջ�����ڴ��ʼ��ʧ��
	ZL_ERR_CP_SYNTAX_ID_INVALID_NEXT_NODE, //�﷨����id������ʶ����������ǲ��������(����ȡ�������ú͸��������)���������Ż����������Ż������Ż��������Ż�ֺ�
	ZL_ERR_CP_SYNTAX_INVALID_NEXT_NODE, //�﷨�����޷�ʶ��Ľڵ�
	ZL_ERR_CP_SYNTAX_EXPRESS_OP_INVALID_NEXT_NODE, //�﷨���󣺱��ʽ�����������������Ǳ��������֣��ַ����Ȳ������ӻ��߼ӼӼ�������ȡ��������򸺺Ż�������
	ZL_ERR_CP_SYNTAX_START_INVALID_TOKEN, //�﷨���󣺿�ͷ��Ч��token
	ZL_ERR_CP_AST_SCAN_STACK_MALLOC_FAILED, //�������쳣���﷨����ɨ���ջ�����ڴ��ʼ��ʧ��
	ZL_ERR_CP_AST_SCAN_STACK_HAS_NOTHING, //�������쳣���﷨����ɨ���ջΪ��
	ZL_ERR_CP_SYNTAX_NUM_FLOAT_STR_INVALID_NEXT_NODE, //�﷨��������,������,�ַ�����������ǳ��˸�ֵ���㣬ȡ�������ã�����֮��Ĳ�����������������Ż����������Ż��߷ֺ�
	ZL_ERR_CP_SYNTAX_INVALID_TOKEN, //�﷨����δ֪��Token������ʶ��ķ���
	ZL_ERR_CP_SYNTAX_ADDRESS_INVALID_NEXT_NODE, //�﷨���������������������Ǳ���֮���ʶ��
	ZL_ERR_CP_SYNTAX_PPMM_INVALID_NEXT_NODE, //�﷨���󣺼ӼӼ������������������Ч�ķ���
	ZL_ERR_CP_SYNTAX_LBRACKET_INVALID_NEXT_NODE, //�﷨���������ź��������id������ʶ�� ....
	ZL_ERR_CP_SYNTAX_RBRACKET_INVALID_NEXT_NODE, //�﷨���������ź�������ǳ��˸�ֵ���㣬ȡ�������ţ�����֮��Ĳ�����������߷ֺ�
	ZL_ERR_CP_SYNTAX_BRACKET_EMPTY, //�﷨������ͨ�����ű��ʽ����Ϊ�գ������Ǻ����Ĳ������ţ���Ϊ��������û�в���
	ZL_ERR_CP_SYNTAX_RBRACKET_NO_LBRACKET, //�﷨����������û�ж�Ӧ��������
	ZL_ERR_CP_SYNTAX_LBRACKET_NO_RBRACKET, //�﷨����������û�ж�Ӧ��������
	ZL_ERR_CP_SYNTAX_PARSER_EXPRESS_INVALID_TOKEN, // �﷨���󣺱��ʽ�е�token��Ч����token�������ڱ��ʽ
	ZL_ERR_CP_SYNTAX_PARSER_EXPRESS_UNKNOWN_STATE, //�﷨������parser�����﷨����express������������Ч��״̬���������ǻ��޷�������token
	ZL_ERR_CP_SYNTAX_LMBRACKET_INVALID_NEXT_NODE, //�﷨�����������ź��������id������ʶ���������������֣��ַ�����ӼӼ��� .....
	ZL_ERR_CP_SYNTAX_INVALID_ARRAY_ITEM, //�﷨������Ч������Ԫ�ظ�ʽ
	ZL_ERR_CP_SYNTAX_RMBRACKET_NO_LMBRACKET, //�﷨������������û�ж�Ӧ��������
	ZL_ERR_CP_SYNTAX_LMBRACKET_NO_RMBRACKET, //�﷨������������û�ж�Ӧ����������
	ZL_ERR_CP_SYNTAX_RMBRACKET_INVALID_NEXT_NODE, //�﷨�����������ź�������ǳ���ȡ�������ţ�����֮��Ĳ�����������������Ż����������Ż��߷ֺ�
	ZL_ERR_CP_SYNTAX_COMMA_INVALID_NEXT_NODE, //�﷨���󣺶�����������������id������ʶ�� .....
	ZL_ERR_CP_SYNTAX_DOT_INVALID_NEXT_NODE, //�﷨���󣺵���������������id������ʶ��
	ZL_ERR_CP_SYNTAX_COLON_INVALID_NEXT_NODE, //�﷨����ð����������������id������ʶ�� .....
	ZL_ERR_CP_SYNTAX_QUESTION_MARK_INVALID_NEXT_NODE, //�﷨�����ʺ���������������id������ʶ��......
	ZL_ERR_CP_SYNTAX_QUESTION_NO_COLON_RIGHT, //�﷨�����ʺ��Ҳ�û����ƥ���ð��
	ZL_ERR_CP_CHECK_QUESTION_VALID_FUNC_INVALID_TOKEN, //�����쳣��zengl_CheckQstColonValidֻ������ʺ���������м��
	ZL_ERR_CP_SYNTAX_COLON_NO_QUESTION_LEFT, //�﷨����ð�����û����ƥ����ʺ�
	ZL_ERR_CP_SYNTAX_IF_NO_LBRACKET, //�﷨����if�жϱ��ʽû��������
	ZL_ERR_CP_DETECT_SET_RBRACKET_TO_SEMI_FUNC_INVALID_START_TOKEN, //�����쳣��zengl_detectSetRBracketToSemi��������������ŵ�token��ʼ���
	ZL_ERR_CP_SYNTAX_ELIF_MUST_BEFORE_ELSE, //�﷨����elif��������else֮ǰ
	ZL_ERR_CP_SYNTAX_ELSE_MORE_THAN_ONE, //�﷨����һ��if�ṹ��ֻ������һ��else
	ZL_ERR_CP_SYNTAX_UNKNOWN_KEYWORD_INIF, //�﷨������if�ṹ��������Ч�Ĺؼ���
	ZL_ERR_CP_SYNTAX_UNKNOWN_KEYWORD, //�﷨������Ч�Ĺؼ���
	ZL_ERR_CP_SYNTAX_IF_NO_ENDIF, //�﷨����if����Ƕ��if���û��ƥ���endif
	ZL_ERR_CP_SYNTAX_FOR_NO_ENDFOR, //�﷨����forû�ж�Ӧ��endfor
	ZL_ERR_CP_SYNTAX_FOR_NO_LBRACKET, //�﷨����for�����жϱ��ʽû��������
	ZL_ERR_CP_SYNTAX_FOR_STMT_MUST_HAVE_THREE_EXP, //�﷨����for�����жϱ��ʽ�����������ӱ��ʽ���
	ZL_ERR_CP_SYNTAX_ITS_NOT_ENDFOR, //�﷨����for������endfor����
	ZL_ERR_CP_SYNTAX_FUN_CONTAIN_ANOTHER_FUN, //�﷨����һ��fun�����в����԰�����һ��fun�Ķ���
	ZL_ERR_CP_SYNTAX_FUN_NO_ENDFUN, //�﷨����funû�ж�Ӧ��endfun
	ZL_ERR_CP_SYNTAX_FUN_NO_LBRACKET, //�﷨����fun��������û��������
	ZL_ERR_CP_SYNTAX_FUN_ARGLIST_INVALID_TOKEN, //�﷨����fun���������б�������Ч��token
	ZL_ERR_CP_SYNTAX_FUN_NO_NAME, //�﷨����fun��������û�к�����
	ZL_ERR_CP_SYNTAX_ITS_NOT_ENDFUN, //�﷨����fun������endfun����
	ZL_ERR_CP_SYNTAX_INVALID_EXP_AFTER_PRINT, //�﷨����print�ؼ��ֺ���ı��ʽ��Ч
	ZL_ERR_CP_SYNTAX_INVALID_EXP_AFTER_GLOBAL, //�﷨����global�ؼ��ֺ�������Ч�ı��ʽ
	ZL_ERR_CP_SYNTAX_INVALID_EXP_AFTER_USE,	 //�﷨����use�ؼ��ֺ�������Ч�ı��ʽ
	ZL_ERR_CP_SYNTAX_INVALID_BREAK_EXP, //�﷨������Ч��break���
	ZL_ERR_CP_SYNTAX_INVALID_CONTINUE_EXP, //�﷨������Ч��continue���
	ZL_ERR_CP_SYNTAX_SWITCH_NO_ENDSWT, //�﷨����switchû�ж�Ӧ��endswt����endswitch
	ZL_ERR_CP_SYNTAX_SWITCH_NO_LBRACKET, //�﷨����switch�����жϱ��ʽû��������
	ZL_ERR_CP_SYNTAX_CASE_MUST_WITH_NUM_FLOAT_STR, //�﷨����switch���case������������֣����������ַ���
	ZL_ERR_CP_SYNTAX_CASE_MUST_END_WITH_COLON, //�﷨����switch���case��������֣����������ַ��������ұ�����ð�Ž����������Խӱ��ʽ
	ZL_ERR_CP_SYNTAX_CASE_MUST_BEFORE_DEFAULT, //�﷨����case��������default֮ǰ
	ZL_ERR_CP_SYNTAX_DEFAULT_USE_MORE_THAN_ONE, //�﷨����switch�ṹ��ֻ����һ��default
	ZL_ERR_CP_SYNTAX_DEFAULT_MUST_END_WITH_COLON, //�﷨����switch���default�������ֱ����ð�Ž���
	ZL_ERR_CP_SYNTAX_UNKNOWN_KEYWORD_INSWITCH, //�﷨������switch�ṹ��������Ч�Ĺؼ���
	ZL_ERR_CP_SYNTAX_INVALID_MEMBER_INCLASS, //�﷨����class�ṹ�а�����Ч�ĳ�Ա
	ZL_ERR_CP_SYNTAX_CLASS_NO_ENDCLS,  //�﷨����classû��ƥ���endclass��endcls
	ZL_ERR_CP_SYNTAX_CLASS_NO_CLASS_NAME, //�﷨����class�ඨ����û���������������������class�ؼ��ֺ���
	ZL_ERR_CP_SYNTAX_ITS_NOT_ENDCLS,	//�﷨����class������endcls��endclass����
	ZL_ERR_CP_SYNTAX_WHILE_NO_ENDWHILE, //�﷨����while����Ƕ��whileû�ж�Ӧ��endwhile
	ZL_ERR_CP_SYNTAX_WHILE_NO_LBRACKET,	//�﷨����while�����жϱ��ʽû��������
	ZL_ERR_CP_SYNTAX_ITS_NOT_ENDWHILE,	//�﷨����while������endwhile����
	ZL_ERR_CP_SYNTAX_DO_NO_DOWHILE,		//�﷨����doû�ж�Ӧ��dowhile
	ZL_ERR_CP_SYNTAX_ITS_NOT_DOWHILE,	//�﷨����do������dowhile����
	ZL_ERR_CP_SYNTAX_DOWHILE_NO_LBRACKET,	//�﷨����dowhile�����жϱ��ʽû��������
	ZL_ERR_CP_SYNTAX_INVALID_EXP_AFTER_CLASS_STATEMENT,	//�﷨������Ч�����������
	ZL_ERR_CP_SYNTAX_KEYWORD_DETECT_UNEXPECT_KEYWORD,		//�﷨����%s��⵽�������Ĺؼ���%s
	ZL_ERR_CP_SYNTAX_UNEXPECT_ENDKEYWORD,	//�﷨���󣺲������Ľ����ؼ���%s
	ZL_ERR_CP_SYNTAX_UNEXPECT_LOC_OF_USE,	//�﷨����use�ؼ��ֳ����ڲ�������λ��
	ZL_ERR_CP_SYNTAX_UNEXPECT_LOC_OF_GLOBAL,//�﷨����global�ؼ��ֳ����ڲ�������λ��
	ZL_ERR_CP_SYNTAX_UNEXPECT_LOC_OF_RETURN,//�﷨����return�ؼ��ֳ����ڲ�������λ��
	ZL_ERR_CP_SYNTAX_UNEXPECT_LOC_OF_BREAK_CONTINUE,	//�﷨����break��continue��������while,do...dowhile,for,switch�ṹ��
	ZL_ERR_CP_SYNTAX_INVALID_CLASS_FUNCALL,	//�﷨���󣺵����ຯ����ʽ����
	ZL_ERR_CP_SYM_GLOBAL_TABLE_MALLOC_FAILED, //���������죺��zengl_SymInitGlobalTable������ΪSymGlobalTableȫ�ֱ������ű�����ڴ��ʼ��ʧ��
	ZL_ERR_CP_SYM_GLOBAL_TABLE_NO_NOT_VALID_INDEX,		//�������쳣��zengl_SymInsertGlobalTable�����޷���ȫ�ֱ������ű����ҵ�û��ռ�õ�����
	ZL_ERR_CP_SYM_GLOBAL_TABLE_FIND_NOT_VALID_INDEX,	//�������쳣��zengl_SymInsertHashTableForGlobal������SymGlobalTable���ű����ҵ���tmpindex����Ч��
	ZL_ERR_CP_SYM_GLOBAL_TABLE_INDEX_LINECOL_NOT_INIT,	//�������쳣��tmpindex��Ӧ�ķ��ŵ����к�û�г�ʼ��
	ZL_ERR_CP_SYM_GLOBAL_TABLE_INDEX_HAVE_NO_LINECOL_WHEN_PRINT,	//�������쳣��SymGlobalTable�е�[%d]������Ӧ�ķ���û�����к���Ϣ
	ZL_ERR_CP_SYM_CLASS_TABLE_MALLOC_FAILED, //�������쳣����zengl_SymInitClassTable������ΪSymClassTable����ű��Ӧ�Ķ�̬��������ڴ��ʼ��ʧ��
	ZL_ERR_CP_SYM_CLASS_TABLE_NO_NOT_VALID_INDEX,		//�������쳣��zengl_SymInsertClassTable�����޷�������ű����ҵ�û��ռ�õ�����
	ZL_ERR_CP_SYM_CLASS_TABLE_FIND_NOT_VALID_INDEX,		//�������쳣��zengl_SymInsertHashTableForClass������SymClassTable����ű����ҵ���tmpindex����Ч��
	ZL_ERR_CP_SYM_CLASS_HAS_BEEN_DEFINED,	//%s���Ѿ����������
	ZL_ERR_CP_SYM_CLASS_TABLE_INDEX_HAVE_NO_LINECOL_WHEN_PRINT,		//�������쳣��SymClassTable����ű��е�[%d]������Ӧ���ඨ����û�����к���Ϣ
	ZL_ERR_CP_SYM_FUN_TABLE_MALLOC_FAILED,	//�������쳣����zengl_SymInitFunTable������ΪSymFunTable�������ű�����ڴ��ʼ��ʧ��
	ZL_ERR_CP_SYM_FUN_TABLE_NO_NOT_VALID_INDEX,			//�������쳣��zengl_SymInsertFunTable�����޷��ں������ű����ҵ�û��ռ�õ�����
	ZL_ERR_CP_SYM_FUN_TABLE_FIND_NOT_VALID_INDEX,		//�������쳣��zengl_SymInsertHashTableForFun������SymFunTable���������ҵ���tmpindex����Ч��
	ZL_ERR_CP_SYM_FUN_HAS_BEEN_DEFINED,		//%s�ű������Ѿ����������
	ZL_ERR_CP_SYM_FUN_TABLE_INDEX_HAVE_NO_LINECOL_WHEN_PRINT,		//�������쳣��SymFunTable�������ű��е�[%d]������Ӧ�ĺ���������û�����к���Ϣ
	ZL_ERR_CP_SYNTAX_CLASS_NAME_OF_CLS_STMT_NOT_EXISTS,	//�﷨��������'%s'������
	ZL_ERR_CP_SYM_CLASS_TABLE_FIND_NOT_VALID_INDEX_WHEN_LOOKUP,		//�������쳣��zengl_SymLookupClass������SymClassTable����ű����ҵ���tmpindex����Ч������
	ZL_ERR_CP_SYM_CLASSMEMBER_TABLE_MALLOC_FAILED,		//�������쳣����zengl_SymInitClassMemberTableΪSymClassMemberTable���Ա���ű�̬��������ڴ��ʼ��ʧ��
	ZL_ERR_CP_SYM_CLASSMEMBER_TABLE_NO_NOT_VALID_INDEX,	//�������쳣��zengl_SymInsertClassMemberTable�����޷������Ա���ű����ҵ�û��ռ�õ�����
	ZL_ERR_CP_SYM_CLASSMEMBER_TABLE_FIND_NOT_VALID_INDEX,	//�������쳣��zengl_SymInsertHashTableForClassMember������SymClassMemberTable���Ա���ű����ҵ���tmpindex����Ч��
	ZL_ERR_CP_SYNTAX_CLASSMEMBER_HAS_BEEN_DEFINED,		//%s���Ա�Ѿ����������
	ZL_ERR_CP_SYNTAX_ASM_ASSIGN_MUST_HAVE_TWO_CHILD,	//�﷨���󣺸�ֵ�ڵ�����������ӽڵ�
	ZL_ERR_CP_SYM_CAN_NOT_FIND_ID_MEMLOC,	//�������쳣���޷��ҵ�'%s'�������ڴ��ַ
	ZL_ERR_CP_SYM_GLOBAL_TABLE_FIND_NOT_VALID_INDEX_WHEN_LOOKUP,	//�������쳣��zengl_SymLookupID������ȫ�ַ��ű����ҵ���tmpindex����Ч������
	ZL_ERR_CP_SYNTAX_ASM_ASSIGN_SECOND_NODE_IS_INVALID_TYPE,		//�﷨���󣺸�ֵ������ڶ����ӽڵ�Ȳ��Ǳ��������ֻ򸡵������ַ�����Ҳ���ǼӼ��˳��ȱ��ʽ
	ZL_ERR_CP_SYNTAX_ASM_ASSIGN_FIRST_NODE_IS_INVALID_TYPE,			//�﷨���󣺸�ֵ���ĵ�һ���ӽڵ㲻����Ч�Ľṹ
	ZL_ERR_RUN_ERROR_EXIT_WHEN_IN_COMPILE,	// run err exit when in compile 
	ZL_ERR_RUN_MEM_POOL_MALLOC_FAILED,	//�������쳣��Ϊ�������ڴ�ط����ڴ��ʼ��ʧ��
	ZL_ERR_RUN_MEM_ALLOC_FUNC_FAILED,	//�������쳣��memAlloc���������ڴ�ʧ��
	ZL_ERR_RUN_MEM_ALLOC_FUNC_CAN_NOT_FIND_NULL_POINT,	//�������쳣��memAlloc�����޷����ڴ�����ҵ�NULLָ�룬Ҳ���޷������µ�ָ��
	ZL_ERR_RUN_MEM_FREE_POOL_MALLOC_FAILED,	//�������쳣��Ϊ���������ڴ��ͷųط����ڴ��ʼ��ʧ��
	ZL_ERR_RUN_INST_LIST_MALLOC_FAILED,		//�������쳣��Ϊ�������Ļ��ָ������ڴ��ʼ��ʧ��
	ZL_ERR_RUN_MEM_RE_ALLOC_INVALID_POINT,	//�������쳣��memReAllocʱ����Ҫ������ָ�����ڴ������Ч
	ZL_ERR_RUN_MEM_RE_ALLOC_FUNC_FAILED,	//�������쳣��memReAlloc�������·����ڴ�ʧ��
	ZL_ERR_RUN_INST_INVALID_PC,	//�������쳣��ָ���PCֵ��PC�Ĵ������ֵ��һ��
	ZL_ERR_RUN_INST_DATA_STRING_POOL_MALLOC_FAILED,	//�������쳣����zenglrun_initInstDataStringPool������Ϊָ��������ַ����ط����ڴ��ʼ��ʧ��
	ZL_ERR_RUN_INST_DATA_STR_POOL_ADD_I_OUT_OF_BOUNDS, //�������쳣����ָ��������ַ���������ַ���ʱ����i������Χ
	ZL_ERR_RUN_VMEM_LIST_MALLOC_FAILED,		//�������쳣��Ϊȫ�������ڴ涯̬��������ڴ��ʼ��ʧ��
	ZL_ERR_RUN_HASH_ARRAY_MEM_ALLOC_FAILED,		//�������쳣��Ϊ�����ڴ��Ĺ�ϣ�������ϵͳ�ڴ�ʧ��
	ZL_ERR_RUN_MEM_RE_USE_PTR_MALLOC_FAILED,	//�������쳣���������ڴ��ָ��ʱ����������ϵͳ�ڴ�ʧ��
	ZL_ERR_RUN_INVALID_VMEM_TYPE,			//�������쳣����Ч�������ڴ�����
	ZL_ERR_RUN_INVALID_INST_SRC_TYPE,		//�������쳣��ָ����Դ������������Ч
	ZL_ERR_RUN_INVALID_INST_DEST_TYPE,		//�������쳣��ָ����Ŀ�������������Ч
	ZL_ERR_RUN_INVALID_INST_TYPE,			//�������쳣����Ч�Ļ��ָ��
	ZL_ERR_CP_SYNTAX_ASM_INVALID_TOKEN_CAN_NOT_GEN_CODE, //�﷨������Ч��token����tokenĿǰ�޷����ɶ�Ӧ�Ļ��ָ��
	ZL_ERR_CP_SYNTAX_ASM_PRINT_MUST_HAVE_ONE_CHILD,		//�﷨���󣺴�ӡ�ڵ�������һ���ӽڵ�
	ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_TWO_CHILDS, //�﷨���󣺵�ǰ�ڵ������������ӽڵ�
	ZL_ERR_CP_SYNTAX_ASM_INVALID_CHILD_NODE_TYPE,	//�﷨����������Ч���ӽڵ�����
	ZL_ERR_RUN_VSTACK_LIST_MALLOC_FAILED,	//�������쳣��Ϊ�����ջ��̬��������ڴ��ʼ��ʧ��
	ZL_ERR_RUN_VSTACK_LIST_OVERFLOW,		//�������쳣�������ջ���������ջ��������С��0����ǰPC�Ĵ���ֵ��%d
	ZL_ERR_RUN_MOD_BY_ZERO,		//����������ʱ����ȡ������ʱ�������������
	ZL_ERR_RUN_DIVIDE_BY_ZERO,	//����������ʱ���󣺳�������ʱ�������������
	ZL_ERR_RUN_SOURCE_CODE_INFO,//source code info: %s %d:%d <'%s'>
	ZL_ERR_CP_ASM_GCSTACK_LIST_MALLOC_FAILED,		//�������쳣������ջ��̬��������ڴ��ʼ��ʧ��
	ZL_ERR_CP_ASM_GC_LOOP_STACK_LIST_MALLOC_FAILED,		//�������쳣�����ģ���ջ��̬��������ڴ��ʼ��ʧ��
	ZL_ERR_CP_LD_ADDR_LIST_MALLOC_FAILED,	//�������쳣�����ӵ�ַ��̬��������ڴ��ʼ��ʧ��
	ZL_ERR_CP_LD_INDEX_ADDR_MUST_UNIQUE,	//�������쳣�����ӵ�ַ����Ψһ��һ��α��ַ��Ӧһ��������λ��
	ZL_ERR_CP_LD_INVALID_ADDR_INDEX,		//�������쳣����Ч��α��ַ:adr%d
	ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_ONE_CHILD,	//�﷨���󣺵�ǰ�ڵ��������ֻ��һ���ӽڵ�
	ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_AT_LEAST_TWO_CHILDS, //�﷨���󣺵�ǰ�ڵ���������������ӽڵ�
	ZL_ERR_CP_SYNTAX_ASM_INVALID_OP_FACTOR,	//�﷨������Ч�Ĳ�������
	ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_FOUR_CHILDS,	//�﷨���󣺵�ǰ�ڵ��������ֻ���ĸ��ӽڵ�
	ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_THREE_CHILDS,	//�﷨���󣺵�ǰ�ڵ��������ֻ�������ӽڵ�
	ZL_ERR_CP_SYNTAX_ASM_FUN_CAN_NOT_DEFINED_IN_OTHER_FUN,		//�﷨������һ���������ڲ����ٶ�����һ������
	ZL_ERR_CP_SYM_AST_TREE_SCAN_STACK_INVALID_COUNT,	//�������쳣��ASTɨ���ջ���Ԫ�ظ���С��0
	ZL_ERR_CP_SYM_AST_TREE_SCAN_STACK_NOT_EMPTY,		//�������쳣��ASTɨ���ջ��Ϊ��
	ZL_ERR_CP_SYM_LOCAL_TABLE_MALLOC_FAILED,	//�������쳣���ֲ��������ű�̬��������ڴ��ʼ��ʧ��
	ZL_ERR_CP_SYM_LOCAL_TABLE_CAN_NOT_FIND_NOT_VALID_INDEX_WHEN_INSERT, //�������쳣���ֲ����ű��޷��ҵ�û��ռ�õ��������޷���ɷ��Ų������
	ZL_ERR_CP_SYM_LOCAL_TABLE_INVALID_INDEX,	//�������쳣���ֲ����ű�������Ч����
	ZL_ERR_CP_SYNTAX_SYM_GLOBAL_MUST_HAVE_CHILD_NODE,	//�﷨����global���治����Ϊ�սڵ㼴�����
	ZL_ERR_CP_SYM_CAN_NOT_FIND_LOCAL_ID_MEMLOC,	//�������쳣���޷��ҵ�'%s'�������ڴ��ַ
	ZL_ERR_CP_SYM_LOCAL_ID_INVALID_FUN_ID,		//�������쳣���ֲ�����������Ϣ��Ӧ�ĺ���ID��Ч
	ZL_ERR_CP_SYM_LOCAL_ID_INVALID_TYPE,		//�������쳣����Ч�ľֲ���������
	ZL_ERR_RUN_MAIN_INVALID_FUN_RET_ADDRESS,	//����������ʱ���󣺺����Ļ�෵�ص�ַ����������
	ZL_ERR_RUN_MAIN_FUN_VSTACK_STRUCT_EXCEPTION,//����������ʱ���󣺺���������ջ�ṹ�쳣
	ZL_ERR_CP_SYNTAX_SYM_USE_MUST_HAVE_CHILD_NODE,		//�﷨����use�ؼ��ֺ��治����Ϊ�սڵ㼴�����
	ZL_ERR_RUN_MODULE_TALBE_MALLOC_FAILED,		//�������쳣��ģ�鶯̬��������ڴ��ʼ��ʧ��
	ZL_ERR_RUN_MODULE_TALBE_NO_NOT_VALID_INDEX,	//�������쳣��ģ�鶯̬�������޷��ҵ�û��ռ�õ�����
	ZL_ERR_RUN_MODULE_TABLE_FIND_INVALID_INDEX,	//�������쳣��ģ�鶯̬������������Ч������
	ZL_ERR_RUN_MODULE_HAS_BEEN_SET_HANDLE_BEFORE,		//����������ʱ����ģ��'%s'�Ѿ������ù���������
	ZL_ERR_RUN_MOD_FUN_TABLE_MALLOC_FAILED,		//�������쳣��ģ�麯����̬��������ڴ��ʼ��ʧ��
	ZL_ERR_RUN_MOD_FUN_TABLE_NO_NOT_VALID_INDEX,//�������쳣��ģ�麯����̬�������޷��ҵ�û��ռ�õ�����
	ZL_ERR_RUN_MOD_FUN_TABLE_FIND_INVALID_INDEX,//�������쳣��ģ�麯����̬������������Ч������
	ZL_ERR_RUN_MOD_FUN_HAS_BEEN_SET_HANDLE_IN_OTHER_MOD_BEFORE, //����������ʱ����ģ�麯��'%s'�Ѿ���'%s'ģ�������ù���������
	ZL_ERR_RUN_MODULE_IS_INVALID,				//����������ʱ����ģ��'%s'��Ч������ԭ�����û�û�ж����ģ���Use�����������ߴ�������Ч
	ZL_ERR_RUN_FUNCTION_IS_INVALID,				//����������ʱ���󣺺���'%s'��Ч
	ZL_ERR_VM_API_GET_FUN_ARG_ARGNUM_IS_INVALID,//����������ʱ����GetFunArg...�ӿڵ�argnum������Ч������С��1��Ҳ���ɴ��ڲ�������
	ZL_ERR_VM_API_USER_DEFINED_ERROR,			//user defined error �û��Զ���Ĵ�����Ϣ
	ZL_ERR_VM_API_INVALID_FUNCTION_NAME,		//����������ʱ����Call�ӿ��еĺ�������Ч
	ZL_ERR_VM_API_INVALID_SCRIPT_NAME,			//����������ʱ����API�ӿ��еĽű�����Ч
	ZL_ERR_VM_API_VM_MUST_RESET,				//����������ʱ����API�ӿ��е������֮ǰ���ù��ˣ���ʹ��Reset�ӿڽ�������
	ZL_ERR_VM_API_GET_VALUE_AS_MUST_IN_DEBUG_MODE,		//����������ʱ����GetValueAs...֮��Ľӿ�ֻ������DEBUG����ģʽ��
	ZL_ERR_VM_API_GET_VALUE_AS_VALUE_NAME_INVALID,		//����������ʱ����GetValueAs...֮��Ľӿ��е�ȫ�ֱ�������Ч�����ܲ������ڽű���
	ZL_ERR_VM_API_SYM_GLOBAL_TABLE_INVALID_INDEX,		//����������ʱ����ͨ���ӿڲ��ұ�����ʱ��ȫ�ַ��ű��з�����Ч����
	ZL_ERR_VM_API_GET_VALUE_AS_VALUE_RUNTYPE_INVALID,	//����������ʱ����GetValueAs...֮��Ľӿڲ��ҵ��ı���������ʱ������Ч��ֻ�������������������ַ�������
	ZL_ERR_RUN_INVALID_RUNTYPE_IN_GEN_BY_RUNTYPE_MACRO, //����������ʱ����ZENGL_RUN_VMEM_OP_GET_BY_RUNTYPE��������Ч������ʱ����
	ZL_ERR_RUN_MEM_BLOCK_MALLOC_FAILED,			//����������ʱ����Ϊ�ڴ������ڴ��ʼ��ʧ��
	ZL_ERR_RUN_MEM_BLOCK_INVALID_INDEX,			//����������ʱ���󣺶����飬���Ա���ڴ���������ʱ����������ֵ��Ч����������ֵС��0���߳����ڴ�鷶Χ
	ZL_ERR_RUN_MEM_BLOCK_VSTACK_STRUCT_EXCEPTION,		//����������ʱ�����ڴ�������ջ�ṹ�쳣
	ZL_ERR_RUN_ARRAY_CLASS_ITEM_IS_NOT_INIT,	//����������ʱ�����������Ԫ�ز����ڣ������û�б���ʼ����������ֵС��0����ʹ��array������ֱ�Ӹ�ֵ�ȷ�ʽ���г�ʼ��
	ZL_ERR_VM_API_SET_RET_VAL_INVALID_TYPE,		//����������ʱ����SetRetVal�ӿ�������Ч��ֵ����
	ZL_ERR_SYNTAX_SYM_CLASS_STATEMENT_MUST_HAVE_CHILD_NODE,	//�﷨���������������治����Ϊ�սڵ㼴�����
	ZL_ERR_CP_SYM_SCAN_DOT_FOR_CLASS_ID_ZERO_WHEN_NOT_INIT,	//�������쳣��ScanDotForClass�ڷǳ�ʼɨ��״̬��������IDΪ0
	ZL_ERR_SYNTAX_SYM_CAN_NOT_FIND_CLASS_INFO_FOR_ID,		//�﷨���󣺸ñ�������һ�����������ʹ������������������
	ZL_ERR_SYNTAX_INVALID_CLASS_MEMBER,			//�﷨������Ч�����Ա
	ZL_ERR_CP_SYNTAX_ASM_INVALID_RESERVE,				//�﷨������Ч�Ĺؼ���
	ZL_ERR_CP_SYNTAX_ASM_CURRENT_NODE_MUST_HAVE_AT_LEAST_ONE_CHILD, //�﷨���󣺵�ǰ�ڵ����������һ���ӽڵ�
	ZL_ERR_CP_ASM_CASE_JMP_TABLE_MALLOC_FAILED,	//�������쳣��switch...case�ṹ��ת������ڴ��ʼ��ʧ��
	ZL_ERR_CP_ASM_CASE_JMP_TABLE_REALLOC_FAILED,//�������쳣��switch...case�ṹ��ת�����·����ڴ�ʧ��
	ZL_ERR_CP_SYNTAX_ASM_SWITCH_FIND_SAME_CASE_VAL,		//�﷨����switch�ṹ�з�����ͬ��caseֵ
	ZL_ERR_CP_ASM_CASE_JMP_TABLE_CASEADDR_CAN_NOT_BE_ZERO,			//�������쳣��switch...case�ṹ��ת����case��Ŀ�����λ�ò���Ϊ0
	ZL_ERR_RUN_SWITCH_HAVE_NO_LONG_INST_BELOW,	//�������쳣��SWITCHָ����������LONGָ��
	ZL_ERR_VM_API_INVALID_CLASS_NAME_WHEN_CALL,	//����������ʱ����Call�ӿ��е�������Ч
	ZL_ERR_VM_API_INVALID_CLASS_TABLE_INDEX_WHEN_CALL,	//����������ʱ�쳣��Call�ӿڵ���ʱ�ҵ���Ч��������
	ZL_ERR_VM_API_INVALID_MEMBLOCK_ARG_IN_CREATE_MEM_BLOCK,			//����������ʱ����CreateMemBlock�ӿ���memblock������Ч������Ϊ��ָ��
	ZL_ERR_VM_API_INVALID_SIZE_ARG_IN_CREATE_MEM_BLOCK,	//����������ʱ����CreateMemBlock�ӿ���size������Ч��size������ڵ���0
	ZL_ERR_VM_API_INVALID_MEMBLOCK_ARG_IN_SET_RETVAL,	//����������ʱ����SetRetValAsMemBlock�ӿ���memblock������Ч������Ϊ��ָ��
	ZL_ERR_VM_API_INVALID_ARGNUM_ARG_IN_SET_FUN_ARG,	//����������ʱ����SetFunArg�ӿ���argnum������Ч
	ZL_ERR_VM_API_INVALID_MEMBLOCK_ARG_IN_GET_MEM_BLOCK_SIZE,		//����������ʱ����GetMemBlockSize�ӿ���memblock������Ч������Ϊ��ָ��
	ZL_ERR_VM_API_INVALID_MEMBLOCK_ARG_IN_GET_MEM_BLOCK_NNCOUNT,	//����������ʱ����GetMemBlockNNCount�ӿ���memblock������Ч������Ϊ��ָ��
	ZL_ERR_VM_API_INIT_EXTRA_DATA_TABLE_FAILED,	//����������ʱ�����û��������ݶ�̬��������ڴ��ʼ��ʧ��
	ZL_ERR_VM_API_CAN_NOT_FIND_NOT_VALID_INDEX_IN_EXTRA_DATA_TABLE,	//����������ʱ���󣺶������ݱ����޷��ҵ�û��ռ�õ��������޷������ӹ���
	ZL_ERR_VM_API_INVALID_EXTRA_DATA_NAME_OR_POINT,		//����������ʱ���󣺶����������ƻ��Ӧ��ָ��Ϊ��
	ZL_ERR_VM_API_EXTRA_DATA_TABLE_INVALID_INDEX,		//����������ʱ���󣺶������ݶ�̬�����з�����Ч������
	ZL_ERR_VM_API_EXTRA_DATA_NAME_EXIST,		//����������ʱ���󣺶�����������'%s'�Ѿ�����
	ZL_ERR_VM_API_EXTRA_DATA_NAME_INVALID,		//����������ʱ���󣺶�����������'%s'��Ч
	ZL_ERR_CP_SYNTAX_ASM_IF_HAVE_NO_EXPRESS,	//�﷨����if���������жϱ��ʽΪ��
	ZL_ERR_VM_API_INVALID_CALL_POSITION,		//����������ʱ����'%s'��API�ӿڵĵ���λ�ò���
	ZL_ERR_VM_API_INVALID_ZLVM,					//����������ʱ����API�ӿ��з��ʵ�����Ч�����������ͨ��zenglApi_Open������Ч�������
	ZL_ERR_VM_API_INVALID_ARG_TYPE_WHEN_PUSH,		//����������ʱ����zenglApi_Push�ӿڵĵڶ�����������Ч�Ĳ�������
	ZL_ERR_VM_API_INVALID_RETVAL_TYPE_WHEN_SETMEMBLOCK,	//����������ʱ����zenglApi_SetMemBlock�ӿ������һ�����������ʹ���
	ZL_ERR_VM_API_INVALID_SRC_WHEN_ALLOCMEM_FOR_STRING,	//����������ʱ����zenglApi_AllocMemForStringԴ�ַ���ָ�������Ч
	ZL_ERR_VM_API_INVALID_SIZE_WHEN_ALLOCMEM,	//����������ʱ����zenglApi_AllocMem�е�size������Ч
	ZL_ERR_VM_API_INVALID_PTR_WHEN_FREEMEM,		//����������ʱ����zenglApi_FreeMem�е�ptrָ�������Ч
	ZL_ERR_VM_API_INVALID_SCRIPT_STR_WHEN_RUNSTR,		//����������ʱ����zenglApi_RunStr�ӿ��е��ַ����ű���Ч
	ZL_ERR_VM_API_DEBUGVM_OPEN_FAILED,			//����������ʱ����%s�е�DeubugVM����ʧ��
	ZL_ERR_VM_API_INVALID_DEBUG_STR,			//����������ʱ����%s�е�debug_str�����ַ�����Ч
	ZL_ERR_VM_API_DEBUG_ERR,					//DebugErr:%s
	ZL_ERR_VM_API_INIT_DEBUG_BREAK_FAILED,		//����������ʱ���󣺵������ϵ��ʼ��ʧ��
	ZL_ERR_VM_API_SET_DEBUG_BREAK_FAILED,		//����������ʱ���󣺵��������öϵ�ʧ��
	ZL_ERR_VM_API_INVALID_BREAK_LOCATION,		//��Ч�Ķϵ�λ��
	ZL_ERR_VM_API_DEBUG_TRACE_INVALID_ARGS,		//���������нű�����׷��ʱ���ṩ��API�ӿ�'%s'�Ĳ�����Ч
	ZL_ERR_VM_API_INVALID_PTR_WHEN_REALLOC,		//����������ʱ����zenglApi_ReAllocMem�е�ptrָ�������Ч
	ZL_ERR_VM_API_FILENAME_TOO_LONG, //����������ʱ�����ļ�·����Ϣ̫��
	ZL_ERR_VM_API_FILENAME_TOO_LONG_WHEN_MERGE, //����������ʱ���󣺺ϲ�����ļ�·����Ϣ̫��
	ZL_ERR_VM_API_INVALID_ARG,	//����������ʱ�����ṩ��API�ӿ�'%s'�Ĳ�����Ч
	ZL_ERR_VM_API_CACHE_SEARCH_MEMPOOL_POINT_FAILED, //����������ʱ����API����ӿ�'%s'�����ڴ��ָ��ʧ��
	ZL_ERR_VM_API_CACHE_INVALID_CACHE_DATA, //����������ʱ����API����ӿ�'%s'��ȡ���Ļ���������Ч
}ZENGL_ERRORNO;
/*���ִ����ö�ٶ������*/

/*���ֹؼ��ֵ�ö�ٶ���*/
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
/*���ֹؼ��ֵ�ö�ٶ������*/

#endif/* _ZENGL_LOCALS_H_ */
