#include "stdafx.h"
#include "Input.h"
#include "Tools.h"
#include "GameStructs.h"
#include "Game.h"
#include "Event.h"
#include "War3Window.h"
#include "Offsets.h"
#include "KeyCode.h"

double wc3_max_x = 0.8;
double wc3_max_y = 0.6;


float WindowMaxX = (float)GetSystemMetrics(SM_CXSCREEN);
float WindowMaxY = (float)GetSystemMetrics(SM_CYSCREEN);


static war3::CObserver* InputObserverObject;

static Event KeyboardEventObject;
static KeyboardEventData KeyboardEventObjectData;

static bool InputKeyState[0x30C];

bool KeyIsDown(const unsigned int keyCode) {
	if (keyCode == KEYCODE::KEY_SHIFT)
		return ((GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0);
	if (keyCode == KEYCODE::KEY_CONTROL)
		return ((GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0);
	if (keyCode == KEYCODE::KEY_ALT)
		return ((GetAsyncKeyState(VK_MENU) & 0x8000) != 0);
	return InputKeyState[keyCode];
}
void KeySetDown(const unsigned int keyCode, bool down) { InputKeyState[keyCode] = down; }
void KeyStateReset() { memset(InputKeyState, false, sizeof(InputKeyState)); }

static Event MouseEventObject;
static MouseEventData MouseEventObjectData;

static Event MouseEventScrollObject;
static MouseEventScrollData MouseEventScrollObjectData;

static bool ButtonClickByProgramm;
static bool KeyPressByProgramm;

static RECT ClientRect;
static POINT CursorPoint;
static MousePosition ClientMousePosition;
static ScreenPosition ClientScreenPosition;

static Event LocalChatEventObject;
static LocalChatEventData LocalChatEventObjectData;

static Event CustomEvent;

ScreenPosition* GetScreenPositionFromWar3(float _x, float _y)
{
	ClientScreenPosition.x = (float)((double)_x * ((double)WindowMaxX / wc3_max_x));
	ClientScreenPosition.y = (float)((wc3_max_y - (double)_y) * ((double)WindowMaxY / wc3_max_y));
	return &ClientScreenPosition;
}

ScreenPosition* GetWar3ScreenPosition(float _x, float _y)
{
	ClientScreenPosition.x = (float)((wc3_max_x / (double)WindowMaxX) * (double)_x);
	ClientScreenPosition.y = (float)(wc3_max_y - ((wc3_max_y / (double)WindowMaxY) * (double)_y));
	return &ClientScreenPosition;
}

ScreenPosition* GetWar3ScreenPosition(unsigned int _x, unsigned int _y)
{
	return GetWar3ScreenPosition((float)_x, (float)_y);
}

MousePosition* GetMousePosition() {
	ClientMousePosition.x = GetMousePosX();
	ClientMousePosition.y = GetMousePosY();
	return &ClientMousePosition;
}

static Event ItemClickEventObject;
static ItemClickData ItemClickEventObjectData;

void ItemClick_Report(war3::CEvent* evt) {
	ItemClickEventObject.setId(EVENT_ITEM_CLICK);
	ItemClickEventObjectData.mouseCode =
		reinterpret_cast<war3::CSimpleButtonClickEvent*>(evt)
		->keyFlag;
	war3::CCommandButton* invBar = (war3::CCommandButton*)
		reinterpret_cast<war3::CSimpleButtonClickEvent*>(evt)->baseEvent.object;
	war3::CCommandButtonData* cmdButtonData = invBar->commandButtonData;
	void* pItem = CItemFromHash(&(cmdButtonData->agentHash));
	ItemClickEventObjectData.clickedItem
		= ObjectToHandle(pItem);
	ItemClickEventObjectData.evtObj
		= reinterpret_cast<war3::CSimpleButtonClickEvent*>(evt);
	ItemClickEventObjectData.byProgramm = ButtonClickByProgramm;
	ItemClickEventObject.setData<ItemClickData>(&ItemClickEventObjectData);
	MainDispatcher()->dispatch(&ItemClickEventObject);
}

//�����ť
void GameUIButtonClick(void* button, int mouseCode, bool sendAsProgramm) {
	if (!button) return;
	void* buttonClickFunc = *(*(void***)button + 0x6C / 4);//vtable 0x6C
	if (sendAsProgramm)
		ButtonClickByProgramm = true;
	__asm {
		push mouseCode;
		mov ecx, button;
		call buttonClickFunc;
	}
	if (sendAsProgramm)
		ButtonClickByProgramm = false;
}

//�������:
void GameUIKeyPress(int keyCode, bool down, bool sendAsProgramm) {
	if (!InputObserverObject) return;
	void* keyPressFunc = down ?
		Offset(INPUT_KEY_DOWN) : Offset(INPUT_KEY_UP);
	int code = keyCode;
	if (sendAsProgramm)
		KeyPressByProgramm = true;
	__asm {
		lea ecx, code;
		mov edx, InputObserverObject;
		call keyPressFunc;
	}
	if (sendAsProgramm)
		KeyPressByProgramm = false;
}

void Input_Init() {
	memset(&KeyboardEventObjectData, 0, sizeof(KeyboardEventData));
	//KeyboardEventObject.setData<KeyboardEventData>(&KeyboardEventObjectData);
	//memset( &MouseEventObject, 0, sizeof( MouseEventObject ) );

	LocalChatEventObject.setId(EVENT_LOCAL_CHAT);
	LocalChatEventObject.setData<LocalChatEventData>(&LocalChatEventObjectData);

	InputObserverObject = InputObserverGet();
}

bool GlobalSkipInputSingleShot = false;

bool Input_Update(war3::CEvent* evt) {
	unsigned int evtId = evt->id;
	int keyCode;
	war3::CKeyEvent* keyEvent;

	switch (evtId) {
	case war3::EVENT_KEYDOWN:
		keyEvent = reinterpret_cast<war3::CKeyEvent*>(evt);
		keyCode = keyEvent->keyCode;
		KeySetDown(keyCode, true);


		if (IsInGame()) {
			if (!isChatBoxOn()) {
				// slash������
				if (keyCode == KEYCODE::KEY_OEM_SLASH
					&& !KeyIsDown(KEYCODE::KEY_ALT)
					&& !KeyIsDown(KEYCODE::KEY_SHIFT)
					&& !KeyIsDown(KEYCODE::KEY_CONTROL)
					)
				{
					GameUIKeyPress(KEYCODE::KEY_ENTER, true, true);
					GameUIKeyPress(KEYCODE::KEY_ENTER, false, true);
				}

				KeyboardEventObject.setId(EVENT_KEY_DOWN);
				KeyboardEventObjectData.code = keyCode;
				KeyboardEventObjectData.vkcode = KEYCODE::ToVKeyCode(keyCode);
				KeyboardEventObjectData.alt = KeyIsDown(KEYCODE::KEY_ALT);
				KeyboardEventObjectData.ctrl = KeyIsDown(KEYCODE::KEY_CONTROL);
				KeyboardEventObjectData.shift = KeyIsDown(KEYCODE::KEY_SHIFT);
				KeyboardEventObjectData.byProgramm = KeyPressByProgramm;
				KeyboardEventObjectData.evtObj = (war3::CKeyEvent*)evt;
				KeyboardEventObject.setData<KeyboardEventData>(&KeyboardEventObjectData);
				MainDispatcher()->dispatch(&KeyboardEventObject);
			}
			else {
				if (keyCode == KEYCODE::KEY_ENTER) {
					LocalChatEventObjectData.content = GameUIObjectGet()->chatEditBar->chatEditBox->text;
					LocalChatEventObject.setId(EVENT_LOCAL_CHAT);
					LocalChatEventObject.setData(&LocalChatEventObjectData);
					if (!MainDispatcher()->dispatch(&LocalChatEventObject))
						keyEvent->keyCode = KEYCODE::KEY_ESC;
				}
			}
		}
		break;
	case war3::EVENT_KEYUP:
		keyCode = reinterpret_cast<war3::CKeyEvent*>(evt)->keyCode;
		KeySetDown(keyCode, false);
		if (IsInGame()) {
			if (!isChatBoxOn()) {
				KeyboardEventObject.setId(EVENT_KEY_UP);
				KeyboardEventObjectData.code = keyCode;
				KeyboardEventObjectData.alt = KeyIsDown(KEYCODE::KEY_ALT);
				KeyboardEventObjectData.ctrl = KeyIsDown(KEYCODE::KEY_CONTROL);
				KeyboardEventObjectData.shift = KeyIsDown(KEYCODE::KEY_SHIFT);
				KeyboardEventObjectData.byProgramm = KeyPressByProgramm;
				KeyboardEventObjectData.evtObj = (war3::CKeyEvent*)evt;
				KeyboardEventObject.setData<KeyboardEventData>(&KeyboardEventObjectData);
				MainDispatcher()->dispatch(&KeyboardEventObject);
			}
		}
		break;

	case war3::EVENT_FOCUS:
		if (IsInGame()) {
			KeyStateReset();
			CustomEvent.setId(EVENT_FOCUS);
			MainDispatcher()->dispatch(&CustomEvent);
		}
		break;

		/*
		case war3::EVENT_MOUSE_MOVE:
			if (IsInGame()) {
				//OutputScreen(10, "Mouse move: %.4f, %.4f", MouseX, MouseY);
				MouseEventObject.setId(EVENT_MOUSE_MOVE);
				MouseEventObjectData.mouseCode = 0;
				MouseEventObjectData.x = reinterpret_cast<war3::CControlDragEvent*>(evt)->x;
				MouseEventObjectData.y = reinterpret_cast<war3::CControlDragEvent*>(evt)->y;
				MouseEventObject.setData<MouseEventData>(&MouseEventObjectData);
				MainDispatcher()->dispatch(&MouseEventObject);

			}
			break;
		*/

	case war3::EVENT_MOUSE_DOWN:
		if (IsInGame()) {
			int mouseCode = reinterpret_cast<war3::CMouseEvent*>(evt)->keyCurrent;

			MouseEventObject.setId(EVENT_MOUSE_DOWN);
			MouseEventObjectData.mouseCode = mouseCode;
			MouseEventObjectData.x = reinterpret_cast<war3::CMouseEvent*>(evt)->x;
			MouseEventObjectData.y = reinterpret_cast<war3::CMouseEvent*>(evt)->y;
			MouseEventObjectData.buttonPushed = NULL;
			MouseEventObject.setData<MouseEventData>(&MouseEventObjectData);
			MainDispatcher()->dispatch(&MouseEventObject);

			//��Ϊ�����¼�
			if (mouseCode != MOUSECODE::MOUSE_LEFT && mouseCode != MOUSECODE::MOUSE_RIGHT)
			{
				KeyboardEventObject.setId(EVENT_KEY_DOWN);
				KeyboardEventObjectData.code = KEYCODE::MouseCodeToKeyCode(mouseCode);
				KeyboardEventObjectData.vkcode = -1;
				KeyboardEventObjectData.alt = KeyIsDown(KEYCODE::KEY_ALT);
				KeyboardEventObjectData.ctrl = KeyIsDown(KEYCODE::KEY_CONTROL);
				KeyboardEventObjectData.shift = KeyIsDown(KEYCODE::KEY_SHIFT);
				KeyboardEventObjectData.byProgramm = ButtonClickByProgramm;
				KeyboardEventObjectData.evtObj = (war3::CKeyEvent*)evt;
				KeyboardEventObject.setData<KeyboardEventData>(&KeyboardEventObjectData);
				MainDispatcher()->dispatch(&KeyboardEventObject);
			}
		}
		break;
	case war3::EVENT_MOUSE_UP:
		if (IsInGame()) {
			int mouseCode = reinterpret_cast<war3::CMouseEvent*>(evt)->keyCurrent;

			MouseEventObject.setId(EVENT_MOUSE_UP);
			MouseEventObjectData.mouseCode = reinterpret_cast<war3::CMouseEvent*>(evt)->keyCurrent;
			MouseEventObjectData.x = reinterpret_cast<war3::CMouseEvent*>(evt)->x;
			MouseEventObjectData.y = reinterpret_cast<war3::CMouseEvent*>(evt)->y;
			MouseEventObject.setData<MouseEventData>(&MouseEventObjectData);
			MainDispatcher()->dispatch(&MouseEventObject);

			//��Ϊ�����¼�
			if (mouseCode != MOUSECODE::MOUSE_LEFT && mouseCode != MOUSECODE::MOUSE_RIGHT)
			{
				KeyboardEventObject.setId(EVENT_KEY_UP);
				KeyboardEventObjectData.code = KEYCODE::MouseCodeToKeyCode(mouseCode);
				KeyboardEventObjectData.vkcode = -1;
				KeyboardEventObjectData.alt = KeyIsDown(KEYCODE::KEY_ALT);
				KeyboardEventObjectData.ctrl = KeyIsDown(KEYCODE::KEY_CONTROL);
				KeyboardEventObjectData.shift = KeyIsDown(KEYCODE::KEY_SHIFT);
				KeyboardEventObjectData.byProgramm = ButtonClickByProgramm;
				KeyboardEventObjectData.evtObj = (war3::CKeyEvent*)evt;
				KeyboardEventObject.setData<KeyboardEventData>(&KeyboardEventObjectData);
				MainDispatcher()->dispatch(&KeyboardEventObject);
			}
		}
		break;
	case war3::EVENT_MOUSE_SCROLL:
		if (IsInGame()) {
			bool bWheelUp = reinterpret_cast<war3::CControlWheelEvent*>(evt)->code1 == 0x78;

			MouseEventScrollObject.setId(EVENT_MOUSE_SCROLL);
			MouseEventScrollObjectData.evtObj = reinterpret_cast<war3::CControlWheelEvent*>(evt);
			MouseEventScrollObjectData.up = bWheelUp;
			MouseEventScrollObject.setData<MouseEventScrollData>(&MouseEventScrollObjectData);
			MainDispatcher()->dispatch(&MouseEventScrollObject);

			//������Ϊ�����¼�
			KeyboardEventObject.setId(EVENT_KEY_DOWN);
			KeyboardEventObjectData.code = bWheelUp ? KEYCODE::KEY_MOUSE_SCROLL_UP : KEYCODE::KEY_MOUSE_SCROLL_DOWN;
			KeyboardEventObjectData.vkcode = -1;
			KeyboardEventObjectData.alt = KeyIsDown(KEYCODE::KEY_ALT);
			KeyboardEventObjectData.ctrl = KeyIsDown(KEYCODE::KEY_CONTROL);
			KeyboardEventObjectData.shift = KeyIsDown(KEYCODE::KEY_SHIFT);
			KeyboardEventObjectData.byProgramm = ButtonClickByProgramm;
			KeyboardEventObjectData.evtObj = (war3::CKeyEvent*)evt;
			KeyboardEventObject.setData<KeyboardEventData>(&KeyboardEventObjectData);
			MainDispatcher()->dispatch(&KeyboardEventObject);
		}
		break;
	}

	if (GlobalSkipInputSingleShot)
	{
		GlobalSkipInputSingleShot = false;
		return false;
	}

	return true;
}

bool isChatBoxOn() {
	if (GameUIObjectGet() && GameUIObjectGet()->chatEditBar)
		return GameUIObjectGet()->chatEditBar->visible == 1;
	return false;
}

void* PositionGetButton(float x, float y, bool ignoreTooLarge) {
	war3::CSimpleTop* simpleTop = *(war3::CSimpleTop**)Offset(GLOBAL_SIMPLETOP);
	war3::CLayoutFrame* button = NULL;
	if (simpleTop) {
		for (unsigned int i = 0; i < simpleTop->buttonListArrayCount; i++) {
			button = (war3::CLayoutFrame*)simpleTop->pButtonListArray->pButtonLists[i]->firstButton;
			if (button->borderB > 0//�ų�console����
				&& (!ignoreTooLarge || button->borderU - button->borderB < 0.05f)//�ų��������?
				&& button->borderB <= y
				&& button->borderU >= y
				&& button->borderL <= x
				&& button->borderR >= x
				) {
				//OutputScreen(10, "button found! B=%.5f, U=%.5f, L=%.5f, R=%.5f", button->borderB, button->borderU, button->borderL, button->borderR);
				return button;
			}
		}
	}
	return NULL;
}
war3::CCommandButton* HotkeyGetButton(unsigned int key)
{
	war3::CSimpleTop* simpleTop = *(war3::CSimpleTop**)Offset(GLOBAL_SIMPLETOP);
	war3::CCommandButton* button = NULL;
	if (simpleTop)
	{
		for (unsigned int i = 0; i < simpleTop->buttonListArrayCount; i++)
		{
			button = (war3::CCommandButton*)simpleTop->pButtonListArray->pButtonLists[i]->firstButton;
			//TODO ΪCTrainableButton�����
			//�ж�vtable(CCommandButton)
			if (button && IsCommandButton((war3::CSimpleButton*)button))
			{
				if (button->commandButtonData)
				{
					if (button->commandButtonData->hotkey == key)
					{
						return button;
					}
				}
			}
		}
	}
	return NULL;
}

bool IsCancelPanelOn()
{
	war3::CSimpleTop* simpleTop = *(war3::CSimpleTop**)Offset(GLOBAL_SIMPLETOP);
	war3::CCommandButton* button = NULL;
	if (simpleTop)
	{
		for (unsigned int i = 0; i < simpleTop->buttonListArrayCount; i++)
		{
			button = (war3::CCommandButton*)simpleTop->pButtonListArray->pButtonLists[i]->firstButton;
			if (button && IsCommandButton((war3::CSimpleButton*)button))
			{
				if (button->commandButtonData)
				{
					if (button->commandButtonData->orderId_8 == 0xD000B) //cancel
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}
