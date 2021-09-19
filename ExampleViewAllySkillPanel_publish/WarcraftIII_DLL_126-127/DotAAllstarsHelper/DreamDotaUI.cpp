#include "Main.h"
#include <DreamUI.h>
#pragma comment(lib, "DreamUI.lib")

// Thanks @thewisp and @fluxxu for DreamDota https://github.com/DreamHacks/dreamdota

BOOL DreamDotaInitialized = FALSE;

void InitializeDreamDotaAPI( BOOL config, HMODULE _GameDll, HWND war3hwnd )
{
	if ( config )
	{
		RegisterConfigWindow( );

		if ( !DreamDotaInitialized )
		{
			InitializeDreamUI( _GameDll, war3hwnd );
			DreamDotaInitialized = TRUE;
		}
	}
	else
	{
		if ( !DreamDotaInitialized )
		{
			InitializeDreamUI( _GameDll, war3hwnd );
			DreamDotaInitialized = TRUE;
		}
	}
}

void UninitializeDreamDotaAPI( )
{
	if ( DreamDotaInitialized )
	{
		DreamDotaInitialized = FALSE;
		UnitializeDreamUI( );
	}
}


