#pragma once

namespace Storm {
	extern int AllocationCount;
	extern HMODULE StormModule;
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

	template <typename ReturnType>
	ReturnType Offset( uint32_t offset ) {
		return StormModule ? aero::pointer_calc<ReturnType>( StormModule, offset ) : NULL;
	}

	typedef BOOL( __stdcall *PROTOTYPE_FileArchiveOpen )( const char* mpqName, DWORD priority, DWORD flags, HANDLE *pMpqHandle );
	extern PROTOTYPE_FileArchiveOpen FileOpenArchive;

	typedef BOOL( __stdcall *PROTOTYPE_FileArchiveClose )( HANDLE mpqHandle );
	extern PROTOTYPE_FileArchiveClose FileCloseArchive;

	typedef uint32_t( __stdcall *PROTOTYPE_StringGetHash )( const char* str );
	extern PROTOTYPE_StringGetHash StringGetHash;

	void* MemAlloc( uint32_t size );

	template <typename Type>
	Type* MemAllocStruct( void* userData = NULL ) {

		Type* rv = reinterpret_cast< Type* >( MemAlloc( sizeof( Type ) ) );

		return rv;
	}

	template <typename Type>
	void* MemGetUserData( Type* addr ) {

		return NULL;
	}
	void ShowAllLeaks( );
	void* MemFree( void* addr );
	int MemGetSize( void* addr );
	void* MemReAlloc( void* addr, uint32_t size );

	HANDLE FileOpenFileEx( HANDLE hMpq, const char *szFileName, uint32_t dwSearchScope, HANDLE *phFile );
	uint32_t FileGetFileSize( HANDLE hFile, uint32_t *pdwFileSizeHigh );
	bool FileReadFile( HANDLE hFile, void* lpBuffer, uint32_t dwToRead, uint32_t* pdwRead, LPOVERLAPPED lpOverlapped );
	bool FileCloseFile( HANDLE hFile );
	LANGID FileGetLocale( );

	void Init( HMODULE hModule );
	void FreeAllMemory( );
}
