#include "Main.h"
#include <d3d8.h>
#include <d3dx8.h>
#pragma comment(lib,"d3dx8.lib")
#include "RawImageApi.h"

// Thanks ENAleksey(http://xgm.guru/user/ENAleksey) for help 

IDirect3DDevice8* deviceglobal = NULL;
bool DirectxHookInitialized = false;

typedef HRESULT( STDMETHODCALLTYPE * d3dreset )( IDirect3DDevice8*,
	D3DPRESENT_PARAMETERS* );

d3dreset d3dreset_org = NULL;
d3dreset d3dreset_ptr = NULL;

BOOL vsyncEnabled = FALSE;

BOOL vsyncInitialized = FALSE;

BOOL OLD_D3D_PARAMETERS_LOADED = FALSE;

D3DCAPS8 caps8;


DWORD GlobalMultiSampleType = 0xFFFF;

bool resetcalled = false;


static HRESULT STDMETHODCALLTYPE d3dreset_my( IDirect3DDevice8 *device,
	D3DPRESENT_PARAMETERS *parameters )
{
	if ( !d3dreset_ptr )
	{
		PrintText( "FATAL ERROR IN d3dreset_my" );
		return 0;
	}

	for ( auto & img : ListOfRawImages )
	{
		img.needResetTexture = TRUE;
	}

	if ( resetcalled )
		return d3dreset_ptr( device, parameters );

	resetcalled = true;
	HRESULT hr;


	if ( !OLD_D3D_PARAMETERS_LOADED )
	{
		OLD_D3D_PARAMETERS_LOADED = TRUE;
	}



	parameters->SwapEffect = D3DSWAPEFFECT_DISCARD;

	if ( GlobalMultiSampleType > 1000 )
	{
			parameters->MultiSampleType = ( D3DMULTISAMPLE_TYPE )( 0 );
	}
	else
	{
		if ( GlobalMultiSampleType != 0xFFFF && GlobalMultiSampleType < caps8.MaxAnisotropy )
			parameters->MultiSampleType = ( D3DMULTISAMPLE_TYPE )( GlobalMultiSampleType );
	}

	if ( vsyncEnabled )
	{
		parameters->FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_ONE;
		if ( parameters->FullScreen_RefreshRateInHz == D3DPRESENT_RATE_UNLIMITED )
			parameters->FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	}
	else
	{
		parameters->FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		parameters->FullScreen_RefreshRateInHz = D3DPRESENT_RATE_UNLIMITED;
	}


	hr = d3dreset_ptr( device, parameters );


	if ( SetInfoObjDebugVal )
	{
		if ( vsyncEnabled )
			PrintText( "DEVICE RESTARTED. VSYNC STATE: ENABLED" );
		else
			PrintText( "DEVICE RESTARTED. VSYNC STATE: DISABLED" );
	}

	resetcalled = false;
	return hr;
}


typedef int( __thiscall * Wc3SetTextureStageState )( int a1, DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value );
Wc3SetTextureStageState Wc3SetTextureStageState_org;
Wc3SetTextureStageState Wc3SetTextureStageState_ptr = NULL;


DWORD MipMapCount = 0;

