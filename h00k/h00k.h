#pragma once

#include "stdafx.h"
#include "resource.h"

size_t GetForegroundTitle(TCHAR *title);
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
void UnhookKeyboard();
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow);

