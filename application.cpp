#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
//#define USE_PREDEFINED_PARAMETERS // enable this define for use predefined value
/*************************************************************
* N.Kozak // Lviv'2018 // Compiler-compiler (example for SP) *
*                   file: application.cpp                    *
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
#include <windows.h>
//#include <winbase.h>
//#include <winuser.h>
//#include <shlobj.h>
//#include <shlwapi.h>
//#include <objbase.h>

#include "stdio.h"
#include "stdlib.h"
//#include "conio.h"

#include "locale.h"

#include <direct.h>

#include "tables.h"
#include "scanner.h"
#include "translator.h"


void comandLineParser(int argc, char* argv[], unsigned int * mode, char(*parameters)[MAX_PARAMETERS_SIZE]){
	char tempTemp[PATH_NAME_LENGH] = { '\0' }, *tempPtrPrev, *tempPtrNext, nameTemp[PATH_NAME_LENGH] = { '\0' };
	char useDefaultModes = 1;
	*mode = 0;
	for (int index = 1; index < argc; ++index){
		if (!strcmp(argv[index], "-lex")){
			*mode |= LEXICAL_ANALISIS_MODE;
			useDefaultModes = 0;
			continue;
		}
		else if (!strcmp(argv[index], "-stx")){
			*mode |= SYNTAX_ANALISIS_MODE;
			useDefaultModes = 0;
			continue;
		}
		else if (!strcmp(argv[index], "-smt")){
			*mode |= SEMANTIC_ANALISIS_MODE;
			useDefaultModes = 0;
			continue;
		}
		else if (!strcmp(argv[index], "-gen")){
			*mode |= MAKE_ASSEMBLY | MAKE_BINARY;
			useDefaultModes = 0;
			continue;
		}
		else if (!strcmp(argv[index], "-run")){
			*mode |= RUN_BINARY;
			useDefaultModes = 0;
			continue;
		}
		else if (!strcmp(argv[index], "-all")){
			*mode |= LEXICAL_ANALISIS_MODE | SYNTAX_ANALISIS_MODE | SEMANTIC_ANALISIS_MODE | MAKE_ASSEMBLY | MAKE_BINARY | RUN_BINARY;
			useDefaultModes = 0;
			continue;
		}
		else if (!strcmp(argv[index], "-d")){
			*mode |= DEBUG_MODE;
			useDefaultModes = 0;
			continue;
		}

		// other keys
		// TODO:...

		// input filename
		strncpy(parameters[INPUT_FILENAME_WITH_EXTENSION_PARAMETER], argv[index], MAX_PARAMETERS_SIZE);
	}

	// default mode,  if not entered manually
	if (!*mode/*useDefaultModes*/ ){
		if (parameters[INPUT_FILENAME_WITH_EXTENSION_PARAMETER][0]){
			*mode = LEXICAL_ANALISIS_MODE | SYNTAX_ANALISIS_MODE | SEMANTIC_ANALISIS_MODE | MAKE_ASSEMBLY | MAKE_BINARY;
		}
		else{
			//*mode = 0;
			printf("Used manual mode\r\n\r\n");
		}
	}

	// default input filename,  if not entered manually
	if (parameters[INPUT_FILENAME_WITH_EXTENSION_PARAMETER][0] == '\0'){
		strcpy(parameters[INPUT_FILENAME_WITH_EXTENSION_PARAMETER], DEFAULT_INPUT_FILENAME);
		printf("Input filename not setted. Used defaule input filename \"%s\"\r\n\r\n", parameters[INPUT_FILENAME_WITH_EXTENSION_PARAMETER]);
	}
	
	strncpy(tempTemp, parameters[INPUT_FILENAME_WITH_EXTENSION_PARAMETER], PATH_NAME_LENGH);
	tempPtrPrev = tempTemp;
	tempPtrPrev[0] == '\"' ? ++tempPtrPrev : 0;
	tempPtrNext = tempPtrPrev = strtok(tempPtrPrev, " .\\/:");
	while (tempPtrNext != NULL){
		tempPtrNext = strtok(NULL, " .\\/:");
		if (tempPtrPrev && tempPtrNext){
			strncpy(nameTemp, tempPtrPrev, PATH_NAME_LENGH);
		}
		tempPtrPrev = tempPtrNext;
	}

	// default input filename,  if not entered manually
	if (parameters[INOUT_ASSEMBLY_FILENAME_WITHOUT_EXTENSION_PARAMETER][0] == '\0'){
		strncpy(parameters[INOUT_ASSEMBLY_FILENAME_WITHOUT_EXTENSION_PARAMETER], nameTemp, PATH_NAME_LENGH);
		printf("Inout assembly filename not setted. Used defaule input filename \"%s\"\r\n\r\n", parameters[INOUT_ASSEMBLY_FILENAME_WITHOUT_EXTENSION_PARAMETER]);
	}

	// default input filename,  if not entered manually
	if (parameters[OUT_BINARY_FILENAME_WITHOUT_EXTENSION_PARAMETER][0] == '\0'){
		strncpy(parameters[OUT_BINARY_FILENAME_WITHOUT_EXTENSION_PARAMETER], nameTemp, PATH_NAME_LENGH);
		printf("Out binary filename not setted. Used defaule input filename \"%s\"\r\n\r\n", parameters[OUT_BINARY_FILENAME_WITHOUT_EXTENSION_PARAMETER]);
	}

	return;
}

