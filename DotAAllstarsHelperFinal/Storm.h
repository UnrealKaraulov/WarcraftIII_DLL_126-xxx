#pragma once
#include <fp_call.h>

namespace Storm {
	extern int AllocationCount;
	extern void* StormModule;
	extern bool StormAvailable;
	extern void* AddrMemAlloc;
	extern void* AddrMemFree;
	extern void* AddrMemGetSize;
	extern void* AddrMemReAlloc;
	extern void* AddrFileOpenFileEx;
	extern void* AddrFileGetFileSize;
	extern void* AddrFileReadFile;
	extern void* AddrFileCloseFile;
	extern void* AddrFileGetLocale;

	typedef int(__stdcall* PROTOTYPE_FileArchiveOpen)(const char* mpqName, unsigned long priority, unsigned long flags, void** pMpqHandle);
	extern PROTOTYPE_FileArchiveOpen FileOpenArchive;

	typedef int(__stdcall* PROTOTYPE_FileArchiveClose)(void* mpqHandle);
	extern PROTOTYPE_FileArchiveClose FileCloseArchive;

	typedef unsigned int(__stdcall* PROTOTYPE_StringGetHash)(const char* str);
	extern PROTOTYPE_StringGetHash StringGetHash;

	void* MemAlloc(unsigned int size);

	template <typename Type>
	Type* MemAllocStruct(void* userData = NULL) {

		Type* rv = reinterpret_cast<Type*>(MemAlloc(sizeof(Type)));

		return rv;
	}

	template <typename Type>
	void* MemGetUserData(Type* addr) {

		return NULL;
	}
	void ShowAllLeaks();
	void* MemFree(void* addr);
	int MemGetSize(void* addr);
	void* MemReAlloc(void* addr, unsigned int size);

	void* FileOpenFileEx(void* hMpq, const char* szFileName, unsigned int dwSearchScope, void** phFile);
	unsigned int FileGetFileSize(void* hFile, unsigned int* pdwFileSizeHigh);
	bool FileReadFile(void* hFile, void* lpBuffer, unsigned int dwToRead, unsigned int* pdwRead, LPOVERLAPPED lpOverlapped);
	bool FileCloseFile(void* hFile);
	unsigned short FileGetLocale();

	void Init(void* hModule);
	void FreeAllMemory();
}
