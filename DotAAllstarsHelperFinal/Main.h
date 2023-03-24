#pragma once
#define _WIN32_WINNT 0x0501 
#define WINVER 0x0501 
#define NTDDI_VERSION 0x05010000
#define VC_EXTRALEAN
#define _ATL_XP_TARGETING
#define WIN32_LEAN_AND_MEAN
#define PSAPI_VERSION 1

#include <stdint.h>
#include <cstdio>
#include <cstring>
#include <csetjmp>
#include <ctime>
#include <Windows.h>
#include <string>
#include <vector>

#include "fp_call.h"

#include <concurrent_vector.h>
#define safevector concurrency::concurrent_vector

#include <fstream> 
#include <iostream>
#include <sstream>
#include <algorithm>
#include <MinHook.h>
#include <thread>
#include <chrono>
#include <map>
#include <utility>
#include <codecvt>
#include <filesystem>
namespace fs = std::filesystem;


#include <winsock2.h>
#include "buffer.h"

#include <Psapi.h>


#define MASK_56 (((u_int64_t)1<<56)-1) /* i.e., (u_int64_t)0xffffffffffffff */

#include "fnv.h"


#include "Structures.h"

#include "Storm.h"

#include "WarcraftFrameHelper.h"

#include "MegaPacketHandler.h"


#include <stdlib.h>

/*
#include <portaudio.h>
#pragma comment(lib,"portaudio.lib")
*/

#include "quantizer.h"

#include "BlpReadWrite.h"



#pragma comment(lib,"ijl15.lib")
#include <ijl.h>


#include "GameStructs.h"


extern bool DEBUG_FULL;

#pragma comment(lib,"legacy_stdio_definitions.lib")
#pragma comment(lib,"Psapi.lib")
#pragma comment (lib, "Ws2_32.lib")
#pragma comment(lib, "libMinHook.x86.lib")

#define IsKeyPressed(CODE) ((GetAsyncKeyState(CODE) & 0x8000) > 0)

extern int SELF_UNLOAD_DLL_AFTER_GAME_END;
extern void* GameDllModule;
extern void* StormDllModule;
extern void* GetCurrentModule;
void __stdcall DisableAllHooks();
void* __stdcall DllSelfUnloading(void* hModule);

extern bool FIRST_GAME_START_FOUND;
bool IsGame();

extern unsigned char* ConvertHandle(int handle);

extern int InitFunctionCalled;

extern int IsVEHex;
extern int TestModeActivated;

extern const char* configfilename;

extern float CustomFovFix;

extern int NewCallBackTriggerHandle;
extern int LastEventId;

void FrameDefHelperUninitialize();
void FrameDefHelperInitialize();

void PatchOffsetValue1(void* addr, unsigned char value);
extern int RenderStage;

war3::CGameUI* GameUIObjectGet();
int FileExist(const char* name);
unsigned long GetDllCrc32();
typedef void* (__cdecl* _TriggerExecute)(int TriggerHandle);
extern _TriggerExecute TriggerExecute;

int __stdcall SendMessageToChat(const char* msg, int toAll);

typedef void(__cdecl* pExecuteFunc)(jRCString* funcname);
extern pExecuteFunc ExecuteFunc;
typedef void(__thiscall* pConvertStrToJassStr)(jRCString* jStr, const char* cStr);
extern pConvertStrToJassStr str2jstr;

std::string uint_to_hex(unsigned int i);

int IsClassEqual(int ClassID1, int ClassID2);
int GetTypeId(unsigned char* unit_item_abil_etc_addr);
int __stdcall Wc3MessageBox(const char* message, int type);
typedef void(__fastcall* pGame_Wc3MessageBox) (int type, const char* text, int IsUsedCallBack, int callbackaddr, int unk2, int unk3, int unk4);
extern pGame_Wc3MessageBox Game_Wc3MessageBox;
typedef void(__fastcall* pLoadFrameDefList)(const char* filepath, int env);
extern pLoadFrameDefList LoadFrameDefList;

inline std::string ToLower(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return (char)std::tolower(c); });
	return str;
}
inline std::string ToUpper(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return  (char)std::toupper(c); });
	return str;
}

extern bool MainFuncWork;

extern int ChatEditBoxVtable;
int IsChatActive();
int IsGameFrameActive();
extern int SetInfoObjDebugVal;

