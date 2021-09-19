#pragma once
#define _WIN32_WINNT 0x0501 
#define WINVER 0x0501 
#define NTDDI_VERSION 0x05010000
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
//#define PSAPI_VERSION 1

#pragma region Includes


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

using namespace std;
#include <winsock2.h>
#include "buffer.h"

#define PSAPI_VERSION 1
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


#pragma endregion



#pragma comment(lib,"legacy_stdio_definitions.lib")
#pragma comment(lib,"Psapi.lib")
#pragma comment (lib, "Ws2_32.lib")
#pragma comment(lib, "libMinHook.x86.lib")

#define IsKeyPressed(CODE) ((GetAsyncKeyState(CODE) & 0x8000) > 0)


BOOL IsGame( );

extern int ConvertHandle( int handle );

extern BOOL InitFunctionCalled;

extern BOOL TerminateStarted;
extern BOOL IsVEHex;
extern BOOL TestModeActivated;

extern const char * configfilename;

void FrameDefHelperUninitialize( );
void FrameDefHelperInitialize( );

void PatchOffsetValue1(void * addr, unsigned char value);
extern int RenderStage;

int GetGlobalClassAddr( );
BOOL FileExist( const char * name );
DWORD GetDllCrc32( );
typedef void *( __cdecl * _TriggerExecute )( int TriggerHandle );
extern _TriggerExecute TriggerExecute;

int __stdcall SendMessageToChat( const char * msg, BOOL toAll );

typedef void( __cdecl * pExecuteFunc )( jRCString * funcname );
extern pExecuteFunc ExecuteFunc;
typedef void( __thiscall * pConvertStrToJassStr )( jRCString * jStr, const char * cStr );
extern pConvertStrToJassStr str2jstr;

std::string uint_to_hex( unsigned int i );

BOOL IsClassEqual( int ClassID1, int ClassID2 );
int GetTypeId( int unit_item_abil_etc_addr );
int __stdcall Wc3MessageBox( const char * message, int type );
typedef void( __fastcall * pGame_Wc3MessageBox ) ( int type, const char * text, BOOL IsUsedCallBack, int callbackaddr, int unk2, int unk3, int unk4 );
extern pGame_Wc3MessageBox Game_Wc3MessageBox;
typedef void( __fastcall * pLoadFrameDefList )( const char * filepath, int env );
extern pLoadFrameDefList LoadFrameDefList;

inline string ToLower( string str )
{
	std::transform( str.begin( ), str.end( ), str.begin( ), tolower );
	return str;
}
inline string ToUpper( string str )
{
	std::transform( str.begin( ), str.end( ), str.begin( ), toupper );
	return str;
}

extern BOOL MainFuncWork;

extern int ChatEditBoxVtable;
BOOL IsChatActive( );
BOOL IsGameFrameActive( );
extern BOOL SetInfoObjDebugVal;

char *  GetWar3Preferense( int ID );
extern char MyFpsString[ 512 ];


#pragma region DotaPlayerHelper.cpp

BOOL IsPlayerEnemy( int hPlayer1, int hPlayer2 );
extern std::map<std::pair<int, int>, BOOL> PlayerEnemyCache;

extern int playercache[ 16 ];
extern int player_real_cache[ 16 ];
extern BOOL player_observers[ 16 ];
extern int player_local_id;

typedef BOOL( __cdecl * pGetPlayerAlliance )( unsigned int hPlayer1, unsigned int hPlayer2, int hAlliancetype );
extern pGetPlayerAlliance GetPlayerAlliance;
typedef unsigned int( __cdecl * pGetPlayerColor )( unsigned int whichPlayer );
extern pGetPlayerColor GetPlayerColor2;
typedef int( __cdecl * pPlayer )( int number );
extern pPlayer _Player;
int __stdcall Player( int number );
typedef int( __cdecl * pIsPlayerObs )( unsigned int hPlayer );
extern pIsPlayerObs IsPlayerObs;


