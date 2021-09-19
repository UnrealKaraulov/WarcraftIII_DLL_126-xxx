#include "Main.h"
#include "ViewAllySkill.h"

#include <TlHelp32.h>

#include <stdint.h>
#include "Crc32Dynamic.h"
#include "Storm.h"


#include "DotaClickHelper.h"


#include <Tools.h>
#include <EventData.h>
#include <Game.h>
#include <War3Window.h>
#include <Timer.h>
#include <Input.h>
#include "DotaDreamRawImage.h"



#include "RawImageApi.h"


BOOL FileExist(const char * name)
{
	ifstream f(name);
	return f.good();
}


//DWORD GameDllsz = 0;
//DWORD StormDLLsz = 0;


BOOL SetInfoObjDebugVal = TRUE;

#pragma region All Offsets Here

//  Game.dll
int GameDll = 0;
//	Storm.dll
int StormDll = 0;
HMODULE GameDllModule = 0;
HMODULE StormDllModule = 0;
HMODULE GetCurrentModule = NULL;


char buffer[4096];

int DrawSkillPanelOffset = 0;
int DrawSkillPanelOverlayOffset = 0;

int IsDrawSkillPanelOffset = 0;
int IsDrawSkillPanelOverlayOffset = 0;
int IsNeedDrawUnitOriginOffset = 0;
int IsNeedDrawUnit2offset = 0;
int IsNeedDrawUnit2offsetRetAddress = 0;
int Wc3ControlClickButton_offset = 0;

int * InGame = 0;
int _GlobalGlueObj = 0;
int _GameUI = 0;
BOOL * IsWindowActive = 0;
int ChatFound = 0;

int pW3XGlobalClass = 0;
int pGameClass1 = 0;
int pWar3GlobalData1 = 0;

int UnitVtable = 0;
int ItemVtable = 0;

int pPrintText2 = 0;


int MapNameOffset1 = 0;
int MapNameOffset2 = 0;

int pOnChatMessage_offset;

int _BarVTable = 0;

int pAttackSpeedLimit = 0;

int GetItemInSlotAddr = 0;

float * GetWindowXoffset = 0;
float * GetWindowYoffset = 0;


int StormErrorHandlerOffset = 0;
int JassNativeLookupOffset = 0;
int JassFuncLookupOffset = 0;
int ProcessNetEventsOffset = 0;
int BlizzardDebug1Offset = 0;
int BlizzardDebug2Offset = 0;
int BlizzardDebug3Offset = 0;
int BlizzardDebug4Offset = 0;
int BlizzardDebug5Offset = 0;
int BlizzardDebug6Offset = 0;
int GameFrameAtMouseStructOffset = 0;
int pTriggerExecute = 0;
int SetGameAreaFOVoffset = 0;
int GameGetFileOffset = 0;

int Warcraft3WindowProcOffset = 0;

int pPreferencesOffset = 0;
int pCurrentFrameFocusedAddr = 0;
pGetPlayerAlliance GetPlayerAlliance;


int DrawUnitBarOffset = 0;


void __stdcall DisableAllHooks();
#pragma endregion

BOOL MainFuncWork = FALSE;

int GetGlobalClassAddr()
{
	if (pW3XGlobalClass == NULL)
		return 0;

	return *(int*)pW3XGlobalClass;
}



int ChatEditBoxVtable = 0;


BOOL IsChatActive()
{
	return isChatBoxOn();
	//return  *(int*)pCurrentFrameFocusedAddr  && **(int**)pCurrentFrameFocusedAddr == GameDll + ChatEditBoxVtable;
}

BOOL IsGameFrameActive()
{


	if (IsChatActive())
	{
		return FALSE;
	}

	BOOL a1 = TRUE, a2 = TRUE, a3 = FALSE;


	int pGlAddr = GetGlobalClassAddr();
	if (pGlAddr > 0)
	{
		pGlAddr = *(int*)(pGlAddr + 0x3D0);
		if (pGlAddr > 0)
		{
			pGlAddr = *(int*)(pGlAddr + 0x164);

			a1 = pGlAddr > 0;
		}
	}

	pGlAddr = GetGlobalClassAddr();
	if (pGlAddr > 0)
	{
		pGlAddr = *(int*)(pGlAddr + 0x258);
		a2 = pGlAddr != 1;
	}



	if (*(int*)pCurrentFrameFocusedAddr == 0)
	{
		a3 = TRUE;
	}


	return a3 && a2 && a1;
}



#pragma optimize("",off)


void PrintText(const char * text, float staytime, BOOL force)
{
	if (IsGame() || force)
	{
		__asm
		{
			push - 1;
			push staytime;
			push text;
			mov ecx, pW3XGlobalClass;
			mov ecx, [ecx];
			mov eax, pPrintText2;
			call eax;
		}
	}
}


void PrintText(std::string strtext, float staytime, BOOL force)
{
	const char * text = strtext.c_str();
	if (IsGame() || force)
	{
		__asm
		{
			push - 1;
			push staytime;
			push text;
			mov ecx, pW3XGlobalClass;
			mov ecx, [ecx];
			mov eax, pPrintText2;
			call eax;
		}
	}
}


const char * GetBoolStr(BOOL val)
{
	return val ? "TRUE" : "FALSE";
}

#pragma optimize("",on)

LPVOID TlsValue;
DWORD TlsIndex;
DWORD _W3XTlsIndex;

DWORD GetIndex()
{
	return *(DWORD*)(_W3XTlsIndex);
}

DWORD GetW3TlsForIndex(DWORD index)
{
	DWORD pid = GetCurrentProcessId();
	THREADENTRY32 te32;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, pid);
	te32.dwSize = sizeof(THREADENTRY32);

	if (Thread32First(hSnap, &te32))
	{
		do
		{
			if (te32.th32OwnerProcessID == pid)
			{
				HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, false, te32.th32ThreadID);
				if (hThread)
				{
					CONTEXT ctx = { CONTEXT_SEGMENTS };
					LDT_ENTRY ldt;
					GetThreadContext(hThread, &ctx);
					GetThreadSelectorEntry(hThread, ctx.SegFs, &ldt);
					DWORD dwThreadBase = ldt.BaseLow | (ldt.HighWord.Bytes.BaseMid <<
						16u) | (ldt.HighWord.Bytes.BaseHi << 24u);
					CloseHandle(hThread);
					if (dwThreadBase == NULL)
						continue;
					DWORD* dwTLS = *(DWORD**)(dwThreadBase + 0xE10 + 4 * index);
					if (dwTLS == NULL)
						continue;
					return (DWORD)dwTLS;
				}
			}
		} while (Thread32Next(hSnap, &te32));
	}

	return NULL;
}

void SetTlsForMe()
{
	TlsIndex = GetIndex();
	LPVOID tls = (LPVOID)GetW3TlsForIndex(TlsIndex);
	TlsValue = tls;
	LPVOID oldtlsvalue = TlsGetValue(TlsIndex);
	if (oldtlsvalue != TlsValue)
		TlsSetValue(TlsIndex, TlsValue);
}


void __stdcall UnloadAllOldHelpers(int)
{
	HMODULE hMods[2048];
	DWORD cbNeeded;
	unsigned int i;
	if (EnumProcessModules(GetCurrentProcess(), hMods, sizeof(hMods), &cbNeeded))
	{
		for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			if (GetProcAddress(hMods[i], "InitDotaHelper") && hMods[i] != GetCurrentModule)
			{
				FreeLibrary(hMods[i]);
			}
		}
	}

}


BOOL ForceGameStart = FALSE;

void SetGameFound(const Event *)
{
	//MessageBoxA(0, "Force game start", "", 0);
	ForceGameStart = TRUE;
}


#pragma optimize("",off)

typedef void(WINAPI *PSleep)(DWORD ms);
typedef BOOL(WINAPI *PFreeLibrary)(HMODULE hModule);
typedef VOID(WINAPI *PExitThread)(DWORD dwExitCode);
typedef unsigned int (WINAPI *PTHREADPROC)(LPVOID lParam);

typedef struct _DLLUNLOADINFO {
	PFreeLibrary	m_fpFreeLibrary;
	PSleep m_fpSleep;
	PExitThread		m_fpExitThread;
	HMODULE		m_hFreeModule;
}DLLUNLOADINFO, *PDLLUNLOADINFO;

DWORD WINAPI DllUnloadThreadProc(LPVOID lParam)
{
	PDLLUNLOADINFO pDllUnloadInfo = (PDLLUNLOADINFO)lParam;
	(pDllUnloadInfo->m_fpSleep)(5000);
	(pDllUnloadInfo->m_fpFreeLibrary)(pDllUnloadInfo->m_hFreeModule);
	pDllUnloadInfo->m_fpExitThread(0);
	return 0;
}

VOID DllSelfUnloading(_In_ const HMODULE hModule)
{
	PVOID pMemory = NULL;
	pMemory = VirtualAlloc(NULL, 0x1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (pMemory != NULL)
	{
		SIZE_T rbytes = 0;
		if (ReadProcessMemory(GetCurrentProcess(), DllUnloadThreadProc, pMemory, 0x500, &rbytes) && rbytes > 0)
		{
			((PDLLUNLOADINFO)(((DWORD)pMemory) + 0x500))->m_fpFreeLibrary = FreeLibrary;
			((PDLLUNLOADINFO)(((DWORD)pMemory) + 0x500))->m_fpSleep = Sleep;
			((PDLLUNLOADINFO)(((DWORD)pMemory) + 0x500))->m_fpExitThread = ExitThread;
			((PDLLUNLOADINFO)(((DWORD)pMemory) + 0x500))->m_hFreeModule = hModule;

			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)pMemory,
				(PVOID)(((DWORD)pMemory) + 0x500), 0, 0);
		}
	}
}
#pragma optimize("",on)






BOOL SELF_UNLOAD_DLL_AFTER_GAME_END = FALSE;

void __stdcall SET_SELF_UNLOAD_DLL_AFTER_GAME_END(BOOL unload)
{
	SELF_UNLOAD_DLL_AFTER_GAME_END = unload;
}

void SetGameEnd(const Event *)
{
	//MessageBoxA(0, "Force game end","", 0);
	if (ForceGameStart)
	{
		DisableAllHooks();
		if (SELF_UNLOAD_DLL_AFTER_GAME_END)
			DllSelfUnloading(GetCurrentModule);
	}

	ForceGameStart = FALSE;
}



BOOL IsGame()
{
	return ForceGameStart /*&& GameUIObjectGet( ) != NULL*/;
}

pConvertStrToJassStr str2jstr;



pGame_Wc3MessageBox Game_Wc3MessageBox;



// Warning = 0
// Error = 1
// Question = 2
int __stdcall Wc3MessageBox(const char * message, int type)
{
	if (!InitFunctionCalled)
		return 0;
	Game_Wc3MessageBox(type, message, 0, 0, 0, 0, 0);
	return TRUE;
}


char MyFpsString[512];

//sub_6F004390
char *  GetWar3Preferense(int ID)
{
	int pPrefAddr = *(int*)pPreferencesOffset + 40;
	char * result = 0;
	if (pPrefAddr > 0)
	{
		int pPrefVal = *(int *)(pPrefAddr + 36);
		if (pPrefVal != -1)
		{
			result = *(char **)(*(int *)(pPrefAddr + 28) + 12 * (ID & pPrefVal) + 8);
			if (!result)
			{

			}
			else
			{
				while (*(int *)result != ID)
				{
					result = *(char **)&result[*(int *)(*(int *)(ID + 28) + 12 * (*(int *)(pPrefAddr + 36) & ID)) + 4];
					if (!result)
					{
						break;
					}
				}
			}
		}
	}
	return result > 0 ? result : 0;
}


