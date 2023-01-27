#include "Main.h"
#include "Storm.h"


GetTownUnitCount_p GetTownUnitCount_org;
GetTownUnitCount_p GetTownUnitCount_ptr;



int __cdecl Wc3MemoryRW(int* addr, int value, int write)
{
	if (write)
	{
		*addr = value;
	}
	else
	{
		return *addr;
	}
	return 0;
}

struct ModuleInfoStr
{
	void* addr;
	int size;
};

std::vector<ModuleInfoStr> Modules;

void* GetModuleFromAddress(unsigned char* addr)
{
	HMODULE hModule = NULL;
	GetModuleHandleEx(
		GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
		(LPCTSTR)addr,
		&hModule);

	return hModule;
}


int __stdcall ScanJassStringForErrors(int dump)
{

	unsigned char* strrepvtable = GameDll + 0x954658;



	int* firstoffset = (int*)(StormDll + 0x055370);




	int memsize = 0x100;
	int i = 0;
	int stringcount = 0;
	FILE* f = NULL;

	if (dump)
		fopen_s(&f, "dumpallbadstr.txt", "w");


	std::vector<int> foundoffsets;

	for (i = 0; i < memsize; i++)
	{
		int currentmemoffset = firstoffset[i];
		while (currentmemoffset > 0)
		{
			int currentstrrepoffset = currentmemoffset + 0x8C;
			if (std::find(foundoffsets.begin(), foundoffsets.end(), currentstrrepoffset) == foundoffsets.end())
			{
				foundoffsets.push_back(currentstrrepoffset);
				const char* currentnameoffset = (const char*)(currentmemoffset + 0x70);


				if (*(unsigned char**)(currentstrrepoffset) == strrepvtable)
				{

					StringRep* curstr = (StringRep*)currentstrrepoffset;

					while ((int)curstr->prev > 0 && (unsigned char*)curstr->prev->vtable == strrepvtable)
					{
						curstr = curstr->prev;
					}



					while ((unsigned char*)curstr->vtable == strrepvtable && (int)curstr->next > 0)
					{
						unsigned int strhash = Storm::StringGetHash(curstr->text);
						if (strhash != curstr->hash)
						{
							if (dump && f)
							{
								fprintf_s(f, "Found bad std::string[#1-%X]:%s\n", (unsigned int)curstr, curstr->text);
							}
							stringcount++;
						}

						curstr = curstr->next;
					}


				}
				else if (memcmp(currentnameoffset, "String2H", 8) == 0)
				{
					int curstr2hoffset = currentmemoffset + 0x94;
					String2H* curstr = (String2H*)curstr2hoffset;
					String2H* firststr = curstr;

					while ((int)curstr > 0)
					{

						unsigned int strhash = Storm::StringGetHash(curstr->text);
						if (strhash != curstr->hash)
						{
							if (dump && f)
								fprintf_s(f, "Found bad std::string[#2-%X]:%s\n", (unsigned int)curstr, curstr->text);
							stringcount++;
						}

						if ((int)curstr->next <= 0)
							break;
						curstr = curstr->next;
						if (firststr == curstr)
							break;

					}


				}
			}
			currentmemoffset = *(int*)currentmemoffset;
		}
	}

	if (dump)
		fclose(f);

	foundoffsets.clear();

	return stringcount;
}




int __stdcall GetJassStringCount(int dump)
{

	unsigned char* strrepvtable = GameDll + 0x954658;


	int* firstoffset = (int*)(StormDll + 0x055370);




	int memsize = 0x100;
	int i = 0;
	int stringcount = 0;
	FILE* f = NULL;

	if (dump)
		fopen_s(&f, "dumpallstr.txt", "w");

	std::vector<int> foundoffsets;

	for (i = 0; i < memsize; i++)
	{
		int currentmemoffset = firstoffset[i];
		while (currentmemoffset > 0)
		{
			int currentstrrepoffset = currentmemoffset + 0x8C;
			const char* currentnameoffset = (const char*)(currentmemoffset + 0x70);

			if (std::find(foundoffsets.begin(), foundoffsets.end(), currentstrrepoffset) == foundoffsets.end())
			{
				foundoffsets.push_back(currentstrrepoffset);

				if (*(unsigned char**)(currentstrrepoffset) == strrepvtable)
				{

					StringRep* curstr = (StringRep*)currentstrrepoffset;

					while ((int)curstr->prev > 0 && (unsigned char*)curstr->prev->vtable == strrepvtable)
					{
						curstr = curstr->prev;
					}



					while ((unsigned char*)curstr->vtable == strrepvtable && (int)curstr->next > 0)
					{
						if (dump && f)
						{
							fprintf_s(f, "%s\n", curstr->text);
						}
						stringcount++;
						curstr = curstr->next;
					}


				}
				else if (memcmp(currentnameoffset, "String2H", 8) == 0)
				{
					int curstr2hoffset = currentmemoffset + 0x94;
					String2H* curstr = (String2H*)curstr2hoffset;
					String2H* firststr = curstr;

					while ((int)curstr > 0)
					{

						if (dump && f)
							fprintf_s(f, "%s\n", curstr->text);
						stringcount++;


						if ((int)curstr->next <= 0)
							break;
						curstr = curstr->next;
						if (firststr == curstr)
							break;

					}


				}
			}
			currentmemoffset = *(int*)currentmemoffset;
		}
	}

	if (dump)
		fclose(f);

	foundoffsets.clear();

	return stringcount;
}


int UseWarnIsBadReadPtr = 0;


int IsOkayPtr(void* ptr, unsigned int size)
{
	if (UseWarnIsBadReadPtr == 0)
	{
		if (!ptr || size > 0x20000000)
			return false;
		return true;
	}
	if (UseWarnIsBadReadPtr == 1)
	{
		int returnvalue = IsBadReadPtr(ptr, size) == 0;
		return returnvalue;
	}
	else if (UseWarnIsBadReadPtr == 2)
	{
		MEMORY_BASIC_INFORMATION mbi;

		if (VirtualQuery(ptr, &mbi, sizeof(MEMORY_BASIC_INFORMATION)) == 0)
		{
			return 0;
		}

		if ((int)ptr + size > (int)mbi.BaseAddress + mbi.RegionSize)
		{
			return 0;
		}

		if ((int)ptr < (int)mbi.BaseAddress)
		{
			return 0;
		}

		if (mbi.State != MEM_COMMIT)
		{
			return 0;
		}


		if (mbi.Protect != PAGE_READWRITE && mbi.Protect != PAGE_WRITECOPY && mbi.Protect != PAGE_READONLY)
		{
			return 0;
		}

		return 1;
	}
	else
		return 1;
}