char* GetWar3Preferense(int ID);
extern char MyFpsString[512];

int IsPlayerEnemy(int hPlayer1, int hPlayer2);
extern std::map<std::pair<int, int>, int> PlayerEnemyCache;

extern int playercache[16];
extern int player_real_cache[16];
extern int player_observers[16];
extern int player_local_id;

typedef int(__cdecl* pGetPlayerAlliance)(unsigned int hPlayer1, unsigned int hPlayer2, int hAlliancetype);
extern pGetPlayerAlliance GetPlayerAlliance;
typedef unsigned int(__cdecl* pGetPlayerColor)(unsigned int whichPlayer);
extern pGetPlayerColor GetPlayerColor2;
typedef int(__cdecl* pPlayer)(int number);
extern pPlayer _Player;
int __stdcall Player(int number);
typedef int(__cdecl* pIsPlayerObs)(unsigned int hPlayer);
extern pIsPlayerObs IsPlayerObs;


int GetLocalPlayerId();
int _GetLocalPlayerId();
int GetPlayerByNumber(int number);
int _GetPlayerByNumber(int number);
// Проверить являются ли игроки врагами
typedef int(__cdecl* IsPlayerEnemy_org)(unsigned int Player1, unsigned int Player2);
// Получить игрока по ID
typedef unsigned int(__cdecl* GetPlayerByID)(int PlayerId);
typedef char* (__fastcall* p_GetPlayerName)(int a1, int a2);
extern p_GetPlayerName GetPlayerName;
int __stdcall MutePlayer(const char* str);
int __stdcall UnMutePlayer(const char* str);
typedef void(__fastcall* pOnChatMessage)(int a1, int unused, int PlayerID, char* message, int a4, float a5);
void __fastcall pOnChatMessage_my(int a1, int unused, int PlayerID, char* message, int a4, float a5);
extern std::vector<char*> mutedplayers;
//sub_6F2FB480
extern pOnChatMessage pOnChatMessage_org, pOnChatMessage_ptr;
int IsPlayerObserver(int pid);
int _IsPlayerObserver(int pid);
int IsLocalPlayerObserver();
extern bool ShowSkillPanelForObservers;
extern bool ShowSkillPanelOnlyForHeroes;

extern void(__thiscall* SelectUnitReal)(int pPlayerSelectData, unsigned char * pUnit, int id, int unk1, int unk2, int unk3);
extern void(__thiscall* UpdatePlayerSelection)(int pPlayerSelectData, int unk);
extern int(__cdecl* ClearSelection)(void);
void SelectUnit(unsigned char* unit);
std::vector< unsigned char*> GetUnitsFromGroup(int groupid);
unsigned char** FindUnitAbils(unsigned char* unitaddr, unsigned int* count, int abilcode = 0, int abilbasecode = 0);
int __stdcall GetUnitOwnerSlot(unsigned char* unitaddr);
int __stdcall IsEnemy(unsigned char * UnitAddr);
int __stdcall IsHero(unsigned char* unitaddr);
int __stdcall IsTower(unsigned char* unitaddr);
int __stdcall IsNotBadUnit(unsigned char* unitaddr, int onlymem = false);
int __stdcall IsUnitInvulnerable(unsigned char* unitaddr);
int __stdcall IsUnitIllusion(unsigned char* unitaddr);
int __stdcall IsNotBadItem(unsigned char * itemaddr, int extracheck = false);
typedef int(__fastcall* pGetHeroInt)(unsigned char* unitaddr, int unused, int withbonus);
extern pGetHeroInt GetHeroInt;
int GetSelectedUnitCountBigger(int slot);
unsigned char* GetSelectedUnit(int slot);
void GetItemLocation2D(unsigned char * itemaddr, float* x, float* y);
void GetUnitLocation2D(unsigned char* unitaddr, float* x, float* y);
int* GetUnitCountAndUnitArray(unsigned char*** unitarray);
int* GetItemCountAndItemArray(unsigned char*** itemarray);
float GetUnitMPregen(unsigned char* unitaddr);
float GetUnitHPregen(unsigned char* unitaddr);
float GetUnitMP(unsigned char* unitaddr);
float GetUnitHP(unsigned char* unitaddr);
unsigned char* GetUnitAddressFloatsRelated(unsigned char* unitaddr, int step);
float GetUnitX_real(unsigned char* unitaddr);
float GetUnitY_real(unsigned char* unitaddr);

