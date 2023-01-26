#include "stdafx.h"
#include "Storm.h"
#include "FunctionTemplate.h"
#include <new>
#include <cstdlib>
#include "Tools.h"

namespace DreamStorm {
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

	void Init(void* hModule) {
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

	void Cleanup() {
		StormAvailable = false;
	}

	std::map<void*, void*> MemUserDataMap;

	//294	SFileGetLocale() 
	unsigned short FileGetLocale() {
		return dreamaero::generic_c_call<uint16_t>(AddrFileGetLocale);
	}

	//TODO Debug版本加上调试信息
	void* MemAlloc(unsigned int size) {
		return dreamaero::generic_std_call<void*>(
			AddrMemAlloc,
			size,
			"",
			0,
			0
			);
	}

	void* MemFree(void* addr) {
		MemUserDataMap.erase(addr);
		return dreamaero::generic_std_call<void*>(
			AddrMemFree,
			addr,
			"",
			0,
			0
			);
	}

	int MemGetSize(void* addr) {
		return dreamaero::generic_std_call<unsigned int>(
			AddrMemGetSize,
			addr,
			"",
			0
			);
	}

	void* MemReAlloc(void* addr, unsigned int size) {
		return dreamaero::generic_std_call<void*>(
			AddrMemReAlloc,
			addr,
			size,
			"",
			0,
			0
			);
	}

	void* FileOpenFileEx(void* hMpq, const char* szFileName, unsigned int dwSearchScope, void** phFile) {
		return dreamaero::generic_std_call<void*>(
			AddrFileOpenFileEx,
			hMpq,
			szFileName,
			dwSearchScope,
			phFile
			);
	}

	unsigned int FileGetFileSize(void* hFile, unsigned int* pdwFileSizeHigh) {
		return dreamaero::generic_std_call<unsigned int>(
			AddrFileGetFileSize,
			hFile,
			pdwFileSizeHigh
			);
	}

	bool FileReadFile(void* hFile, void* lpBuffer, unsigned int dwToRead, unsigned int* pdwRead, LPOVERLAPPED lpOverlapped) {
		return dreamaero::generic_std_call<bool>(
			AddrFileReadFile,
			hFile,
			lpBuffer,
			dwToRead,
			pdwRead,
			lpOverlapped
			);
	}

	bool FileCloseFile(void* hFile) {
		return dreamaero::generic_std_call<bool>(
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
		//;//OutputDebug("NonStormAlloc: 0x%X (%u)", rv, size);

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
		//;//OutputDebug("NonStormFree: 0x%X", rawmemory);
		return;
	}

	Storm::MemFree(rawmemory);
	return;
}
#endif
*/