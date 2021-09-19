#include "Crc32Dynamic.h"




//***********************************************
CCrc32Dynamic::CCrc32Dynamic( ) : m_pdwCrc32Table( NULL )
{
}

//***********************************************
CCrc32Dynamic::~CCrc32Dynamic( )
{
	Free( );
}

//***********************************************
void CCrc32Dynamic::Init( void )
{
	// This is the official polynomial used by CRC32 in PKZip.
	// Often times the polynomial shown reversed as 0x04C11DB7.
	DWORD dwPolynomial = 0xEDB88320;
	DWORD i, j;

	Free( );
	m_pdwCrc32Table = new DWORD[ 256 ];

	DWORD dwCrc;
	for ( i = 0; i < 256; i++ )
	{
		dwCrc = i;
		for ( j = 8; j > 0; j-- )
		{
			if ( dwCrc & 1 )
				dwCrc = ( dwCrc >> 1 ) ^ dwPolynomial;
			else
				dwCrc >>= 1;
		}
		m_pdwCrc32Table[ i ] = dwCrc;
	}
}

//***********************************************
void CCrc32Dynamic::Free( void )
{
	delete [] m_pdwCrc32Table;
	m_pdwCrc32Table = NULL;
}

//***********************************************
inline void CCrc32Dynamic::CalcCrc32( const unsigned char byte, DWORD &dwCrc32 ) const
{
	dwCrc32 = ( ( dwCrc32 ) >> 8 ) ^ m_pdwCrc32Table[ ( unsigned char )byte ^ ( ( dwCrc32 ) & 0x000000FF ) ];
}

//***********************************************
BOOL CCrc32Dynamic::GetFileSizeQW( const HANDLE hFile, QWORD &qwSize )
{
	_ASSERTE( hFile != INVALID_HANDLE_VALUE );

	BOOL bSuccess = TRUE;

	try
	{
		DWORD dwLo = 0, dwHi = 0;
		dwLo = GetFileSize( hFile, &dwHi );

		if ( dwLo == INVALID_FILE_SIZE && GetLastError( ) != NO_ERROR )
		{
			bSuccess = FALSE;
			qwSize = 0;
		}
		else
		{
			qwSize = MAKEQWORD( dwHi, dwLo );
		}
	}
	catch ( ... )
	{
		bSuccess = FALSE;
	}

	return bSuccess;
}

//***********************************************
DWORD CCrc32Dynamic::StringCrc32( char * szString, DWORD &dwCrc32 ) const
{
	_ASSERTE( szString );

	DWORD dwErrorCode = NO_ERROR;

	dwCrc32 = 0xFFFFFFFF;

	try
	{
		// Is the table initialized?
		if ( m_pdwCrc32Table == NULL )
			throw 0;

		while ( *szString != '\0' )
		{
			CalcCrc32( ( unsigned char )*szString, dwCrc32 );
			szString++;
		}
	}
	catch ( ... )
	{
		// An unknown exception happened, or the table isn't initialized
		dwErrorCode = ERROR_CRC;
	}

	dwCrc32 = ~dwCrc32;

	return dwErrorCode;
}

//***********************************************
DWORD CCrc32Dynamic::FileCrc32Streams( char * szFilename, DWORD &dwCrc32 ) const
{
#if UNICODE || _UNICODE
	return ERROR_NOT_SUPPORTED;
#else
	_ASSERTE( szFilename );
	_ASSERTE( lstrlen( szFilename ) );

	DWORD dwErrorCode = NO_ERROR;
	ifstream file;

	dwCrc32 = 0xFFFFFFFF;

	try
	{
		// Is the table initialized?
		if ( m_pdwCrc32Table == NULL )
			throw 0;

		// Open the file
		file.open( szFilename, ios::in | ios::binary );
		if ( !file.is_open( ) )
			dwErrorCode = ( DWORD )file.fail( );
		else
		{
			char buffer[ MAX_BUFFER_SIZE ];
			int nLoop, nCount;
			nCount = ( int )file.read( buffer, sizeof( buffer ) ).gcount( );
			while ( nCount )
			{
				for ( nLoop = 0; nLoop < nCount; nLoop++ )
					CalcCrc32( ( unsigned char )buffer[ nLoop ], dwCrc32 );
				nCount = ( int )file.read( buffer, sizeof( buffer ) ).gcount( );
			}

			file.close( );
		}
	}
	catch ( ... )
	{
		// An unknown exception happened, or the table isn't initialized
		dwErrorCode = ERROR_CRC;
	}

	if ( file.is_open( ) ) file.close( );

	dwCrc32 = ~dwCrc32;

	return dwErrorCode;
#endif
}