pLoadFrameDefList LoadFrameDefList;



int __stdcall SetMainFuncWork(BOOL state)
{
	MainFuncWork = state;
	return 0;
}



BOOL BlockKeyAndMouseEmulation = FALSE;
BOOL EnableSelectHelper = FALSE;
BOOL DoubleClickHelper = FALSE;



_TriggerExecute TriggerExecute;
pExecuteFunc ExecuteFunc;

char CurrentMapPath[MAX_PATH];
char NewMapPath[MAX_PATH];


void SaveCurrentMapPath()
{

	memset(CurrentMapPath, 0, MAX_PATH);
	int offset1 = *(int*)MapNameOffset1;
	if (offset1 > 0)
	{
		const char * mPath = (const char *)(offset1 + MapNameOffset2);
		sprintf_s(CurrentMapPath, MAX_PATH, "%s", mPath);
	}
}


void BuildFilePath(char * fname)
{

	SaveCurrentMapPath();


	unsigned int MapPathSize = strlen(CurrentMapPath);
	if (MapPathSize > 8)// "Maps\a.w3x"
	{
		for (unsigned int i = MapPathSize - 1; i > 0; i--)
		{
			if (CurrentMapPath[i] != '\\')
			{
				CurrentMapPath[i] = '\0';
			}
			else
				break;
		}

		MapPathSize = strlen(CurrentMapPath);
		if (MapPathSize > 4) // "Maps\"
		{

			memset(NewMapPath, 0, MAX_PATH);
			if (fname != NULL && *fname != 0)
				sprintf_s(NewMapPath, MAX_PATH, "%s%s", CurrentMapPath, fname);
			else
				sprintf_s(NewMapPath, MAX_PATH, "%s", CurrentMapPath);

		}
	}
}




const char * __stdcall GetCurrentMapPath(int)
{
	BuildFilePath(NULL);
	return CurrentMapPath;
}

BOOL OverlayDrawed = FALSE;
int WriteAccessStatus = -1;

typedef int(__fastcall * DrawInterface_p)(int, int);
DrawInterface_p DrawInterface_org;
DrawInterface_p DrawInterface_ptr;

int __fastcall DrawInterface_my(int arg1, int arg2)
{
	if (IsGame()/* && *IsWindowActive*/)
	{
		//if ( WriteAccessStatus == -1 )
		//{
		//	WriteAccessStatus = CheckWriteAccess( );
		//}


		DrawOverlayDx8();
		DrawOverlayGl();
		OverlayDrawed = TRUE;


	}

	return DrawInterface_ptr(arg1, arg2);
}



int RenderStage;

typedef void(__fastcall * Wc3DrawStage)(int arg1, int unused, int RenderStage, int type1, int type2, int arg1_2);
Wc3DrawStage Wc3DrawStage_org;
Wc3DrawStage Wc3DrawStage_ptr;

void __fastcall Wc3DrawStage_my(int arg1, int unused, int _RenderStage, int type1, int type2, int arg1_2)
{
	RenderStage = _RenderStage;
	Wc3DrawStage_ptr(arg1, unused, _RenderStage, type1, type2, arg1_2);
	RenderStage = -_RenderStage;
}


typedef signed int(__fastcall * Wc3DrawObject)(int a1, int a2);
Wc3DrawObject Wc3DrawObject_ptr;//sub_6F60FC00
Wc3DrawObject Wc3DrawObject_org;

int unlimunitaddr = -1;

int __stdcall SetUnitForUnlimiteDraw(int unitaddr)
{
	unlimunitaddr = unitaddr;
	return 0;
}

int __fastcall Wc3DrawObject_my(int a1, int a2)
{

	return 0;
}



void InitHook()
{


	//EnableErrorHandler(0);


	if (!Warcraft3Window)
	{
		Warcraft3Window = FindWindowA("Warcraft III", 0);
		if (!Warcraft3Window)
			Warcraft3Window = FindWindowA(0, "Warcraft III");
	}

	if (Warcraft3Window)
	{
		WarcraftRealWNDProc_org = (WarcraftRealWNDProc)Warcraft3WindowProcOffset;

		MH_CreateHook(WarcraftRealWNDProc_org, &WarcraftWindowProcHooked, reinterpret_cast<void**>(&WarcraftRealWNDProc_ptr));
		MH_EnableHook(WarcraftRealWNDProc_org);

	}
	else
	{
	}

	SetGameAreaFOV_org = (SetGameAreaFOV)(SetGameAreaFOVoffset + GameDll);

	MH_CreateHook(SetGameAreaFOV_org, &SetGameAreaFOV_my, reinterpret_cast<void**>(&SetGameAreaFOV_ptr));

	MH_EnableHook(SetGameAreaFOV_org);



	// Установить адрес для IsDrawSkillPanel_org
	IsDrawSkillPanel_org = (IsDrawSkillPanel)(IsDrawSkillPanelOffset + GameDll);
	// Создать хук (перехват) для IsDrawSkillPanel_org и сохранить его в памяти
	MH_CreateHook(IsDrawSkillPanel_org, &IsDrawSkillPanel_my, reinterpret_cast<void**>(&IsDrawSkillPanel_ptr));
	// Активировать хук для IsDrawSkillPanel_org
	MH_EnableHook(IsDrawSkillPanel_org);


	// Установить адрес для IsDrawSkillPanelOverlay_org
	IsDrawSkillPanelOverlay_org = (IsDrawSkillPanelOverlay)(IsDrawSkillPanelOverlayOffset + GameDll);
	// Создать хук (перехват) для IsDrawSkillPanel_org и сохранить его в памяти
	MH_CreateHook(IsDrawSkillPanelOverlay_org, &IsDrawSkillPanelOverlay_my, reinterpret_cast<void**>(&IsDrawSkillPanelOverlay_ptr));
	// Активировать хук для IsDrawSkillPanel_org
	MH_EnableHook(IsDrawSkillPanelOverlay_org);


	// Установить адрес для IsNeedDrawUnit2org
	IsNeedDrawUnit2org = (IsNeedDrawUnit2)(IsNeedDrawUnit2offset + GameDll);
	// Создать хук (перехват) для IsNeedDrawUnit2org и сохранить его в памяти
	MH_CreateHook(IsNeedDrawUnit2org, &IsNeedDrawUnit2_my, reinterpret_cast<void**>(&IsNeedDrawUnit2ptr));
	// Активировать хук для IsNeedDrawUnit2org
	MH_EnableHook(IsNeedDrawUnit2org);



	GameGetFile_org = (GameGetFile)(GameDll + GameGetFileOffset);
	MH_CreateHook(GameGetFile_org, &GameGetFile_my, reinterpret_cast<void**>(&GameGetFile_ptr));
	MH_EnableHook(GameGetFile_org);



	//SFileReadFile_org = ( SFileReadFile )( GetProcAddress( StormModule, ( LPCSTR )269 ) );
	//MH_CreateHook( SFileReadFile_org, &SFileReadFile_my, reinterpret_cast< void** >( &SFileReadFile_ptr ) );
	//MH_EnableHook( SFileReadFile_org );


	//SFileOpenFile_org = ( SFileOpenFile )( GetProcAddress( StormModule, ( LPCSTR )267 ) );
	//MH_CreateHook( SFileOpenFile_org, &SFileOpenFile_my, reinterpret_cast< void** >( &SFileOpenFile_ptr ) );
	//MH_EnableHook( SFileOpenFile_org );


	pOnChatMessage_org = (pOnChatMessage)(GameDll + pOnChatMessage_offset);
	MH_CreateHook(pOnChatMessage_org, &pOnChatMessage_my, reinterpret_cast<void**>(&pOnChatMessage_ptr));
	MH_EnableHook(pOnChatMessage_org);

	Wc3ControlClickButton_org = (pWc3ControlClickButton)(Wc3ControlClickButton_offset);
	MH_CreateHook(Wc3ControlClickButton_org, &Wc3ControlClickButton_my, reinterpret_cast<void**>(&Wc3ControlClickButton_ptr));
	MH_EnableHook(Wc3ControlClickButton_org);


	DrawBarForUnit_org = (pDrawBarForUnit)(DrawUnitBarOffset);
	MH_CreateHook(DrawBarForUnit_org, &DrawBarForUnit_my, reinterpret_cast<void**>(&DrawBarForUnit_ptr));
	MH_EnableHook(DrawBarForUnit_org);

	MH_CreateHook(SimpleButtonClickEvent_org, &SimpleButtonClickEvent_my, reinterpret_cast<void**>(&SimpleButtonClickEvent_ptr));
	MH_EnableHook(SimpleButtonClickEvent_org);



	IssueFixerInit();

	/*MH_CreateHook( DrawWc3UI_org, &DrawWc3UI_my, reinterpret_cast< void** >( &DrawWc3UI_ptr ) );
	MH_EnableHook( DrawWc3UI_org );

	MH_CreateHook( DrawWc3Cursor_org, &DrawWc3Cursor_my, reinterpret_cast< void** >( &DrawWc3Cursor_ptr ) );
	MH_EnableHook( DrawWc3Cursor_org );*/

	MH_CreateHook(DrawInterface_org, &DrawInterface_my, reinterpret_cast<void**>(&DrawInterface_ptr));
	MH_EnableHook(DrawInterface_org);

	MH_CreateHook(GetTownUnitCount_org, &Wc3MemoryRW, reinterpret_cast<void**>(&GetTownUnitCount_ptr));
	MH_EnableHook(GetTownUnitCount_org);


	//MH_CreateHook( Wc3DrawStage_org, &Wc3DrawStage_my, reinterpret_cast< void** >( &Wc3DrawStage_ptr ) );
	//MH_EnableHook( Wc3DrawStage_org );

	GetTimer(0.20, PressKeyWithDelay_timed, true, TimeType::TimeGame)->start();

}

void UninitializeHook()
{


#pragma region Game.dll hook


	// Отключить хук для IsDrawSkillPanel_org
	if (IsDrawSkillPanel_org)
	{
		MH_DisableHook(IsDrawSkillPanel_org);
		IsDrawSkillPanel_org = 0;
	}
	// Отключить хук для IsDrawSkillPanelOverlay_org
	if (IsDrawSkillPanelOverlay_org)
	{
		MH_DisableHook(IsDrawSkillPanelOverlay_org);
		IsDrawSkillPanelOverlay_org = 0;
	}
	// Отключить хук для IsNeedDrawUnit2org
	if (IsNeedDrawUnit2org)
	{
		MH_DisableHook(IsNeedDrawUnit2org);
		IsNeedDrawUnit2org = 0;
	}

	if (pOnChatMessage_org)
	{
		MH_DisableHook(pOnChatMessage_org);
		pOnChatMessage_org = 0;
	}

	if (Wc3ControlClickButton_org)
	{
		MH_DisableHook(Wc3ControlClickButton_org);
		Wc3ControlClickButton_org = 0;
	}

	if (SetGameAreaFOV_org)
	{
		MH_DisableHook(SetGameAreaFOV_org);
		SetGameAreaFOV_org = 0;
	}

	if (GameGetFile_org)
	{
		MH_DisableHook(GameGetFile_org);
		GameGetFile_org = 0;
	}

	/*if ( SFileReadFile_org )
	{
		MH_DisableHook( SFileReadFile_org );
		SFileReadFile_org = 0;
	}

	if ( SFileOpenFile_org )
	{
		MH_DisableHook( SFileOpenFile_org );
		SFileOpenFile_org = 0;
	}*/

	if (DrawInterface_org)
	{
		MH_DisableHook(DrawInterface_org);
		DrawInterface_org = NULL;
	}

	if (GetTownUnitCount_org)
	{
		MH_DisableHook(GetTownUnitCount_org);
		DrawInterface_org = NULL;
	}


	if (DrawBarForUnit_org)
	{
		MH_DisableHook(DrawBarForUnit_org);
		DrawBarForUnit_org = NULL;
	}

	if (SimpleButtonClickEvent_org)
	{
		MH_DisableHook(SimpleButtonClickEvent_org);
		SimpleButtonClickEvent_org = NULL;
	}

	if (Wc3DrawStage_org)
	{
		MH_DisableHook(Wc3DrawStage_org);
		Wc3DrawStage_org = NULL;
	}

	if (SimpleButtonPreClickEvent_org)
	{
		MH_DisableHook(SimpleButtonPreClickEvent_org);
		SimpleButtonPreClickEvent_org = NULL;
	}

	if (pSetChatTargetUsers_org)
	{
		MH_DisableHook(pSetChatTargetUsers_org);
		pSetChatTargetUsers_org = NULL;
	}

	if (SimpleButtonPreClickEvent_org)
	{
		MH_DisableHook(SimpleButtonPreClickEvent_org);
		SimpleButtonPreClickEvent_org = NULL;
	}


#pragma endregion

	IssueFixerDisable();
}


