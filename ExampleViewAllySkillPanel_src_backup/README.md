# WarcraftIII_DLL_126-127
## Improvements for Warcraft III 126a and 127a, used in new DoTA (d1stats.ru)

# Features:

>
>	"Attack Speed" and "Magic amplification"
>![More unit info](/Images/AttackSpeedAndMagicAmplification.jpg?raw=true "You can see real Attack Speed and Magic amplification")
>
>
>	"Move Speed" and "Magic Protection" 
>![More unit info](/Images/MagicProtectionAndMoveSpeed.jpg?raw=true "You can see real Move Speed and Magic protection")
>	
>
>	"Widescreen support"
>
>*(Widescreen support disabled:)*
>
>![Widescreen off](/Images/WideScreen_NO.jpg?raw=true "Widescreen support disabled 16:9")
>
>*(Widescreen support enabled:)*
>
>![Widescreen on](/Images/WideScreen_YES.jpg?raw=true "Widescreen support enabled 16:9")
>
>
>
>	"Ally skill view"
>![Ally skill viewer](/Images/AllySkillView.jpg?raw=true "You can see ally skills")
>
>
>	"Ally manabar view"
>![Ally manabar viewer](/Images/ManaBar.jpg?raw=true "You can see ally manabars")


# RawImage update:
```cpp
// Создает RawImage (RGBA) с указанным цветом
// Create RawImage with color
int __stdcall CreateRawImage( int width, int height, RGBAPix defaultcolor )
// Загружает RawImage из filename (tga,blp)
// Load RawImage from game file
int __stdcall LoadRawImage( const char * filename )
// Рисует RawImage2 на RawImage
// Draw RawImage2 at RawImage
int __stdcall RawImage_DrawImg( int RawImage, int RawImage2, int drawx, int drawy, int blendmode )
// Заполняет выбранный пиксель указанным цветом
// Fill pixel with color
int __stdcall RawImage_DrawPixel( int RawImage, int x, int y, RGBAPix color )//RGBAPix = unsigned int
// Рисует прямоугольник с указанным цветом и размером
// Fill rectangle with color
int __stdcall RawImage_DrawRect( int RawImage, int drawx, int drawy, int widthsize, int heightsize, RGBAPix color )
// Рисует линию с указанным цветом и размером
// Draw line with color
int __stdcall RawImage_DrawLine( int RawImage, int x1, int y1, int x2, int y2, int size, RGBAPix color )
// Рисует круг с указанным радиусом, толщиной, цветом
// Draw circle with color
int __stdcall RawImage_DrawCircle( int RawImage, int x, int y, int radius, int size, RGBAPix color )
// Заполняет круг указанным цветом
// Fill circle with color
int __stdcall RawImage_FillCircle( int RawImage, int x, int y, int radius, RGBAPix color )
// Оставляет только круг с указанным радиусом
// Erase circle
int __stdcall RawImage_EraseCircle( int RawImage, int x, int y, int radius, BOOL inverse )
// Делает пиксели с цветом color - прозрачными, power от 0 до 255
// Not working
int __stdcall RawImage_EraseColor( int RawImage, RGBAPix color, int power )
// Загружает шрифт из файла
// Load font from game file
int __stdcall RawImage_LoadFontFromResource( const char * filepath )
// Устанавливает настройки шрифта для RawImage_DrawText
// Set font settings for DrawText
int __stdcall RawImage_SetFontSettings( const char * fontname, int fontsize, unsigned int flags )
// Пишет текст в указанных координатах с указанными цветом и настройками шрифта RawImage_SetFontSettings
// Draw text at RawImage
int __stdcall RawImage_DrawText( int RawImage, const char * text, int x, int y, RGBAPix color )
// Сохраняет RawImage в blp и делает доступным для использования в игре
// Save RawImage to game file
int __stdcall SaveRawImageToGameFile( int RawImage, const char * filename, BOOL IsTga, BOOL enabled )
// Сохраняет RawImage на диск в TGA по выбранному пути
// Save RawIamge to file
int __stdcall DumpRawImageToFile( int RawImage, const char * filename )
// Получает RawImage из списка RawImages по имени файла.
// Search RawImage by filename
int __stdcall GetRawImageByFile( const char * filename )
// Получает ширину RawImage
// Get width
int __stdcall RawImage_GetWidth( int RawImage )
// Получает высоту RawImage
// Get height
int __stdcall RawImage_GetHeight( int RawImage )
// Изменяет размер RawImage
// Resize RawImage
int __stdcall RawImage_Resize( int RawImage, int newwidth, int newheight )
// Рисует RawImage по заданным координатам (от 0.0 до 1.0) в игре. Можно установить размер (от 0.0 до 1.0).
// Draw RawImage as overlay ingame. xsize / ysize not working in this version (use 0.0)
int __stdcall RawImage_DrawOverlay( int RawImage, BOOL enabled, float xpos, float ypos, float xsize, float ysize )
int __stdcall RawImage_EnableOverlay( unsigned int RawImage, BOOL enabled )
int __stdcall RawImage_MoveTimed( unsigned int RawImage, float x2, float y2, unsigned int Time1, unsigned int Time2, unsigned int SleepTime )
int __stdcall RawImage_SetPacketCallback( unsigned int RawImage, BOOL enable, unsigned int events )
int __stdcall RawImage_AddCallback( unsigned int RawImage, const char * MouseActionCallback, RawImageCallbackData * callbackdata, unsigned int events )
int __stdcall RawImage_IsBtn( unsigned int RawImage, BOOL enabled )

```
"Example result:"
![RawImage Draw API](/Images/DrawRawImageApi.png?raw=true "You can see result of using experimental Draw API")

