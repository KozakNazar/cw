#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
/*************************************************************
* N.Kozak // Lviv'2018 // Compiler-compiler (example for SP) *
*                   file: tables.cpp                         *
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
#include "faststr.h"
#include "scanner.h"
#include "translator.h"

struct TokensSequenceTable * tokensSequenceTable;
unsigned int tokensSequenceTableLastFreeIndex;
struct TokensTable * tokensTable, *processedTokensTable, *productionsTable;
unsigned int tokensTableLastFreeIndex, processedTokensTableLastFreeIndex, productionsTableLastFreeIndex;
char * tokenNames;
unsigned int tokenNamesSize;
void * tokenValues;
unsigned int tokenValuesSize;
unsigned int mode;
char parameters[PARAMETERS_COUNT][MAX_PARAMETERS_SIZE] = { "" };
unsigned int tryGetTokenId(struct TokensTable * tokensTable, char * tokenName, unsigned int recordTypeFlags){
	for (unsigned int index = 1; tokensTable[index].tokenStr; ++index){		
		if (CHECK_FLAGS(recordTypeFlags, tokensTable[index].recordType) && !strncmp(tokenName, tokensTable[index].tokenStr, MAX_TOKEN_LENGTH_T)){
			return index;
		}
	}

	return 0;
}

unsigned int getTokenId(struct TokensTable * tokensTable, unsigned int * tokensTableLastFreeIndex, char * tokenName, unsigned int recordTypeFlags, unsigned int selfIdentMask){
	unsigned int nonmarkedRecordTypeFlags = recordTypeFlags & ~NONMARKER;
	for (unsigned int index = 1; tokensTable[index].tokenStr; ++index){
		if (CHECK_FLAGS(nonmarkedRecordTypeFlags, tokensTable[index].recordType) && !strncmp(tokenName, tokensTable[index].tokenStr, MAX_TOKEN_LENGTH_T)){
			return index;
		}
	}

	//
	strcpy(tokenNames + tokenNamesSize + 1, tokenName);
	tokensTable[*tokensTableLastFreeIndex].tokenStr = tokenNames + tokenNamesSize + 1;
	tokenNamesSize += strlen(tokenName) + 1;
	tokensTable[*tokensTableLastFreeIndex].recordType |= recordTypeFlags;

	return ++*tokensTableLastFreeIndex - 1;
}
unsigned int addRecordToTokensTable(char * tokenName, unsigned int recordType){
	strcpy(tokenNames + tokenNamesSize + 1, tokenName);
	tokensTable[tokensTableLastFreeIndex].tokenStr = tokenNames + tokenNamesSize + 1;
	tokenNamesSize += strlen(tokenName) + 1;
	tokensTable[tokensTableLastFreeIndex].recordType |= recordType;

	return ++tokensTableLastFreeIndex - 1;
}

void addNotationTokensToLastRecordInTokensTable(unsigned int tokenId, unsigned int atribute, unsigned int notationTokenId){
	tokensTable[tokensSequenceTableLastFreeIndex].notationTokens[notationTokenId].tokenId = tokenId;
	tokensTable[tokensSequenceTableLastFreeIndex].notationTokens[notationTokenId].atribute |= atribute;
}

unsigned int addRecordToTokensSequenceTable(unsigned int tokenId, unsigned int flags, unsigned int row, unsigned int column){
	tokensSequenceTable[tokensSequenceTableLastFreeIndex].tokenId = tokenId;
	tokensSequenceTable[tokensSequenceTableLastFreeIndex].row = row;
	tokensSequenceTable[tokensSequenceTableLastFreeIndex].column = column;

	tokensSequenceTable[tokensSequenceTableLastFreeIndex].flags = flags;

	return ++tokensSequenceTableLastFreeIndex - 1;
}

void clearMarkInTokensSequenceTable(){
	for (unsigned int tokensSequenceTableIndex = 1; tokensSequenceTable[tokensSequenceTableIndex].tokenId; ++tokensSequenceTableIndex){	
		tokensSequenceTable[tokensSequenceTableIndex].flags &= ~MARKER;		
	}
}


char * captureByPatern(char * analysePoint, unsigned int tokenId4Patern, unsigned char analyseTokenMode, unsigned int marker, unsigned int * productionIndex, unsigned int variativeSelectorPass, unsigned int unroll_deep_downcounter){

	if (--unroll_deep_downcounter, !(unroll_deep_downcounter + 1)){
		productionIndex ? *productionIndex = 0 : 0;
		return 0;
	}


	unsigned int tokensSequenceTableIndex = (unsigned int)analysePoint;
	if (analysePoint && !analyseTokenMode){
		for (; tokensSequenceTable[tokensSequenceTableIndex].tokenId && tokensSequenceTable[tokensSequenceTableIndex].flags & MARKER; ++tokensSequenceTableIndex);
	}
	else if (!analysePoint){
		return 0;
	}

	if (!analyseTokenMode && !(tokensSequenceTable[tokensSequenceTableIndex].tokenId)){
		return 0;
	}

	if (!analyseTokenMode && CHECK_FLAGS(tokenId4Patern, tokensTable[tokensSequenceTable[tokensSequenceTableIndex].tokenId].recordType)){ 
		processedTokensTable[*productionIndex].recordType |= tokenId4Patern;			
		processedTokensTable[*productionIndex].tokenStr = getStrFromTowoZeroEndStrs(tokenNames, &tokenNamesSize, tokensTable[tokensSequenceTable[tokensSequenceTableIndex].tokenId].tokenStr);

		productionIndex ? ++*productionIndex : 0;

		return (tokensTable[tokenId4Patern].recordType & VARIATIVE_EXCLUSION_PATERN_FOR_FIND_SUBNON_ABSTRACT_TOKEN_TYPE_TA) ? 0 : (tokensSequenceTable[tokensSequenceTableIndex].flags |= (marker & tokenId4Patern), ++analysePoint);

	}

	if (tokensTable[tokenId4Patern].recordType & ABSTRACT_TOKEN_TYPE){
		struct SourceNotations_2 temp = { "", 0, 0, { { "", 0, 0 }, { "", 0, 0 } } };
		unsigned int localProductionIndex = productionIndex ? *productionIndex : 0;
		if (tokensTable[tokenId4Patern].recordType & VARIATIVE_EXCLUSION_PATERN_FOR_FIND_SUBNON_ABSTRACT_TOKEN_TYPE_TA){
			for (unsigned int notationTokensIndex = 1; tokensTable[tokenId4Patern].notationTokens[notationTokensIndex].tokenId; ++notationTokensIndex){
				localProductionIndex = productionIndex ? *productionIndex : 0;
				unsigned int analysePoint_ = (unsigned int)captureByPatern(analysePoint, tokensTable[tokenId4Patern].notationTokens[notationTokensIndex].tokenId, analyseTokenMode, /*analyseTokenMode &*/ ((~0) | marker), productionIndex ? &localProductionIndex : NULL, variativeSelectorPass, unroll_deep_downcounter);
				if (analysePoint_){
					productionIndex ? *productionIndex = 0 : 0;
					return 0;
				}
			}
			if (productionIndex){
				processedTokensTable[*productionIndex].notationTokens[1].tokenId |= *productionIndex;
				processedTokensTable[*productionIndex].recordType |= tokenId4Patern | ABSTRACT_TOKEN_TYPE;

				processedTokensTable[*productionIndex].tokenStr = getStrFromTowoZeroEndStrs(tokenNames, &tokenNamesSize, tokensTable[tokensSequenceTable[tokensSequenceTableIndex].tokenId].tokenStr);

			}
			productionIndex ? ++*productionIndex : 0;
			return ++analysePoint;
		}
		else if (tokensTable[tokenId4Patern].recordType & VARIATIVE_PATERN_TYPE_TA){
			unsigned int analysePoint_ = 0;
			for (unsigned int notationTokensIndex = 1; tokensTable[tokenId4Patern].notationTokens[notationTokensIndex].tokenId; ++notationTokensIndex){
				notationTokensIndex > 1 && unroll_deep_downcounter <= VARIATIVE_DEEP_MERGE ? unroll_deep_downcounter += VARIATIVE_DEEP_ADDON : 0;
				
				localProductionIndex = productionIndex ? *productionIndex : 0;

				localProductionIndex = productionIndex ? *productionIndex : 0;
				unsigned int analysePoint_ = (unsigned int)captureByPatern(analysePoint, tokensTable[tokenId4Patern].notationTokens[notationTokensIndex].tokenId, analyseTokenMode, (~0) | marker, productionIndex ? &localProductionIndex : NULL, variativeSelectorPass, unroll_deep_downcounter);
				if (analysePoint_){
					if (productionIndex){
						*productionIndex = localProductionIndex;
						processedTokensTable[*productionIndex].notationTokens[1].tokenId |= localProductionIndex - 1;
						processedTokensTable[*productionIndex].recordType |= tokenId4Patern | ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA;
						processedTokensTable[*productionIndex].tokenStr = getStrFromTowoZeroEndStrs(tokenNames, &tokenNamesSize, tokensTable[tokenId4Patern].tokenStr);
					}
					productionIndex ? ++*productionIndex : 0;
					return (char*)analysePoint_;
				}
			}

			productionIndex ? *productionIndex = 0 : 0;
			return 0;
		}
		else if (tokensTable[tokenId4Patern].recordType & SEQUENCE_PATERN_TYPE_TA){
			unsigned int analysePoint__ = 0;
			unsigned int prevAnalysePoint4CurrNotationTokensIndex = 0;
			for (unsigned int notationTokensIndex = 1, scannedNotationTokensIndex = 1; tokensTable[tokenId4Patern].notationTokens[notationTokensIndex].tokenId;){
				localProductionIndex = productionIndex ? *productionIndex : 0;
				unsigned int analysePoint_ = (unsigned int)captureByPatern(analysePoint, tokensTable[tokenId4Patern].notationTokens[notationTokensIndex].tokenId, analyseTokenMode, (~0) | marker, productionIndex ? &localProductionIndex : NULL, variativeSelectorPass, unroll_deep_downcounter);
				if (analysePoint_){
					if (productionIndex){
						*productionIndex = localProductionIndex;
						temp.notationTokens[scannedNotationTokensIndex].tokenId = localProductionIndex - 1;
					}

					if (tokensTable[tokenId4Patern].notationTokens[notationTokensIndex].atribute & REPEAT_TOKEN_ATRIBUTE_TYPE_TA){
						prevAnalysePoint4CurrNotationTokensIndex = analysePoint_;
					}
					else{
						prevAnalysePoint4CurrNotationTokensIndex = 0;
						++notationTokensIndex;						
					}
					++scannedNotationTokensIndex;
					analysePoint__ = analysePoint_;
					analysePoint = (char*)analysePoint__;
					continue;

				}
				else if (prevAnalysePoint4CurrNotationTokensIndex){
					analysePoint__ = prevAnalysePoint4CurrNotationTokensIndex;
					analysePoint = (char*)analysePoint__;
					prevAnalysePoint4CurrNotationTokensIndex = 0;
					++notationTokensIndex;
					continue;
				}
				else{
					if (tokensTable[tokenId4Patern].notationTokens[notationTokensIndex].atribute & MANDATORY_TOKEN_ATRIBUTE_TYPE_TA){
						++notationTokensIndex;
					}
					else{
						productionIndex ? *productionIndex = 0 : 0;
						return 0;
					}
				}
			}

			if (productionIndex){
				for (unsigned int currNotationTokenId = 1; temp.notationTokens[currNotationTokenId].tokenId; ++currNotationTokenId){
					processedTokensTable[*productionIndex].notationTokens[currNotationTokenId].tokenId = temp.notationTokens[currNotationTokenId].tokenId;
					printf("--(%s, %d, %d,       %d)\r\n", tokensTable[tokenId4Patern].tokenStr, *productionIndex, unroll_deep_downcounter, processedTokensTable[*productionIndex].notationTokens[currNotationTokenId].tokenId);
				}

				processedTokensTable[*productionIndex].recordType |= tokenId4Patern | ABSTRACT_TOKEN_TYPE | SEQUENCE_PATERN_TYPE_TA;
				processedTokensTable[*productionIndex].tokenStr = getStrFromTowoZeroEndStrs(tokenNames, &tokenNamesSize, tokensTable[tokenId4Patern].tokenStr);
				printf("(%s, %d)\r\n", tokensTable[tokenId4Patern].tokenStr, unroll_deep_downcounter);
			}
						
			productionIndex ? ++*productionIndex : 0;
			return (char*)analysePoint__;
		}
	}
	else if (tokensTable[tokenId4Patern].recordType & VIRTUAL_END_LINE_TOKEN){
		if (analyseTokenMode){
			return 0;
		}
		else{
			unsigned int tokensSequenceTableIndex = (unsigned int)analysePoint;
			unsigned int prevTokensSequenceTableIndex = (tokensSequenceTableIndex > 1) ? tokensSequenceTableIndex - 1 : 1;
			return (tokensSequenceTable[tokensSequenceTableIndex].row > tokensSequenceTable[prevTokensSequenceTableIndex].row) ? (char*)tokensSequenceTableIndex : NULL;
		}
	}
	else{
		if (analyseTokenMode){
			return (*analysePoint == tokensTable[tokenId4Patern].tokenStr[0]) ? (++analysePoint) : 0;
		}
		else{
			unsigned int tokensSequenceTableIndex = (unsigned int)analysePoint;
			unsigned int completeCondition;
			if (tokensTable[tokenId4Patern].recordType & KEYWORD_TOKEN_TYPE){
				completeCondition = CHECK_FLAGS(tokenId4Patern, tokensTable[tokensSequenceTable[tokensSequenceTableIndex].tokenId].recordType);
			}
			else{
				completeCondition = tokensTable[tokensSequenceTable[tokensSequenceTableIndex].tokenId].recordType & tokensTable[tokenId4Patern].recordType;
				completeCondition = CHECK_FLAGS(tokenId4Patern, tokensSequenceTable[tokensSequenceTableIndex].flags);
			}

			if (productionIndex && (!!completeCondition ^ !!(tokensTable[tokenId4Patern].recordType & VARIATIVE_EXCLUSION_PATERN_FOR_FIND_SUBNON_ABSTRACT_TOKEN_TYPE_TA))){
				processedTokensTable[*productionIndex].recordType |= tokenId4Patern;				
				processedTokensTable[*productionIndex].tokenStr = getStrFromTowoZeroEndStrs(tokenNames, &tokenNamesSize, tokensTable[tokenId4Patern].tokenStr);	
				productionIndex && completeCondition ? ++*productionIndex : 0;
			}

			return (!!completeCondition ^ !!(tokensTable[tokenId4Patern].recordType & VARIATIVE_EXCLUSION_PATERN_FOR_FIND_SUBNON_ABSTRACT_TOKEN_TYPE_TA)) ? (tokensSequenceTable[tokensSequenceTableIndex].flags |= (marker & tokenId4Patern), ++analysePoint) : 0;
		}
	}

	return 0;
}

