#include "Main.h"
// only for test ( http://i.imgur.com/40qQr8X.jpg )



int DefaultCStatus = 0;
int LoadFramesVar1 = 0;
int LoadFramesVar2 = 0;

pLoadNewFrameDef LoadNewFrameDef_org;
pCreateNewFrame CreateNewFrame;
pShowFrameAlternative ShowFrameAlternative;
pSetFramePos SetFramePos;
pShowThisFrame ShowThisFrame;
pDestructThisFrame DestructThisFrame;//Without clean memory
pGetFrameItemAddress GetFrameItemAddress;

BOOL usedcustomframes = FALSE;

char ConfigPath[MAX_PATH];


void ReadAllConfig()
{

}


void WriteAllConfig()
{

}


void LoadNewFrameDef(const char * filename)
{
	usedcustomframes = TRUE;
	LoadNewFrameDef_org(filename, LoadFramesVar1, LoadFramesVar2, DefaultCStatus);
}


int CreateNewFrameAndShow(const char * FrameName, BOOL Show = FALSE)
{
	int pGlobalGameClass = GetGlobalClassAddr();
	int FrameAddr = CreateNewFrame(FrameName, pGlobalGameClass, 0, 0, 0);
	if (Show && FrameAddr)
	{
		ShowThisFrame(FrameAddr);
	}
	return FrameAddr;
}

void DestroyFrame(int FrameAddr, BOOL unk = TRUE)
{
	DestructThisFrame(FrameAddr, unk);
}

void ShowFrameWithPos(int FrameAddr, float left, float bottom, BOOL unk = TRUE)
{
	SetFramePos(FrameAddr + 180, 6, left, bottom, TRUE);
	ShowThisFrame(FrameAddr);
}

void ShowFrameItemWithPos(int FrameAddr, float left, float bottom, BOOL unk = TRUE)
{
	SetFramePos(FrameAddr, 6, left, bottom, TRUE);
	ShowThisFrame(FrameAddr);
}


void ShowFrameWithPosAligned(int FrameAddr, float left, float bottom, int align, BOOL unk = TRUE)
{
	SetFramePos(FrameAddr + 180, align, left, bottom, TRUE);
	ShowThisFrame(FrameAddr);
}

void ShowFrameWithPosAlternativeAligned(int FrameAddr, float left, float bottom, int alignFirst, int alignTwo, BOOL unk = TRUE)
{
	int pGlobalGameClass = GetGlobalClassAddr();
	ShowFrameAlternative(FrameAddr + 180, alignFirst, pGlobalGameClass + 180, alignTwo, left, bottom, 1);
	ShowThisFrame(FrameAddr);
}


void ShowFrameWithPosAlternative(int FrameAddr, float left, float bottom)
{
	int pGlobalGameClass = GetGlobalClassAddr();
	ShowFrameAlternative(FrameAddr + 180, 6, pGlobalGameClass + 180, 6, left, bottom, 1);
	ShowThisFrame(FrameAddr);
}


struct FrameDefStatus
{
	int FDefVtable;
	int zeroint;
	int this_add_8;
	int this_add_8_negative;
	int this_one;
};
FrameDefStatus fStatus;

BOOL FrameDefFileUpdated = FALSE;

void LoadCustomFramesIfNeed()
{
	if (!FrameDefFileUpdated)
	{
		FrameDefFileUpdated = TRUE;
		LoadNewFrameDef("CustomFrames.txt");
	}
}


BOOL ConfigWindowCreated = FALSE;
int ConfigFrameAddr = 0;

