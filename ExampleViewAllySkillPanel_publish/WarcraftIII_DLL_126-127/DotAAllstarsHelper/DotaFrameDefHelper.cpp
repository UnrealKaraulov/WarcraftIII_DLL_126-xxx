
#include "Main.h"
#include "WarcraftFrameHelper.h"

using namespace NWar3Frame;

struct CFrameBuffer
{
	CWar3Frame * frame;
	string callbackfuncname;
	bool packetcallback;
	unsigned int callbackeventid;
	int FrameCode;
};

vector<CFrameBuffer> CFrameBufferList;


void DotaHelperEventEndCallback( )
{
	if ( IsGameFrameActive( ) && pCurrentFrameFocusedAddr && *( int* )pCurrentFrameFocusedAddr )
		*( int* )pCurrentFrameFocusedAddr = 0;
}



jRCString NewCallBackFuncName = jRCString( );

int NewCallBackTriggerHandle = 0;



int LastEventId = 0;



int __stdcall CFrame_GetLastEventId( int )
{
	return LastEventId;
}


CWar3Frame * latestcframe = NULL;

CWar3Frame *  __stdcall CFrame_GetTriggerCFrame( int )
{
	return latestcframe;
}

int DotaHelperFrameCallback( CWar3Frame*frame, int FrameAddr, unsigned int EventId )
{
	LastEventId = EventId;
	latestcframe = frame;
	//MessageBoxA( 0, frame->FrameName.c_str( ), frame->FrameName.c_str( ), 0 );
	for ( auto s : CFrameBufferList )
	{
		if ( s.frame == frame && s.callbackeventid == EventId )
		{
			if ( s.callbackfuncname.length( ) > 0 )
			{
				str2jstr( &NewCallBackFuncName, s.callbackfuncname.c_str( ) );
				ExecuteFunc( &NewCallBackFuncName );
				//MessageBoxA( 0, CWar3Frame::DumpAllFrames( ).c_str( ), " Frames ", 0 );
			}
			else if ( s.packetcallback  )
			{
				std::vector<unsigned char> SendKeyEvent;
				SendKeyEvent.push_back( 0x50 );
				// header custom packets
				SendKeyEvent.push_back( 0xFF );
				// size custom packets 
				SendKeyEvent.push_back( 0 );
				SendKeyEvent.push_back( 0 );
				SendKeyEvent.push_back( 0 );
				SendKeyEvent.push_back( 0 );
				// packet type
				int packettype = 'FRAM';
				SendKeyEvent.insert( SendKeyEvent.end( ), ( unsigned char * )&packettype, ( ( unsigned char * )&packettype ) + 4 );
				*( int* )&SendKeyEvent[ 2 ] += 4;
				// data
				int locpid = GetLocalPlayerId( );
				SendKeyEvent.insert( SendKeyEvent.end( ), ( unsigned char * )&locpid, ( ( unsigned char * )&locpid ) + 4 );
				*( int* )&SendKeyEvent[ 2 ] += 4;
				SendKeyEvent.insert( SendKeyEvent.end( ), ( unsigned char * )&s.FrameCode, ( ( unsigned char * )&s.FrameCode ) + 4 );
				*( int* )&SendKeyEvent[ 2 ] += 4;
				SendKeyEvent.insert( SendKeyEvent.end( ), ( unsigned char * )&LastEventId, ( ( unsigned char * )&LastEventId ) + 4 );
				*( int* )&SendKeyEvent[ 2 ] += 4;

				int IsCtrlPressed = IsKeyPressed( VK_CONTROL );
				int IsAltPressed = IsKeyPressed( VK_MENU );
				int IsLeftMousePressed = IsKeyPressed( 1 );
				int IsRightMousePressed = IsKeyPressed( 2 );
				int IsMiddleMousePressed = IsKeyPressed( 3 );

				SendKeyEvent.insert( SendKeyEvent.end( ), ( unsigned char * )&IsCtrlPressed, ( ( unsigned char * )&IsCtrlPressed ) + 4 );
				*( int* )&SendKeyEvent[ 2 ] += 4;
				SendKeyEvent.insert( SendKeyEvent.end( ), ( unsigned char * )&IsAltPressed, ( ( unsigned char * )&IsAltPressed ) + 4 );
				*( int* )&SendKeyEvent[ 2 ] += 4;
				SendKeyEvent.insert( SendKeyEvent.end( ), ( unsigned char * )&IsLeftMousePressed, ( ( unsigned char * )&IsLeftMousePressed ) + 4 );
				*( int* )&SendKeyEvent[ 2 ] += 4;
				SendKeyEvent.insert( SendKeyEvent.end( ), ( unsigned char * )&IsRightMousePressed, ( ( unsigned char * )&IsRightMousePressed ) + 4 );
				*( int* )&SendKeyEvent[ 2 ] += 4;
				SendKeyEvent.insert( SendKeyEvent.end( ), ( unsigned char * )&IsMiddleMousePressed, ( ( unsigned char * )&IsMiddleMousePressed ) + 4 );
				*( int* )&SendKeyEvent[ 2 ] += 4;

				
				SendPacket( ( BYTE* )&SendKeyEvent[ 0 ], SendKeyEvent.size( ) );
				SendKeyEvent.clear( );
			}
			break;
		}
	}
	return 0;
}


