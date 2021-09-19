#pragma once
#include "buffer.h"
#include "Structures.h"

using namespace Storm;
#define OLD_CODE


enum class RawImageEventType : unsigned int
{
	MouseUp = 1U,
	MouseDown = 2U,
	MouseClick = 4U,
	MouseEnter = 8U,
	MouseLeave = 16U,
	MouseMove = 32U,
	ALL = 63U
};


struct RawImageCallbackData
{
	int RawImage;
	RawImageEventType EventType;
	float mousex;
	float mousey;
	BOOL IsAltPressed;
	BOOL IsCtrlPressed;
	BOOL IsLeftButton;
	int offsetx;
	int offsety;
	int RawImageCustomId;
};



struct RawImageStruct
{
	int RawImage; 
	int width;
	int height;

	StormBuffer img;
	StormBuffer imgFlipped;
	StormBuffer ingamebuffer;

	BOOL ingame;
	string filename;
	BOOL used_for_overlay;
	float overlay_x; // 0.0 1.0 
	float overlay_y; // 0.0 1.0
	BOOL button;
	void * textureaddr;
	BOOL needResetTexture;

	BOOL MouseCallback;

	BOOL	MouserExecuteFuncCallback;
	BOOL	PacketCallback;
	jRCString MouseActionCallback;


	BOOL IsMouseDown;
	BOOL IsMouseEntered;
	unsigned int events;
	float overlay_x2;
	float overlay_y2;
	float overlay_x0;
	float overlay_y0;
	DWORD MoveTime1;
	DWORD MoveTime2;
	DWORD SleepTime;
	DWORD StartTimer;

	int RawImageCustomId;

	char * backup_img;
	int backup_width;
	int backup_height;

	int drawdevice;

	BOOL UseImageCoords;
	BOOL Flipped;

	RawImageStruct( )
	{
		UseImageCoords = FALSE;
		drawdevice = 0; // 0 = d3d8, 1 = d3d9, 2 = opengl
		backup_img = 0;
		backup_width = 0;
		backup_height = 0;
		width = 0;
		height = 0;
		RawImageCustomId = 0;
		//img = StormBuffer( );
		ingamebuffer = StormBuffer( );
		imgFlipped = StormBuffer( );
		ingame = FALSE;
		filename = string( );
		used_for_overlay = FALSE;
		overlay_x = overlay_y = overlay_x0 = overlay_y0 = overlay_y2 = overlay_x2 = 0.0f;
		textureaddr = NULL;
		needResetTexture = FALSE;
		RawImage = 0;
		events = 0;
		IsMouseDown = FALSE;
		IsMouseEntered = FALSE;
		MouseCallback = FALSE;
		MouseActionCallback = jRCString( );
		MouserExecuteFuncCallback = FALSE;
		PacketCallback = FALSE;
		button = FALSE;
		MoveTime1 = 0;
		MoveTime2 = 0;
		SleepTime = 0;
		StartTimer = 0;
		Flipped = FALSE;
	}
};
