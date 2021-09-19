#include "Main.h"
#include "ViewAllySkill.h"


IsNeedDrawUnit2 IsNeedDrawUnit2org;
IsNeedDrawUnit2 IsNeedDrawUnit2ptr;

IsDrawSkillPanel IsDrawSkillPanel_org = NULL;
IsDrawSkillPanel IsDrawSkillPanel_ptr;
IsDrawSkillPanelOverlay IsDrawSkillPanelOverlay_org = NULL;
IsDrawSkillPanelOverlay IsDrawSkillPanelOverlay_ptr;

BOOL ShowSkillPanelForObservers = FALSE;
BOOL ShowSkillPanelOnlyForHeroes = TRUE;



int __stdcall ShowObserverSkillPanel( BOOL enabled )
{
	ShowSkillPanelForObservers = enabled;
	return ShowSkillPanelForObservers;
}

int __stdcall ShowSkillPanelForAllUnits( BOOL enabled )
{
	ShowSkillPanelOnlyForHeroes = !enabled;
	return ShowSkillPanelOnlyForHeroes;
}


signed int __fastcall  IsDrawSkillPanel_my( void *UnitAddr, int addr1 )
{
	signed int result;
	int GETOID;
	int OID;
	if ( addr1 )
	{
		GETOID = *( int * )( addr1 + 444 );
		if ( GETOID > 0 )
			OID = *( int * )( GETOID + 8 );
		else
			OID = 852290;
		result = 1;

		// Сначала вызвать оригинальную функцию
		if ( ( ( IsNeedDrawUnitOrigin )( GameDll + IsNeedDrawUnitOriginOffset ) )( UnitAddr ) )
		{
			( ( DrawSkillPanel )( GameDll + DrawSkillPanelOffset ) )( UnitAddr, OID );
		}
		else if ( IsNotBadUnit( ( int )UnitAddr ) )
		{
			// Затем дополнительную которая отрисует скилы всем союзным героям.
			if ( IsEnemy( ( int )UnitAddr ) == FALSE )
			{
				if ( IsHero( ( int )UnitAddr ) )
				{
					( ( DrawSkillPanel )( GameDll + DrawSkillPanelOffset ) )( UnitAddr, OID );
					return result;
				}
			}

			if ( ShowSkillPanelForObservers && IsLocalPlayerObserver( ) )
			{
				if ( ( !ShowSkillPanelOnlyForHeroes || IsHero( ( int )UnitAddr ) ) &&
					GetUnitOwnerSlot( ( int )UnitAddr ) <= 12 )
					( ( DrawSkillPanel )( GameDll + DrawSkillPanelOffset ) )( UnitAddr, OID );
			}
		}

	}
	else
	{
		result = 0;
	}
	return result;
}

signed int __fastcall  IsDrawSkillPanelOverlay_my( void *UnitAddr, int addr1 )
{
	signed int result; // eax@2
	int GETOID; // eax@3
	int OID; // esi@4
	if ( addr1 )
	{
		GETOID = *( int * )( addr1 + 444 );
		if ( GETOID > 0 )
			OID = *( int * )( GETOID + 8 );
		else
			OID = 852290;

		result = 1;


		// Сначала вызвать оригинальную функцию
		if ( ( ( IsNeedDrawUnitOrigin )( GameDll + IsNeedDrawUnitOriginOffset ) )( UnitAddr ) )
		{
			( ( DrawSkillPanelOverlay )( GameDll + DrawSkillPanelOverlayOffset ) )( UnitAddr, OID );
		}
		else if ( IsNotBadUnit( ( int )UnitAddr ) )
		{
			// Затем дополнительную которая отрисует скилы всем союзным героям.
			if ( IsEnemy( ( int )UnitAddr ) == FALSE )
			{
				if ( IsHero( ( int )UnitAddr ) )
				{
					( ( DrawSkillPanelOverlay )( GameDll + DrawSkillPanelOverlayOffset ) )( UnitAddr, OID );
					return result;
				}
			}
			if ( ShowSkillPanelForObservers && IsLocalPlayerObserver( ) )
			{
				if ( ( !ShowSkillPanelOnlyForHeroes || IsHero( ( int )UnitAddr )) &&
					GetUnitOwnerSlot( ( int )UnitAddr ) <= 12 )
						( ( DrawSkillPanelOverlay )( GameDll + DrawSkillPanelOverlayOffset ) )( UnitAddr, OID );
			}
		}
	
	}
	else
	{
		result = 0;
	}
	return result;
}


int __fastcall IsNeedDrawUnit2_my( int UnitAddr, int unused/* converted from thiscall to fastcall*/ )
{
	if ( IsNotBadUnit( UnitAddr ) )
	{
		if ( IsEnemy( UnitAddr ) == FALSE )
		{
			if ( IsHero( ( int )UnitAddr ) )
				return 1;
		}

		if ( ShowSkillPanelForObservers && IsLocalPlayerObserver( ) )
		{
			if ( !ShowSkillPanelOnlyForHeroes || IsHero( ( int )UnitAddr ) )
				if ( GetUnitOwnerSlot( ( int )UnitAddr ) <= 12 )
					return 1;
		}
	}
	return IsNeedDrawUnit2ptr( UnitAddr );
}