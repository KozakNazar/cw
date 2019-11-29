#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
/*************************************************************
* N.Kozak // Lviv'2018 // Compiler-compiler (example for SP) *
*                   file: scanner.cpp                        *
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
#include "translator.h"

void initScanner();
void runScanner(char * source);
struct Scanner scanner = { initScanner, runScanner };
void initScanner(){
}
struct LexemInfo{
	char lexemStr[MAX_LEXEM_SIZE];
	unsigned int lexemId;
	unsigned int tokenType;
	unsigned int ifvalue;
	unsigned int row;
	unsigned int col;
	// TODO: ...
};

struct LexemInfo lexemesInfoTable[MAX_WORD_COUNT] = { { "", 0, 0, 0 } };
struct LexemInfo * lastLexemInfoInTable = lexemesInfoTable; // first for begin

char identifierIdsTable[MAX_WORD_COUNT][MAX_LEXEM_SIZE] = { "" };

char strongReservedLexemes[MAX_STRONG_RESERVED_LEXEMES_COUNT + 1][MAX_LEXEM_SIZE] = {
	",", ";",                                          //(1)
	//(2)
	">>",                                              //(3)
	//(4)
	//(5)
	//(6)
	"+", "-", "*",                                     //(7)
	"==", "!=", ">=", "<=",                            //(8)
	//(9)
	//(10)
	//(11)
	":", "(", ")",                                     //(12(+))
	"'"
};

char reservedLexemes[MAX_RESERVED_LEXEMES_COUNT + 1][MAX_LEXEM_SIZE] = {
	"NAME", "BODY", "DATA", "END",                     //(1)
	"GET", "PUT",                                      //(2)
	//(3)
	"FOR", "TO", "DO", "IF", "THEN", "ELSE", "GOTO",   //(4)
	//(5)
	//(6)
	"DIV", "MOD",                                      //(7)
	//(8)
	"NOT",                                             //(9)
	"INTEGER16",                                       //(10)
	//(11)
	"WHILE",                                           //(12(+))
	"'"
};

char reservedLexemesForCommentSpace[MAX_COMMENT_LEXEMES_COUNT + 1][MAX_LEXEM_SIZE] = {
	//(1)
	//(2)
	//(3)
	//(4)
	//(5)
	//(6)
	//(7)
	//(8)
	//(9)
	//(10)
	"/*", "*/",                                        //(11)
	"'"
};

char identifierCasePattern[] = "_UUUUUUU";

void printLexemes(struct LexemInfo * lexemInfoTable, char printBadLexeme){
	if (printBadLexeme){
		printf("Bad lexeme:\r\n");
	}
	else{
		printf("Lexemes table:\r\n");
	}
	printf("-------------------------------------------------------------------\r\n");
	printf("index\t\tlexeme\t\tid\ttype\tifvalue\trow\tcol\r\n");
	printf("-------------------------------------------------------------------\r\n");
	for (unsigned int index = 0; (!index || !printBadLexeme) && lexemInfoTable[index].lexemStr[0] != '\0'; ++index){
		printf("%5d%17s%12d%10d%11d%4d%8d\r\n", index, lexemInfoTable[index].lexemStr, lexemInfoTable[index].lexemId, lexemInfoTable[index].tokenType, lexemInfoTable[index].ifvalue, lexemInfoTable[index].row, lexemInfoTable[index].col);
	}
	printf("-------------------------------------------------------------------\r\n\r\n");

	return;
}

// try to get strong reserved lexeme
unsigned int tryToGetStrongReservedLexeme(struct LexemInfo ** lastLexemInfoInTable, char * str, char(*strongReservedLexemes)[MAX_LEXEM_SIZE], unsigned int row, unsigned int col){
	for (unsigned int index = 0; strongReservedLexemes[index][0] != '\''; ++index){
		unsigned int currLexemeLength = strlen(strongReservedLexemes[index]);
		if (!strncmp(str, strongReservedLexemes[index], currLexemeLength) && currLexemeLength <= strlen(str)){
			strncpy((*lastLexemInfoInTable)->lexemStr, strongReservedLexemes[index], MAX_LEXEM_SIZE);
			(*lastLexemInfoInTable)->row = row;
			(*lastLexemInfoInTable)->col = col;
			(*lastLexemInfoInTable)->lexemId = index;
			(*lastLexemInfoInTable)->tokenType = STRONG_RESERVED_LEXEME_TYPE;
			++(*lastLexemInfoInTable);
			return currLexemeLength;
		}
	}
	return 0;
}

