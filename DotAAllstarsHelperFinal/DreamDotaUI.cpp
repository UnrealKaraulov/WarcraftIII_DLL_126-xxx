#include "Main.h"
#include <DreamUI.h>
#pragma comment(lib, "DreamUI.lib")

// Thanks @thewisp and @fluxxu for DreamDota https://github.com/DreamHacks/dreamdota

int DreamDotaInitialized = false;

void InitializeDreamDotaAPI(int config, void* _GameDll, HWND war3hwnd)
{
	if (config)
	{
		RegisterConfigWindow();

		if (!DreamDotaInitialized)
		{
			InitializeDreamUI(_GameDll, war3hwnd);
			DreamDotaInitialized = true;
		}
	}
	else
	{
		if (!DreamDotaInitialized)
		{
			InitializeDreamUI(_GameDll, war3hwnd);
			DreamDotaInitialized = true;
		}
	}
}

void UninitializeDreamDotaAPI()
{
	if (DreamDotaInitialized)
	{
		DreamDotaInitialized = false;
		UnitializeDreamUI();
	}
}