typedef int(__stdcall * pStorm_503)(int a1, int a2, int a3);
pStorm_503 Storm_503;

BOOL PlantDetourJMP(unsigned char* source, const unsigned char* destination, size_t length)
{

	DWORD oldProtection;
	BOOL bRet = VirtualProtect(source, length, PAGE_EXECUTE_READWRITE, &oldProtection);

	if (bRet == FALSE)
		return FALSE;

	source[0] = 0xE9;
	*(DWORD*)(source + 1) = (DWORD)(destination - source) - 5;

	for (unsigned int i = 5; i < length; i++)
		source[i] = 0x90;

	VirtualProtect(source, length, oldProtection, &oldProtection);
	FlushInstructionCache(GetCurrentProcess(), source, length);
	return TRUE;
}


#pragma optimize("",off)




char * bufferaddr = 0;



int __declspec(naked) __cdecl GetUnitItemInSlot126a(int unitaddr, int slotid)
{
	__asm
	{
		mov ecx, [esp + 4];
		push esi;
		mov eax, ecx;
		jmp GetItemInSlotAddr;
	}
}

int __declspec(naked) __cdecl GetUnitItemInSlot127a(int unitaddr, int slotid)
{
	__asm
	{
		push ebp;
		mov ebp, esp;
		mov ecx, [ebp + 8];
		push esi;
		mov eax, ecx;
		jmp GetItemInSlotAddr;
	}
}


typedef int(__cdecl * pGetItemTypeId)(int itemhandle);
pGetItemTypeId GetItemTypeId;

int __cdecl GetItemTypeInSlot(int unitaddr, int slotid)
{

	int itemhandle = 0;


	itemhandle = GetUnitItemInSlot126a(unitaddr, slotid);

	if (itemhandle)
	{
		return GetItemTypeId(itemhandle);
	}

	return 0;
}

int GetTypeId(int unit_item_abil_etc_addr)
{
	if (unit_item_abil_etc_addr)
		return *(int*)(unit_item_abil_etc_addr + 0x30);
	else
		return 0;
}

BOOL IsClassEqual(int ClassID1, int ClassID2)
{
	return ClassID1 == ClassID2;
}



std::vector<SpellBonusItemStruct> SpellBonusItemList;

void __stdcall AddSpellBonusItem(int id, int pc)
{
	if (id == 0)
	{
		SpellBonusItemList.clear();
	}
	else
	{
		SpellBonusItemList.push_back({ id,pc });
	}
}

int magicampval = 16;

void __stdcall SetMagicCampValue(int value)
{
	magicampval = value;
}

// Функция принимает данные о скорости атаки (и о увеличении урона от способностей) и сохраняет в буфер который будет использоваться при отрисовке
int __stdcall PrintAttackSpeedAndOtherInfo(int addr, float * attackspeed, float * BAT, int * unitaddr)
{
	int retval = 0;
	__asm mov retval, eax;
	if (unitaddr > 0)
	{

		if (IsNotBadUnit(*unitaddr) && IsHero(*unitaddr))
		{
			bufferaddr = buffer;
			float realBAT = *(float*)BAT;
			float fixedattackspeed = *(float*)attackspeed;
			float realattackspeed = fixedattackspeed;
			if (fixedattackspeed > *(float*)(GameDll + pAttackSpeedLimit))
				fixedattackspeed = *(float*)(GameDll + pAttackSpeedLimit);

			if (realattackspeed < 0.0f)
				realattackspeed = 0.01f;
			if (fixedattackspeed < 0.0f)
				fixedattackspeed = 0.01f;


			/*	if ( fixedattackspeed == 0 )
				{
					fixedattackspeed = 0.0001f;
				}

				if ( realBAT == 0 )
				{
					realBAT = 0.0001f;
				}*/

			int magicamp = GetHeroInt(*unitaddr, 0, TRUE) / magicampval;


			int magicampbonus = 0;

			for (int i = 0; i < 6; i++)
			{
				int item = GetItemTypeInSlot(*unitaddr, i);
				for (auto const & s : SpellBonusItemList)
				{
					if (IsClassEqual(item, s.id))
					{
						magicampbonus += s.pc;
					}
				}
			}

			float AttacksPerSec = 0.0f;

			float AttackReload = 0.0f;
			if (fixedattackspeed != 0.0f && realBAT != 0.0f)
			{
				AttacksPerSec = fixedattackspeed / realBAT;
				AttackReload = 1.0f / (fixedattackspeed / realBAT);
			}
			float AttackSpeedBonus = realattackspeed * 100.0f - 100.0f;

			if (magicampbonus)
				sprintf_s(buffer, sizeof(buffer), "%.1f/sec (Reload: %.2f sec)|nAttack speed bonus: %.0f|nSpell Damage: %i%% (|cFF20FF20+%i%%|r)|n", AttacksPerSec, AttackReload, AttackSpeedBonus, magicamp, magicampbonus);
			else
				sprintf_s(buffer, sizeof(buffer), "%.1f/sec (Reload: %.2f sec)|nAttack speed bonus: %.0f|nSpell Damage: %i%% (0%%)|n", AttacksPerSec, AttackReload, AttackSpeedBonus, magicamp);

			if (fixedattackspeed > *(float*)(GameDll + pAttackSpeedLimit))
				fixedattackspeed = *(float*)(GameDll + pAttackSpeedLimit);

			if (fixedattackspeed < 0.2f)
				fixedattackspeed = 0.2f;

			__asm
			{
				PUSH 0x200;
				PUSH bufferaddr;
				PUSH addr;
				CALL Storm_503;
			}
		}
		else
		{
			bufferaddr = buffer;
			float oldaddtackspeed = *(float*)attackspeed;
			float fixedattackspeed = oldaddtackspeed;

			if (fixedattackspeed > *(float*)(GameDll + pAttackSpeedLimit))
				fixedattackspeed = *(float*)(GameDll + pAttackSpeedLimit);

			if (fixedattackspeed < 0.2f)
				fixedattackspeed = 0.2f;

			sprintf_s(buffer, sizeof(buffer), "%.3f (Reload: %.2f sec)", (fixedattackspeed / *(float*)BAT), 1.0f / (fixedattackspeed / *(float*)BAT));

			__asm
			{
				PUSH 0x200;
				PUSH bufferaddr;
				PUSH addr;
				CALL Storm_503;
			}
		}

	}

	return retval;
}

int saveeax = 0;
int saveebx = 0;
int saveecx = 0;
int saveedx = 0;
int saveesi = 0;
int saveedi = 0;
int saveebp = 0;
int saveesp = 0;


void __declspec(naked)  PrintAttackSpeedAndOtherInfoHook126a()
{
	__asm
	{
		mov saveeax, eax;
		mov eax, [esp + 0x14];
		cmp eax, 0;
		JE JUSTEND;
		add eax, 0x30;
		push eax;
		add eax, 0x128;
		push eax;
		add eax, 0x58;
		push eax;
		push esi;
		call PrintAttackSpeedAndOtherInfo;
	JUSTEND:;
		mov eax, saveeax;
		ret 8;
	}
}


void __declspec(naked)  PrintAttackSpeedAndOtherInfoHook127a()
{
	__asm
	{
		mov saveeax, eax;
		mov eax, [esp + 0x10];
		cmp eax, 0;
		JE JUSTEND;
		add eax, 0x30;
		push eax;
		add eax, 0x128;
		push eax;
		add eax, 0x58;
		push eax;
		push ecx;
		call PrintAttackSpeedAndOtherInfo;
	JUSTEND:;
		mov eax, saveeax;
		ret 8;
	}
}

float __stdcall GetMagicProtectionForHero_org(int UnitAddr)
{
	float indmg = 100.0;
	if (IsNotBadUnit(UnitAddr))
	{


		unsigned int abilscount = 0;
		int * abils = FindUnitAbils(UnitAddr, &abilscount, 0, 'AIdd');
		for (unsigned int i = 0; i < abilscount; i++)
		{
			int pData = *(int*)(abils[i] + 0x54);
			if (pData != 0)
			{
				float DmgProt = *(float*)(pData + 0x20 + 0x68 * (*(int*)(abils[i] + 0x50) + 1));
				indmg = indmg * DmgProt;
			}
		}


		// new 
		if (*(int*)(UnitAddr + 0x158))
		{
			indmg *= 1.4;
		}



	}

	return (float)(100.0 - indmg);
}


// Only for game. Int retval = fix missing eax
int __stdcall GetMagicProtectionForHero(int UnitAddr)
{
	float retval = GetMagicProtectionForHero_org(UnitAddr);
	return *(int*)&retval;
}

float __stdcall GetMagicProtectionForHero_by_abiladdr(int abil_addr)
{
	if (abil_addr > 0)
	{
		return GetMagicProtectionForHero_org(*(int*)(abil_addr + 0x30));
	}
	return 0.0f;
}

int __stdcall PrintMoveSpeed(int addr, float * movespeed, int AmovAddr)
{
	int retval = 0;
	__asm mov retval, eax;
	if (AmovAddr > 0)
	{

		float MagicProtection = GetMagicProtectionForHero_by_abiladdr(AmovAddr);
		bufferaddr = buffer;

		if (MagicProtection == 0.0f)
			sprintf_s(buffer, sizeof(buffer), "%.1f", (*(float*)movespeed));
		else if (MagicProtection > 30.0f)
			sprintf_s(buffer, sizeof(buffer), "%.1f|nMagic Protection: |cFF00C800%.1f|r%%", (*(float*)movespeed), MagicProtection);
		else if (MagicProtection <= 30.0f && MagicProtection > 0.0f)
			sprintf_s(buffer, sizeof(buffer), "%.1f|nMagic Protection: %.1f%%", (*(float*)movespeed), MagicProtection);
		else
			sprintf_s(buffer, sizeof(buffer), "%.1f|nMagic Protection: |cFFD82005%.1f|r%%", (*(float*)movespeed), MagicProtection);
		__asm
		{
			PUSH 0x200;
			PUSH bufferaddr;
			PUSH addr;
			CALL Storm_503;
		}
	}
	return retval;
}


void __declspec(naked)  PrintMoveSpeedHook126a()
{
	__asm
	{
		mov saveeax, eax;
		mov eax, esp;
		add eax, 4;
		push ebx;
		push eax;
		push esi;
		call PrintMoveSpeed;
		mov eax, saveeax;
		ret 8;
	}
}

void __declspec(naked)  PrintMoveSpeedHook127a()
{
	__asm
	{
		mov saveeax, eax;
		mov eax, esp;
		add eax, 4;
		push edi;
		push eax;
		push ecx;
		call PrintMoveSpeed;
		mov eax, saveeax;
		ret 8;
	}
}