int GetLocalPlayerId( );
int _GetLocalPlayerId( );
int GetPlayerByNumber( int number );
int _GetPlayerByNumber( int number );
// Проверить являются ли игроки врагами
typedef int( __cdecl * IsPlayerEnemy_org )( unsigned int Player1, unsigned int Player2 );
// Получить игрока по ID
typedef unsigned int( __cdecl * GetPlayerByID )( int PlayerId );
typedef char *( __fastcall * p_GetPlayerName )( int a1, int a2 );
extern p_GetPlayerName GetPlayerName;
int __stdcall MutePlayer( const char * str );
int __stdcall UnMutePlayer( const char * str );
typedef void( __fastcall * pOnChatMessage )( int a1, int unused, int PlayerID, char * message, int a4, float a5 );
void __fastcall pOnChatMessage_my( int a1, int unused, int PlayerID, char * message, int a4, float a5 );
extern vector<char *> mutedplayers;
//sub_6F2FB480
extern pOnChatMessage pOnChatMessage_org, pOnChatMessage_ptr;
BOOL IsPlayerObserver( int pid );
BOOL _IsPlayerObserver( int pid );
BOOL IsLocalPlayerObserver( );
extern BOOL ShowSkillPanelForObservers;
extern BOOL ShowSkillPanelOnlyForHeroes;
#pragma endregion

#pragma region UnitAndItem.cpp
extern void( __thiscall * SelectUnitReal )( int pPlayerSelectData, int pUnit, int id, int unk1, int unk2, int unk3 );
extern void( __thiscall * UpdatePlayerSelection )( int pPlayerSelectData, int unk );
extern int( __cdecl * ClearSelection )( void );
void SelectUnit( int unit );
std::vector<int> GetUnitsFromGroup( int groupid );
int * FindUnitAbils( int unitaddr, unsigned int * count, int abilcode = 0, int abilbasecode = 0 );
int __stdcall GetUnitOwnerSlot( int unitaddr );
BOOL __stdcall IsEnemy( int UnitAddr );
BOOL __stdcall IsHero( int unitaddr );
BOOL __stdcall IsTower( int unitaddr );
BOOL __stdcall IsNotBadUnit( int unitaddr, BOOL onlymem = FALSE );
BOOL __stdcall IsUnitInvulnerable( int unitaddr );
BOOL __stdcall IsUnitIllusion( int unitaddr );
BOOL __stdcall IsNotBadItem( int itemaddr, BOOL extracheck = FALSE );
typedef int( __fastcall * pGetHeroInt )( int unitaddr, int unused, BOOL withbonus );
extern pGetHeroInt GetHeroInt;
int GetSelectedUnitCountBigger( int slot );
int GetSelectedUnit( int slot );
void GetItemLocation2D( int itemaddr, float * x, float * y );
void GetUnitLocation2D( int unitaddr, float * x, float * y );
int * GetUnitCountAndUnitArray( int ** unitarray );
int * GetItemCountAndItemArray( int ** itemarray );
float GetUnitMPregen( int unitaddr );
float GetUnitHPregen( int unitaddr );
float GetUnitMP( int unitaddr );
float GetUnitHP( int unitaddr );
int GetUnitAddressFloatsRelated( int unitaddr, int step );
float GetUnitX_real( int unitaddr );
float GetUnitY_real( int unitaddr );
#pragma endregion

#pragma region DotaMPBarHelper.cpp
typedef float *( __thiscall * _GetUnitFloatStat )( int unitaddr, float *a2, int a3 );
extern _GetUnitFloatStat GetUnitFloatState;
extern unsigned char BarVtableClone[ 0x80 ];
void ManaBarSwitch( BOOL b );
void PatchOffset( void * addr, void * buffer, unsigned int size );
PBYTE HookVTableFunction( PDWORD* dwVTable, PBYTE dwHook, INT Index );
PBYTE GetVTableFunction( PDWORD* dwVTable, INT Index );


extern vector<int> MpBarUnitWhiteList;

#pragma endregion


#pragma region DotaHPBarHelper.cpp
extern unsigned int hpbarcolorsHero[ 20 ];
extern unsigned int hpbarcolorsUnit[ 20 ];
extern unsigned int hpbarcolorsTower[ 20 ];
extern float hpbarscaleHeroX[ 20 ];
extern float hpbarscaleUnitX[ 20 ];
extern float hpbarscaleTowerX[ 20 ];
extern float hpbarscaleHeroY[ 20 ];
extern float hpbarscaleUnitY[ 20 ];
extern float hpbarscaleTowerY[ 20 ];
extern vector<CustomHPBar> CustomHPBarList[ 20 ];
int __stdcall SetColorForUnit( unsigned int  * coloraddr, BarStruct * BarStruct );