//***********************************************
DWORD CCrc32Dynamic::FileCrc32Win32( char * szFilename, DWORD &dwCrc32 ) const
{
	DWORD dwErrorCode = NO_ERROR;
	HANDLE hFile = NULL;

	dwCrc32 = 0xFFFFFFFF;

	try
	{
		// Is the table initialized?
		if ( m_pdwCrc32Table == NULL )
			throw 0;

		// Open the file
		hFile = CreateFileA( szFilename,
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_SYSTEM | FILE_FLAG_SEQUENTIAL_SCAN,
			NULL );
		if ( hFile == INVALID_HANDLE_VALUE )
			dwErrorCode = GetLastError( );
		else
		{
			unsigned char buffer[ MAX_BUFFER_SIZE ];
			DWORD dwBytesRead, dwLoop;
			BOOL bSuccess = ReadFile( hFile, buffer, sizeof( buffer ), &dwBytesRead, NULL );
			while ( bSuccess && dwBytesRead )
			{
				for ( dwLoop = 0; dwLoop < dwBytesRead; dwLoop++ )
					CalcCrc32( buffer[ dwLoop ], dwCrc32 );
				bSuccess = ReadFile( hFile, buffer, sizeof( buffer ), &dwBytesRead, NULL );
			}
		}
	}
	catch ( ... )
	{
		// An unknown exception happened, or the table isn't initialized
		dwErrorCode = ERROR_CRC;
	}

	if ( hFile != NULL ) CloseHandle( hFile );

	dwCrc32 = ~dwCrc32;

	return dwErrorCode;
}

//***********************************************
DWORD CCrc32Dynamic::FileCrc32Filemap( char * szFilename, DWORD &dwCrc32 ) const
{
	DWORD dwErrorCode = NO_ERROR;
	HANDLE hFile = NULL, hFilemap = NULL;

	dwCrc32 = 0xFFFFFFFF;

	try
	{
		// Is the table initialized?
		if ( m_pdwCrc32Table == NULL )
			throw 0;

		// Open the file
		hFile = CreateFileA( szFilename,
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_SYSTEM | FILE_FLAG_SEQUENTIAL_SCAN,
			NULL );
		if ( hFile == INVALID_HANDLE_VALUE )
			dwErrorCode = GetLastError( );
		else
		{
			QWORD qwFileSize = 0, qwFileOffset = 0;
			DWORD dwByteCount, dwViewSize;
			DWORD dwBaseAddress;

			// Get the file size
			if ( !GetFileSizeQW( hFile, qwFileSize ) )
				dwErrorCode = ERROR_BAD_LENGTH;
			else if ( qwFileSize != 0 )	// We cannot CRC zero unsigned char files
			{
				// Create the file mapping
				hFilemap = CreateFileMapping( hFile,
					NULL,
					PAGE_READONLY,
					0,
					0,
					NULL );
				if ( hFilemap == NULL )
					dwErrorCode = GetLastError( );
				else
				{
					LPBYTE pByte;

					// Loop while we map a section of the file and CRC it
					while ( qwFileSize > 0 )
					{
						if ( qwFileSize < MAX_VIEW_SIZE )
							dwViewSize = LODWORD( qwFileSize );
						else
							dwViewSize = MAX_VIEW_SIZE;

						dwBaseAddress = ( DWORD )MapViewOfFile( hFilemap,
							FILE_MAP_READ,
							HIDWORD( qwFileOffset ),
							LODWORD( qwFileOffset ),
							dwViewSize );

						dwByteCount = dwViewSize;
						pByte = ( LPBYTE )dwBaseAddress;

						if ( dwBaseAddress > 0 )
						{
							while ( dwByteCount-- > 0 )
							{
								CalcCrc32( *pByte, dwCrc32 );
								pByte++;
							}

							UnmapViewOfFile( ( LPVOID )dwBaseAddress );
						}
						qwFileOffset += dwViewSize;
						qwFileSize -= dwViewSize;
					}
				}
			}
		}
	}
	catch ( ... )
	{
		// An unknown exception happened, or the table isn't initialized
		dwErrorCode = ERROR_CRC;
	}

	if ( hFile != NULL ) CloseHandle( hFile );
	if ( hFilemap != NULL ) CloseHandle( hFilemap );

	dwCrc32 = ~dwCrc32;

	return dwErrorCode;
}