char itemstr1[512];
char itemstr2[512];

char unitstr1[512];
char unitstr2[512];


unsigned int         PLAYER_COLOR_RED = 0;
unsigned int         PLAYER_COLOR_BLUE = 1;
unsigned int         PLAYER_COLOR_CYAN = 2;
unsigned int         PLAYER_COLOR_PURPLE = 3;
unsigned int         PLAYER_COLOR_YELLOW = 4;
unsigned int         PLAYER_COLOR_ORANGE = 5;
unsigned int         PLAYER_COLOR_GREEN = 6;
unsigned int         PLAYER_COLOR_PINK = 7;
unsigned int         PLAYER_COLOR_LIGHT_GRAY = 8;
unsigned int         PLAYER_COLOR_LIGHT_BLUE = 9;
unsigned int         PLAYER_COLOR_AQUA = 10;
unsigned int         PLAYER_COLOR_BROWN = 11;


const char * GetPlayerColorString2(int player)
{
	unsigned int c = GetPlayerColor2(player);
	if (c == PLAYER_COLOR_RED)
		return "|cffFF0202";
	else if (c == PLAYER_COLOR_BLUE)
		return "|cff0031FF";
	else if (c == PLAYER_COLOR_CYAN)
		return "|cff1BE5B8";
	else if (c == PLAYER_COLOR_PURPLE)
		return "|cff530080";
	else if (c == PLAYER_COLOR_YELLOW)
		return "|cffFFFC00";
	else if (c == PLAYER_COLOR_ORANGE)
		return "|cffFE890D";
	else if (c == PLAYER_COLOR_GREEN)
		return "|cff1FBF00";
	else if (c == PLAYER_COLOR_PINK)
		return "|cffE45AAF";
	else if (c == PLAYER_COLOR_LIGHT_GRAY)
		return "|cff949596";
	else if (c == PLAYER_COLOR_LIGHT_BLUE)
		return "|cff7DBEF1";
	else if (c == PLAYER_COLOR_AQUA)
		return "|cff0F6145";
	else if (c == PLAYER_COLOR_BROWN)
		return "|cff4D2903";
	else
		return "|cffFFFFFF";

}



int __stdcall SaveStringsForPrintItem(int itemaddr)
{
	if (itemaddr > 0)
	{

		if (IsNotBadItem(itemaddr))
		{
			int itemowner = *(int*)(itemaddr + 0x74);
			if (itemowner <= 15 && itemowner >= 0)
			{
				sprintf_s(itemstr1, 512, "Owned by %s%s|r|n%%s%%s%%s%%s%%s", GetPlayerColorString2(Player(itemowner)), GetPlayerName(itemowner, 0));
				sprintf_s(itemstr2, 512, "Owned by %s%s|r|n%%s%%s%%s", GetPlayerColorString2(Player(itemowner)), GetPlayerName(itemowner, 0));
				return itemaddr;
			}
		}
	}
	sprintf_s(itemstr1, 512, "%%s%%s%%s%%s%%s");
	sprintf_s(itemstr2, 512, "%%s%%s%%s");
	return itemaddr;
}


BOOL NeedDrawRegen = FALSE;


int __stdcall DrawRegenAllways(BOOL enabled)
{
	NeedDrawRegen = enabled;
	return enabled;
}


int __stdcall SaveStringForHP_MP(int unitaddr)
{
	if (*IsWindowActive && (NeedDrawRegen || IsKeyPressed(VK_LMENU)))
	{

		if (NeedDrawRegen && IsKeyPressed(VK_LMENU))
		{
			sprintf_s(unitstr1, 512, "%%u / %%u");
			sprintf_s(unitstr2, 512, "%%u / %%u");
			return unitaddr;
		}


		if (IsNotBadUnit(unitaddr))
		{
			float unitreghp = GetUnitHPregen(unitaddr);
			float unitregmp = GetUnitMPregen(unitaddr);

			if (unitreghp < 0.0f)
			{
				sprintf_s(unitstr1, 512, "%%u |cFF00FF00%.1f|r", unitreghp);
			}
			else if (unitreghp < 9999.0f)
			{
				sprintf_s(unitstr1, 512, "%%u |cFF00FF00+%.1f|r", unitreghp);
			}
			else
			{
				sprintf_s(unitstr1, 512, "%%u |cFF00FF00+BIG|r");
			}

			if (unitregmp < 0.0f)
			{
				sprintf_s(unitstr2, 512, "%%u |cFF00FFFF%.1f|r", unitregmp);
			}
			else if (unitregmp < 9999.0f)
			{
				sprintf_s(unitstr2, 512, "%%u |cFF00FFFF+%.1f|r", unitregmp);
			}
			else
			{
				sprintf_s(unitstr2, 512, "%%u |cFF00FFFF+BIG|r");
			}
			return unitaddr;
		}
	}
	sprintf_s(unitstr1, 512, "%%u / %%u");
	sprintf_s(unitstr2, 512, "%%u / %%u");
	return unitaddr;
}



#pragma region HookFunctions

int JumpBackAddr1()
{
	MessageBoxA(0, 0, 0, 1);
	return 0;
}

int JumpBackAddr2()
{
	MessageBoxA(0, 0, 0, 2);
	return 0;
}

int JumpBackAddr3()
{
	MessageBoxA(0, 0, 0, 3);
	return 0;
}


int JumpBackAddr4()
{
	MessageBoxA(0, 0, 0, 4);
	return 0;
}

int JumpBackAddr5()
{
	MessageBoxA(0, 0, 0, 5);
	return 0;
}

int JumpBackAddr6()
{
	MessageBoxA(0, 0, 0, 6);
	return 0;
}


int JumpBackAddr7()
{
	MessageBoxA(0, 0, 0, 7);
	return 0;
}


int JumpBackAddr8()
{
	MessageBoxA(0, 0, 0, 8);
	return 0;
}


int JumpBackAddr9()
{
	MessageBoxA(0, 0, 0, 9);
	return 0;
}



void __declspec(naked) HookHPBarColorHelper126a()
{
	__asm
	{
		lea edx, [esp + 0x10];
		fstp st(0);
		pushad;
		pusha;
		push edi;
		push edx;
		call SetColorForUnit;
		popa;
		popad;
		jmp JumpBackAddr9;
	}
}


int calladdr1 = 0;



void __declspec(naked) HookHPBarColorHelper127a()
{
	__asm
	{
		lea eax, [ebp + 0x08];
		pushad;
		pusha;
		push esi;
		push eax;
		call SetColorForUnit;
		popa;
		popad;
		push eax;
		call calladdr1;
		jmp JumpBackAddr9;
	}
}



void __declspec(naked) HookItemAddr126a()
{
	__asm
	{
		pushad;
		pusha;
		push eax;
		call SaveStringsForPrintItem;
		popa;
		popad;
		mov ebx, eax;
		test ebx, ebx;
		mov[esp + 0x1C], ebx;
		jmp JumpBackAddr1;
	}
}


void __declspec(naked) HookItemAddr127a()
{
	__asm
	{
		pushad;
		pusha;
		push eax;
		call SaveStringsForPrintItem;
		popa;
		popad;
		mov edi, eax;
		mov[ebp - 0x4F8], edi;
		jmp JumpBackAddr1;
	}
}



void __declspec(naked) HookUnitAddr126a()
{
	__asm
	{
		push edx;
		mov eax, ecx;
		pushad;
		pusha;
		push eax;
		call SaveStringForHP_MP;
		popa;
		popad;
		mov ecx, eax;
		pop edx;
		push edx;
		lea eax, [esp + 0x40];
		jmp JumpBackAddr4;
	}
}


void __declspec(naked) HookUnitAddr127a()
{
	__asm
	{
		mov ecx, [edi + 0x00000238];
		mov eax, ecx;
		pushad;
		pusha;
		push eax;
		call SaveStringForHP_MP;
		popa;
		popad;
		mov ecx, eax;
		test ecx, ecx;
		jmp JumpBackAddr4;
	}
}


char * GlobalTextBuffer = 0;


void __declspec(naked) HookPrint1_126a()
{
	bufferaddr = itemstr1;
	__asm
	{
		push bufferaddr;
		mov eax, GlobalTextBuffer;
		push 0x2000;
		jmp JumpBackAddr2;
	}
}


void __declspec(naked) HookPrint2_126a()
{
	bufferaddr = itemstr2;
	__asm
	{
		push bufferaddr;
		mov edx, GlobalTextBuffer;
		push 0x2000;
		jmp JumpBackAddr3;
	}
}



void __declspec(naked) HookPrint3_126a()
{
	bufferaddr = unitstr1;
	__asm
	{
		push bufferaddr;
		lea ecx, [esp + 0x0000100];
		push 0x28;
		jmp JumpBackAddr5;
	}
}

void __declspec(naked) HookPrint4_126a()
{
	bufferaddr = unitstr2;
	__asm
	{
		push bufferaddr;
		lea eax, [esp + 0x0000128];
		push 0x28;
		jmp JumpBackAddr6;
	}
}


void __declspec(naked) HookPrint1_127a()
{
	bufferaddr = itemstr1;
	__asm
	{
		push bufferaddr;
		lea eax, [ebp - 0x2F0];
		push 0x200;
		jmp JumpBackAddr2;
	}
}


void __declspec(naked) HookPrint2_127a()
{
	bufferaddr = itemstr2;
	__asm
	{
		push bufferaddr;
		lea eax, [ebp - 0x2F0];
		push 0x200;
		jmp JumpBackAddr3;
	}
}




void __declspec(naked) HookPrint3_127a()
{
	bufferaddr = unitstr1;
	__asm
	{
		push bufferaddr;
		lea eax, [ebp - 0x60];
		push 0x28;
		jmp JumpBackAddr5;
	}
}

void __declspec(naked) HookPrint4_127a()
{
	bufferaddr = unitstr2;
	__asm
	{
		push bufferaddr;
		lea eax, [ebp - 0x38];
		push 0x28;
		jmp JumpBackAddr6;
	}
}

void __stdcall SetCdForAddr(int cd_addr)
{
	if (cd_addr > 0xb0 /*eax */)
	{
		int abiladdr = cd_addr - 0xb0;
		int pData = *(int*)(abiladdr + 0xDC);
		if (pData > 0)
		{
			float val1 = *(float*)(pData + 0x4);
			int pData2 = *(int*)(pData + 0xC);
			if (pData2 > 0)
			{
				float val2 = *(float*)(pData2 + 0x40);
				float val3 = val1 - val2;
				if (val3 > 100)
					*(float*)(cd_addr + 4) = 1000.0f;
				else
					*(float*)(cd_addr + 4) = 100.0f;
				return;
			}

		}

	}

	if (*(float*)(cd_addr + 4) != 1000.0f)
		*(float*)(cd_addr + 4) = 100.0f;
}

void __declspec(naked) HookSetCD_1000s_126a()
{
	//int cd_addr;
	__asm
	{
		//	mov cd_addr, eax;
		pushad;
		pusha;
		push eax;
		call SetCdForAddr;
		popa;
		popad;
		push esi;
		push eax;
		mov eax, [ecx];
		mov eax, [eax + 0x18];
		lea edx, [esp + 0x08];
		push edx;
		jmp JumpBackAddr7;
	}
}

//37ed3
void __declspec(naked) HookSetCD_1000s_127a()
{
	//	int cd_addr;
	__asm
	{
		//	mov cd_addr, eax;
		pushad;
		pusha;
		push eax;
		call SetCdForAddr;
		popa;
		popad;
		lea ecx, [edx + 0xD0];
		jmp JumpBackAddr7;
	}
}






#pragma optimize("",on)



#pragma endregion


#pragma region BackupOffsets