// try to get reserved lexeme
unsigned int tryToGetReservedLexeme(struct LexemInfo ** lastLexemInfoInTable, char * str, char(*reservedLexemes)[MAX_LEXEM_SIZE], char(*strongReservedLexemes)[MAX_LEXEM_SIZE], unsigned int row, unsigned int col){
	unsigned int currSize = strlen(str);
	for (unsigned int index = 0; reservedLexemes[index][0] != '\''; ++index){
		unsigned int currLexemeLength = strlen(reservedLexemes[index]);
		if (currLexemeLength && !strncmp(str, reservedLexemes[index], currLexemeLength)){
			if (currSize == currLexemeLength) {
				strncpy((*lastLexemInfoInTable)->lexemStr, reservedLexemes[index], MAX_LEXEM_SIZE);
				(*lastLexemInfoInTable)->row = row;
				(*lastLexemInfoInTable)->col = col;
				(*lastLexemInfoInTable)->lexemId = MAX_RESERVED_LEXEMES_COUNT + index;
				(*lastLexemInfoInTable)->tokenType = RESERVED_LEXEME_TYPE;
				++(*lastLexemInfoInTable);
				return currLexemeLength;
			}
			else if (currSize > currLexemeLength){
				unsigned int strongLexemeLength = tryToGetStrongReservedLexeme(lastLexemInfoInTable, str + currLexemeLength, strongReservedLexemes, row, col);
				if (strongLexemeLength){
					strncpy((*lastLexemInfoInTable)->lexemStr, reservedLexemes[index], currLexemeLength);
					(*lastLexemInfoInTable)->lexemStr[currLexemeLength] = '\0';
					(*lastLexemInfoInTable)->row = row;
					(*lastLexemInfoInTable)->col = col;
					(*lastLexemInfoInTable)->lexemId = MAX_RESERVED_LEXEMES_COUNT + index;
					(*lastLexemInfoInTable)->tokenType = RESERVED_LEXEME_TYPE;
					++(*lastLexemInfoInTable);
					return currLexemeLength;
				}
			}
		}
	}

	return 0;
}

// get identifier id
unsigned int getIdentifierId(char(*identifierIdsTable)[MAX_LEXEM_SIZE], char * str){
	unsigned int index = 0;
	for (; identifierIdsTable[index][0] != '\0'; ++index){
		if (!strncmp(identifierIdsTable[index], str, MAX_LEXEM_SIZE)){
			return index;
		}
	}
	strncpy(identifierIdsTable[index], str, MAX_LEXEM_SIZE);
	identifierIdsTable[index + 1][0] = '\0'; // not necessarily for zero-init identifierIdsTable
	return index;
}

// try to get identifier
unsigned int tryToGetIdentifier(struct LexemInfo ** lastLexemInfoInTable, char * str, char(*strongReservedLexemes)[MAX_LEXEM_SIZE], char * identifierCasePattern, char(*identifierIdsTable)[MAX_LEXEM_SIZE], unsigned int row, unsigned int col){
	unsigned int currSize = strlen(str);
	unsigned int currLexemeLength = 0;
	for (currLexemeLength = 0; currLexemeLength < currSize && (
		(identifierCasePattern[currLexemeLength] == 'L' && str[currLexemeLength] >= 'a' && str[currLexemeLength] <= 'z')
		|| (identifierCasePattern[currLexemeLength] == 'U' && str[currLexemeLength] >= 'A' && str[currLexemeLength] <= 'Z')
		|| (identifierCasePattern[currLexemeLength] == '_' && str[currLexemeLength] == '_')
		);
	++currLexemeLength);

	if (currLexemeLength < strlen(identifierCasePattern)){
		return 0;
	}

	if (currSize == currLexemeLength) {
		strncpy((*lastLexemInfoInTable)->lexemStr, str, MAX_LEXEM_SIZE);
		(*lastLexemInfoInTable)->row = row;
		(*lastLexemInfoInTable)->col = col;
		(*lastLexemInfoInTable)->lexemId = MAX_STRONG_RESERVED_LEXEMES_COUNT + MAX_RESERVED_LEXEMES_COUNT + getIdentifierId(identifierIdsTable, str);
		(*lastLexemInfoInTable)->tokenType = IDENTIFIER_LEXEME_TYPE;
		++(*lastLexemInfoInTable);
		return currLexemeLength;
	}
	else {
		unsigned int strongLexemeLength = tryToGetStrongReservedLexeme(lastLexemInfoInTable, str + currLexemeLength, strongReservedLexemes, row, col);
		if (strongLexemeLength){
			strncpy((*lastLexemInfoInTable)->lexemStr, str, currLexemeLength);
			(*lastLexemInfoInTable)->lexemStr[currLexemeLength] = '\0';
			(*lastLexemInfoInTable)->row = row;
			(*lastLexemInfoInTable)->col = col;
			(*lastLexemInfoInTable)->lexemId = MAX_STRONG_RESERVED_LEXEMES_COUNT + MAX_RESERVED_LEXEMES_COUNT + getIdentifierId(identifierIdsTable, str);
			(*lastLexemInfoInTable)->tokenType = IDENTIFIER_LEXEME_TYPE;
			++(*lastLexemInfoInTable);
			return currLexemeLength;
		}
	}

	return 0;
}