# CFrame API update: 

# globals 
```
	integer testclickcount = 0


	integer pCFrame_GetLastEventId = 0 
	integer pCFrame_GetTriggerCFrame = 0
	integer pCFrame_SetCustomValue = 0
	integer pCFrame_GetCustomValue = 0
	integer pCFrame_LoadFramesListFile = 0 
	integer pCFrame_CreateNewFrame = 0 
	integer pCFrame_LoadFrame = 0 
	integer pCFrame_SetFrameType = 0 
	integer pCFrame_SetFrameModel = 0 
	integer pCFrame_SetFrameTexture = 0 
	integer pCFrame_SetFrameText = 0 
	integer pCFrame_SetAbsolutePosition = 0 
	integer pCFrame_SetRelativePosition = 0 
	integer pCFrame_Destroy = 0 
	integer pCFrame_AddCallack = 0 
	integer pCFrame_AddCallackPacket = 0 
	integer pCFrame_Enable = 0
	integer pCFrame_IsEnabled = 0
	integer pCFrame_GetFrameAddress = 0
	integer pCFrame_StartCustomAnimate = 0
	integer	pCFrame_SetCustomAnimateOffset = 0
	integer	pCFrame_StopCustomAnimate = 0
	integer pCFrame_SetScale = 0
	integer pCFrame_Show = 0
	
	integer CFrameBackType_ControlFrame = 0
	integer CFrameBackType_ControlBackdrop = 1
	integer CFrameBackType_ControlPushedBackdrop = 2
	integer CFrameBackType_ControlDisabledBackdrop = 3

	integer	CFramePosition_TOP_LEFT = 0
	integer	CFramePosition_TOP_CENTER = 1
	integer	CFramePosition_TOP_RIGHT = 2
	integer	CFramePosition_LEFT = 3
	integer	CFramePosition_CENTER = 4
	integer	CFramePosition_RIGHT = 5
	integer	CFramePosition_BOTTOM_LEFT = 6
	integer	CFramePosition_BOTTOM_CENTER = 7
	integer	CFramePosition_BOTTOM_RIGHT = 8

	integer	FRAMETYPE_FRAME = 0
	integer	FRAMETYPE_BACKDROP = 1
	integer	FRAMETYPE_ITEM = 2
	integer	FRAMETYPE_POPUPMENU = 3
	integer	FRAMETYPE_EDITBOX = 4
	integer	FRAMETYPE_BUTTON = 5
	integer	FRAMETYPE_TEXTBUTTON = 6
	integer	FRAMETYPE_SPRITE = 7
	integer	FRAMETYPE_HIGHLIGHT = 8
	integer	FRAMETYPE_SCROLLBAR = 9
	integer	FRAMETYPE_MODEL = 10

	integer FRAME_EVENT_PRESSED = 1
	integer FRAME_MOUSE_ENTER = 2
	integer FRAME_MOUSE_LEAVE = 3
	integer FRAME_MOUSE_UP = 4
	integer FRAME_MOUSE_DOWN = 5
	integer FRAME_MOUSE_WHEEL = 6
	integer FRAME_FOCUS_ENTER = FRAME_MOUSE_ENTER
	integer FRAME_FOCUS_LEAVE = FRAME_MOUSE_LEAVE
	integer FRAME_CHECKBOX_CHECKED = 7
	integer FRAME_CHECKBOX_UNCHECKED = 8
	integer FRAME_EDITBOX_TEXT_CHANGED = 9
	integer FRAME_POPUPMENU_ITEM_CHANGE_START = 10
	integer FRAME_POPUPMENU_ITEM_CHANGED = 11
	integer FRAME_MOUSE_DOUBLECLICK = 12
	integer FRAME_SPRITE_ANIM_UPDATE = 13
	
```		

#endglobals

#code 