void makeProductionHeaderOut(char ** productionOut, struct TokensTable * tokensTable, unsigned int tokenId4Patern, unsigned int unroll_deep_downcounter, unsigned int upStageProductionTokenId, unsigned int * endPointFilters){
	*productionOut += sprintf(*productionOut, ".386\r\n");
	*productionOut += sprintf(*productionOut, ".model flat, stdcall\r\n");
	*productionOut += sprintf(*productionOut, "option casemap:none\r\n");
	*productionOut += sprintf(*productionOut, "includelib masm32\\lib\\msvcrt.lib;\r\n");
	*productionOut += sprintf(*productionOut, "includelib masm32\\lib\\kernel32.lib;\r\n");
	*productionOut += sprintf(*productionOut, "\r\n");
	*productionOut += sprintf(*productionOut, "sprintf PROTO near C :DWORD,:VARARG\r\n");
	*productionOut += sprintf(*productionOut, "gets PROTO near C :DWORD\r\n");
	*productionOut += sprintf(*productionOut, "puts PROTO near C :DWORD\r\n");
	*productionOut += sprintf(*productionOut, "atoi PROTO near C :DWORD\r\n");
	*productionOut += sprintf(*productionOut, "strlen PROTO near C :DWORD\r\n");
	*productionOut += sprintf(*productionOut, "GET MACRO txt\r\n");
	*productionOut += sprintf(*productionOut, "    push offset txt\r\n");
	*productionOut += sprintf(*productionOut, "    call gets\r\n");
	*productionOut += sprintf(*productionOut, "    call atoi\r\n");
	*productionOut += sprintf(*productionOut, "    add esp, 4\r\n");
	*productionOut += sprintf(*productionOut, "    push ax\r\n");
	*productionOut += sprintf(*productionOut, "ENDM\r\n");
	*productionOut += sprintf(*productionOut, "PUT MACRO fmt, txt\r\n");
	*productionOut += sprintf(*productionOut, "    pop ax\r\n");
	*productionOut += sprintf(*productionOut, "    and eax, 32767\r\n");
	*productionOut += sprintf(*productionOut, "    push eax\r\n");
	*productionOut += sprintf(*productionOut, "    push OFFSET fmt\r\n");
	*productionOut += sprintf(*productionOut, "    push OFFSET txt\r\n");
	*productionOut += sprintf(*productionOut, "    call sprintf\r\n");
	*productionOut += sprintf(*productionOut, "    add esp, 12\r\n");
	*productionOut += sprintf(*productionOut, "    push offset txt\r\n");
	*productionOut += sprintf(*productionOut, "    call puts\r\n");
	*productionOut += sprintf(*productionOut, "    add esp, 4\r\n");
	*productionOut += sprintf(*productionOut, "ENDM\r\n");

	*productionOut += sprintf(*productionOut, ".data\r\n");
	*productionOut += sprintf(*productionOut, "    txt  db 256 dup(0)\r\n");
	*productionOut += sprintf(*productionOut, "    fmt db '%%d', 13, 10, 0;\r\n");
	*productionOut += sprintf(*productionOut, "\r\n");
}