int __fastcall Wc3SetTextureStageState_my( int a1, int a2, DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value )
{
	if ( Type == D3DTEXTURESTAGESTATETYPE::D3DTSS_TEXTURETRANSFORMFLAGS || MipMapCount >= 1000 )
	{
		if ( MipMapCount >= 1000 )
			MipMapCount = MipMapCount - 1000;
		Wc3SetTextureStageState_ptr( a1, Stage, D3DTSS_MAXMIPLEVEL, MipMapCount );
	}

	if ( GlobalMultiSampleType > 1000 )
	{
		if ( Type == D3DTEXTURESTAGESTATETYPE::D3DTSS_TEXTURETRANSFORMFLAGS )
		{
			Wc3SetTextureStageState_ptr( a1, Stage, D3DTSS_MAXANISOTROPY, 0 );
		}


		if ( GlobalMultiSampleType - 1000 == 1 )
		{
			if ( Type == D3DTEXTURESTAGESTATETYPE::D3DTSS_MINFILTER )
			{
				return Wc3SetTextureStageState_ptr( a1, Stage, Type, D3DTEXF_FLATCUBIC );
			}
			if ( Type == D3DTEXTURESTAGESTATETYPE::D3DTSS_MAGFILTER )
			{
				return Wc3SetTextureStageState_ptr( a1, Stage, Type, D3DTEXF_FLATCUBIC );
			}
		}

		if ( GlobalMultiSampleType - 1000 == 2 )
		{
			if ( Type == D3DTEXTURESTAGESTATETYPE::D3DTSS_MINFILTER )
			{
				return Wc3SetTextureStageState_ptr( a1, Stage, Type, D3DTEXF_NONE );
			}
			if ( Type == D3DTEXTURESTAGESTATETYPE::D3DTSS_MAGFILTER )
			{
				return Wc3SetTextureStageState_ptr( a1, Stage, Type, D3DTEXF_NONE );
			}
		}

		if ( GlobalMultiSampleType - 1000 == 3 )
		{
			if ( Type == D3DTEXTURESTAGESTATETYPE::D3DTSS_MINFILTER )
			{
				return Wc3SetTextureStageState_ptr( a1, Stage, Type, D3DTEXF_GAUSSIANCUBIC );
			}
			if ( Type == D3DTEXTURESTAGESTATETYPE::D3DTSS_MAGFILTER )
			{
				return Wc3SetTextureStageState_ptr( a1, Stage, Type, D3DTEXF_GAUSSIANCUBIC );
			}
		}

		if ( GlobalMultiSampleType - 1000 == 4 )
		{
			if ( Type == D3DTEXTURESTAGESTATETYPE::D3DTSS_MINFILTER )
			{
				return Wc3SetTextureStageState_ptr( a1, Stage, Type, D3DTEXF_LINEAR );
			}
			if ( Type == D3DTEXTURESTAGESTATETYPE::D3DTSS_MAGFILTER )
			{
				return Wc3SetTextureStageState_ptr( a1, Stage, Type, D3DTEXF_LINEAR );
			}
		}

		if ( GlobalMultiSampleType - 1000 == 5 )
		{
			if ( Type == D3DTEXTURESTAGESTATETYPE::D3DTSS_MINFILTER )
			{
				return Wc3SetTextureStageState_ptr( a1, Stage, Type, D3DTEXF_FORCE_DWORD );
			}
			if ( Type == D3DTEXTURESTAGESTATETYPE::D3DTSS_MAGFILTER )
			{
				return Wc3SetTextureStageState_ptr( a1, Stage, Type, D3DTEXF_FORCE_DWORD );
			}
		}

		return Wc3SetTextureStageState_ptr( a1, Stage, Type, Value );
	}




	if ( GlobalMultiSampleType == 0 )
	{
		if ( Type == D3DTEXTURESTAGESTATETYPE::D3DTSS_TEXTURETRANSFORMFLAGS )
		{
			Wc3SetTextureStageState_ptr( a1, Stage, D3DTSS_MAXANISOTROPY, 0 );
		}

		if ( Type == D3DTEXTURESTAGESTATETYPE::D3DTSS_MINFILTER )
		{
			return Wc3SetTextureStageState_ptr( a1, Stage, Type, D3DTEXF_POINT );
		}

		if ( Type == D3DTEXTURESTAGESTATETYPE::D3DTSS_MAGFILTER )
		{
			return Wc3SetTextureStageState_ptr( a1, Stage, Type, D3DTEXF_POINT );
		}
	}
	else
	{
		if ( Type == D3DTEXTURESTAGESTATETYPE::D3DTSS_TEXTURETRANSFORMFLAGS )
		{
			Wc3SetTextureStageState_ptr( a1, Stage, D3DTSS_MAXANISOTROPY, GlobalMultiSampleType );
		}

		if ( Type == D3DTEXTURESTAGESTATETYPE::D3DTSS_MINFILTER )
		{
			return Wc3SetTextureStageState_ptr( a1, Stage, Type, D3DTEXF_ANISOTROPIC );
		}

		if ( Type == D3DTEXTURESTAGESTATETYPE::D3DTSS_MAGFILTER )
		{
			return Wc3SetTextureStageState_ptr( a1, Stage, Type, D3DTEXF_ANISOTROPIC );
		}
	}

	return Wc3SetTextureStageState_ptr( a1, Stage, Type, Value );
}

