#pragma once
#include "Main.h"
#include <Tools.h>
#include <Timer.h>

extern vector<int> doubleclickSkillIDs;
extern vector<int> WhiteListForTeleport;
extern BOOL ShopHelperEnabled;
extern BOOL TeleportShiftPress;
extern BOOL BlockKeyAndMouseEmulation;
extern BOOL EnableSelectHelper;
extern BOOL DoubleClickHelper;
extern BOOL AutoSelectHero;
extern BOOL LOCK_MOUSE_IN_WINDOW;
extern BOOL BlockKeyboardAndMouseWhenTeleport;
extern BOOL rawimage_skipmouseevent;
typedef LRESULT( __fastcall *  WarcraftRealWNDProc )( HWND hWnd, unsigned int Msg, WPARAM wParam, LPARAM lParam );
extern WarcraftRealWNDProc WarcraftRealWNDProc_org, WarcraftRealWNDProc_ptr;
LRESULT __fastcall WarcraftWindowProcHooked( HWND hWnd, unsigned int Msg, WPARAM _wParam, LPARAM lParam );
extern int ShiftPressed;
extern BOOL SkipAllMessages;
extern int IssueWithoutTargetOrderOffset;
extern int IssueTargetOrPointOrder2Offset;
extern int sub_6F339D50Offset;
extern int IssueTargetOrPointOrderOffset;
extern int sub_6F339E60Offset;
extern int sub_6F339F00Offset;
extern int sub_6F339F80Offset;
extern int sub_6F33A010Offset;
void IssueFixerInit( );
void IssueFixerDisable( );
typedef int( __fastcall * c_SimpleButtonClickEvent )( int pButton, int unused, int ClickEventType );
int __fastcall SimpleButtonClickEvent_my( int pButton, int unused, int ClickEventType );
extern c_SimpleButtonClickEvent SimpleButtonClickEvent_org;
extern c_SimpleButtonClickEvent SimpleButtonClickEvent_ptr;
extern int CommandButtonVtable;
extern std::vector<ClickPortrainForId> ClickPortrainForIdList;

typedef int( __fastcall * pSimpleButtonPreClickEvent )( int pButton, int unused, int a2 );
int __fastcall SimpleButtonPreClickEvent_my( int pButton, int unused, int a2 );
extern pSimpleButtonPreClickEvent SimpleButtonPreClickEvent_org;
extern pSimpleButtonPreClickEvent SimpleButtonPreClickEvent_ptr;

extern std::vector<ObjInfoAction> IgnoreObjInfo;

extern std::vector< int > InfoWhitelistedObj;

unsigned int CovertStringToKeyCode( std::string code );
std::string CovertKeyCodeToString( unsigned int val );
unsigned int BuildKeyCode( );
void PressKeyWithDelay_timed( Timer *tm );