int __stdcall ShowConfigWindow(const char * filename)
{
	LoadCustomFramesIfNeed();

	sprintf_s(ConfigPath, MAX_PATH, "%s", filename);

	if (!ConfigWindowCreated && filename && filename[0] != '\0')
	{
		ConfigFrameAddr = CreateNewFrameAndShow("DotaConfigEditor");
		if (ConfigFrameAddr)
		{
			ShowFrameWithPosAlternativeAligned(ConfigFrameAddr, 0.0f, 0.6f, 2, 8);
			ConfigWindowCreated = TRUE;
		}
	}
	else
	{
		ConfigWindowCreated = FALSE;
		DestroyFrame(ConfigFrameAddr);
		ConfigFrameAddr = 0;
	}
	return TRUE;
}

pUpdateFrameFlags UpdateFrameFlags;


BOOL GlyphButtonEnabled = TRUE;
BOOL GlyphButtonCreated = FALSE;
int GlyphButtonAddr = 0;

jRCString CallBackFuncName;

int __stdcall CreateGlyphButton(const char * callbackfunc, BOOL create)
{
	LoadCustomFramesIfNeed();

	if (create && GlyphButtonCreated)
	{
		return 0;
	}

	if (create)
	{
		str2jstr(&CallBackFuncName, callbackfunc);

		GlyphButtonAddr = CreateNewFrameAndShow("GlyphButton");
		if (GlyphButtonAddr)
		{
			GlyphButtonCreated = TRUE;
			ShowFrameWithPos(GlyphButtonAddr, 0.155f, 0.006f);
		}
	}

	if (!create && GlyphButtonCreated)
	{
		GlyphButtonCreated = FALSE;
		DestroyFrame(GlyphButtonAddr);
		GlyphButtonAddr = 0;
	}


	return 0;
}



int __stdcall SetGlyphButtonEnabled(BOOL enabled)
{
	GlyphButtonEnabled = enabled;

	if (GlyphButtonCreated && GlyphButtonAddr)
	{
		int GlyphButtonItemFrame = GetFrameItemAddress("GlyphItemButton", 0);
		if (GlyphButtonItemFrame > 0)
		{
			if (enabled)
			{
				if (!(*(UINT*)(GlyphButtonItemFrame + 0x1D4) & 1))
					*(UINT*)(GlyphButtonItemFrame + 0x1D4) += 1;
			}
			else
			{
				if ((*(UINT*)(GlyphButtonItemFrame + 0x1D4) & 1))
					*(UINT*)(GlyphButtonItemFrame + 0x1D4) -= 1;
			}

			//if ( !( *( UINT* )( GlyphButtonItemFrame + 0x1CC ) & 0x80 ) )
			//{
				// Block change focus
			//	*( UINT* )( GlyphButtonItemFrame + 0x1CC ) += 0x80;
			//}

			UpdateFrameFlags(GlyphButtonItemFrame, 0);
		}
	}
	return enabled;
}



void ProcessClickAtCustomFrames()
{
	/*if ( GlyphButtonCreated && GlyphButtonAddr )
	{
		int GlyphButtonItemFrame = GetFrameItemAddress( "GlyphItemButton", 0 );
		if ( GlyphButtonItemFrame > 0 )
		{
			int GlyphBtnFlags = *( UINT* )( GlyphButtonItemFrame + 0x1D4 );
			if ( GlyphBtnFlags & 1 && GlyphBtnFlags & 4 && GlyphBtnFlags & 0x10 )
			{
				ExecuteFunc( &CallBackFuncName );
			}
		}
	}*/
}

pWc3ControlClickButton Wc3ControlClickButton_ptr;
pWc3ControlClickButton Wc3ControlClickButton_org;

int __fastcall Wc3ControlClickButton_my(int btnaddr, int, int unk)
{
	int retval = Wc3ControlClickButton_ptr(btnaddr, unk);
	if (GlyphButtonCreated && GlyphButtonAddr)
	{
		int GlyphButtonItemFrame = GetFrameItemAddress("GlyphItemButton", 0);
		if (GlyphButtonItemFrame > 0)
		{
			if (btnaddr == GlyphButtonItemFrame)
			{
				ExecuteFunc(&CallBackFuncName);
			}
		}
	}
	return retval;
}
