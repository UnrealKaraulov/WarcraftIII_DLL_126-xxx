#pragma once
#include "Main.h"

extern std::vector<int> doubleclickSkillIDs;
extern std::vector<int> WhiteListForTeleport;
extern int ShopHelperEnabled;
extern int TeleportShiftPress;
extern int BlockKeyAndMouseEmulation;
extern int EnableSelectHelper;
extern int DoubleClickHelper;
extern int AutoSelectHero;
extern int LOCK_MOUSE_IN_WINDOW;
extern int BlockKeyboardAndMouseWhenTeleport;
extern int rawimage_skipmouseevent;
typedef LRESULT(__fastcall* WarcraftRealWNDProc)(HWND hWnd, unsigned int Msg, WPARAM wParam, LPARAM lParam);
extern WarcraftRealWNDProc WarcraftRealWNDProc_org, WarcraftRealWNDProc_ptr;
LRESULT __fastcall WarcraftWindowProcHooked(HWND hWnd, unsigned int Msg, WPARAM _wParam, LPARAM lParam);
extern int ShiftPressed;
extern int SkipAllMessages;
extern int IssueWithoutTargetOrderOffset;
extern int IssueTargetOrPointOrder2Offset;
extern int sub_6F339D50Offset;
extern int IssueTargetOrPointOrderOffset;
extern int sub_6F339E60Offset;
extern int sub_6F339F00Offset;
extern int sub_6F339F80Offset;
extern int sub_6F33A010Offset;
void IssueFixerInit();
void IssueFixerDisable();
typedef int(__fastcall* c_SimpleButtonClickEvent)(unsigned char* pButton, unsigned char* unused, int ClickEventType);
int __fastcall SimpleButtonClickEvent_my(unsigned char* pButton, unsigned char* unused, int ClickEventType);
extern c_SimpleButtonClickEvent SimpleButtonClickEvent_org;
extern c_SimpleButtonClickEvent SimpleButtonClickEvent_ptr;
extern unsigned char* CommandButtonVtable;
extern std::vector<ClickPortrainForId> ClickPortrainForIdList;

typedef int(__fastcall* pSimpleButtonPreClickEvent)(unsigned char* pButton, int unused, int a2);
int __fastcall SimpleButtonPreClickEvent_my(unsigned char* pButton, int unused, int a2);
extern pSimpleButtonPreClickEvent SimpleButtonPreClickEvent_org;
extern pSimpleButtonPreClickEvent SimpleButtonPreClickEvent_ptr;

extern std::vector<ObjInfoAction> IgnoreObjInfo;

extern std::vector< int > InfoWhitelistedObj;

unsigned int CovertStringToKeyCode(std::string code);
std::string CovertKeyCodeToString(unsigned int val);
unsigned int BuildKeyCode();
void PressKeyWithDelay_timed();


extern int EmulateKeyInputForHWND;
extern int ShiftPressed;
extern unsigned long SkipSingleShift;
extern unsigned long SingleShift;
extern int SkipAllMessages;