std::vector<waroffsetdata> offsetslist;
int __stdcall AddNewOffset_(int address, int data, unsigned int FeatureFlag = 0)
{
	for (unsigned int i = 0; i < offsetslist.size(); i++)
	{
		if (offsetslist[i].offaddr == address)
		{
			return 0;
		}
	}

	waroffsetdata temp;
	temp.offaddr = address;
	temp.offdata = data;
	temp.newdataapp = false;
	temp.FeatureFlag = FeatureFlag;
	offsetslist.push_back(temp);

	return 1;
}

int __stdcall UpdateNewDataOffest(int address)
{
	for (auto & offdata : offsetslist)
	{
		if (offdata.offaddr == address)
		{
			offdata.newdataapp = true;
			offdata.offnewdata = *(int*)address;
			return 1;
		}
	}
	return 0;
}


int __stdcall AddNewOffset(int address, int data)
{
	AddNewOffset_(address, data);
	return 0;
}

int __stdcall RemoveOffset(int address)
{
	for (unsigned int i = 0; i < offsetslist.size(); i++)
	{
		if (offsetslist[i].offaddr == address)
		{
			offsetslist.erase(offsetslist.begin() + i);
		}
	}
	return 0;
}


vector<LPVOID> FreeExecutableMemoryList;

int __stdcall FreeExecutableMemory(void * addr)
{
	FreeExecutableMemoryList.push_back(addr);
	return 0;
}

void __stdcall RestoreFeatureOffsets(unsigned int FeatureFlag)
{
	for (unsigned int i = 0; i < offsetslist.size(); i++)
	{
		if (FeatureFlag & offsetslist[i].FeatureFlag)
		{
			waroffsetdata temp = offsetslist[i];
			DWORD oldprotect, oldprotect2;
			if (VirtualProtect((void*)temp.offaddr, 4, PAGE_EXECUTE_READWRITE, &oldprotect))
			{
				*(int*)temp.offaddr = temp.offdata;
				VirtualProtect((void*)temp.offaddr, 4, oldprotect, &oldprotect2);
				FlushInstructionCache(GetCurrentProcess(), (void*)temp.offaddr, 4);
			}
		}
	}
}

void __stdcall EnableFeatureOffsets(unsigned int FeatureFlag)
{
	for (unsigned int i = 0; i < offsetslist.size(); i++)
	{
		if (FeatureFlag & offsetslist[i].FeatureFlag && offsetslist[i].newdataapp)
		{
			waroffsetdata temp = offsetslist[i];
			DWORD oldprotect, oldprotect2;
			if (VirtualProtect((void*)temp.offaddr, 4, PAGE_EXECUTE_READWRITE, &oldprotect))
			{
				*(int*)temp.offaddr = temp.offnewdata;
				VirtualProtect((void*)temp.offaddr, 4, oldprotect, &oldprotect2);
				FlushInstructionCache(GetCurrentProcess(), (void*)temp.offaddr, 4);
			}
		}
	}
}

void __stdcall RestoreAllOffsets()
{

	for (unsigned int i = 0; i < offsetslist.size(); i++)
	{
		waroffsetdata temp = offsetslist[i];
		DWORD oldprotect, oldprotect2;
		if (VirtualProtect((void*)temp.offaddr, 4, PAGE_EXECUTE_READWRITE, &oldprotect))
		{
			*(int*)temp.offaddr = temp.offdata;
			VirtualProtect((void*)temp.offaddr, 4, oldprotect, &oldprotect2);
			FlushInstructionCache(GetCurrentProcess(), (void*)temp.offaddr, 4);
		}
	}
	if (!offsetslist.empty())
		offsetslist.clear();
}



#define Feature_AttackSpeed 0x1
#define Feature_MoveSpeed 0x2
#define Feature_ItemText 0x4
#define Feature_UnitHP_MP 0x8
#define Feature_CUSTOM_FPS_INFO 0x10
#define Feature_COOLDOWNFIX 0x20
#define Feature_MANABAR 0x40
#define Feature_HPBAR 0x80
#define Feature_FileHelper 0x100
#define Feature_Widescreen 0x200
#define Feature_MutePlayer 0x400
#define Feature_AllySkillViewer 0x800
#define Feature_ClickHelper 0x1000
#define Feature_FPSfix1 0x2000
#define Feature15 0x4000
#define Feature16 0x8000


int __stdcall DisableFeatures(unsigned int Flags)
{
	if (!InitFunctionCalled)
		return 0;
	RestoreFeatureOffsets(Flags);

	if (Flags & Feature_MANABAR)
	{
		ManaBarSwitch(FALSE);
	}

	if (Flags & Feature_FileHelper)
	{
		if (GameGetFile_org)
		{
			MH_DisableHook(GameGetFile_org);
		}

		//if ( SFileOpenFile_org )
		//{
		//	//MH_DisableHook( SFileOpenFile_org );
		//}

		//if ( SFileOpenFile_org )
		//{
		//	//MH_DisableHook( SFileOpenFile_org );
		//}
	}

	if (Flags & Feature_Widescreen)
	{
		if (SetGameAreaFOV_org)
		{
			MH_DisableHook(SetGameAreaFOV_org);
		}
	}

	if (Flags & Feature_MutePlayer)
	{
		if (pOnChatMessage_org)
		{
			MH_DisableHook(pOnChatMessage_org);
		}
	}

	if (Flags & Feature_AllySkillViewer)
	{
		if (IsDrawSkillPanel_org)
		{
			MH_DisableHook(IsDrawSkillPanel_org);
		}
		if (IsDrawSkillPanelOverlay_org)
		{
			MH_DisableHook(IsDrawSkillPanelOverlay_org);
		}
		if (IsNeedDrawUnit2org)
		{
			MH_DisableHook(IsNeedDrawUnit2org);
		}
	}

	if (Flags & Feature_ClickHelper)
	{
		if (WarcraftRealWNDProc_org)
		{
			MH_DisableHook(WarcraftRealWNDProc_org);
		}
		if (Wc3ControlClickButton_org)
		{
			MH_DisableHook(Wc3ControlClickButton_org);
		}
	}

	if (Flags & Feature_FPSfix1)
	{
		if (DrawBarForUnit_org)
		{
			MH_DisableHook(DrawBarForUnit_org);
		}
	}

	return 0;
}


int __stdcall InitHpBar(int);
int __stdcall EnableFeatures(unsigned int Flags)
{
	if (!InitFunctionCalled)
		return 0;

	EnableFeatureOffsets(Flags);



	if (Flags & Feature_HPBAR)
		InitHpBar(0);

	if (Flags & Feature_MANABAR)
	{
		ManaBarSwitch(TRUE);
	}
	if (Flags & Feature_FileHelper)
	{
		if (GameGetFile_org)
		{
			MH_EnableHook(GameGetFile_org);
		}
	}
	if (Flags & Feature_Widescreen)
	{
		if (SetGameAreaFOV_org)
		{
			MH_EnableHook(SetGameAreaFOV_org);
		}
	}
	if (Flags & Feature_MutePlayer)
	{
		if (pOnChatMessage_org)
		{
			MH_EnableHook(pOnChatMessage_org);
		}
	}

	if (Flags & Feature_AllySkillViewer)
	{
		if (IsDrawSkillPanel_org)
		{
			MH_EnableHook(IsDrawSkillPanel_org);
		}
		if (IsDrawSkillPanelOverlay_org)
		{
			MH_EnableHook(IsDrawSkillPanelOverlay_org);
		}
		if (IsNeedDrawUnit2org)
		{
			MH_EnableHook(IsNeedDrawUnit2org);
		}
	}

	if (Flags & Feature_ClickHelper)
	{
		if (WarcraftRealWNDProc_org)
		{
			MH_EnableHook(WarcraftRealWNDProc_org);
		}
		if (Wc3ControlClickButton_org)
		{
			MH_EnableHook(Wc3ControlClickButton_org);
		}
	}

	if (Flags & Feature_FPSfix1)
	{
		if (DrawBarForUnit_org)
		{
			MH_EnableHook(DrawBarForUnit_org);
		}
	}

	return 0;
}

#pragma endregion




void __stdcall ClearCustomsBars()
{

	for (int i = 0; i < 20; i++)
	{
		if (!CustomHPBarList[i].empty())
			CustomHPBarList[i].clear();
	}

	if (!MpBarUnitWhiteList.empty())
		MpBarUnitWhiteList.clear();
}

void __stdcall FreeAllVectors()
{

	if (!ModelCollisionFixList.empty())
		ModelCollisionFixList.clear();
	if (!ModelTextureFixList.empty())
		ModelTextureFixList.clear();
	if (!ModelPatchList.empty())
		ModelPatchList.clear();
	if (!ModelRemoveTagList.empty())
		ModelRemoveTagList.clear();
	if (!ModelSequenceReSpeedList.empty())
		ModelSequenceReSpeedList.clear();
	if (!ModelSequenceValueList.empty())
		ModelSequenceValueList.clear();
}

void __stdcall UnloadHWNDHandler(BOOL Force = FALSE)
{
	if (WarcraftRealWNDProc_org)
	{
		SkipAllMessages = TRUE;
		if (!Force)
			MH_DisableHook(WarcraftRealWNDProc_org);
		SkipAllMessages = FALSE;
		WarcraftRealWNDProc_org = NULL;
	}
}

void __stdcall DisableAllHooks()
{
	/*MessageBoxA( 0, "Очистка по выходе из карты", "проверка", 0 );
*/
	Packet_Uninitalize();


	// Вернуть стандартное ограничение FPS
	//_SetMaxFps( 200 );
	sprintf_s(MyFpsString, 512, "%s", "|nFPS: %.1f");
	ClipCursor(0);
	// Выгрузить перехватчики функций
	UnloadHWNDHandler();
	UninitializeHook();
	// Убрать все патчи и вернуть стандартные данные
	RestoreAllOffsets();
	// Очистить список кастом баров
	ClearCustomsBars();
	// Отключить ManaBar 
	ManaBarSwitch(FALSE);

	if (!FreeExecutableMemoryList.empty())
	{
		for (LPVOID lpAddr : FreeExecutableMemoryList)
			VirtualFree(lpAddr, 0, MEM_RELEASE);
		FreeExecutableMemoryList.clear();
	}

	UninitOpenglHook();

	Uninitd3d8Hook(TRUE);

	FreeAllIHelpers();

	FreeAllVectors();

	EnableSelectHelper = FALSE;
	AutoSelectHero = FALSE;
	BlockKeyAndMouseEmulation = FALSE;
	DoubleClickHelper = FALSE;
	LOCK_MOUSE_IN_WINDOW = FALSE;
	BlockKeyboardAndMouseWhenTeleport = FALSE;
	rawimage_skipmouseevent = TRUE;

	if (!WhiteListForTeleport.empty())
		WhiteListForTeleport.clear();
	if (!doubleclickSkillIDs.empty())
		doubleclickSkillIDs.clear();
	//	if ( !NeedDrawBarForUnit.empty( ) )
	//		NeedDrawBarForUnit.clear( );

	ShopHelperEnabled = FALSE;
	TeleportShiftPress = FALSE;
	SetWidescreenFixState(FALSE);
	MainFuncWork = FALSE;
	NeedDrawRegen = FALSE;
	GlyphButtonCreated = FALSE;
	GlobalRawImageCallbackData = NULL;
	ShowSkillPanelForObservers = FALSE;
	ShowSkillPanelOnlyForHeroes = TRUE;
	NeedReleaseUnusedMemory = FALSE;
	PlayerEnemyCache.clear();

	InitFunctionCalled = FALSE;
	SetCustomFovFix(1.0f);

	FrameDefHelperUninitialize();
	if (!ClickPortrainForIdList.empty())
		ClickPortrainForIdList.clear();

	if (!IgnoreObjInfo.empty())
		IgnoreObjInfo.clear();

	if (!InfoWhitelistedObj.empty())
		InfoWhitelistedObj.clear();

	if (!SpellBonusItemList.empty())
		SpellBonusItemList.clear();

	UninitializePacketHandler();

	LatestDownloadedString = "";

	UninitializeDreamDotaAPI();
	//	UninitializeVoiceClient( );

	magicampval = 16;

	//UnInitDreamRawImages();


	MH_DisableHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}

