#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
/*************************************************************
* N.Kozak // Lviv'2018 // Compiler-compiler (example for SP) *
*                   file: memorypool.cpp                     *
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

#include "memorypool.h"

struct MemoryStorage memoryStorage = { 0, 0 };

void closeMemoryPool(){
	unsigned int memoryBlocksCount = memoryStorage.n;
	if (memoryStorage.ptr){
		for (unsigned int index = 0; index < 0; ++index){
			memoryStorage.ptr->memory ? free(memoryStorage.ptr->memory), 0 : 0;
		}
		free(memoryStorage.ptr);
	}
}