void __stdcall EnableAntialiasing( DWORD value )
{
	if ( Wc3SetTextureStageState_ptr == NULL )
	{
		if ( SetInfoObjDebugVal )
		{
			PrintText( "HOOK TEXPARAMS" );
		}
		Wc3SetTextureStageState_org = ( Wc3SetTextureStageState )( GameDll + 0x52E170 );
		MH_CreateHook( Wc3SetTextureStageState_org, &Wc3SetTextureStageState_my, reinterpret_cast< void** >( &Wc3SetTextureStageState_ptr ) );
		MH_EnableHook( Wc3SetTextureStageState_org );
	}


	if ( value > 1000 )
	{
		GlobalMultiSampleType = value;
	}
	else
	{

		value = value * 2;
		value = value / 2;

		if ( value == 1 )
			value = 2;

		if ( value > caps8.MaxAnisotropy )
			value = caps8.MaxAnisotropy;

		GlobalMultiSampleType = value;
	}


	if ( SetInfoObjDebugVal )
	{
		PrintText( "Set antialiasing to " + std::to_string( value ) );
	}

	ShowWindow( Warcraft3Window, SW_MINIMIZE );
	ShowWindow( Warcraft3Window, SW_RESTORE );
}

void __stdcall SetMipMaps( DWORD value )
{
	if ( Wc3SetTextureStageState_ptr == NULL )
	{
		if ( SetInfoObjDebugVal )
		{
			PrintText( "HOOK TEXPARAMS" );
		}
		Wc3SetTextureStageState_org = ( Wc3SetTextureStageState )( GameDll + 0x52E170 );
		MH_CreateHook( Wc3SetTextureStageState_org, &Wc3SetTextureStageState_my, reinterpret_cast< void** >( &Wc3SetTextureStageState_ptr ) );
		MH_EnableHook( Wc3SetTextureStageState_org );
	}

	MipMapCount = value;


	if ( SetInfoObjDebugVal )
	{
		PrintText( "Set mipmaps to " + std::to_string( value ) );
	}

	ShowWindow( Warcraft3Window, SW_MINIMIZE );
	ShowWindow( Warcraft3Window, SW_RESTORE );
}

void InitD3DVSync( BOOL enabled )
{
	vsyncEnabled = enabled;

	if ( SetInfoObjDebugVal )
	{
		PrintText( "Wait for restart device...." );
	}

	//if (/*OLD_D3D_PARAMETERS_LOADED &&*/ d3dreset_org && d3dreset_ptr)
	//{
	ShowWindow( Warcraft3Window, SW_MINIMIZE );
	ShowWindow( Warcraft3Window, SW_RESTORE );
	//}
}

/*************************************************************************
* D3DXMatrixTransformation2D
*/
D3DXMATRIX* WINAPI D3DXMatrixTransformation2D(
	D3DXMATRIX *pout, CONST D3DXVECTOR2 *pscalingcenter,
	FLOAT scalingrotation, CONST D3DXVECTOR2 *pscaling,
	CONST D3DXVECTOR2 *protationcenter, FLOAT rotation,
	CONST D3DXVECTOR2 *ptranslation )
{
	D3DXQUATERNION rot, sca_rot;
	D3DXVECTOR3 rot_center, sca, sca_center, trans;

	if ( pscalingcenter )
	{
		sca_center.x = pscalingcenter->x;
		sca_center.y = pscalingcenter->y;
		sca_center.z = 0.0f;
	}
	else
	{
		sca_center.x = 0.0f;
		sca_center.y = 0.0f;
		sca_center.z = 0.0f;
	}

	if ( pscaling )
	{
		sca.x = pscaling->x;
		sca.y = pscaling->y;
		sca.z = 0.0f;
	}
	else
	{
		sca.x = 0.0f;
		sca.y = 0.0f;
		sca.z = 0.0f;
	}

	if ( protationcenter )
	{
		rot_center.x = protationcenter->x;
		rot_center.y = protationcenter->y;
		rot_center.z = 0.0f;
	}
	else
	{
		rot_center.x = 0.0f;
		rot_center.y = 0.0f;
		rot_center.z = 0.0f;
	}

	if ( ptranslation )
	{
		trans.x = ptranslation->x;
		trans.y = ptranslation->y;
		trans.z = 0.0f;
	}
	else
	{
		trans.x = 0.0f;
		trans.y = 0.0f;
		trans.z = 0.0f;
	}

	rot.w = cos( rotation / 2.0f );
	rot.x = 0.0f;
	rot.y = 0.0f;
	rot.z = sin( rotation / 2.0f );

	sca_rot.w = cos( scalingrotation / 2.0f );
	sca_rot.x = 0.0f;
	sca_rot.y = 0.0f;
	sca_rot.z = sin( scalingrotation / 2.0f );

	D3DXMatrixTransformation( pout, &sca_center, &sca_rot, &sca, &rot_center, &rot, &trans );

	return pout;
}

