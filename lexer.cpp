#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
/*************************************************************
* N.Kozak // Lviv'2018 // Compiler-compiler (example for SP) *
*                   file: lexer.cpp                          *
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
#include "lexer.h"

unsigned int initLexer();
unsigned int runLexer(char * source);
struct Lexer lexer = { initLexer, runLexer };

unsigned int initLexer(){
	return 0;
}

unsigned int runLexer(char * source){
	unsigned int notationsBlockPatternId = getTokenId(tokensTable, &tokensTableLastFreeIndex, (char*)"<notations_block>", NONMARKER, 0);
	if (tokensTable[notationsBlockPatternId].recordType & NONMARKER){
		return 0;
	}
	unsigned int identPatternId = getTokenId(tokensTable, &tokensTableLastFreeIndex, (char*)"<ident>", NONMARKER, 0);
	if (tokensTable[identPatternId].recordType & NONMARKER){
		return 0;
	}
	unsigned int valuePatternId = getTokenId(tokensTable, &tokensTableLastFreeIndex, (char*)"<value>", NONMARKER, 0);
	if (tokensTable[valuePatternId].recordType & NONMARKER){
		return 0;
	}
	unsigned int labelPatternId = getTokenId(tokensTable, &tokensTableLastFreeIndex, (char*)"<label>", NONMARKER, 0);
	if (tokensTable[labelPatternId].recordType & NONMARKER){
		return 0;
	}

	for (unsigned int tokensSequenceTableIndex = 1; tokensSequenceTableIndex && tokensSequenceTable[tokensSequenceTableIndex].tokenId; ++tokensSequenceTableIndex){			
		unsigned int tokensSequenceTableIndex_;
		tokensSequenceTableIndex_ = (unsigned int)captureByPatern((char*)tokensSequenceTableIndex, notationsBlockPatternId, 0, COMMENTED_TOKEN_TYPE, NULL, 0, MAX_CAPTURE_DEEP);
		tokensSequenceTableIndex_ ? tokensSequenceTableIndex = tokensSequenceTableIndex_ - 1 : 0;
	}		

	for (unsigned int newTokensSequenceTableIndex = 1, tokensSequenceTableIndex = 1;; ++newTokensSequenceTableIndex, ++tokensSequenceTableIndex){			
		for (; tokensSequenceTable[tokensSequenceTableIndex].tokenId && tokensSequenceTable[tokensSequenceTableIndex].flags & COMMENTED_TOKEN_TYPE; ++tokensSequenceTableIndex);			
		if (!tokensSequenceTable[tokensSequenceTableIndex].tokenId){
			break;
		}
		if (newTokensSequenceTableIndex != tokensSequenceTableIndex){		
			tokensSequenceTable[newTokensSequenceTableIndex] = tokensSequenceTable[tokensSequenceTableIndex];		
		}		
	}

	for (unsigned int tokensSequenceTableIndex = 1; tokensSequenceTable[tokensSequenceTableIndex].tokenId; ++tokensSequenceTableIndex){
		if (tokensSequenceTable[tokensSequenceTableIndex].flags & RAW_TOKEN_TYPE){
			char *captureByPaternResult;
			unsigned int valueId = tryGetTokenId(tokensTable, tokensTable[tokensSequenceTable[tokensSequenceTableIndex].tokenId].tokenStr, KEYWORD_TOKEN_TYPE);
			if (valueId){
				tokensTable[tokensSequenceTable[tokensSequenceTableIndex].tokenId].recordType |= valueId;
				tokensSequenceTable[tokensSequenceTableIndex].flags = valueId;
				continue;
			}
			captureByPaternResult = captureByPatern(tokensTable[tokensSequenceTable[tokensSequenceTableIndex].tokenId].tokenStr, identPatternId, 1, ~0, NULL, 0, MAX_CAPTURE_DEEP);
			if (captureByPaternResult){
				tokensTable[tokensSequenceTable[tokensSequenceTableIndex].tokenId].recordType |= identPatternId;
				tokensSequenceTable[tokensSequenceTableIndex].flags = identPatternId;
				continue;
			}
			captureByPaternResult = captureByPatern(tokensTable[tokensSequenceTable[tokensSequenceTableIndex].tokenId].tokenStr, valuePatternId, 1, ~0, NULL, 0, MAX_CAPTURE_DEEP);
			if (captureByPaternResult){
				tokensTable[tokensSequenceTable[tokensSequenceTableIndex].tokenId].recordType |= valuePatternId;
				tokensSequenceTable[tokensSequenceTableIndex].flags = valuePatternId;
				continue;
			}
			captureByPaternResult = captureByPatern(tokensTable[tokensSequenceTable[tokensSequenceTableIndex].tokenId].tokenStr, labelPatternId, 1, ~0, NULL, 0, MAX_CAPTURE_DEEP);
			if (captureByPaternResult){
				tokensTable[tokensSequenceTable[tokensSequenceTableIndex].tokenId].recordType |= labelPatternId;
				tokensSequenceTable[tokensSequenceTableIndex].flags = labelPatternId;
				continue;
			}
		}
	}

	return 0;
}