void FrameDefHelperInitialize( )
{
	CWar3Frame::SetGlobalEventCallback( DotaHelperEventEndCallback );
	CWar3Frame::Init(GameDll );
	CWar3Frame::InitCallbackHook( );
	NewCallBackFuncName = jRCString( );
	NewCallBackTriggerHandle = 0;
}

void FrameDefHelperUninitialize( )
{
	CFrameBufferList.clear( );
	CWar3Frame::UninitializeAllFrames( TRUE );
	CWar3Frame::UninitializeCallbackHook( );
	NewCallBackFuncName = jRCString( );
	NewCallBackTriggerHandle = 0;
}

string GetCallbackFuncName( CWar3Frame * frame )
{
	if ( !frame )
		return string( );
	for ( auto s : CFrameBufferList )
	{
		if ( s.frame == frame )
		{
			return s.callbackfuncname;
		}
	}
	return string( );
}


void __stdcall CFrame_SetCustomValue( CWar3Frame*frame, int id, int value )
{
	if ( id >= 1 && id <= 10 )
		frame->SetFrameCustomValue( value, id );
	else
		frame->SetFrameCustomValue( value, 1 );
}

int __stdcall CFrame_GetCustomValue( CWar3Frame*frame, int id )
{
	if ( id >= 1 && id <= 10 )
		return frame->GetFrameCustomValue( id );
	return frame->GetFrameCustomValue( 1 );
}

int __stdcall CFrame_GetFrameAddress( CWar3Frame*frame )
{
	return frame->FrameAddr;
}

// Load custom FrameDef.toc file ( .txt file with fdf path list)
void __stdcall CFrame_LoadFramesListFile( const char * path_to_listfile, BOOL forcereload )
{
	CWar3Frame::LoadFrameDefFiles( path_to_listfile, forcereload );
}


// Create frame from fdf file.  relativeframe can be NULL for using 'current frame'
// show - show frame or create invisibled.
// frameid - load same file but with another id
CWar3Frame * __stdcall CFrame_CreateNewFrame( const char * FrameName, int relativeframeaddr, BOOL show, int FrameId )
{

	CWar3Frame::InitCallbackHook( );

	CWar3Frame * returnframe = new CWar3Frame( FrameName, FrameId, show, relativeframeaddr );

	if ( !returnframe->CheckIsOk( ) )
	{
		delete returnframe;
		return NULL;
	}
	returnframe->SetCallbackFunc( DotaHelperFrameCallback );
	return returnframe;
}

// Load exists frame (get name from any .fdf files) with FrameId (use 0 if frame not created with another id)
CWar3Frame * __stdcall CFrame_LoadFrame( const char * FrameName, int FrameId )
{
	CWar3Frame * returnframe = new CWar3Frame( );
	if ( !returnframe->Load( FrameName, FrameId ) )
	{
		delete returnframe;
		return NULL;
	}
	returnframe->SetCallbackFunc( DotaHelperFrameCallback );
	return returnframe;
}

// Set frametype for frame . Possibe in new version i can add autodetect frametype
void __stdcall CFrame_SetFrameType( CWar3Frame * frame, CFrameType FrameType )
{
	if ( !frame )
		return;
	frame->SetFrameType( FrameType );
}

// Set model for frame now support SPRITE frametype
void __stdcall CFrame_SetFrameModel( CWar3Frame * frame, const char * modelpath )
{
	if ( !frame )
		return;
	frame->SetModel( modelpath );
}
// Experimenal version of CFrame_SetFrameModel with more flags
void __stdcall CFrame_SetFrameModelEx( CWar3Frame * frame, const char * modelpath, int MdlType, int Flags )
{
	if ( !frame )
		return;
	frame->SetModel( modelpath, MdlType, Flags );
}
// Start animate with ID (ID or just flag?)
void __stdcall CFrame_StartCustomAnimate( CWar3Frame * frame, int anim_id )
{
	if ( !frame )
		return;
	frame->StartAnimate( anim_id );
}
// Stop animate frame
void __stdcall CFrame_StopCustomAnimate( CWar3Frame * frame )
{
	if ( !frame )
		return;
	frame->StopAnimate( );
}

// Set custom animation offset. For example start=0.0 end=1.0 , set 0.5 = middle of animation.
void __stdcall CFrame_SetCustomAnimateOffset( CWar3Frame * frame, float offset )
{
	if ( !frame )
		return;
	frame->anim_offset = offset;
}


