#include "stdafx.h"
#ifndef CURSOR_LOCK_H_INCLUDED_
#define CURSOR_LOCK_H_INCLUDED_

typedef bool(*CustomWndProc)(HWND win, unsigned int message, WPARAM wParam, LPARAM lParam);

void AddWindowProc(CustomWndProc proc);
HWND GetWar3Window();
void War3Window_Init();
void War3Window_Cleanup();
bool KeyIsDown(const unsigned int keyCode);
void SetWar3Window(HWND war3hwnd);
LRESULT __fastcall DreamUI_WarWindow3Proc(HWND hwnd, unsigned int uMsg, WPARAM wParam, LPARAM lParam);
float War3WindowRatioX();
float War3WindowRatioY();
float GetWindowSizeY();
float GetWindowSizeX();

float GetMousePosY();
float GetMousePosX();
#endif