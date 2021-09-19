#pragma once
#include "Common.h"

class CCrc32Dynamic
{
public:
	CCrc32Dynamic( );
	virtual ~CCrc32Dynamic( );

	void Init( void );
	void Free( void );

	DWORD StringCrc32( char * szString, DWORD &dwCrc32 ) const;
	DWORD FileCrc32Streams( char * szFilename, DWORD &dwCrc32 ) const;
	DWORD FileCrc32Win32( char * szFilename, DWORD &dwCrc32 ) const;
	DWORD FileCrc32Filemap( char * szFilename, DWORD &dwCrc32 ) const;
	DWORD FileCrc32Assembly( char * szFilename, DWORD &dwCrc32 ) const;

protected:
	static BOOL GetFileSizeQW( const HANDLE hFile, QWORD &qwSize );
	inline void CalcCrc32( const unsigned char, DWORD &dwCrc32 ) const;

	DWORD *m_pdwCrc32Table;
};