void PatchOffset(void * addr, void * lpbuffer, unsigned int size)
{
	DWORD OldProtect1, OldProtect2;
	VirtualProtect(addr, size, PAGE_EXECUTE_READWRITE, &OldProtect1);

	for (unsigned int i = 0; i < size; i++)
	{
		*(unsigned char*)((int)addr + i) = *(unsigned char*)((int)lpbuffer + i);
	}

	VirtualProtect(addr, size, OldProtect1, &OldProtect2);
	FlushInstructionCache(GetCurrentProcess(), addr, size);
}

int __stdcall _FlushInstructionCache(int addr, unsigned int size)
{
	return FlushInstructionCache(GetCurrentProcess(), (void*)addr, size);
}

void PatchOffsetValue4(void * addr, UINT value)
{
	DWORD OldProtect1, OldProtect2;
	VirtualProtect(addr, 4, PAGE_EXECUTE_READWRITE, &OldProtect1);
	*(UINT*)addr = value;
	VirtualProtect(addr, 4, OldProtect1, &OldProtect2);
	FlushInstructionCache(GetCurrentProcess(), addr, 4);
}
void PatchOffsetValue1(void * addr, unsigned char value)
{
	DWORD OldProtect1, OldProtect2;
	VirtualProtect(addr, 1, PAGE_EXECUTE_READWRITE, &OldProtect1);
	*(unsigned char*)addr = value;
	VirtualProtect(addr, 1, OldProtect1, &OldProtect2);
	FlushInstructionCache(GetCurrentProcess(), addr, 1);
}


PBYTE HookVTableFunction(PDWORD* dwVTable, PBYTE dwHook, INT Index)
{
	DWORD dwOld = 0;
	VirtualProtect((void*)((*dwVTable) + (Index * 4)), 4, PAGE_EXECUTE_READWRITE, &dwOld);

	PBYTE pOrig = ((PBYTE)(*dwVTable)[Index]);
	(*dwVTable)[Index] = (DWORD)dwHook;

	VirtualProtect((void*)((*dwVTable) + (Index * 4)), 4, dwOld, &dwOld);

	return pOrig;
}

PBYTE GetVTableFunction(PDWORD* dwVTable, INT Index)
{
	return ((PBYTE)(*dwVTable)[Index]);
}

DWORD GetDllCrc32()
{
	char outfilename[MAX_PATH];
	GetModuleFileNameA(GetCurrentModule, outfilename, MAX_PATH);
	DWORD dwCrc32;
	CCrc32Dynamic *pobCrc32Dynamic = new CCrc32Dynamic;
	pobCrc32Dynamic->Init();
	pobCrc32Dynamic->FileCrc32Assembly(outfilename, dwCrc32);
	pobCrc32Dynamic->Free();
	delete pobCrc32Dynamic;
	return dwCrc32;
}

DWORD __stdcall GetFileCrc32(char * file)
{
	DWORD dwCrc32;
	CCrc32Dynamic *pobCrc32Dynamic = new CCrc32Dynamic;
	pobCrc32Dynamic->Init();
	pobCrc32Dynamic->FileCrc32Assembly(file, dwCrc32);
	pobCrc32Dynamic->Free();
	delete pobCrc32Dynamic;
	return dwCrc32;
}

BOOL __stdcall DeleteFileByName(char * file)
{
	return DeleteFileA(file);
}



int __stdcall InitHpBar(int)
{
	int pHPBARHELPER = GameDll + 0x364beb;
	AddNewOffset_(pHPBARHELPER, *(int*)pHPBARHELPER, Feature_HPBAR);
	AddNewOffset_(pHPBARHELPER + 3, *(int*)(pHPBARHELPER + 3), Feature_HPBAR);
	PlantDetourJMP((unsigned char*)(pHPBARHELPER), (unsigned char*)HookHPBarColorHelper126a, 6);
	PlantDetourJMP((unsigned char*)(JumpBackAddr9), (unsigned char*)(GameDll + 0x364bf1), 5);

	return 0;
}

int __stdcall InitOverlay(int)
{

	Initd3d8Hook();
	InitOpenglHook();


	return 0;
}
BOOL InitFunctionCalled = FALSE;


std::string uint_to_hex(unsigned int i)
{
	char out[50];
	sprintf_s(out, "%08X", i);
	return string(out);
}


p_GetTypeInfo GetTypeInfo = NULL;


int ConvertHandle(int handleid)
{
	int offset = GameDll + GlobalGameStateOffset;
	if (offset && handleid >= 0x100000)
	{
		offset = *(int*)offset;
		if (offset)
		{
			offset = *(int*)(offset + 28);
			if (offset)
			{
				offset = *(int*)(offset + 412);
				if (offset)
				{
					offset = *(int*)(offset + handleid * 12 - 0xBFFFFC);
					return offset;
				}
				else
				{
					if (SetInfoObjDebugVal)
					{
						PrintText("ConvertHandle: Problem 4:");
					}
				}
			}
			else
			{
				if (SetInfoObjDebugVal)
				{
					PrintText("ConvertHandle: Problem 3:");
				}
			}
		}
		else
		{
			if (SetInfoObjDebugVal)
			{
				PrintText("ConvertHandle: Problem 2:");
			}
		}
	}
	else
	{
		if (SetInfoObjDebugVal)
		{
			PrintText("ConvertHandle: Problem 1:");
		}
	}
	return 0;
}

BOOL InitializedDream = FALSE;