typedef float* (__thiscall* _GetUnitFloatStat)(unsigned char* unitaddr, float* a2, int a3);
extern _GetUnitFloatStat GetUnitFloatState;
extern unsigned char BarVtableClone[0x80];
void ManaBarSwitch(int b);
void PatchOffset(void* addr, void* buffer, unsigned int size);
PBYTE HookVTableFunction(PDWORD* dwVTable, PBYTE dwHook, int Index);
PBYTE GetVTableFunction(PDWORD* dwVTable, int Index);


extern std::vector<int> MpBarUnitWhiteList;

extern unsigned int hpbarcolorsHero[20];
extern unsigned int hpbarcolorsUnit[20];
extern unsigned int hpbarcolorsTower[20];
extern float hpbarscaleHeroX[20];
extern float hpbarscaleUnitX[20];
extern float hpbarscaleTowerX[20];
extern float hpbarscaleHeroY[20];
extern float hpbarscaleUnitY[20];
extern float hpbarscaleTowerY[20];
extern std::vector<CustomHPBar> CustomHPBarList[20];
int __stdcall SetColorForUnit(unsigned int* coloraddr, BarStruct* BarStruct);

void  __stdcall AddNewLineToJassLog(const char* s);
void __stdcall  AddNewLineToDotaChatLog(const char* s);
void __stdcall  AddNewLineToDotaHelperLog(const char* s, int line);//( const char * s, int line );
void __stdcall  AddNewLineToJassNativesLog(const char* s);
void __stdcall EnableErrorHandler(int);
void __stdcall DisableErrorHandler(int);


typedef int(__fastcall* LookupNative)(int global, int unused, const char* name);
extern LookupNative LookupNative_org;
typedef signed int(__fastcall* LookupJassFunc)(int global, int unused, const char* funcname);
extern LookupJassFunc LookupJassFunc_org;

extern int bDllLogEnable;


extern LPTOP_LEVEL_EXCEPTION_FILTER OriginFilter;

//  Game.dll
extern unsigned char* GameDll;
extern unsigned long GetGameDllThread;
//	Storm.dll
extern unsigned char* StormDll;
extern void* GameDllModule;
extern void* StormDllModule;
extern HWND Warcraft3Window;
extern bool ClickHelperDisabled;
extern int GlobalGameStateOffset;
extern int IsPlayerEnemyOffset;
extern int DrawSkillPanelOffset;
extern int DrawSkillPanelOverlayOffset;
extern int IsDrawSkillPanelOffset;
extern int IsDrawSkillPanelOverlayOffset;
extern int IsNeedDrawUnitOriginOffset;
extern int IsNeedDrawUnit2offset;
extern int IsNeedDrawUnit2offsetRetAddress;
extern unsigned char* _GlobalGlueObj;
extern unsigned char* _GameUI;
extern bool GAME_START_CALLED;
extern int* IsWindowActive;
extern unsigned char* ChatFound;
extern unsigned char* pW3XGlobalClass;
extern unsigned char* pGameClass1;
extern unsigned char* pWar3GlobalData1;
extern unsigned char* UnitVtable;
extern unsigned char* ItemVtable;
extern unsigned char* pPrintText2;
extern const char* GetBoolStr(int val);
extern void PrintText(const char* text, float staytime = 10.0f, int force = false);
extern void PrintText(std::string text, float staytime = 10.0f, int force = false);
extern unsigned char* MapNameOffset1;
extern int MapNameOffset2;
extern int pOnChatMessage_offset;
extern unsigned char* _BarVTable;
extern int pAttackSpeedLimit;
extern unsigned char* GetItemInSlotAddr;
extern float* GetWindowXoffset;
extern float* GetWindowYoffset;
extern unsigned char* GameFrameAtMouseStructOffset;
extern int pTriggerExecute;

extern std::vector<ModelCollisionFixStruct> ModelCollisionFixList;
extern std::vector<ModelTextureFixStruct> ModelTextureFixList;
extern std::vector<ModelPatchStruct> ModelPatchList;
extern std::vector<ModelRemoveTagStruct> ModelRemoveTagList;
extern std::vector<ModelSequenceReSpeedStruct> ModelSequenceReSpeedList;
extern std::vector<ModelSequenceValueStruct> ModelSequenceValueList;
extern std::vector<ModelScaleStruct> ModelScaleList;