void DrawImage( ID3DXSprite* pSprite, IDirect3DTexture8* texture, float width, float height, float x, float y )
{
	D3DXMATRIX matAll;
	float scalex = *GetWindowXoffset / DefaultSceenWidth;
	float scaley = *GetWindowYoffset / DefaultSceenHeight;

	//scalex *= DesktopScreen_Width / DefaultSceenWidth;
	//scaley *= DesktopScreen_Height / DefaultSceenHeight;

	float posx = x;
	float posy = y + scaley * height;

	if ( x <= 0.0f )
		x = 0.0f;

	if ( x >= *GetWindowYoffset )
		x = *GetWindowYoffset;

	D3DXVECTOR2 position = D3DXVECTOR2( posx, posy );
	D3DXVECTOR2 scaling( scalex, -scaley );
	D3DXVECTOR2 spriteCentre = D3DXVECTOR2( width *scalex / 2, height * scaley / 2 );
	D3DXMatrixTransformation2D( &matAll, NULL, 0.0f, &scaling, &spriteCentre, 0.0f, &position );
	pSprite->DrawTransform( texture, NULL, &matAll, 0xffffffff );
}

D3DMATERIAL8 oldmat;
void SetNewLightDx8( int id )
{
	if ( deviceglobal != NULL )
	{
		D3DMATERIAL8 mymat;

		mymat.Ambient.r = 1.0;
		mymat.Ambient.g = 1.0;
		mymat.Ambient.b = 1.0;
		mymat.Diffuse.r = 1.0;
		mymat.Diffuse.g = 1.0;
		mymat.Diffuse.b = 1.0;

		mymat.Specular.r = 1.0;
		mymat.Specular.g = 1.0;
		mymat.Specular.b = 1.0;

		deviceglobal->SetMaterial( &mymat );

	}
}

void SetOldLightDx8( int id )
{
	if ( deviceglobal != NULL )
	{

	}
}


typedef HRESULT( __stdcall *  DrawIndexedPrimitiveDx8p )( D3DPRIMITIVETYPE PrimitiveType, UINT MinIndex, UINT NumVertices, UINT StartIndex, UINT PrimitiveCount );
DrawIndexedPrimitiveDx8p DrawIndexedPrimitiveDx8_org;


HRESULT DrawIndexedPrimitiveDx8( D3DPRIMITIVETYPE PrimitiveType, UINT MinIndex, UINT NumVertices, UINT StartIndex, UINT PrimitiveCount )
{
	HRESULT retval;

	retval = DrawIndexedPrimitiveDx8_org( PrimitiveType, MinIndex, NumVertices, StartIndex, PrimitiveCount );

	return retval;
}


