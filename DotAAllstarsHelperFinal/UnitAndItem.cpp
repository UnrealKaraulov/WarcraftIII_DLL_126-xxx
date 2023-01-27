#include "Main.h"


pGetHeroInt GetHeroInt;
// Получить владельца юнита
int __stdcall GetUnitOwnerSlot(unsigned char* unitaddr)
{
	if (unitaddr)
		return *(int*)(unitaddr + 88);
	return 15;
}


std::vector< unsigned char*> GetUnitsFromGroup(int grouphandle)
{
	std::vector< unsigned char*> localvector;

	if (SetInfoObjDebugVal)
	{
		PrintText(("int:" + std::to_string(grouphandle)).c_str());
	}

	// Get group address
	unsigned char* GroupAddr = ConvertHandle(grouphandle);

	if (SetInfoObjDebugVal)
	{
		PrintText(("GroupAddr:" + std::to_string((unsigned long)GroupAddr)).c_str());
	}

	if (GroupAddr)
	{

		// Set group offset
		GroupAddr += 0x24;
		// Set group data
		if (SetInfoObjDebugVal)
		{
			PrintText("Search units...");
		}
		int GroupData = *(int*)(GroupAddr + 0xC);
		GroupData = *(int*)(GroupAddr + 0xC);
		while (GroupData > 0)
		{

			// Get unit 
			unsigned char* UnitAddr = *(unsigned char**)(GroupData + 0x8);

			if (SetInfoObjDebugVal)
			{
				PrintText(("Found:" + std::to_string((unsigned long)UnitAddr)).c_str());
			}

			// Get next data
			GroupData = *(int*)(GroupData + 0x4);

			if (UnitAddr && GetTypeId(UnitAddr) != 'H00J' && /* IsNotBadUnit( UnitAddr ) &&*/ IsHero(UnitAddr) && !IsUnitIllusion(UnitAddr))
				// save unit to list
				localvector.push_back(UnitAddr);
		}

		GroupData = *(int*)(GroupAddr + 0xC);
		while (GroupData > 0)
		{

			// Get unit 
			unsigned char* UnitAddr = *(unsigned char**)(GroupData + 0x8);

			if (SetInfoObjDebugVal)
			{
				PrintText(("Found:" + std::to_string((unsigned long)UnitAddr)).c_str());
			}

			// Get next data
			GroupData = *(int*)(GroupData + 0x4);

			if (UnitAddr &&/* IsNotBadUnit( UnitAddr ) &&*/ !IsHero(UnitAddr) && !IsUnitIllusion(UnitAddr))
				// save unit to list
				localvector.push_back(UnitAddr);
		}


		GroupData = *(int*)(GroupAddr + 0xC);
		while (GroupData > 0)
		{

			// Get unit 
			unsigned char* UnitAddr = *(unsigned char**)(GroupData + 0x8);

			if (SetInfoObjDebugVal)
			{
				PrintText(("Found:" + std::to_string((unsigned long)UnitAddr)).c_str());
			}

			// Get next data
			GroupData = *(int*)(GroupData + 0x4);

			if (UnitAddr /*&& IsNotBadUnit( UnitAddr ) &&* !IsHero( UnitAddr )*/ && GetTypeId(UnitAddr) == 'H00J')
				// save unit to list
				localvector.push_back(UnitAddr);
		}

		GroupData = *(int*)(GroupAddr + 0xC);
		while (GroupData > 0)
		{

			// Get unit 
			unsigned char* UnitAddr = *(unsigned char**)(GroupData + 0x8);

			if (SetInfoObjDebugVal)
			{
				PrintText(("Found:" + std::to_string((unsigned long)UnitAddr)).c_str());
			}

			// Get next data
			GroupData = *(int*)(GroupData + 0x4);

			if (UnitAddr /*&& IsNotBadUnit( UnitAddr ) &&* !IsHero( UnitAddr )*/ && IsUnitIllusion(UnitAddr))
				// save unit to list
				localvector.push_back(UnitAddr);
		}

	}



	return localvector;
}

// Является ли юнит героем
int __stdcall IsHero(unsigned char* unitaddr)
{
	if (unitaddr)
	{
		unsigned int ishero = *(unsigned int*)(unitaddr + 48);
		ishero = ishero >> 24;
		ishero = ishero - 64;
		return ishero < 0x19;
	}
	return false;
}


