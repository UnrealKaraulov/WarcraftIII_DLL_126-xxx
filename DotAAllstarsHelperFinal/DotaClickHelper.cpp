#include "DotaClickHelper.h"
#include "Main.h"
#include <codecvt>
#include "RawImageApi.h"

HWND Warcraft3Window = 0;

WarcraftRealWNDProc WarcraftRealWNDProc_org = NULL;
WarcraftRealWNDProc WarcraftRealWNDProc_ptr;


LPARAM lpF1ScanKeyUP = (LPARAM)(0xC0000001 | (LPARAM)(MapVirtualKey(VK_F1, 0) << 16));
LPARAM lpF1ScanKeyDOWN = (LPARAM)(0x00000001 | (LPARAM)(MapVirtualKey(VK_F1, 0) << 16));

LPARAM lpAltScanKeyUP = (LPARAM)(0xC0000001 | (LPARAM)(MapVirtualKey(VK_MENU, 0) << 16));
LPARAM lpAltScanKeyDOWN = (LPARAM)(0x00000001 | (LPARAM)(MapVirtualKey(VK_MENU, 0) << 16));

LPARAM lpCtrlScanKeyUP = (LPARAM)(0xC0000001 | (LPARAM)(MapVirtualKey(VK_CONTROL, 0) << 16));
LPARAM lpCtrlScanKeyDOWN = (LPARAM)(0x00000001 | (LPARAM)(MapVirtualKey(VK_CONTROL, 0) << 16));

bool EmulateKeyInputForHWND = false;


int ShiftPressed = 0;
unsigned long SkipSingleShift = 0;
unsigned long SingleShift = 0;

bool SkipAllMessages = false;


void PressKeyboard(int VK)
{
	bool PressedKey = false;
	INPUT Input = { 0 };
	Input.type = INPUT_KEYBOARD;
	Input.ki.wScan = (WORD)MapVirtualKey((unsigned int)VK, 0);
	Input.ki.wVk = (WORD)VK;
	if (IsKeyPressed(VK))
	{
		PressedKey = true;
		Input.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &Input, sizeof(INPUT));
	}

	Input.ki.dwFlags = 0;
	SendInput(1, &Input, sizeof(INPUT));
	if (!PressedKey)
	{
		Input.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &Input, sizeof(INPUT));
	}
}

unsigned long LastKeyPressedTime = 0;
unsigned long LastKeyPressedKey = 0;

void __fastcall PressCancel(int data)
{
	__asm
	{
		push data;
		mov ecx, GameDll;
		add ecx, 0xACE9EC;
		mov edi, GameDll;
		add edi, 0x3747E0;
		call edi;
	}
}

int IsCursorSelectTarget()
{
	unsigned char* pOffset1 = GetGlobalClassAddr();
	if (pOffset1 && *(int*)(pOffset1 + 0x1BC) == 1)
	{
		/*char tmp[ 100 ];
		sprintf_s( tmp, 100, "%X", pOffset1 );
		MessageBoxA( 0, tmp, tmp, 0 );*/
		return true;
	}
	return false;
}

int GetCursorSkillID()
{
	unsigned char* pOffset1 = GetGlobalClassAddr();
	if (pOffset1 && (pOffset1 = *(unsigned char**)(pOffset1 + 0x1B4)))
	{
		return *(int*)(pOffset1 + 0xC);
	}
	return 0;
}

int GetCursorOrder()
{
	unsigned char* pOffset1 = GetGlobalClassAddr();
	if (pOffset1 && (pOffset1 = *(unsigned char**)(pOffset1 + 0x1B4)))
	{
		return *(int*)(pOffset1 + 0x10);
	}
	return 0;
}

std::vector<int> doubleclickSkillIDs;

int __stdcall AddDoubleClickSkillID(int skillID)
{
	if (DEBUG_FULL)
		std::cout << __func__ << std::endl;
	/*char addedid[ 100 ];
	sprintf_s( addedid, "Added new id:%i", skillID );
	MessageBoxA( 0, addedid, "", 0 );*/
	if (skillID == 0 && !doubleclickSkillIDs.empty())
	{
		//MessageBoxA( 0, "ERROR! IDS CLEARED", "", 0 );
		doubleclickSkillIDs.clear();
	}
	else
		doubleclickSkillIDs.push_back(skillID);

	return skillID;
}

float HeroPortX = 0.318f;
float HeroPortY = 0.888f;

bool ScreenToClientReplace = false;

POINT ScreenToClientReplacedPoint;


bool ClientToScreenReplace = false;

POINT ClientToScreenReplacedPoint;

LPARAM oldlParam = 0;

bool BLOCKMOUSEMOVING = false;

float HeroFrameX = 0.256f;
float HeroFrameY = 0.0666f;

float HeroFrameX_old = 0.256f;
float HeroFrameY_old = 0.0666f;



void SetHeroFrameXY()
{
	HeroFrameX_old = *(float*)(GameFrameAtMouseStructOffset + 0x14);
	*(float*)(GameFrameAtMouseStructOffset + 0x14) = HeroFrameX;
	HeroFrameY_old = *(float*)(GameFrameAtMouseStructOffset + 0x18);
	*(float*)(GameFrameAtMouseStructOffset + 0x18) = HeroFrameY;
}

void SetHeroFrameXY_old()
{
	if (*(float*)(GameFrameAtMouseStructOffset + 0x14) == HeroFrameX)
	{
		*(float*)(GameFrameAtMouseStructOffset + 0x14) = HeroFrameX_old;
		*(float*)(GameFrameAtMouseStructOffset + 0x18) = HeroFrameY_old;
	}
}



void MouseClickX(int toX, int toY)
{
	Sleep(5);
	POINT point;
	GetCursorPos(&point);
	PostMessage(Warcraft3Window, WM_LBUTTONDOWN, MK_LBUTTON, MAKELONG(point.x, point.y));
	PostMessage(Warcraft3Window, WM_LBUTTONUP, MK_LBUTTON, MAKELONG(point.x, point.y));

	/*
		POINT cursorPos;
		GetCursorPos( &cursorPos );

		double dx = toX * ( 65535.0f / DesktopScreen_Width );
		double dy = toY * ( 65535.0f / DesktopScreen_Height );
		INPUT Input = { 0 };

		Input.type = INPUT_MOUSE;
		Input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
		Input.mi.dx = LONG( dx );
		Input.mi.dy = LONG( dy );
		SendInput( 1, &Input, sizeof( INPUT ) );
		SetHeroFrameXY( );

		SendInput( 1, &Input, sizeof( INPUT ) );
		SetHeroFrameXY( );

		Input.type = INPUT_MOUSE;
		Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
		SendInput( 1, &Input, sizeof( INPUT ) );
		SetHeroFrameXY( );

		Input.type = INPUT_MOUSE;
		Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
		SendInput( 1, &Input, sizeof( INPUT ) );
		SetHeroFrameXY( );

		dx = cursorPos.x*( 65535.0f / DesktopScreen_Width );
		dy = cursorPos.y*( 65535.0f / DesktopScreen_Height );

		Input.type = INPUT_MOUSE;
		Input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
		Input.mi.dx = LONG( dx );
		Input.mi.dy = LONG( dy );
		SendInput( 1, &Input, sizeof( INPUT ) );
		SetHeroFrameXY( );*/
}




unsigned long __stdcall ThreadTest(void* lpp)
{
	POINT* p = (POINT*)lpp;
	SkipAllMessages = true;
	MouseClickX(p->x, p->y);
	SkipAllMessages = false;
	delete p;
	return 0;
}

void MouseClick(int toX, int toY)
{
	POINT* ClickPoint = new POINT();
	ClickPoint->x = toX;
	ClickPoint->y = toY;
	CloseHandle(CreateThread(0, 0, ThreadTest, ClickPoint, 0, 0));
}

void JustClickMouse()
{
	bool ButtonDown = false;
	if (IsKeyPressed(VK_LBUTTON))
	{
		ButtonDown = true;
		SendMessage(Warcraft3Window, WM_LBUTTONUP, 0, oldlParam);
	}

	INPUT Input = { 0 };
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput(1, &Input, sizeof(INPUT));

	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &Input, sizeof(INPUT));

}
int PressMouseAtSelectedHero(int IsItem)
{
	int errorvalue = 0;
	if (!IsCursorSelectTarget())
	{
		errorvalue = 1;
		//PrintText( "ERROR 1" );
	}
	if (GetCursorOrder() == 0xD000F ||
		GetCursorOrder() == 0xD0012 ||
		GetCursorOrder() == 0xD0016)
	{
		errorvalue = 2;
		//	PrintText( "ERROR 2" );
	}

	if (IsCursorSelectTarget() &&
		GetCursorOrder() != 0xD000F &&
		GetCursorOrder() != 0xD0012 &&
		GetCursorOrder() != 0xD0016)
	{
		if ( /*!IsItem || */doubleclickSkillIDs.empty() ||
			std::find(doubleclickSkillIDs.begin(), doubleclickSkillIDs.end(), GetCursorSkillID()) != doubleclickSkillIDs.end()
			)
		{
			unsigned char* PortraitButtonAddr = GetGlobalClassAddr();
			if (PortraitButtonAddr)
			{
				PortraitButtonAddr = *(unsigned char**)(PortraitButtonAddr + 0x3F4);
			}
			if (PortraitButtonAddr && !IsItem)
			{
				//PrintText( "NEED CLICK PORTRAIT" );
				Wc3ControlClickButton_org(PortraitButtonAddr, 1);
			}
			else
			{
				errorvalue = 4;
				//PrintText( "ERROR 4" );
			}

			/*int ButtonDown = false;
			if ( IsKeyPressed( VK_LBUTTON ) )
			{
				ButtonDown = true;
				SendMessage( Warcraft3Window, WM_LBUTTONUP, 0, oldlParam );
			}

			int x = ( int )( *GetWindowXoffset * HeroPortX );
			int y = ( int )( *GetWindowYoffset * HeroPortY );

			POINT cursorhwnd;
			GetCursorPos( &cursorhwnd );
			ScreenToClient( Warcraft3Window, &cursorhwnd );
			POINT cursor;
			GetCursorPos( &cursor );

			x = x - cursorhwnd.x;
			y = y - cursorhwnd.y;

			cursor.x = cursor.x + x;
			cursor.y = cursor.y + y;
			//( toXX, toYY );

			MouseClick( cursor.x, cursor.y );*/
		}
		else
		{
			errorvalue = 3;
			//PrintText( ( "ERROR 3:" + to_string( GetCursorSkillID( ) ) ).c_str( ) );
		}

	}

	return errorvalue;
}




unsigned long LastPressedKeysTime[1024];


std::vector<int> RegisteredKeyCodes;
std::vector<int> BlockedKeyCodes;

std::vector<KeyActionStruct> KeyActionList;




struct mMessage
{
	unsigned int Msg;
	unsigned int wParam;
};

std::vector<mMessage> SkipMessagesList;


WPARAM LastShift = 0;

LPARAM MakeLParamVK(unsigned int VK, int up, int Extended = false)
{
	if (up) return (LPARAM)(0xC0000001 | ((unsigned int)Extended << 24) | (LPARAM)(MapVirtualKey(VK, 0) << 16));
	else return (LPARAM)(0x00000001 | ((unsigned int)Extended << 24) | (LPARAM)(MapVirtualKey(VK, 0) << 16));
}


int __stdcall TriggerRegisterPlayerKeyboardEvent(int KeyCode)
{
	if (DEBUG_FULL)
		std::cout << __func__ << std::endl;
	if (!KeyCode)
	{
		if (!RegisteredKeyCodes.empty())
			RegisteredKeyCodes.clear();
		return 0;
	}


	RegisteredKeyCodes.push_back(KeyCode);
	return 0;
}

int bTriggerRegisterPlayerKeyboardBlock = true;

void __stdcall TriggerRegisterPlayerKeyboardBlock(int enabled)
{
	bTriggerRegisterPlayerKeyboardBlock = enabled;
}

int __stdcall BlockKeyAction(int KeyCode)
{
	if (DEBUG_FULL)
		std::cout << __func__ << std::endl;
	if (!KeyCode)
	{
		if (!BlockedKeyCodes.empty())
			BlockedKeyCodes.clear();
		return 0;
	}
	BlockedKeyCodes.push_back(KeyCode);

	return 0;
}

int GetAltBtnID(int btnID)
{
	switch (btnID)
	{
	case 2:
		return 0;
	case 5:
		return 3;
	case 8:
		return 6;
	case 11:
		return 9;
	case 4:
		return 1;
	case 7:
		return 4;
	}

	return -1;
}

int EnabledReplaceHotkeyFlag = true;

void __stdcall EnableReplaceHotkeyFlag(int enabled)
{
	EnabledReplaceHotkeyFlag = enabled;
}

std::vector<KeySelectActionStruct> KeySelectActionList;

int __stdcall AddKeySelectAction(unsigned int KeyCode, int GroupHandle)
{
	if (DEBUG_FULL)
		std::cout << __func__ << std::endl;
	if (!KeyCode)
	{
		if (!KeySelectActionList.empty())
			KeySelectActionList.clear();
		return 0;
	}

	KeySelectActionStruct tmpstr;
	tmpstr.VK = KeyCode & 0xFF;
	tmpstr.IsAlt = (KeyCode & 0x10000) > 0;
	tmpstr.IsCtrl = (KeyCode & 0x20000) > 0;
	tmpstr.IsShift = (KeyCode & 0x40000) > 0;
	tmpstr.GroupHandle = GroupHandle;
	tmpstr.units = GetUnitsFromGroup(GroupHandle);
	//reverse( tmpstr.units.begin( ), tmpstr.units.end( ) );


	if (!EnabledReplaceHotkeyFlag || (KeyCode & 0x100000) > 0)
	{
		for (KeySelectActionStruct& curstr : KeySelectActionList)
		{
			if (curstr.VK == tmpstr.VK)
			{
				if (curstr.GroupHandle == tmpstr.GroupHandle ||
					(((!curstr.IsAlt && !curstr.IsCtrl && !curstr.IsShift)
						&&
						(!tmpstr.IsAlt && !tmpstr.IsCtrl && !tmpstr.IsShift))

						|| (curstr.IsAlt && tmpstr.IsAlt)

						|| (curstr.IsCtrl && tmpstr.IsCtrl)

						|| (curstr.IsShift && tmpstr.IsShift)))
				{
					if (SetInfoObjDebugVal)
					{
						PrintText("Replace old selection hotkey.");
					}

					curstr = tmpstr;
					return 0;
				}
			}
		}
	}
	if (SetInfoObjDebugVal)
	{
		PrintText("Add new selection hotkey.");
	}

	KeySelectActionList.push_back(tmpstr);
	return 0;
}


std::vector<KeyChatActionStruct> KeyChatActionList;

int __stdcall AddKeyChatAction(unsigned int KeyCode, const char* str, int SendToAll)
{
	if (DEBUG_FULL)
		std::cout << __func__ << std::endl;
	if (!KeyCode)
	{
		if (!KeyChatActionList.empty())
			KeyChatActionList.clear();
		return 0;
	}

	KeyChatActionStruct tmpstr;
	tmpstr.VK = KeyCode & 0xFF;
	tmpstr.IsAlt = (KeyCode & 0x10000) > 0;
	tmpstr.IsCtrl = (KeyCode & 0x20000) > 0;
	tmpstr.IsShift = (KeyCode & 0x40000) > 0;
	tmpstr.SendToAll = SendToAll;
	tmpstr.Message = str && strlen(str) < 127 ? str : "Bad message length";

	if (!EnabledReplaceHotkeyFlag || (KeyCode & 0x100000) > 0)
	{
		for (KeyChatActionStruct& curstr : KeyChatActionList)
		{
			if (curstr.VK == tmpstr.VK)
			{
				if (((!curstr.IsAlt && !curstr.IsCtrl && !curstr.IsShift) &&
					(!tmpstr.IsAlt && !tmpstr.IsCtrl && !tmpstr.IsShift))
					|| (curstr.IsAlt && tmpstr.IsAlt)
					|| (curstr.IsCtrl && tmpstr.IsCtrl)
					|| (curstr.IsShift && tmpstr.IsShift)
					)
				{
					if (SetInfoObjDebugVal)
					{
						PrintText("Replace old chat hotkey.");
					}
					curstr = tmpstr;
					return 0;
				}
			}
		}
	}
	if (SetInfoObjDebugVal)
	{
		PrintText("Add new chat hotkey.");
	}
	KeyChatActionList.push_back(tmpstr);

	return 0;
}

std::vector<KeyCalbackActionStruct> KeyCalbackActionList;

int __stdcall AddKeyCalbackAction(unsigned int KeyCode, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8)
{
	if (DEBUG_FULL)
		std::cout << __func__ << std::endl;
	if (!KeyCode)
	{
		if (!KeyCalbackActionList.empty())
			KeyCalbackActionList.clear();
		return 0;
	}

	KeyCalbackActionStruct tmpstr;
	tmpstr.VK = KeyCode & 0xFF;
	tmpstr.IsAlt = (KeyCode & 0x10000) > 0;
	tmpstr.IsCtrl = (KeyCode & 0x20000) > 0;
	tmpstr.IsShift = (KeyCode & 0x40000) > 0;
	// save args safe
	tmpstr.args[0] = arg2;
	tmpstr.args[1] = arg3;
	tmpstr.args[2] = arg4;
	tmpstr.args[3] = arg5;
	tmpstr.args[4] = arg6;
	tmpstr.args[5] = arg7;
	tmpstr.args[6] = arg8;

	if (!EnabledReplaceHotkeyFlag || (KeyCode & 0x100000) > 0)
	{
		for (KeyCalbackActionStruct& curstr : KeyCalbackActionList)
		{
			if (curstr.VK == tmpstr.VK)
			{
				if (
					((!curstr.IsAlt && !curstr.IsCtrl && !curstr.IsShift) &&
						(!tmpstr.IsAlt && !tmpstr.IsCtrl && !tmpstr.IsShift))


					|| (curstr.IsAlt && tmpstr.IsAlt)
					|| (curstr.IsCtrl && tmpstr.IsCtrl)
					|| (curstr.IsShift && tmpstr.IsShift)
					)
				{
					if (SetInfoObjDebugVal)
					{
						PrintText("Replace old callback hotkey.");
					}
					curstr = tmpstr;
					return 0;
				}
			}
		}
	}

	if (SetInfoObjDebugVal)
	{
		char bufka[200];
		sprintf_s(bufka, "Add new callback hotkey. VK %X/ALT %X/CTRL %X/SHIFT %X", tmpstr.VK, tmpstr.IsAlt, tmpstr.IsCtrl, tmpstr.IsShift);

		PrintText(bufka);
	}

	KeyCalbackActionList.push_back(tmpstr);

	return 0;
}


