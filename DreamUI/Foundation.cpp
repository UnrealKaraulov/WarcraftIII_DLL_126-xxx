#include "stdafx.h"

#include "Foundation.h"

#include <process.h>
#include <new>
#include <time.h>
#include <stdlib.h>
#include "Tools.h"
#include "Offsets.h"
#include "Hook.h"
#include "Storm.h"
#include "Input.h"
#include "War3Window.h"
#include "RefreshManager.h"
#include "Version.h"
#include "SystemTools.h"
#include "Game.h"
#include "ObjectHookManager.h"
void* ModuleGame;		//Game.dll 模块
void* ModuleStorm;		//Storm.dll 模块
void* ModuleDream;		//自身模块
static unsigned long VersionGame;	//Game.dll版本
static bool Inited = false;			//标记是否需要清理

char RootPath[MAX_PATH];

int GetGameVersion() { return (int)VersionGame; }

__declspec(noinline) void Init(void* _GameDll) {

	ModuleStorm = GetModuleHandleA("Storm.dll");
	ModuleGame = _GameDll;

	if (ModuleGame != NULL) {
		VersionGame = VersionGet("Game.dll", 4);

		Offset_Init(VersionGame, (unsigned long)ModuleGame);

		srand((unsigned int)(time(NULL)));


		DreamStorm::Init(ModuleStorm);

		ObjectHookManager_Init();

		Tools_Init();

		War3Window_Init();

		Input_Init();

		Inited = true;
	}
	else {

	}

}



__declspec(noinline) void Cleanup() {

	if (Inited) {
		DreamStorm::Cleanup();
		ObjectHookManager_Cleanup();
		War3Window_Cleanup();
	}

}

extern "C" unsigned int GetRevision() {
	return VERSION.revision;
}