#pragma endregion

#pragma region ErrorHandler.cpp

void  __stdcall AddNewLineToJassLog( const char * s );
void __stdcall  AddNewLineToDotaChatLog( const char * s );
void __stdcall  AddNewLineToDotaHelperLog( const char * s, int line );//( const char * s, int line );
void __stdcall  AddNewLineToJassNativesLog( const char * s );
void __stdcall EnableErrorHandler( int );
void __stdcall DisableErrorHandler( int );


typedef int( __fastcall *LookupNative )( int global, int unused, const char * name );
extern LookupNative LookupNative_org;
typedef signed int( __fastcall * LookupJassFunc )( int global, int unused, const char * funcname );
extern LookupJassFunc LookupJassFunc_org;

extern BOOL bDllLogEnable;


extern LPTOP_LEVEL_EXCEPTION_FILTER OriginFilter;

#pragma endregion


//  Game.dll
extern int GameDll;
//	Storm.dll
extern int StormDll;
extern HMODULE GameDllModule;
extern HMODULE StormDllModule;
extern HWND Warcraft3Window;

#pragma region All Offsets Here
extern int GlobalGameStateOffset;
extern int IsPlayerEnemyOffset;
extern int DrawSkillPanelOffset;
extern int DrawSkillPanelOverlayOffset;
extern int IsDrawSkillPanelOffset;
extern int IsDrawSkillPanelOverlayOffset;
extern int IsNeedDrawUnitOriginOffset;
extern int IsNeedDrawUnit2offset;
extern int IsNeedDrawUnit2offsetRetAddress;
extern int _GlobalGlueObj;
extern int _GameUI;
extern int * InGame;
extern BOOL * IsWindowActive;
extern int ChatFound;
extern int pW3XGlobalClass;
extern int pGameClass1;
extern int pWar3GlobalData1;
extern int UnitVtable;
extern int ItemVtable;
extern int pPrintText2;
extern const char * GetBoolStr( BOOL val );
extern void PrintText( const char * text, float staytime = 10.0f, BOOL force = FALSE );
extern void PrintText( std::string text, float staytime = 10.0f, BOOL force = FALSE);
extern int MapNameOffset1;
extern int MapNameOffset2;
extern int pOnChatMessage_offset;
extern int _BarVTable;
extern int pAttackSpeedLimit;
extern int GetItemInSlotAddr;
extern float * GetWindowXoffset;
extern float * GetWindowYoffset;
extern int GameFrameAtMouseStructOffset;
extern int pTriggerExecute;
#pragma endregion


#pragma region DotaFilesHelper.cpp
extern vector<ModelCollisionFixStruct> ModelCollisionFixList;
extern vector<ModelTextureFixStruct> ModelTextureFixList;
extern vector<ModelPatchStruct> ModelPatchList;
extern vector<ModelRemoveTagStruct> ModelRemoveTagList;
extern vector<ModelSequenceReSpeedStruct> ModelSequenceReSpeedList;
extern vector<ModelSequenceValueStruct> ModelSequenceValueList;
extern vector<ModelScaleStruct> ModelScaleList;

extern vector<ICONMDLCACHE> ICONMDLCACHELIST;

extern vector<FileRedirectStruct> FileRedirectList;
typedef BOOL( __fastcall * GameGetFile )( const char * filename, int * OutDataPointer, size_t * OutSize, BOOL unknown );
BOOL __fastcall GameGetFile_my( const char * filename, int * OutDataPointer, unsigned int * OutSize, BOOL unknown );
extern GameGetFile GameGetFile_org, GameGetFile_ptr;