int __stdcall AddKeyButtonAction(unsigned int KeyCode, int btnID, int IsSkill)
{
	if (DEBUG_FULL)
		std::cout << __func__ << std::endl;
	if (!KeyCode)
	{
		if (!KeyActionList.empty())
			KeyActionList.clear();
		return 0;
	}

	KeyActionStruct tmpstr;
	tmpstr.VK = KeyCode & 0xFF;
	tmpstr.btnID = btnID;
	tmpstr.IsSkill = IsSkill;

	if (IsSkill)
		tmpstr.altbtnID = (GetAltBtnID(btnID));
	else
		tmpstr.altbtnID = 0;

	tmpstr.IsAlt = (KeyCode & 0x10000) > 0;
	tmpstr.IsCtrl = (KeyCode & 0x20000) > 0;
	tmpstr.IsShift = (KeyCode & 0x40000) > 0;
	tmpstr.IsRightClick = (KeyCode & 0x80000) > 0;
	tmpstr.IsQuickCast = (KeyCode & 0x200000) > 0;

	if (!EnabledReplaceHotkeyFlag || (KeyCode & 0x100000) > 0)
	{
		for (KeyActionStruct& curstr : KeyActionList)
		{
			if (curstr.btnID == tmpstr.btnID)
			{
				if (((!curstr.IsAlt && !curstr.IsCtrl && !curstr.IsShift) &&
					(!tmpstr.IsAlt && !tmpstr.IsCtrl && !tmpstr.IsShift))
					|| (curstr.IsAlt && tmpstr.IsAlt)
					|| (curstr.IsCtrl && tmpstr.IsCtrl)
					|| (curstr.IsShift && tmpstr.IsShift)
					)
				{
					if (SetInfoObjDebugVal)
					{
						PrintText("Replace hotkey");
					}
					curstr = tmpstr;
					return 0;
				}
			}
		}
	}
	if (SetInfoObjDebugVal)
	{
		char debugtext[512];
		sprintf_s(debugtext, "%s:%X %s:%s %s:%s %s:%s %s:%s %s:%s",
			"added new hotkey", KeyCode,
			"IsAlt", GetBoolStr(tmpstr.IsAlt),
			"IsCtrl", GetBoolStr(tmpstr.IsCtrl),
			"IsShift", GetBoolStr(tmpstr.IsShift),
			"IsRightClick", GetBoolStr(tmpstr.IsRightClick),
			"IsQuickCast", GetBoolStr(tmpstr.IsQuickCast));
		PrintText(debugtext);

	}
	KeyActionList.push_back(tmpstr);

	return 0;
}




int IsNULLButtonFound(unsigned char* pButton)
{
	if (pButton && *(int*)(pButton) > 0)
	{
		if (*(int*)(pButton + 0x190) != 0 && *(int*)(*(unsigned char**)(pButton + 0x190) + 4) == 0)
			return true;
	}
	return false;
}

// | 0 | 3 | 6 | 9  |
// | 1 | 4 | 7 | 10 | 
// | 2 | 5 | 8 | 11 |

#define flagsOffset 0x138
#define sizeOfCommandButtonObj 0x1c0

unsigned char* __stdcall GetSkillPanelButton(int idx)
{

	if (GetSelectedUnit(GetLocalPlayerId()))
	{
		unsigned char* pclass = GetGlobalClassAddr();
		if (pclass)
		{
			unsigned char* pGamePlayerPanelSkills = *(unsigned char**)(pclass + 0x3c8);
			if (pGamePlayerPanelSkills)
			{
				unsigned char* topLeftCommandButton = *(unsigned char**)(pGamePlayerPanelSkills + 0x154);
				if (topLeftCommandButton)
				{
					topLeftCommandButton = **(unsigned char***)(topLeftCommandButton + 0x8);
					if (topLeftCommandButton)
						return topLeftCommandButton + sizeOfCommandButtonObj * idx;
				}
			}
		}
	}
	return 0;
}

// | 0 | 1
// | 2 | 3
// | 4 | 5

unsigned char* __stdcall GetItemPanelButton(int idx)
{
	if (GetSelectedUnit(GetLocalPlayerId()))
	{

		unsigned char* pclass = GetGlobalClassAddr();
		if (pclass)
		{
			unsigned char* pGamePlayerPanelItems = *(unsigned char**)(pclass + 0x3c4);
			if (pGamePlayerPanelItems)
			{
				unsigned char* topLeftCommandButton = *(unsigned char**)(pGamePlayerPanelItems + 0x148);
				if (topLeftCommandButton)
				{
					topLeftCommandButton = *(unsigned char**)(topLeftCommandButton + 0x130);
					if (topLeftCommandButton)
					{
						topLeftCommandButton = *(unsigned char**)(topLeftCommandButton + 0x4);
						if (topLeftCommandButton)
						{
							return topLeftCommandButton + sizeOfCommandButtonObj * idx;
						}
					}
				}
			}
		}
	}
	return 0;
}


unsigned char* GetHeroButton(int idx)
{
	unsigned char* pclass = GetGlobalClassAddr();
	if (pclass)
	{
		unsigned char* pGamePlayerHeroBtn = *(unsigned char**)(pclass + 0x3c8);
		if (pGamePlayerHeroBtn)
		{
			pGamePlayerHeroBtn = *(unsigned char**)(pGamePlayerHeroBtn + 0x40);
			if (pGamePlayerHeroBtn)
			{
				pGamePlayerHeroBtn = *(unsigned char**)(pGamePlayerHeroBtn + 0x20);
				return pGamePlayerHeroBtn;
			}
		}
	}
	return 0;
}


c_SimpleButtonClickEvent SimpleButtonClickEvent_org;
c_SimpleButtonClickEvent SimpleButtonClickEvent_ptr;

unsigned char* CommandButtonVtable = 0;

int IsCommandButton(unsigned char* addr)
{
	if (addr)
	{
		if (CommandButtonVtable)
		{
			return *(unsigned char**)addr == CommandButtonVtable;
		}

	}
	return false;
}

std::vector<ClickPortrainForId> ClickPortrainForIdList;

int __stdcall AddClickPortrainForId(int abilid, int keycode)
{
	if (DEBUG_FULL)
		std::cout << __func__ << std::endl;
	if (abilid == 0 || keycode == 0)
	{
		ClickPortrainForIdList.clear();
		return false;
	}

	ClickPortrainForId tmpClickPortrainForId = ClickPortrainForId();
	tmpClickPortrainForId.abilid = abilid;
	tmpClickPortrainForId.keycode = keycode;
	tmpClickPortrainForId.checkforcd = false;

	ClickPortrainForIdList.push_back(tmpClickPortrainForId);

	return true;
}

int __stdcall AddClickPortrainForIdEx(int abilid, int keycode, int checkforcd)
{
	if (DEBUG_FULL)
		std::cout << __func__ << std::endl;
	if (abilid == 0 || keycode == 0)
	{
		ClickPortrainForIdList.clear();
		return false;
	}

	ClickPortrainForId tmpClickPortrainForId = ClickPortrainForId();
	tmpClickPortrainForId.abilid = abilid;
	tmpClickPortrainForId.keycode = keycode;
	tmpClickPortrainForId.checkforcd = checkforcd;

	ClickPortrainForIdList.push_back(tmpClickPortrainForId);

	return true;
}

int CheckBtnForClickPortrain(unsigned char* pButton)
{
	if (pButton && IsCommandButton(pButton))
	{
		//PrintText( "SimpleButton IsCommandButton" );
		unsigned char* CommandButtonData = *(unsigned char**)(pButton + 0x190);
		if (CommandButtonData)
		{
			//	PrintText( "Click command button." );
			unsigned char* pAbil = *(unsigned char**)(CommandButtonData + 0x6D4);
			if (pAbil)
			{
				//	PrintText( "Abil found." );
				int pAbilId = *(int*)(pAbil + 0x34);
				if (pAbilId)
				{
					//	PrintText( "Abil id found." );
					for (auto & tmpClick : ClickPortrainForIdList)
					{
						if (pAbilId == tmpClick.abilid && !IsKeyPressed(tmpClick.keycode))
						{
							if (tmpClick.checkforcd)
							{
								if ((*(unsigned int*)(pAbil + 0x20) & 0x200) > 0)
									return 2;
							}
							//		PrintText( "OK! Need click!" );
							return 1;
						}
						else
						{
							//	PrintText( "NO Need click!" );
						}
					}
				}
			}
		}
	}
	return 0;
}

unsigned long LatestButtonClickTime = 0;

int __fastcall SimpleButtonClickEvent_my(unsigned char* pButton, unsigned char* unused, int ClickEventType)
{

	if (ClickEventType == 1 && CheckBtnForClickPortrain(pButton))
	{
		//PrintText( "Abil id found in list." );
		int retval = SimpleButtonClickEvent_ptr(pButton, unused, ClickEventType);
		unsigned char* PortraitButtonAddr = GetGlobalClassAddr();
		if (PortraitButtonAddr)
		{
			PortraitButtonAddr = *(unsigned char**)(PortraitButtonAddr + 0x3F4);
		}
		if (PortraitButtonAddr)
		{
			//PrintText( "Click to portrain." );
			if (Wc3ControlClickButton_org(PortraitButtonAddr, 1))
			{
				LatestButtonClickTime = GetTickCount();
			}
		}
		return retval;
	}

	bool CheckDoubleOk = false;

	if (SetInfoObjDebugVal)
	{
		PrintText("Click SimpleButton.");
	}

	//
	if (IsCommandButton(pButton))
	{
		if (SetInfoObjDebugVal)
		{
			PrintText("SimpleButton IsCommandButton");
		}

		//
		unsigned char* CommandButtonData = *(unsigned char**)(pButton + 0x190);
		if (CommandButtonData)
		{
			if (SetInfoObjDebugVal)
			{
				PrintText("Click command button.");
			}
			//
			unsigned char* pAbil = *(unsigned char**)(CommandButtonData + 0x6D4);
			if (pAbil)
			{
				if (SetInfoObjDebugVal)
				{
					PrintText("Abil found.");
				}
				//
				int pAbilId = *(int*)(pAbil + 0x34);
				if (pAbilId)
				{
					if (doubleclickSkillIDs.empty() ||
						std::find(doubleclickSkillIDs.begin(), doubleclickSkillIDs.end(), pAbilId) != doubleclickSkillIDs.end())
					{
						if (SetInfoObjDebugVal)
						{
							PrintText("Abilid found!!!!");
						}
						CheckDoubleOk = true;
					}
				}
			}
		}
	}

	int retval = SimpleButtonClickEvent_ptr(pButton, unused, ClickEventType);
	if (retval && CheckDoubleOk)
	{
		if (SetInfoObjDebugVal)
		{
			PrintText("Button click success");
		}
		LatestButtonClickTime = GetTickCount();
	}
	return retval;
}


int __stdcall SimpleButtonClick(unsigned char* simplebtnaddr, int LeftMouse)
{
	if (DEBUG_FULL)
		std::cout << __func__ << std::endl;
	return SimpleButtonClickEvent_org(simplebtnaddr, simplebtnaddr, LeftMouse ? 1 : 4);
}

int PressSkillPanelButton(int idx, int RightClick)
{

	unsigned char* button = GetSkillPanelButton(idx);
	if (button && IsCommandButton(button))
	{
		unsigned int oldflag = *(unsigned int*)(button + flagsOffset);
		if (!(oldflag & 2))
			*(unsigned int*)(button + flagsOffset) = oldflag | 2;
		int retval = SimpleButtonClickEvent_org(button, 0, RightClick ? 4 : 1);
		*(unsigned int*)(button + flagsOffset) = oldflag;
		return retval;
	}
	return 0;
}

int PressItemPanelButton(int idx, int RightClick)
{

	unsigned char* button = GetItemPanelButton(idx);
	if (button && IsCommandButton(button))
	{
		unsigned int oldflag = *(unsigned int*)(button + flagsOffset);
		if (!(oldflag & 2))
			*(unsigned int*)(button + flagsOffset) = oldflag | 2;
		int retval = SimpleButtonClickEvent_org(button, 0, RightClick ? 4 : 1);
		*(unsigned int*)(button + flagsOffset) = oldflag;
		return retval;
	}
	return 0;
}

int PressHeroPanelButton(int idx, int RightClick)
{

	unsigned char* button = GetHeroButton(idx);
	if (button && IsCommandButton(button))
	{
		unsigned int oldflag = *(unsigned int*)(button + flagsOffset);
		if (!(oldflag & 2))
			*(unsigned int*)(button + flagsOffset) = oldflag | 2;
		int retval = SimpleButtonClickEvent_org(button, 0, RightClick ? 4 : 1);
		*(unsigned int*)(button + flagsOffset) = oldflag;
		return retval;
	}
	return 0;
}

int IsMouseOverWindow(RECT pwi, POINT cursorPos)
{
	return PtInRect(&pwi, cursorPos);
}

std::vector<unsigned char> SendKeyEvent;

auto t_start = std::chrono::high_resolution_clock::now();

bool LOCK_MOUSE_IN_WINDOW = false;

int __stdcall LockMouseInWindow(int enable)
{
	if (DEBUG_FULL)
		std::cout << __func__ << std::endl;
	LOCK_MOUSE_IN_WINDOW = enable;

	if (!LOCK_MOUSE_IN_WINDOW)
		ClipCursor(0);

	return enable;
}


bool BlockKeyboardAndMouseWhenTeleport = false;

int __stdcall TeleportHelper(int enabled)
{
	if (DEBUG_FULL)
		std::cout << __func__ << std::endl;
	BlockKeyboardAndMouseWhenTeleport = enabled;
	return enabled;
}

std::vector<int> WhiteListForTeleport;


bool TeleportShiftPress = false;

int __stdcall TeleportShiftKey(int enabled)
{
	if (DEBUG_FULL)
		std::cout << __func__ << std::endl;
	TeleportShiftPress = enabled;
	return enabled;
}

int __stdcall TeleportWhiteListKey(int VK)
{
	if (DEBUG_FULL)
		std::cout << __func__ << std::endl;
	if (VK == 0 && !WhiteListForTeleport.empty())
		WhiteListForTeleport.clear();
	WhiteListForTeleport.push_back(VK);
	return VK;
}

bool ShopHelperEnabled = false;

int __stdcall ShopHelper(int enable)
{
	if (DEBUG_FULL)
		std::cout << __func__ << std::endl;
	ShopHelperEnabled = enable;
	return enable;
}
bool rawimage_skipmouseevent = true;

int __stdcall RawImage_SkipMouseClick(int enabled)
{
	if (DEBUG_FULL)
		std::cout << __func__ << std::endl;
	rawimage_skipmouseevent = enabled;
	return rawimage_skipmouseevent;
}

bool AutoSelectHero = false;

int __stdcall SetAutoSelectHero(int enabled)
{
	if (DEBUG_FULL)
		std::cout << __func__ << std::endl;
	AutoSelectHero = true;
	return AutoSelectHero;
}



safevector<DelayedPress> DelayedPressList;


void DelayedPressList_pushback(DelayedPress& d)
{
	for (unsigned int i = 0; i < DelayedPressList.size(); i++)
	{
		if (DelayedPressList[i].IsNull())
		{
			DelayedPressList[i] = d;
			DelayedPressList[i].ISNULL = false;

			return;
		}
	}
	DelayedPressList.push_back(d);
}

int DisableTargetCurcorWORK = 0;
unsigned long latDisableTargetCurcor = 0;

void  DisableTargetCurcor()
{
	if (DisableTargetCurcorWORK <= 0)
		return;

	if (GetTickCount() - latDisableTargetCurcor > 25)
	{
		latDisableTargetCurcor = GetTickCount();
		DisableTargetCurcorWORK--;
		if (SetInfoObjDebugVal)
		{
			PrintText("w1");
		}
		if (IsCursorSelectTarget())
		{
			if (SetInfoObjDebugVal)
			{
				PrintText("w2");
			}
			POINT cursorhwnd;
			GetCursorPos(&cursorhwnd);
			ScreenToClient(Warcraft3Window, &cursorhwnd);

			WarcraftRealWNDProc_ptr(Warcraft3Window, WM_RBUTTONDOWN, MK_RBUTTON, MAKELPARAM(cursorhwnd.x, cursorhwnd.y));
			WarcraftRealWNDProc_ptr(Warcraft3Window, WM_RBUTTONUP, 0, MAKELPARAM(cursorhwnd.x, cursorhwnd.y));
			DisableTargetCurcorWORK = 0;
			//__asm
			//{
			//	mov ecx, GameDll;
			//	add ecx, 0xAB4F80;
			//	mov ecx, [ ecx ];
			//	mov edi, GameDll;
			//	add edi, 0x2FB920; //  clear cursor
			//	call edi;
			//}
		}
	}

}


void PressKeyWithDelay_timed( )
{
	if (IsGame() && *IsWindowActive)
	{
		for (unsigned int i = 0; i < DelayedPressList.size(); i++)
		{
			if (DelayedPressList[i].IsNull())
				continue;

			if (DelayedPressList[i].TimeOut >= 20)
			{
				DelayedPressList[i].TimeOut -= 20;
			}
			else if (DelayedPressList[i].TimeOut > 0)
			{
				DelayedPressList[i].TimeOut = 0;
			}
			else
			{
				if (!DelayedPressList[i].IsCustom)
				{
					if (DelayedPressList[i].NeedPressMsg == 0)
					{
						WarcraftRealWNDProc_ptr(Warcraft3Window, WM_KEYDOWN, DelayedPressList[i].NeedPresswParam, DelayedPressList[i].NeedPresslParam);
						WarcraftRealWNDProc_ptr(Warcraft3Window, WM_KEYUP, DelayedPressList[i].NeedPresswParam, (LPARAM)(0xC0000000 | DelayedPressList[i].NeedPresslParam));
					}
					else
					{
						WarcraftRealWNDProc_ptr(Warcraft3Window, DelayedPressList[i].NeedPressMsg, DelayedPressList[i].NeedPresswParam, DelayedPressList[i].NeedPresslParam);
					}

				}
				else
				{

					for (KeyActionStruct& keyAction : KeyActionList)
					{
						if (keyAction.VK == (int)DelayedPressList[i].NeedPresswParam && IsGameFrameActive())
						{

							if ((!keyAction.IsAlt && !keyAction.IsCtrl && !keyAction.IsShift)
								|| (keyAction.IsAlt && DelayedPressList[i].IsAlt)
								|| (keyAction.IsCtrl && DelayedPressList[i].IsCtrl)
								|| (keyAction.IsShift && DelayedPressList[i].IsCustom)
								)
							{
								int selectedunits = GetSelectedUnitCountBigger(GetLocalPlayerId());
								int itempressed = keyAction.IsSkill ? 0 : 1;

								if (itempressed || (keyAction.IsSkill && !IsCursorSelectTarget()))
								{
									unsigned char* selectedunit = GetSelectedUnit(GetLocalPlayerId());
									if (selectedunit && selectedunits > 0)
									{
										int unitowner = GetUnitOwnerSlot(selectedunit);
										if (unitowner != 15)
										{
											bool PressedButton = false;

											if (IsNULLButtonFound(GetSkillPanelButton(11)))
											{
												if (keyAction.altbtnID >= 0)
												{
													if (!((DelayedPressList[i].NeedPresslParam & 0x40000000) > 0))
													{
														if (keyAction.IsSkill)
															PressedButton = PressSkillPanelButton(keyAction.altbtnID, keyAction.IsRightClick);
														else
															PressedButton = PressItemPanelButton(keyAction.btnID, keyAction.IsRightClick);
													}

												}
											}
											else
											{
												if (!((DelayedPressList[i].NeedPresslParam & 0x40000000) > 0))
												{
													if (keyAction.IsSkill)
														PressedButton = PressSkillPanelButton(keyAction.btnID, keyAction.IsRightClick);
													else
														PressedButton = PressItemPanelButton(keyAction.btnID, keyAction.IsRightClick);
													//PressedButton = true;
												}

											}

											/*	if ( !PressedButton )
												{
													if ( SetInfoObjDebugVal )
													{
														PrintText( "NO ButtonPressed!" );
													}
												}

	*/
											if (keyAction.IsQuickCast && PressedButton && IsCursorSelectTarget())
											{
												if (SetInfoObjDebugVal)
												{
													unsigned char* button = GetSkillPanelButton(11);
													PrintText("QuickCast called: ButtonAddr:" + std::to_string((unsigned long)button) + ". IsCursorSelectTarget: " + std::to_string(IsCursorSelectTarget()));
												}

												/*int x = ( int )( *GetWindowXoffset );
												int y = ( int )( *GetWindowYoffset );
												*/
												POINT cursorhwnd;
												GetCursorPos(&cursorhwnd);
												ScreenToClient(Warcraft3Window, &cursorhwnd);

												WarcraftRealWNDProc_ptr(Warcraft3Window, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(cursorhwnd.x, cursorhwnd.y));
												WarcraftRealWNDProc_ptr(Warcraft3Window, WM_LBUTTONUP, 0, MAKELPARAM(cursorhwnd.x, cursorhwnd.y));

												DisableTargetCurcorWORK = 3;


												/*	if ( IsCursorSelectTarget( ) )
												{
												PressSkillPanelButton( 11, false );
												}*/

												//POINT cursor;
												//GetCursorPos( &cursor );

												//x = x - cursorhwnd.x;
												//y = y - cursorhwnd.y;

												//cursor.x = cursor.x + x;
												//cursor.y = cursor.y + y;
												////( toXX, toYY );

												//MouseClick( cursor.x, cursor.y );
											}
											else if (!keyAction.IsQuickCast)
											{
												if (SetInfoObjDebugVal)
												{
													PrintText("Skip quick cast: no flag!");
												}
											}
											else if (!PressedButton)
											{
												if (SetInfoObjDebugVal)
												{
													PrintText("Skip quick cast: Button not pressed!");
												}
											}


											if (PressedButton)
												break;
										}

									}
								}
								break;
							}
						}
					}

				}
				DelayedPressList[i].ISNULL = true;

			}

		}


	}

}