void makeProductionOut(char ** productionOut, struct TokensTable * tokensTable, unsigned int tokenId4Patern, unsigned int unroll_deep_downcounter, unsigned int upStageProductionTokenId, unsigned int * endPointFilters){

	if (tokensTable[tokenId4Patern].recordType & ABSTRACT_TOKEN_TYPE){

		if (tokensTable[tokenId4Patern].recordType & VARIATIVE_EXCLUSION_PATERN_FOR_FIND_SUBNON_ABSTRACT_TOKEN_TYPE_TA){
			printf("(>> bad production)\r\n");
		}
		else if (tokensTable[tokenId4Patern].recordType & VARIATIVE_PATERN_TYPE_TA){
			printf("(>> unrolled production)\r\n");
		}
		else if (tokensTable[tokenId4Patern].recordType & SEQUENCE_PATERN_TYPE_TA){
			unsigned int productionTokenId = tryGetTokenId(productionsTable, tokensTable[tokenId4Patern].tokenStr, PRE_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE);

			if (productionTokenId && productionsTable[productionTokenId].notationTokens[1].tokenId && productionsTable[productionsTable[productionTokenId].notationTokens[1].tokenId].tokenStr){

				unsigned int assignationTypeProductionsTable = (productionsTable[productionTokenId].assignationType & BASE_ASSIGNATION_STAGE) ? tokenId4Patern : upStageProductionTokenId;
				*productionOut += sprintf(*productionOut, productionsTable[productionsTable[productionTokenId].notationTokens[1].tokenId].tokenStr, assignationTypeProductionsTable, assignationTypeProductionsTable, assignationTypeProductionsTable);
			}
			for (unsigned int notationTokensIndex = 1; tokensTable[tokenId4Patern].notationTokens[notationTokensIndex].tokenId; ++notationTokensIndex){
				makeProductionOut(productionOut, tokensTable, tokensTable[tokenId4Patern].notationTokens[notationTokensIndex].tokenId, unroll_deep_downcounter, tokenId4Patern, endPointFilters);
			}
			productionTokenId = tryGetTokenId(productionsTable, tokensTable[tokenId4Patern].tokenStr, POST_RODUCTION_FOR_ABSTRACT_TOKEN_TYPE);
			if (productionTokenId && productionsTable[productionTokenId].notationTokens[1].tokenId && productionsTable[productionsTable[productionTokenId].notationTokens[1].tokenId].tokenStr){
				unsigned int assignationTypeProductionsTable = (productionsTable[productionTokenId].assignationType & BASE_ASSIGNATION_STAGE) ? tokenId4Patern : upStageProductionTokenId;
				*productionOut += sprintf(*productionOut, productionsTable[productionsTable[productionTokenId].notationTokens[1].tokenId].tokenStr, assignationTypeProductionsTable, assignationTypeProductionsTable, assignationTypeProductionsTable);
			}

			return;
		}
	}
	else if (tokensTable[tokenId4Patern].recordType & VIRTUAL_END_LINE_TOKEN){
		printf("(>> bad production)\r\n");
	}
	else{

		unsigned int endPointFilter = 0;
		for (unsigned int endPointFilterIndex = 0; endPointFilters[endPointFilterIndex] && !(endPointFilter |= CHECK_FLAGS(endPointFilters[endPointFilterIndex], tokensTable[tokenId4Patern].recordType)); ++endPointFilterIndex);
		if (endPointFilter){
			*productionOut += sprintf(*productionOut, "%s", tokensTable[tokenId4Patern].tokenStr);
		}

		return;
	}

	return;
}

