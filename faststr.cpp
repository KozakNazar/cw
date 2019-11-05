#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
/*************************************************************
* N.Kozak // Lviv'2018 // Compiler-compiler (example for SP) *
*                   file: faststr.cpp                        *
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

#include "faststr.h"

char * findStrInTowoZeroEndStrs(char * strs, char * str, unsigned int deep) {
	for (const char *strs_ = strs, *str_ = str;;){
		if (!(*strs_ || *str_)){
			return (char *)strs;
		}
		else if (!*(short*)strs_) {
			return NULL;
		}
		else if (*strs_++ != *str_++) {
			strs_ = ++strs;
			str_ = str;
		}
	}
}

char * getStrFromTowoZeroEndStrs(char * strs, unsigned int *strsSize, char * str) {

	char * strptr = findStrInTowoZeroEndStrs(strs, str, 0);

	if (strptr){
		return strptr;
	}

	strcpy(strs + *strsSize + 1, str);
	strptr = strs + *strsSize + 1;
	*strsSize += strlen(str) + 1;

	return strptr;
}