// try to get value
unsigned int tryToGetValue(struct LexemInfo ** lastLexemInfoInTable, char * str, char(*strongReservedLexemes)[MAX_LEXEM_SIZE], unsigned int row, unsigned int col){
	unsigned int currSize = strlen(str);
	unsigned int currLexemeLength = 0;
	for (currLexemeLength = 0; currLexemeLength < currSize && (!currLexemeLength && (str[currLexemeLength] == '+' || str[currLexemeLength] == '-') || str[currLexemeLength] >= '0' && str[currLexemeLength] <= '9'); ++currLexemeLength);

	if (!currLexemeLength || currLexemeLength == 1 && (str[0] == '+' || str[0] == '-')){
		return 0;
	}

	if (currSize == currLexemeLength) {
		strncpy((*lastLexemInfoInTable)->lexemStr, str, MAX_LEXEM_SIZE);
		(*lastLexemInfoInTable)->row = row;
		(*lastLexemInfoInTable)->col = col;
		(*lastLexemInfoInTable)->ifvalue = atoi(str);
		(*lastLexemInfoInTable)->lexemId = MAX_STRONG_RESERVED_LEXEMES_COUNT + MAX_RESERVED_LEXEMES_COUNT + MAX_WORD_COUNT;
		(*lastLexemInfoInTable)->tokenType = VALUE_LEXEME_TYPE;
		++(*lastLexemInfoInTable);
		return currLexemeLength;
	}
	else {
		unsigned int strongLexemeLength = tryToGetStrongReservedLexeme(lastLexemInfoInTable, str + currLexemeLength, strongReservedLexemes, row, col);
		if (strongLexemeLength){
			strncpy((*lastLexemInfoInTable)->lexemStr, str, currLexemeLength);
			(*lastLexemInfoInTable)->lexemStr[currLexemeLength] = '\0';
			(*lastLexemInfoInTable)->row = row;
			(*lastLexemInfoInTable)->col = col;
			(*lastLexemInfoInTable)->ifvalue = atoi(str);
			(*lastLexemInfoInTable)->lexemId = MAX_STRONG_RESERVED_LEXEMES_COUNT + MAX_RESERVED_LEXEMES_COUNT + MAX_WORD_COUNT;
			(*lastLexemInfoInTable)->tokenType = VALUE_LEXEME_TYPE;
			++(*lastLexemInfoInTable);
			return currLexemeLength;
		}
	}

	return 0;
}

