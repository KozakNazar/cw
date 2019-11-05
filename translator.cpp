#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
/*************************************************************
* N.Kozak // Lviv'2018 // Compiler-compiler (example for SP) *
*                   file: translator.cpp                     *
*                                 h-files:                   *
*                                         coder.h            *
*                                         compiler.h         *
*                                         config.h           *
*                                         faststr.h          *
*                                         lexer.h            *
*                                         memorypool.h       *
*                                         scanner.h          *
*                                         semantixer.h       *
*                                         syntaxer.h         *
*                                         tables.h           *
*                                         translator.h       *
*                               cpp-files:                   *
*                                         application.cpp    *
*                                         coder.cpp          *
*                                         compiler.cpp       *
*                                         faststr.cpp        *
*                                         lexer.cpp          *
*                                         memorypool.cpp     *
*                                         scanner.cpp        *
*                                         semantixer.cpp     *
*                                         syntaxer.cpp       *
*                                         tables.cpp         *
*                                         translator.cpp     *
**************************************************************/
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "tables.h"
#include "scanner.h"
#include "lexer.h"
#include "syntaxer.h"
#include "translator.h"

char * sourceCode_f1__OLD = (char*)
"NAME _PROGRAM ; \r\n"
"BODY DATA INTEGER16 _VALUEIN , _RESULTO , _CYCLEAC ;\r\n"
"  GET ( _VALUEIN )\r\n"
"  1 >> _RESULTO\r\n"
"  FOR  0 >> _CYCLEAC TO 32767 DO\r\n"
"    IF _VALUEIN != 0 THEN ; ELSE GOTO endcyclea ;\r\n"
"    _RESULTO * _VALUEIN >> _RESULTO \r\n"
"    _VALUEIN - 1 >> _VALUEIN\r\n"
"  ;\r\n"
"  endcyclea :\r\n"
"  PUT ( _RESULTO )\r\n"
"  END\r\n"
"\r\n"
;

char * sourceCode_f2__OLD = (char*)
"NAME _PROGRAM ; \r\n"
"BODY DATA INTEGER16 _VALUEIN , _DEVIDER , _REMAIND , _CYCLEAC , _CYCLEBC , _CYCLECC ;\r\n"
"  GET ( _VALUEIN )\r\n"
"  1 + _VALUEIN >> _VALUEIN\r\n"
"  FOR  0 >> _CYCLEAC TO 32767 DO\r\n"
"    _VALUEIN - 1 >> _DEVIDER\r\n"
"    FOR  0 >> _CYCLEBC TO 32767 DO \r\n"
"      IF _DEVIDER >= 2 THEN ; ELSE GOTO endcycleb ;\r\n"
"      _VALUEIN >> _REMAIND\r\n"
"      FOR  0 >> _CYCLECC TO 32767 DO\r\n"
"        IF _REMAIND >= _DEVIDER THEN ; ELSE GOTO endcyclec ;\r\n"
"        _REMAIND - _DEVIDER >> _REMAIND\r\n"
"      ;\r\n"
"      endcyclec :\r\n"
"      IF _REMAIND == 0 THEN GOTO endcycleb ;\r\n"
"      _DEVIDER - 1 >> _DEVIDER\r\n"
"    ;\r\n"
"    endcycleb :\r\n"
"    IF _DEVIDER == 1 THEN GOTO endcyclea ;\r\n"
"    1 + _VALUEIN >> _VALUEIN\r\n"
"  ;\r\n"
"  endcyclea :\r\n"
"  PUT ( _VALUEIN )\r\n"
"  END\r\n"
"\r\n"
;

// X = K + B - D / 2 + E * 4
// K = 25630
// B = 4
// D = 5
// E = 6
char * sourceCode_f3__OLD = (char*)
"NAME _PROGRAM ; \r\n"
"BODY DATA INTEGER16 _BBBBBBB , _DDDDDDD , _EEEEEEE , _KKKKKKK ;\r\n"
"  25630 >> _KKKKKKK\r\n"
"  GET ( _BBBBBBB )\r\n"
"  GET ( _DDDDDDD )\r\n"
"  GET ( _EEEEEEE )\r\n"
"  PUT ( _KKKKKKK + _BBBBBBB - _DDDDDDD DIV 2 + _EEEEEEE * 4 )\r\n"
"  END"
;

