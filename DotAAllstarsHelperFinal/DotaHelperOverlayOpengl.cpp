#include "Main.h"
#define GL_GLEXT_PROTOTYPES
#include <gl\gl.h>
#include <gl\glu.h>
#include <wglext.h>
#pragma comment(lib,"opengl32.lib")
#include "RawImageApi.h"

typedef int(__stdcall* wglSwapLayerBuffers_p)(HDC, unsigned int);
bool OpnglHookInitialized = false;

wglSwapLayerBuffers_p wglSwapLayerBuffers_org;
wglSwapLayerBuffers_p wglSwapLayerBuffers_ptr;

void DrawAllRawImages()
{
	if (!OpnglHookInitialized)
		return;

	glEnable(GL_BLEND);
	float ScreenX = *GetWindowXoffset;
	float ScreenY = *GetWindowYoffset;


	glViewport(0, 0, (GLsizei)ScreenX, (GLsizei)ScreenY);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0.0, ScreenX, ScreenY, 0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float scalex = ScreenX / DefaultSceenWidth;
	float scaley = ScreenY / DefaultSceenHeight;


	//scalex *= DesktopScreen_Width / DefaultSceenWidth;
	//scaley *= DesktopScreen_Height / DefaultSceenHeight;
	glPixelZoom(scalex, -scaley);

	for (auto& img : ListOfRawImages)
	{
		if (!img.used_for_overlay)
			continue;

		//	glTranslatef( ScreenX *img.overlay_x, ScreenY *img.overlay_y, 0.0f );

			//float AspectRatio = DesktopScreen_Height / DesktopScreen_Width;
			//float AspectRatio2 = DesktopScreen_Width / DesktopScreen_Height;


		if (!img.Flipped || img.needResetTexture)
		{
			img.needResetTexture = false;
			img.imgFlipped.Clear();
			img.imgFlipped.copy_to_new(img.img);
			img.Flipped = true;
			flip_vertically(reinterpret_cast<unsigned char*>(&img.imgFlipped.buf[0]), img.width, img.height, 4);
		}


		if (img.MoveTime1)
		{
			float lenx = abs(img.overlay_x - img.overlay_x2);
			float leny = abs(img.overlay_y - img.overlay_y2);

			if (lenx > 0.004f)
			{
				if (img.overlay_x > img.overlay_x2)
					img.overlay_x -= 0.002f;
				else if (img.overlay_x < img.overlay_x2)
					img.overlay_x += 0.002f;
			}

			if (leny > 0.004f)
			{
				if (img.overlay_y > img.overlay_y2)
					img.overlay_y -= 0.002f;
				else if (img.overlay_y < img.overlay_y2)
					img.overlay_y += 0.002f;
			}

			unsigned long newTickImg = GetTickCount() - img.StartTimer;
			if (newTickImg > img.MoveTime1)
				img.MoveTime1 = 0;
			else
				img.MoveTime1 -= newTickImg;

			img.StartTimer = GetTickCount();
		}
		else if (img.SleepTime)
		{
			unsigned long newTickImg = GetTickCount() - img.StartTimer;
			if (newTickImg > img.SleepTime)
				img.SleepTime = 0;
			else
				img.SleepTime -= newTickImg;

			img.StartTimer = GetTickCount();
		}
		else if (img.MoveTime2)
		{

			float lenx = abs(img.overlay_x - img.overlay_x0);
			float leny = abs(img.overlay_y - img.overlay_y0);

			if (lenx > 0.004f)
			{
				if (img.overlay_x > img.overlay_x0)
					img.overlay_x -= 0.002f;
				else if (img.overlay_x < img.overlay_x0)
					img.overlay_x += 0.002f;
			}

			if (leny > 0.004f)
			{
				if (img.overlay_y > img.overlay_y0)
					img.overlay_y -= 0.002f;
				else if (img.overlay_y < img.overlay_y0)
					img.overlay_y += 0.002f;
			}



			unsigned long newTickImg = GetTickCount() - img.StartTimer;
			if (newTickImg > img.MoveTime2)
				img.MoveTime2 = 0;
			else
				img.MoveTime2 -= newTickImg;

			img.StartTimer = GetTickCount();
		}


		glRasterPos3f(ScreenX * img.overlay_x, ScreenY * img.overlay_y, 0.0f);
		glDrawPixels((GLsizei)img.width, (GLsizei)img.height, GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.imgFlipped.buf);

	}

	glDisable(GL_BLEND);
	glFlush();
}

