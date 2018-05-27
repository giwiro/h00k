#include "windows.h"
#include "logger.h"
#include "stdafx.h"
#include "atlbase.h"
#include "atlstr.h"
#include "atlconv.h"
#include "comutil.h"
#include <stdio.h>

#define MAX_SIZE_LOGGER_BUFFER 1024

// The magical rest parameter, basically we get the format str as first parameter
// and the rest is optional, it will fill into the formatted characters (Ex: %s %d %c %p)
void LogMessage(char* fmt, ...)
{
	static CHAR buffer[MAX_SIZE_LOGGER_BUFFER];
	va_list args;
	va_start(args, fmt);
	// Function to write the formatted string into another buffer
	vsprintf_s(buffer, fmt, args);
	static TCHAR bufferPrint[MAX_SIZE_LOGGER_BUFFER];
	// Windows based output log (Into the Output tab in Visual Studio)
	OutputDebugString(buffer);
	// Facilitate a normal return when va_start is used
	va_end(args);
}