void DrawOverlayDx8( )
{
	if (!DirectxHookInitialized)
		return;
	HRESULT rv;
	IDirect3DDevice8 * d = deviceglobal;
	if ( !d || OverlayDrawed )
	{
		return;
	}
	ID3DXSprite* pSprite = NULL;
	rv = D3DXCreateSprite( d, &pSprite );
	if ( !pSprite || rv != S_OK )
		return;


	pSprite->Begin( );

	d->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	d->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	d->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	for ( auto & img : ListOfRawImages )
	{
		if ( !img.used_for_overlay )
		{
			if ( img.textureaddr )
			{
				IDirect3DTexture8 * ppTexture = ( IDirect3DTexture8 * )img.textureaddr;
				ppTexture->Release( );
				ppTexture = NULL;
				img.needResetTexture = FALSE;
				img.textureaddr = NULL;
			}

			continue;
		}
		if ( img.needResetTexture )
		{
			img.needResetTexture = FALSE;
			if ( img.textureaddr )
			{
				IDirect3DTexture8 * ppTexture = ( IDirect3DTexture8 * )img.textureaddr;
				ppTexture->Release( );
				ppTexture = NULL;
				img.textureaddr = NULL;
			}
		}
		IDirect3DTexture8 * ppTexture = NULL;
		if ( img.textureaddr )
		{
			ppTexture = ( IDirect3DTexture8 * )img.textureaddr;
		}
		else
		{
			rv = d->CreateTexture( ( UINT )img.width, ( UINT )img.height, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &ppTexture );
			if ( ppTexture && rv == D3D_OK )
			{
				D3DLOCKED_RECT rect;

				rv = ppTexture->LockRect( 0, &rect, 0, 0 );
				if ( rv == D3D_OK )
				{
					unsigned char* dest = static_cast< unsigned char* >( rect.pBits );
#ifdef OLD_CODE
					std::memcpy( dest, img.img.buf, ( size_t )( img.width * img.height * 4 ) );

#endif
					ppTexture->UnlockRect( 0 );

					if ( D3DXFilterTexture( ppTexture, NULL, D3DX_DEFAULT, D3DX_DEFAULT ) != D3D_OK )
					{
						if ( SetInfoObjDebugVal )
						{
							PrintText( "D3DXFilterTexture error" );
						}
					}

					img.textureaddr = ppTexture;
					img.drawdevice = 0;
				}
			}

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
		if ( ppTexture && rv == S_OK )
		{
			DrawImage( pSprite, ppTexture, ( float )img.width, ( float )img.height, *GetWindowXoffset * img.overlay_x, *GetWindowYoffset * img.overlay_y );
		}
	}
	pSprite->End( );
	pSprite->Release( );
}




typedef HRESULT( __fastcall * EndScene_p )( int GlobalWc3Data );
EndScene_p EndScene_org;
EndScene_p EndScene_ptr;

HRESULT __fastcall EndScene_my( int GlobalWc3Data )
{
	IDirect3DDevice8 * d = *( IDirect3DDevice8** )( GlobalWc3Data + 1412 );
	if ( d != NULL && deviceglobal == NULL && DirectxHookInitialized)
	{
		deviceglobal = d;
		d->GetDeviceCaps( &caps8 );

		if ( deviceglobal && !vsyncInitialized )
		{
			d3dreset_org = ( d3dreset )( ( *( uintptr_t** )deviceglobal )[ 14 ] );
			if ( d3dreset_org )
			{
				char helpfatal[ 256 ];
				auto val1 = MH_CreateHook( d3dreset_org, &d3dreset_my, reinterpret_cast< void** >( &d3dreset_ptr ) );
				auto val2 = MH_EnableHook( d3dreset_org );
				sprintf_s( helpfatal, 256, "%s = %s", MH_StatusToString( val1 ), MH_StatusToString( val2 ) );
				if ( SetInfoObjDebugVal )
				{
					PrintText( "D3D RESET SUCCESS HOOKED" );
				}
				vsyncInitialized = TRUE;
			}
		}
	}
	OverlayDrawed = FALSE;
	HRESULT retval = deviceglobal->EndScene( );
	return retval;
}


void Uninitd3d8Hook( BOOL cleartextures )
{
	if (!DirectxHookInitialized)
		return;
	DirectxHookInitialized = false;
	if ( EndScene_org )
	{
		if ( cleartextures )
		{
			for ( auto & img : ListOfRawImages )
			{
				if ( img.textureaddr && img.drawdevice == 0 )
				{
					IDirect3DTexture8 * ppTexture = ( IDirect3DTexture8 * )img.textureaddr;

					if ( IsOkayPtr( ppTexture ) )
					{
						try
						{
							ppTexture->Release( );
						}
						catch ( ... )
						{
							if ( SetInfoObjDebugVal )
							{
								PrintText( "Error texture release" );
							}
						}
					}

					ppTexture = NULL;
					img.textureaddr = NULL;
					img.ingame = FALSE;
					img.used_for_overlay = FALSE;
				}
			}
		}
		MH_DisableHook( EndScene_org );
		EndScene_org = NULL;

		deviceglobal = NULL;

		if ( vsyncInitialized )
		{
			if ( d3dreset_org )
			{
				MH_DisableHook( d3dreset_org );
				d3dreset_org = NULL;
				d3dreset_ptr = NULL;
			}

			if ( Wc3SetTextureStageState_org )
			{
				MH_DisableHook( Wc3SetTextureStageState_org );
				Wc3SetTextureStageState_org = NULL;
				Wc3SetTextureStageState_ptr = NULL;
			}
		}
		vsyncInitialized = FALSE;

		GlobalMultiSampleType = 0xFFFF;
	}
}

void Initd3d8Hook( )
{
	if (!DirectxHookInitialized)
	{
		DirectxHookInitialized = true;
		deviceglobal = NULL;
		EndScene_org = (EndScene_p)(GameDll + 0x52F270);
		MH_STATUS s = MH_CreateHook(EndScene_org, &EndScene_my, reinterpret_cast<void**>(&EndScene_ptr));


	/*if ( MH_STATUS::MH_OK != s && s != MH_STATUS::MH_ERROR_ALREADY_CREATED )
	{
		char temp[ 256 ];
		sprintf_s( temp, "Can't install directx hook. Error %s", MH_StatusToString( s ) );
		MessageBoxA( 0, temp, "Error", 0 );
	}*/
	/*if ( MH_STATUS::MH_OK !=*/ MH_EnableHook( EndScene_org ); /*)
	{
		MessageBoxA( 0, "Can't install directx hook. Error 2", "Error", 0 );
	}*/

	}
}