extern std::vector<ICONMDLCACHE> ICONMDLCACHELIST;

extern std::vector<FileRedirectStruct> FileRedirectList;
typedef int(__fastcall* GameGetFile)(const char* filename, unsigned char ** OutDataPointer, size_t* OutSize, int unknown);
int __fastcall GameGetFile_my(const char* filename, unsigned char ** OutDataPointer, unsigned int* OutSize, int unknown);
extern GameGetFile GameGetFile_org, GameGetFile_ptr;

//
//
//typedef int( __stdcall * SFileOpenFile )( const char* filename, int * handle );
//typedef int( __stdcall * SFileReadFile )( int file_handle, void* buffer, unsigned int size, unsigned int* size_ptr, unsigned int unk );
//
//extern SFileOpenFile SFileOpenFile_org, SFileOpenFile_ptr;
//extern SFileReadFile SFileReadFile_org, SFileReadFile_ptr;
//
//
//int __stdcall SFileOpenFile_my( const char* filename, int * handle );
//int __stdcall SFileReadFile_my( int file_handle, void* buffer, unsigned int size, unsigned int* size_ptr, unsigned int unk );

void FreeAllIHelpers();


typedef int(__fastcall* p_GetTypeInfo)(int unit_item_code, int unused);
extern p_GetTypeInfo GetTypeInfo;



extern std::vector<FakeFileStruct> FakeFileList;

std::vector<std::string> get_file_list(const fs::path& path, bool dotolower = false);
std::string GetFileContent(std::string filename);

int __stdcall CheckWriteAccess(int a1 = 0);
void __stdcall EnableLocalFiles(int enable);
int __stdcall IsLocalFilesEnabled(int a1 = 0);



extern int EnableFixFOV;
int __stdcall SetWidescreenFixState(int widefixenable);
int __stdcall SetCustomFovFix(float _CustomFovFix);
extern Matrix1 globalmatrix;
void __fastcall SetGameAreaFOV_my(Matrix1* a1, int a2, float a3, float a4, float a5, float a6);
typedef int(__fastcall* SetGameAreaFOV)(Matrix1* a1, int a2, float a3, float a4, float a5, float a6);
extern SetGameAreaFOV SetGameAreaFOV_org, SetGameAreaFOV_ptr;



std::string SendHttpPostRequest(const char* url, const char* data);
std::string SendHttpGetRequest(const char* host, const char* path);
extern std::string LatestDownloadedString;



extern int FPS_LIMIT_ENABLED;
extern void* Warcraft3_Process;
extern unsigned int CPU_cores;
void InitThreadCpuUsage();
double GetWar3CpuUsage();
void UpdateFPS();
typedef int(__cdecl* p_SetMaxFps)(int maxfps);
extern p_SetMaxFps _SetMaxFps;
typedef void(__fastcall* pDrawBarForUnit)(unsigned char* unitaddr);
void __fastcall DrawBarForUnit_my(unsigned char* unitaddr);
extern pDrawBarForUnit DrawBarForUnit_org, DrawBarForUnit_ptr;
//extern map<int, int> NeedDrawBarForUnit;
extern int FPSfix1Enabled;

typedef int(__fastcall* pGameChatSetState)(unsigned char* chat, int unused, int IsOpened);
extern pGameChatSetState GameChatSetState;

typedef int(__fastcall* pSetChatTargetUsers/*sub_6F3412F0*/)(int chataddr, int ecx, int valtype);
extern pSetChatTargetUsers pSetChatTargetUsers_org;
extern pSetChatTargetUsers pSetChatTargetUsers_ptr;
int __fastcall SetChatTargetUsers_my(int chataddr, int ecx, int valtype);

extern LPARAM lpRShiftScanKeyUP;
extern LPARAM lpRShiftScanKeyDOWN;

extern LPARAM lpShiftScanKeyUP;
extern LPARAM lpShiftScanKeyDOWN;

extern LPARAM lpLShiftScanKeyUP;
extern LPARAM lpLShiftScanKeyDOWN;

extern unsigned char* _EventVtable;
extern unsigned char* _ChatSendEvent;