//
//
//typedef BOOL( __stdcall * SFileOpenFile )( const char* filename, int * handle );
//typedef BOOL( __stdcall * SFileReadFile )( int file_handle, void* buffer, uint32_t size, uint32_t* size_ptr, uint32_t unk );
//
//extern SFileOpenFile SFileOpenFile_org, SFileOpenFile_ptr;
//extern SFileReadFile SFileReadFile_org, SFileReadFile_ptr;
//
//
//BOOL __stdcall SFileOpenFile_my( const char* filename, int * handle );
//BOOL __stdcall SFileReadFile_my( int file_handle, void* buffer, uint32_t size, uint32_t* size_ptr, uint32_t unk );

void FreeAllIHelpers( );


typedef int( __fastcall * p_GetTypeInfo )( int unit_item_code, int unused );
extern p_GetTypeInfo GetTypeInfo;



extern vector<FakeFileStruct> FakeFileList;
extern BOOL NeedReleaseUnusedMemory;
std::vector<std::string> get_file_list( const fs::path & path, bool dotolower = false );
std::string GetFileContent( std::string filename );

BOOL __stdcall CheckWriteAccess( int a1 = 0 );
void __stdcall EnableLocalFiles( BOOL enable );
BOOL __stdcall IsLocalFilesEnabled( int a1 = 0 );
#pragma endregion


#pragma region DotaFovFix.cpp
extern BOOL EnableFixFOV;
int __stdcall SetWidescreenFixState( BOOL widefixenable );
int __stdcall SetCustomFovFix( float _CustomFovFix );
extern Matrix1 globalmatrix;
void __fastcall SetGameAreaFOV_my( Matrix1 * a1, int a2, float a3, float a4, float a5, float a6 );
typedef int( __fastcall * SetGameAreaFOV )( Matrix1 * a1, int a2, float a3, float a4, float a5, float a6 );
extern SetGameAreaFOV SetGameAreaFOV_org, SetGameAreaFOV_ptr;
#pragma endregion


#pragma region DotaWebHelper.cpp
string SendHttpPostRequest( const char * url, const char * data );
string SendHttpGetRequest( const char * host, const char * path );
extern string LatestDownloadedString;
#pragma endregion 


#pragma region DotaAutoFPSlimit.cpp
extern BOOL FPS_LIMIT_ENABLED;
extern HANDLE Warcraft3_Process;
extern unsigned int CPU_cores;
void InitThreadCpuUsage( );
double GetWar3CpuUsage( );
void UpdateFPS( );
typedef int( __cdecl * p_SetMaxFps )( int maxfps );
extern p_SetMaxFps _SetMaxFps;
typedef void( __fastcall * pDrawBarForUnit )( int unitaddr );
void __fastcall DrawBarForUnit_my( int unitaddr );
extern pDrawBarForUnit DrawBarForUnit_org, DrawBarForUnit_ptr;
//extern map<int, BOOL> NeedDrawBarForUnit;
extern BOOL FPSfix1Enabled;
#pragma endregion


#pragma region DotaChatHelper.cpp
typedef int( __fastcall * pGameChatSetState )( int chat, int unused, BOOL IsOpened );
extern pGameChatSetState GameChatSetState;

typedef int( __fastcall * pSetChatTargetUsers/*sub_6F3412F0*/ )( int chataddr, int ecx, int valtype );
extern pSetChatTargetUsers pSetChatTargetUsers_org;
extern pSetChatTargetUsers pSetChatTargetUsers_ptr;
int __fastcall SetChatTargetUsers_my( int chataddr, int ecx, int valtype );

extern LPARAM lpRShiftScanKeyUP;
extern LPARAM lpRShiftScanKeyDOWN;

extern LPARAM lpShiftScanKeyUP;
extern LPARAM lpShiftScanKeyDOWN;

extern LPARAM lpLShiftScanKeyUP;
extern LPARAM lpLShiftScanKeyDOWN;

extern int _EventVtable;
extern int _ChatSendEvent;
#pragma endregion