//***********************************************
DWORD CCrc32Dynamic::FileCrc32Assembly( char * szFilename, DWORD &dwCrc32 ) const
{
	DWORD dwErrorCode = NO_ERROR;
	HANDLE hFile = NULL;

	dwCrc32 = 0xFFFFFFFF;

	try
	{
		// Is the table initialized?
		if ( m_pdwCrc32Table == NULL )
			throw 0;

		// Open the file
		hFile = CreateFileA( szFilename,
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_SYSTEM | FILE_FLAG_SEQUENTIAL_SCAN,
			NULL );
		if ( hFile == INVALID_HANDLE_VALUE )
			dwErrorCode = GetLastError( );
		else
		{
			unsigned char buffer[ MAX_BUFFER_SIZE ];
			DWORD dwBytesRead;
			BOOL bSuccess = ReadFile( hFile, buffer, sizeof( buffer ), &dwBytesRead, NULL );
			while ( bSuccess && dwBytesRead )
			{
				// Register use:
				//		eax - CRC32 value
				//		ebx - a lot of things
				//		ecx - CRC32 value
				//		edx - Address of end of buffer
				//		esi - Address of start of buffer
				//		edi - CRC32 table
				__asm
				{
					// Save the esi and edi registers
					push esi
					push edi

					mov eax, dwCrc32			// Load the pointer to dwCrc32
					mov ecx, [ eax ]				// Dereference the pointer to load dwCrc32

					mov ebx, this				// Load the CRC32 table
					mov edi, [ ebx ]CCrc32Dynamic.m_pdwCrc32Table

					lea esi, buffer				// Load buffer
					mov ebx, dwBytesRead		// Load dwBytesRead
					lea edx, [ esi + ebx ]		// Calculate the end of the buffer

					crc32loop:
					xor eax, eax				// Clear the eax register
						mov bl, byte ptr[ esi ]		// Load the current source unsigned char

						mov al, cl					// Copy crc value into eax
						inc esi						// Advance the source pointer

						xor al, bl					// Create the index into the CRC32 table
						shr ecx, 8

						mov ebx, [ edi + eax * 4 ]	// Get the value out of the table
						xor ecx, ebx				// xor with the current unsigned char

						cmp edx, esi				// Have we reached the end of the buffer?
						jne crc32loop

						// Restore the edi and esi registers
						pop edi
						pop esi

						mov eax, dwCrc32			// Load the pointer to dwCrc32
						mov[ eax ], ecx				// Write the result
				}
				bSuccess = ReadFile( hFile, buffer, sizeof( buffer ), &dwBytesRead, NULL );
			}
		}
	}
	catch ( ... )
	{
		// An unknown exception happened, or the table isn't initialized
		dwErrorCode = ERROR_CRC;
	}

	if ( hFile != NULL ) CloseHandle( hFile );

	dwCrc32 = ~dwCrc32;

	return dwErrorCode;
}
