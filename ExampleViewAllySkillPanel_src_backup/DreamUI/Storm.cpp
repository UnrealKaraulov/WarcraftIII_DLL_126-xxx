#include "stdafx.h"
#include "Storm.h"
#include "FunctionTemplate.h"
#include <new>
#include <cstdlib>
#include "Tools.h"

namespace DreamStorm {
	bool StormAvailable = false;
	HMODULE StormModule = NULL;
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

	void Init(HMODULE hModule) {
		StormModule = hModule;
		FileOpenArchive = (PROTOTYPE_FileArchiveOpen)GetProcAddress(hModule, (LPCSTR)266);
		FileCloseArchive = (PROTOTYPE_FileArchiveClose)GetProcAddress(hModule, (LPCSTR)252);
		StringGetHash = (PROTOTYPE_StringGetHash)GetProcAddress(hModule, (LPCSTR)590);
		AddrMemAlloc = (void*)GetProcAddress(hModule, (LPCSTR)401);
		AddrMemFree = (void*)GetProcAddress(hModule, (LPCSTR)403);
		AddrMemGetSize = (void*)GetProcAddress(hModule, (LPCSTR)404);
		AddrMemReAlloc = (void*)GetProcAddress(hModule, (LPCSTR)405);
		AddrFileOpenFileEx = (void*)GetProcAddress(hModule, (LPCSTR)268); 
		AddrFileGetFileSize = (void*)GetProcAddress(hModule, (LPCSTR)265); 
		AddrFileReadFile = (void*)GetProcAddress(hModule, (LPCSTR)269); 
		AddrFileCloseFile = (void*)GetProcAddress(hModule, (LPCSTR)253); 
		AddrFileGetLocale = (void*)GetProcAddress(hModule, (LPCSTR)294);
		StormAvailable = true;
	}

	void Cleanup() {
		StormAvailable = false;
	}

	std::map<void*, void*> MemUserDataMap;

	//294	SFileGetLocale() 
	LANGID FileGetLocale(){
		return dreamaero::generic_c_call<uint16_t>(AddrFileGetLocale);
	}

	//TODO Debug版本加上调试信息
	void* MemAlloc(uint32_t size) {
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
		return dreamaero::generic_std_call<uint32_t>(
			AddrMemGetSize,
			addr,
			"",
			0
		);
	}

	void* MemReAlloc(void* addr, uint32_t size) {
		return dreamaero::generic_std_call<void*>(
			AddrMemReAlloc,
			addr,
			size,
			"",
			0,
			0
		);
	}

	HANDLE FileOpenFileEx(HANDLE hMpq, const char *szFileName, uint32_t dwSearchScope, HANDLE *phFile) {
		return dreamaero::generic_std_call<HANDLE>(
			AddrFileOpenFileEx,
			hMpq,
			szFileName,
			dwSearchScope,
			phFile
		);
	}

	uint32_t FileGetFileSize(HANDLE hFile, uint32_t *pdwFileSizeHigh) {
		return dreamaero::generic_std_call<uint32_t>(
			AddrFileGetFileSize,
			hFile,
			pdwFileSizeHigh
		);
	}

	bool FileReadFile(HANDLE hFile, void* lpBuffer,	uint32_t dwToRead, uint32_t* pdwRead, LPOVERLAPPED lpOverlapped) {
		return dreamaero::generic_std_call<bool>(
			AddrFileReadFile,
			hFile,
			lpBuffer,
			dwToRead,
			pdwRead,
			lpOverlapped
		);
	}

	bool FileCloseFile(HANDLE hFile) {
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