// Set texture for frame, now support only SPRITE and some other frametype(USE FRAMETYPE_ITEM FOR TEST)
void __stdcall CFrame_SetFrameTexture( CWar3Frame * frame, const char * texturepath, const char * borderpath, BOOL tiled )
{
	if ( !frame )
		return;
	frame->SetTexture( texturepath, borderpath, tiled );
}

// Set text for frame, support various frame types
void __stdcall CFrame_SetFrameText( CWar3Frame * frame, const char * text )
{
	if ( !frame )
		return;
	frame->SetText( text );
}

// Set frame position (Relative main frame)
void __stdcall CFrame_SetAbsolutePosition( CWar3Frame * frame, CFramePosition originpos, float AbsoluteX, float AbsoluteY )
{
	if ( !frame )
		return;
	frame->SetFrameAbsolutePosition( originpos, AbsoluteX, AbsoluteY );
}

// Set frame relative position 
void __stdcall CFrame_SetRelativePosition( CWar3Frame * frame, CFramePosition originpos, int relframeaddr, CFramePosition dstpos, float RelativeX, float RelativeY )
{
	if ( !frame )
		return;
	frame->SetFrameRelativePosition( originpos, relframeaddr, dstpos, RelativeX, RelativeY );
}


// Remove frame from memory and game callback
void __stdcall CFrame_Destroy( CWar3Frame * frame )
{
	if ( !frame )
		return;
	frame->DestroyThisFrame( );

	for ( unsigned int i = 0; i < CFrameBufferList.size( ); i++ )
	{
		if ( CFrameBufferList[ i ].frame == frame )
		{
			CFrameBufferList.erase( CFrameBufferList.begin( ) + i );
			if ( i > 0 )
				i--;
			else
				break;
		}
	}

	delete frame;
}

// Add callback func for selected frame with selected callbackeventid
// skipothercallback can be TRUE or FALSE (possibly useless)
void __stdcall CFrame_AddCallack( CWar3Frame * frame, const char * callbackfuncname, unsigned int callbackeventid, BOOL skipothercallback )
{
	if ( !frame )
		return;
	CFrameBuffer tmpFrameBuf = CFrameBuffer( );
	tmpFrameBuf.frame = frame;
	tmpFrameBuf.callbackfuncname = callbackfuncname;
	tmpFrameBuf.packetcallback = false;
	tmpFrameBuf.callbackeventid = callbackeventid;
	tmpFrameBuf.FrameCode = 0;
	CFrameBufferList.push_back( tmpFrameBuf );
	frame->RegisterEventCallback( callbackeventid );
	//frame->SkipOtherEvents = skipothercallback;
}

void __stdcall CFrame_AddCallackPacket( CWar3Frame * frame, int FrameCode,  unsigned int callbackeventid, BOOL skipothercallback )
{
	if ( !frame )
		return;
	CFrameBuffer tmpFrameBuf = CFrameBuffer( );
	tmpFrameBuf.frame = frame;
	tmpFrameBuf.callbackfuncname = "";
	tmpFrameBuf.packetcallback = true;
	tmpFrameBuf.callbackeventid = callbackeventid;
	tmpFrameBuf.FrameCode = FrameCode;
	CFrameBufferList.push_back( tmpFrameBuf );
	frame->RegisterEventCallback( callbackeventid );
	//frame->SkipOtherEvents = skipothercallback;
}


void __stdcall CFrame_Enable( CWar3Frame * frame, BOOL enabled )
{
	if ( !frame )
		return;
	frame->Enable( enabled );
}

BOOL __stdcall CFrame_IsEnabled( CWar3Frame * frame )
{
	if ( !frame )
		return FALSE;
	return frame->IsEnabled( );
}

void __stdcall CFrame_SetScale( CWar3Frame * frame, CFrameBackdropType backtype, BOOL FillToFrame, float scalex, float scaley )
{
	if ( !frame )
		return;
	//frame->FillToParentFrame( backtype, FillToFrame );
	frame->SetFrameScale( backtype, scalex, scaley );
}

void __stdcall CFrame_SetTextWidth( CWar3Frame * frame, float scalex )
{
	if ( !frame )
		return;
	frame->SetTextFrameFontWidth(scalex );
}

void __stdcall CFrame_SetTextHeight( CWar3Frame * frame, float scaley )
{
	if ( !frame )
		return;
	frame->SetTextFrameFontHeight( scaley );
}


void __stdcall CFrame_SetTextFont( CWar3Frame * frame, const char * font, float scale, int flag )
{
	if ( !frame )
		return;
	SetTextFrameFont( frame->FrameAddr, font, scale, flag );
}

void __stdcall CFrame_Update( CWar3Frame * frame )
{
	if ( !frame )
		return;
	frame->UpdateFlagsV2( );
}

void __stdcall CFrame_Show( CWar3Frame * frame, BOOL show )
{
	if ( !frame )
		return;
	frame->Show( show );
}

