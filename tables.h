/*************************************************************
* N.Kozak // Lviv'2018 // Compiler-compiler (example for SP) *
*                   file: tables.h                           *
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
#define KEY_WATERMARK 0x12345678

#define REPEAT_WATERMARK( ) 0x12345678

#define PATH_NAME_LENGH 2048

#define MAX_TOKENS_SEQUENCE_TABLE_SIZE 32768
#define MAX_TOKENS_TABLE_SIZE 4096
#define MAX_PROCESSED_TOKENS_TABLE_SIZE 8192
#define MAX_PRODUCTION_TOKENS_TABLE_SIZE 4096

#define MAX_TOKEN_NAMES_SIZE (8192 * 16)
#define MAX_TOKEN_VALUES_SIZE (8192 * 16)

#define MAX_TOKENS4TOKEN_COUNT_T 128
#define MAX_TOKEN_LENGTH_T 128

#define MAX_CAPTURE_DEEP 16

#define VARIATIVE_DEEP_MERGE 8

#define VARIATIVE_DEEP_ADDON (MAX_CAPTURE_DEEP - 1) // 19

#define ASSIGNATION_COUNT_PER_NOTATION                                             8

#define BASE_ASSIGNATION_STAGE                                            0x10000000

#define NOTATIONS_SPACE                                                   0x00000fff

#define POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE                            0x00001000
#define PRE_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE                             0x00040000 // <->

//#define EXCLUSION_NON_ABSTRACT_TOKEN_TYPE                               0x000001000
#define VARIATIVE_EXCLUSION_PATERN_FOR_FIND_SUBNON_ABSTRACT_TOKEN_TYPE_TA 0x00002000
#define VARIATIVE_PATERN_TYPE_TA                                          0x00004000
#define SEQUENCE_PATERN_TYPE_TA                                           0x00008000 // SEQUENCE_TYPE_TA
#define REPEAT_TOKEN_ATRIBUTE_TYPE_TA                                     0x00010000
#define MANDATORY_TOKEN_ATRIBUTE_TYPE_TA                                  0x00020000
//										                                  
#define COMMENTED_TOKEN_TYPE                                              0x00040000 // <->
#define MARKER                                                            0x00080000
#define NONMARKER                                                         0x00100000
//--									                                  
// union								                                  
#define COMMENT_TOKEN_TYPE                                                0x00200000
#define LEXEM_TOKEN_TYPE                                                  0x00400000
#define COMMENT_NOTATIONS_TYPE                                            0x00800000
#define LEXEM_NOTATIONS_TYPE                                              0x01000000
//--									                                  
#define VIRTUAL_END_LINE_TOKEN                                            0x02000000 
#define ABSTRACT_TOKEN_TYPE                                               0x04000000 
#define KEYWORD_TOKEN_TYPE                                                0x08000000
#define RAW_TOKEN_TYPE                                                    0x10000000
#define IDENT_TOKEN_TYPE                                                  0x20000000
#define VALUE_TOKEN_TYPE                                                  0x40000000
#define LABEL_TOKEN_TYPE                                                  0x80000000

#define CHECK_FLAGS(VERIFIE_FLAGS, VALUE) !(\
	((VERIFIE_FLAGS) ^ (VERIFIE_FLAGS)& (VALUE)) & ~NOTATIONS_SPACE \
	|| ((VERIFIE_FLAGS) ^ (VALUE)) & NOTATIONS_SPACE && ((VERIFIE_FLAGS)& NOTATIONS_SPACE)\
	)

#define DEFAULT_INPUT_FILENAME "file1.cwl"

#define PREDEFINED_TEXT1 \
	"NAME _PROGRAM ; \r\n" \
	"BODY DATA INTEGER16 _VALUEIN , _RESULTO , _CYCLEAC ;\r\n" \
	"  GET ( _VALUEIN )\r\n" \
	"  1 >> _RESULTO\r\n" \
	"  FOR  0 >> _CYCLEAC TO 32767 DO\r\n" \
	"    IF _VALUEIN != 0 THEN ; ELSE GOTO endcyclea ;\r\n" \
	"    _RESULTO * _VALUEIN >> _RESULTO \r\n" \
	"    _VALUEIN - 1 >> _VALUEIN\r\n" \
	"  ;\r\n" \
	"  endcyclea :\r\n" \
	"  PUT ( _RESULTO )\r\n" \
	"  END\r\n" \
	"\r\n"

#define PREDEFINED_TEXT2 \
	"NAME _PROGRAM ; \r\n" \
	"BODY DATA INTEGER16 _VALUEIN , _DEVIDER , _REMAIND , _CYCLEAC , _CYCLEBC , _CYCLECC ;\r\n" \
	"  GET ( _VALUEIN )\r\n" \
	"  1 + _VALUEIN >> _VALUEIN\r\n" \
	"  FOR  0 >> _CYCLEAC TO 32767 DO\r\n" \
	"    _VALUEIN - 1 >> _DEVIDER\r\n" \
	"    FOR  0 >> _CYCLEBC TO 32767 DO \r\n" \
	"      IF _DEVIDER >= 2 THEN ; ELSE GOTO endcycleb ;\r\n" \
	"      _VALUEIN >> _REMAIND\r\n" \
	"      FOR  0 >> _CYCLECC TO 32767 DO\r\n" \
	"        IF _REMAIND >= _DEVIDER THEN ; ELSE GOTO endcyclec ;\r\n" \
	"        _REMAIND - _DEVIDER >> _REMAIND\r\n" \
	"      ;\r\n" \
	"      endcyclec :\r\n" \
	"      IF _REMAIND == 0 THEN GOTO endcycleb ;\r\n" \
	"      _DEVIDER - 1 >> _DEVIDER\r\n" \
	"    ;\r\n" \
	"    endcycleb :\r\n" \
	"    IF _DEVIDER == 1 THEN GOTO endcyclea ;\r\n" \
	"    1 + _VALUEIN >> _VALUEIN\r\n" \
	"  ;\r\n" \
	"  endcyclea :\r\n" \
	"  PUT ( _VALUEIN )\r\n" \
	"  END\r\n" \
	"\r\n"


// X = K + B - D / 2 + E * 4
// K = 25630
// B = 4
// D = 5
// E = 6
#define PREDEFINED_TEXT3 \
	"NAME _PROGRAM ; \r\n" \
	"BODY DATA INTEGER16 _BBBBBBB , _DDDDDDD , _EEEEEEE , _KKKKKKK ;\r\n" \
	"  25630 >> _KKKKKKK\r\n" \
	"  GET ( _BBBBBBB )\r\n" \
	"  GET ( _DDDDDDD )\r\n" \
	"  GET ( _EEEEEEE )\r\n" \
	"  PUT ( _KKKKKKK + _BBBBBBB - _DDDDDDD DIV 2 + _EEEEEEE * 4 )\r\n" \
	"  END\r\n" \
	"\r\n"

#define MAX_TEXT_SIZE 16384
#define MAX_WORD_COUNT (MAX_TEXT_SIZE / 5)
#define MAX_LEXEM_SIZE 1024
#define MAX_STRONG_RESERVED_LEXEMES_COUNT 64
#define MAX_RESERVED_LEXEMES_COUNT 64
#define MAX_COMMENT_LEXEMES_COUNT 2

#define STRONG_RESERVED_LEXEME_TYPE 1
#define RESERVED_LEXEME_TYPE 2
#define IDENTIFIER_LEXEME_TYPE 3
#define VALUE_LEXEME_TYPE 4
#define LABEL_LEXEME_TYPE 5
#define UNEXPEXTED_LEXEME_TYPE 127

#define LEXICAL_ANALISIS_MODE 1
#define SYNTAX_ANALISIS_MODE 2
#define SEMANTIC_ANALISIS_MODE 4
#define MAKE_ASSEMBLY 8
#define MAKE_BINARY 16
#define RUN_BINARY 32

#define DEBUG_MODE 512

#define MAX_PARAMETERS_SIZE 4096
#define PARAMETERS_COUNT 4
#define INPUT_FILENAME_WITH_EXTENSION_PARAMETER 0
#define INOUT_ASSEMBLY_FILENAME_WITHOUT_EXTENSION_PARAMETER 1
#define OUT_BINARY_FILENAME_WITHOUT_EXTENSION_PARAMETER 2

#define DEFAULT_MODE (LEXICAL_ANALISIS_MODE | SYNTAX_ANALISIS_MODE | SEMANTIC_ANALISIS_MODE | MAKE_ASSEMBLY | MAKE_BINARY)


extern unsigned int mode;
extern char parameters[PARAMETERS_COUNT][MAX_PARAMETERS_SIZE];

struct SourceNotations{
	char token[MAX_TOKEN_LENGTH_T];
	unsigned int atribute;
	struct {
		char token[MAX_TOKEN_LENGTH_T];
		unsigned int atribute;
	} notationTokens[MAX_TOKENS4TOKEN_COUNT_T];
	unsigned int assignationType;
};

struct SourceNotations_2{
	char token[MAX_TOKEN_LENGTH_T];
	unsigned int atribute;
	unsigned int tokenId;
	struct {
		char token[MAX_TOKEN_LENGTH_T];
		unsigned int atribute;
		unsigned int tokenId;
	} notationTokens[MAX_TOKENS4TOKEN_COUNT_T];
};

extern struct TokensTable {
	struct {
		unsigned int tokenId;
		unsigned int atribute;
	} notationTokens[MAX_TOKENS4TOKEN_COUNT_T];
	char * tokenStr;
	unsigned int recordType;
	void * tokenValue;
	unsigned int elementarPoint;
	unsigned int assignationType;
} *tokensTable, *processedTokensTable, *productionsTable;
extern unsigned int tokensTableLastFreeIndex, processedTokensTableLastFreeIndex, productionsTableLastFreeIndex;

extern char * tokenNames;
extern unsigned int tokenNamesSize;
extern void * tokenValues;
extern unsigned int tokenValuesSize;

extern struct TokensSequenceTable{
	unsigned int tokenId;
	unsigned int row;
	unsigned int column;
	unsigned int scaner_marker;
	unsigned int lexer_marker;
	unsigned int syntaxer_marker;
	unsigned int semantixer_marker;
	unsigned int pragmatixer_marker;
	unsigned int synthesizer_marker;
	unsigned int flags;
	unsigned int error;
	struct {
		unsigned int tokenId;
		char * tokenStr;
	} assignedAbstractToken;
} *tokensSequenceTable;
extern unsigned int tokensSequenceTableLastFreeIndex;
extern unsigned int tryGetTokenId(struct TokensTable * tokensTable, char * tokenName, unsigned int recordTypeFlags);
extern unsigned int getTokenId(struct TokensTable * tokensTable, unsigned int * tokensTableLastFreeIndex, char * tokenName, unsigned int recordType, unsigned int selfIdentMask); // recordTypeFlags
extern unsigned int addRecordToTokensTable(char * tokenName, unsigned int recordType);
extern unsigned int addRecordToTokensSequenceTable(unsigned int tokenId, unsigned int flags, unsigned int row, unsigned int column);
extern char * captureByPatern(char * analysePoint, unsigned int tokenId4Patern, unsigned char analyseTokenMode, unsigned int marker, unsigned int * productionIndex, unsigned int variativeSelectorPass, unsigned int unroll_deep_downcounter);
extern void makeProductionHeaderOut(char ** productionOut, struct TokensTable * tokensTable, unsigned int tokenId4Patern, unsigned int unroll_deep_downcounter, unsigned int upStageProductionTokenId/* = 123*/, unsigned int * endPointFilters);
extern void makeProductionOut(char ** productionOut, struct TokensTable * tokensTable, unsigned int tokenId4Patern, unsigned int unroll_deep_downcounter, unsigned int upStageProductionTokenId/* = 123*/, unsigned int * endPointFilters);
extern void printTokensTable(struct TokensTable * tokensTable, struct TokensTable * syntaxTokensTable, unsigned int recordType, unsigned int printOnlyNotation);
extern void printTokensSequenceTable(unsigned char onlyText);