/*************************************************************
* N.Kozak // Lviv'2018 // Compiler-compiler (example for SP) *
*                   file: memorypool.h                       *
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
struct MemoryBlocks{
	void * memory;
	unsigned int x_n;
	unsigned int y_n;
	unsigned int z_n;
	unsigned int n;
};

extern struct MemoryStorage{
	struct MemoryBlocks * ptr;
	unsigned int n;
} memoryStorage;

void closeMemoryPool();