int IsNumpadPressed(int VK)
{
	return VK == VK_NUMPAD1 ||
		VK == VK_NUMPAD2 ||
		VK == VK_NUMPAD4 ||
		VK == VK_NUMPAD5 ||
		VK == VK_NUMPAD7 ||
		VK == VK_NUMPAD8;
}

// | 0 | 1
// | 2 | 3
// | 4 | 5

//int __stdcall GetItemPanelButton( int idx )

int GetBtnIdByNumpad(int VK)
{
	switch (VK)
	{
	case VK_NUMPAD1:
		return  4;
	case VK_NUMPAD2:
		return 5;
	case VK_NUMPAD4:
		return  2;
	case VK_NUMPAD5:
		return 3;
	case VK_NUMPAD7:
		return 0;
	case VK_NUMPAD8:
		return  1;
	default:
		break;
	}

	return -1;
}
unsigned char* GetBtnAddrByNumpad(int VK)
{
	int btnid = GetBtnIdByNumpad(VK);
	if (btnid == -1)
		return 0;

	return GetItemPanelButton(GetBtnIdByNumpad(VK));
}

WPARAM LatestPressedKey = NULL;

POINTS GlobalMousePos = { 0,0 };

//int DebugMsgShow = false;

bool InitTestValues = false;
unsigned int TestValues[10];

void GetMousePosition(float* x, float* y, float* z)
{
#ifdef BOTDEBUG
	PrintDebugInfo("Mouse info");
#endif
	unsigned char* globalclass = GetGlobalClassAddr();

	unsigned char* offset1 = globalclass + 0x3BC;

	if (globalclass)
	{
		if (IsOkayPtr((void*)offset1))
		{
			offset1 = *(unsigned char**)offset1;
			if (IsOkayPtr((void*)offset1))
			{
				*x = *(float*)(offset1 + 0x310);
				*y = *(float*)(offset1 + 0x310 + 4);
				*z = *(float*)(offset1 + 0x310 + 4 + 4);
			}
			else
			{
				*x = 0.0f;
				*y = 0.0f;
				*z = 0.0f;
			}
		}
		else
		{
			*x = 0.0f;
			*y = 0.0f;
			*z = 0.0f;
		}

	}
}


int _stdcall GetMouseX(int)
{
	return GlobalMousePos.x;
}

int _stdcall GetMouseY(int)
{
	return GlobalMousePos.y;
}

float _stdcall GetMouseFrameX(int)
{
	float X,Y,Z;
	GetMousePosition(&X, &Y, &Z);
	return X;
}

float _stdcall GetMouseFrameY(int)
{
	float X, Y, Z;
	GetMousePosition(&X, &Y, &Z);
	return Y;
}

float __stdcall GetMouseIngameX(int)
{
	float x, y, z;
	GetMousePosition(&x, &y, &z);
	return x;
}

float __stdcall GetMouseIngameY(int)
{
	float x, y, z;
	GetMousePosition(&x, &y, &z);
	return y;
}

float __stdcall GetMouseIngameZ(int)
{
	float x, y, z;
	GetMousePosition(&x, &y, &z);
	return z;
}


unsigned int __stdcall GetTestValue(int id)
{
	if (DEBUG_FULL)
		std::cout << __func__ << std::endl;
	if (id >= 0 && id <= 7)
	{
		return TestValues[id];
	}
	return 0;
}

bool ForceLvl1 = false;
bool ForceLvl2 = false;
bool ForceLvl3 = false;

void __stdcall SetForceHotkeyProcess(int lvl1, int lvl2, int lvl3)
{
	ForceLvl1 = lvl1;
	ForceLvl2 = lvl2;
	ForceLvl3 = lvl3;
}

