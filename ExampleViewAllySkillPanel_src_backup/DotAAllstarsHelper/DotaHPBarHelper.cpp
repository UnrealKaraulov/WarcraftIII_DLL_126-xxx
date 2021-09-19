#include "Main.h"


unsigned int hpbarcolorsHero[ 20 ];
unsigned int hpbarcolorsUnit[ 20 ];
unsigned int hpbarcolorsTower[ 20 ];

float hpbarscaleHeroX[ 20 ];
float hpbarscaleUnitX[ 20 ];
float hpbarscaleTowerX[ 20 ];

float hpbarscaleHeroY[ 20 ];
float hpbarscaleUnitY[ 20 ];
float hpbarscaleTowerY[ 20 ];


vector<CustomHPBar> CustomHPBarList[ 20 ];

int __stdcall SetHPBarColorForPlayer( unsigned int playerflag, unsigned int herocolor,
	unsigned int unitcolor, unsigned int towercolor )
{

	int playerid = 0;
	int currentflag = 1;
	while ( playerid <= 16 )
	{
		if ( playerflag & currentflag )
		{
			hpbarcolorsHero[ playerid ] = herocolor;
			hpbarcolorsUnit[ playerid ] = unitcolor;
			hpbarcolorsTower[ playerid ] = towercolor;
		}

		currentflag = currentflag * 2;
		playerid++;
	}
	return 0;
}


int __stdcall SetHPBarXScaleForPlayer( unsigned int playerflag, float heroscale,
	float unitscale, float towerscale )
{
	int playerid = 0;
	int currentflag = 1;
	while ( playerid <= 16 )
	{
		if ( playerflag & currentflag )
		{
			hpbarscaleHeroX[ playerid ] = heroscale;
			hpbarscaleUnitX[ playerid ] = unitscale;
			hpbarscaleTowerX[ playerid ] = towerscale;
		}

		currentflag = currentflag * 2;
		playerid++;
	}
	return 0;
}

int __stdcall SetHPBarYScaleForPlayer( unsigned int playerflag, float heroscale,
	float unitscale, float towerscale )
{
	int playerid = 0;
	int currentflag = 1;
	while ( playerid <= 16 )
	{
		if ( playerflag & currentflag )
		{
			hpbarscaleHeroY[ playerid ] = heroscale;
			hpbarscaleUnitY[ playerid ] = unitscale;
			hpbarscaleTowerY[ playerid ] = towerscale;
		}

		currentflag = currentflag * 2;
		playerid++;
	}
	return 0;
}


int __stdcall SetHPCustomHPBarUnit( unsigned int playerflag, int tid, unsigned int color, float xscale, float yscale )
{
	int playerid = 0;
	int currentflag = 1;
	while ( playerid <= 16 )
	{
		if ( playerflag & currentflag )
		{
			BOOL needadd = TRUE;
			for ( unsigned int i = 0; i < CustomHPBarList[ playerid ].size( ); i++ )
			{
				if ( CustomHPBarList[ playerid ][ i ].unittypeid == tid )
				{
					needadd = FALSE;
					CustomHPBarList[ playerid ][ i ].color = color;
					CustomHPBarList[ playerid ][ i ].scalex = xscale;
					CustomHPBarList[ playerid ][ i ].scaley = yscale;

					break;
				}
			}

			if ( needadd )
			{
				CustomHPBar tmpcbar;
				tmpcbar.unittypeid = tid;
				tmpcbar.color = color;
				tmpcbar.scalex = xscale;
				tmpcbar.scaley = yscale;
				CustomHPBarList[ playerid ].push_back( tmpcbar );
			}
		}

		currentflag = currentflag * 2;
		playerid++;
	}
	return 0;
}


int __stdcall SetColorForUnit( unsigned int * coloraddr, BarStruct * BarStruct )
{
	int retval = 0;
	__asm mov retval, eax;

	if ( !BarStruct )
	{
		return retval;
	}
	if ( BarStruct->_BarClass != _BarVTable && BarStruct->_BarClass != ( int )BarVtableClone )
	{
		return retval;
	}
	else
	{
	}


	int unitaddr = BarStruct->unitaddr;
	if ( !unitaddr || !IsNotBadUnit( unitaddr ) )
	{
		return retval;
	}

	int unitslot = GetUnitOwnerSlot( unitaddr );

	if ( unitslot > 15 || unitslot < 0 )
	{
		return retval;
	}


	for ( unsigned int i = 0; i < CustomHPBarList[ unitslot ].size( ); i++ )
	{
		if ( IsClassEqual( GetTypeId( unitaddr ), CustomHPBarList[ unitslot ][ i ].unittypeid ) )
		{
			if ( BarStruct->bartype == 1 )
			{
				if ( CustomHPBarList[ unitslot ][ i ].scalex != 0.0 )
				{
					BarStruct->ScaleX = CustomHPBarList[ unitslot ][ i ].scalex;
				}

				if ( CustomHPBarList[ unitslot ][ i ].scaley != 0.0 )
				{
					BarStruct->ScaleY = CustomHPBarList[ unitslot ][ i ].scaley;
				}

				if ( !coloraddr  )
				{
					return retval;
				}
				if ( CustomHPBarList[ unitslot ][ i ].color != 0 )
				{
					*coloraddr = CustomHPBarList[ unitslot ][ i ].color;
				}
				return retval;
			}
		}
	}

	if ( IsHero( unitaddr ) )
	{
		if ( BarStruct->bartype == 1 )
		{
			if ( hpbarscaleHeroX[ unitslot ] != 0.0 )
			{
				BarStruct->ScaleX = hpbarscaleHeroX[ unitslot ];
			}

			if ( hpbarscaleHeroY[ unitslot ] != 0.0 )
			{
				BarStruct->ScaleY = hpbarscaleHeroY[ unitslot ];
			}
		}
		if ( !coloraddr  )
		{
			return retval;
		}
		if ( hpbarcolorsHero[ unitslot ] != 0 )
		{
			*coloraddr = hpbarcolorsHero[ unitslot ];
		}

	}
	else if ( IsTower( unitaddr ) )
	{
		if ( BarStruct->bartype == 1 )
		{
			if ( hpbarscaleTowerX[ unitslot ] != 0.0 )
			{
				BarStruct->ScaleX = hpbarscaleTowerX[ unitslot ];
			}

			if ( hpbarscaleTowerY[ unitslot ] != 0.0 )
			{
				BarStruct->ScaleY = hpbarscaleTowerY[ unitslot ];
			}
		}
		if ( !coloraddr  )
		{
			return retval;
		}
		if ( hpbarcolorsTower[ unitslot ] != 0 )
		{
			*coloraddr = hpbarcolorsTower[ unitslot ];
		}
	}
	else
	{
		if ( BarStruct->bartype == 1 )
		{
			if ( hpbarscaleUnitX[ unitslot ] != 0.0 )
			{
				BarStruct->ScaleX = hpbarscaleUnitX[ unitslot ];
			}

			if ( hpbarscaleUnitY[ unitslot ] != 0.0 )
			{
				BarStruct->ScaleY = hpbarscaleUnitY[ unitslot ];
			}
		}
		if ( !coloraddr )
		{
			return retval;
		}

		if ( hpbarcolorsUnit[ unitslot ] != 0 )
		{
			*coloraddr = hpbarcolorsUnit[ unitslot ];
		}
	}

	return retval;
}