// try to get label
unsigned int tryToGetLabel(struct LexemInfo ** lastLexemInfoInTable, char * str, char(*strongReservedLexemes)[MAX_LEXEM_SIZE], unsigned int row, unsigned int col){
	unsigned int currSize = strlen(str);
	unsigned int currLexemeLength = 0;
	for (currLexemeLength = 0; currLexemeLength < currSize && (!currLexemeLength || str[currLexemeLength] >= 'a' && str[currLexemeLength] <= 'z'); ++currLexemeLength);

	if (currSize == currLexemeLength) {
		strncpy((*lastLexemInfoInTable)->lexemStr, str, MAX_LEXEM_SIZE);
		(*lastLexemInfoInTable)->row = row;
		(*lastLexemInfoInTable)->col = col;
		(*lastLexemInfoInTable)->ifvalue = atoi(str);
		(*lastLexemInfoInTable)->lexemId = MAX_STRONG_RESERVED_LEXEMES_COUNT + MAX_RESERVED_LEXEMES_COUNT + (2 * MAX_WORD_COUNT);
		(*lastLexemInfoInTable)->tokenType = LABEL_LEXEME_TYPE;
		++(*lastLexemInfoInTable);
		return currLexemeLength;
	}
	else {
		unsigned int strongLexemeLength = tryToGetStrongReservedLexeme(lastLexemInfoInTable, str + currLexemeLength, strongReservedLexemes, row, col);
		if (strongLexemeLength){
			strncpy((*lastLexemInfoInTable)->lexemStr, str, currLexemeLength);
			(*lastLexemInfoInTable)->lexemStr[currLexemeLength] = '\0';
			(*lastLexemInfoInTable)->row = row;
			(*lastLexemInfoInTable)->col = col;
			(*lastLexemInfoInTable)->ifvalue = atoi(str);
			(*lastLexemInfoInTable)->lexemId = MAX_STRONG_RESERVED_LEXEMES_COUNT + MAX_RESERVED_LEXEMES_COUNT + (2 * MAX_WORD_COUNT);
			(*lastLexemInfoInTable)->tokenType = LABEL_LEXEME_TYPE;
			++(*lastLexemInfoInTable);
			return currLexemeLength;
		}
	}

	return 0;
}

struct LexemInfo runLexicalAnalysis(struct LexemInfo ** lastLexemInfoInTable, char * text, char(*strongReservedLexemes)[MAX_LEXEM_SIZE], char(*reservedLexemes)[MAX_LEXEM_SIZE], char * identifierCasePattern, char(*identifierIdsTable)[MAX_LEXEM_SIZE]){
	struct LexemInfo ifBadLexemeInfo = { 0 };
	char * const endOfText = text + strlen(text);
	for (unsigned int addonSize = 0, row = 1, col = 1; text < endOfText; addonSize = 0){
		char part[MAX_LEXEM_SIZE] = { 0 };
		int ifNewEndLine = sscanf(text, "%*[' ''\t''\r']%['\n']", part);
		if (ifNewEndLine > 0) ++row;
		while (ifNewEndLine && ifNewEndLine != EOF){
			text = strstr(text, "\n") + 1;
			col = 1;
			ifNewEndLine = sscanf(text, "%*[' ''\t''\r']%['\n']", part);
			if (ifNewEndLine > 0) ++row;
		}
		int currScan = sscanf(text, "%s", part);
		if (!currScan || currScan == EOF){
			break;
		}
		unsigned int offsetToNewSubStr = strstr(text, part) - text;
		text += offsetToNewSubStr;
		col += offsetToNewSubStr;

		// try to get strong reserved lexeme
		if (!addonSize) {
			addonSize = tryToGetStrongReservedLexeme(lastLexemInfoInTable, part, strongReservedLexemes, row, col);
		}

		// try to get reserved lexeme
		if (!addonSize) {
			addonSize = tryToGetReservedLexeme(lastLexemInfoInTable, part, reservedLexemes, strongReservedLexemes, row, col);
		}

		// try to get identifier
		if (!addonSize) {
			addonSize = tryToGetIdentifier(lastLexemInfoInTable, part, strongReservedLexemes, identifierCasePattern, identifierIdsTable, row, col);
		}

		// try to get value
		if (!addonSize) {
			addonSize = tryToGetValue(lastLexemInfoInTable, part, strongReservedLexemes, row, col);
		}

		// try to get label
		if (!addonSize) {
			addonSize = tryToGetLabel(lastLexemInfoInTable, part, strongReservedLexemes, row, col);
		}

		if (addonSize) {
			text += addonSize;
			col += addonSize;

			unsigned int tokenId = 0;
			(tokenId = tryGetTokenId(tokensTable, part, KEYWORD_TOKEN_TYPE)) ||
				(tokenId = tryGetTokenId(tokensTable, part, RAW_TOKEN_TYPE));
			if (!tokenId && part[0] != '\0'){
				tokenId = addRecordToTokensTable(part, RAW_TOKEN_TYPE);
			}
			if (tokenId){
				addRecordToTokensSequenceTable(tokenId, RAW_TOKEN_TYPE, row, col);
			}
		}
		else{
			strncpy(ifBadLexemeInfo.lexemStr, part, MAX_LEXEM_SIZE);
			ifBadLexemeInfo.row = row;
			ifBadLexemeInfo.col = col;
			ifBadLexemeInfo.tokenType = UNEXPEXTED_LEXEME_TYPE;
			return ifBadLexemeInfo;
		}
	}

