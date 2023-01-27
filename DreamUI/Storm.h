#include "stdafx.h"
#ifndef STORM_H_INCLUDED_
#define STORM_H_INCLUDED_

namespace DreamStorm {
	extern void* StormModule;

	template <typename ReturnType>
	ReturnType Offset(unsigned int offset) {
		return StormModule ? dreamaero::pointer_calc<ReturnType>(StormModule, offset) : NULL;
	}

	typedef int(__stdcall* PROTOTYPE_FileArchiveOpen)(const char* mpqName, unsigned long priority, unsigned long flags, void** pMpqHandle);
	extern PROTOTYPE_FileArchiveOpen FileOpenArchive;

	typedef int(__stdcall* PROTOTYPE_FileArchiveClose)(void* mpqHandle);
	extern PROTOTYPE_FileArchiveClose FileCloseArchive;

	typedef unsigned int(__stdcall* PROTOTYPE_StringGetHash)(const char* str);
	extern PROTOTYPE_StringGetHash StringGetHash;

	void* MemAlloc(unsigned int size);

	extern std::map<void*, void*> MemUserDataMap;
	template <typename Type>
	Type* MemAllocStruct(void* userData = NULL) {
		Type* rv = reinterpret_cast<Type*>(MemAlloc(sizeof(Type)));
		MemUserDataMap[rv] = userData;
		return rv;
	}

	template <typename Type>
	void* MemGetUserData(Type* addr) {
		if (MemUserDataMap.count(addr))
			return MemUserDataMap[addr];
		return NULL;
	}

	void* MemFree(void* addr);
	int MemGetSize(void* addr);
	void* MemReAlloc(void* addr, unsigned int size);

	void* FileOpenFileEx(void* hMpq, const char* szFileName, unsigned int dwSearchScope, void** phFile);
	unsigned int FileGetFileSize(void* hFile, unsigned int* pdwFileSizeHigh);
	bool FileReadFile(void* hFile, void* lpBuffer, unsigned int dwToRead, unsigned int* pdwRead, LPOVERLAPPED lpOverlapped);
	bool FileCloseFile(void* hFile);
	unsigned short FileGetLocale();

	void Init(void* hModule);
	void Cleanup();
}
#endif