unsigned int __stdcall InitDotaHelper(int)
{
	UnloadAllOldHelpers(0);

	MH_Initialize();
	InitThreadCpuUsage();

	//RemoveMapSizeLimit( );


	// start hotfix
	if (!StormDll)
	{
		StormDllModule = GetModuleHandleA("Storm.dll");
		StormDll = (int)StormDllModule;
	}

	if (!GameDllModule)
		GameDllModule = (HMODULE)GameDll;
	// end hotfix

	while (mutedplayers.size())
	{
		char * fMemAddr = mutedplayers.back();
		if (fMemAddr)
			free(fMemAddr);
		mutedplayers.pop_back();
	}

	memset(hpbarcolorsHero, 0, sizeof(hpbarcolorsHero));
	memset(hpbarcolorsUnit, 0, sizeof(hpbarcolorsUnit));
	memset(hpbarcolorsTower, 0, sizeof(hpbarcolorsTower));
	memset(hpbarscaleHeroX, 0, sizeof(hpbarscaleHeroX));
	memset(hpbarscaleUnitX, 0, sizeof(hpbarscaleUnitX));
	memset(hpbarscaleTowerX, 0, sizeof(hpbarscaleTowerX));
	memset(hpbarscaleHeroY, 0, sizeof(hpbarscaleHeroY));
	memset(hpbarscaleUnitY, 0, sizeof(hpbarscaleUnitY));
	memset(hpbarscaleTowerY, 0, sizeof(hpbarscaleTowerY));

	Warcraft3Window = 0;
	EnableSelectHelper = FALSE;
	AutoSelectHero = FALSE;
	BlockKeyAndMouseEmulation = FALSE;
	DoubleClickHelper = FALSE;
	LOCK_MOUSE_IN_WINDOW = FALSE;
	BlockKeyboardAndMouseWhenTeleport = FALSE;
	rawimage_skipmouseevent = TRUE;
	if (!WhiteListForTeleport.empty())
		WhiteListForTeleport.clear();
	if (!doubleclickSkillIDs.empty())
		doubleclickSkillIDs.clear();
	//	if ( !NeedDrawBarForUnit.empty( ) )
	//		NeedDrawBarForUnit.clear( );

	ShopHelperEnabled = FALSE;
	TeleportShiftPress = FALSE;
	SetWidescreenFixState(FALSE);
	MainFuncWork = TRUE;
	NeedDrawRegen = FALSE;
	GlyphButtonCreated = FALSE;
	ShowSkillPanelForObservers = FALSE;
	ShowSkillPanelOnlyForHeroes = TRUE;
	FPSfix1Enabled = FALSE;
	GlobalRawImageCallbackData = NULL;
	SetCustomFovFix(1.0f);
	ScanId = 0;
	NeedReleaseUnusedMemory = FALSE;
	PlayerEnemyCache.clear();

	sprintf_s(MyFpsString, 512, "%s", "|nFPS: %.1f / 64.0 ");

	if (!GlobalTextBuffer)
		GlobalTextBuffer = new char[0x2000];
	memset(GlobalTextBuffer, 0, 0x2000);


	_W3XTlsIndex = 0xAB7BF4 + GameDll;

	UninitializeHook();
	FreeAllIHelpers();

	pOnChatMessage_offset = 0x2FB480;
	IsNeedDrawUnit2offset = 0x28E1D0;
	IsNeedDrawUnit2offsetRetAddress = 0x2F9B60;
	IsPlayerEnemyOffset = 0x3C9580;
	IsDrawSkillPanelOffset = 0x34F280;
	DrawSkillPanelOffset = 0x2774C0;
	IsDrawSkillPanelOverlayOffset = 0x34F2C0;
	DrawSkillPanelOverlayOffset = 0x277570;
	IsNeedDrawUnitOriginOffset = 0x285DC0;
	GlobalGameStateOffset = 0xAB65F4;
	pAttackSpeedLimit = 0xAB0074;
	pW3XGlobalClass = GameDll + 0xAB4F80;
	pPrintText2 = GameDll + 0x2F69A0;
	pGameClass1 = GameDll + 0xAB7788;
	UnitVtable = GameDll + 0x931934;
	ItemVtable = GameDll + 0x9320B4;
	GetHeroInt = (pGetHeroInt)(GameDll + 0x277850);
	Storm_503 = (pStorm_503)(*(int*)(GameDll + 0x86D584));
	_GlobalGlueObj = GameDll + 0xACE66C;
	_GameUI = GameDll + 0x93631C;
	InGame = (int *)(_GlobalGlueObj);
	_EventVtable = GameDll + 0xA9ACB0;
	_ChatSendEvent = GameDll + 0x2FC700;
	GetItemInSlotAddr = GameDll + 0x3C7730 + 0xA;
	GetItemTypeId = (pGetItemTypeId)(GameDll + 0x3C4C60);
	GetPlayerColor2 = (pGetPlayerColor)(GameDll + 0x3C1240);
	_Player = (pPlayer)(GameDll + 0x3BBB30);
	GetPlayerName = (p_GetPlayerName)(GameDll + 0x2F8F90);
	_BarVTable = GameDll + 0x93E604;
	IsWindowActive = (BOOL *)(GameDll + 0xA9E7A4);
	ChatFound = GameDll + 0xAD15F0;
	TriggerExecute = (_TriggerExecute)(GameDll + 0x3C3F40);
	ExecuteFunc = (pExecuteFunc)(GameDll + 0x3D3F30);
	StormErrorHandlerOffset = StormDll + 0x28F0;
	JassNativeLookupOffset = GameDll + 0x45D070;
	JassFuncLookupOffset = GameDll + 0x45AE80;
	ProcessNetEventsOffset = GameDll + 0x551D80;
	BlizzardDebug1Offset = GameDll + 0x7AEC10;
	BlizzardDebug2Offset = GameDll + 0x537e80;
	BlizzardDebug3Offset = GameDll + 0x20BC60;
	BlizzardDebug4Offset = GameDll + 0x41F5E0;
	BlizzardDebug5Offset = GameDll + 0x39E970;
	BlizzardDebug6Offset = GameDll + 0x579C10;


	IssueWithoutTargetOrderOffset = 0x339C60;
	IssueTargetOrPointOrder2Offset = 0x339CC0;
	sub_6F339D50Offset = 0x339D50;
	IssueTargetOrPointOrderOffset = 0x339DD0;
	sub_6F339E60Offset = 0x339E60;
	sub_6F339F00Offset = 0x339F00;
	sub_6F339F80Offset = 0x339F80;
	sub_6F33A010Offset = 0x33A010;

	GameGetFileOffset = 0x4C1550;


	OriginFilter = (LPTOP_LEVEL_EXCEPTION_FILTER)(StormDll + 0x16880);


	GameFrameAtMouseStructOffset = GameDll + 0xA9A444;


	int pDrawAttackSpeed = GameDll + 0x339150;
	AddNewOffset_(pDrawAttackSpeed, *(int*)pDrawAttackSpeed, Feature_AttackSpeed);
	AddNewOffset_(pDrawAttackSpeed + 3, *(int*)(pDrawAttackSpeed + 3), Feature_AttackSpeed);
	PlantDetourJMP((unsigned char*)(pDrawAttackSpeed), (unsigned char*)PrintAttackSpeedAndOtherInfoHook126a, 5);
	UpdateNewDataOffest(pDrawAttackSpeed);
	UpdateNewDataOffest(pDrawAttackSpeed + 3);

	int pDrawMoveSpeed = GameDll + 0x338FB0;
	AddNewOffset_(pDrawMoveSpeed, *(int*)pDrawMoveSpeed, Feature_MoveSpeed);
	AddNewOffset_(pDrawMoveSpeed + 3, *(int*)(pDrawMoveSpeed + 3), Feature_MoveSpeed);
	PlantDetourJMP((unsigned char*)(pDrawMoveSpeed), (unsigned char*)PrintMoveSpeedHook126a, 5);
	UpdateNewDataOffest(pDrawMoveSpeed);
	UpdateNewDataOffest(pDrawMoveSpeed + 3);


	int pDrawItemText1 = GameDll + 0x369e72;
	AddNewOffset_(pDrawItemText1, *(int*)pDrawItemText1, Feature_ItemText);
	AddNewOffset_(pDrawItemText1 + 3, *(int*)(pDrawItemText1 + 3), Feature_ItemText);
	PlantDetourJMP((unsigned char*)(pDrawItemText1), (unsigned char*)HookPrint1_126a, 5);
	PlantDetourJMP((unsigned char*)(JumpBackAddr2), (unsigned char*)(GameDll + 0x369e83), 5);
	UpdateNewDataOffest(pDrawItemText1);
	UpdateNewDataOffest(pDrawItemText1 + 3);


	int pDrawItemText2 = GameDll + 0x369ee6;
	AddNewOffset_(pDrawItemText2, *(int*)pDrawItemText2, Feature_ItemText);
	AddNewOffset_(pDrawItemText2 + 3, *(int*)(pDrawItemText2 + 3), Feature_ItemText);
	PlantDetourJMP((unsigned char*)(pDrawItemText2), (unsigned char*)HookPrint2_126a, 5);
	PlantDetourJMP((unsigned char*)(JumpBackAddr3), (unsigned char*)(GameDll + 0x369ef7), 5);
	UpdateNewDataOffest(pDrawItemText2);
	UpdateNewDataOffest(pDrawItemText2 + 3);


	int MovEaxByte = 0xB8;
	int MovEcxByte = 0xB9;
	int MovEdxByte = 0xBA;
	int NopByte = 0x90;
	int pDrawItemTextFix1 = GameDll + 0x369F62;

	AddNewOffset_(pDrawItemTextFix1, *(int*)pDrawItemTextFix1, Feature_ItemText);
	AddNewOffset_(pDrawItemTextFix1 + 3, *(int*)(pDrawItemTextFix1 + 3), Feature_ItemText);
	PatchOffset((void*)pDrawItemTextFix1, &MovEaxByte, 1);
	PatchOffsetValue4((void*)(pDrawItemTextFix1 + 1), (UINT)(void*)(GlobalTextBuffer));
	PatchOffset((void*)(pDrawItemTextFix1 + 5), &NopByte, 1);
	PatchOffset((void*)(pDrawItemTextFix1 + 6), &NopByte, 1);
	UpdateNewDataOffest(pDrawItemTextFix1);
	UpdateNewDataOffest(pDrawItemTextFix1 + 3);


	int pDrawItemTextFix2 = GameDll + 0x369F81;

	AddNewOffset_(pDrawItemTextFix2, *(int*)pDrawItemTextFix2, Feature_ItemText);
	AddNewOffset_(pDrawItemTextFix2 + 3, *(int*)(pDrawItemTextFix2 + 3), Feature_ItemText);
	PatchOffset((void*)pDrawItemTextFix2, &MovEdxByte, 1);
	PatchOffsetValue4((void*)(pDrawItemTextFix2 + 1), (UINT)(void*)(GlobalTextBuffer));
	PatchOffset((void*)(pDrawItemTextFix2 + 5), &NopByte, 1);
	PatchOffset((void*)(pDrawItemTextFix2 + 6), &NopByte, 1);
	UpdateNewDataOffest(pDrawItemTextFix2);
	UpdateNewDataOffest(pDrawItemTextFix2 + 3);




	int pDrawItemTextFix3 = GameDll + 0x369FA6;

	AddNewOffset_(pDrawItemTextFix3, *(int*)pDrawItemTextFix3, Feature_ItemText);
	AddNewOffset_(pDrawItemTextFix3 + 3, *(int*)(pDrawItemTextFix3 + 3), Feature_ItemText);
	PatchOffset((void*)pDrawItemTextFix3, &MovEdxByte, 1);
	PatchOffsetValue4((void*)(pDrawItemTextFix3 + 1), (UINT)(void*)(GlobalTextBuffer));
	PatchOffset((void*)(pDrawItemTextFix3 + 5), &NopByte, 1);
	PatchOffset((void*)(pDrawItemTextFix3 + 6), &NopByte, 1);
	UpdateNewDataOffest(pDrawItemTextFix3);
	UpdateNewDataOffest(pDrawItemTextFix3 + 3);




	int pDrawItemTextFix4 = GameDll + 0x369FC3;

	AddNewOffset_(pDrawItemTextFix4, *(int*)pDrawItemTextFix4, Feature_ItemText);
	AddNewOffset_(pDrawItemTextFix4 + 3, *(int*)(pDrawItemTextFix4 + 3), Feature_ItemText);
	PatchOffset((void*)pDrawItemTextFix4, &MovEcxByte, 1);
	PatchOffsetValue4((void*)(pDrawItemTextFix4 + 1), (UINT)(void*)(GlobalTextBuffer));
	PatchOffset((void*)(pDrawItemTextFix4 + 5), &NopByte, 1);
	PatchOffset((void*)(pDrawItemTextFix4 + 6), &NopByte, 1);
	UpdateNewDataOffest(pDrawItemTextFix4);
	UpdateNewDataOffest(pDrawItemTextFix4 + 3);



	int pDrawItemTextFix5 = GameDll + 0x369FEB;

	AddNewOffset_(pDrawItemTextFix5, *(int*)pDrawItemTextFix5, Feature_ItemText);
	AddNewOffset_(pDrawItemTextFix5 + 3, *(int*)(pDrawItemTextFix5 + 3), Feature_ItemText);
	PatchOffset((void*)pDrawItemTextFix5, &MovEdxByte, 1);
	PatchOffsetValue4((void*)(pDrawItemTextFix5 + 1), (UINT)(void*)(GlobalTextBuffer));
	PatchOffset((void*)(pDrawItemTextFix5 + 5), &NopByte, 1);
	PatchOffset((void*)(pDrawItemTextFix5 + 6), &NopByte, 1);
	UpdateNewDataOffest(pDrawItemTextFix5);
	UpdateNewDataOffest(pDrawItemTextFix5 + 3);



	int pDrawItemTextFix6 = GameDll + 0x36A007;

	AddNewOffset_(pDrawItemTextFix6, *(int*)pDrawItemTextFix6, Feature_ItemText);
	AddNewOffset_(pDrawItemTextFix6 + 3, *(int*)(pDrawItemTextFix6 + 3), Feature_ItemText);
	PatchOffset((void*)pDrawItemTextFix6, &MovEcxByte, 1);
	PatchOffsetValue4((void*)(pDrawItemTextFix6 + 1), (UINT)(void*)(GlobalTextBuffer));
	PatchOffset((void*)(pDrawItemTextFix6 + 5), &NopByte, 1);
	PatchOffset((void*)(pDrawItemTextFix6 + 6), &NopByte, 1);
	UpdateNewDataOffest(pDrawItemTextFix6);
	UpdateNewDataOffest(pDrawItemTextFix6 + 3);








	int pSaveLatestItem = GameDll + 0x369b3d;
	AddNewOffset_(pSaveLatestItem, *(int*)pSaveLatestItem, Feature_ItemText);
	AddNewOffset_(pSaveLatestItem + 3, *(int*)(pSaveLatestItem + 3), Feature_ItemText);
	PlantDetourJMP((unsigned char*)(pSaveLatestItem), (unsigned char*)HookItemAddr126a, 5);
	PlantDetourJMP((unsigned char*)(JumpBackAddr1), (unsigned char*)(GameDll + 0x369b45), 5);
	UpdateNewDataOffest(pSaveLatestItem);
	UpdateNewDataOffest(pSaveLatestItem + 3);


	int pSaveLatestUnit = GameDll + 0x3580ad;
	AddNewOffset_(pSaveLatestUnit, *(int*)pSaveLatestUnit, Feature_UnitHP_MP);
	AddNewOffset_(pSaveLatestUnit + 3, *(int*)(pSaveLatestUnit + 3), Feature_UnitHP_MP);
	PlantDetourJMP((unsigned char*)(pSaveLatestUnit), (unsigned char*)HookUnitAddr126a, 5);
	PlantDetourJMP((unsigned char*)(JumpBackAddr4), (unsigned char*)(GameDll + 0x3580b2), 5);
	UpdateNewDataOffest(pSaveLatestUnit);
	UpdateNewDataOffest(pSaveLatestUnit + 3);


	int pDrawUnitText1 = GameDll + 0x358198;
	AddNewOffset_(pDrawUnitText1, *(int*)pDrawUnitText1, Feature_UnitHP_MP);
	AddNewOffset_(pDrawUnitText1 + 3, *(int*)(pDrawUnitText1 + 3), Feature_UnitHP_MP);
	PlantDetourJMP((unsigned char*)(pDrawUnitText1), (unsigned char*)HookPrint3_126a, 5);
	PlantDetourJMP((unsigned char*)(JumpBackAddr5), (unsigned char*)(GameDll + 0x3581a6), 5);
	UpdateNewDataOffest(pDrawUnitText1);
	UpdateNewDataOffest(pDrawUnitText1 + 3);


	int pDrawUnitText2 = GameDll + 0x3583c2;
	AddNewOffset_(pDrawUnitText2, *(int*)pDrawUnitText2, Feature_UnitHP_MP);
	AddNewOffset_(pDrawUnitText2 + 3, *(int*)(pDrawUnitText2 + 3), Feature_UnitHP_MP);
	PlantDetourJMP((unsigned char*)(pDrawUnitText2), (unsigned char*)HookPrint4_126a, 5);
	PlantDetourJMP((unsigned char*)(JumpBackAddr6), (unsigned char*)(GameDll + 0x3583d0), 5);
	UpdateNewDataOffest(pDrawUnitText2);
	UpdateNewDataOffest(pDrawUnitText2 + 3);


	int pAlwaysRefresh1 = GameDll + 0x35818c;
	int pAlwaysRefresh2 = GameDll + 0x3583ba;
	unsigned char JMPBYTE = 0xEB;

	AddNewOffset_(pAlwaysRefresh1, *(int*)pAlwaysRefresh1, Feature_UnitHP_MP);
	AddNewOffset_(pAlwaysRefresh2, *(int*)pAlwaysRefresh2, Feature_UnitHP_MP);
	PatchOffset((void*)pAlwaysRefresh1, &JMPBYTE, 1);
	PatchOffset((void*)pAlwaysRefresh2, &JMPBYTE, 1);
	UpdateNewDataOffest(pAlwaysRefresh1);
	UpdateNewDataOffest(pAlwaysRefresh2);


	int pSetCooldown = GameDll + 0x37ed3;
	AddNewOffset_(pSetCooldown, *(int*)pSetCooldown, Feature_COOLDOWNFIX);
	AddNewOffset_(pSetCooldown + 3, *(int*)(pSetCooldown + 3), Feature_COOLDOWNFIX);
	PlantDetourJMP((unsigned char*)(pSetCooldown), (unsigned char*)HookSetCD_1000s_126a, 5);
	PlantDetourJMP((unsigned char*)(JumpBackAddr7), (unsigned char*)(GameDll + 0x37edf), 5);
	UpdateNewDataOffest(pSetCooldown);
	UpdateNewDataOffest(pSetCooldown + 3);


	InitHpBar(0);

	SimpleButtonClickEvent_org = (c_SimpleButtonClickEvent)(GameDll + 0x603440);
	CommandButtonVtable = GameDll + 0x93EBC4;


	MapNameOffset1 = GameDll + 0xAAE788;
	MapNameOffset2 = 8;

	GetWindowXoffset = (float *)(GameDll + 0xADE91C);
	GetWindowYoffset = (float *)(GameDll + 0xADE918);

	SetGameAreaFOVoffset = 0x7B66F0;

	pWar3GlobalData1 = GameDll + 0xACBD40;
	pWar3GlobalData1 = *(int*)pWar3GlobalData1;
	Warcraft3Window = *(HWND*)(GameDll + 0xAD147C);
	Warcraft3WindowProcOffset = GameDll + 0x6C6AA0;

	Game_Wc3MessageBox = (pGame_Wc3MessageBox)(GameDll + 0x55CEB0);

	pPreferencesOffset = GameDll + 0xAAE314;


	int PatchFPSDraw = GameDll + 0x33b8de;

	int PatchMemMB1 = GameDll + 0x33ba0a;
	int PatchMemMB2 = GameDll + 0x33ba0e;



	AddNewOffset_(PatchMemMB1, *(int*)PatchMemMB1, Feature_CUSTOM_FPS_INFO);
	AddNewOffset_(PatchMemMB2, *(int*)PatchMemMB2, Feature_CUSTOM_FPS_INFO);
	AddNewOffset_(PatchFPSDraw, *(int*)PatchFPSDraw, Feature_CUSTOM_FPS_INFO);


	AddNewOffset_((GameDll + 0x62D7FB), *(int*)(GameDll + 0x62D7FB), 0);

	PatchOffsetValue4((void*)PatchMemMB1, 0x8324148B);
	PatchOffsetValue4((void*)PatchMemMB2, 0x90521CC4);
	PatchOffsetValue4((void*)PatchFPSDraw, (UINT)(void*)(MyFpsString));

	UpdateNewDataOffest(PatchMemMB1);
	UpdateNewDataOffest(PatchMemMB2);
	UpdateNewDataOffest(PatchFPSDraw);


	GameChatSetState = (pGameChatSetState)(GameDll + 0x341460);

	_SetMaxFps = (p_SetMaxFps)(GameDll + 0x383640);
	_SetMaxFps(200);
	LoadFrameDefList = (pLoadFrameDefList)(GameDll + 0x5C8510);
	//ManaBarSwitch( TRUE );

	DefaultCStatus = GameDll + 0xA8C804;
	LoadFramesVar1 = GameDll + 0xACD214;
	LoadFramesVar2 = GameDll + 0xACD264;
	LoadNewFrameDef_org = (pLoadNewFrameDef)(GameDll + 0x5D8DE0);
	CreateNewFrame = (pCreateNewFrame)(GameDll + 0x5C9560);
	ShowThisFrame = (pShowThisFrame)(GameDll + 0x368B90);
	DestructThisFrame = (pDestructThisFrame)(GameDll + 0x606910);
	SetFramePos = (pSetFramePos)(GameDll + 0x6061B0);

	ShowFrameAlternative = (pShowFrameAlternative)(GameDll + 0x606770);
	GetFrameItemAddress = (pGetFrameItemAddress)(GameDll + 0x5FA970);
	str2jstr = (pConvertStrToJassStr)(GameDll + 0x4C5CF0);//old 0x11300

	UpdateFrameFlags = (pUpdateFrameFlags)(GameDll + 0x602370);
	pCurrentFrameFocusedAddr = GameDll + 0xACE67C;


	GetPlayerAlliance = (pGetPlayerAlliance)(GameDll + 0x3C9D70);
	Wc3ControlClickButton_offset = GameDll + 0x601F20;


	DrawUnitBarOffset = GameDll + 0x2C74B0;

	IsPlayerObs = (pIsPlayerObs)(GameDll + 0x3C9600);

	DrawInterface_org = (DrawInterface_p)(GameDll + 0x341740);
	GetTownUnitCount_org = (GetTownUnitCount_p)(GameDll + 0x2DD0C0);


	Wc3DrawStage_org = (Wc3DrawStage)(GameDll + 0x395620);

	GetTypeInfo = (p_GetTypeInfo)(GameDll + 0x32C880); // 127a 0x327020

	GetUnitFloatState = (_GetUnitFloatStat)(GameDll + 0x27AE90);  // 6F27B9B0 


	SimpleButtonPreClickEvent_org = (pSimpleButtonPreClickEvent)(GameDll + 0x6033A0);//1.27a  0x0BB560
	MH_CreateHook(SimpleButtonPreClickEvent_org, &SimpleButtonPreClickEvent_my, reinterpret_cast<void**>(&SimpleButtonPreClickEvent_ptr));
	MH_EnableHook(SimpleButtonPreClickEvent_org);

	pSetChatTargetUsers_org = (pSetChatTargetUsers)(GameDll + 0x3412F0);
	MH_CreateHook(pSetChatTargetUsers_org, &SetChatTargetUsers_my, reinterpret_cast<void**>(&pSetChatTargetUsers_ptr));
	MH_EnableHook(pSetChatTargetUsers_org);

	ChatEditBoxVtable = 0x93A7A4;

	SelectUnitReal = (void(__thiscall *)(int pPlayerSelectData, int pUnit, int id, int unk1, int unk2, int unk3))(GameDll + 0x424B80);
	UpdatePlayerSelection = (void(__thiscall *)(int pPlayerSelectData, int unk))(GameDll + 0x425490);
	ClearSelection = (int(__cdecl *)(void))(GameDll + 0x3BBAA0);

	InitHook();

	for (int i = 0; i < 16; i++)
	{
		playercache[i] = _Player(i);
	}


	for (int i = 0; i < 16; i++)
	{
		player_real_cache[i] = _GetPlayerByNumber(i);
	}


	for (int i = 0; i < 16; i++)
	{
		player_observers[i] = _IsPlayerObserver(i);
	}


	player_local_id = _GetLocalPlayerId();


	InitFunctionCalled = TRUE;



	DWORD crc32 = GetDllCrc32();


	FrameDefHelperInitialize();

	InitializePacketHandler();

	InitializeDreamDotaAPI(TRUE, GameDllModule, Warcraft3Window);

	MainDispatcher()->listen(EVENT_GAME_START, SetGameFound);
	MainDispatcher()->listen(EVENT_GAME_END, SetGameEnd);

	//InitDreamRawImages( );

	//		InitVoiceClientThread( );
	return crc32;

}

