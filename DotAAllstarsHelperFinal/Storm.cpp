#include "Main.h"
#include "Storm.h"
#include "FunctionTemplate.h"
#include <new>
#include <cstdlib>
#include <vector>

namespace Storm {

	struct StormLeakHelper
	{
		void* memaddr;
		int retaddr;
	};

	std::vector<StormLeakHelper> LeakHelperList;

	int AllocationCount = 0;
	bool StormAvailable = false;
	void* StormModule = NULL;
	PROTOTYPE_FileArchiveOpen FileOpenArchive = NULL;
	PROTOTYPE_FileArchiveClose FileCloseArchive = NULL;
	PROTOTYPE_StringGetHash StringGetHash = NULL;
	void* AddrMemAlloc;
	void* AddrMemFree;
	void* AddrMemGetSize;
	void* AddrMemReAlloc;
	void* AddrFileOpenFileEx;
	void* AddrFileGetFileSize;
	void* AddrFileReadFile;
	void* AddrFileCloseFile;
	void* AddrFileGetLocale;

	void FreeAllMemory()
	{

		for (auto s : LeakHelperList)
		{
			if (s.memaddr != NULL)
				Storm::MemFree(s.memaddr);
		}
		LeakHelperList.clear();
	}

	void Init(void* hModule) {
		if (!hModule)
			return;

		StormModule = hModule;
		FileOpenArchive = (PROTOTYPE_FileArchiveOpen)GetProcAddress((HMODULE)hModule, (LPCSTR)266);
		FileCloseArchive = (PROTOTYPE_FileArchiveClose)GetProcAddress((HMODULE)hModule, (LPCSTR)252);
		StringGetHash = (PROTOTYPE_StringGetHash)GetProcAddress((HMODULE)hModule, (LPCSTR)590);
		AddrMemAlloc = (void*)GetProcAddress((HMODULE)hModule, (LPCSTR)401);
		AddrMemFree = (void*)GetProcAddress((HMODULE)hModule, (LPCSTR)403);
		AddrMemGetSize = (void*)GetProcAddress((HMODULE)hModule, (LPCSTR)404);
		AddrMemReAlloc = (void*)GetProcAddress((HMODULE)hModule, (LPCSTR)405);
		AddrFileOpenFileEx = (void*)GetProcAddress((HMODULE)hModule, (LPCSTR)268);
		AddrFileGetFileSize = (void*)GetProcAddress((HMODULE)hModule, (LPCSTR)265);
		AddrFileReadFile = (void*)GetProcAddress((HMODULE)hModule, (LPCSTR)269);
		AddrFileCloseFile = (void*)GetProcAddress((HMODULE)hModule, (LPCSTR)253);
		AddrFileGetLocale = (void*)GetProcAddress((HMODULE)hModule, (LPCSTR)294);
		StormAvailable = true;
	}

	//294	SFileGetLocale() 
	unsigned short FileGetLocale() {

		return aero::generic_c_call<uint16_t>(AddrFileGetLocale);
	}

	//TODO Debug版本加上调试信息
	void* MemAlloc(unsigned int size) {
		auto retaddr = _ReturnAddress();
		
		if (!StormAvailable)
			MessageBoxA(0, "Storm not initialized", "Error1", 0);

		void* retval = aero::generic_std_call<void*>(
			AddrMemAlloc,
			size + 4,
			"DotaMem",
			1,
			0
			);

		if (!retval)
			MessageBoxA(0, "Storm error. No memory!", "Fatal error", 0);

		if (SetInfoObjDebugVal)
		{
			AllocationCount++;
			StormLeakHelper tmpleak = StormLeakHelper();
			tmpleak.retaddr = (int)retaddr;

			tmpleak.memaddr = retval;
			LeakHelperList.push_back(tmpleak);
		}
		return retval;
	}

	void ShowAllLeaks()
	{
		std::string outleaks;
		char tmps[50];

		for (auto s : LeakHelperList)
		{
			sprintf_s(tmps, "%X:%X\r\n", (unsigned int)s.retaddr, (unsigned int)s.memaddr);
			outleaks = outleaks + tmps;
		}
		MessageBoxA(0, outleaks.c_str(), (std::to_string(LeakHelperList.size()) + " leak found!").c_str(), 0);
	}

