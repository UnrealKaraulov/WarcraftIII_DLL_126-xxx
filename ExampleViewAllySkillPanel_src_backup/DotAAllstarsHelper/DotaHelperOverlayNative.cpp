#include "Main.h"
#include "RawImageApi.h"
#include <Tools.h>
#include <UISimpleFrame.h>
#include <Button.h>
#include <CheckBox.h>
#include <DropDownList.h>
#include <Label.h>
#include <ToolTip.h>
#include <HotkeyButton.h>
#include <EditBoxButton.h>
#include <Game.h>
#include <War3Window.h>
#include <KeyCode.h>
#include <UIObject.h>
#include <UISimpleTexture.h>
#include <Input.h>
#include <Timer.h>
#include "DotaDreamRawImage.h"

bool NativeOverlayInitialized = false;

/* Warcraft 'native' render for RawImages. In progress... */


void TestTimer(Timer *tm)
{
	if (IsGame() && *IsWindowActive)
	{
		float x = GetMousePosition()->x;
		float y = GetMousePosition()->y;

		float x2 = GetScreenPositionFromWar3(x, y)->x;
		float y2 = GetScreenPositionFromWar3(x, y)->y;

		float x3 = *GetWindowXoffset;
		float y3 = *GetWindowYoffset;

		static RECT ClientRect;
		static POINT CursorPoint;

		HWND hwnd = GetWar3Window();
		if (GetClientRect(hwnd, &ClientRect) && GetCursorPos(&CursorPoint)) {
			int width = ClientRect.right;
			int height = ClientRect.bottom;
			if (ScreenToClient(hwnd, &CursorPoint)) {
				char tempbuf[256];
				sprintf_s(tempbuf, "WC3 COORDINATES: %f / %f", x, y);
				PrintText(tempbuf, 5.0f);
				sprintf_s(tempbuf, "WINDOWS COORDINATES: %f / %f ----- %f / %f", x2, y2, x3, y3);
				PrintText(tempbuf, 5.0f);
				sprintf_s(tempbuf, "REAL WINDOWS COORDINATES: %i / %i", CursorPoint.x, CursorPoint.y);
				PrintText(tempbuf, 5.0f);
			}
		}
	}
}

int updateid = 0;

void UpdateDreamRawImages()
{
	if (!IsGame())
		return;

	float ScreenX = *GetWindowXoffset;
	float ScreenY = *GetWindowYoffset;

	float scalex = ScreenX / DefaultSceenWidth;
	float scaley = ScreenY / DefaultSceenHeight;

	char tmpDreamBuffer[256];


	for (auto & s : ListOfRawImages)
	{
		if (s.used_for_overlay)
		{
			/*

			  Width = 64
			  WindowsWidth = 1440

			  Get one pixel size = 0.8 / 1440
			  Get 64 pixels size =

			  ...

			*/
			//char tempbuf[ 256 ];
			//sprintf_s( tempbuf, "image timer..." );
			if (s.needResetTexture)
			{
				if (IsKeyPressed(VK_F1))
					continue;

				if (s.DreamTexture)
				{
					UISimpleTexture::Destroy(s.DreamTexture);
					s.DreamTexture = NULL;
				}
				s.DreamTexture = UISimpleTexture::Create(UISimpleFrame::GetGameSimpleConsole());
				s.DreamTexture->setWidthForce( /*scalex **/ GetWar3ScreenPosition(s.width, s.height)->x);
				s.DreamTexture->setHeight( /*scaley * */(0.6f - GetWar3ScreenPosition(s.width, s.height)->y));
				updateid++;
				sprintf_s(tmpDreamBuffer, "%s%i%i.tga", "rawimage", s.RawImage, updateid);
				//SaveRawImageToGameFile( s.RawImage, tmpDreamBuffer, TRUE, TRUE );

				BOOL oldlocalfilestate = IsLocalFilesEnabled();
				EnableLocalFiles(TRUE);
				DumpRawImageToFile(s.RawImage, tmpDreamBuffer);
				s.DreamTexture->fillBitmap(tmpDreamBuffer);
				s.DreamTexture->setAbsolutePosition(0, RawImageOffsetToWar3_X(s.overlay_x),
					RawImageOffsetToWar3_Y(s.overlay_y));
				s.needResetTexture = FALSE;
				EnableLocalFiles(oldlocalfilestate);
				DeleteFileA(tmpDreamBuffer);
				/*sprintf_s( tempbuf, "Image X/Y: %f / %f. Image Size W/H: %i / %i -> %f / %f", RawImageOffsetToWar3_X( s.overlay_x ),
					RawImageOffsetToWar3_Y( s.overlay_y ), s.width, s.height, GetWar3ScreenPosition( s.width, s.height )->x,
					GetWar3ScreenPosition( s.width, s.height )->y );*/
			}
			//PrintText( tempbuf, 5.0f );
		}
		else
		{
			if (s.DreamTexture)
			{
				UISimpleTexture::Destroy(s.DreamTexture);
				s.DreamTexture = NULL;
			}
		}
	}
	//TestTimer( 0 );
}



void TestTimer2(Timer *tm)
{
	UpdateDreamRawImages();
}



void InitDreamRawImages()
{
	if (!NativeOverlayInitialized)
	{
		NativeOverlayInitialized = true;
		/*int RawImageTest = CreateRawImage(128, 128, RGBAPix(255, 0, 0, 255));
		RawImage_DrawOverlay(RawImageTest, TRUE, 0.1f, 0.1f);*/
		GetTimer(1.20, TestTimer2, true, TimeType::TimeGame)->start();
	}
}

void UnInitDreamRawImages()
{
	if (NativeOverlayInitialized)
	{
		NativeOverlayInitialized = false;
		for (auto & s : ListOfRawImages)
		{
			if (s.used_for_overlay)
			{
				if (s.DreamTexture)
				{
					UISimpleTexture::Destroy(s.DreamTexture);
					s.DreamTexture = NULL;
				}
			}
		}
	}
}