// Является ли юнит зданием
int __stdcall IsTower(unsigned char* unitaddr)
{
	if (unitaddr)
	{
		unsigned int istower = *(unsigned int*)(unitaddr + 0x5C);
		return (istower & 0x10000) > 0;
	}
	return false;
}


int __stdcall IsUnitInvulnerable(unsigned char* unitaddr)
{
	return (*(unsigned int*)(unitaddr + 0x20) & 8);
}

int __stdcall IsUnitIllusion(unsigned char* unitaddr)
{
	return (*(unsigned int*)(unitaddr + 0x5C) & 0x40000000) > 0;
}

// Проверяет юнит или не юнит
int __stdcall IsNotBadUnit(unsigned char* unitaddr, int onlymem)
{
	if (unitaddr)
	{
		int xaddraddr = (int)&UnitVtable;

		if (*(unsigned char*)xaddraddr != *(unsigned char*)unitaddr)
			return false;
		else if (*(unsigned char*)(xaddraddr + 1) != *(unsigned char*)(unitaddr + 1))
			return false;
		else if (*(unsigned char*)(xaddraddr + 2) != *(unsigned char*)(unitaddr + 2))
			return false;
		else if (*(unsigned char*)(xaddraddr + 3) != *(unsigned char*)(unitaddr + 3))
			return false;

		unsigned int x1 = *(unsigned int*)(unitaddr + 0xC);
		unsigned int y1 = *(unsigned int*)(unitaddr + 0x10);

		int udata = *(int*)(unitaddr + 0x28);


		if (x1 == 0xFFFFFFFF || y1 == 0xFFFFFFFF || udata == 0)
		{
			if (SetInfoObjDebugVal)
			{
				PrintText("Coordinates or 0x28 offset bad");
			}
			return false;
		}

		if (onlymem)
			return true;

		unsigned int unitflag = *(unsigned int*)(unitaddr + 0x20);
		unsigned int unitflag2 = *(unsigned int*)(unitaddr + 0x5C);

		if (unitflag & 1u)
		{
			if (SetInfoObjDebugVal)
			{
				PrintText("Flag 1 bad");
			}
			return false;
		}

		if (!(unitflag & 2u))
		{
			if (SetInfoObjDebugVal)
			{
				PrintText("Flag 2 bad");
			}
			return false;
		}

		if ((unitflag2 & 0x100u) > 0)
		{
			if (SetInfoObjDebugVal)
			{
				PrintText("Flag 3 bad");
			}
			return false;
		}

		/*	if ( unitflag2 == 0x1001u )
			{
				if ( SetInfoObjDebugVal )
				{
					PrintText( "Flag 4 bad" );
				}
				return false;
			}
	*/
		return true;
	}
	if (SetInfoObjDebugVal)
	{
		PrintText("FATAL ERROR. NO UNIT ADDRESS FOUND");
	}
	return false;
}


// Проверяет враг юнит локальному игроку или нет
int __stdcall IsEnemy( unsigned char * UnitAddr)
{
	if (UnitAddr && IsNotBadUnit(UnitAddr))
	{
		int unitownerslot = GetUnitOwnerSlot(UnitAddr);

		if (GetLocalPlayerId() == unitownerslot || IsPlayerObserver(GetLocalPlayerId()))
		{
			return false;
		}

		if (unitownerslot <= 15 && unitownerslot >= 0 && GetLocalPlayerId() <= 15 && GetLocalPlayerId() >= 0)
		{
			int Player1 = Player(unitownerslot);
			int Player2 = Player(GetLocalPlayerId());

			if (Player1 == Player2)
			{
				return false;
			}
			if (Player1 == 0 || Player2 == 0)
			{
				return false;
			}

			int retval = IsPlayerEnemy(Player1, Player2);
			return retval;
		}
	}
	return false;
}