struct SourceNotations
commentsNotations[] = {
	{ "~~", 0, { { "~~", 0 }, { "", 0 } } },

	{ "<not_end_notations_block>", ABSTRACT_TOKEN_TYPE | VARIATIVE_EXCLUSION_PATERN_FOR_FIND_SUBNON_ABSTRACT_TOKEN_TYPE_TA, { { "~~", 0 }, { "*/", KEYWORD_TOKEN_TYPE }, { "", 0 } } },

	{ "<notations_block>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA, { { "~~", 0 }, { "/*", KEYWORD_TOKEN_TYPE }, { "<not_end_notations_block>", ABSTRACT_TOKEN_TYPE }, { "*/", KEYWORD_TOKEN_TYPE }, { "", 0 } } },

	{ "", 0, { { "~~", 0 }, { "", 0 } } }
},
lexemNotations[] = {
	//<>
	{ "~~", 0, { { "", 0 } } },

	{ "<number>", ABSTRACT_TOKEN_TYPE | VARIATIVE_PATERN_TYPE_TA, { { "~~", 0 }, { "0", 0 }, { "1", 0 }, { "2", 0 }, { "3", 0 }, { "4", 0 }, { "5", 0 }, { "6", 0 }, { "7", 0 }, { "8", 0 }, { "9", 0 }, { "", 0 } } },

	{ "<value>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA, { { "~~", 0 }, { "<number>", ABSTRACT_TOKEN_TYPE }, { "<number>", ABSTRACT_TOKEN_TYPE | REPEAT_TOKEN_ATRIBUTE_TYPE_TA | MANDATORY_TOKEN_ATRIBUTE_TYPE_TA }, { "", 0 } } },

	{ "<letter_in_lower_case>", ABSTRACT_TOKEN_TYPE | VARIATIVE_PATERN_TYPE_TA, { { "~~", 0 }, { "a", 0 }, { "b", 0 }, { "c", 0 }, { "d", 0 }, { "e", 0 }, { "f", 0 }, { "g", 0 }, { "h", 0 }, { "i", 0 }, { "j", 0 }, { "k", 0 }, { "l", 0 }, { "n", 0 }, { "m", 0 }, { "o", 0 }, { "p", 0 }, { "q", 0 }, { "r", 0 }, { "s", 0 }, { "t", 0 }, { "u", 0 }, { "v", 0 }, { "w", 0 }, { "x", 0 }, { "y", 0 }, { "z", 0 }, { "", 0 } } },

	{ "<letter_in_upper_case>", ABSTRACT_TOKEN_TYPE | VARIATIVE_PATERN_TYPE_TA, { { "~~", 0 }, { "A", 0 }, { "B", 0 }, { "C", 0 }, { "D", 0 }, { "E", 0 }, { "F", 0 }, { "G", 0 }, { "H", 0 }, { "I", 0 }, { "J", 0 }, { "K", 0 }, { "L", 0 }, { "N", 0 }, { "M", 0 }, { "O", 0 }, { "P", 0 }, { "Q", 0 }, { "R", 0 }, { "S", 0 }, { "T", 0 }, { "U", 0 }, { "V", 0 }, { "W", 0 }, { "X", 0 }, { "Y", 0 }, { "Z", 0 }, { "", 0 } } },

	{ "<ident>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA, { { "~~", 0 }, { "_", 0 }, { "<letter_in_upper_case>", ABSTRACT_TOKEN_TYPE }, { "<letter_in_upper_case>", ABSTRACT_TOKEN_TYPE }, { "<letter_in_upper_case>", ABSTRACT_TOKEN_TYPE }, { "<letter_in_upper_case>", ABSTRACT_TOKEN_TYPE }, { "<letter_in_upper_case>", ABSTRACT_TOKEN_TYPE }, { "<letter_in_upper_case>", ABSTRACT_TOKEN_TYPE }, { "<letter_in_upper_case>", ABSTRACT_TOKEN_TYPE }, { "", 0 } } },

	{ "<label>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA, { { "~~", 0 }, { "<letter_in_lower_case>", ABSTRACT_TOKEN_TYPE }, { "<letter_in_lower_case>", ABSTRACT_TOKEN_TYPE | REPEAT_TOKEN_ATRIBUTE_TYPE_TA | MANDATORY_TOKEN_ATRIBUTE_TYPE_TA }, { "", 0 } } },

	{ "", 0, { { "~~", 0 }, { "", 0 } } }
},
sourceNotations[] = {
	//<>
	{ "~~", 0, { { "", 0 } } },

	{ "<recursive_descent_end_point>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA, { { "~~", 0 }, { "RECURSIVE_DESCENT_END_POINT", KEYWORD_TOKEN_TYPE }, { "", 0 } } },

	//

	{ "<value_read>", ABSTRACT_TOKEN_TYPE | VARIATIVE_PATERN_TYPE_TA, { { "~~", 0 }, { "<value>", ABSTRACT_TOKEN_TYPE }, { "", 0 } } },

	{ "<ident_read>", ABSTRACT_TOKEN_TYPE | VARIATIVE_PATERN_TYPE_TA, { { "~~", 0 }, { "<ident>", ABSTRACT_TOKEN_TYPE }, { "", 0 } } },

	{ "<ident_write>", ABSTRACT_TOKEN_TYPE | VARIATIVE_PATERN_TYPE_TA, { { "~~", 0 }, { "<ident>", ABSTRACT_TOKEN_TYPE }, { "", 0 } } },
	
	//

	{ "<labeled_point>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA, { { "~~", 0 }, { "<label>", ABSTRACT_TOKEN_TYPE }, { ":", KEYWORD_TOKEN_TYPE }, { "", 0 } } },

	{ "<goto_label>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA, { { "~~", 0 }, { "GOTO", KEYWORD_TOKEN_TYPE }, { "<label>", ABSTRACT_TOKEN_TYPE }, { "", 0 } } },

	{ "<program_name>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA, { { "~~", 0 }, { "<ident>", ABSTRACT_TOKEN_TYPE }, { "", 0 } } },

	{ "<value_type>", ABSTRACT_TOKEN_TYPE | VARIATIVE_PATERN_TYPE_TA, { { "~~", 0 }, { "INTEGER16", KEYWORD_TOKEN_TYPE }, { "", 0 } } },

	{ "<declaration_ident>", ABSTRACT_TOKEN_TYPE | VARIATIVE_PATERN_TYPE_TA, { { "~~", 0 }, { "<ident>", ABSTRACT_TOKEN_TYPE }, { "", 0 } } },

	{ "<other_declaration_ident>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA, { { "~~", 0 }, { ",", KEYWORD_TOKEN_TYPE }, { "<ident>", ABSTRACT_TOKEN_TYPE }, { "", 0 } } },

	{ "<declaration>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA, { { "~~", 0 }, { "<value_type>", ABSTRACT_TOKEN_TYPE }, { "<declaration_ident>", ABSTRACT_TOKEN_TYPE }, { "<other_declaration_ident>", ABSTRACT_TOKEN_TYPE | REPEAT_TOKEN_ATRIBUTE_TYPE_TA | MANDATORY_TOKEN_ATRIBUTE_TYPE_TA }, { "", 0 } } },

	{ "<operation_not>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA, { { "~~", 0 }, { "NOT", KEYWORD_TOKEN_TYPE }, { "<inseparable_expression>", ABSTRACT_TOKEN_TYPE }, { "", 0 } } },

	{ "<and_action>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA, { { "~~", 0 }, { "AND", KEYWORD_TOKEN_TYPE }, { "<inseparable_expression>", ABSTRACT_TOKEN_TYPE }, { "", 0 } } },

	{ "<or_action>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA, { { "~~", 0 }, { "OR", KEYWORD_TOKEN_TYPE }, { "<high_prioryty_expression>", ABSTRACT_TOKEN_TYPE }, { "", 0 } } },

	{ "<equal_action>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA, { { "~~", 0 }, { "==", KEYWORD_TOKEN_TYPE }, { "<middle_prioryty_expression>", ABSTRACT_TOKEN_TYPE }, { "", 0 } } },

	{ "<not_equal_action>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA, { { "~~", 0 }, { "!=", KEYWORD_TOKEN_TYPE }, { "<middle_prioryty_expression>", ABSTRACT_TOKEN_TYPE }, { "", 0 } } },

	{ "<less_or_equal_action>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA, { { "~~", 0 }, { "<=", KEYWORD_TOKEN_TYPE }, { "<middle_prioryty_expression>", ABSTRACT_TOKEN_TYPE }, { "", 0 } } },

	{ "<greater_or_equal_action>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA, { { "~~", 0 }, { ">=", KEYWORD_TOKEN_TYPE }, { "<middle_prioryty_expression>", ABSTRACT_TOKEN_TYPE }, { "", 0 } } },

	{ "<add_action>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA, { { "~~", 0 }, { "+", KEYWORD_TOKEN_TYPE }, { "<high_prioryty_expression>", ABSTRACT_TOKEN_TYPE }, { "", 0 } } },

	{ "<sub_action>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA, { { "~~", 0 }, { "-", KEYWORD_TOKEN_TYPE }, { "<high_prioryty_expression>", ABSTRACT_TOKEN_TYPE }, { "", 0 } } },

	{ "<mul_action>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA, { { "~~", 0 }, { "*", KEYWORD_TOKEN_TYPE }, { "<inseparable_expression>", ABSTRACT_TOKEN_TYPE }, { "", 0 } } },

	{ "<div_action>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA, { { "~~", 0 }, { "DIV", KEYWORD_TOKEN_TYPE }, { "<inseparable_expression>", ABSTRACT_TOKEN_TYPE }, { "", 0 } } },

	{ "<mod_action>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA, { { "~~", 0 }, { "MOD", KEYWORD_TOKEN_TYPE }, { "<inseparable_expression>", ABSTRACT_TOKEN_TYPE }, { "", 0 } } },
    
	{ "<unary_operation>", ABSTRACT_TOKEN_TYPE | VARIATIVE_PATERN_TYPE_TA, { { "~~", 0 }, { "<operation_not>", ABSTRACT_TOKEN_TYPE }, { "", 0 } } },

	//

	{ "<inseparable_expression>", ABSTRACT_TOKEN_TYPE | VARIATIVE_PATERN_TYPE_TA, { { "~~", 0 }, { "<group_expression>", ABSTRACT_TOKEN_TYPE }, { "<unary_operation>", ABSTRACT_TOKEN_TYPE }, { "<ident_read>", ABSTRACT_TOKEN_TYPE }, { "<value_read>", ABSTRACT_TOKEN_TYPE }, { "", 0 } } },
  
	//

	{ "<high_prioryty_left_expression>", ABSTRACT_TOKEN_TYPE | VARIATIVE_PATERN_TYPE_TA, { { "~~", 0 }, { "<high_prioryty_expression>", ABSTRACT_TOKEN_TYPE }, { "<group_expression>", ABSTRACT_TOKEN_TYPE }, { "<unary_operation>", ABSTRACT_TOKEN_TYPE }, { "<ident_read>", ABSTRACT_TOKEN_TYPE }, { "<value_read>", ABSTRACT_TOKEN_TYPE }, { "", 0 } } },

	{ "<high_prioryty_action>", ABSTRACT_TOKEN_TYPE | VARIATIVE_PATERN_TYPE_TA, { { "~~", 0 }, { "<recursive_descent_end_point>", ABSTRACT_TOKEN_TYPE }, { "<mul_action>", ABSTRACT_TOKEN_TYPE }, { "<div_action>", ABSTRACT_TOKEN_TYPE }, { "<mod_action>", ABSTRACT_TOKEN_TYPE }, { "<and_action>", ABSTRACT_TOKEN_TYPE }, { "", 0 } } },

	{ "<high_prioryty_expression>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA, { { "~~", 0 }, { "<high_prioryty_left_expression>", ABSTRACT_TOKEN_TYPE }, { "<high_prioryty_action>", ABSTRACT_TOKEN_TYPE | MANDATORY_TOKEN_ATRIBUTE_TYPE_TA }, { "", 0 } } },

	//

	{ "<middle_prioryty_left_expression>", ABSTRACT_TOKEN_TYPE | VARIATIVE_PATERN_TYPE_TA, { { "~~", 0 }, { "<middle_prioryty_expression>", ABSTRACT_TOKEN_TYPE }, { "<high_prioryty_expression>", ABSTRACT_TOKEN_TYPE }, { "<group_expression>", ABSTRACT_TOKEN_TYPE }, { "<unary_operation>", ABSTRACT_TOKEN_TYPE }, { "<ident_read>", ABSTRACT_TOKEN_TYPE }, { "<value_read>", ABSTRACT_TOKEN_TYPE }, { "", 0 } } },

	{ "<middle_prioryty_action>", ABSTRACT_TOKEN_TYPE | VARIATIVE_PATERN_TYPE_TA, { { "~~", 0 }, { "<recursive_descent_end_point>", ABSTRACT_TOKEN_TYPE }, { "<add_action>", ABSTRACT_TOKEN_TYPE }, { "<sub_action>", ABSTRACT_TOKEN_TYPE }, { "<or_action>", ABSTRACT_TOKEN_TYPE }, { "", 0 } } },

	{ "<middle_prioryty_expression>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA, { { "~~", 0 }, { "<middle_prioryty_left_expression>", ABSTRACT_TOKEN_TYPE }, { "<middle_prioryty_action>", ABSTRACT_TOKEN_TYPE | MANDATORY_TOKEN_ATRIBUTE_TYPE_TA }, { "", 0 } } },

	//

	{ "<low_prioryty_left_expression>", ABSTRACT_TOKEN_TYPE | VARIATIVE_PATERN_TYPE_TA, { { "~~", 0 }, { "<low_prioryty_expression>", ABSTRACT_TOKEN_TYPE }, { "<middle_prioryty_expression>", ABSTRACT_TOKEN_TYPE }, { "<high_prioryty_expression>", ABSTRACT_TOKEN_TYPE }, { "<group_expression>", ABSTRACT_TOKEN_TYPE }, { "<unary_operation>", ABSTRACT_TOKEN_TYPE }, { "<ident_read>", ABSTRACT_TOKEN_TYPE }, { "<value_read>", ABSTRACT_TOKEN_TYPE }, { "", 0 } } },

	{ "<low_prioryty_action>", ABSTRACT_TOKEN_TYPE | VARIATIVE_PATERN_TYPE_TA, { { "~~", 0 }, { "<recursive_descent_end_point>", ABSTRACT_TOKEN_TYPE }, { "<less_or_equal_action>", ABSTRACT_TOKEN_TYPE }, { "<greater_or_equal_action>", ABSTRACT_TOKEN_TYPE }, { "<equal_action>", ABSTRACT_TOKEN_TYPE }, { "<not_equal_action>", ABSTRACT_TOKEN_TYPE }, { "", 0 } } },

	{ "<low_prioryty_expression>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA, { { "~~", 0 }, { "<low_prioryty_left_expression>", ABSTRACT_TOKEN_TYPE }, { "<low_prioryty_action>", ABSTRACT_TOKEN_TYPE | MANDATORY_TOKEN_ATRIBUTE_TYPE_TA }, { "", 0 } } },

	//

	{ "<expression>", ABSTRACT_TOKEN_TYPE | VARIATIVE_PATERN_TYPE_TA, { { "~~", 0 }, { "<low_prioryty_expression>", ABSTRACT_TOKEN_TYPE }, { "", 0 } } },

	//

	{ "<group_expression>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA, { { "~~", 0 }, { "(", KEYWORD_TOKEN_TYPE }, { "<expression>", ABSTRACT_TOKEN_TYPE }, { ")", KEYWORD_TOKEN_TYPE }, { "", 0 } } },

	//

	{ "<bind>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA, { { "~~", 0 }, { "<expression>", ABSTRACT_TOKEN_TYPE }, { ">>", KEYWORD_TOKEN_TYPE }, { "<ident_write>", ABSTRACT_TOKEN_TYPE }, { "", 0 } } },

    //

	{ "<if_expression>", ABSTRACT_TOKEN_TYPE | VARIATIVE_PATERN_TYPE_TA, { { "~~", 0 }, { "<low_prioryty_expression>", ABSTRACT_TOKEN_TYPE }, { "", 0 } } },

	{ "<body_for_true>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA, { { "~~", 0 }, { "THEN", KEYWORD_TOKEN_TYPE }, { "<statement>", ABSTRACT_TOKEN_TYPE | REPEAT_TOKEN_ATRIBUTE_TYPE_TA | MANDATORY_TOKEN_ATRIBUTE_TYPE_TA }, { ";", KEYWORD_TOKEN_TYPE }, { "", 0 } } },

	{ "<body_for_false>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA, { { "~~", 0 }, { "ELSE", KEYWORD_TOKEN_TYPE }, { "<statement>", ABSTRACT_TOKEN_TYPE | REPEAT_TOKEN_ATRIBUTE_TYPE_TA | MANDATORY_TOKEN_ATRIBUTE_TYPE_TA }, { ";", KEYWORD_TOKEN_TYPE }, { "", 0 } } },

	{ "<cond_block>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA, { { "~~", 0 }, { "IF", KEYWORD_TOKEN_TYPE }, { "<if_expression>", ABSTRACT_TOKEN_TYPE }, { "<body_for_true>", ABSTRACT_TOKEN_TYPE }, { "<body_for_false>", ABSTRACT_TOKEN_TYPE | MANDATORY_TOKEN_ATRIBUTE_TYPE_TA }, { "", 0 } } },

    //

	{ "<cycle_begin_expression>", ABSTRACT_TOKEN_TYPE | VARIATIVE_PATERN_TYPE_TA, { { "~~", 0 }, { "<low_prioryty_expression>", ABSTRACT_TOKEN_TYPE }, { "", 0 } } },

	{ "<cycle_counter>", ABSTRACT_TOKEN_TYPE | VARIATIVE_PATERN_TYPE_TA, { { "~~", 0 }, { "<ident>", ABSTRACT_TOKEN_TYPE }, { "", 0 } } },

	{ "<cycle_counter_last_value>", ABSTRACT_TOKEN_TYPE | VARIATIVE_PATERN_TYPE_TA, { { "~~", 0 }, { "<value>", ABSTRACT_TOKEN_TYPE }, { "", 0 } } },

	{ "<cycle_body>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA, { { "~~", 0 }, { "DO", KEYWORD_TOKEN_TYPE }, { "<statement>", ABSTRACT_TOKEN_TYPE }, { "<statement>", ABSTRACT_TOKEN_TYPE | REPEAT_TOKEN_ATRIBUTE_TYPE_TA | MANDATORY_TOKEN_ATRIBUTE_TYPE_TA }, { "", 0 } } },

	{ "<forto_cycle>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA, { { "~~", 0 }, { "FOR", KEYWORD_TOKEN_TYPE }, { "<cycle_begin_expression>", ABSTRACT_TOKEN_TYPE }, { ">>", KEYWORD_TOKEN_TYPE }, { "<cycle_counter>", ABSTRACT_TOKEN_TYPE }, { "TO", KEYWORD_TOKEN_TYPE }, { "<cycle_counter_last_value>", ABSTRACT_TOKEN_TYPE }, { "<cycle_body>", ABSTRACT_TOKEN_TYPE }, { ";", KEYWORD_TOKEN_TYPE }, { "", 0 } } },

	//

	{ "<cycle_head_expression>", ABSTRACT_TOKEN_TYPE | VARIATIVE_PATERN_TYPE_TA, { { "~~", 0 }, { "<low_prioryty_expression>", ABSTRACT_TOKEN_TYPE }, { "", 0 } } },

	{ "<while_cycle>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA, { { "~~", 0 }, { "WHILE", KEYWORD_TOKEN_TYPE }, { "<cycle_head_expression>", ABSTRACT_TOKEN_TYPE }, { "<cycle_body>", ABSTRACT_TOKEN_TYPE }, { ";", KEYWORD_TOKEN_TYPE }, { "", 0 } } },

	//

	{ "<input>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA, { { "~~", 0 }, { "GET", KEYWORD_TOKEN_TYPE }, { "(", KEYWORD_TOKEN_TYPE }, { "<ident_write>", ABSTRACT_TOKEN_TYPE }, { ")", KEYWORD_TOKEN_TYPE }, { "", 0 } } },

	{ "<output>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA, { { "~~", 0 }, { "PUT", KEYWORD_TOKEN_TYPE }, { "(", KEYWORD_TOKEN_TYPE }, { "<expression>", ABSTRACT_TOKEN_TYPE }, { ")", KEYWORD_TOKEN_TYPE }, { "", 0 } } },

	{ "<statement>", ABSTRACT_TOKEN_TYPE | VARIATIVE_PATERN_TYPE_TA, { { "~~", 0 }, { "<recursive_descent_end_point>", ABSTRACT_TOKEN_TYPE }, { "<bind>", ABSTRACT_TOKEN_TYPE }, { "<cond_block>", ABSTRACT_TOKEN_TYPE }, { "<forto_cycle>", ABSTRACT_TOKEN_TYPE }, { "<while_cycle>", ABSTRACT_TOKEN_TYPE }, { "<labeled_point>", ABSTRACT_TOKEN_TYPE }, { "<goto_label>", ABSTRACT_TOKEN_TYPE }, { "<input>", ABSTRACT_TOKEN_TYPE }, { "<output>", ABSTRACT_TOKEN_TYPE }, { "", 0 } } },

	{ "<program>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA, { { "~~", 0 }, { "NAME", KEYWORD_TOKEN_TYPE }, { "<program_name>", 0 }, { ";", KEYWORD_TOKEN_TYPE }, { "BODY", KEYWORD_TOKEN_TYPE }, { "DATA", KEYWORD_TOKEN_TYPE }, { "<declaration>", ABSTRACT_TOKEN_TYPE | (REPEAT_TOKEN_ATRIBUTE_TYPE_TA | 0/*~0*/) | MANDATORY_TOKEN_ATRIBUTE_TYPE_TA }, { ";", KEYWORD_TOKEN_TYPE }, { "<statement>", ABSTRACT_TOKEN_TYPE | REPEAT_TOKEN_ATRIBUTE_TYPE_TA | MANDATORY_TOKEN_ATRIBUTE_TYPE_TA }, { "END", KEYWORD_TOKEN_TYPE }, { "", 0 } } },

	{ "", 0, { { "~~", 0 }, { "", 0 }, { "", 0 }, { "", 0 }, { "", 0 }, { "", 0 }, { "", 0 }, { "", 0 }, { "", 0 }, { "", 0 } } }
},
productionNotations[] = {
	//<>
	{ "~~", 0, { { "", 0 } } },

	{ "<value>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | PRE_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "", 0 }, { "", 0 } } },

	{ "<value>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "", 0 }, { "", 0 } } },

	{ "<ident>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | PRE_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "", 0 }, { "", 0 } } },

	{ "<label>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "", 0 }, { "", 0 } } },

	//

	{ "<recursive_descent_end_point>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "", 0 }, { "", 0 } } },

	//

	{ "<value_read>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | PRE_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "    mov ax, ", 0 }, { "", 0 } } },

	{ "<value_read>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { ";\r\n    push ax;\r\n", 0 }, { "", 0 } } },

	{ "<ident_read>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | PRE_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "    mov ax, ", 0 }, { "", 0 } } },

	{ "<ident_read>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { ";\r\n    push ax;\r\n", 0 }, { "", 0 } } },

	{ "<ident_write>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | PRE_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "    pop ax;\r\n    mov ", 0 }, { "", 0 } } },

	{ "<ident_write>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { ", ax;\r\n", 0 }, { "", 0 } } },

	//
  
	{ "<labeled_point>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | PRE_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "    ", 0 }, { "", 0 } } },

	{ "<labeled_point>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { ":\r\n", 0 }, { "", 0 } } },

	{ "<goto_label>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | PRE_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "    jmp ", 0 }, { "", 0 } } },

	{ "<goto_label>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { ";\r\n", 0 }, { "", 0 } } },

	{ "<program_name>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | PRE_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { ";", 0 }, { "", 0 } } },

	{ "<program_name>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "\r\n", 0 }, { "", 0 } } },

	{ "<value_type>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "", 0 }, { "", 0 } } },

	{ "<declaration_ident>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | PRE_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "    ", 0 }, { "", 0 } } },

	{ "<declaration_ident>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { " dw ?;\r\n", 0 }, { "", 0 } } },
   
	{ "<other_declaration_ident>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | PRE_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "    ", 0 }, { "", 0 } } },

	{ "<other_declaration_ident>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { " dw ?;\r\n", 0 }, { "", 0 } } },

	{ "<declaration>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | PRE_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "\r\n.data?\r\n", 0 }, { "", 0 } } },

	{ "<declaration>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { ";end data seg.\r\n\r\n.code\r\nmain PROC C\r\n", 0 }, { "", 0 } } },

	{ "<operation_not>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "    pop ax;\r\n    not ax;\r\n    push ax;\r\n", 0 }, { "", 0 } } },

	{ "<and_action>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "    pop bx;\r\n    pop ax;\r\n    and ax, bx;\r\n    push ax;\r\n", 0 }, { "", 0 } } },

	{ "<or_action>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "    pop bx;\r\n    pop ax;\r\n    or ax, bx;\r\n    push ax;\r\n", 0 }, { "", 0 } } },

	{ "<equal_action>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "    pop bx;\r\n    pop ax;\r\n    xor ax, bx;\r\n    push ax;\r\n", 0 }, { "", 0 } } }, // ==
  
	{ "<not_equal_action>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "    pop bx;\r\n    pop ax;\r\n    xor ax, bx;\r\n    lahf;\r\n    and ax, 16384;\r\n    push ax;\r\n", 0 }, { "", 0 } } }, // !=

	{ "<less_or_equal_action>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "    pop bx;\r\n    pop ax;\r\n    cmp bx, ax;\r\n    lahf;\r\n    and ax, 32768;\r\n    push ax;\r\n", 0 }, { "", 0 } } }, // <=
 
	{ "<greater_or_equal_action>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "    pop bx;\r\n    pop ax;\r\n    cmp ax, bx;\r\n    lahf;\r\n    and ax, 32768;\r\n    push ax;\r\n", 0 }, { "", 0 } } }, // >=

	{ "<add_action>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "    pop bx;\r\n    pop ax;\r\n    add ax, bx;\r\n    push ax;\r\n", 0 }, { "", 0 } } },

	{ "<sub_action>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "    pop bx;\r\n    pop ax;\r\n    sub ax, bx;\r\n    push ax;\r\n", 0 }, { "", 0 } } },

	{ "<mul_action>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "    pop bx;\r\n    pop ax;\r\n    mul bx;\r\n    push ax;\r\n", 0 }, { "", 0 } } },

	{ "<div_action>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "    pop bx;\r\n    pop ax;\r\n    xor dx, dx;\r\n    div bx;\r\n    push ax;\r\n", 0 }, { "", 0 } } },

	{ "<mod_action>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "    pop bx;\r\n    pop ax;\r\n    xor dx, dx;\r\n    div bx;\r\n    push dx;\r\n", 0 }, { "", 0 } } },

	{ "<unary_operation>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "", 0 }, { "", 0 } } },

	//
 
	{ "<inseparable_expression>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | PRE_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "", 0 }, { "", 0 } } },

	{ "<inseparable_expression>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "", 0 }, { "", 0 } } },

	//

	{ "<high_prioryty_left_expression>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "", 0 }, { "", 0 } } },

	{ "<high_prioryty_action>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "", 0 }, { "", 0 } } },

	{ "<high_prioryty_expression>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "", 0 }, { "", 0 } } },

	//

	{ "<middle_prioryty_left_expression>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "", 0 }, { "", 0 } } },
 
	{ "<middle_prioryty_action>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "", 0 }, { "", 0 } } },

	{ "<middle_prioryty_expression>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "", 0 }, { "", 0 } } },

	//

	{ "<low_prioryty_left_expression>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "", 0 }, { "", 0 } } },

	{ "<low_prioryty_action>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "", 0 }, { "", 0 } } },

	{ "<low_prioryty_expression>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "", 0 }, { "", 0 } } },

	//

	{ "<expression>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "", 0 }, { "", 0 } } },

	//

	{ "<group_expression>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "", 0 }, { "", 0 } } },

	//

	{ "<bind>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "", 0 }, { "", 0 } } },

	//

	{ "<if_expression>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "    pop ax;\r\n    and ax, ax;\r\n    jnz label%d_body_for_false;\r\n", 0 }, { "", 0 } } },

	{ "<body_for_true>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "    jmp label%d_point_end;\r\n    label%d_body_for_false:\r\n", 0 }, { "", 0 } } },

	{ "<body_for_false>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | PRE_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "", 0 }, { "", 0 } } },
    
	{ "<cond_block>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | PRE_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "", 0 }, { "", 0 } }, BASE_ASSIGNATION_STAGE },

	{ "<cond_block>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "    label%d_point_end:\r\n", 0 }, { "", 0 } }, BASE_ASSIGNATION_STAGE },

	//

	{ "<cycle_begin_expression>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "    pop ax;\r\n    mov cx, ax;\r\n", 0 }, { "", 0 } } },

	{ "<cycle_counter>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | PRE_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "    label%d_point_begin:\r\n    mov ", 0 }, { "", 0 } } },

	{ "<cycle_counter>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { ", cx;\r\n", 0 }, { "", 0 } } },

	{ "<cycle_counter_last_value>", ABSTRACT_TOKEN_TYPE | VARIATIVE_PATERN_TYPE_TA | PRE_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "    mov dx, ", ABSTRACT_TOKEN_TYPE }, { "", 0 } } },

	{ "<cycle_counter_last_value>", ABSTRACT_TOKEN_TYPE | VARIATIVE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { ";\r\n    cmp dx, cx;\r\n    js label%d_point_end;\r\n", ABSTRACT_TOKEN_TYPE }, { "", 0 } } },

	{ "<cycle_body>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "", 0 }, { "", 0 } } },

	{ "<forto_cycle>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | PRE_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "    ;forto_cycle_%d\r\n", 0 }, { "", 0 } }, BASE_ASSIGNATION_STAGE },

	{ "<forto_cycle>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "    inc cx;\r\n    jmp label%d_point_begin;\r\n    label%d_point_end:\r\n", 0 }, { "", 0 } }, BASE_ASSIGNATION_STAGE },

    //

	{ "<cycle_head_expression>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "jz label%d_point_end;\r\n", 0 }, { "", 0 } } },

	{ "<while_cycle>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | PRE_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "label%d_point_begin:\r\n", 0 }, { "", 0 } }, BASE_ASSIGNATION_STAGE },

	{ "<while_cycle>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "jmp label%d_point_begin\r\nlabel%d_point_end:\r\n", 0 }, { "", 0 } }, BASE_ASSIGNATION_STAGE },

	//

	{ "<input>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | PRE_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "    GET txt;\r\n", 0 }, { "", 0 } } },

	{ "<output>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "    PUT fmt, txt;\r\n", 0 }, { "", 0 } } },

	{ "<statement>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "", 0 }, { "", 0 } } },

	{ "<program>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | PRE_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { ";code example; KI-36/KI-37; 2018\r\n\r\n", 0 }, { "", 0 } } },
	
	{ "<program>", ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA | POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE, { { "~~", 0 }, { "    ret;\r\n\r\nmain ENDP\r\n\r\nend main;\r\n", 0 }, { "", 0 } } },

	{ "", 0, { { "~~", 0 }, { "", 0 }, { "", 0 }, { "", 0 }, { "", 0 }, { "", 0 }, { "", 0 }, { "", 0 }, { "", 0 }, { "", 0 } } }
}
;

void initTranslator();
void runTranslator(char * source, char * productionSource);
void closeTranslator();
struct Translator translator = { initTranslator, runTranslator, closeTranslator };
void initTranslator(){

	tokenNames = (char *)malloc(MAX_TOKEN_NAMES_SIZE * sizeof(char));
	memset(tokenNames, 0, MAX_TOKENS_TABLE_SIZE * sizeof(char));
	unsigned int tokenNamesSize = 0;

	tokenValues = malloc(MAX_TOKEN_VALUES_SIZE);
	memset(tokenNames, 0, MAX_TOKEN_VALUES_SIZE);
	unsigned int tokenValuesSize = 0;

	tokensTable = (struct TokensTable *)malloc(MAX_TOKENS_TABLE_SIZE * sizeof(struct TokensTable));
	memset(tokensTable, 0, MAX_TOKENS_TABLE_SIZE * sizeof(struct TokensTable));
	tokensTableLastFreeIndex = 1;
	processedTokensTable = (struct TokensTable *)malloc(MAX_PROCESSED_TOKENS_TABLE_SIZE * sizeof(struct TokensTable));
	memset(processedTokensTable, 0, MAX_PROCESSED_TOKENS_TABLE_SIZE * sizeof(struct TokensTable));
	processedTokensTableLastFreeIndex = 1;
	productionsTable = (struct TokensTable *)malloc(MAX_PRODUCTION_TOKENS_TABLE_SIZE * sizeof(struct TokensTable));
	memset(productionsTable, 0, MAX_PRODUCTION_TOKENS_TABLE_SIZE * sizeof(struct TokensTable));
	productionsTableLastFreeIndex = 1;

	//

	for (unsigned int notationIndex = 1; commentsNotations[notationIndex].token[0] != '\0'; ++notationIndex){
		 getTokenId(tokensTable, &tokensTableLastFreeIndex, commentsNotations[notationIndex].token, COMMENT_NOTATIONS_TYPE | ABSTRACT_TOKEN_TYPE | commentsNotations[notationIndex].atribute, NOTATIONS_SPACE);
	}
	for (unsigned int notationIndex = 1; lexemNotations[notationIndex].token[0] != '\0'; ++notationIndex){
		 getTokenId(tokensTable, &tokensTableLastFreeIndex, lexemNotations[notationIndex].token, LEXEM_NOTATIONS_TYPE | ABSTRACT_TOKEN_TYPE | lexemNotations[notationIndex].atribute, NOTATIONS_SPACE);
	}
	for (unsigned int notationIndex = 1; sourceNotations[notationIndex].token[0] != '\0'; ++notationIndex){			
		 getTokenId(tokensTable, &tokensTableLastFreeIndex, sourceNotations[notationIndex].token, ABSTRACT_TOKEN_TYPE | sourceNotations[notationIndex].atribute, NOTATIONS_SPACE);
	}

	for (unsigned int notationIndex = 1; commentsNotations[notationIndex].token[0] != '\0'; ++notationIndex){
		for (unsigned int currNotationTokenId = 1; commentsNotations[notationIndex].notationTokens[currNotationTokenId].token[0] != '\0'; ++currNotationTokenId){
			if (!(commentsNotations[notationIndex].notationTokens[currNotationTokenId].token[0] == '<'  &&
				  commentsNotations[notationIndex].notationTokens[currNotationTokenId].token[1] >= 'a'  &&
				  commentsNotations[notationIndex].notationTokens[currNotationTokenId].token[1] <= 'z'   )
				  && !getTokenId(tokensTable, &tokensTableLastFreeIndex, commentsNotations[notationIndex].notationTokens[currNotationTokenId].token, COMMENT_TOKEN_TYPE, NOTATIONS_SPACE)){
			}
		}
	}

	for (unsigned int notationIndex = 1; lexemNotations[notationIndex].token[0] != '\0'; ++notationIndex){
		for (unsigned int currNotationTokenId = 1; lexemNotations[notationIndex].notationTokens[currNotationTokenId].token[0] != '\0'; ++currNotationTokenId){
			if (!(lexemNotations[notationIndex].notationTokens[currNotationTokenId].token[0] == '<'  &&
				  lexemNotations[notationIndex].notationTokens[currNotationTokenId].token[1] >= 'a'  &&
				  lexemNotations[notationIndex].notationTokens[currNotationTokenId].token[1] <= 'z'   )
				  && !getTokenId(tokensTable, &tokensTableLastFreeIndex, lexemNotations[notationIndex].notationTokens[currNotationTokenId].token, LEXEM_TOKEN_TYPE, NOTATIONS_SPACE)){
			}
		}
	}

	for (unsigned int notationIndex = 1; sourceNotations[notationIndex].token[0] != '\0'; ++notationIndex){
		for (unsigned int currNotationTokenId = 1; sourceNotations[notationIndex].notationTokens[currNotationTokenId].token[0] != '\0'; ++currNotationTokenId){
			if (!(sourceNotations[notationIndex].notationTokens[currNotationTokenId].token[0] == '<'  && 
				  sourceNotations[notationIndex].notationTokens[currNotationTokenId].token[1] >= 'a'  &&
				  sourceNotations[notationIndex].notationTokens[currNotationTokenId].token[1] <= 'z'   ) 
				  && !getTokenId(tokensTable, &tokensTableLastFreeIndex, sourceNotations[notationIndex].notationTokens[currNotationTokenId].token, KEYWORD_TOKEN_TYPE, NOTATIONS_SPACE)){
			}
		}
	}


	unsigned int tokenId = 1;
	for (unsigned int commentsNotationIndex = 1; commentsNotations[commentsNotationIndex].token[0] != '\0'; ++commentsNotationIndex, ++tokenId){
		if (tokensTable[tokenId].recordType & COMMENT_NOTATIONS_TYPE && tokensTable[tokenId].recordType & ABSTRACT_TOKEN_TYPE){
			for (unsigned int currNotationTokenId = 1; commentsNotations[commentsNotationIndex].notationTokens[currNotationTokenId].token[0] != '\0'; ++currNotationTokenId){
				tokensTable[tokenId].notationTokens[currNotationTokenId].atribute |= commentsNotations[commentsNotationIndex].notationTokens[currNotationTokenId].atribute;
				for (unsigned int findTokenId = 1; tokensTable[findTokenId].tokenStr; ++findTokenId){
					if (!strncmp(commentsNotations[commentsNotationIndex].notationTokens[currNotationTokenId].token, tokensTable[findTokenId].tokenStr, MAX_TOKEN_LENGTH_T)){
						tokensTable[tokenId].notationTokens[currNotationTokenId].tokenId = findTokenId;
						break;
					}
				}
			}	
		}
	}

	for (unsigned int lexemsNotationIndex = 1; lexemNotations[lexemsNotationIndex].token[0] != '\0'; ++lexemsNotationIndex, ++tokenId){
		if (tokensTable[tokenId].recordType & LEXEM_NOTATIONS_TYPE && tokensTable[tokenId].recordType & ABSTRACT_TOKEN_TYPE){
			for (unsigned int currNotationTokenId = 1; lexemNotations[lexemsNotationIndex].notationTokens[currNotationTokenId].token[0] != '\0'; ++currNotationTokenId){
				tokensTable[tokenId].notationTokens[currNotationTokenId].atribute |= lexemNotations[lexemsNotationIndex].notationTokens[currNotationTokenId].atribute;
				for (unsigned int findTokenId = 1; tokensTable[findTokenId].tokenStr; ++findTokenId){
					if (!strncmp(lexemNotations[lexemsNotationIndex].notationTokens[currNotationTokenId].token, tokensTable[findTokenId].tokenStr, MAX_TOKEN_LENGTH_T)){
						tokensTable[tokenId].notationTokens[currNotationTokenId].tokenId = findTokenId;
						break;
					}
				}
			}
		}
	}

	for (unsigned int sourceNotationIndex = 1; sourceNotations[sourceNotationIndex].token[0] != '\0'; ++sourceNotationIndex, ++tokenId){
		if (!(tokensTable[tokenId].recordType & COMMENT_NOTATIONS_TYPE) && !(tokensTable[tokenId].recordType & LEXEM_NOTATIONS_TYPE) && tokensTable[tokenId].recordType & ABSTRACT_TOKEN_TYPE){
			for (unsigned int currNotationTokenId = 1; sourceNotations[sourceNotationIndex].notationTokens[currNotationTokenId].token[0] != '\0'; ++currNotationTokenId){
				tokensTable[tokenId].notationTokens[currNotationTokenId].atribute |= sourceNotations[sourceNotationIndex].notationTokens[currNotationTokenId].atribute;
				for (unsigned int findTokenId = 1; tokensTable[findTokenId].tokenStr; ++findTokenId){
					if (!strncmp(sourceNotations[sourceNotationIndex].notationTokens[currNotationTokenId].token, tokensTable[findTokenId].tokenStr, MAX_TOKEN_LENGTH_T)){
						tokensTable[tokenId].notationTokens[currNotationTokenId].tokenId = findTokenId;
						break;
					}
				}
			}
		}
	}

	//

	for (unsigned int notationIndex = 1; productionNotations[notationIndex].token[0] != '\0'; ++notationIndex){
		unsigned int tokenId = getTokenId(productionsTable, &productionsTableLastFreeIndex, productionNotations[notationIndex].token, ABSTRACT_TOKEN_TYPE | productionNotations[notationIndex].atribute, NOTATIONS_SPACE);
		productionsTable[tokenId].assignationType = productionNotations[notationIndex].assignationType; /*move*/
	}

	for (unsigned int notationIndex = 1; productionNotations[notationIndex].token[0] != '\0'; ++notationIndex){
		for (unsigned int currNotationTokenId = 1; productionNotations[notationIndex].notationTokens[currNotationTokenId].token[0] != '\0'; ++currNotationTokenId){
			if (!(productionNotations[notationIndex].notationTokens[currNotationTokenId].token[0] == '<'  &&
				productionNotations[notationIndex].notationTokens[currNotationTokenId].token[1] >= 'a'  &&
				productionNotations[notationIndex].notationTokens[currNotationTokenId].token[1] <= 'z')
				&& !getTokenId(productionsTable, &productionsTableLastFreeIndex, productionNotations[notationIndex].notationTokens[currNotationTokenId].token, productionNotations[notationIndex].notationTokens[currNotationTokenId].atribute, NOTATIONS_SPACE)){
			}
		}
	}

	tokenId = 1;
	for (unsigned int productionNotationIndex = 1; productionNotations[productionNotationIndex].token[0] != '\0'; ++productionNotationIndex, ++tokenId){
		if (!(productionsTable[tokenId].recordType & COMMENT_NOTATIONS_TYPE) && !(productionsTable[tokenId].recordType & LEXEM_NOTATIONS_TYPE) && productionsTable[tokenId].recordType & ABSTRACT_TOKEN_TYPE){
			for (unsigned int currNotationTokenId = 1; productionNotations[productionNotationIndex].notationTokens[currNotationTokenId].token[0] != '\0'; ++currNotationTokenId){
				productionsTable[tokenId].notationTokens[currNotationTokenId].atribute |= productionNotations[productionNotationIndex].notationTokens[currNotationTokenId].atribute;
				for (unsigned int findTokenId = 1; productionsTable[findTokenId].tokenStr; ++findTokenId){
					if (!strncmp(productionNotations[productionNotationIndex].notationTokens[currNotationTokenId].token, productionsTable[findTokenId].tokenStr, MAX_TOKEN_LENGTH_T)){
						productionsTable[tokenId].notationTokens[currNotationTokenId].tokenId = findTokenId;
						break;
					}
				}
			}
		}
	}

	//

	tokensSequenceTable = (struct TokensSequenceTable *) malloc(MAX_TOKENS_SEQUENCE_TABLE_SIZE * sizeof(struct TokensSequenceTable));
	memset(tokensSequenceTable, 0, MAX_TOKENS_TABLE_SIZE * sizeof(struct TokensSequenceTable));
	tokensSequenceTableLastFreeIndex = 1;

}
void runTranslator(char * source, char * productionSource){

	unsigned int endPointFilters[4] = { 0 };
	unsigned int notationsBlockPatternId = getTokenId(tokensTable, &tokensTableLastFreeIndex, (char*)"<notations_block>", NONMARKER, 0);
	if (tokensTable[notationsBlockPatternId].recordType & NONMARKER){
		notationsBlockPatternId = 0;
	}
	unsigned int identPatternId = endPointFilters[0] = getTokenId(tokensTable, &tokensTableLastFreeIndex, (char*)"<ident>", NONMARKER, 0);
	if (tokensTable[identPatternId].recordType & NONMARKER){
		identPatternId = 0;
	}
	unsigned int valuePatternId = endPointFilters[1] = getTokenId(tokensTable, &tokensTableLastFreeIndex, (char*)"<value>", NONMARKER, 0);
	if (tokensTable[valuePatternId].recordType & NONMARKER){
		valuePatternId = 0;
	}
	unsigned int labelPatternId = endPointFilters[2] = getTokenId(tokensTable, &tokensTableLastFreeIndex, (char*)"<label>", NONMARKER, 0);
	if (tokensTable[labelPatternId].recordType & NONMARKER){
		labelPatternId = 0;
	}

	printTokensTable(tokensTable, tokensTable, COMMENT_NOTATIONS_TYPE, 1);
	printTokensTable(tokensTable, tokensTable, LEXEM_NOTATIONS_TYPE, 1);
	printTokensTable(tokensTable, tokensTable, ABSTRACT_TOKEN_TYPE, 1);
	printTokensTable(tokensTable, tokensTable, COMMENT_TOKEN_TYPE, 1);
	printTokensTable(tokensTable, tokensTable, LEXEM_TOKEN_TYPE, 1);
	printTokensTable(tokensTable, tokensTable, identPatternId, 1);
	printTokensTable(tokensTable, tokensTable, valuePatternId, 1);
	printTokensTable(tokensTable, tokensTable, labelPatternId, 1);
	printTokensTable(tokensTable, tokensTable, KEYWORD_TOKEN_TYPE, 1);
	printTokensTable(tokensTable, tokensTable, RAW_TOKEN_TYPE, 1);

	scanner.run(source);  // (1)
	lexer.run(source);    // (1.2)
	syntaxer.run(source); // (3)

	printf("=====================================\r\n");
	printTokensTable(tokensTable, tokensTable, COMMENT_NOTATIONS_TYPE, 1);
	printTokensTable(tokensTable, tokensTable, LEXEM_NOTATIONS_TYPE, 1);
	printTokensTable(tokensTable, tokensTable, ABSTRACT_TOKEN_TYPE, 1);
	printTokensTable(tokensTable, tokensTable, COMMENT_TOKEN_TYPE, 1);
	printTokensTable(tokensTable, tokensTable, LEXEM_TOKEN_TYPE, 1);
	if (identPatternId) printTokensTable(tokensTable, tokensTable, identPatternId, 1);
	if (valuePatternId) printTokensTable(tokensTable, tokensTable, valuePatternId, 1);
	if (labelPatternId) printTokensTable(tokensTable, tokensTable, labelPatternId, 1);
	printTokensTable(tokensTable, tokensTable, KEYWORD_TOKEN_TYPE, 1);
	printTokensTable(tokensTable, tokensTable, RAW_TOKEN_TYPE, 1);
	printf("=====================================\r\n");

	unsigned int fg_1 = 1;
	printTokensTable(processedTokensTable, tokensTable, COMMENT_NOTATIONS_TYPE, fg_1);
	printTokensTable(processedTokensTable, tokensTable, LEXEM_NOTATIONS_TYPE, fg_1);
	printTokensTable(processedTokensTable, tokensTable, ABSTRACT_TOKEN_TYPE, fg_1);
	printTokensTable(processedTokensTable, tokensTable, COMMENT_TOKEN_TYPE, fg_1);
	printTokensTable(processedTokensTable, tokensTable, LEXEM_TOKEN_TYPE, fg_1);
	if (identPatternId) printTokensTable(processedTokensTable, tokensTable, identPatternId, fg_1);
	if (valuePatternId) printTokensTable(processedTokensTable, tokensTable, valuePatternId, fg_1);
	if (labelPatternId) printTokensTable(processedTokensTable, tokensTable, labelPatternId, fg_1);
	printTokensTable(processedTokensTable, tokensTable, KEYWORD_TOKEN_TYPE, fg_1);
	printTokensTable(processedTokensTable, tokensTable, RAW_TOKEN_TYPE, fg_1);
	printf("=====================================\r\n");
	printTokensTable(processedTokensTable, tokensTable, 0, fg_1);
	printf("=====================================\r\n");
	printf("=====================================\r\n");
	printTokensSequenceTable(0);
	printTokensSequenceTable(1);
	printf("=============production==============\r\n");
	unsigned int processedProgramPatternId = getTokenId(processedTokensTable, &processedTokensTableLastFreeIndex, (char*)"<program>", NONMARKER, 0);
	if (tokensTable[processedProgramPatternId].recordType & NONMARKER){
		return;
	}

	char * productionOutPtr = productionSource;

	makeProductionHeaderOut(&productionOutPtr, processedTokensTable, processedProgramPatternId, MAX_CAPTURE_DEEP, 0, endPointFilters);
	makeProductionOut(&productionOutPtr, processedTokensTable, processedProgramPatternId, MAX_CAPTURE_DEEP, 0, endPointFilters);

	printf("=============production==============\r\n");
	puts(productionSource);
	printf("=====================================\r\n");
}
void closeTranslator(){
	puts("mem dealocate>>");
	free(tokenNames);
	free(tokenValues);
	free(tokensTable);
	free(processedTokensTable);
	free(productionsTable);
	free(tokensSequenceTable);
	puts("translation complete");
	puts("Press any key to continue . . .");
	getchar();
}