```


function CFrame_GetLastEventId takes nothing returns integer
	if pCFrame_GetLastEventId == 0 then
		set pCFrame_GetLastEventId = GetModuleProcAddress(EXTRADLLNAME, "CFrame_GetLastEventId")
	endif
	if pCFrame_GetLastEventId != 0 then 
		return CallStdCallWith1Args(pCFrame_GetLastEventId,0)
	endif
	return 0
endfunction

function CFrame_GetTriggerCFrame takes nothing returns integer
	if pCFrame_GetTriggerCFrame == 0 then
		set pCFrame_GetTriggerCFrame = GetModuleProcAddress(EXTRADLLNAME, "CFrame_GetTriggerCFrame")
	endif
	if pCFrame_GetTriggerCFrame != 0 then 
		return CallStdCallWith1Args(pCFrame_GetTriggerCFrame,0)
	endif
	return 0
endfunction

function CFrame_GetCustomValue takes integer pCframe, integer ValueID returns integer
	if pCFrame_GetCustomValue == 0 then
		set pCFrame_GetCustomValue = GetModuleProcAddress(EXTRADLLNAME, "CFrame_GetCustomValue")
	endif
	if pCFrame_GetCustomValue != 0 then 
		return CallStdCallWith2Args(pCFrame_GetCustomValue,pCframe,ValueID)
	endif
	return 0	
endfunction

function CFrame_SetCustomValue takes integer pCframe, integer ValueID, integer customval returns nothing
	if pCFrame_SetCustomValue == 0 then
		set pCFrame_SetCustomValue = GetModuleProcAddress(EXTRADLLNAME, "CFrame_SetCustomValue")
	endif
	if pCFrame_SetCustomValue != 0 then 
		call CallStdCallWith3Args(pCFrame_SetCustomValue,pCframe,ValueID,customval)
	endif
endfunction

function CFrame_LoadFramesListFile takes string filepath, boolean reload returns nothing
	if pCFrame_LoadFramesListFile == 0 then
		set pCFrame_LoadFramesListFile = GetModuleProcAddress(EXTRADLLNAME, "CFrame_LoadFramesListFile")
	endif
	if pCFrame_LoadFramesListFile != 0 then 
		call CallStdCallWith2Args(pCFrame_LoadFramesListFile,GetStringAddress(filepath),B2I(reload))
	endif
endfunction

function CFrame_CreateNewFrame takes string framename, integer relativeframe, boolean show, integer frameid returns integer
	if pCFrame_CreateNewFrame == 0 then
		set pCFrame_CreateNewFrame = GetModuleProcAddress(EXTRADLLNAME, "CFrame_CreateNewFrame")
	endif
	if pCFrame_CreateNewFrame != 0 then 
		return CallStdCallWith4Args(pCFrame_CreateNewFrame,GetStringAddress(framename),relativeframe,B2I(show),frameid)
	endif
	return 0
endfunction

function CFrame_LoadFrame takes string framename, integer frameid returns integer
	if pCFrame_LoadFrame == 0 then
		set pCFrame_LoadFrame = GetModuleProcAddress(EXTRADLLNAME, "CFrame_LoadFrame")
	endif
	if pCFrame_LoadFrame != 0 then 
		return CallStdCallWith2Args(pCFrame_LoadFrame,GetStringAddress(framename),frameid)
	endif
	return 0
endfunction

function CFrame_SetFrameType takes integer pCframe, integer frametype returns nothing
	if pCFrame_SetFrameType == 0 then
		set pCFrame_SetFrameType = GetModuleProcAddress(EXTRADLLNAME, "CFrame_SetFrameType")
	endif
	if pCFrame_SetFrameType != 0 then 
		call CallStdCallWith2Args(pCFrame_SetFrameType,pCframe,frametype)
	endif
endfunction

function CFrame_SetFrameModel takes integer pCframe, string modelpath returns nothing
	if pCFrame_SetFrameModel == 0 then
		set pCFrame_SetFrameModel = GetModuleProcAddress(EXTRADLLNAME, "CFrame_SetFrameModel")
	endif
	if pCFrame_SetFrameModel != 0 then 
		call CallStdCallWith2Args(pCFrame_SetFrameModel,pCframe,GetStringAddress(modelpath))
	endif
endfunction

function CFrame_SetFrameTexture takes integer pCframe, string texturepath, string borderpath, boolean tiled returns nothing
	if pCFrame_SetFrameTexture == 0 then
		set pCFrame_SetFrameTexture = GetModuleProcAddress(EXTRADLLNAME, "CFrame_SetFrameTexture")
	endif
	if pCFrame_SetFrameTexture != 0 then 
		call CallStdCallWith4Args(pCFrame_SetFrameTexture,pCframe,GetStringAddress(texturepath),GetStringAddress(borderpath),B2I(tiled) )
	endif
endfunction

function CFrame_SetFrameText takes integer pCframe, string text returns nothing
	if pCFrame_SetFrameText == 0 then
		set pCFrame_SetFrameText = GetModuleProcAddress(EXTRADLLNAME, "CFrame_SetFrameText")
	endif
	if pCFrame_SetFrameText != 0 then 
		call CallStdCallWith2Args(pCFrame_SetFrameText,pCframe,GetStringAddress(text))
	endif
endfunction

function CFrame_SetAbsolutePosition takes integer pCframe, integer origpos, real AbsoluteX, real AbsoluteY returns nothing
	if pCFrame_SetAbsolutePosition == 0 then
		set pCFrame_SetAbsolutePosition = GetModuleProcAddress(EXTRADLLNAME, "CFrame_SetAbsolutePosition")
	endif
	if pCFrame_SetAbsolutePosition != 0 then 
		call CallStdCallWith4Args(pCFrame_SetAbsolutePosition,pCframe,origpos,mR2I(AbsoluteX),mR2I(AbsoluteY))
	endif
endfunction

function CFrame_SetRelativePosition takes integer pCframe, integer origpos, integer relativeframeaddr, integer dstpos, real RelativeX, real RelativeY returns nothing
	if pCFrame_SetRelativePosition == 0 then
		set pCFrame_SetRelativePosition = GetModuleProcAddress(EXTRADLLNAME, "CFrame_SetRelativePosition")
	endif
	if pCFrame_SetRelativePosition != 0 then 
		call CallStdCallWith6Args(pCFrame_SetRelativePosition,pCframe,origpos,relativeframeaddr,dstpos, mR2I(RelativeX),mR2I(RelativeY))
	endif
endfunction

function CFrame_Destroy takes integer pCframe returns nothing
	if pCFrame_Destroy == 0 then
		set pCFrame_Destroy = GetModuleProcAddress(EXTRADLLNAME, "CFrame_Destroy")
	endif
	if pCFrame_Destroy != 0 then 
		call CallStdCallWith1Args(pCFrame_Destroy,pCframe)
	endif
endfunction

function CFrame_AddCallack takes integer pCframe, string callbackfuncname, integer callbackeventid returns nothing
	if pCFrame_AddCallack == 0 then
		set pCFrame_AddCallack = GetModuleProcAddress(EXTRADLLNAME, "CFrame_AddCallack")
	endif
	if pCFrame_AddCallack != 0 then 
		call CallStdCallWith4Args(pCFrame_AddCallack,pCframe,GetStringAddress(callbackfuncname),callbackeventid,0)
	endif
endfunction

function CFrame_AddCallackPacket takes integer pCframe, integer framecode, integer callbackeventid returns nothing
	if pCFrame_AddCallackPacket == 0 then
		set pCFrame_AddCallackPacket = GetModuleProcAddress(EXTRADLLNAME, "CFrame_AddCallackPacket")
	endif
	if pCFrame_AddCallackPacket != 0 then 
		call CallStdCallWith4Args(pCFrame_AddCallackPacket,pCframe,framecode,callbackeventid,0)
	endif
endfunction

function CFrame_Enable takes integer pCframe, boolean enabled returns nothing
	if pCFrame_Enable == 0 then
		set pCFrame_Enable = GetModuleProcAddress(EXTRADLLNAME, "CFrame_Enable")
	endif
	if pCFrame_Enable != 0 then 
		call CallStdCallWith2Args(pCFrame_Enable,pCframe,B2I(enabled))
	endif
endfunction


function CFrame_IsEnabled takes integer pCframe returns boolean
	if pCFrame_IsEnabled == 0 then
		set pCFrame_IsEnabled = GetModuleProcAddress(EXTRADLLNAME, "CFrame_IsEnabled")
	endif
	if pCFrame_IsEnabled != 0 then 
		return I2B(CallStdCallWith1Args(pCFrame_IsEnabled,pCframe))
	endif
	return false
endfunction


function CFrame_GetFrameAddress takes integer pCframe returns integer
	if pCFrame_GetFrameAddress == 0 then
		set pCFrame_GetFrameAddress = GetModuleProcAddress(EXTRADLLNAME, "CFrame_GetFrameAddress")
	endif
	if pCFrame_GetFrameAddress != 0 then 
		return CallStdCallWith1Args(pCFrame_GetFrameAddress,pCframe)
	endif
	return 0
endfunction

function CFrame_StartCustomAnimate takes integer frame,integer anim_id returns nothing
	if pCFrame_StartCustomAnimate == 0 then
		set pCFrame_StartCustomAnimate = GetModuleProcAddress(EXTRADLLNAME, "CFrame_StartCustomAnimate")
	endif
	if pCFrame_StartCustomAnimate != 0 then 
		call CallStdCallWith2Args(pCFrame_StartCustomAnimate,pCframe,anim_id)//  0 = anim_id ?
	endif
endfunction

function CFrame_StopCustomAnimate takes integer pCframe returns nothing
	if pCFrame_StopCustomAnimate == 0 then
		set pCFrame_StopCustomAnimate = GetModuleProcAddress(EXTRADLLNAME, "CFrame_StopCustomAnimate")
	endif
	if pCFrame_StopCustomAnimate != 0 then 
		call CallStdCallWith1Args(pCFrame_StopCustomAnimate,pCframe)
	endif
endfunction

function CFrame_SetCustomAnimateOffset takes integer pCframe, real anim_offset returns nothing
	if pCFrame_SetCustomAnimateOffset == 0 then
		set pCFrame_SetCustomAnimateOffset = GetModuleProcAddress(EXTRADLLNAME, "CFrame_SetCustomAnimateOffset")
	endif
	if pCFrame_SetCustomAnimateOffset != 0 then 
		call CallStdCallWith2Args(pCFrame_SetCustomAnimateOffset,pCframe,mR2I(anim_offset))
	endif
endfunction

function CFrame_SetScale takes integer pCframe, integer backtype, boolean filltoparentframe, real scalex,real scaley returns nothing
	if pCFrame_SetScale == 0 then
		set pCFrame_SetScale = GetModuleProcAddress(EXTRADLLNAME, "CFrame_SetScale")
	endif
	if pCFrame_SetScale != 0 then 
		call CallStdCallWith5Args(pCFrame_SetScale,pCframe,backtype, B2I(filltoparentframe),mR2I(scalex),mR2I(scaley) )
	endif
endfunction


function CFrame_Show takes integer pCframe,boolean show returns nothing
	if ( pCFrame_Show == 0 ) then
		set pCFrame_Show = GetModuleProcAddress(EXTRADLLNAME, "CFrame_Show")
	endif
	if ( pCFrame_Show != 0 ) then
		call CallStdCallWith2Args(pCFrame_Show , pCframe , B2I(show))
	endif
endfunction


function CFrameTestCallback takes nothing returns nothing
	local integer frameeventid = CFrame_GetLastEventId( )
	local integer eventframe = CFrame_GetTriggerCFrame( )
	if eventframe > 0 then 
		call CFrame_SetFrameText(eventframe,"Clicked " + I2S(testclickcount) + " times.")
		if frameeventid == FRAME_EVENT_PRESSED then
		call echo("Clicked " + I2S(testclickcount) + " times.")
		set testclickcount = testclickcount + 1 
		endif 
		call echo("Event id:" + Int2Hex(frameeventid) + ". Frame:" + Int2Hex(eventframe) + ". Flags:" +  Int2Hex( RMem( CFrame_GetFrameAddress(eventframe) + 0x1D4) ))
	endif
	
endfunction
		
function CFrameTest takes nothing returns nothing
	local integer glyphframe = 0
	call CFrame_LoadFramesListFile("DotaFrameList.txt",true)
	set glyphframe = CFrame_CreateNewFrame("GlyphButton",RMem(pGameClass2),false,0)
	call CFrame_SetAbsolutePosition(glyphframe, CFramePosition_BOTTOM_LEFT, 0.155, 0.006  )
	set glyphframe = CFrame_LoadFrame("GlyphItemButton",0)
	
	//call CFrame_AddCallack(glyphframe,"CFrameTestCallback",FRAME_EVENT_PRESSED)
	//call CFrame_AddCallack(glyphframe,"CFrameTestCallback",FRAME_MOUSE_ENTER)
	//call CFrame_AddCallack(glyphframe,"CFrameTestCallback",FRAME_MOUSE_LEAVE)
	//call CFrame_AddCallack(glyphframe,"CFrameTestCallback",FRAME_MOUSE_UP)
	//call CFrame_AddCallack(glyphframe,"CFrameTestCallback",FRAME_MOUSE_DOWN)
	//call CFrame_AddCallack(glyphframe,"CFrameTestCallback",FRAME_MOUSE_WHEEL)
	//call CFrame_AddCallack(glyphframe,"CFrameTestCallback",FRAME_CHECKBOX_CHECKED)
	//call CFrame_AddCallack(glyphframe,"CFrameTestCallback",FRAME_CHECKBOX_UNCHECKED)
	//call CFrame_AddCallack(glyphframe,"CFrameTestCallback",FRAME_EDITBOX_TEXT_CHANGED)
	
	
	call CFrame_AddCallackPacket(glyphframe,'GLPH',FRAME_EVENT_PRESSED)
	call CFrame_AddCallackPacket(glyphframe,'GLPH',FRAME_MOUSE_UP)
	call CFrame_AddCallackPacket(glyphframe,'GLPH',FRAME_MOUSE_DOWN)
	call CFrame_AddCallackPacket(glyphframe,'GLPH',FRAME_MOUSE_ENTER)
	call CFrame_AddCallackPacket(glyphframe,'GLPH',FRAME_MOUSE_LEAVE)
	
	
	call CFrame_SetFrameType(glyphframe,FRAMETYPE_BUTTON)
	call CFrame_SetFrameText(glyphframe,"Clicked 0 times")
endfunction

```
#endcode