// Проверяет предмет или не предмет
int __stdcall IsNotBadItem(unsigned char * itemaddr, int extracheck)
{
	if (itemaddr)
	{
		int xaddraddr = (int)&ItemVtable;

		if (*(unsigned char*)xaddraddr != *(unsigned char*)itemaddr)
			return false;
		else if (*(unsigned char*)(xaddraddr + 1) != *(unsigned char*)(itemaddr + 1))
			return false;
		else if (*(unsigned char*)(xaddraddr + 2) != *(unsigned char*)(itemaddr + 2))
			return false;
		else if (*(unsigned char*)(xaddraddr + 3) != *(unsigned char*)(itemaddr + 3))
			return false;

		if (extracheck)
		{
			float hitpoint = *(float*)(itemaddr + 0x58);

			return hitpoint != 0.0f;
		}

		return true;
	}

	return false;
}





int GetSelectedUnitCountBigger(int slot)
{
	int plr = GetPlayerByNumber(slot);
	if (plr > 0)
	{
		int PlayerData1 = *(int*)(plr + 0x34);
		if (PlayerData1 > 0)
		{
			int unitcount = *(int*)(PlayerData1 + 0x10);
			int unitcount2 = *(int*)(PlayerData1 + 0x1D4);

			if (unitcount > unitcount2)
				return unitcount;
			else
				return unitcount2;
		}
	}

	return NULL;
}


unsigned char* GetSelectedUnit(int slot)
{
	if (slot >= 0 && slot <= 15)
	{
		int plr = GetPlayerByNumber(slot);
		if (plr > 0)
		{
			int PlayerData1 = *(int*)(plr + 0x34);
			if (PlayerData1 > 0)
			{
				return *(unsigned char**)(PlayerData1 + 0x1e0);
			}
		}
	}

	return NULL;
}





int ReadObjectAddrFromGlobalMat(unsigned int a1, unsigned int a2)
{
	int found1;
	int result;
	int AddrType1;
	int v5;

	if (!(a1 >> 31))
	{
		if (a1 < *(unsigned int*)(*(int*)pGameClass1 + 28))
		{
			found1 = *(int*)(*(int*)(*(int*)pGameClass1 + 12) + 8 * a1) == -2;
			if (!found1)
				return 0;
			if (a1 >> 31)
			{
				AddrType1 = *(int*)(*(int*)(*(int*)pGameClass1 + 44) + 8 * a1 + 4);
				result = *(unsigned int*)(AddrType1 + 24) != a2 ? 0 : AddrType1;
			}
			else
			{
				v5 = *(int*)(*(int*)(*(int*)pGameClass1 + 12) + 8 * a1 + 4);
				result = *(unsigned int*)(v5 + 24) != a2 ? 0 : v5;
			}
			return result;
		}
		return 0;
	}
	if ((a1 & 0x7FFFFFFF) >= *(unsigned int*)(*(int*)pGameClass1 + 60))
		return 0;
	found1 = *(int*)(*(int*)(*(int*)pGameClass1 + 44) + 8 * a1) == -2;
	if (!found1)
		return 0;
	if (a1 >> 31)
	{
		AddrType1 = *(int*)(*(int*)(*(int*)pGameClass1 + 44) + 8 * a1 + 4);
		result = *(unsigned int*)(AddrType1 + 24) != a2 ? 0 : AddrType1;
	}
	else
	{
		v5 = *(int*)(*(int*)(*(int*)pGameClass1 + 12) + 8 * a1 + 4);
		result = *(unsigned int*)(v5 + 24) != a2 ? 0 : v5;
	}
	return result;
}


unsigned char* GetObjectDataAddr(unsigned char* addr)
{
	int mataddr;
	unsigned char* result; // eax@3

	mataddr = ReadObjectAddrFromGlobalMat(*(unsigned int*)addr, *(unsigned int*)(addr + 4));

	if (!mataddr || *(int*)(mataddr + 32))
		result = 0;
	else
		result = *(unsigned char**)(mataddr + 84);
	return result;
}


std::vector<unsigned char*> ReturnAbils;

