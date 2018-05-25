#include "windows.h"
#include "logger.h"
#include "stdafx.h"
#include "atlbase.h"
#include "atlstr.h"
#include "atlconv.h"
#include "comutil.h"
#include <string>
#include <stdio.h>

#define MAX_SIZE_LOGGER_BUFFER 1024

void LogMessage(char* fmt, ...) {
	static CHAR buffer[MAX_SIZE_LOGGER_BUFFER];
	va_list args;
	va_start(args, fmt);
	vsprintf_s(buffer, fmt, args);
	static TCHAR bufferPrint[MAX_SIZE_LOGGER_BUFFER];
	OutputDebugString(buffer);
	va_end(args);
}