extern int usedcustomframes;
extern unsigned char* pCurrentFrameFocusedAddr;
extern unsigned char* DefaultCStatus;
extern unsigned char* LoadFramesVar1;
extern unsigned char* LoadFramesVar2;
typedef void(__fastcall* pLoadNewFrameDef)(const char* filename, unsigned char* var1, unsigned char* var2, unsigned char* cstatus);
extern pLoadNewFrameDef LoadNewFrameDef_org;
typedef unsigned char* (__fastcall* pCreateNewFrame) (const char* FrameName, unsigned char* pGlobalGameClass, int unk1, int unk2, int unk3);
extern pCreateNewFrame CreateNewFrame;
typedef void(__thiscall* pSetFramePos)(unsigned char* FrameAddr180, int rowid, float left, float bottom, int unk);
extern pSetFramePos SetFramePos;
//sub_6F606770
typedef void* (__thiscall* pShowFrameAlternative)(unsigned char* FrameAddr180, int unk1_0, unsigned char* pGlobalGameClass180, int unk1_1, float x, float y, int unk);
extern pShowFrameAlternative ShowFrameAlternative;
typedef void* (__thiscall* pShowThisFrame)(unsigned char* FrameAddr);
extern pShowThisFrame ShowThisFrame;
typedef void* (__thiscall* pDestructThisFrame)(unsigned char* FrameAddr, int unk);
extern pDestructThisFrame DestructThisFrame;//Without clean memory
typedef unsigned char* (__fastcall* pGetFrameItemAddress)(const char* name, int id);
extern pGetFrameItemAddress GetFrameItemAddress;
typedef int(__thiscall* pUpdateFrameFlags)(unsigned char* FrameAddr, char unk);
extern pUpdateFrameFlags UpdateFrameFlags;
typedef int(__thiscall* pWc3ControlClickButton)(void * btnaddr, int unk);
extern pWc3ControlClickButton Wc3ControlClickButton_org, Wc3ControlClickButton_ptr;
int __fastcall Wc3ControlClickButton_my(void* btnaddr, int, int unk);
int __stdcall ShowConfigWindow(const char*);
extern int ConfigWindowCreated;
void ProcessClickAtCustomFrames();
extern int GlyphButtonCreated;




// for all warcraft versions
void UninitOpenglHook();
void InitOpenglHook();

// 1.26a
void Uninitd3d8Hook(int cleartextures);
void Initd3d8Hook();

// 1.27a
void DrawOverlayDx8();
void DrawOverlayGl();

void SetNewLightDx8(int id);
void SetOldLightDx8(int id);

extern bool OverlayDrawed;

void InitD3DVSync(int enabled);


//Get hModule from addr
void* GetModuleFromAddress(unsigned char* addr);
typedef int(__cdecl* GetTownUnitCount_p)(int*, int, int);
int __cdecl Wc3MemoryRW(int* addr, int value, int write);
extern GetTownUnitCount_p GetTownUnitCount_org;
extern GetTownUnitCount_p GetTownUnitCount_ptr;
int __stdcall GetJassStringCount(int dump);
int __stdcall ScanJassStringForErrors(int dump);

int IsOkayPtr(void* ptr, unsigned int size = 4);


const float DesktopScreen_Width = (float)GetSystemMetrics(SM_CXSCREEN);
const float DesktopScreen_Height = (float)GetSystemMetrics(SM_CYSCREEN);

extern float DefaultSceenWidth;
extern float DefaultSceenHeight;

extern int ScanId;

extern int protect_integer;
extern int protect_integer2;


inline std::string WStringToString(LPCWSTR s)
{
	if (!s)
		return "";
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.to_bytes(s);
}

inline std::wstring StringToWString(LPCSTR s)
{
	if (!s)
		return L"";
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.from_bytes(s);
}

int __stdcall InitHpBar(int);
void SetTlsForMe();
void __stdcall Packet_Initialize(int TriggerHandle);
/* Voice chat. Works only in a single player game :(
In the local game or on the Internet there is a limit on sending data size, so the voice is recorded no more than one second! It is necessary to find and destroy the sending limit.
void UninitializeVoiceClient( );
void InitVoiceClientThread( );
void AddNewPaTestData( std::vector<unsigned char> _samples, int playerid, int packetsize, bool compressed );
*/