#pragma region DotaCustomFrames.cpp
extern BOOL usedcustomframes;
extern int pCurrentFrameFocusedAddr;
extern int DefaultCStatus;
extern int LoadFramesVar1;
extern int LoadFramesVar2;
typedef void( __fastcall * pLoadNewFrameDef )( const char * filename, int var1, int var2, int cstatus );
extern pLoadNewFrameDef LoadNewFrameDef_org;
typedef int( __fastcall * pCreateNewFrame ) ( const char * FrameName, int pGlobalGameClass, int unk1, int unk2, int unk3 );
extern pCreateNewFrame CreateNewFrame;
typedef void( __thiscall * pSetFramePos )( int FrameAddr180, int rowid, float left, float bottom, BOOL unk );
extern pSetFramePos SetFramePos;
//sub_6F606770
typedef void *( __thiscall * pShowFrameAlternative )( int FrameAddr180, int unk1_0, int pGlobalGameClass180, int unk1_1, float x, float y, BOOL unk );
extern pShowFrameAlternative ShowFrameAlternative;
typedef void *( __thiscall * pShowThisFrame )( int FrameAddr );
extern pShowThisFrame ShowThisFrame;
typedef void *( __thiscall * pDestructThisFrame )( int FrameAddr, BOOL unk );
extern pDestructThisFrame DestructThisFrame;//Without clean memory
typedef int( __fastcall * pGetFrameItemAddress )( const char * name, int id );
extern pGetFrameItemAddress GetFrameItemAddress;
typedef int( __thiscall * pUpdateFrameFlags )( int FrameAddr, char unk );
extern pUpdateFrameFlags UpdateFrameFlags;
typedef int( __thiscall *  pWc3ControlClickButton )( int btnaddr, int unk );
extern pWc3ControlClickButton Wc3ControlClickButton_org, Wc3ControlClickButton_ptr;
int __fastcall Wc3ControlClickButton_my( int btnaddr, int, int unk );
int __stdcall ShowConfigWindow( const char * );
extern BOOL ConfigWindowCreated;
void ProcessClickAtCustomFrames( );
extern BOOL GlyphButtonCreated;
#pragma endregion


#pragma region Overlays
// for all warcraft versions
void UninitOpenglHook( );
void InitOpenglHook( );

// 1.26a
void Uninitd3d8Hook( BOOL cleartextures );
void Initd3d8Hook( );

// 1.27a
void DrawOverlayDx8( );
void DrawOverlayGl( );

void SetNewLightDx8( int id );
void SetOldLightDx8( int id );

extern BOOL OverlayDrawed;

void InitD3DVSync(BOOL enabled);

#pragma endregion


#pragma region MemoryHack
//Get hModule from addr
HMODULE GetModuleFromAddress( int addr );
typedef int( __cdecl * GetTownUnitCount_p )( int *, int, BOOL );
int __cdecl Wc3MemoryRW( int * addr, int value, BOOL write );
extern GetTownUnitCount_p GetTownUnitCount_org;
extern GetTownUnitCount_p GetTownUnitCount_ptr;
int __stdcall GetJassStringCount( BOOL dump );
int __stdcall ScanJassStringForErrors( BOOL dump );

int IsOkayPtr( void *ptr, unsigned int size = 4 );
#pragma endregion

const float DesktopScreen_Width = ( float )GetSystemMetrics( SM_CXSCREEN );
const float DesktopScreen_Height = ( float )GetSystemMetrics( SM_CYSCREEN );

extern float DefaultSceenWidth;
extern float DefaultSceenHeight;

#pragma region DotaDesyncScan
extern int ScanId;


#pragma endregion



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


void SetTlsForMe( );
void __stdcall Packet_Initialize( int TriggerHandle );
/* Voice chat. Works only in a single player game :( 
In the local game or on the Internet there is a limit on sending data size, so the voice is recorded no more than one second! It is necessary to find and destroy the sending limit.
void UninitializeVoiceClient( );
void InitVoiceClientThread( );
void AddNewPaTestData( std::vector<unsigned char> _samples, int playerid, int packetsize, bool compressed );
*/






bool IsAnyHotkeyIsActive();
bool IsAnyEditBoxIsActive( );

void DisableInputForAnyHotkeyAndEditBox( );

void CurrentEditBoxRemoveCharacter( bool right = false );
void CurrentEditBoxMoveCursorLeft( );
void CurrentEditBoxMoveCursorRight( );
void CurrentEditBoxEnterText( std::wstring text );

void InitializeDreamDotaAPI(BOOL config,HMODULE _GameDll, HWND war3hwnd);
void UninitializeDreamDotaAPI();
void RegisterConfigWindow();