# Packet API update! (for sync any data)

# globals 
```

		
	constant integer Packet_RawImageCode = 'IIMG'
	constant integer Packet_KeyEventCode = 'IKEY'
	constant integer Packet_CFrameEventCode = 'FRAM'

	integer pPacket_Clear = 0
	integer pPacket_Initialize = 0
	integer pPacket_PushInteger = 0
	integer pPacket_PopInteger = 0
	integer pPacket_PushReal = 0
	integer pPacket_PopReal = 0
	integer pPacket_Send = 0
	integer pPacket_GetTriggerPlayerId = 0


```
# endglobals 


# code 
```

function Packet_Clear takes nothing returns integer
	if pPacket_Clear == 0 then
		set pPacket_Clear = GetModuleProcAddress(EXTRADLLNAME, "Packet_Clear")
	endif
	if pPacket_Clear != 0 then
		return CallStdCallWith1Args(pPacket_Clear,0)
	endif
	return 0
endfunction

function Packet_Initialize takes integer pTriggerHandle returns integer
	if pPacket_Initialize == 0 then
		set pPacket_Initialize = GetModuleProcAddress(EXTRADLLNAME, "Packet_Initialize")
	endif
	if pPacket_Initialize != 0 then
		return CallStdCallWith1Args(pPacket_Initialize,pTriggerHandle)
	endif
	return 0
endfunction

function Packet_PushInteger takes integer i returns integer
	if pPacket_PushInteger == 0 then
		set pPacket_PushInteger = GetModuleProcAddress(EXTRADLLNAME, "Packet_PushInteger")
	endif
	if pPacket_PushInteger != 0 then
		return CallStdCallWith1Args(pPacket_PushInteger,i)
	endif
	return 0
endfunction

function Packet_PopInteger takes nothing returns integer
	if pPacket_PopInteger == 0 then
		set pPacket_PopInteger = GetModuleProcAddress(EXTRADLLNAME, "Packet_PopInteger")
	endif
	if pPacket_PopInteger != 0 then
		return CallStdCallWith1Args(pPacket_PopInteger,0)
	endif
	return 0
endfunction

function Packet_PushReal takes real i returns integer
	if pPacket_PushReal == 0 then
		set pPacket_PushReal = GetModuleProcAddress(EXTRADLLNAME, "Packet_PushReal")
	endif
	if pPacket_PushReal != 0 then
		return CallStdCallWith1Args(pPacket_PushReal,mR2I(i))
	endif
	return 0
endfunction

function Packet_PopReal takes nothing returns real
	local real r = 0.0
	local integer i = 0
	if pPacket_PopReal == 0 then
		set pPacket_PopReal = GetModuleProcAddress(EXTRADLLNAME, "Packet_PopReal")
	endif
	if pPacket_PopReal != 0 then
		return mI2R(CallStdCallWith1Args(pPacket_PopReal,0))
	endif
	return r
endfunction

function Packet_Send takes nothing returns integer
	if pPacket_Send == 0 then
		set pPacket_Send = GetModuleProcAddress(EXTRADLLNAME, "Packet_Send")
	endif
	if pPacket_Send != 0 then
		return CallStdCallWith1Args(pPacket_Send,0)
	endif
	return 0
endfunction

function Packet_GetTriggerPlayerId takes nothing returns integer
	if pPacket_GetTriggerPlayerId == 0 then
		set pPacket_GetTriggerPlayerId = GetModuleProcAddress(EXTRADLLNAME, "Packet_GetTriggerPlayerId")
	endif
	if pPacket_GetTriggerPlayerId != 0 then
		return CallStdCallWith1Args(pPacket_GetTriggerPlayerId,0)
	endif
	return 0
endfunction

function Packet_TestRecv takes nothing returns nothing 
	local integer PacketType = Packet_PopInteger( )
	local integer PacketValueInteger = Packet_PopInteger( )
	local real PacketValueReal = Packet_PopReal( )
	local real PacketValueReal2 = Packet_PopReal( )
	call echo("Packet type:" + Int2Hex(PacketType) + ". Packet integer value:" + I2S(PacketValueInteger) + ". Packet real value:" + R2S(PacketValueReal) + ". Packet real value2:" + R2S(PacketValueReal2) + ". Trigger Player:" + Int2Hex(Packet_GetTriggerPlayerId( )) )
endfunction

function Packet_TestRecv2 takes nothing returns nothing 
	local integer PacketType = Packet_PopInteger( )
	local integer p = Packet_GetTriggerPlayerId( )
	if (PacketType == Packet_KeyEventCode) then 
		call echo("Packet_KeyEventCode: Player:" + I2S(p) + ". Player(duplicate):" + I2S(Packet_PopInteger( )) + ". Message:" + I2S(Packet_PopInteger( ))+ ". KeyCode:" + I2S(Packet_PopInteger( )))
	endif 
	if (PacketType == Packet_CFrameEventCode) then 
		call echo("Packet_KeyEventCode: Player:" + I2S(p) + ". Player(duplicate):" + I2S(Packet_PopInteger( )) + ". Frame code:" + I2S(Packet_PopInteger( ))+ ". Event:" + I2S(Packet_PopInteger( )) + ". CTRL:" + I2S(Packet_PopInteger( )) + ". ALT:" + I2S(Packet_PopInteger( )) + ". LEFT MOUSE:" + I2S(Packet_PopInteger( )) + ". RIGHT MOUSE:" + I2S(Packet_PopInteger( )) + ". MIDLE MOUSE:" + I2S(Packet_PopInteger( )))
	endif 
	if (PacketType == Packet_RawImageCode) then 
		call echo("Packet_KeyEventCode: Player:" + I2S(p) + ". Player(duplicate):" + I2S(Packet_PopInteger( )) + ". RawImageId:" + I2S(Packet_PopInteger( ))+ ". EventType:" + I2S(Packet_PopInteger( ))+ ". mouse x/y:" + R2S(Packet_PopReal( )) + "/" + R2S(Packet_PopReal( ))+ ". Alt:" + I2S(Packet_PopInteger( ))+ ". Ctrl:" + I2S(Packet_PopInteger( ))+ ". LeftBtn:" + I2S(Packet_PopInteger( ))+ ". EventType:" + I2S(Packet_PopInteger( ))+ ". OffsetX:" + I2S(Packet_PopInteger( ))+ ". OffsetY:" + I2S(Packet_PopInteger( ))+ ". ImageID:" + I2S(Packet_PopInteger( )))
	endif 
endfunction


function Packet_TestSend takes nothing returns nothing 
	local integer maxpackets = 20
	loop 
	call echo("Packet clear:")
	call Packet_Clear( )
	call echo("Packet push packettype(just integer)")
	call Packet_PushInteger(0x12345678)
	call echo("Packet push integer")
	call Packet_PushInteger(maxpackets)
	call echo("Packet push real")
	call Packet_PushReal(1234.56)
	call echo("Packet push real")
	call Packet_PushReal(1234.56)
	call echo("Packet send ")
	call Packet_Send( )
	call echo("Packet send ok")
	exitwhen ( maxpackets <= 0 )
	set maxpackets = maxpackets - 1
	endloop
endfunction

function Packet_TestInitialize takes nothing returns nothing
	// global code
	local trigger t =CreateTrigger()
	//call TriggerAddAction(t,function Packet_TestRecv)
	call TriggerAddAction(t,function Packet_TestRecv2)
	call Packet_Initialize(GetHandleId(t))
	set t = null
	
	// local send
	//call Packet_TestSend( )
	call FuncTriggerRegisterPlayerKeyboardEvent('0')
	call CFrameTest( )
endfunction



```
# endcode 