unsigned char** FindUnitAbils(unsigned char* unitaddr, unsigned int* count, int abilcode, int abilbasecode)
{
	if (!ReturnAbils.empty())
		ReturnAbils.clear();
	*count = 0;
	if (unitaddr)
	{
		unsigned char * pAddr1 = unitaddr + 0x1DC;
		unsigned char* pAddr2 = unitaddr + 0x1E0;

		if ((int)(*(unsigned int*)(pAddr1) & *(unsigned int*)(pAddr2)) != -1)
		{
			//PrintText( "Found abils ... 1" );
			unsigned char* pData = GetObjectDataAddr(pAddr1);

			while ((long long)pData > 0)
			{
				//	PrintText( "Found abils ... 2" );
				unsigned char* pData2 = *(unsigned char**)(pData + 0x54);
				if (pData2)
				{
					/*	char foundabil3[ 100 ];
						sprintf_s( foundabil3, "%s:%X:%X", "Found new abil:", *( int* )( pData2 + 0x30 ), *( int* )( pData2 + 0x34 ) );

						PrintText(foundabil3 );*/

					if (abilcode != 0 && *(int*)(pData2 + 0x34) == abilcode)
					{
						if (abilbasecode != 0 && *(int*)(pData2 + 0x30) == abilbasecode)
						{
							ReturnAbils.push_back(pData);
						}
						else if (abilbasecode == 0)
						{
							ReturnAbils.push_back(pData);
						}
					}
					else if (abilcode == 0)
					{
						if (abilbasecode != 0 && *(int*)(pData2 + 0x30) == abilbasecode)
						{
							ReturnAbils.push_back(pData);
						}
						else if (abilbasecode == 0)
						{
							ReturnAbils.push_back(pData);
						}
					}
				}
				pData = GetObjectDataAddr(pData + 0x24);
			}

			*count = ReturnAbils.size();
		}
	}

	return &ReturnAbils[0];
}





int* GetItemCountAndItemArray(unsigned char*** itemarray)
{
	int GlobalClassOffset = *(int*)(pW3XGlobalClass);
	if (GlobalClassOffset)
	{
		int ItemsOffset1 = *(int*)(GlobalClassOffset + 0x3BC) + 0x10;
		if (ItemsOffset1)
		{
			int* ItemsCount = (int*)(ItemsOffset1 + 0x604);
			if (*ItemsCount > 0)
			{
				*itemarray = (unsigned char**)*(int*)(ItemsOffset1 + 0x608);
				return ItemsCount;
			}
		}
	}

	*itemarray = 0;
	return 0;
}



int* GetUnitCountAndUnitArray(unsigned char*** unitarray)
{
	int GlobalClassOffset = *(int*)(pW3XGlobalClass);
	if (GlobalClassOffset)
	{
		int UnitsOffset1 = *(int*)(GlobalClassOffset + 0x3BC);
		if (UnitsOffset1)
		{
			int* UnitsCount = (int*)(UnitsOffset1 + 0x604);
			if (*UnitsCount > 0)
			{
				*unitarray = (unsigned char**)*(int*)(UnitsOffset1 + 0x608);
				return UnitsCount;
			}
		}
	}

	*unitarray = 0;
	return 0;
}



void GetUnitLocation2D(unsigned char* unitaddr, float* x, float* y)
{
	if (unitaddr && *(int*)(unitaddr + 0x284) != 0 && *(float*)(unitaddr + 0x288) != 0)
	{
		*x = *(float*)(unitaddr + 0x284);
		*y = *(float*)(unitaddr + 0x288);
	}
	else
	{
		*x = 0.0;
		*y = 0.0;
	}
}


void GetItemLocation2D(unsigned char * itemaddr, float* x, float* y)
{
	if (itemaddr)
	{
		int iteminfo = *(int*)(itemaddr + 0x28);
		if (iteminfo)
		{
			*x = *(float*)(iteminfo + 0x88);
			*y = *(float*)(iteminfo + 0x8C);
		}
		else
		{
			*x = 0.0;
			*y = 0.0;
		}
	}
	else
	{
		*x = 0.0;
		*y = 0.0;
	}
}




unsigned char* GetUnitAddressFloatsRelated(unsigned char* unitaddr, int step)
{
	if (unitaddr)
	{
		unsigned char* offset1 = unitaddr + step;
		unsigned char* offset2 = *(unsigned char**)pGameClass1;

		if (*(int*)offset1 && offset2)
		{
			int offset3 = *(int*)offset1;
			offset2 = *(unsigned char**)(offset2 + 0xC);
			if (offset2)
			{
				return *(unsigned char **)((offset3 * 8) + offset2 + 4);
			}
		}
	}
	return 0;
}