void printTokensTable(struct TokensTable * tokensTable, struct TokensTable * syntaxTokensTable, unsigned int recordType, unsigned int printOnlyNotation){
	if (tokensTable){
		unsigned int notationsBlockPatternId = tryGetTokenId(syntaxTokensTable, (char*)"<notations_block>", 0);
		unsigned int identPatternId = tryGetTokenId(syntaxTokensTable, (char*)"<ident>", 0);
		unsigned int valuePatternId = tryGetTokenId(syntaxTokensTable, (char*)"<value>", 0);
		unsigned int labelPatternId = tryGetTokenId(syntaxTokensTable, (char*)"<label>", 0);

		if ((recordType & COMMENT_NOTATIONS_TYPE) && !(recordType & ABSTRACT_TOKEN_TYPE)){
			printf("COMMENT_NOTATIONS_TYPE:\r\n");
		}
		if ((recordType & LEXEM_NOTATIONS_TYPE) && !(recordType & ABSTRACT_TOKEN_TYPE)){
			printf("LEXEM_NOTATIONS_TYPE:\r\n");
		}
		if (recordType & ABSTRACT_TOKEN_TYPE){
			printf("ABSTRACT_TOKEN_TYPE:\r\n");
		}
		if (recordType & COMMENT_TOKEN_TYPE){
			printf("COMMENT_TOKEN_TYPE:\r\n");
		}
		if (recordType & LEXEM_TOKEN_TYPE){
			printf("LEXEM_TOKEN_TYPE:\r\n");
		}
		if (recordType & KEYWORD_TOKEN_TYPE){
			printf("KEYWORD_TOKEN_TYPE:\r\n");			
		}						
		if (recordType & RAW_TOKEN_TYPE){
			printf("RAW_TOKEN_TYPE:\r\n");
		}
		if (identPatternId && CHECK_FLAGS(identPatternId, recordType)){
			printf("IDENTS:\r\n");			
		}	
		if (valuePatternId && CHECK_FLAGS(valuePatternId, recordType)){
			printf("VALUES:\r\n");
		}
		if (labelPatternId && CHECK_FLAGS(labelPatternId, recordType)){
			printf("LABELS:\r\n");
		}
		typedef int a;
		typedef int a;
		struct b;
		struct b;
		for (unsigned int tokenId = 1; tokensTable[tokenId].tokenStr; ++tokenId){
			if (CHECK_FLAGS(recordType, tokensTable[tokenId].recordType)){
				printf("%3d(%8x): %s", tokenId, tokensTable[tokenId].recordType, tokensTable[tokenId].tokenStr);
				if (tokensTable[tokenId].recordType & (ABSTRACT_TOKEN_TYPE)){
					printf(" ::=");

					if (tokensTable[tokenId].recordType & VARIATIVE_PATERN_TYPE_TA){
						for (unsigned int currNotationTokenId = 1; tokensTable[tokenId].notationTokens[currNotationTokenId].tokenId; ++currNotationTokenId){
							if (currNotationTokenId == 1){
								printOnlyNotation ? printf(" %s", tokensTable[tokensTable[tokenId].notationTokens[currNotationTokenId].tokenId].tokenStr) :
									                printf(" %s(id=%d)", tokensTable[tokensTable[tokenId].notationTokens[currNotationTokenId].tokenId].tokenStr, tokensTable[tokenId].notationTokens[currNotationTokenId].tokenId);
							}
							else{																	
								printOnlyNotation ? printf(" | %s", tokensTable[tokensTable[tokenId].notationTokens[currNotationTokenId].tokenId].tokenStr):
									                printf(" | %s(id=%d)", tokensTable[tokensTable[tokenId].notationTokens[currNotationTokenId].tokenId].tokenStr, tokensTable[tokenId].notationTokens[currNotationTokenId].tokenId);
							}
						}
					}
					else if (tokensTable[tokenId].recordType & SEQUENCE_PATERN_TYPE_TA){
						for (unsigned int currNotationTokenId = 1; tokensTable[tokenId].notationTokens[currNotationTokenId].tokenId; ++currNotationTokenId){								
							printf(" ");
							if (tokensTable[tokenId].notationTokens[currNotationTokenId].atribute & MANDATORY_TOKEN_ATRIBUTE_TYPE_TA){
								printf("[");
							}
							if (tokensTable[tokenId].notationTokens[currNotationTokenId].atribute & REPEAT_TOKEN_ATRIBUTE_TYPE_TA){
								printf("{");
							}

							printOnlyNotation ? printf("%s", tokensTable[tokensTable[tokenId].notationTokens[currNotationTokenId].tokenId].tokenStr) :							
								                printf("%s(id=%d)", tokensTable[tokensTable[tokenId].notationTokens[currNotationTokenId].tokenId].tokenStr, tokensTable[tokenId].notationTokens[currNotationTokenId].tokenId);

							if (tokensTable[tokenId].notationTokens[currNotationTokenId].atribute & REPEAT_TOKEN_ATRIBUTE_TYPE_TA){
								printf("}");
							}
							if (tokensTable[tokenId].notationTokens[currNotationTokenId].atribute & MANDATORY_TOKEN_ATRIBUTE_TYPE_TA){
								printf("]");
							}
						}
					}
					else if (tokensTable[tokenId].recordType & VARIATIVE_EXCLUSION_PATERN_FOR_FIND_SUBNON_ABSTRACT_TOKEN_TYPE_TA){
						for (unsigned int currNotationTokenId = 1; tokensTable[tokenId].notationTokens[currNotationTokenId].tokenId; ++currNotationTokenId){
							printf(" /NOT/ ");

							printOnlyNotation ? printf("%s", tokensTable[tokensTable[tokenId].notationTokens[currNotationTokenId].tokenId].tokenStr) :
								printf("%s(id=%d)", tokensTable[tokensTable[tokenId].notationTokens[currNotationTokenId].tokenId].tokenStr, tokensTable[tokenId].notationTokens[currNotationTokenId].tokenId);

						}
					}
				}
				printf("\r\n");
			}
		}	
	}
}

void printTokensSequenceTable(unsigned char onlyText){
	if(onlyText){
		if (tokensSequenceTable){
			unsigned int rowNumber = 1;
			printf("\r\n");
			for (unsigned int index = 1; tokensSequenceTable[index].tokenId; ++index){
				if (rowNumber != tokensSequenceTable[index].row){
					rowNumber = tokensSequenceTable[index].row;
					printf("\r\n");
				}
				printf("%s ", tokensTable[tokensSequenceTable[index].tokenId].tokenStr);
			}
		}
	}
	else {
		if (tokensSequenceTable){
			unsigned int rowNumber = 1;
			printf("\r\n");
			printf("1: ", rowNumber);
			for (unsigned int index = 1; tokensSequenceTable[index].tokenId; ++index){
				if (rowNumber != tokensSequenceTable[index].row){
					rowNumber = tokensSequenceTable[index].row;
					printf("\r\n%d: ", rowNumber);
				}
				printf("<%s>(id=%d)", tokensTable[tokensSequenceTable[index].tokenId].tokenStr, tokensSequenceTable[index].tokenId);
			}
		}
	}
}