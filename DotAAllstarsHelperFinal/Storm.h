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
	/*n aero::generic_std_call<void*>(
			AddrMemFree,
			addr,
			"DotaMem",
			2,
			0
			); */
	void* __stdcall MemFree(void* addr, const char* sourcename = "delete", int sourceline = -1, unsigned int flags = 0);
	typedef void *(__stdcall* PROTOTYPE_MemFree)(void * addr, const char * sourcename, int sourceline, unsigned int flags);
	extern PROTOTYPE_MemFree MemFree_org;
	extern PROTOTYPE_MemFree MemFree_ptr;

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
	int MemGetSize(void* addr);
	void* MemReAlloc(void* addr, unsigned int size);

	void* FileOpenFileEx(void* hMpq, const char* szFileName, unsigned int dwSearchScope, void** phFile);
	unsigned int FileGetFileSize(void* hFile, unsigned int* pdwFileSizeHigh);
	bool FileReadFile(void* hFile, void* lpBuffer, unsigned int dwToRead, unsigned int* pdwRead, LPOVERLAPPED lpOverlapped);
	bool FileCloseFile(void* hFile);
	unsigned short FileGetLocale();

	void ClearAllLeaks();
	void Init(void* hModule);
	void Cleanup();
}