	return ifBadLexemeInfo;
}

int commentRemover(char * text, char * openStrSpc, char * closeStrSpc, unsigned char eofAlternativeCloseStrSpcType/* = true*/, unsigned char explicitCloseStrSpc/* = false*/){
	unsigned int commentSpace = 0;

	unsigned int textLength = strlen(text);               // strnlen(text, MAX_TEXT_SIZE)
	unsigned int openStrSpcLength = strlen(openStrSpc);   // strnlen(openStrSpc, MAX_TEXT_SIZE)
	unsigned int closeStrSpcLength = strlen(closeStrSpc); // strnlen(closeStrSpc, MAX_TEXT_SIZE)
	if (!closeStrSpcLength){
		return -1; // no set closeStrSpc
	}
	unsigned char oneLevelComment = 0;
	if (!strncmp(openStrSpc, closeStrSpc, MAX_LEXEM_SIZE)){
		oneLevelComment = 1;
	}

	for (unsigned int index = 0; index < textLength; ++index){
		if (!strncmp(text + index, closeStrSpc, closeStrSpcLength) && (explicitCloseStrSpc || commentSpace)) {
			if (commentSpace == 1 && explicitCloseStrSpc){
				for (unsigned int index2 = 0; index2 < closeStrSpcLength; ++index2){
					text[index + index2] = ' ';
				}
			}
			else if (commentSpace == 1 && !explicitCloseStrSpc){
				index += closeStrSpcLength - 1;
			}
			oneLevelComment ? (commentSpace = !commentSpace) : (commentSpace = 0);
		}
		else if (!strncmp(text + index, openStrSpc, openStrSpcLength)) {
			oneLevelComment ? (commentSpace = !commentSpace) : (commentSpace = 1);
		}

		if (commentSpace && text[index] != ' ' && text[index] != '\t' && text[index] != '\r' && text[index] != '\n'){
			text[index] = ' ';
		}

	}

	if (commentSpace && !eofAlternativeCloseStrSpcType){
		return -1;
	}

	return 0;
}

void runScanner(char * source){
	unsigned char eofAlternativeCloseStrSpcType = 0;
	unsigned char explicitCloseStrSpc = 1;
	if (!strcmp(reservedLexemesForCommentSpace[1], "\r\n")){
		eofAlternativeCloseStrSpcType = 1;
		explicitCloseStrSpc = 0;
	}
	int commentRemoverResult = commentRemover(source, reservedLexemesForCommentSpace[0], reservedLexemesForCommentSpace[1], eofAlternativeCloseStrSpcType, explicitCloseStrSpc);
	if (commentRemoverResult){
		printf("Comment remover return %d\r\n", commentRemoverResult);
		printf("Press any key to continue . . .");
		getchar();
		return;
	}
	if (mode & DEBUG_MODE){
		printf("Source after comment removing:\r\n");
		printf("-------------------------------------------------------------------\r\n");
		printf("%s\r\n", source);
		printf("-------------------------------------------------------------------\r\n\r\n");
	}

	struct LexemInfo ifBadLexemeInfo = runLexicalAnalysis(&lastLexemInfoInTable, source, strongReservedLexemes, reservedLexemes, identifierCasePattern, identifierIdsTable);
	if (ifBadLexemeInfo.tokenType == UNEXPEXTED_LEXEME_TYPE){
		UNEXPEXTED_LEXEME_TYPE;
		ifBadLexemeInfo.tokenType;
		printf("Lexical analysis detected unexpexted lexeme\r\n");
		printLexemes(&ifBadLexemeInfo, 1);
		printf("Press any key to continue . . .");
		getchar();
		return;
	}
	if (mode & DEBUG_MODE){
		printLexemes(lexemesInfoTable, 0);
	}
	else{
		printf("Lexical analysis complete successe\r\n");
	}

	printf("Press any key to next steps ");
	getchar();
}