float GetUnitHPregen(unsigned char* unitaddr)
{
	float result = 0.0f;
	if (unitaddr)
	{
		unsigned char* offset1 = GetUnitAddressFloatsRelated(unitaddr, 0xA0);
		if (offset1)
		{

			result = *(float*)(offset1 + 0x7C);
		}
	}
	return result;
}

float GetUnitHP(unsigned char* unitaddr)
{
	float result = 0.0f;
	if (unitaddr)
	{
		unsigned char* offset1 = GetUnitAddressFloatsRelated(unitaddr, 0xA0);
		if (offset1)
		{

			result = *(float*)(offset1 + 0x80);
		}
	}
	return result;
}

_GetUnitFloatStat GetUnitFloatState = NULL;


float GetUnitMP(unsigned char* unitaddr)
{
	float result = 0.0f;
	if (unitaddr)
	{
		unsigned char* offset1 = GetUnitAddressFloatsRelated(unitaddr, 0xC0);
		if (offset1)
		{
			/*char test[ 100 ];
			sprintf_s( test, 100, "Unit MP struct: %X", offset1 );
			PrintText( test );*/
			result = *(float*)(offset1 + 0x80);
		}
	}
	return result;
}

float GetUnitMPregen(unsigned char* unitaddr)
{
	float result = 0.0f;
	if (unitaddr)
	{
		unsigned char* offset1 = GetUnitAddressFloatsRelated(unitaddr, 0xC0);
		if (offset1)
		{
			result = *(float*)(offset1 + 0x7C);
		}
	}
	return result;
}




float GetUnitX_real(unsigned char* unitaddr)
{
	float result = 0.0f;
	if (unitaddr)
	{
		unsigned char* offset1 = GetUnitAddressFloatsRelated(unitaddr, 0xA0);
		if (offset1)
		{
			unsigned int x1 = *(unsigned int*)(offset1 + 0x14);
			unsigned int y1 = *(unsigned int*)(offset1 + 0x18);
			if (x1 != 0xFFFFFFFF && y1 != 0xFFFFFFFF)
			{
				offset1 = *(unsigned char**)(offset1 + 0x28);
				if (offset1)
				{
					result = *(float*)(offset1 + 0x54);
				}
			}
		}
	}
	return result;
}

float GetUnitY_real(unsigned char* unitaddr)
{
	float result = 0.0f;
	if (unitaddr)
	{
		unsigned char* offset1 = GetUnitAddressFloatsRelated(unitaddr, 0xA0);
		if (offset1)
		{
			unsigned int x1 = *(unsigned int*)(offset1 + 0x14);
			unsigned int y1 = *(unsigned int*)(offset1 + 0x18);
			if (x1 != 0xFFFFFFFF && y1 != 0xFFFFFFFF)
			{
				offset1 = *(unsigned char**)(offset1 + 0x28);
				if (offset1)
				{
					result = *(float*)(offset1 + 0x58);
				}
			}
		}
	}
	return result;
}

void(__thiscall* SelectUnitReal)(int pPlayerSelectData, unsigned char * pUnit, int id, int unk1, int unk2, int unk3) = NULL;
void(__thiscall* UpdatePlayerSelection)(int pPlayerSelectData, int unk) = NULL;
int(__cdecl* ClearSelection)(void) = NULL;

int(__thiscall* sub_6F332700)(void* a1);

void SelectUnit(unsigned char* unit)
{
	if (SetInfoObjDebugVal)
	{
		PrintText(("Select unit:" + uint_to_hex((unsigned long)unit)).c_str());
		PrintText(("IsNotBadUnit:" + uint_to_hex(IsNotBadUnit(unit))).c_str());
	}
	if (SelectUnitReal && UpdatePlayerSelection && IsNotBadUnit(unit))
	{
		sub_6F332700 = (int(__thiscall*)(void* a1))(GameDll + 0x332700);
		int playerslot = GetLocalPlayerId();
		int LocalPlayer = GetPlayerByNumber(playerslot);
		int playerseldata = *(int*)(LocalPlayer + 0x34);
		SelectUnitReal(playerseldata, unit, playerslot, 0, 1, 1);
		UpdatePlayerSelection((int)playerseldata, 0);
		sub_6F332700(0);
	}
}