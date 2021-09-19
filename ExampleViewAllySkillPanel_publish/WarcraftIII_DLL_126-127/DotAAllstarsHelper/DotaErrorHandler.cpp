#include "Main.h"

#pragma optimize("",off)


#include <iomanip>
#include <eh.h>



std::string ReadRegistryKeyString( const char *registryKey, const char *registryValue )
{
	// Open the key
	HKEY hKey;
	if ( RegOpenKeyExA( HKEY_LOCAL_MACHINE, registryKey, 0, KEY_QUERY_VALUE, &hKey ) != ERROR_SUCCESS )
		return std::string( );

	char str[ 256 ] = {};
	DWORD dwLen = 255;
	LONG ret = RegQueryValueExA( hKey, registryValue, NULL, NULL, ( LPBYTE )str, &dwLen );
	RegCloseKey( hKey );

	if ( ret == ERROR_SUCCESS )
		return str;
	else
		return std::string( );
}

std::string LatestOsDisplayString;

std::string GetOSDisplayString( )
{
	std::string productName = ReadRegistryKeyString( "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\", "ProductName" );
	std::string servicePack = ReadRegistryKeyString( "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\", "CSDVersion" );
	std::string bitness = ReadRegistryKeyString( "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\", "BuildLabEx" );

	if ( productName.length( ) == 0 )
		productName = "Win98/2000?";

	if ( bitness.find( "amd64" ) != std::string::npos )
		bitness = "64-bit";
	else
		bitness = "32-bit";

	LatestOsDisplayString = productName + " " + bitness + " " + servicePack;
	return LatestOsDisplayString;
}

const char * __stdcall get_os_string( int )
{
	GetOSDisplayString( );

	return LatestOsDisplayString.c_str( );
}




LookupNative LookupNative_org = NULL;
LookupNative LookupNative_ptr;

int __fastcall LookupNative_my( int global, int unused, const char * funcname )
{
	if ( funcname && *funcname != '\0' )
	{

	}
	else
	{

	}
	int retval = LookupNative_ptr( global, unused, funcname );


	return retval;
}

LookupJassFunc LookupJassFunc_org = NULL;
LookupJassFunc LookupJassFunc_ptr;

signed int __fastcall LookupJassFunc_my( int global, int unused, const char * funcname )
{
	BOOL funcnamefound = FALSE;
	if ( funcname &&  *funcname != '\0' )
	{
		funcnamefound = TRUE;

	}
	else
	{

	}

	signed int retval = LookupJassFunc_ptr( global, unused, funcname );


	if ( retval == 0 )
	{
		if ( IsGame( ) )
		{

		}
	}

	return retval;
}

BOOL bDllLogEnable = TRUE;
int __stdcall DllLogEnable( BOOL enable )
{
	bDllLogEnable = enable;
	return 0;
}




std::string GetLastErrorAsString( )
{
	//Get the error message, if any.
	DWORD errorMessageID = ::GetLastError( );
	if ( errorMessageID == 0 )
		return std::string( ); //No error message has been recorded

	LPSTR messageBuffer = NULL;
	size_t size = FormatMessageA( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID( LANG_ENGLISH, SUBLANG_ENGLISH_US ), ( LPSTR )&messageBuffer, 0, NULL );

	if ( size > 2 && messageBuffer != NULL )
	{
		std::string message( messageBuffer, size );

		//Free the buffer.
		LocalFree( messageBuffer );

		return message;
	}
	return "NULL";
}



string url_encode( const string & value )
{
	ostringstream escaped;
	escaped.fill( '0' );
	escaped << hex;

	for ( string::const_iterator i = value.begin( ), n = value.end( ); i != n; ++i )
	{
		string::value_type c = ( *i );

		// Keep alphanumeric and other accepted characters intact
		/*if ( isalnum( c ) || c == '-' || c == '_' || c == '.' )
		{
		escaped << c;
		continue;
		}*/

		// Any other characters are percent-encoded
		escaped << uppercase;
		escaped << '%' << setw( 2 ) << int( ( unsigned char )c );
		escaped << nouppercase;
	}

	return escaped.str( );
}


#define MAX_PROCESSES 1024 


DWORD __stdcall FindProcess( const char * lpcszFileName )
{
	DWORD dwProcessId = 0;
	LPDWORD lpdwProcessIds;
	char *  lpszBaseName;
	HANDLE  hProcess;
	DWORD   i, cdwProcesses;


	lpdwProcessIds = ( LPDWORD )HeapAlloc( GetProcessHeap( ), 0, MAX_PROCESSES * sizeof( DWORD ) );
	if ( lpdwProcessIds != NULL )
	{
		if ( EnumProcesses( lpdwProcessIds, MAX_PROCESSES * sizeof( DWORD ), &cdwProcesses ) )
		{
			lpszBaseName = ( char * )HeapAlloc( GetProcessHeap( ), 0, MAX_PATH * sizeof( TCHAR ) );
			if ( lpszBaseName != NULL )
			{
				cdwProcesses /= sizeof( DWORD );
				for ( i = 0; i < cdwProcesses; i++ )
				{
					hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, lpdwProcessIds[ i ] );
					if ( hProcess != NULL )
					{
						if ( GetModuleBaseNameA( hProcess, NULL, lpszBaseName, MAX_PATH ) > 0 )
						{
							if ( !lstrcmpiA( lpszBaseName, lpcszFileName ) )
							{
								dwProcessId = lpdwProcessIds[ i ];
								CloseHandle( hProcess );
								break;
							}
						}
						CloseHandle( hProcess );
					}
				}
				HeapFree( GetProcessHeap( ), 0, ( LPVOID )lpszBaseName );
			}
		}
		HeapFree( GetProcessHeap( ), 0, ( LPVOID )lpdwProcessIds );
	}

	return dwProcessId;
}

std::string GetPlatformName( )
{
	if ( GetModuleHandleA( "iccwc3.icc" ) )
		return "[iCCup]";
	if ( GetModuleHandleA( "InputHook.dll" ) && GetModuleHandleA( "Overlay.dll" ) )
		return "[Garena Plus]";
	if ( FindProcess( "rgc.exe" ) )
		return "[RGC]";
	if ( GetModuleHandleA( "mroc.dll" ) || FindProcess( "myroc.exe" ) )
		return "[RGC]";
	if ( !GetModuleHandleA( "w3lh.dll" ) )
		return "[Unknown Or Battle.net]";

	return "[Unknown PVPGN server]";
}
string ConvertMemoryToHex( unsigned char * buffer, int size )
{
	stringstream ss;
	ss << std::hex << std::setfill( '0' );
	for ( int i = 0; i < size; ++i )
	{
		ss << std::setw( 2 ) << static_cast< unsigned >( buffer[ i ] );
	}
	return ss.str( );
}

string ConvertMemoryToHexReverse( unsigned char * buffer, int size )
{
	stringstream ss;
	ss << std::hex << std::setfill( '0' );
	for ( int i = size - 1; i >= 0; i-- )
	{
		ss << std::setw( 2 ) << static_cast< unsigned >( buffer[ i ] );
	}
	return ss.str( );
}

BOOL IsVEHex = FALSE;




int __stdcall JassLog( int )
{
	return 0;
}
int __stdcall StartExtraErrorHandler( int )
{
	return 0;
}

int __stdcall TraceEsp_Print( int )
{
	return 0;
}



LONG __stdcall DotaVectoredToSehHandler(_EXCEPTION_POINTERS *ExceptionInfo)
{
	if (IsVEHex)
	{
		return 0;
	}


	if (!ExceptionInfo)
	{
		cerr << "Test: error1" << endl;
		ExceptionInfo = new	_EXCEPTION_POINTERS();
	}
	if (!ExceptionInfo->ContextRecord)
	{
		cerr << "Test: error2" << endl;
		ExceptionInfo->ContextRecord = new CONTEXT();
	}

	if (!ExceptionInfo->ExceptionRecord)
	{
		cerr << "Test: error3" << endl;
		ExceptionInfo->ExceptionRecord = new EXCEPTION_RECORD();
	}


	PEXCEPTION_RECORD ex = ExceptionInfo->ExceptionRecord;

	DWORD exceptionCode = ExceptionInfo->ExceptionRecord->ExceptionCode;

	if ((exceptionCode & ERROR_SEVERITY_ERROR) != ERROR_SEVERITY_ERROR) {
		cerr << "Found ERROR_SEVERITY_ERROR..." << endl;
		return ExceptionContinueSearch;
	}


	if (exceptionCode & APPLICATION_ERROR_MASK) {
		cerr << "Found APPLICATION_ERROR_MASK..." << endl;
		return ExceptionContinueSearch;
	}


	char continueablecode[200];
	if ((ex->ExceptionFlags & EXCEPTION_NONCONTINUABLE) == 0)
	{
		sprintf_s(continueablecode, 200, "%s:%X:%s addr:%X gamedlladdr:%X", "Test: [VEH]ExceptionContinueExecution", ex->ExceptionCode, to_string(ex->ExceptionFlags & EXCEPTION_NONCONTINUABLE).c_str(),(unsigned int)ex->ExceptionAddress,GameDll);
		cerr << continueablecode << endl;
		return ExceptionContinueSearch;
	}



	IsVEHex = TRUE;


	sprintf_s(continueablecode, 200, "%s:%X:%s  addr:%X gamedlladdr:%X", "Test: [VEH]TopLevelExceptionFilter", ex->ExceptionCode, to_string(ex->ExceptionFlags & EXCEPTION_NONCONTINUABLE).c_str(), (unsigned int)ex->ExceptionAddress, GameDll);
	cerr << continueablecode << endl;

	return ExceptionContinueSearch;
}


LPTOP_LEVEL_EXCEPTION_FILTER OriginFilter = NULL;


void InitTopLevelExceptionFilter()
{
	//SetUnhandledExceptionFilter( 0 );
	//SetUnhandledExceptionFilter( TopLevelExceptionFilter );
	//AddVectoredExceptionHandler(0, DotaVectoredToSehHandler);
}

void ResetTopLevelExceptionFilter()
{
	//SetUnhandledExceptionFilter(OriginFilter);
}

void __stdcall DisableErrorHandler(int)
{
	//ResetTopLevelExceptionFilter();
}
void __stdcall EnableErrorHandler(int)
{
	//InitTopLevelExceptionFilter();

}