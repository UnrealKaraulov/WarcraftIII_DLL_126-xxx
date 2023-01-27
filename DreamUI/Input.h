#include "stdafx.h"
#ifndef INPUT_H_INCLUDED_
#define INPUT_H_INCLUDED_

#include "GameStructs.h"

struct MousePosition {
	float x;
	float y;
	MousePosition() : x(0), y(0) {}
	MousePosition(float _x, float _y) : x(_x), y(_y) {}
};

struct ScreenPosition {
	float x;
	float y;
	ScreenPosition() : x(0), y(0) {}
	ScreenPosition(float _x, float _y) : x(_x), y(_y) {}
};

bool isChatBoxOn();

//点击按钮
void GameUIButtonClick(void* button, int mouseCode, bool sendAsProgramm);
//点击按键
void GameUIKeyPress(int keyCode, bool down, bool sendAsProgramm);

void Input_Init();
bool Input_Update(war3::CEvent* evt);
void ItemClick_Report(war3::CEvent* evt);

bool KeyIsDown(const unsigned int keyCode);

MousePosition* GetMousePosition();
ScreenPosition* GetScreenPositionFromWar3(float _x, float _y);
ScreenPosition* GetWar3ScreenPosition(float _x, float _y);
ScreenPosition* GetWar3ScreenPosition(unsigned int _x, unsigned int _y);
void* PositionGetButton(float x, float y, bool ignoreTooLarge = true);
war3::CCommandButton* HotkeyGetButton(unsigned int key);
bool IsCancelPanelOn();
extern bool GlobalSkipInputSingleShot;

#endif