int __stdcall UpdatePlayerCache(int)
{
	for (int i = 0; i < 16; i++)
	{
		playercache[i] = _Player(i);
	}


	for (int i = 0; i < 16; i++)
	{
		player_real_cache[i] = _GetPlayerByNumber(i);
	}


	for (int i = 0; i < 16; i++)
	{
		player_observers[i] = _IsPlayerObserver(i);
	}



	player_local_id = _GetLocalPlayerId();



	PlayerEnemyCache.clear();

	return 0;
}


const char * GameDllName = "Game.dll";
const char * StormDllName = "Storm.dll";

int __stdcall SetCustomGameDllandStormDLL(const char * _GameDllName, const char * _StormDllName)
{
	GameDllModule = GetModuleHandleA(_GameDllName);
	if (!GameDllModule)
		return FALSE;
	GameDll = (int)GameDllModule;

	StormDllModule = GetModuleHandleA(_StormDllName);



	if (!StormDllModule)
		return FALSE;
	StormDll = (int)StormDllModule;
	Storm::Init(StormDllModule);
	return 0;
}

int __stdcall SetGameDllAddr(HMODULE GameDllmdl)
{
	GameDllModule = GameDllmdl;
	GameDll = (int)GameDllModule;
	return 0;
}

BOOL TerminateStarted = FALSE;


BOOL TestModeActivated = FALSE;

#pragma region Main
BOOL __stdcall DllMain(HINSTANCE Module, unsigned int reason, LPVOID)
{
	GetCurrentModule = Module;
	if (reason == DLL_PROCESS_ATTACH)
	{
		/*std::streambuf *coutbuf = std::cout.rdbuf( );
		std::ofstream out( "debug.log" );
		std::cout.rdbuf( out.rdbuf( ) );
*/

/*	FILE * f;

	fopen_s( &f, "DotaAllstarsDataLog.txt", "w" );
	if ( f )
	{
		fclose( f );
		f = NULL;
		freopen_s( &f, "DotaAllstarsDataLog.txt", "w", stdout );
		f = NULL;
	}

	fopen_s( &f, "DotaAllstarsErrorLog.txt", "w" );
	if ( f )
	{
		fclose( f );
		f = NULL;
		freopen_s( &f, "DotaAllstarsErrorLog.txt", "w", stderr );
	}*/

	//cerr << "Dota Helper Error Log out:" << endl;
	//cout << "Dota Helper Debug Log out" << endl;

		DisableThreadLibraryCalls(Module);

		GameDllModule = GetModuleHandleA(GameDllName);
		GameDll = (int)GameDllModule;
		StormDllModule = GetModuleHandleA(StormDllName);
		StormDll = (int)StormDllModule;
		Storm::Init(StormDllModule);

		Warcraft3_Process = GetCurrentProcess();
		// NEXT LINES ONLY FOR TEST !!!
		 //TestModeActivated = TRUE;
		 //InitDotaHelper( 0x27a );
		 ////DisableFeatures( 0xEFFF );
		 //MainFuncWork = TRUE;
		// EnableErrorHandler( 0);
	}
	else if (reason == DLL_PROCESS_DETACH)
	{
		TerminateStarted = TRUE;

		if (!GetProcAddress(GameDllModule, "GameMain") || !GetProcAddress(StormDllModule, (LPCSTR)266))
		{
			// Unable to cleanup, need just terminate process :(
			ExitProcess(0);
		}

		DisableAllHooks();
		MH_DisableHook(MH_ALL_HOOKS);
		MH_Uninitialize();
	}
	return TRUE;
}
#pragma endregion










































#pragma region Developer
//Karaulov 
#pragma endregion