Key codes:

```
	 0-Z = 0-Z 
	 "LBTN" =  Left mouse button
	 "RBTN" =  Right mouse button
	 "CANCEL" =  Control-break processing
	 "MBTN" =  Middle mouse button (three-button mouse)
	 "XBTN1" =  X1 mouse button
	 "XBTN2" =  X2 mouse button
	 "BACK" =  BACKSPACE key
	 "TAB" =  TAB key
	 "CLEAR" =  CLEAR key
	 "RETURN" =  ENTER key
	 "SHIFT" =  SHIFT key
	 "CTRL" =  CTRL key
	 "ALT" =  ALT key
	 "PAUSE" =  PAUSE key
	 "CAPS" =  CAPS LOCK key
	 "KANA" =  IME Kana mode
	 "VK_HANGUEL" =  IME Hangul mode
	 "JUNJA" =  IME Junja mode
	 "FINAL" =  IME final mode
	 "HANJA" =  IME Hanja mode
	 "VK_KANJI" =  IME Kanji mode
	 "ESC" =  ESC key
	 "CONV" =  IME convert
	 "NCONV" =  IME nonconvert
	 "ACCEPT" =  IME accept
	 "MCHANGE" =  IME mode change request
	 "SPACE" =  SPACEBAR
	 "PAGEUP" =  PAGE UP key
	 "PAGEDN" =  PAGE DOWN key
	 "END" =  END key
	 "HOME" =  HOME key
	 "LEFT" =  LEFT ARROW key
	 "UP" =  UP ARROW key
	 "RIGHT" =  RIGHT ARROW key
	 "DOWN" =  DOWN ARROW key
	 "SELECT" =  SELECT key
	 "PRINT" =  PRINT key
	 "EXEC" =  EXECUTE key
	 "SSHOT" =  PRINT SCREEN key
	 "INSERT" =  INS key
	 "DELETE" =  DEL key
	 "HELP" =  HELP key
	 "LWIN" =  Left Windows key (Natural keyboard)
	 "RWIN" =  Right Windows key (Natural keyboard)
	 "APPS" =  Applications key (Natural keyboard)
	 "SLEEP" =  Computer Sleep key
	 "NPAD0" =  Numeric keypad 0 key
	 "NPAD1" =  Numeric keypad 1 key
	 "NPAD2" =  Numeric keypad 2 key
	 "NPAD3" =  Numeric keypad 3 key
	 "NPAD4" =  Numeric keypad 4 key
	 "NPAD5" =  Numeric keypad 5 key
	 "NPAD6" =  Numeric keypad 6 key
	 "NPAD7" =  Numeric keypad 7 key
	 "NPAD8" =  Numeric keypad 8 key
	 "NPAD9" =  Numeric keypad 9 key
	 "MULT" =  Multiply key
	 "ADD" =  Add key
	 "SEP" =  Separator key
	 "SUB" =  Subtract key
	 "DEC" =  Decimal key
	 "DIV" =  Divide key
	 "F1" =  F1 key
	 "F2" =  F2 key
	 "F3" =  F3 key
	 "F4" =  F4 key
	 "F5" =  F5 key
	 "F6" =  F6 key
	 "F7" =  F7 key
	 "F8" =  F8 key
	 "F9" =  F9 key
	 "F10" =  F10 key
	 "F11" =  F11 key
	 "F12" =  F12 key
	 "F13" =  F13 key
	 "F14" =  F14 key
	 "F15" =  F15 key
	 "F16" =  F16 key
	 "F17" =  F17 key
	 "F18" =  F18 key
	 "F19" =  F19 key
	 "F20" =  F20 key
	 "F21" =  F21 key
	 "F22" =  F22 key
	 "F23" =  F23 key
	 "F24" =  F24 key
	 "NLOCK" =  NUM LOCK key
	 "SCRL" =  SCROLL LOCK key
	 "LSHFT" =  Left SHIFT key
	 "RSHFT" =  Right SHIFT key
	 "LCTRL" =  Left CONTROL key
	 "RCTRL" =  Right CONTROL key
	 "LALT" =  Left MENU key
	 "RALT" =  Right MENU key
	 "BBACK" =  Browser Back key
	 "BFORW" =  Browser Forward key
	 "BREFR" =  Browser Refresh key
	 "BSTOP" =  Browser Stop key
	 "BSEARCH" =  Browser Search key
	 "BFAV" =  Browser Favorites key
	 "BHOME" =  Browser Start and Home key
	 "MUTE" =  Volume Mute key
	 "V_DOWN" =  Volume Down key
	 "V_UP" =  Volume Up key
	 "NEXT" =  Next Track key
	 "PREV" =  Previous Track key
	 "STOP" =  Stop Media key
	 "MPLAY" =  Play/Pause Media key
	 "MAIL" =  Start Mail key
	 "MSEL" =  Select Media key
	 "APP1" =  Start Application 1 key
	 "APP2" =  Start Application 2 key
	 "OEM_1" =  Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ';:' key
	 "OEM_P" =  For any country/region, the '+' key
	 "COMMA" =  For any country/region, the ',' key
	 "MINUS" =  For any country/region, the '-' key
	 "PERIOD" =  For any country/region, the '.' key
	 "OEM_2" =  Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '/?' key
	 "OEM_3" =  Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '`~' key
	 "ABNT_C1" =  Brazilian (ABNT) Keyboard
	 "ABNT_C2" =  Brazilian (ABNT) Keyboard
	 "OEM_4" =  Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '[{' key
	 "OEM_5" =  Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '\|' key
	 "OEM_6" =  Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ']}' key
	 "OEM_7" =  Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the 'single-quote/double-quote' key
	 "OEM_8" =  Used for miscellaneous characters; it can vary by keyboard.
	 "OEM102" =  Either the angle bracket key or the backslash key on the RT 102-key keyboard
	 "PROCKEY" =  IME PROCESS key
	 "PACKET" =  Used to pass Unicode characters as if they were keystrokes.
	 "ATTN" =  Attn key
	 "CRSEL" =  CrSel key
	 "EXSEL" =  ExSel key
	 "EREOF" =  Erase EOF key
	 "PLAY" =  Play key
	 "ZOOM" =  Zoom key
	 "NONAME" =  Reserved
	 "PA1" =  PA1 key
	 "OCLEAR" =  Clear key
```
```
// конвертировать строку в код клавиши (комбинации клавиш)
int __stdcall ConvertKeyStringToKeyCode( const char * str )
// конвертировать код клавиши в строку
const char * __stdcall ConvertKeyCodeToKeyString( unsigned int code )
```
