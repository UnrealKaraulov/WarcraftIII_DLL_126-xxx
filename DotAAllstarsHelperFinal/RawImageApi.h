#pragma once

#include "Main.h"
#include "DotaClickHelper.h"
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
	int IsAltPressed;
	int IsCtrlPressed;
	int IsLeftButton;
	int offsetx;
	int offsety;
	int RawImageCustomId;
};



struct RawImageStruct
{
	int RawImage;
	unsigned int width;
	unsigned int height;

	StormBuffer img;
	StormBuffer imgFlipped;
	StormBuffer ingamebuffer;

	int ingame;
	std::string filename;
	int used_for_overlay;
	float overlay_x; // 0.0 1.0 
	float overlay_y; // 0.0 1.0
	int button;
	void* textureaddr;
	int needResetTexture;

	int MouseCallback;

	int	MouserExecuteFuncCallback;
	int	PacketCallback;
	jRCString MouseActionCallback;


	int IsMouseDown;
	int IsMouseEntered;
	unsigned int events;
	float overlay_x2;
	float overlay_y2;
	float overlay_x0;
	float overlay_y0;
	unsigned long MoveTime1;
	unsigned long MoveTime2;
	unsigned long SleepTime;
	unsigned long StartTimer;

	int RawImageCustomId;

	char* backup_img;
	unsigned int backup_width;
	unsigned int backup_height;

	int drawdevice;

	int UseImageCoords;
	int Flipped;

	RawImageStruct()
	{
		UseImageCoords = false;
		drawdevice = 0; // 0 = d3d8, 1 = d3d9, 2 = opengl
		backup_img = 0;
		backup_width = 0;
		backup_height = 0;
		width = 0;
		height = 0;
		RawImageCustomId = 0;
		//img = StormBuffer( );
		ingamebuffer = StormBuffer();
		imgFlipped = StormBuffer();
		ingame = false;
		filename = std::string();
		used_for_overlay = false;
		overlay_x = overlay_y = overlay_x0 = overlay_y0 = overlay_y2 = overlay_x2 = 0.0f;
		textureaddr = NULL;
		needResetTexture = false;
		RawImage = 0;
		events = 0;
		IsMouseDown = false;
		IsMouseEntered = false;
		MouseCallback = false;
		MouseActionCallback = jRCString();
		MouserExecuteFuncCallback = false;
		PacketCallback = false;
		button = false;
		MoveTime1 = 0;
		MoveTime2 = 0;
		SleepTime = 0;
		StartTimer = 0;
		Flipped = false;
	}
};

int __stdcall ClearRawImage(unsigned int RawImage, RGBAPix FillByte);
int __stdcall BackupRawImage(unsigned int RawImage);
int __stdcall RestoreRawImage(unsigned int RawImage);
int __stdcall CreateRawImage(int width, int height, RGBAPix defaultcolor);
int __stdcall LoadRawImage(const char* filename);
int __stdcall RawImage_EnableAutoFix(int enable);
int __stdcall RawImage_DrawImg(unsigned int RawImage, unsigned int RawImage2, int drawx, int drawy, int blendmode);
int __stdcall RawImage_DrawPixel(unsigned int RawImage, unsigned int x, unsigned int y, RGBAPix color);
int __stdcall RawImage_FillRectangle(unsigned int RawImage, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, RGBAPix color);
int __stdcall RawImage_DrawRect(unsigned int RawImage, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int size, RGBAPix color);
int __stdcall RawImage_DrawLine(unsigned int RawImage, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int size, RGBAPix color);
int __stdcall RawImage_DrawCircle(unsigned int RawImage, unsigned int x, unsigned int y, unsigned int radius, unsigned int size, RGBAPix color);
int __stdcall RawImage_FillCircle(unsigned int RawImage, unsigned int x, unsigned int y, unsigned int radius, RGBAPix color);
int __stdcall RawImage_EraseCircle(unsigned int RawImage, unsigned int x, unsigned int y, unsigned int radius, int inverse);
int __stdcall RawImage_LoadFontFromResource(const char* filepath);
int __stdcall RawImage_SetFontSettings(const char* fontname, int fontsize, unsigned int flags);
int __stdcall RawImage_DrawText(unsigned int RawImage, const char* text, unsigned int x, unsigned int y, RGBAPix color);
int __stdcall SaveRawImageToGameFile(unsigned int RawImage, const char* filename, int IsTga, int enabled);
int __stdcall DumpRawImageToFile(unsigned int RawImage, const char* filename);
int __stdcall GetRawImageByFile(const char* filename);
int __stdcall RawImage_GetWidth(unsigned int RawImage);
int __stdcall RawImage_GetHeight(unsigned int RawImage);
int __stdcall RawImage_Resize(unsigned int RawImage, unsigned int newwidth, unsigned int newheight);
int __stdcall RawImage_DrawOverlay(unsigned int RawImage, int enabled, float xpos, float ypos);
int __stdcall RawImage_EnableOverlay(unsigned int RawImage, int enabled);
int __stdcall RawImage_MoveTimed(unsigned int RawImage, float x2, float y2, unsigned int Time1, unsigned int Time2, unsigned int SleepTime);
int __stdcall RawImage_SetPacketCallback(unsigned int RawImage, int enable, unsigned int events);
int __stdcall RawImage_AddCallback(unsigned int RawImage, const char* MouseActionCallback, RawImageCallbackData* callbackdata, unsigned int events);
int __stdcall RawImage_IsBtn(unsigned int RawImage, int enabled);
int __stdcall RawImage_UseImageCoords(unsigned int RawImage, int enabled);
int RawImageGlobalCallbackFunc(RawImageEventType callbacktype, float mousex, float mousey);
void ApplyIconFrameFilter(std::string filename, int* OutDataPointer, size_t* OutSize);
void ClearAllRawImages();
int __stdcall GetScreenWidth(int);
int __stdcall GetScreenHeight(int);
int __stdcall GetWindowWidth(int);
int __stdcall GetWindowHeight(int);
int __stdcall SetDefaultSceenSize(int w, int h);

extern RawImageCallbackData* GlobalRawImageCallbackData;
extern std::vector<RawImageStruct> ListOfRawImages;

float RawImageOffsetToWar3_X(float raw_offset_x);
float RawImageOffsetToWar3_Y(float raw_offset_y);