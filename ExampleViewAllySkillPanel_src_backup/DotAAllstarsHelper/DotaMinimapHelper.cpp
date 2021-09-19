#include "Main.h"



typedef void( __fastcall * DrawMinimap /*sub_6F362190*/ )( int gClass );

DrawMinimap DrawMinimap_org;
DrawMinimap DrawMinimap_ptr;


/*

ShowMinimap type:

0. No minimaps
1. Only karaulov minimap
2. Only blizzard minimap
3. Two minimaps


*/

int ShowMinimapType = 3;

void __stdcall ShowMinimap( int type )
{
	ShowMinimapType = type;
}



void __fastcall DrawMinimap_new( )
{



}


void __fastcall DrawMinimap_my( int gClass )
{
	if ( ShowMinimapType == 4 )
		return;

	if ( ShowMinimapType == 1 )
	{
		DrawMinimap_new( );
	}
	else if ( ShowMinimapType == 2 )
	{
		DrawMinimap_ptr( gClass );
	}
	else if ( ShowMinimapType == 3 )
	{
		DrawMinimap_ptr( gClass );
		DrawMinimap_new(  );
	}





}