HGLRC DotaGlobalOverlay_OPENGL = NULL;
HDC GlobalDc = NULL;

int __stdcall wglSwapLayerBuffers_my(HDC dc, unsigned int b)
{
	if (!dc || !(IsGame())) {
		return	wglSwapLayerBuffers_ptr(dc, b);
	}

	GlobalDc = dc;

	if (!DotaGlobalOverlay_OPENGL)
		DotaGlobalOverlay_OPENGL = wglCreateContext(dc);

	/*HGLRC oldcontext = wglGetCurrentContext( );

	wglMakeCurrent( dc, DotaGlobalOverlay_OPENGL );

	DrawAllRawImages( );

	wglMakeCurrent( dc, oldcontext );*/

	return wglSwapLayerBuffers_ptr(dc, b);
}

void DrawOverlayGl()
{
	if (!GlobalDc || !(IsGame()) || !OpnglHookInitialized) {
		return;
	}
	if (!DotaGlobalOverlay_OPENGL)
		DotaGlobalOverlay_OPENGL = wglCreateContext(GlobalDc);


	HGLRC oldcontext = wglGetCurrentContext();

	wglMakeCurrent(GlobalDc, DotaGlobalOverlay_OPENGL);
	// clear with transparent
	DrawAllRawImages();

	wglMakeCurrent(GlobalDc, oldcontext);
}


void UninitOpenglHook()
{
	if (wglSwapLayerBuffers_org && OpnglHookInitialized)
		MH_DisableHook(wglSwapLayerBuffers_org);
	wglSwapLayerBuffers_org = NULL;
	OpnglHookInitialized = false;
}

//
//typedef int( __fastcall  * DrawWc3UI_p )( int );
//DrawWc3UI_p DrawWc3UI_org;
//DrawWc3UI_p DrawWc3UI_ptr;
//
//int __fastcall  DrawWc3UI_my( int a1 )
//{
//	if ( !GlobalDC || !(IsGame( )) ) {
//		return DrawWc3UI_ptr( a1 );
//	}
//
//
//
//	if ( !DotaGlobalOverlay_OPENGL )
//		DotaGlobalOverlay_OPENGL = wglCreateContext( GlobalDC );
//
//	HGLRC oldcontext = wglGetCurrentContext( );
//
//	wglMakeCurrent( GlobalDC, DotaGlobalOverlay_OPENGL );
//
//	DrawAllRawImages( );
//
//	wglMakeCurrent( GlobalDC, oldcontext );
//
//
//	return DrawWc3UI_ptr( a1 );
//}DrawWc3UI_org = ( DrawWc3UI_p )( GameDll + 0x6290C0 );
//MH_CreateHook( DrawWc3UI_org, &DrawWc3UI_my, reinterpret_cast< void** >( &DrawWc3UI_ptr ) );
//MH_EnableHook( DrawWc3UI_org );

void InitOpenglHook()
{
	if (!OpnglHookInitialized)
	{
		OpnglHookInitialized = true;
		void* hOpenGL = GetModuleHandleA("opengl32.dll");
		if (hOpenGL)
		{
			wglSwapLayerBuffers_org = (wglSwapLayerBuffers_p)GetProcAddress((HMODULE)hOpenGL, "wglSwapLayerBuffers");
			MH_CreateHook(wglSwapLayerBuffers_org, &wglSwapLayerBuffers_my, reinterpret_cast<void**>(&wglSwapLayerBuffers_ptr));
			MH_EnableHook(wglSwapLayerBuffers_org);
		}
	}
}

