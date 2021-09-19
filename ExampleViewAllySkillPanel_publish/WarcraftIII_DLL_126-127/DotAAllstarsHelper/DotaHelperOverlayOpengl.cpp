#include "Main.h"
#include "BlpReadWrite.h"
#include <windows.h>
#include <string>
#include <tlhelp32.h>
#include <iostream>
#define GL_GLEXT_PROTOTYPES
#include <gl\gl.h>
#include <gl\glu.h>

#pragma comment(lib,"opengl32.lib")

typedef BOOL( __stdcall * wglSwapLayerBuffers_p )( HDC, UINT );

wglSwapLayerBuffers_p wglSwapLayerBuffers_org;
wglSwapLayerBuffers_p wglSwapLayerBuffers_ptr;

void DrawAllRawImages( )
{
	glEnable( GL_BLEND );
	float ScreenX = *GetWindowXoffset;
	float ScreenY = *GetWindowYoffset;


	glViewport( 0, 0, ( GLsizei )ScreenX, ( GLsizei )ScreenY );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );

	glOrtho( 0.0, ScreenX, ScreenY, 0, -1.0, 1.0 );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );

	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	float scalex = ScreenX / DefaultSceenWidth;
	float scaley = ScreenY / DefaultSceenHeight;


	//scalex *= DesktopScreen_Width / DefaultSceenWidth;
	//scaley *= DesktopScreen_Height / DefaultSceenHeight;
	glPixelZoom( scalex, -scaley );

	for ( auto & img : ListOfRawImages )
	{
		if ( !img.used_for_overlay )
			continue;

		//	glTranslatef( ScreenX *img.overlay_x, ScreenY *img.overlay_y, 0.0f );

			//float AspectRatio = DesktopScreen_Height / DesktopScreen_Width;
			//float AspectRatio2 = DesktopScreen_Width / DesktopScreen_Height;

		
		if ( !img.Flipped  || img.needResetTexture )
		{
			img.needResetTexture = FALSE;
			img.imgFlipped.Clear( );
			img.imgFlipped.Clone( img.img );
			img.Flipped = TRUE;
			flip_vertically( reinterpret_cast< BYTE* >( &img.imgFlipped.buf[ 0 ] ), img.width, img.height, 4 );
		}


		if ( img.MoveTime1 )
		{
			float lenx = abs( img.overlay_x - img.overlay_x2 );
			float leny = abs( img.overlay_y - img.overlay_y2 );

			if ( lenx > 0.004 )
			{
				if ( img.overlay_x > img.overlay_x2 )
					img.overlay_x -= 0.002;
				else if ( img.overlay_x < img.overlay_x2 )
					img.overlay_x += 0.002;
			}

			if ( leny > 0.004 )
			{
				if ( img.overlay_y > img.overlay_y2 )
					img.overlay_y -= 0.002;
				else if ( img.overlay_y < img.overlay_y2 )
					img.overlay_y += 0.002;
			}

			DWORD newTickImg = GetTickCount( ) - img.StartTimer;
			if ( newTickImg > img.MoveTime1 )
				img.MoveTime1 = 0;
			else
				img.MoveTime1 -= newTickImg;

			img.StartTimer = GetTickCount( );
		}
		else if ( img.SleepTime )
		{
			DWORD newTickImg = GetTickCount( ) - img.StartTimer;
			if ( newTickImg > img.SleepTime )
				img.SleepTime = 0;
			else
				img.SleepTime -= newTickImg;

			img.StartTimer = GetTickCount( );
		}
		else if ( img.MoveTime2 )
		{

			float lenx = abs( img.overlay_x - img.overlay_x0 );
			float leny = abs( img.overlay_y - img.overlay_y0 );

			if ( lenx > 0.004 )
			{
				if ( img.overlay_x > img.overlay_x0 )
					img.overlay_x -= 0.002;
				else if ( img.overlay_x < img.overlay_x0 )
					img.overlay_x += 0.002;
			}

			if ( leny > 0.004 )
			{
				if ( img.overlay_y > img.overlay_y0 )
					img.overlay_y -= 0.002;
				else if ( img.overlay_y < img.overlay_y0 )
					img.overlay_y += 0.002;
			}



			DWORD newTickImg = GetTickCount( ) - img.StartTimer;
			if ( newTickImg > img.MoveTime2 )
				img.MoveTime2 = 0;
			else
				img.MoveTime2 -= newTickImg;

			img.StartTimer = GetTickCount( );
		}


		glRasterPos3f( ScreenX *img.overlay_x, ScreenY *img.overlay_y, 0.0f );
		glDrawPixels( img.width, img.height, GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.imgFlipped.buf );

	}

	glDisable( GL_BLEND );
	glFlush( );
}

HGLRC DotaGlobalOverlay_OPENGL = NULL;
HDC GlobalDc = NULL;

BOOL __stdcall wglSwapLayerBuffers_my( HDC dc, UINT b )
{
	if ( !dc || !(IsGame( )) ) {
		return	wglSwapLayerBuffers_ptr( dc, b );
	}

	GlobalDc = dc;

	if ( !DotaGlobalOverlay_OPENGL )
		DotaGlobalOverlay_OPENGL = wglCreateContext( dc );

	/*HGLRC oldcontext = wglGetCurrentContext( );

	wglMakeCurrent( dc, DotaGlobalOverlay_OPENGL );

	DrawAllRawImages( );

	wglMakeCurrent( dc, oldcontext );*/

	return wglSwapLayerBuffers_ptr( dc, b );
}

void DrawOverlayGl( )
{
	if ( !GlobalDc || !(IsGame( )) ) {
		return;
	}
	if ( !DotaGlobalOverlay_OPENGL )
		DotaGlobalOverlay_OPENGL = wglCreateContext( GlobalDc );


	HGLRC oldcontext = wglGetCurrentContext( );

	wglMakeCurrent( GlobalDc, DotaGlobalOverlay_OPENGL );

	DrawAllRawImages( );

	wglMakeCurrent( GlobalDc, oldcontext );


}


void UninitOpenglHook( )
{
	if ( wglSwapLayerBuffers_org )
		MH_DisableHook( wglSwapLayerBuffers_org );
	wglSwapLayerBuffers_org = NULL;
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

void InitOpenglHook( )
{
	HMODULE hOpenGL = GetModuleHandleA( "opengl32.dll" );
	if ( hOpenGL )
	{
		wglSwapLayerBuffers_org = ( wglSwapLayerBuffers_p )GetProcAddress( hOpenGL, "wglSwapLayerBuffers" );
		if ( !wglSwapLayerBuffers_org )
		{
			MessageBoxA( 0, "Can't install opengl hook. Error 2", "Error", 0 );
		}
		/*if ( MH_STATUS::MH_OK !=*/ MH_CreateHook( wglSwapLayerBuffers_org, &wglSwapLayerBuffers_my, reinterpret_cast< void** >( &wglSwapLayerBuffers_ptr ) );
		//{
			/*MessageBoxA( 0, "Can't install opengl hook. Error 3", "Error", 0 );*/
		//}
			/*if ( MH_STATUS::MH_OK !=*/ MH_EnableHook( wglSwapLayerBuffers_org );/* )
		{
			MessageBoxA( 0, "Can't install opengl hook. Error 4", "Error", 0 );
		}*/
	}
	else
	{
		//MessageBoxA( 0, "Can't install opengl hook. Error 1", "Error", 0 );
	}
	//	glGetIntegerv( GL_MAX_VIEWPORT_DIMS, maxviewport );

}