// after using this function use free(void *) function to release text buffer
size_t loadSource(char ** text, char * fileName){
	if (!fileName){
		printf("No input file name\r\n");
		return 0;
	}

	FILE * file = fopen(fileName, "rb");

	if (file == NULL){
		printf("File not opened\r\n");
		return 0;
	}

	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);
	rewind(file);

	*text = (char*)malloc(sizeof(char)* (fileSize + 1));
	if (text == NULL) {
		fputs("Memory error", stderr); exit(2); // TODO: ...
	}

	size_t result = fread(*text, sizeof(char), fileSize, file);
	if (result != fileSize) {
		fputs("Reading error", stderr);
		fclose(file);
		exit(3); // TODO: ...
	}

	(*text)[fileSize] = '\0';

	fclose(file);

	return fileSize;
}

void saveProductionSource(char * text, char * fileName, long fileSize){

	if (!fileName){
		printf("No output file name\r\n");
		return;
	}

	FILE * file = fopen(fileName, "wb");

	if (file == NULL){
		printf("File not opened\r\n");
		return;
	}

	size_t result = fwrite(text, sizeof(char), fileSize, file);
	if (result != fileSize) {
		fputs("Writing error", stderr);
		fclose(file);
		exit(3); // TODO: ...
	}

	fclose(file);
}

int main(int argc, char* argv[]){	
	PostMessage(GetForegroundWindow(), WM_INPUTLANGCHANGEREQUEST, 2, (UINT)LoadKeyboardLayoutA("00000409", KLF_ACTIVATE));

	char path[PATH_NAME_LENGH];
	char temp[2 * PATH_NAME_LENGH];
	char productionOut[MAX_TEXT_SIZE] = { 0 };

	comandLineParser(argc, argv, &mode, parameters);
#ifdef	USE_PREDEFINED_PARAMETERS
	//mode = DEFAULT_MODE;
	char text[MAX_TEXT_SIZE] = PREDEFINED_TEXT1;
#else
	//comandLineParser(argc, argv, &mode, parameters);
	char * text;
	size_t sourceSize = loadSource(&text, parameters[INPUT_FILENAME_WITH_EXTENSION_PARAMETER]);
	if (!sourceSize){
		printf("Press any key to continue . . .");
		getchar();
		return 0;
	}
#endif

	if (!_getcwd(path, PATH_NAME_LENGH))
	{
		printf("getcwd error ...\r\n");
		printf("Press any key to continue . . .");
		return -1;
	}

	if (!mode){
		printf("Enter 'y' to assembly program action(to pass action process enter 'n' or others key)");
	}	
	fflush(stdin);
	if (mode & MAKE_ASSEMBLY || getchar() == 'y'){
		if (false && !(mode & LEXICAL_ANALISIS_MODE)){
			printf("NO SUPORTED MODE ...\r\n");
			printf("Press any key to continue . . .");
			getchar();
			return 0;
		}

		if (mode & DEBUG_MODE){
			printf("Original source:\r\n");
			printf("-------------------------------------------------------------------\r\n");
			printf("%s\r\n", text);
			printf("-------------------------------------------------------------------\r\n\r\n");
		}

		translator.init();
		translator.run(text, productionOut);

#ifndef	USE_PREDEFINED_PARAMETERS
		free(text);
#endif
		//sprintf(temp, "masm32p\\%s.asm", parameters[INOUT_ASSEMBLY_FILENAME_WITHOUT_EXTENSION_PARAMETER]);
		sprintf(temp, "%s.asm", parameters[INOUT_ASSEMBLY_FILENAME_WITHOUT_EXTENSION_PARAMETER]);
		saveProductionSource(productionOut, temp, (long)strlen(productionOut));
		translator.close();

		if (!mode){
			printf("\r\n\r\nPress any key to next step");
			fflush(stdin);
			getchar();
			system("CLS");
			fflush(stdin);
		}
	}
	else if (mode ^ MAKE_ASSEMBLY){
		printf("\r\n");
	}

	if (!mode){
		printf("Enter 'y' to compile program action(to pass action process enter 'n' or others key)");
	}
	fflush(stdin);
	if (mode & MAKE_BINARY || getchar() == 'y'){
		//sprintf(temp, "START /b /wait \"\" /D \"%s\\masm32p\" make_cw.bat %s && IF ERRORLEVEL 1 EXIT ERRORLEVEL", path, parameters[INOUT_ASSEMBLY_FILENAME_WITHOUT_EXTENSION_PARAMETER]);
		sprintf(temp, "START /b /wait \"\" make_cw.bat %s && IF ERRORLEVEL 1 EXIT ERRORLEVEL", parameters[INOUT_ASSEMBLY_FILENAME_WITHOUT_EXTENSION_PARAMETER]);
		if (system((char *)temp))
		{
			fflush(stdin);
			printf("\r\n\r\nCompile error\r\nPress any key to continue . . .");
			getchar();
			return 0;
		}
		else if (!mode){
			fflush(stdin);
			printf("\r\n\r\nPress any key to next step");
			getchar();
			system("CLS");
			fflush(stdin);
		}
	}
	else if (mode ^ MAKE_BINARY){
		printf("\r\n");
	}

	if (!mode){
		printf("Enter 'y' to run program action(to pass action process enter 'n' or others key)");
	}
	fflush(stdin);
	if (mode & RUN_BINARY || getchar() == 'y'){
		printf("\r\n");
		//sprintf(temp, "START /b /wait \"\" /D \"%s\\masm32p\" %s.exe", path, parameters[OUT_BINARY_FILENAME_WITHOUT_EXTENSION_PARAMETER]);
		sprintf(temp, "START /b /wait \"\" %s.exe", parameters[OUT_BINARY_FILENAME_WITHOUT_EXTENSION_PARAMETER]);
		fflush(stdin);
		system((char *)temp);
		fflush(stdin);
	}
	else if (mode ^ RUN_BINARY){
		printf("\r\n");
	}

	printf("\r\n\r\nPress any key to continue . . .");
	getchar();

	return 0;
}
