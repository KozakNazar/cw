#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
/*************************************************************
* N.Kozak // Lviv'2018 // Compiler-compiler (example for SP) *
*                   file: syntaxer.cpp                       *
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
#include "syntaxer.h"

unsigned int initSyntaxer();
unsigned int runSyntaxer(char * source);
struct Syntaxer syntaxer = { initSyntaxer, runSyntaxer };

unsigned int initSyntaxer(){
	return 0;
}

unsigned int runSyntaxer(char * source){
	unsigned int programPatternId = getTokenId(tokensTable, &tokensTableLastFreeIndex, (char*)"<program>", NONMARKER, 0);
	if (tokensTable[programPatternId].recordType & NONMARKER){
		return 0;
	}
	char *captureByPaternResult;
	unsigned int variativeSelectorPass = 0;
	printf("\r\n\r\n"); 
	unsigned int productionIndex = 1;
	captureByPaternResult = captureByPatern((char *)1, programPatternId, 0, ~0, &productionIndex, variativeSelectorPass, MAX_CAPTURE_DEEP);
	if (!captureByPaternResult){
		printf("captureByPatern fail...\r\n");
		getchar();
		return 0;
	}

	return 0;
}
