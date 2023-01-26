#include "stdafx.h"
#include "Foundation.h"
#include <process.h>
#include "SystemTools.h"
#include "DreamUI.h"
#include "War3Window.h"

static HFONT LoadingFont = NULL;
void* hMdl;
//bool Initialized = false;

//unsigned long WINAPI INITTHREAD(LPVOID)
//{
//	Init();
//	Initialized = true;
//	return 0;
//}

void InitializeDreamUI(void* _GameDll, HWND war3hwnd)
{
	//if (!Initialized)
	//{
	SetWar3Window(war3hwnd);
	if (!SystemTools::Init()) {
		return;
	}
	Init(_GameDll);
	//Initialized = true;
	//CreateThread(0, 0, INITTHREAD, 0, 0, 0);
//}
}

void UnitializeDreamUI()
{
	Cleanup();
	if (LoadingFont) {
		DeleteObject(LoadingFont);
		LoadingFont = NULL;
	}
	SystemTools::Cleanup();
	//Initialized = false;
}
