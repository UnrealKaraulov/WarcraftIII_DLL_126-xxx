#include "Main.h"
#include "ViewAllySkill.h"


IsNeedDrawUnit2 IsNeedDrawUnit2org;
IsNeedDrawUnit2 IsNeedDrawUnit2ptr;

IsDrawSkillPanel IsDrawSkillPanel_org = NULL;
IsDrawSkillPanel IsDrawSkillPanel_ptr;

IsDrawSkillPanelOverlay IsDrawSkillPanelOverlay_org = NULL;
IsDrawSkillPanelOverlay IsDrawSkillPanelOverlay_ptr;

bool ShowSkillPanelForObservers = false;
bool ShowSkillPanelOnlyForHeroes = false;
bool ShowSkillPanelNeutral = false;


int __stdcall ShowObserverSkillPanel(int enabled)
{
	ShowSkillPanelForObservers = enabled;
	return ShowSkillPanelForObservers;
}

int __stdcall ShowSkillPanelForAllUnits(int enabled)
{
	ShowSkillPanelOnlyForHeroes = !enabled;
	return ShowSkillPanelOnlyForHeroes;
}

int __stdcall ShowSkillPanelForEnemyNeurals(int enabled)
{
	ShowSkillPanelNeutral = enabled != 0;
	return ShowSkillPanelNeutral;
}


signed int __fastcall  IsDrawSkillPanel_my(unsigned char* UnitAddr, int addr1)
{
	signed int result;
	int GETOID;
	int OID;
	if (addr1)
	{
		GETOID = *(int*)(addr1 + 444);
		if (GETOID > 0)
			OID = *(int*)(GETOID + 8);
		else
			OID = 852290;
		result = 1;

		// Сначала вызвать оригинальную функцию отрисовки, в случае успеха ничего не делать
		if (((IsNeedDrawUnitOrigin)(GameDll + IsNeedDrawUnitOriginOffset))(UnitAddr))
		{
			((DrawSkillPanel)(GameDll + DrawSkillPanelOffset))(UnitAddr, OID);
		}
		else if (IsNotBadUnit(UnitAddr))
		{
			// Если не враг
			if (!IsEnemy(UnitAddr))
			{
				//Если герой или включено отображение для всех
				if (!ShowSkillPanelOnlyForHeroes || IsHero(UnitAddr))
				{
					((DrawSkillPanel)(GameDll + DrawSkillPanelOffset))(UnitAddr, OID);
					return result;
				}
			}

			// Отрисовать Neutrals
			if (ShowSkillPanelNeutral && GetUnitOwnerSlot(UnitAddr) >= 12)
			{
				((DrawSkillPanel)(GameDll + DrawSkillPanelOffset))(UnitAddr, OID);
				return result;
			}

			// Отображать для зрителя
			if (ShowSkillPanelForObservers && IsLocalPlayerObserver())
			{
				if (!ShowSkillPanelOnlyForHeroes || IsHero(UnitAddr))
				{
					((DrawSkillPanel)(GameDll + DrawSkillPanelOffset))(UnitAddr, OID);
					return result;
				}
			}
		}
	}
	else
	{
		result = 0;
	}
	return result;
}

signed int __fastcall  IsDrawSkillPanelOverlay_my(unsigned char* UnitAddr, int addr1)
{
	signed int result; // eax@2
	int GETOID; // eax@3
	int OID; // esi@4
	if (addr1)
	{
		GETOID = *(int*)(addr1 + 444);
		if (GETOID > 0)
			OID = *(int*)(GETOID + 8);
		else
			OID = 852290;

		result = 1;


		// Сначала вызвать оригинальную функцию
		if (((IsNeedDrawUnitOrigin)(GameDll + IsNeedDrawUnitOriginOffset))(UnitAddr))
		{
			((DrawSkillPanelOverlay)(GameDll + DrawSkillPanelOverlayOffset))(UnitAddr, OID);
		}
		else if (IsNotBadUnit(UnitAddr))
		{
			// Если не враг
			if (!IsEnemy(UnitAddr))
			{
				//Если герой или включено отображение для всех
				if (!ShowSkillPanelOnlyForHeroes || IsHero(UnitAddr))
				{
					((DrawSkillPanelOverlay)(GameDll + DrawSkillPanelOverlayOffset))(UnitAddr, OID);
					return result;
				}
			}

			//// Отрисовать Neutrals
			if (!ShowSkillPanelOnlyForHeroes && GetUnitOwnerSlot(UnitAddr) >= 12)
			{
				((DrawSkillPanelOverlay)(GameDll + DrawSkillPanelOverlayOffset))(UnitAddr, OID);
				return result;
			}

			// Отображать для зрителя
			if (ShowSkillPanelForObservers && IsLocalPlayerObserver())
			{
				if (!ShowSkillPanelOnlyForHeroes || IsHero(UnitAddr)) 
				{
					((DrawSkillPanelOverlay)(GameDll + DrawSkillPanelOverlayOffset))(UnitAddr, OID);
					return result;
				}
			}
		}

	}
	else
	{
		result = 0;
	}
	return result;
}


int __fastcall IsNeedDrawUnit2_my(unsigned char* UnitAddr, int /* converted from thiscall to fastcall*/)
{
	if (IsNotBadUnit(UnitAddr))
	{
		// Если не враг
		if (!IsEnemy(UnitAddr))
		{
			//Если герой или включено отображение для всех
			if (!ShowSkillPanelOnlyForHeroes || IsHero(UnitAddr))
				return 1;
		}

		// Отрисовать Neutrals
		if (ShowSkillPanelNeutral && GetUnitOwnerSlot(UnitAddr) >= 12)
		{
			return 1;
		}

		// Отображать для зрителя
		if (ShowSkillPanelForObservers && IsLocalPlayerObserver())
		{
			if (!ShowSkillPanelOnlyForHeroes || IsHero(UnitAddr))
				return 1;
		}
	}
	return IsNeedDrawUnit2ptr(UnitAddr);
}