// Заменяет текст в строке 
bool replaceAll(std::string& str, const std::string& from, const std::string& to, int addtofrom = 0)
{
	if (from.empty())
		return false;
	bool replaced = false;
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		replaced = true;
		str.replace(start_pos, from.length() + addtofrom, to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
	return replaced;
}


std::string ReturnStringWithoutWarcraftTags(std::string str)
{
	if (str.length() <= 1)
		return str;
	replaceAll(str, "|c", "", 8);
	replaceAll(str, "|n", "");
	replaceAll(str, "|r", "");
	replaceAll(str, "|", "");
	replaceAll(str, "- [Level ", "[");
	return str;
}


std::string ReturnStringBeforeFirstChar(std::string str, char crepl)
{
	if (str.length() <= 1)
		return str;

	for (auto& c : str)
	{
		if (c == crepl)
		{
			c = '\0';
			break;
		}
	}
	return str;
}

std::string ReturnStringBeforeFirstString(std::string str, char crepl)
{
	if (str.length() <= 1)
		return str;

	for (auto& c : str)
	{
		if (c == crepl)
		{
			c = '\0';
			break;
		}
	}
	return str;
}

std::string GetObjectNameByID(int clid)
{
	if (clid > 0)
	{
		int tInfo = GetTypeInfo(clid, 0);
		int tInfo_1d, tInfo_2id;
		if (tInfo && (tInfo_1d = *(int*)(tInfo + 40)) != 0)
		{
			tInfo_2id = tInfo_1d - 1;
			if (tInfo_2id >= (unsigned int)0)
				tInfo_2id = 0;
			return (const char*)*(int*)(*(int*)(tInfo + 44) + 4 * tInfo_2id);
		}
		else
		{
			return "Default String";
		}
	}
	return "Default String";
}

std::string IsCooldownMessage = "%s > On cooldown ( %02i:%02i ).";
std::string IsCooldownAndNoMana = "%s > On cooldown and no mana.";
std::string IsReadyMessage = "%s > is ready.";
std::string WantToLearnMessage = "I want to %s";
std::string WantToPickMessage = "I want to pick > %s";
std::string NeedToChooseMessage = "We need > %s";


std::string WantToBuyMessage = "I want to buy > %s";
std::string NeedToBuyMessage = "We need to buy > %s";


std::string ItemAbiltPlayerHasItem = "%s has > %s";
std::string NeedMoreMana = "Need %i mana for > %s";
std::string IgotSilence = "Can't use %s while silenced";

int ignorelist[] = { 1,2,3 };

std::vector<ObjInfoAction> IgnoreObjInfo;

void __stdcall SetCustomDefaultMessage(int id, const char* message)
{
	if (id < 1 || id > 10 || message == NULL)
		return;

	switch (id)
	{
	case 1:
		IsCooldownMessage = message;
		break;
	case 2:
		IsReadyMessage = message;
		break;
	case 3:
		WantToLearnMessage = message;
		break;
	case 4:
		WantToPickMessage = message;
		break;
	case 5:
		NeedToChooseMessage = message;
		break;
	case 6:
		WantToBuyMessage = message;
		break;
	case 7:
		NeedToBuyMessage = message;
		break;
	case 8:
		ItemAbiltPlayerHasItem = message;
		break;
	case 9:
		NeedMoreMana = message;
		break;
	case 10:
		IgotSilence = message;
		break;
	default:
		break;
	}

}

/*
 action

-1 ignore click
0 skip message
1 IsCooldownMessage
2 IsReadyMessage
3 WantToLearnMessage
4 WantToPickMessage
5 WantToBuyMessage
6 ItemAbiltPlayerHasItem
7 CustomMessage ( custommessage )
8 CustomPrintTextMessage ( custommessage )

*/

/*
ObjId

ObjId = CommandData + 4 offset
ObjId2 = CommandData + 8 offset

OrderId
AbilId
ItemId
*/
void __stdcall AddInfoObjCodeCustomAction(int ObjId, int ObjId2, int action, const char* custommessage)
{
	if (ObjId == 0 && ObjId2 == 0 && action == 0)
	{
		IgnoreObjInfo.clear();
	}

	for (auto& s : IgnoreObjInfo)
	{
		if (s.ObjId == ObjId && s.ObjId2 == ObjId2)
		{
			s.Action = action;
			s.CustomMessage = custommessage ? custommessage : "";
			return;
		}
	}
	IgnoreObjInfo.emplace_back(ObjInfoAction(ObjId, ObjId2, action, custommessage ? custommessage : ""));

}


std::vector< int > InfoWhitelistedObj;

void __stdcall AddInfoWhiteListedObj(int ObjId)
{
	if (ObjId == 0)
	{
		InfoWhitelistedObj.clear();
	}

	InfoWhitelistedObj.push_back(ObjId);

}

void __stdcall SetInfoObjDebug(int debug)
{
	SetInfoObjDebugVal = debug;
}

int GetAbilityManacost(unsigned char * pAbil)
{
	if (pAbil)
	{
		int vtable = *(int*)pAbil;
		if (vtable)
		{
			int GetAbilMPcostAddr = *(int*)(vtable + 0x3d4);
			if (GetAbilMPcostAddr)
			{
				auto GetAbilMPcost = (int(__thiscall*)(unsigned char*))(GetAbilMPcostAddr);
				return GetAbilMPcost(pAbil);
			}
		}
	}
	return 0;
}



pSimpleButtonPreClickEvent SimpleButtonPreClickEvent_org;
pSimpleButtonPreClickEvent SimpleButtonPreClickEvent_ptr;
int __fastcall SimpleButtonPreClickEvent_my(unsigned char* pButton, int unused, int a2)
{

	/*__try
	{*/
	bool incooldown = false;
	std::string incooldownmessage = "";

	unsigned char* selectedunit = 0;
	char PrintAbilState[2048];
	PrintAbilState[0] = '\0';
	if (pButton && (IsKeyPressed(VK_LMENU) || (IsKeyPressed(VK_LMENU) && IsKeyPressed(VK_LCONTROL))) && IsCommandButton(pButton) && (selectedunit = GetSelectedUnit(GetLocalPlayerId())))
	{
		int CommandButtonData = *(int*)(pButton + 0x190);
		if (CommandButtonData)
		{
			//CONSOLE_Print( "Command button" );
			int pObjId = *(int*)(CommandButtonData + 4);
			int pItemUnitID = *(int*)(CommandButtonData + 8);
			const char* pAbilTitle = (const char*)(CommandButtonData + 0x2C);
			std::string AbilName = /*ReturnStringBeforeFirstChar(*/ ReturnStringWithoutWarcraftTags(pAbilTitle ? pAbilTitle : "")/*, '(' )*/;
			//	AbilName = ReturnStringBeforeFirstChar( AbilName, '[' );
			if (!pObjId)
			{
				AbilName = ReturnStringBeforeFirstChar(AbilName, '(');
			}
			unsigned char * pAbil = *(unsigned char**)(CommandButtonData + 0x6D4);
			//bool AbilFound = pAbil > 0 ;
			int pObjId_1 = *(int*)(CommandButtonData + 0x6F8);
			int pObjId_2 = *(int*)(CommandButtonData + 0x6FC);
			int pBtnFlag = *(int*)(CommandButtonData + 0x5BC);


			int unitownerslot = GetUnitOwnerSlot(selectedunit);
			int localplayeridslot = GetLocalPlayerId();


			int UnitMana = 0;
			int AbilManacost = 0;

			if (SetInfoObjDebugVal)
			{
				char buttoninfo[256];

				sprintf_s(buttoninfo, " Command button %p data -> %X\n Object id: %X \nObject id2: %X \nObject id2: %X \n Abil addr: %p \n Title :%s \n Manacost: %i \n Unit Mana: %i", pButton, CommandButtonData, pObjId, pItemUnitID, pObjId_1, pAbil, (pAbilTitle ? pAbilTitle : " no "), AbilManacost, UnitMana);


				PrintText(buttoninfo);
			}

			if (pObjId && !pAbil)
			{
				unsigned int abilscount = 0;
				unsigned char** abils = FindUnitAbils(selectedunit, &abilscount, pObjId);
				if (abilscount > 0)
				{
					pAbil = abils[0];
				}
			}

			if (pAbil)
			{
				int pAbilId = *(int*)(pAbil + 0x34);
				if (pAbilId)
				{
					if (AbilName.length() <= 2)
					{
						AbilName = ReturnStringWithoutWarcraftTags(GetObjectNameByID(pAbilId));
					}
				}
			}


			if (*(unsigned int*)(pButton + 0x140) & 16 || (std::find(InfoWhitelistedObj.begin(), InfoWhitelistedObj.end(), pObjId) != InfoWhitelistedObj.end()))
			{
				if (pObjId != 'AHer' && pObjId != 'Asel' && pObjId != 'Asud' && pObjId != 'Asid' && pObjId != 'Aneu'
					&& pObjId != 0x77123477 && pObjId != 0x07123407 && pObjId != 0x77000077)
				{
					if (pAbil || pObjId)
					{
						if (localplayeridslot == unitownerslot)
						{
							if (pAbil && *(unsigned char**)(pAbil + 0x54))
							{
								unsigned char* tmpIdx = *(unsigned char**)(pAbil + 0x54);
								pObjId = *(int*)(tmpIdx + 0x34);
							}
							if ((std::find(InfoWhitelistedObj.begin(), InfoWhitelistedObj.end(), pObjId) != InfoWhitelistedObj.end())
								|| (pObjId != 'AHer' && pObjId != 'Asel' && pObjId != 'Asud' && pObjId != 'Asid' && pObjId != 'Aneu'))
							{

								unsigned int abilscount = 0;
								unsigned char** abils = FindUnitAbils(selectedunit, &abilscount, pObjId);
								if (abilscount > 0)
								{
									AbilManacost = GetAbilityManacost(abils[0]);
								}
							}
						}
					}
				}
			}


			if (AbilManacost)
			{
				float fUnitMana = 0.0f;
				GetUnitFloatState(selectedunit, &fUnitMana, 2);
				UnitMana = (int)fUnitMana;
			}


			if (SetInfoObjDebugVal && AbilManacost == 0)
			{
				PrintText("Ignore mana request for button");
			}


			for (auto& s : IgnoreObjInfo)
			{
				int tmpObj1 = 0;
				if (pAbil && *(int*)(pAbil + 0x54))
				{
					tmpObj1 = *(int*)(pAbil + 0x54);
					tmpObj1 = *(int*)(tmpObj1 + 0x30);
				}
				if ((s.ObjId == pObjId || s.ObjId == tmpObj1 || s.ObjId == 0) && (s.ObjId2 == pObjId || s.ObjId2 == pObjId_1 || s.ObjId2 == pItemUnitID || s.ObjId2 == 0))
				{
					switch (s.Action)
					{
					case -1:
						return 0;
					case 1:
						sprintf_s(PrintAbilState, IsCooldownMessage.c_str(), AbilName.c_str());
						SendMessageToChat(PrintAbilState, 0);
						return 0;
					case 2:
						sprintf_s(PrintAbilState, IsReadyMessage.c_str(), AbilName.c_str());
						SendMessageToChat(PrintAbilState, 0);
						return 0;
					case 3:
						sprintf_s(PrintAbilState, WantToLearnMessage.c_str(), AbilName.c_str());
						SendMessageToChat(PrintAbilState, 0);
						return 0;
					case 4:
						if (!IsKeyPressed(VK_LCONTROL))
							sprintf_s(PrintAbilState, WantToPickMessage.c_str(), AbilName.c_str());
						else
							sprintf_s(PrintAbilState, NeedToChooseMessage.c_str(), AbilName.c_str());
						SendMessageToChat(PrintAbilState, 0);
						return 0;
					case 5:
						if (!IsKeyPressed(VK_LCONTROL))
							sprintf_s(PrintAbilState, WantToBuyMessage.c_str(), AbilName.c_str());
						else
							sprintf_s(PrintAbilState, NeedToBuyMessage.c_str(), AbilName.c_str());

						SendMessageToChat(PrintAbilState, 0);
						return 0;
					case 6:
						sprintf_s(PrintAbilState, ItemAbiltPlayerHasItem.c_str(), GetPlayerName(unitownerslot, 1), AbilName.c_str());
						SendMessageToChat(PrintAbilState, 0);
						return 0;
					case 7:
						sprintf_s(PrintAbilState, (s.CustomMessage.length() > 0 ? s.CustomMessage.c_str() : AbilName.c_str()), AbilName.c_str());
						SendMessageToChat(PrintAbilState, 0);
						return 0;
					case 8:
						PrintText((s.CustomMessage.length() > 0 ? s.CustomMessage.c_str() : AbilName.c_str()));
						return 0;
					default:
						return SimpleButtonPreClickEvent_ptr(pButton, unused, a2);
					}
				}
			}


			if (AbilName.length() > 1 &&
				(pItemUnitID == 0xD0142
					||
					((pBtnFlag != 2 && (pObjId != 'AHer' || pObjId_1 != 0)) && (pAbil || pObjId || (pAbilTitle && pAbilTitle[0] != '\0' && localplayeridslot != unitownerslot)))
					||
					(std::find(InfoWhitelistedObj.begin(), InfoWhitelistedObj.end(), pObjId) != InfoWhitelistedObj.end())))
			{
				if (SetInfoObjDebugVal)
				{
					PrintText("Click Button 1");
				}


				if (unitownerslot != localplayeridslot && unitownerslot != 15)
				{
					sprintf_s(PrintAbilState, ItemAbiltPlayerHasItem.c_str(), GetPlayerName(unitownerslot, 1), AbilName.c_str());
				}
				else
				{
					sprintf_s(PrintAbilState, IsReadyMessage.c_str(), AbilName.c_str());
				}

				if (unitownerslot == localplayeridslot)
				{
					if (pBtnFlag != 2 && pObjId == 'AHer')
					{
						sprintf_s(PrintAbilState, WantToLearnMessage.c_str(), AbilName.c_str());
						SendMessageToChat(PrintAbilState, 0);
						return 0;
					}

				}

				if (pObjId == 'Asel')
				{
					//PrintText( "Sell Button" );
					char* pObjectIdToStr = (char*)&pItemUnitID;
					if (isupper(pObjectIdToStr[3]))
					{
						//	PrintText( "Sell hero" );
						if (!IsKeyPressed(VK_LCONTROL))
							sprintf_s(PrintAbilState, WantToPickMessage.c_str(), AbilName.c_str());
						else
							sprintf_s(PrintAbilState, NeedToChooseMessage.c_str(), AbilName.c_str());
					}
					else
					{
						//PrintText( "Sell item" );
						if (!IsKeyPressed(VK_LCONTROL))
							sprintf_s(PrintAbilState, WantToBuyMessage.c_str(), AbilName.c_str());
						else
							sprintf_s(PrintAbilState, NeedToBuyMessage.c_str(), AbilName.c_str());

					}


					if (AbilManacost > UnitMana)
					{
						sprintf_s(PrintAbilState, NeedMoreMana.c_str(), (AbilManacost - UnitMana), AbilName.c_str());
						/*SendMessageToChat( PrintAbilState, 0 );
						return 0;*/
					}

					SendMessageToChat(PrintAbilState, 0);
					return 0;
				}


				if (pAbil && unitownerslot == localplayeridslot)
				{
					//CONSOLE_Print( 
					if (SetInfoObjDebugVal)
					{
						PrintText("Click Button owner ability!");
					}

					int pAbilId = *(int*)(pAbil + 0x34);
					if (pAbilId && (*(unsigned int*)(pButton + 0x140) & 16 || (std::find(InfoWhitelistedObj.begin(), InfoWhitelistedObj.end(), pObjId) != InfoWhitelistedObj.end())))
					{
						if (pObjId != 'AHer' && pObjId != 'Asel' && pObjId != 'Asud' && pObjId != 'Asid' && pObjId != 'Aneu')
						{
							//if ( !pObjId )
							//	AbilName = ReturnStringBeforeFirstChar( ReturnStringWithoutWarcraftTags( pAbilTitle ), '(' );
							//else


							if (SetInfoObjDebugVal)
							{
								PrintText("Search cooldown ");
							}


							int pAbilData = *(int*)(pAbil + 0xDC);
							if (pAbilData)
							{
								if (SetInfoObjDebugVal)
								{
									PrintText("Found 1");
								}


								float pAbilDataVal1 = *(float*)(pAbilData + 0x4);
								int pAbilDataVal2tmp = *(int*)(pAbilData + 0xC);
								if (pAbilDataVal1 > 0.0f && pAbilDataVal2tmp)
								{
									if (SetInfoObjDebugVal)
									{
										PrintText("Found 2");
									}
									float pAbilDataVal2 = *(float*)(pAbilDataVal2tmp + 0x40);
									float AbilCooldown = pAbilDataVal1 - pAbilDataVal2;
									int AbilCooldownMinutes = (int)(AbilCooldown / 60.0f);
									int AbilCooldownSeconds = (int)((int)AbilCooldown % 60);

									incooldown = true;
									sprintf_s(PrintAbilState, IsCooldownMessage.c_str(), AbilName.c_str(), AbilCooldownMinutes, AbilCooldownSeconds);
									incooldownmessage = PrintAbilState;

								}
								else
								{
									if (SetInfoObjDebugVal)
									{
										PrintText("No cooldown Button 1.1");
									}
									sprintf_s(PrintAbilState, IsReadyMessage.c_str(), AbilName.c_str());
								}
							}
							else
							{
								if (SetInfoObjDebugVal)
								{
									PrintText("No cooldown Button 1.2");
								}
								sprintf_s(PrintAbilState, IsReadyMessage.c_str(), AbilName.c_str());
							}
						}
					}
					if (*(int*)(pAbil + 0x3C) > 0)
					{
						sprintf_s(PrintAbilState, IgotSilence.c_str(), AbilName.c_str());
						SendMessageToChat(PrintAbilState, 0);
						return 0;
					}

				}
				if (AbilManacost > UnitMana)
				{
					if (incooldown)
						sprintf_s(PrintAbilState, IsCooldownAndNoMana.c_str(), AbilName.c_str());
					else
						sprintf_s(PrintAbilState, NeedMoreMana.c_str(), (AbilManacost - UnitMana), AbilName.c_str());
				}
				else if (incooldown)
				{
					sprintf_s(PrintAbilState, "%s", incooldownmessage.c_str());
				}
				SendMessageToChat(PrintAbilState, 0);
				return 0;
			}
			else if (pAbil)
			{
				if (SetInfoObjDebugVal)
				{
					PrintText("Click Button 2");
				}

				if (*(int*)(pAbil + 0x3C) > 0)
				{
					sprintf_s(PrintAbilState, IgotSilence.c_str(), AbilName.c_str());
				}
				SendMessageToChat(PrintAbilState, 0);
				return 0;
			}

		}

	}

	//}
	//__except ( g_crashRpt.SendReport( GetExceptionInformation( ) ) )
	//{
	//	::ExitProcess( 0 ); // It is better to stop the process here or else corrupted data may incomprehensibly crash it later.
	//	return 0;
	//}
	return SimpleButtonPreClickEvent_ptr(pButton, unused, a2);
}



typedef float(__fastcall* pGetCameraHeight/*sub_6F3019A0*/)(unsigned int a1);
pGetCameraHeight GetCameraHeight_org;
pGetCameraHeight GetCameraHeight_ptr;

float cameraoffset = 0;


void IncreaseCameraOffset(float offset = 50)
{
	if (cameraoffset > 3000)
		return;
	cameraoffset += offset;
}

void DecreaseCameraOffset(float offset = 50)
{
	if (cameraoffset < -1000)
		return;
	cameraoffset -= offset;
}

void ResetCameraOffset()
{
	cameraoffset = 0;
}

float __fastcall GetCameraHeight_my(unsigned int a1)
{
	return GetCameraHeight_ptr(a1) + cameraoffset;
}


unsigned long GroupSelectLastTime = GetTickCount();
int LastSelectedGroupHandle = 0;

int ProcessHotkeys(HWND& hWnd, unsigned int& Msg, WPARAM& wParam, LPARAM& lParam, bool& _IsAltPressed,
	bool& _IsCtrlPressed, bool& _IsShiftPressed, bool& itempressed, bool& ClickHelperWork, int WithModifiers)
{
	for (KeyActionStruct& keyAction : KeyActionList)
	{
		if (keyAction.VK == (int)wParam)
		{
			if (SetInfoObjDebugVal)
			{
				PrintText("Hotkey vk code found.");
			}
			TestValues[6]++;
			if (Msg == WM_SYSKEYDOWN)
				Msg = WM_KEYDOWN;

			if ((!keyAction.IsAlt && !keyAction.IsCtrl && !keyAction.IsShift && !WithModifiers)
				||
				(WithModifiers && ((keyAction.IsAlt && _IsAltPressed)
					|| (keyAction.IsCtrl && _IsCtrlPressed)
					|| (keyAction.IsShift && _IsShiftPressed)))
				)
			{
				bool DoubleClicked = false;


				if (!keyAction.IsQuickCast)
				{
					if (GetTickCount() - keyAction.LastPressTime < 200)
					{
						itempressed = !keyAction.IsSkill;
						if (SetInfoObjDebugVal)
						{
							PrintText("need doubleclick!");
						}
						int selectedunits = GetSelectedUnitCountBigger(GetLocalPlayerId());
						unsigned char* selectedunit = GetSelectedUnit(GetLocalPlayerId());
						if (selectedunit && selectedunits > 0)
						{
							int unitowner = GetUnitOwnerSlot(selectedunit);
							if (SetInfoObjDebugVal)
							{
								if (!DoubleClickHelper)
								{
									PrintText("doubleclick disabled (!");
								}
							}

							if (DoubleClickHelper && unitowner != 15)
							{
								if (IsCursorSelectTarget())
								{
									if (SetInfoObjDebugVal)
									{
										PrintText("Select hero!");
									}

									if (keyAction.IsShift)
									{
										SkipSingleShift = GetTickCount();
									}

									if (PressMouseAtSelectedHero(itempressed) == 0)
									{
										ClickHelperWork = true;
										if (SetInfoObjDebugVal)
										{
											PrintText("Success double click.");
										}
										DisableTargetCurcorWORK = 3;
										DoubleClicked = true;
									}
								}
							}
						}
						else
						{
							if (SetInfoObjDebugVal)
							{
								PrintText("no doubleclick! no units");
							}
						}

					}
				}

				if (DoubleClicked)
				{
					keyAction.LastPressTime = 0;
					return true;
				}

				if (SetInfoObjDebugVal)
				{
					PrintText("Hotkey availabled!");
				}

				//if (itempressed || (keyAction.IsSkill && !IsCursorSelectTarget()))
				{
					int selectedunits = GetSelectedUnitCountBigger(GetLocalPlayerId());

					unsigned char* selectedunit = GetSelectedUnit(GetLocalPlayerId());
					if (selectedunit && selectedunits > 0)
					{
						int unitowner = GetUnitOwnerSlot(selectedunit);

						if (unitowner != 15 && !ClickHelperWork)
						{
							if (EnableSelectHelper)
							{
								if (unitowner != GetLocalPlayerId() && !GetPlayerAlliance(Player(unitowner), Player(GetLocalPlayerId()), 6))
								{
									//	sprintf_s( keystateprint, 200, "Start emulate #2..." );
									//	PrintText( keystateprint );
									//PressHeroPanelButton( 0, false );
									WarcraftRealWNDProc_ptr(hWnd, WM_KEYDOWN, VK_F1, lpF1ScanKeyDOWN);
									WarcraftRealWNDProc_ptr(hWnd, WM_KEYUP, VK_F1, lpF1ScanKeyUP);
									if (SetInfoObjDebugVal)
									{
										PrintText("Hotkey delay press start!!");
									}

									DelayedPress tmpDelayPress = DelayedPress();
									tmpDelayPress.IsCustom = true;
									tmpDelayPress.IsAlt = _IsAltPressed;
									tmpDelayPress.IsCtrl = _IsCtrlPressed;
									tmpDelayPress.IsShift = _IsShiftPressed;
									tmpDelayPress.NeedPresslParam = lParam;
									tmpDelayPress.NeedPresswParam = wParam;
									tmpDelayPress.NeedPressMsg = Msg;
									tmpDelayPress.TimeOut = 140;
									DelayedPressList_pushback(tmpDelayPress);

									return true;
								}
							}

							bool PressedButton = false;

							if (IsNULLButtonFound(GetSkillPanelButton(11)))
							{
								if (keyAction.altbtnID >= 0)
								{
									if (keyAction.IsSkill)
										PressedButton = PressSkillPanelButton(keyAction.altbtnID, keyAction.IsRightClick);
									else
										PressedButton = PressItemPanelButton(keyAction.btnID, keyAction.IsRightClick);
									//PressedButton = true;
								}
							}
							else
							{
								if (keyAction.IsSkill)
									PressedButton = PressSkillPanelButton(keyAction.btnID, keyAction.IsRightClick);
								else
									PressedButton = PressItemPanelButton(keyAction.btnID, keyAction.IsRightClick);
								//PressedButton = true;
							}

							/*	if ( !PressedButton )
							{
							if ( SetInfoObjDebugVal )
							{
							PrintText( "NO ButtonPressed!" );
							}
							}*/

							if (keyAction.IsQuickCast && PressedButton && IsCursorSelectTarget())
							{
								if (SetInfoObjDebugVal)
								{
									unsigned char* button = GetSkillPanelButton(11);
									PrintText("QuickCast called: ButtonAddr:" + std::to_string((unsigned long)button) + ". IsCursorSelectTarget: " + std::to_string(IsCursorSelectTarget()));
								}

								if (keyAction.IsShift)
								{
									SkipSingleShift = GetTickCount();
								}

								/*int x = ( int )( *GetWindowXoffset );
								int y = ( int )( *GetWindowYoffset );
								*/
								POINT cursorhwnd;
								GetCursorPos(&cursorhwnd);
								ScreenToClient(Warcraft3Window, &cursorhwnd);

								WarcraftRealWNDProc_ptr(Warcraft3Window, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(cursorhwnd.x, cursorhwnd.y));
								WarcraftRealWNDProc_ptr(Warcraft3Window, WM_LBUTTONUP, 0, MAKELPARAM(cursorhwnd.x, cursorhwnd.y));

								DisableTargetCurcorWORK = 3;


								/*	if ( IsCursorSelectTarget( ) )
								{
								PressSkillPanelButton( 11, false );
								}*/

								//POINT cursor;
								//GetCursorPos( &cursor );

								//x = x - cursorhwnd.x;
								//y = y - cursorhwnd.y;

								//cursor.x = cursor.x + x;
								//cursor.y = cursor.y + y;
								////( toXX, toYY );

								//MouseClick( cursor.x, cursor.y );
							}
							else if (!keyAction.IsQuickCast)
							{
								if (SetInfoObjDebugVal)
								{
									PrintText("Skip quick cast: no flag!");
								}
							}
							else if (!PressedButton)
							{
								if (SetInfoObjDebugVal)
								{
									PrintText("Skip quick cast: Button not pressed!");
								}
							}


							//if ( IsNULLButtonFound( GetSkillPanelButton( 11 ) ) )
							//{
							//	if ( keyAction.altbtnID >= 0 )
							//	{
							//		if ( SetInfoObjDebugVal )
							//		{
							//			PrintText( "OK. Now press panel button." );
							//		}
							//		//if ( !( lParam & 0x40000000 ) )
							//	//	{
							//		if ( keyAction.IsSkill )
							//			PressSkillPanelButton( keyAction.altbtnID, keyAction.IsRightClick );
							//		else
							//			PressItemPanelButton( keyAction.btnID, keyAction.IsRightClick );
							//		//	}

							//	}
							//	else
							//	{
							//		if ( SetInfoObjDebugVal )
							//		{
							//			PrintText( "ERROR. NO ACTION FOUND!" );
							//		}
							//	}
							//}
							//else
							//{
							//	if ( SetInfoObjDebugVal )
							//	{
							//		PrintText( "OK. Now press panel button." );
							//	}
							//	//if ( !( lParam & 0x40000000 ) )
							////	{
							//	if ( keyAction.IsSkill )
							//		PressSkillPanelButton( keyAction.btnID, keyAction.IsRightClick );
							//	else
							//		PressItemPanelButton( keyAction.btnID, keyAction.IsRightClick );
							//	//	}

							//}
						}
						else
						{
							if (SetInfoObjDebugVal)
							{
								PrintText("Bad selected unit( player 15 ) or hotkey disabled.");
							}
						}
					}
				}
				/*else
				{
					if (SetInfoObjDebugVal)
					{
						PrintText("No selected unit!");
					}

				}*/
				keyAction.LastPressTime = GetTickCount();
				return true;
			}
			else
			{
				if (SetInfoObjDebugVal)
				{
					PrintText("Hotkey not equal skip...");
				}

			}
		}
	}

	return false;
}
int ProcessSelectActionHotkeys(HWND& hWnd, unsigned int& Msg, WPARAM& wParam, LPARAM& lParam, bool& _IsAltPressed,
	bool& _IsCtrlPressed, bool& _IsShiftPressed, int WithModifiers)
{
	for (auto keyAction : KeySelectActionList)
	{
		if (keyAction.VK == (int)wParam)
		{
			if ((!keyAction.IsAlt && !keyAction.IsCtrl && !keyAction.IsShift && !WithModifiers)
				||
				(WithModifiers && ((keyAction.IsAlt && _IsAltPressed)
					|| (keyAction.IsCtrl && _IsCtrlPressed)
					|| (keyAction.IsShift && _IsShiftPressed)))
				)
			{

				if ((keyAction.IsAlt && _IsAltPressed)
					|| (keyAction.IsCtrl && _IsCtrlPressed)
					|| (keyAction.IsShift && _IsShiftPressed)
					)
				{
					if (Msg == WM_SYSKEYDOWN)
						Msg = WM_KEYDOWN;
				}
				else
				{
					if (_IsAltPressed
						|| _IsCtrlPressed)
					{

					}
				}

				if (SetInfoObjDebugVal)
				{
					PrintText("Clear selection");
				}

				std::vector< unsigned char*> units = GetUnitsFromGroup(keyAction.GroupHandle);
				//reverse( units.begin( ), units.end( ) );


				if (SetInfoObjDebugVal)
				{
					PrintText(("Select units:" + uint_to_hex(units.size())).c_str());
				}


				if (units.size() > 0)
				{
					ClearSelection();

					for (auto unit : units)
					{
						if (SetInfoObjDebugVal)
						{
							PrintText(("Select unit:" + uint_to_hex((unsigned long)unit)).c_str());
						}
						SelectUnit(unit);
					}


					if (GetTickCount() - GroupSelectLastTime < 450 &&
						LastSelectedGroupHandle == keyAction.GroupHandle)
					{
						unsigned char* PortraitButtonAddr = GetGlobalClassAddr();
						if (PortraitButtonAddr)
						{
							PortraitButtonAddr = *(unsigned char**)(PortraitButtonAddr + 0x3F4);
						}
						if (PortraitButtonAddr)
						{
							if (SetInfoObjDebugVal)
							{
								PrintText("double click to portrain.");
							}
							Wc3ControlClickButton_org(PortraitButtonAddr, 1);
							Wc3ControlClickButton_org(PortraitButtonAddr, 1);
						}
					}


				}

				LastSelectedGroupHandle = keyAction.GroupHandle;
				GroupSelectLastTime = GetTickCount();

				return true;
			}
		}
	}
	return false;
}
int ProcessCallbackHotkeys(HWND& hWnd, unsigned int& Msg, WPARAM& wParam, LPARAM& lParam, bool& _IsAltPressed,
	bool& _IsCtrlPressed, bool& _IsShiftPressed, int WithModifiers)
{
	int selectedunits = GetSelectedUnitCountBigger(GetLocalPlayerId());

	unsigned char* selectedunit = GetSelectedUnit(GetLocalPlayerId());
	if (selectedunit && selectedunits > 0)
	{
		int unitowner = GetUnitOwnerSlot(selectedunit);
		if (unitowner != 15)
		{
			for (auto keyAction : KeyCalbackActionList)
			{
				if (keyAction.VK == (int)wParam)
				{
					if ((!keyAction.IsAlt && !keyAction.IsCtrl && !keyAction.IsShift && !WithModifiers)
						||
						(WithModifiers && ((keyAction.IsAlt && _IsAltPressed)
							|| (keyAction.IsCtrl && _IsCtrlPressed)
							|| (keyAction.IsShift && _IsShiftPressed)))
						)
					{
						if (SetInfoObjDebugVal)
						{
							PrintText("Need keyAction!");
						}

						if ((keyAction.IsAlt && _IsAltPressed)
							|| (keyAction.IsCtrl && _IsCtrlPressed)
							|| (keyAction.IsShift && _IsShiftPressed)
							)
						{
							if (Msg == WM_SYSKEYDOWN)
								Msg = WM_KEYDOWN;
						}
						else
						{
							if (_IsAltPressed
								|| _IsCtrlPressed)
							{

							}
						}


						__asm
						{
							push eax;
							mov eax, GameDll;
							add eax, 0x3309F0;
							call eax;
							pop eax;
						}


						((int(__thiscall*)(int, int, int, int, int, int, int))(GameDll + 0x37C420))
							(keyAction.args[0], keyAction.args[1], keyAction.args[2],
								keyAction.args[3], keyAction.args[4], keyAction.args[5], keyAction.args[6]);

						return true;
					}
				}
			}

		}
	}
	return false;
}
int ProcessChatHotkeys(HWND& hWnd, unsigned int& Msg, WPARAM& wParam, LPARAM& lParam, bool& _IsAltPressed,
	bool& _IsCtrlPressed, bool& _IsShiftPressed, int WithModifiers)
{
	for (auto keyAction : KeyChatActionList)
	{
		if (keyAction.VK == (int)wParam)
		{
			if ((!keyAction.IsAlt && !keyAction.IsCtrl && !keyAction.IsShift && !WithModifiers) ||

				(WithModifiers && ((keyAction.IsAlt && _IsAltPressed)
					|| (keyAction.IsCtrl && _IsCtrlPressed)
					|| (keyAction.IsShift && _IsShiftPressed)))


				)
			{
				if ((keyAction.IsAlt && _IsAltPressed)
					|| (keyAction.IsCtrl && _IsCtrlPressed)
					|| (keyAction.IsShift && _IsShiftPressed)
					)
				{
					if (Msg == WM_SYSKEYDOWN)
						Msg = WM_KEYDOWN;
				}
				else
				{
					if (_IsAltPressed
						|| _IsCtrlPressed)
					{

					}
				}


				SendMessageToChat(keyAction.Message.c_str(), keyAction.SendToAll);
				return true;
			}
		}
	}
	return false;

}
int ProcessShopHelper(HWND& hWnd, unsigned int& Msg, WPARAM& wParam, LPARAM& lParam)
{
	if (ShopHelperEnabled && IsGameFrameActive() && /*(*/ Msg == WM_KEYDOWN /*|| Msg == WM_KEYUP ) */)
	{

		if (
			wParam == 'Q' ||
			wParam == 'W' ||
			wParam == 'E' ||
			wParam == 'R' ||
			wParam == 'A' ||
			wParam == 'S' ||
			wParam == 'D' ||
			wParam == 'F' ||
			wParam == 'Z' ||
			wParam == 'X' ||
			wParam == 'C' ||
			wParam == 'V'
			)
		{
			unsigned char* selectedunit = GetSelectedUnit(GetLocalPlayerId());
			if (selectedunit && GetSelectedUnitCountBigger(GetLocalPlayerId()) > 0)
			{
				if (GetUnitOwnerSlot(selectedunit) == 15)
				{
					// | 0 | 3 | 6 | 9  |
					// | 1 | 4 | 7 | 10 | 
					// | 2 | 5 | 8 | 11 |

					/*	if ( Msg == WM_KEYDOWN && !( lParam & 0x40000000 ) )
					{*/

					if (wParam == 'Q')
						PressSkillPanelButton(0, false);
					else if (wParam == 'W')
						PressSkillPanelButton(3, false);
					else if (wParam == 'E')
						PressSkillPanelButton(6, false);
					else if (wParam == 'R')
						PressSkillPanelButton(9, false);
					else if (wParam == 'A')
						PressSkillPanelButton(1, false);
					else if (wParam == 'S')
						PressSkillPanelButton(4, false);
					else if (wParam == 'D')
						PressSkillPanelButton(7, false);
					else if (wParam == 'F')
						PressSkillPanelButton(10, false);
					else if (wParam == 'Z')
						PressSkillPanelButton(2, false);
					else if (wParam == 'X')
						PressSkillPanelButton(5, false);
					else if (wParam == 'C')
						PressSkillPanelButton(8, false);
					else if (wParam == 'V')
						PressSkillPanelButton(11, false);
					else
						return false;
					//}
					return true;
				}
			}
		}
	}
	return false;
}
int SkipKeyboardAndMouseWhenTeleport(HWND& hWnd, unsigned int& Msg, WPARAM& wParam, LPARAM& lParam)
{
	if (BlockKeyboardAndMouseWhenTeleport)
	{
		if (Msg == WM_KEYDOWN ||/* Msg == WM_KEYUP || */Msg == WM_RBUTTONDOWN)
		{
			if (Msg == WM_RBUTTONDOWN)
			{
				unsigned char* selectedunit = GetSelectedUnit(GetLocalPlayerId());
				if (selectedunit)
				{
					unsigned int abilscount = 0;
					FindUnitAbils(selectedunit, &abilscount, 'A3VO');
					if (abilscount > 0)
					{
						if (TeleportShiftPress)
						{
							if (ShiftPressed == 0 && !ShiftPressed)
							{
								SingleShift = GetTickCount();
								ShiftPressed = 1;
							}
						}
						else
							return true;
					}
				}
			}



			if ((wParam >= 0x41 && wParam <= 0x5A) || (wParam >= VK_NUMPAD1 && wParam <= VK_NUMPAD8))
			{
				bool NeedSkipForTP = true;

				for (int& VK : WhiteListForTeleport)
				{
					if (wParam == VK)
					{
						NeedSkipForTP = false;
						break;
					}
				}

				if (NeedSkipForTP)
				{
					unsigned char* selectedunit = GetSelectedUnit(GetLocalPlayerId());
					if (selectedunit)
					{
						unsigned int abilscount = 0;
						FindUnitAbils(selectedunit, &abilscount, 'A3VO');
						if (abilscount > 0)
						{
							if (TeleportShiftPress)
							{
								if (!ShiftPressed)
								{
									SingleShift = GetTickCount();
									ShiftPressed = 1;
								}
							}
							else
								return true;
						}
					}
				}

			}

		}
	}

	return false;
}


jRCString GlobalCallbackForRegisteredHotkeys = jRCString();


void __stdcall SetGlobalCallbackForRegisteredHotkeys(const char* func)
{
	str2jstr(&GlobalCallbackForRegisteredHotkeys, func);
}

WPARAM LatestRegisteredHotkeyWPARAM = 0;

int __stdcall GetLatestRegisteredHotkeyWPARAM(int)
{
	if (DEBUG_FULL)
		std::cout << __func__ << std::endl;
	return (int)LatestRegisteredHotkeyWPARAM;
}

int LatestRegisteredHotkeyMsg = 0;

int __stdcall GetLatestRegisteredHotkeyMsg(int)
{
	if (DEBUG_FULL)
		std::cout << __func__ << std::endl;
	return LatestRegisteredHotkeyMsg;
}

int ProcessRegisteredHotkeys(HWND& hWnd, unsigned int& Msg, WPARAM& wParam, LPARAM& lParam)
{
	LatestRegisteredHotkeyWPARAM = wParam;
	LatestRegisteredHotkeyMsg = Msg;
	if (Msg == WM_KEYDOWN || Msg == WM_KEYUP ||
		Msg == WM_LBUTTONDOWN || Msg == WM_MBUTTONDOWN || Msg == WM_RBUTTONDOWN ||
		Msg == WM_LBUTTONUP || Msg == WM_MBUTTONUP || Msg == WM_RBUTTONUP)
	{
		for (int keyCode : RegisteredKeyCodes)
		{
			bool keycodefound = true;
			if (keyCode != (int)wParam)
			{
				keycodefound = false;
				keyCode = keyCode - 0x1000;
			}

			if (keycodefound || keyCode == WM_KEYDOWN || keyCode == WM_KEYUP ||
				keyCode == WM_LBUTTONDOWN || keyCode == WM_MBUTTONDOWN || keyCode == WM_RBUTTONDOWN ||
				keyCode == WM_LBUTTONUP || keyCode == WM_MBUTTONUP || keyCode == WM_RBUTTONUP)
			{
				if (Msg == WM_KEYDOWN || Msg == WM_LBUTTONDOWN || Msg == WM_MBUTTONDOWN || Msg == WM_RBUTTONDOWN)
				{
					if (SetInfoObjDebugVal)
					{
						PrintText("KEY_DOWN");
					}

					if (GlobalCallbackForRegisteredHotkeys.stringRep == NULL)
					{
						//BytesToSend.push_back( 0x50 );
						//// packet header
						//BytesToSend.push_back( 0xFF );
						//// packet size
						//BytesToSend.push_back( 0 );
						//BytesToSend.push_back( 0 );
						//BytesToSend.push_back( 0 );
						//BytesToSend.push_back( 0 );
						SendKeyEvent.push_back(0x50);
						// header custom packets
						SendKeyEvent.push_back(0xFF);
						// size custom packets 
						SendKeyEvent.push_back(0);
						SendKeyEvent.push_back(0);
						SendKeyEvent.push_back(0);
						SendKeyEvent.push_back(0);
						// packet type
						int packettype = 'IKEY';
						SendKeyEvent.insert(SendKeyEvent.end(), (unsigned char*)&packettype, ((unsigned char*)&packettype) + 4);
						*(int*)&SendKeyEvent[2] += 4;
						// data
						int locpid = GetLocalPlayerId();
						SendKeyEvent.insert(SendKeyEvent.end(), (unsigned char*)&locpid, ((unsigned char*)&locpid) + 4);
						*(int*)&SendKeyEvent[2] += 4;
						SendKeyEvent.insert(SendKeyEvent.end(), (unsigned char*)&Msg, ((unsigned char*)&Msg) + 4);
						*(int*)&SendKeyEvent[2] += 4;
						SendKeyEvent.insert(SendKeyEvent.end(), (unsigned char*)&wParam, ((unsigned char*)&wParam) + 4);
						*(int*)&SendKeyEvent[2] += 4;
						SendPacket((unsigned char*)&SendKeyEvent[0], SendKeyEvent.size());
						SendKeyEvent.clear();
					}
					else
					{
						//*KeyboardAddrForKey = ( int ) wParam;
						//*KeyboardAddrForKeyEvent = ( int ) Msg;
						//	TriggerExecute( KeyboardTriggerHandle );

						ExecuteFunc(&GlobalCallbackForRegisteredHotkeys);
					}

					return bTriggerRegisterPlayerKeyboardBlock;
				}
				else if (Msg == WM_KEYUP || Msg == WM_LBUTTONUP || Msg == WM_MBUTTONUP || Msg == WM_RBUTTONUP)
				{
					if (SetInfoObjDebugVal)
					{
						PrintText("KEY_UP");
					}
					if (GlobalCallbackForRegisteredHotkeys.stringRep == NULL)
					{
						SendKeyEvent.push_back(0x50);
						// header custom packets
						SendKeyEvent.push_back(0xFF);
						// size custom packets 
						SendKeyEvent.push_back(0);
						SendKeyEvent.push_back(0);
						SendKeyEvent.push_back(0);
						SendKeyEvent.push_back(0);
						// packet type
						int packettype = 'IKEY';
						SendKeyEvent.insert(SendKeyEvent.end(), (unsigned char*)&packettype, ((unsigned char*)&packettype) + 4);
						*(int*)&SendKeyEvent[2] += 4;
						// data
						int locpid = GetLocalPlayerId();
						SendKeyEvent.insert(SendKeyEvent.end(), (unsigned char*)&locpid, ((unsigned char*)&locpid) + 4);
						*(int*)&SendKeyEvent[2] += 4;
						SendKeyEvent.insert(SendKeyEvent.end(), (unsigned char*)&Msg, ((unsigned char*)&Msg) + 4);
						*(int*)&SendKeyEvent[2] += 4;
						SendKeyEvent.insert(SendKeyEvent.end(), (unsigned char*)&wParam, ((unsigned char*)&wParam) + 4);
						*(int*)&SendKeyEvent[2] += 4;
						SendPacket((unsigned char*)&SendKeyEvent[0], SendKeyEvent.size());
						SendKeyEvent.clear();

					}
					else
					{
						//*KeyboardAddrForKey = ( int ) wParam;
						//*KeyboardAddrForKeyEvent = ( int ) Msg;
						//	TriggerExecute( KeyboardTriggerHandle );

						ExecuteFunc(&GlobalCallbackForRegisteredHotkeys);
					}

					return bTriggerRegisterPlayerKeyboardBlock;
				}
				else
				{
					if (SetInfoObjDebugVal)
					{
						PrintText("UNKNOWN_KEY_EVENT");
					}
				}
			}
		}
	}

	return false;
}

int FixNumpad(HWND& hWnd, unsigned int& Msg, WPARAM& wParam, WPARAM& _wParam, LPARAM& lParam, bool& _IsShiftPressed)
{
	// SHIFT+NUMPAD TRICK
	if (IsGameFrameActive() && (Msg == WM_KEYDOWN || Msg == WM_KEYUP) && (
		wParam == 0xC ||
		wParam == 0x23 ||
		wParam == 0x24 ||
		wParam == 0x25 ||
		wParam == 0x26 ||
		wParam == 0x28
		))
	{
		int  scanCode = (int)((lParam >> 24) & 0x1);

		if (scanCode != 1)
		{
			switch (wParam)
			{
			case 0x23:
				wParam = VK_NUMPAD1;
				break;
			case 0x28:
				wParam = VK_NUMPAD2;
				break;
			case 0x25:
				wParam = VK_NUMPAD4;
				break;
			case 0xC:
				wParam = VK_NUMPAD5;
				break;
			case 0x24:
				wParam = VK_NUMPAD7;
				break;
			case 0x26:
				wParam = VK_NUMPAD8;
				break;
			default:
				break;
			}
			if (wParam != _wParam)
			{
				if (!_IsShiftPressed)
				{
					bool NumLock = (((unsigned short)GetKeyState(VK_NUMLOCK)) & 0xffff) > 0;
					if (NumLock)
						ShiftPressed = 0x1;
					else
						ShiftPressed = 0x0;
				}


				//if (Msg == WM_KEYDOWN && IsNumpadPressed(wParam))
				//{
				//	bool NotFoundInHotKeys = true;
				//	for (KeyActionStruct & keyAction : KeyActionList)
				//	{
				//		if (keyAction.VK == wParam)
				//		{
				//			NotFoundInHotKeys = false;
				//		}
				//	}

				//	if (NotFoundInHotKeys)
				//	{
				//		unsigned char* btnaddr = GetBtnAddrByNumpad(wParam);
				//		if (btnaddr)
				//		{
				//			SimpleButtonClickEvent_my(btnaddr, 0, 1);
				//			/*if ( CheckBtnForClickPortrain( btnaddr ) )
				//			{
				//			SimpleButtonClickEvent_my( btnaddr, 0, 1 );*/
				//			return true;

				//			//int PortraitButtonAddr = GetGlobalClassAddr( );
				//			//if ( PortraitButtonAddr > 0 )
				//			//{
				//			//	PortraitButtonAddr = *( int* )( PortraitButtonAddr + 0x3F4 );
				//			//}
				//			//if ( PortraitButtonAddr > 0 )
				//			//{
				//			//	//PrintText( "Click to portrain." );
				//			//	Wc3ControlClickButton_org( PortraitButtonAddr, 1 );
				//			//}
				//			//}
				//		}
				//	}
				//}
				//LRESULT retval1 = WarcraftRealWNDProc_ptr(hWnd, Msg, wParam, lParam);



				//return false;
			}

		}
		else
		{
			if (!_IsShiftPressed)
			{
				ShiftPressed = 0;
			}
		}
	}


	return false;
}

LRESULT __fastcall WarcraftWindowProcHooked(HWND hWnd, unsigned int _Msg, WPARAM _wParam, LPARAM lParam)
{

	if (Warcraft3Window != hWnd && hWnd != NULL)
	{
		Warcraft3Window = hWnd;
		//SetWar3Window(hWnd);
	}
	//DreamUI_WarWindow3Proc(hWnd, _Msg, _wParam, lParam);

	if (!InitTestValues)
	{
		InitTestValues = true;
		memset(TestValues, 0, sizeof(TestValues));
	}
	TestValues[0]++;


	unsigned int Msg = _Msg;
	bool ClickHelperWork = false;
	WPARAM wParam = _wParam;

	bool _IsCtrlPressed = IsKeyPressed(VK_CONTROL);
	bool _IsShiftPressed = IsKeyPressed(VK_SHIFT);
	bool _IsAltPressed = IsKeyPressed(VK_MENU);

	if (_Msg == WM_KEYDOWN)
	{
		LatestPressedKey = 0;
	}

	if (_Msg == WM_SIZE)
	{
		for (auto& v : ListOfRawImages)
			v.needResetTexture = true;
	}

	// NEXT BLOCK ONLY FOR TEST!!!!
	if ( Msg == WM_KEYDOWN && TestModeActivated )
	{
		ShowConfigWindow( ".\\config.dota" );
	}

	if (SkipAllMessages || TerminateStarted)
	{
		if (SetInfoObjDebugVal)
		{
			PrintText("SkipAllMessages");
		}
		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}


	if (SetInfoObjDebugVal && _Msg == WM_KEYDOWN)
	{
		PrintText("!");
	}

	//if ( Msg == WM_KEYDOWN && wParam == VK_ESCAPE )
	//{
	//	MessageBoxA( 0, "WMKEYDOWNESCAPE", "", 0 );
	//	if ( IsKeyPressed( '1' ) )
	//		return DefWindowProc( hWnd, Msg, wParam, lParam );
	//}



	//if ( Msg == WM_KEYUP && wParam == VK_ESCAPE )
	//{
	//	MessageBoxA( 0, "WMKEYUPESCAPE", "", 0 );
	//	if ( IsKeyPressed( '1' ) )
	//		return DefWindowProc( hWnd, Msg, wParam, lParam );
	//}

	//if ( !DebugMsgShow )
	//{
	//	DebugMsgShow = true;
	//	if ( IsKeyPressed( VK_F1 ) )
	//	{
	//		char debugmsg[ 200 ];
	//		sprintf_s( debugmsg, "Current file cache size:%i", ICONMDLCACHELIST.size( ) );
	//		MessageBoxA( 0, debugmsg, debugmsg, 0 );
	//	}
	//	DebugMsgShow = false;
	//}


	if (!IsGame())
	{
		return WarcraftRealWNDProc_ptr(hWnd, Msg, wParam, lParam);
	}
	if (SetInfoObjDebugVal && _Msg == WM_KEYDOWN)
	{
		PrintText("@");
	}
	else
	{
		if (SetInfoObjDebugVal && Msg == WM_KEYDOWN)
		{
			PrintText("@2");
		}
	}
	if (usedcustomframes && *(int*)ChatFound == 0 && IsGameFrameActive())
	{
		*(int*)pCurrentFrameFocusedAddr = 0;
	}

	if (!IsGameFrameActive())
	{
		if (SetInfoObjDebugVal && Msg == WM_KEYDOWN)
		{
			PrintText("!IsGameFrameActive");
		}
		return WarcraftRealWNDProc_ptr(hWnd, Msg, wParam, lParam);
	}

	DisableTargetCurcor();


	//if ( _Msg == WM_LBUTTONDOWN )
	//{
	//	if ( IsKeyPressed( VK_F1 ) )
	//	{
	//		char debugmsg[ 200 ];
	//		sprintf_s( debugmsg, "_Msg:%X WPARAM:%X LPARAM:%X", Msg, _wParam, lParam );
	//		MessageBoxA( 0, debugmsg, debugmsg, 0 );
	//	}
	//}



	TestValues[1]++;

	if (_Msg == WM_MOUSEWHEEL && IsKeyPressed(VK_LCONTROL))
	{
		short wheeltarg = HIWORD(_wParam);
		if (wheeltarg > 0)
		{
			DecreaseCameraOffset();
		}
		else
		{
			IncreaseCameraOffset();
		}

		WarcraftRealWNDProc_ptr(hWnd, WM_SYSKEYDOWN, VK_PRIOR, NULL);
		WarcraftRealWNDProc_ptr(hWnd, WM_SYSKEYDOWN, VK_NEXT, NULL);

		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}

	if (_Msg == WM_MBUTTONDOWN && IsKeyPressed(VK_LCONTROL))
	{
		ResetCameraOffset();

		WarcraftRealWNDProc_ptr(hWnd, WM_SYSKEYDOWN, VK_PRIOR, NULL);
		WarcraftRealWNDProc_ptr(hWnd, WM_SYSKEYDOWN, VK_NEXT, NULL);

		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}



	if (Msg == WM_LBUTTONDOWN || Msg == WM_RBUTTONDOWN || Msg == WM_MBUTTONDOWN || Msg == WM_LBUTTONUP || Msg == WM_RBUTTONUP || Msg == WM_MBUTTONUP)
	{
		//DisableInputForAnyHotkeyAndEditBox();
	}


	//if (IsAnyEditBoxIsActive())
	//{
	//	if (_Msg == WM_KEYDOWN || _Msg == WM_SYSKEYDOWN)
	//	{
	//		if (wParam == VK_BACK)
	//		{
	//			CurrentEditBoxRemoveCharacter(false);
	//		}
	//		else if (wParam == VK_DELETE)
	//		{
	//			CurrentEditBoxRemoveCharacter(true);
	//		}
	//		else if (wParam == VK_LEFT)
	//		{
	//			CurrentEditBoxMoveCursorLeft();
	//		}
	//		else if (wParam == VK_RIGHT)
	//		{
	//			CurrentEditBoxMoveCursorRight();
	//		}
	//		else
	//		{
	//			unsigned char _keystate[256];
	//			WCHAR  _inputbuf[32]{ 0 };
	//			GetKeyboardState(_keystate);
	//			if (!(MapVirtualKey(wParam, MAPVK_VK_TO_CHAR) >> (sizeof(unsigned int) * 8 - 1) & 1))
	//			{
	//				if (ToUnicode(wParam, MapVirtualKey(wParam, 0),
	//					_keystate, _inputbuf, 32, 0) >= 1)
	//				{
	//					CurrentEditBoxEnterText(_inputbuf);
	//				}
	//			}

	//		}


	//		return DefWindowProc(hWnd, Msg, wParam, lParam);
	//	}
	//}


	if ((lParam & 0x40000000) > 0 && Msg == WM_KEYDOWN && !*(int*)ChatFound)
	{
		if (SetInfoObjDebugVal)
		{
			PrintText("Skip1");
		}

		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}


	//if (IsAnyHotkeyIsActive())
	//{
	//	if (_Msg == WM_KEYDOWN || _Msg == WM_KEYUP || _Msg == WM_SYSKEYDOWN || _Msg == WM_SYSKEYUP ||
	//		/*_Msg == WM_LBUTTONUP || _Msg == WM_LBUTTONDOWN ||*/ _Msg == WM_RBUTTONUP || _Msg == WM_RBUTTONDOWN ||
	//		_Msg == WM_MBUTTONDOWN || _Msg == WM_MBUTTONUP)
	//	{
	//		return DefWindowProc(hWnd, Msg, wParam, lParam);
	//	}
	//}




	TestValues[2]++;


	if (Msg == WM_MOUSEMOVE)
	{
		GlobalMousePos = MAKEPOINTS(lParam);
	}


	if (*IsWindowActive || ForceLvl2)
	{
		TestValues[3]++;

		if (Msg == WM_LBUTTONUP)
		{
			ProcessClickAtCustomFrames();
		}

		if (GlobalRawImageCallbackData)
		{
			if (Msg == WM_LBUTTONUP)
			{
				GlobalRawImageCallbackData->IsLeftButton = true;
				RawImageGlobalCallbackFunc(RawImageEventType::MouseUp, (float)GlobalMousePos.x, (float)GlobalMousePos.y);
			}

			if (Msg == WM_LBUTTONDOWN)
			{
				GlobalRawImageCallbackData->IsLeftButton = true;
				if (RawImageGlobalCallbackFunc(RawImageEventType::MouseDown, (float)GlobalMousePos.x, (float)GlobalMousePos.y))
				{
					if (SetInfoObjDebugVal)
					{
						PrintText("Skip WM_LBUTTONDOWN");
					}
					return DefWindowProc(hWnd, Msg, wParam, lParam);
				}
				else
				{
					if (SetInfoObjDebugVal)
					{
						PrintText("No Skip WM_LBUTTONDOWN");
					}
				}
			}

			if (Msg == WM_RBUTTONUP)
			{
				GlobalRawImageCallbackData->IsLeftButton = false;
				RawImageGlobalCallbackFunc(RawImageEventType::MouseUp, (float)GlobalMousePos.x, (float)GlobalMousePos.y);
			}

			if (Msg == WM_RBUTTONDOWN)
			{
				GlobalRawImageCallbackData->IsLeftButton = false;
				if (RawImageGlobalCallbackFunc(RawImageEventType::MouseDown, (float)GlobalMousePos.x, (float)GlobalMousePos.y))
					return DefWindowProc(hWnd, Msg, wParam, lParam);
			}

			if (Msg == WM_MOUSEMOVE)
			{
				RawImageGlobalCallbackFunc(RawImageEventType::MouseMove, (float)GlobalMousePos.x, (float)GlobalMousePos.y);
			}
		}

		if (LOCK_MOUSE_IN_WINDOW)
		{
			POINT p;
			tagWINDOWINFO pwi;
			if (Warcraft3Window && GetCursorPos(&p) && GetWindowInfo(Warcraft3Window, &pwi) && IsMouseOverWindow(pwi.rcClient, p))
			{
				ClipCursor(&pwi.rcClient);
			}
			else
			{
				ClipCursor(0);
			}
		}


		if (FPS_LIMIT_ENABLED)
		{
			auto t_end = std::chrono::high_resolution_clock::now();
			if (std::chrono::duration<float, std::milli>(t_end - t_start).count() > 250.0)
			{
				t_start = t_end;
				UpdateFPS();
			}
		}

		if (AutoSelectHero)
		{
			if (Msg == WM_KEYDOWN && wParam >= VK_F1 && wParam <= VK_F5)
			{
				LPARAM lpFKEYScanKeyUP = (LPARAM)(0xC0000001 | (LPARAM)(MapVirtualKey(wParam, 0) << 16));
				LPARAM lpFKEYScanKeyDOWN = (LPARAM)(0x00000001 | (LPARAM)(MapVirtualKey(wParam, 0) << 16));

				WarcraftRealWNDProc_ptr(hWnd, WM_KEYDOWN, wParam, lpFKEYScanKeyDOWN);
				WarcraftRealWNDProc_ptr(hWnd, WM_KEYUP, wParam, lpFKEYScanKeyUP);

			}
		}

		if ((Msg == WM_KEYDOWN || Msg == WM_KEYUP) && (_wParam == VK_SHIFT || _wParam == VK_LSHIFT || _wParam == VK_RSHIFT))
		{
			ShiftPressed = (unsigned char)(Msg == WM_KEYDOWN ? 0x1u : 0x0u);
		}

		if (Msg == WM_RBUTTONDOWN)
		{
			ShiftPressed = (unsigned char)(_IsShiftPressed ? 0x1u : 0x0u);
		}



		for (unsigned int i = 0; i < SkipMessagesList.size(); i++)
		{
			if (SkipMessagesList[i].Msg == Msg && SkipMessagesList[i].wParam == wParam)
			{
				SkipMessagesList.erase(SkipMessagesList.begin() + (int)i);
				if (SetInfoObjDebugVal)
				{
					PrintText("SKIP SINGLE MSG");
				}
				return DefWindowProc(hWnd, Msg, wParam, lParam);
			}
		}

		if (Msg == WM_MOUSEMOVE && BLOCKMOUSEMOVING)
		{
			if (SetInfoObjDebugVal)
			{
				PrintText("MOUSE MOVING BLOCKED");
			}
			return DefWindowProc(hWnd, Msg, wParam, lParam);
		}

		TestValues[4]++;


		if ((*(int*)ChatFound == 0 || ForceLvl3) && (IsGameFrameActive() || ForceLvl1))
		{
			TestValues[5]++;

			//char keystateprint[ 200 ];

			if (SetInfoObjDebugVal && (Msg == WM_KEYUP || Msg == WM_KEYDOWN))
			{
				PrintText("Fix numpad...");
			}

			if (FixNumpad(hWnd, Msg, wParam, _wParam, lParam, _IsShiftPressed))
			{
				return DefWindowProc(hWnd, Msg, wParam, lParam);
			}

			if (SetInfoObjDebugVal && (Msg == WM_KEYUP || Msg == WM_KEYDOWN))
			{
				PrintText("Fix numpad OK");
			}


			if (Msg == WM_KEYDOWN && IsNumpadPressed(wParam) && IsGameFrameActive())
			{
				bool NotFoundInHotKeys = true;
				for (KeyActionStruct& keyAction : KeyActionList)
				{
					if (keyAction.VK == wParam)
					{
						NotFoundInHotKeys = false;
					}
				}

				if (NotFoundInHotKeys)
				{
					unsigned char* btnaddr = GetBtnAddrByNumpad(wParam);
					if (btnaddr)
					{
						SimpleButtonClickEvent_my(btnaddr, 0, 1);
						return DefWindowProc(hWnd, Msg, wParam, lParam);
					}
				}
			}
			if (SetInfoObjDebugVal)
			{
				if (Msg == WM_KEYDOWN || Msg == WM_KEYUP ||
					Msg == WM_LBUTTONDOWN || Msg == WM_MBUTTONDOWN || Msg == WM_RBUTTONDOWN ||
					Msg == WM_LBUTTONUP || Msg == WM_MBUTTONUP || Msg == WM_RBUTTONUP)
				{
					char tempDebugMsg[256];
					sprintf_s(tempDebugMsg, "DEBUG MSG:%X WPARAM:%X LPARAM:%X", _Msg, _wParam, lParam);
					PrintText(tempDebugMsg);
				}
			}
			if (ProcessRegisteredHotkeys(hWnd, Msg, wParam, lParam))
			{
				return DefWindowProc(hWnd, Msg, wParam, lParam);
			}


			if (SetInfoObjDebugVal && (Msg == WM_KEYUP || Msg == WM_KEYDOWN))
			{
				PrintText("SkipKeyboardAndMouseWhenTeleport...");
			}


			if (SkipKeyboardAndMouseWhenTeleport(hWnd, Msg, wParam, lParam))
			{
				return DefWindowProc(hWnd, Msg, wParam, lParam);
			}


			if (Msg == WM_KEYDOWN || Msg == WM_XBUTTONDOWN || Msg == WM_MBUTTONDOWN ||
				Msg == WM_SYSKEYDOWN)
			{
				bool itempressed = false;

				if (_Msg == WM_XBUTTONDOWN)
				{
					Msg = WM_KEYDOWN;
					wParam = _wParam & MK_XBUTTON1 ? VK_XBUTTON1 : VK_XBUTTON2;
				}

				if (_Msg == WM_MBUTTONDOWN)
				{
					Msg = WM_KEYDOWN;
					wParam = VK_MBUTTON;
				}

				if (SetInfoObjDebugVal && (Msg == WM_KEYUP || Msg == WM_KEYDOWN))
				{
					PrintText("ProcessShopHelper...");
				}


				if (ProcessShopHelper(hWnd, Msg, wParam, lParam))
				{
					return DefWindowProc(hWnd, Msg, wParam, lParam);
				}
				if (SetInfoObjDebugVal && (Msg == WM_KEYUP || Msg == WM_KEYDOWN))
				{
					PrintText("ProcessChatHotkeys...");
				}


				if (ProcessChatHotkeys(hWnd, Msg, wParam, lParam, _IsAltPressed, _IsCtrlPressed, _IsShiftPressed, true) ||
					ProcessChatHotkeys(hWnd, Msg, wParam, lParam, _IsAltPressed, _IsCtrlPressed, _IsShiftPressed, false))
				{
					return DefWindowProc(hWnd, Msg, wParam, lParam);
				}
				if (SetInfoObjDebugVal && (Msg == WM_KEYUP || Msg == WM_KEYDOWN))
				{
					PrintText("ProcessHotkeys...");
				}

				if (ProcessHotkeys(hWnd, Msg, wParam, lParam, _IsAltPressed, _IsCtrlPressed, _IsShiftPressed, itempressed, ClickHelperWork, true) ||
					ProcessHotkeys(hWnd, Msg, wParam, lParam, _IsAltPressed, _IsCtrlPressed, _IsShiftPressed, itempressed, ClickHelperWork, false))
				{
					return DefWindowProc(hWnd, Msg, wParam, lParam);
				}

				if (SetInfoObjDebugVal && (Msg == WM_KEYUP || Msg == WM_KEYDOWN))
				{
					PrintText("ProcessCallbackHotkeys...");
				}

				if (ProcessCallbackHotkeys(hWnd, Msg, wParam, lParam, _IsAltPressed, _IsCtrlPressed, _IsShiftPressed, true) ||
					ProcessCallbackHotkeys(hWnd, Msg, wParam, lParam, _IsAltPressed, _IsCtrlPressed, _IsShiftPressed, false))
				{
					return DefWindowProc(hWnd, Msg, wParam, lParam);
				}
				if (SetInfoObjDebugVal && (Msg == WM_KEYUP || Msg == WM_KEYDOWN))
				{
					PrintText("ProcessSelectActionHotkeys...");
				}

				if (ProcessSelectActionHotkeys(hWnd, Msg, wParam, lParam, _IsAltPressed, _IsCtrlPressed, _IsShiftPressed, true) ||
					ProcessSelectActionHotkeys(hWnd, Msg, wParam, lParam, _IsAltPressed, _IsCtrlPressed, _IsShiftPressed, false))
				{
					return DefWindowProc(hWnd, Msg, wParam, lParam);
				}

				if (SetInfoObjDebugVal && (Msg == WM_KEYUP || Msg == WM_KEYDOWN))
				{
					PrintText("ProcessSelectActionHotkeys ok");
				}

				for (int& keyCode : BlockedKeyCodes)
				{
					if (keyCode == (int)wParam)
					{
						return DefWindowProc(hWnd, Msg, wParam, lParam);
					}

				}

				Msg = _Msg;
				wParam = _wParam;



				if ((wParam >= 0x41 && wParam <= 0x5A) ||
					(wParam >= VK_NUMPAD1 && wParam <= VK_NUMPAD8))
				{


					int selectedunits = GetSelectedUnitCountBigger(GetLocalPlayerId());
					int unitowner = selectedunits > 0 ? GetUnitOwnerSlot(GetSelectedUnit(GetLocalPlayerId())) : 0;


					if (selectedunits == 1)
					{
						if (EnableSelectHelper)
						{
							if (selectedunits == 0 ||
								(unitowner != GetLocalPlayerId() && !GetPlayerAlliance(Player(unitowner), Player(GetLocalPlayerId()), 6)))
							{

								WarcraftRealWNDProc_ptr(hWnd, WM_KEYDOWN, VK_F1, lpF1ScanKeyDOWN);
								WarcraftRealWNDProc_ptr(hWnd, WM_KEYUP, VK_F1, lpF1ScanKeyUP);



								DelayedPress tmpDelayPress;
								tmpDelayPress.NeedPresslParam = lParam;
								tmpDelayPress.NeedPresswParam = wParam;
								tmpDelayPress.NeedPressMsg = 0;
								tmpDelayPress.TimeOut = 60;
								DelayedPressList_pushback(tmpDelayPress);

								return WarcraftRealWNDProc_ptr(hWnd, Msg, wParam, lParam);
							}
						}
					}


					if (!ClickHelperWork && DoubleClickHelper)
					{
						if (GetTickCount() - LastPressedKeysTime[wParam] < 450 && ((wParam == LatestPressedKey && LatestButtonClickTime + 500 > GetTickCount()) || wParam != LatestPressedKey))
						{
							itempressed = itempressed || (wParam >= VK_NUMPAD1 && wParam <= VK_NUMPAD8);

							if (IsCursorSelectTarget())
							{
								if (PressMouseAtSelectedHero(itempressed) == 0)
								{
									LastPressedKeysTime[wParam] = 0;
									if (wParam >= VK_NUMPAD1 && wParam <= VK_NUMPAD8)
									{
										LastPressedKeysTime[wParam] = 0;
										return DefWindowProc(hWnd, Msg, wParam, lParam);
									}
								}

							}
						}
						else
						{
							LastPressedKeysTime[wParam] = GetTickCount();
						}

					}

				}
			}

			if (Msg == WM_LBUTTONDOWN)
			{
				oldlParam = lParam;
			}


			if (Msg == WM_RBUTTONDOWN)
			{
				if (EnableSelectHelper)
				{
					int selectedunits = GetSelectedUnitCountBigger(GetLocalPlayerId());
					int unitowner = selectedunits > 0 ? GetUnitOwnerSlot(GetSelectedUnit(GetLocalPlayerId())) : 0;


					if (selectedunits == 0 ||
						(unitowner != GetLocalPlayerId() && (unitowner == 15 || !GetPlayerAlliance(Player(unitowner), Player(GetLocalPlayerId()), 6)))
						)
					{
						//PressHeroPanelButton( 0, false );
						WarcraftRealWNDProc_ptr(hWnd, WM_KEYDOWN, VK_F1, lpF1ScanKeyDOWN);
						WarcraftRealWNDProc_ptr(hWnd, WM_KEYUP, VK_F1, lpF1ScanKeyUP);

					}
				}
			}
		}

		if (Msg == WM_KEYDOWN)
		{
			if (SetInfoObjDebugVal)
			{
				PrintText("end 1  ");
			}
		}
	}

	else
	{

		// Process RawImages. Mouse up and leave;

		if (GlobalRawImageCallbackData)
			RawImageGlobalCallbackFunc(RawImageEventType::ALL, 0.0f, 0.0f);

		if (LOCK_MOUSE_IN_WINDOW)
			ClipCursor(0);

		if (BlockKeyAndMouseEmulation)
		{
			if (Msg == WM_RBUTTONDOWN || Msg == WM_KEYDOWN || Msg == WM_KEYUP)
			{
				return DefWindowProc(hWnd, Msg, wParam, lParam);
			}
		}
	}


	return WarcraftRealWNDProc_ptr(hWnd, Msg, wParam, lParam);
}



int __stdcall ToggleBlockKeyAndMouseEmulation(int enable)
{
	if (DEBUG_FULL)
		std::cout << __func__ << std::endl;
	BlockKeyAndMouseEmulation = enable;
	return 0;
}


int __stdcall ToggleForcedSubSelection(int enable)
{
	if (DEBUG_FULL)
		std::cout << __func__ << std::endl;
	EnableSelectHelper = enable;
	return 0;
}

int __stdcall ToggleClickHelper(int enable)
{
	if (DEBUG_FULL)
		std::cout << __func__ << std::endl;
	DoubleClickHelper = enable;
	return 0;
}


#pragma region Фикс шифта для приказов

typedef int(__stdcall* IssueWithoutTargetOrder)(int a1, int a2, unsigned int a3, unsigned int a4);
typedef int(__stdcall* IssueTargetOrPointOrder2)(int a1, int a2, float a3, float a4, int a5, int a6);
typedef int(__stdcall* sub_6F339D50)(int a1, int a2, int a3, unsigned int a4, unsigned int a5);
typedef int(__stdcall* IssueTargetOrPointOrder)(int a1, int a2, float a3, float a4, int a5, int a6, int a7);
typedef int(__stdcall* sub_6F339E60)(int a1, int a2, float a3, float a4, int a5, int a6, int a7, int a8);
typedef int(__stdcall* sub_6F339F00)(int a1, int a2, int a3, unsigned int a4, unsigned int a5);
typedef int(__stdcall* sub_6F339F80)(int a1, int a2, float a3, float a4, int a5, int a6, int a7);
typedef int(__stdcall* sub_6F33A010)(int a1, int a2, float a3, float a4, int a5, int a6, int a7, int a8);


IssueWithoutTargetOrder IssueWithoutTargetOrderorg;
IssueWithoutTargetOrder IssueWithoutTargetOrderptr;

IssueTargetOrPointOrder2 IssueTargetOrPointOrder2org;
IssueTargetOrPointOrder2 IssueTargetOrPointOrder2ptr;

sub_6F339D50 sub_6F339D50org;
sub_6F339D50 sub_6F339D50ptr;

IssueTargetOrPointOrder IssueTargetOrPointOrderorg;
IssueTargetOrPointOrder IssueTargetOrPointOrderptr;

sub_6F339E60 sub_6F339E60org;
sub_6F339E60 sub_6F339E60ptr;

sub_6F339F00 sub_6F339F00org;
sub_6F339F00 sub_6F339F00ptr;

sub_6F339F80 sub_6F339F80org;
sub_6F339F80 sub_6F339F80ptr;

sub_6F33A010 sub_6F33A010org;
sub_6F33A010 sub_6F33A010ptr;


int __stdcall IssueWithoutTargetOrdermy(int a1, int a2, unsigned int a3, unsigned int a4)
{
	if (DEBUG_FULL)
		std::cout << __func__ << std::endl;

	if (a4 & ShiftPressed)
	{
		if (GetTickCount() - SkipSingleShift < 100)
		{
			SkipSingleShift = 0;
			a4 -= ShiftPressed;
		}
	}
	else
	{
		if (ShiftPressed)
		{
			a4 += ShiftPressed;
		}
	}


	int retvalue = IssueWithoutTargetOrderptr(a1, a2, a3, a4);

	if (GetTickCount() - SingleShift < 100)
	{
		SingleShift = 0;
		ShiftPressed = 0;
	}

	return retvalue;
}
int __stdcall IssueTargetOrPointOrder2my(int a1, int a2, float a3, float a4, int a5, int a6)
{
	if (DEBUG_FULL)
		std::cout << __func__ << std::endl;
	if (a6 & ShiftPressed)
	{
		if (GetTickCount() - SkipSingleShift < 100)
		{
			SkipSingleShift = 0;
			a6 -= ShiftPressed;
		}
	}
	else
	{
		if (ShiftPressed)
		{
			a6 += ShiftPressed;
		}
	}
	int retvalue = IssueTargetOrPointOrder2ptr(a1, a2, a3, a4, a5, a6);
	if (GetTickCount() - SingleShift < 100)
	{
		SingleShift = 0;
		ShiftPressed = 0;
	}
	return retvalue;
}
int __stdcall sub_6F339D50my(int a1, int a2, int a3, unsigned int a4, unsigned int a5)
{
	if (DEBUG_FULL)
		std::cout << __func__ << std::endl;
	if (a5 & ShiftPressed)
	{
		if (GetTickCount() - SkipSingleShift < 100)
		{
			SkipSingleShift = 0;
			a5 -= ShiftPressed;
		}
	}
	else
	{
		if (ShiftPressed)
		{
			a5 += ShiftPressed;
		}
	}
	int retvalue = sub_6F339D50ptr(a1, a2, a3, a4, a5);
	if (GetTickCount() - SingleShift < 100)
	{
		SingleShift = 0;
		ShiftPressed = 0;
	}
	return retvalue;
}
int __stdcall IssueTargetOrPointOrdermy(int a1, int a2, float a3, float a4, int a5, int a6, int a7)
{
	if (DEBUG_FULL)
		std::cout << __func__ << std::endl;
	if (a7 & ShiftPressed)
	{
		if (GetTickCount() - SkipSingleShift < 100)
		{
			SkipSingleShift = 0;
			a7 -= ShiftPressed;
		}
	}
	else
	{
		if (ShiftPressed)
		{
			a7 += ShiftPressed;
		}
	}

	int retvalue = IssueTargetOrPointOrderptr(a1, a2, a3, a4, a5, a6, a7);
	if (GetTickCount() - SingleShift < 100)
	{
		SingleShift = 0;
		ShiftPressed = 0;
	}
	return retvalue;
}
int __stdcall sub_6F339E60my(int a1, int a2, float a3, float a4, int a5, int a6, int a7, int a8)
{
	if (DEBUG_FULL)
		std::cout << __func__ << std::endl;
	if (a8 & ShiftPressed)
	{
		if (GetTickCount() - SkipSingleShift < 100)
		{
			SkipSingleShift = 0;
			a8 -= ShiftPressed;
		}
	}
	else
	{
		if (ShiftPressed)
		{
			a8 += ShiftPressed;
		}
	}
	int retvalue = sub_6F339E60ptr(a1, a2, a3, a4, a5, a6, a7, a8);
	if (GetTickCount() - SingleShift < 100)
	{
		SingleShift = 0;
		ShiftPressed = 0;
	}
	return retvalue;
}
int __stdcall sub_6F339F00my(int a1, int a2, int a3, unsigned int a4, unsigned int a5)
{
	if (DEBUG_FULL)
		std::cout << __func__ << std::endl;
	if (a5 & ShiftPressed)
	{
		if (GetTickCount() - SkipSingleShift < 100)
		{
			SkipSingleShift = 0;
			a5 -= ShiftPressed;
		}
	}
	else
	{
		if (ShiftPressed)
		{
			a5 += ShiftPressed;
		}
	}
	int retvalue = sub_6F339F00ptr(a1, a2, a3, a4, a5);
	if (GetTickCount() - SingleShift < 100)
	{
		SingleShift = 0;
		ShiftPressed = 0;
	}
	return retvalue;
}
int __stdcall sub_6F339F80my(int a1, int a2, float a3, float a4, int a5, int a6, int a7)
{
	if (DEBUG_FULL)
		std::cout << __func__ << std::endl;
	if (a7 & ShiftPressed)
	{
		if (GetTickCount() - SkipSingleShift < 100)
		{
			SkipSingleShift = 0;
			a7 -= ShiftPressed;
		}
	}
	else
	{
		if (ShiftPressed)
		{
			a7 += ShiftPressed;
		}
	}
	int retvalue = sub_6F339F80ptr(a1, a2, a3, a4, a5, a6, a7);
	if (GetTickCount() - SingleShift < 100)
	{
		SingleShift = 0;
		ShiftPressed = 0;
	}
	return retvalue;
}
int __stdcall sub_6F33A010my(int a1, int a2, float a3, float a4, int a5, int a6, int a7, int a8)
{
	if (DEBUG_FULL)
		std::cout << __func__ << std::endl;
	if (a8 & ShiftPressed)
	{
		if (GetTickCount() - SkipSingleShift < 100)
		{
			SkipSingleShift = 0;
			a8 -= ShiftPressed;
		}
	}
	else
	{
		if (ShiftPressed)
		{
			a8 += ShiftPressed;
		}
	}
	int retvalue = sub_6F33A010ptr(a1, a2, a3, a4, a5, a6, a7, a8);
	if (GetTickCount() - SingleShift < 100)
	{
		SingleShift = 0;
		ShiftPressed = 0;
	}
	return retvalue;
}

int IssueWithoutTargetOrderOffset = 0;
int IssueTargetOrPointOrder2Offset = 0;
int sub_6F339D50Offset = 0;
int IssueTargetOrPointOrderOffset = 0;
int sub_6F339E60Offset = 0;
int sub_6F339F00Offset = 0;
int sub_6F339F80Offset = 0;
int sub_6F33A010Offset = 0;

#pragma endregion

// Включить фикс шифта для приказов
void IssueFixerInit()
{
	IssueWithoutTargetOrderorg = (IssueWithoutTargetOrder)(GameDll + IssueWithoutTargetOrderOffset);
	MH_CreateHook(IssueWithoutTargetOrderorg, &IssueWithoutTargetOrdermy, reinterpret_cast<void**>(&IssueWithoutTargetOrderptr));

	IssueTargetOrPointOrder2org = (IssueTargetOrPointOrder2)(GameDll + IssueTargetOrPointOrder2Offset);
	MH_CreateHook(IssueTargetOrPointOrder2org, &IssueTargetOrPointOrder2my, reinterpret_cast<void**>(&IssueTargetOrPointOrder2ptr));

	sub_6F339D50org = (sub_6F339D50)(GameDll + sub_6F339D50Offset);
	MH_CreateHook(sub_6F339D50org, &sub_6F339D50my, reinterpret_cast<void**>(&sub_6F339D50ptr));

	IssueTargetOrPointOrderorg = (IssueTargetOrPointOrder)(GameDll + IssueTargetOrPointOrderOffset);
	MH_CreateHook(IssueTargetOrPointOrderorg, &IssueTargetOrPointOrdermy, reinterpret_cast<void**>(&IssueTargetOrPointOrderptr));

	sub_6F339E60org = (sub_6F339E60)(GameDll + sub_6F339E60Offset);
	MH_CreateHook(sub_6F339E60org, &sub_6F339E60my, reinterpret_cast<void**>(&sub_6F339E60ptr));

	sub_6F339F00org = (sub_6F339F00)(GameDll + sub_6F339F00Offset);
	MH_CreateHook(sub_6F339F00org, &sub_6F339F00my, reinterpret_cast<void**>(&sub_6F339F00ptr));

	sub_6F339F80org = (sub_6F339F80)(GameDll + sub_6F339F80Offset);
	MH_CreateHook(sub_6F339F80org, &sub_6F339F80my, reinterpret_cast<void**>(&sub_6F339F80ptr));

	sub_6F33A010org = (sub_6F33A010)(GameDll + sub_6F33A010Offset);
	MH_CreateHook(sub_6F33A010org, &sub_6F33A010my, reinterpret_cast<void**>(&sub_6F33A010ptr));



	MH_EnableHook(IssueWithoutTargetOrderorg);
	MH_EnableHook(IssueTargetOrPointOrder2org);
	MH_EnableHook(sub_6F339D50org);
	MH_EnableHook(IssueTargetOrPointOrderorg);
	MH_EnableHook(sub_6F339E60org);
	MH_EnableHook(sub_6F339F00org);
	MH_EnableHook(sub_6F339F80org);
	MH_EnableHook(sub_6F33A010org);


	GetCameraHeight_org = pGetCameraHeight(GameDll + 0x3019A0);
	MH_CreateHook(GetCameraHeight_org, &GetCameraHeight_my, reinterpret_cast<void**>(&GetCameraHeight_ptr));
	MH_EnableHook(GetCameraHeight_org);

}

// Отключить фикс шифта для приказов
void IssueFixerDisable()
{

	memset(LastPressedKeysTime, 0, sizeof(LastPressedKeysTime));

	if (IssueWithoutTargetOrderorg)
	{
		MH_DisableHook(IssueWithoutTargetOrderorg);
		IssueWithoutTargetOrderorg = NULL;
	}
	if (IssueTargetOrPointOrder2org)
	{
		MH_DisableHook(IssueTargetOrPointOrder2org);
		IssueTargetOrPointOrder2org = NULL;
	}
	if (sub_6F339D50org)
	{
		MH_DisableHook(sub_6F339D50org);
		sub_6F339D50org = NULL;
	}
	if (IssueTargetOrPointOrderorg)
	{
		MH_DisableHook(IssueTargetOrPointOrderorg);
		IssueTargetOrPointOrderorg = NULL;
	}
	if (sub_6F339E60org)
	{
		MH_DisableHook(sub_6F339E60org);
		sub_6F339E60org = NULL;
	}
	if (sub_6F339F00org)
	{
		MH_DisableHook(sub_6F339F00org);
		sub_6F339F00org = NULL;
	}
	if (sub_6F339F80org)
	{
		MH_DisableHook(sub_6F339F80org);
		sub_6F339F80org = NULL;
	}
	if (sub_6F33A010org)
	{
		MH_DisableHook(sub_6F33A010org);
		sub_6F33A010org = NULL;
	}

	if (GetCameraHeight_org)
	{
		MH_DisableHook(GetCameraHeight_org);
		GetCameraHeight_org = NULL;
	}

	if (!RegisteredKeyCodes.empty())
		RegisteredKeyCodes.clear();

	if (!BlockedKeyCodes.empty())
		BlockedKeyCodes.clear();

	if (!KeyActionList.empty())
		KeyActionList.clear();

	if (!KeyChatActionList.empty())
		KeyChatActionList.clear();

	if (!KeySelectActionList.empty())
		KeySelectActionList.clear();

	if (!KeyCalbackActionList.empty())
		KeyCalbackActionList.clear();

	GlobalCallbackForRegisteredHotkeys = jRCString();
	SkipAllMessages = false;
}

// построение текущих нажатых кнопок в код клавиш
unsigned int BuildKeyCode()
{
	unsigned int code = 0;

	if (IsKeyPressed(VK_LSHIFT) ||
		IsKeyPressed(VK_RSHIFT) ||
		IsKeyPressed(VK_SHIFT))
	{
		code += 0x40000;
	}
	else if (IsKeyPressed(VK_MENU) ||
		IsKeyPressed(VK_RMENU) ||
		IsKeyPressed(VK_LMENU))
	{
		code += 0x10000;
	}
	else if (IsKeyPressed(VK_LCONTROL) ||
		IsKeyPressed(VK_RCONTROL) ||
		IsKeyPressed(VK_CONTROL))
	{
		code += 0x20000;
	}

	for (int i = 0; i < 255; i++)
	{
		if (i == (int)VK_LSHIFT ||
			i == (int)VK_RSHIFT ||
			i == (int)VK_SHIFT ||
			i == (int)VK_MENU ||
			i == (int)VK_RMENU ||
			i == (int)VK_LMENU ||
			i == (int)VK_LCONTROL ||
			i == (int)VK_RCONTROL ||
			i == (int)VK_CONTROL)
			continue;

		short x = GetAsyncKeyState(i);
		if ((x & 0x8000) > 0)
		{
			code += i;
			break;
		}

	}

	return code;

}

// конвертировать строку в код клавиши
int _StrToVKey(const std::string& skey) {
	if (skey == "LBTN") return VK_LBUTTON; // Left mouse button
	if (skey == "RBTN") return VK_RBUTTON; // Right mouse button
	if (skey == "CANCEL") return VK_CANCEL; // Control-break processing
	if (skey == "MBTN") return VK_MBUTTON; // Middle mouse button (three-button mouse)
	if (skey == "XBTN1") return VK_XBUTTON1; // X1 mouse button
	if (skey == "XBTN2") return VK_XBUTTON2; // X2 mouse button
	if (skey == "BACK") return VK_BACK; // BACKSPACE key
	if (skey == "TAB") return VK_TAB; // TAB key
	if (skey == "CLEAR") return VK_CLEAR; // CLEAR key
	if (skey == "RETURN") return VK_RETURN; // ENTER key
	if (skey == "SHIFT") return VK_SHIFT; // SHIFT key
	if (skey == "CTRL") return VK_CONTROL; // CTRL key
	if (skey == "ALT") return VK_MENU; // ALT key
	if (skey == "PAUSE") return VK_PAUSE; // PAUSE key
	if (skey == "CAPS") return VK_CAPITAL; // CAPS LOCK key
	if (skey == "KANA") return VK_KANA; // IME Kana mode
	//if (skey == "VK_HANGUEL") return VK_HANGUL; // IME Hangul mode
	if (skey == "JUNJA") return VK_JUNJA; // IME Junja mode
	if (skey == "FINAL") return VK_FINAL; // IME final mode
	if (skey == "HANJA") return VK_HANJA; // IME Hanja mode
	//if (skey == "VK_KANJI") return VK_KANJI; // IME Kanji mode
	if (skey == "ESC") return VK_ESCAPE; // ESC key
	if (skey == "CONV") return VK_CONVERT; // IME convert
	if (skey == "NCONV") return VK_NONCONVERT; // IME nonconvert
	if (skey == "ACCEPT") return VK_ACCEPT; // IME accept
	if (skey == "MCHANGE") return VK_MODECHANGE; // IME mode change request
	if (skey == "SPACE") return VK_SPACE; // SPACEBAR
	if (skey == "PAGEUP") return VK_PRIOR; // PAGE UP key
	if (skey == "PAGEDN") return VK_NEXT; // PAGE DOWN key
	if (skey == "END") return VK_END; // END key
	if (skey == "HOME") return VK_HOME; // HOME key
	if (skey == "LEFT") return VK_LEFT; // LEFT ARROW key
	if (skey == "UP") return VK_UP; // UP ARROW key
	if (skey == "RIGHT") return VK_RIGHT; // RIGHT ARROW key
	if (skey == "DOWN") return VK_DOWN; // DOWN ARROW key
	if (skey == "SELECT") return VK_SELECT; // SELECT key
	if (skey == "PRINT") return VK_PRINT; // PRINT key
	if (skey == "EXEC") return VK_EXECUTE; // EXECUTE key
	if (skey == "SSHOT") return VK_SNAPSHOT; // PRINT SCREEN key
	if (skey == "INSERT") return VK_INSERT; // INS key
	if (skey == "DELETE") return VK_DELETE; // DEL key
	if (skey == "HELP") return VK_HELP; // HELP key

	if (skey == "0") return '0';
	if (skey == "1") return '1';
	if (skey == "2") return '2';
	if (skey == "3") return '3';
	if (skey == "4") return '4';
	if (skey == "5") return '5';
	if (skey == "6") return '6';
	if (skey == "7") return '7';
	if (skey == "8") return '8';
	if (skey == "9") return '9';
	if (skey == "A") return 'A';
	if (skey == "B") return 'B';
	if (skey == "C") return 'C';
	if (skey == "D") return 'D';
	if (skey == "E") return 'E';
	if (skey == "F") return 'F';
	if (skey == "G") return 'G';
	if (skey == "H") return 'H';
	if (skey == "I") return 'I';
	if (skey == "J") return 'J';
	if (skey == "K") return 'K';
	if (skey == "L") return 'L';
	if (skey == "M") return 'M';
	if (skey == "N") return 'N';
	if (skey == "O") return 'O';
	if (skey == "P") return 'P';
	if (skey == "Q") return 'Q';
	if (skey == "R") return 'R';
	if (skey == "S") return 'S';
	if (skey == "T") return 'T';
	if (skey == "U") return 'U';
	if (skey == "V") return 'V';
	if (skey == "W") return 'W';
	if (skey == "X") return 'X';
	if (skey == "Y") return 'Y';
	if (skey == "Z") return 'Z';

	if (skey == "LWIN") return VK_LWIN; // Left Windows key (Natural keyboard)
	if (skey == "RWIN") return VK_RWIN; // Right Windows key (Natural keyboard)
	if (skey == "APPS") return VK_APPS; // Applications key (Natural keyboard)
	if (skey == "SLEEP") return VK_SLEEP; // Computer Sleep key
	if (skey == "NPAD0") return VK_NUMPAD0; // Numeric keypad 0 key
	if (skey == "NPAD1") return VK_NUMPAD1; // Numeric keypad 1 key
	if (skey == "NPAD2") return VK_NUMPAD2; // Numeric keypad 2 key
	if (skey == "NPAD3") return VK_NUMPAD3; // Numeric keypad 3 key
	if (skey == "NPAD4") return VK_NUMPAD4; // Numeric keypad 4 key
	if (skey == "NPAD5") return VK_NUMPAD5; // Numeric keypad 5 key
	if (skey == "NPAD6") return VK_NUMPAD6; // Numeric keypad 6 key
	if (skey == "NPAD7") return VK_NUMPAD7; // Numeric keypad 7 key
	if (skey == "NPAD8") return VK_NUMPAD8; // Numeric keypad 8 key
	if (skey == "NPAD9") return VK_NUMPAD9; // Numeric keypad 9 key
	if (skey == "MULT") return VK_MULTIPLY; // Multiply key
	if (skey == "ADD") return VK_ADD; // Add key
	if (skey == "SEP") return VK_SEPARATOR; // Separator key
	if (skey == "SUB") return VK_SUBTRACT; // Subtract key
	if (skey == "DEC") return VK_DECIMAL; // Decimal key
	if (skey == "DIV") return VK_DIVIDE; // Divide key
	if (skey == "F1") return VK_F1; // F1 key
	if (skey == "F2") return VK_F2; // F2 key
	if (skey == "F3") return VK_F3; // F3 key
	if (skey == "F4") return VK_F4; // F4 key
	if (skey == "F5") return VK_F5; // F5 key
	if (skey == "F6") return VK_F6; // F6 key
	if (skey == "F7") return VK_F7; // F7 key
	if (skey == "F8") return VK_F8; // F8 key
	if (skey == "F9") return VK_F9; // F9 key
	if (skey == "F10") return VK_F10; // F10 key
	if (skey == "F11") return VK_F11; // F11 key
	if (skey == "F12") return VK_F12; // F12 key
	if (skey == "F13") return VK_F13; // F13 key
	if (skey == "F14") return VK_F14; // F14 key
	if (skey == "F15") return VK_F15; // F15 key
	if (skey == "F16") return VK_F16; // F16 key
	if (skey == "F17") return VK_F17; // F17 key
	if (skey == "F18") return VK_F18; // F18 key
	if (skey == "F19") return VK_F19; // F19 key
	if (skey == "F20") return VK_F20; // F20 key
	if (skey == "F21") return VK_F21; // F21 key
	if (skey == "F22") return VK_F22; // F22 key
	if (skey == "F23") return VK_F23; // F23 key
	if (skey == "F24") return VK_F24; // F24 key
	if (skey == "NLOCK") return VK_NUMLOCK; // NUM LOCK key
	if (skey == "SCRL") return VK_SCROLL; // SCROLL LOCK key
	if (skey == "LSHFT") return VK_LSHIFT; // Left SHIFT key
	if (skey == "RSHFT") return VK_RSHIFT; // Right SHIFT key
	if (skey == "LCTRL") return VK_LCONTROL; // Left CONTROL key
	if (skey == "RCTRL") return VK_RCONTROL; // Right CONTROL key
	if (skey == "LALT") return VK_LMENU; // Left MENU key
	if (skey == "RALT") return VK_RMENU; // Right MENU key
	if (skey == "BBACK") return VK_BROWSER_BACK; // Browser Back key
	if (skey == "BFORW") return VK_BROWSER_FORWARD; // Browser Forward key
	if (skey == "BREFR") return VK_BROWSER_REFRESH; // Browser Refresh key
	if (skey == "BSTOP") return VK_BROWSER_STOP; // Browser Stop key
	if (skey == "BSEARCH") return VK_BROWSER_SEARCH; // Browser Search key
	if (skey == "BFAV") return VK_BROWSER_FAVORITES; // Browser Favorites key
	if (skey == "BHOME") return VK_BROWSER_HOME; // Browser Start and Home key
	if (skey == "MUTE") return VK_VOLUME_MUTE; // Volume Mute key
	if (skey == "V_DOWN") return VK_VOLUME_DOWN; // Volume Down key
	if (skey == "V_UP") return VK_VOLUME_UP; // Volume Up key
	if (skey == "NEXT") return VK_MEDIA_NEXT_TRACK; // Next Track key
	if (skey == "PREV") return VK_MEDIA_PREV_TRACK; // Previous Track key
	if (skey == "STOP") return VK_MEDIA_STOP; // Stop Media key
	if (skey == "MPLAY") return VK_MEDIA_PLAY_PAUSE; // Play/Pause Media key
	if (skey == "MAIL") return VK_LAUNCH_MAIL; // Start Mail key
	if (skey == "MSEL") return VK_LAUNCH_MEDIA_SELECT; // Select Media key
	if (skey == "APP1") return VK_LAUNCH_APP1; // Start Application 1 key
	if (skey == "APP2") return VK_LAUNCH_APP2; // Start Application 2 key
	if (skey == "OEM_1") return VK_OEM_1; // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ';:' key
	if (skey == "OEM_P") return VK_OEM_PLUS; // For any country/region, the '+' key
	if (skey == "COMMA") return VK_OEM_COMMA; // For any country/region, the ',' key
	if (skey == "MINUS") return VK_OEM_MINUS; // For any country/region, the '-' key
	if (skey == "PERIOD") return VK_OEM_PERIOD; // For any country/region, the '.' key
	if (skey == "OEM_2") return VK_OEM_2; // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '/?' key
	if (skey == "OEM_3") return VK_OEM_3; // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '`~' key
	if (skey == "ABNT_C1") return 0xC1; // Brazilian (ABNT) Keyboard
	if (skey == "ABNT_C2") return 0xC2; // Brazilian (ABNT) Keyboard
	if (skey == "OEM_4") return VK_OEM_4; // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '[{' key
	if (skey == "OEM_5") return VK_OEM_5; // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '\|' key
	if (skey == "OEM_6") return VK_OEM_6; // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ']}' key
	if (skey == "OEM_7") return VK_OEM_7; // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the 'single-quote/double-quote' key
	if (skey == "OEM_8") return VK_OEM_8; // Used for miscellaneous characters; it can vary by keyboard.
	if (skey == "OEM102") return VK_OEM_102; // Either the angle bracket key or the backslash key on the RT 102-key keyboard
	if (skey == "PROCKEY") return VK_PROCESSKEY; // IME PROCESS key
	if (skey == "PACKET") return VK_PACKET; // Used to pass Unicode characters as if they were keystrokes. The VK_PACKET key is the low word of a 32-bit Virtual Key value used for non-keyboard input methods. For more information, see Remark in KEYBDINPUT, SendInput, WM_KEYDOWN, and WM_KEYUP // 0xE8
	if (skey == "ATTN") return VK_ATTN; // Attn key
	if (skey == "CRSEL") return VK_CRSEL; // CrSel key
	if (skey == "EXSEL") return VK_EXSEL; // ExSel key
	if (skey == "EREOF") return VK_EREOF; // Erase EOF key
	if (skey == "PLAY") return VK_PLAY; // Play key
	if (skey == "ZOOM") return VK_ZOOM; // Zoom key
	if (skey == "NONAME") return VK_NONAME; // Reserved
	if (skey == "PA1") return VK_PA1; // PA1 key
	if (skey == "OCLEAR") return VK_OEM_CLEAR; // Clear key

	return 0;
}

// конвертировать код клавиши в строку
std::string _VKeyToStr(int vkey) {
	switch (vkey) {
	case VK_LBUTTON: return "LBTN"; // Left mouse button
	case VK_RBUTTON: return "RBTN"; // Right mouse button
	case VK_CANCEL: return "CANCEL"; // Control-break processing
	case VK_MBUTTON: return "MBTN"; // Middle mouse button (three-button mouse)
	case VK_XBUTTON1: return "XBTN1"; // X1 mouse button
	case VK_XBUTTON2: return "XBTN2"; // X2 mouse button
	case VK_BACK: return "BACK"; // BACKSPACE key
	case VK_TAB: return "TAB"; // TAB key
	case VK_CLEAR: return "CLEAR"; // CLEAR key
	case VK_RETURN: return "RETURN"; // ENTER key
	case VK_SHIFT: return "SHIFT"; // SHIFT key
	case VK_CONTROL: return "CTRL"; // CTRL key
	case VK_MENU: return "ALT"; // ALT key
	case VK_PAUSE: return "PAUSE"; // PAUSE key
	case VK_CAPITAL: return "CAPS"; // CAPS LOCK key
	case VK_KANA: return "KANA"; // IME Kana mode
		//case VK_HANGUL: return "VK_HANGUEL"; // IME Hangul mode
	case VK_JUNJA: return "JUNJA"; // IME Junja mode
	case VK_FINAL: return "FINAL"; // IME final mode
	case VK_HANJA: return "HANJA"; // IME Hanja mode
		//case VK_KANJI: return "VK_KANJI"; // IME Kanji mode
	case VK_ESCAPE: return "ESC"; // ESC key
	case VK_CONVERT: return "CONV"; // IME convert
	case VK_NONCONVERT: return "NCONV"; // IME nonconvert
	case VK_ACCEPT: return "ACCEPT"; // IME accept
	case VK_MODECHANGE: return "MCHANGE"; // IME mode change request
	case VK_SPACE: return "SPACE"; // SPACEBAR
	case VK_PRIOR: return "PAGEUP"; // PAGE UP key
	case VK_NEXT: return "PAGEDN"; // PAGE DOWN key
	case VK_END: return "END"; // END key
	case VK_HOME: return "HOME"; // HOME key
	case VK_LEFT: return "LEFT"; // LEFT ARROW key
	case VK_UP: return "UP"; // UP ARROW key
	case VK_RIGHT: return "RIGHT"; // RIGHT ARROW key
	case VK_DOWN: return "DOWN"; // DOWN ARROW key
	case VK_SELECT: return "SELECT"; // SELECT key
	case VK_PRINT: return "PRINT"; // PRINT key
	case VK_EXECUTE: return "EXEC"; // EXECUTE key
	case VK_SNAPSHOT: return "SSHOT"; // PRINT SCREEN key
	case VK_INSERT: return "INSERT"; // INS key
	case VK_DELETE: return "DELETE"; // DEL key
	case VK_HELP: return "HELP"; // HELP key

	case '0': return "0";
	case '1': return "1";
	case '2': return "2";
	case '3': return "3";
	case '4': return "4";
	case '5': return "5";
	case '6': return "6";
	case '7': return "7";
	case '8': return "8";
	case '9': return "9";
	case 'A': return "A";
	case 'B': return "B";
	case 'C': return "C";
	case 'D': return "D";
	case 'E': return "E";
	case 'F': return "F";
	case 'G': return "G";
	case 'H': return "H";
	case 'I': return "I";
	case 'J': return "J";
	case 'K': return "K";
	case 'L': return "L";
	case 'M': return "M";
	case 'N': return "N";
	case 'O': return "O";
	case 'P': return "P";
	case 'Q': return "Q";
	case 'R': return "R";
	case 'S': return "S";
	case 'T': return "T";
	case 'U': return "U";
	case 'V': return "V";
	case 'W': return "W";
	case 'X': return "X";
	case 'Y': return "Y";
	case 'Z': return "Z";

	case VK_LWIN: return "LWIN"; // Left Windows key (Natural keyboard)
	case VK_RWIN: return "RWIN"; // Right Windows key (Natural keyboard)
	case VK_APPS: return "APPS"; // Applications key (Natural keyboard)
	case VK_SLEEP: return "SLEEP"; // Computer Sleep key
	case VK_NUMPAD0: return "NPAD0"; // Numeric keypad 0 key
	case VK_NUMPAD1: return "NPAD1"; // Numeric keypad 1 key
	case VK_NUMPAD2: return "NPAD2"; // Numeric keypad 2 key
	case VK_NUMPAD3: return "NPAD3"; // Numeric keypad 3 key
	case VK_NUMPAD4: return "NPAD4"; // Numeric keypad 4 key
	case VK_NUMPAD5: return "NPAD5"; // Numeric keypad 5 key
	case VK_NUMPAD6: return "NPAD6"; // Numeric keypad 6 key
	case VK_NUMPAD7: return "NPAD7"; // Numeric keypad 7 key
	case VK_NUMPAD8: return "NPAD8"; // Numeric keypad 8 key
	case VK_NUMPAD9: return "NPAD9"; // Numeric keypad 9 key
	case VK_MULTIPLY: return "MULT"; // Multiply key
	case VK_ADD: return "ADD"; // Add key
	case VK_SEPARATOR: return "SEP"; // Separator key
	case VK_SUBTRACT: return "SUB"; // Subtract key
	case VK_DECIMAL: return "DEC"; // Decimal key
	case VK_DIVIDE: return "DIV"; // Divide key
	case VK_F1: return "F1"; // F1 key
	case VK_F2: return "F2"; // F2 key
	case VK_F3: return "F3"; // F3 key
	case VK_F4: return "F4"; // F4 key
	case VK_F5: return "F5"; // F5 key
	case VK_F6: return "F6"; // F6 key
	case VK_F7: return "F7"; // F7 key
	case VK_F8: return "F8"; // F8 key
	case VK_F9: return "F9"; // F9 key
	case VK_F10: return "F10"; // F10 key
	case VK_F11: return "F11"; // F11 key
	case VK_F12: return "F12"; // F12 key
	case VK_F13: return "F13"; // F13 key
	case VK_F14: return "F14"; // F14 key
	case VK_F15: return "F15"; // F15 key
	case VK_F16: return "F16"; // F16 key
	case VK_F17: return "F17"; // F17 key
	case VK_F18: return "F18"; // F18 key
	case VK_F19: return "F19"; // F19 key
	case VK_F20: return "F20"; // F20 key
	case VK_F21: return "F21"; // F21 key
	case VK_F22: return "F22"; // F22 key
	case VK_F23: return "F23"; // F23 key
	case VK_F24: return "F24"; // F24 key
	case VK_NUMLOCK: return "NLOCK"; // NUM LOCK key
	case VK_SCROLL: return "SCRL"; // SCROLL LOCK key
	case VK_LSHIFT: return "LSHFT"; // Left SHIFT key
	case VK_RSHIFT: return "RSHFT"; // Right SHIFT key
	case VK_LCONTROL: return "LCTRL"; // Left CONTROL key
	case VK_RCONTROL: return "RCTRL"; // Right CONTROL key
	case VK_LMENU: return "LALT"; // Left MENU key
	case VK_RMENU: return "RALT"; // Right MENU key
	case VK_BROWSER_BACK: return "BBACK"; // Browser Back key
	case VK_BROWSER_FORWARD: return "BFORW"; // Browser Forward key
	case VK_BROWSER_REFRESH: return "BREFR"; // Browser Refresh key
	case VK_BROWSER_STOP: return "BSTOP"; // Browser Stop key
	case VK_BROWSER_SEARCH: return "BSEARCH"; // Browser Search key
	case VK_BROWSER_FAVORITES: return "BFAV"; // Browser Favorites key
	case VK_BROWSER_HOME: return "BHOME"; // Browser Start and Home key
	case VK_VOLUME_MUTE: return "MUTE"; // Volume Mute key
	case VK_VOLUME_DOWN: return "V_DOWN"; // Volume Down key
	case VK_VOLUME_UP: return "V_UP"; // Volume Up key
	case VK_MEDIA_NEXT_TRACK: return "NEXT"; // Next Track key
	case VK_MEDIA_PREV_TRACK: return "PREV"; // Previous Track key
	case VK_MEDIA_STOP: return "STOP"; // Stop Media key
	case VK_MEDIA_PLAY_PAUSE: return "MPLAY"; // Play/Pause Media key
	case VK_LAUNCH_MAIL: return "MAIL"; // Start Mail key
	case VK_LAUNCH_MEDIA_SELECT: return "MSEL"; // Select Media key
	case VK_LAUNCH_APP1: return "APP1"; // Start Application 1 key
	case VK_LAUNCH_APP2: return "APP2"; // Start Application 2 key
	case VK_OEM_1: return "OEM_1"; // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ';:' key
	case VK_OEM_PLUS: return "OEM_P"; // For any country/region, the '+' key
	case VK_OEM_COMMA: return "COMMA"; // For any country/region, the ',' key
	case VK_OEM_MINUS: return "MINUS"; // For any country/region, the '-' key
	case VK_OEM_PERIOD: return "PERIOD"; // For any country/region, the '.' key
	case VK_OEM_2: return "OEM_2"; // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '/?' key
	case VK_OEM_3: return "OEM_3"; // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '`~' key
	case 0xC1: return "ABNT_C1"; // Brazilian (ABNT) Keyboard
	case 0xC2: return "ABNT_C2"; // Brazilian (ABNT) Keyboard
	case VK_OEM_4: return "OEM_4"; // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '[{' key
	case VK_OEM_5: return "OEM_5"; // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '\|' key
	case VK_OEM_6: return "OEM_6"; // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ']}' key
	case VK_OEM_7: return "OEM_7"; // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the 'single-quote/double-quote' key
	case VK_OEM_8: return "OEM_8"; // Used for miscellaneous characters; it can vary by keyboard.
	case VK_OEM_102: return "OEM102"; // Either the angle bracket key or the backslash key on the RT 102-key keyboard
	case VK_PROCESSKEY: return "PROCKEY"; // IME PROCESS key
	case VK_PACKET: return "PACKET"; // Used to pass Unicode characters as if they were keystrokes. The VK_PACKET key is the low word of a 32-bit Virtual Key value used for non-keyboard input methods. For more information, see Remark in KEYBDINPUT, SendInput, WM_KEYDOWN, and WM_KEYUP // 0xE8
	case VK_ATTN: return "ATTN"; // Attn key
	case VK_CRSEL: return "CRSEL"; // CrSel key
	case VK_EXSEL: return "EXSEL"; // ExSel key
	case VK_EREOF: return "EREOF"; // Erase EOF key
	case VK_PLAY: return "PLAY"; // Play key
	case VK_ZOOM: return "ZOOM"; // Zoom key
	case VK_NONAME: return "NONAME"; // Reserved
	case VK_PA1: return "PA1"; // PA1 key
	case VK_OEM_CLEAR: return "OCLEAR"; // Clear key
	}

	return "NOTHING";
}

// конвертировать код клавиши в строку

std::string CovertKeyCodeToString(unsigned int val)
{
	std::string outstr;

	if (val == 0)
	{
		return "";
	}

	int KeyVal = (int)(val & 0xFF);

	outstr = _VKeyToStr(KeyVal);

	if ((val & 0x40000) > 0)
	{
		outstr = "SHIFT+" + outstr;
	}
	if ((val & 0x10000) > 0)
	{
		outstr = "ALT+" + outstr;
	}
	if ((val & 0x20000) > 0)
	{
		outstr = "CTRL+" + outstr;
	}

	if (!KeyVal)
		return "";

	return outstr;
}

// конвертировать строку в код клавиши (комбинации клавиш)
unsigned int CovertStringToKeyCode(std::string code)
{
	if (code.length() == 0)
		return 0;

	unsigned int outcode = 0;
	char* arg2;
	if ((arg2 = strchr(&code[0], '+')) != NULL)
	{
		arg2[0] = '\0';
		arg2++;
		std::string arg1 = &code[0];
		if (arg1 == "CTRL")
		{
			outcode = 0x20000;
		}
		else if (arg1 == "ALT")
		{
			outcode = 0x10000;
		}
		else if (arg1 == "SHIFT")
		{
			outcode = 0x40000;
		}
	}
	else arg2 = &code[0];

	int vkeyout = _StrToVKey(ToUpper(arg2));

	outcode += vkeyout;

	if (vkeyout == 0)
		return 0;

	return outcode;
}



std::string tmpkeycode;

// конвертировать строку в код клавиши (комбинации клавиш)
int __stdcall ConvertKeyStringToKeyCode(const char* str)
{
	if (DEBUG_FULL)
		std::cout << __func__ << std::endl;
	if (str == NULL)
		return 0;
	return CovertStringToKeyCode(str);
}

// конвертировать код клавиши в строку
const char* __stdcall ConvertKeyCodeToKeyString(unsigned int code)
{
	tmpkeycode = CovertKeyCodeToString(code);
	return tmpkeycode.c_str();
}