	void* MemFree(void* addr) {

		if (!addr || !NeedReleaseUnusedMemory)
			return 0;

		if (SetInfoObjDebugVal)
		{
			AllocationCount--;

			bool FounMem = false;

			for (unsigned int i = 0; i < LeakHelperList.size(); i++)
			{
				if (LeakHelperList[i].memaddr == addr)
				{
					LeakHelperList.erase(LeakHelperList.begin() + i);
					FounMem = true;
					break;
				}
			}

			if (!FounMem)
			{
				if (SetInfoObjDebugVal)
				{
					//MessageBoxA( 0, " Tried to free unknown memory!", "Fatal4ik error!", 0 );
					throw std::exception("Fatal Error! Tried to free unknown memory!");
				}
				//
				return 0;
			}
		}

		if (!addr)
			return 0;

		return aero::generic_std_call<void*>(
			AddrMemFree,
			addr,
			"DotaMem",
			2,
			0
			);
	}

	int MemGetSize(void* addr) {
		if (!addr)
			return 0;

		return aero::generic_std_call<unsigned int>(
			AddrMemGetSize,
			addr,
			"DotaMem",
			3
			);
	}

	void* MemReAlloc(void* addr, unsigned int size) {
		auto retaddr = _ReturnAddress();

		if (!addr || size > 1073741824)
		{
			char templog[256];
			sprintf_s(templog, 256, "Error ReAlloc: %X %X %X", (unsigned int)retaddr, size, (unsigned int)addr);
			MessageBoxA(0, templog, "Error1", 0);
			return 0;
		}

		for (unsigned int i = 0; i < LeakHelperList.size(); i++)
		{
			if (LeakHelperList[i].memaddr == addr)
			{
				LeakHelperList.erase(LeakHelperList.begin() + i);
				break;
			}
		}

		if (!StormAvailable)
			MessageBoxA(0, "Storm not initialized", "Error1", 0);


		void* retval = aero::generic_std_call<void*>(
			AddrMemReAlloc,
			addr,
			size + 4,
			"DotaMem",
			4,
			0
			);


		StormLeakHelper tmpleak = StormLeakHelper();
		tmpleak.retaddr = (int)retaddr;

		tmpleak.memaddr = retval;
		LeakHelperList.push_back(tmpleak);

		return retval;
	}

	void* FileOpenFileEx(void* hMpq, const char* szFileName, unsigned int dwSearchScope, void** phFile) {

		return aero::generic_std_call<void*>(
			AddrFileOpenFileEx,
			hMpq,
			szFileName,
			dwSearchScope,
			phFile
			);
	}

	unsigned int FileGetFileSize(void* hFile, unsigned int* pdwFileSizeHigh) {

		return aero::generic_std_call<unsigned int>(
			AddrFileGetFileSize,
			hFile,
			pdwFileSizeHigh
			);
	}

	bool FileReadFile(void* hFile, void* lpBuffer, unsigned int dwToRead, unsigned int* pdwRead, LPOVERLAPPED lpOverlapped) {
		return aero::generic_std_call<bool>(
			AddrFileReadFile,
			hFile,
			lpBuffer,
			dwToRead,
			pdwRead,
			lpOverlapped
			);
	}

	bool FileCloseFile(void* hFile) {

		return aero::generic_std_call<bool>(
			AddrFileCloseFile,
			hFile
			);
	}

}

/*
#ifndef _DEBUG
struct MemInfo {
	const char* FILE;
	int LINE;
};

std::map<void*, MemInfo> MemInfoMap;

void *operator new(size_t size) {
	void* rv;

	if (!Storm::StormAvailable) {
		rv = malloc(size);
		//OutputDebug("NonStormAlloc: 0x%X (%u)", rv, size);

		return rv;
	}

	if (size == 0)
		size = 1;

	while (1) {
		if ((rv = Storm::MemAlloc(size)) != NULL) {
			return rv;
		}

		// 分配不成功，找出当前出错处理函数
		new_handler globalhandler = std::set_new_handler(0);
		std::set_new_handler(globalhandler);

		if (globalhandler)
			(*globalhandler)();
		else throw std::bad_alloc();
	}
}

void operator delete(void *rawmemory) {
	if (rawmemory == 0)
		return;

	if (!Storm::StormAvailable) {
		free(rawmemory);
		//OutputDebug("NonStormFree: 0x%X", rawmemory);
		return;
	}

	Storm::MemFree(rawmemory);
	return;
}
#endif
*/