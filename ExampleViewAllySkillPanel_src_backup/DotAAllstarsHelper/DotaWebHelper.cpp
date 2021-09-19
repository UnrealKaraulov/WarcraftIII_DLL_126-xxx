#include "Main.h"
//#include "HttpClass.h"
#include "base64.h"

#include <WinInet.h>
#pragma comment ( lib, "Wininet.lib" ) 

#undef BOOLAPI
#undef SECURITY_FLAG_IGNORE_CERT_DATE_INVALID
#undef SECURITY_FLAG_IGNORE_CERT_CN_INVALID
#define URL_COMPONENTS URL_COMPONENTS_ANOTHER
#define URL_COMPONENTSA URL_COMPONENTSA_ANOTHER
#define URL_COMPONENTSW URL_COMPONENTSW_ANOTHER
#define LPURL_COMPONENTS LPURL_COMPONENTS_ANOTHER
#define LPURL_COMPONENTSA LPURL_COMPONENTS_ANOTHER
#define LPURL_COMPONENTSW LPURL_COMPONENTS_ANOTHER
#define INTERNET_SCHEME INTERNET_SCHEME_ANOTHER
#define LPINTERNET_SCHEME LPINTERNET_SCHEME_ANOTHER
#define HTTP_VERSION_INFO HTTP_VERSION_INFO_ANOTHER
#define LPHTTP_VERSION_INFO LPHTTP_VERSION_INFO_ANOTHER
#include "WinHttpClient.h"
#undef URL_COMPONENTS
#undef URL_COMPONENTSA
#undef URL_COMPONENTSW
#undef LPURL_COMPONENTS
#undef LPURL_COMPONENTSA
#undef LPURL_COMPONENTSW
#undef INTERNET_SCHEME
#undef LPINTERNET_SCHEME
#undef HTTP_VERSION_INFO
#undef LPHTTP_VERSION_INFO


//

int DownProgress = 0, DownStatus = 0;
string LatestDownloadedString;


bool ProgressProc( double progress )
{
	wprintf( L"Current progress: %-.1f%%\r\n", progress );
	return true;
}


string SendHttpPostRequest( const char * ulr, const char * data )
{
	if ( !ulr || ulr[ 0 ] == '\0' || !data )
		return "";

	string BuildedPath = ulr;
	WinHttpClient client( StringToWString( BuildedPath.c_str( ) ), ProgressProc );
	string postdata = data;
	client.SetAdditionalDataToSend( ( unsigned char * )postdata.c_str( ), postdata.size( ) );
	wchar_t szSize[ 50 ] = L"";
	swprintf_s( szSize, L"%d", postdata.size( ) );
	wstring headers = L"Content-Length: ";
	headers += szSize;
	headers += L"\r\nContent-Type: application/x-www-form-urlencoded\r\n";
	client.SetAdditionalRequestHeaders( headers );
	client.SendHttpRequest( L"POST" );
	if ( !client.GetLastError( ) )
	{
		DownStatus = 1;
		return WStringToString( client.GetResponseContent( ).c_str( ) );
	}
	DownStatus = -1;
	return "";
}

string SendHttpGetRequest( const char * host, const char * path )
{
	if ( !host || host[ 0 ] == '\0' || !path )
		return "";

	string BuildedPath = string( host ) + string( path );

	WinHttpClient client( StringToWString( BuildedPath.c_str( ) ) );

	client.SendHttpRequest( );

	if ( !client.GetLastError( ) )
	{
		DownStatus = 1;
		return  WStringToString( client.GetResponseContent( ).c_str( ) );
	}
	DownStatus = -1;
	return "";
}


void DownloadNewMapToFile( const char* szUrl, const char * filepath )
{
	DownStatus = 0;
	HINTERNET hOpen = NULL;
	HINTERNET hFile = NULL;
	DWORD dataSize = 0;
	DWORD dwBytesRead = 0;
	vector<unsigned char> OutData;
	FILE * outfile = NULL;
	BOOL AllOkay = FALSE;

	if ( filepath == NULL || filepath[ 0 ] == '\0' || FileExist( filepath ) )
	{
		DownStatus = 2;
		return;
	}

	hOpen = InternetOpenA( "Microsoft Internet Explorer", NULL, NULL, NULL, 0 );
	if ( !hOpen )
	{
		DownStatus = -1;
		return;
	}
	DownProgress = 10;
	hFile = InternetOpenUrlA( hOpen, szUrl, NULL, 0, INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE, 0 );

	if ( !hFile )
	{
		InternetCloseHandle( hOpen );
		DownStatus = -1;
		return;
	}
	DownProgress = 20;
	int code = 0;
	DWORD codeLen = 4;
	HttpQueryInfo( hFile, HTTP_QUERY_STATUS_CODE |
		HTTP_QUERY_FLAG_NUMBER, &code, &codeLen, 0 );

	if ( code != HTTP_STATUS_OK )// 200 OK
	{
		InternetCloseHandle( hFile );
		InternetCloseHandle( hOpen );
		DownStatus = -1;
		return;
	}

	unsigned int sizeBuffer = 0;
	DWORD length = sizeof( sizeBuffer );
	HttpQueryInfo( hFile, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, &sizeBuffer, &length, NULL );



	DownProgress = 30;

	do
	{
		dataSize += dwBytesRead;
		if ( sizeBuffer != 0 )
			DownProgress = ( int )( ( dataSize * 100 ) / sizeBuffer );

		dwBytesRead = 0;
		unsigned char buffer[ 2000 ];
		BOOL isRead = InternetReadFile( hFile, ( LPVOID )buffer, _countof( buffer ), &dwBytesRead );
		if ( dwBytesRead && isRead )
		{
			AllOkay = TRUE;
			for ( unsigned int i = 0; i < dwBytesRead; i++ )
				OutData.push_back( buffer[ i ] );
		}
		else
			break;
	} while ( dwBytesRead );

	if ( DownProgress == 30 )
	{
		DownProgress = 70;
	}

	if ( OutData.size( ) > 0 && AllOkay )
	{
		fopen_s( &outfile, filepath, "wb" );
		if ( outfile != NULL )
		{
			fwrite( &OutData[ 0 ], OutData.size( ), 1, outfile );
			OutData.clear( );
			fclose( outfile );
			DownStatus = 1;
		}
		else DownStatus = -1;
	}
	else DownStatus = -1;


	if ( DownProgress == 70 )
	{
		DownProgress = 100;
	}

	InternetCloseHandle( hFile );
	InternetCloseHandle( hOpen );

	return;
}

std::string _addr = "";
std::string _request = "";
std::string _filepath = "";

unsigned long __stdcall SENDGETREQUEST( LPVOID )
{
	try
	{
		LatestDownloadedString = SendHttpGetRequest( _addr.c_str( ), _request.c_str( ) );
		_addr = "";
		_request = "";
	}
	catch ( ... )
	{
		DownStatus = -1;
	}
	return 0;
}


unsigned long __stdcall SENDPOSTREQUEST( LPVOID )
{
	try
	{
		LatestDownloadedString = SendHttpPostRequest( _addr.c_str( ), _request.c_str( ) );
		_addr = "";
		_request = "";
	}
	catch ( ... )
	{
		DownStatus = -1;
	}
	return 0;
}



unsigned long __stdcall SENDSAVEFILEREQUEST( LPVOID )
{
	DownloadNewMapToFile( _addr.c_str( ), _filepath.c_str( ) );
	_addr = "";
	_request = "";
	_filepath = "";
	return 0;
}

int __stdcall SendGetRequest( const char * url, const  char * request )
{
	DownProgress = 0;
	_addr = url; _request = request;
	DownStatus = 0;
	CloseHandle( CreateThread( 0, 0, SENDGETREQUEST, 0, 0, 0 ) );
	return 0;
}


int __stdcall SendPostRequest( const char * url, const  char * request )
{
	DownProgress = 0;
	_addr = url ? url : ""; _request = request ? request : "";
	DownStatus = 0;
	CloseHandle( CreateThread( 0, 0, SENDPOSTREQUEST, 0, 0, 0 ) );
	return 0;
}

int __stdcall SaveNewDotaVersionFromUrl( const  char * addr, const  char * filepath )
{
	DownProgress = 0;
	_addr = addr ? addr : ""; _filepath = filepath ? filepath : "";
	DownStatus = 0;
	CloseHandle( CreateThread( 0, 0, SENDSAVEFILEREQUEST, 0, 0, 0 ) );
	return 0;
}

int __stdcall GetDownloadStatus( int )
{
	return DownStatus;
}

int __stdcall GetDownloadProgress( int )
{
	return DownProgress;
}

const char * __stdcall GetLatestDownloadedString( int )
{
	return LatestDownloadedString.length( ) > 1023 ?
		string( LatestDownloadedString.begin( ), LatestDownloadedString.begin( ) + 1023 ).c_str( )
		: LatestDownloadedString.c_str( );
}

//
//std::experimental::filesystem::file_time_type GetLastFileTime( const std::vector<string> & files )
//{
//	std::experimental::filesystem::file_time_type retval = std::experimental::filesystem::file_time_type( );
//	std::string file = files
//	for ( auto s : files )
//	{
//		auto curfiletime = std::experimental::filesystem::last_write_time( s );
//		if ( curfiletime > retval )
//		{
//			retval = curfiletime;
//		}
//	}
//
//}

bool IsOkayLogFile( string file )
{
	if ( fs::path( file ).extension( ).string( ) == ".txt" )
	{
		string filedata = ToLower( GetFileContent( file ) );
		if ( filedata.length( ) > 0 )
		{
			if ( strstr( filedata.c_str( ), ToLower( "DotAAllstarsHelper" ).c_str( ) ) )
			{
				return true;
			}
			if ( strstr( filedata.c_str( ), ToLower( "DotA Allstars" ).c_str( ) ) )
			{
				return true;
			}
		}
	}
	return false;
}


int __stdcall SendLatestError( const char * url )
{
	auto Errors = get_file_list( "Errors", true );
	std::string sendfilename = "Errors\\lastcheck.txt";
	auto LatestError = FileExist( sendfilename.c_str( ) ) ?
		std::experimental::filesystem::last_write_time( sendfilename ) : std::experimental::filesystem::file_time_type( );
	bool foundlatesterror = false;

	if ( SetInfoObjDebugVal )
	{
		if ( FileExist( sendfilename.c_str( ) ) )
		{
			PrintText( "Found lastcheck.txt" );
		}
		else
		{
			PrintText( "lastcheck.txt not found" );
		}
	}

	for ( auto s : Errors )
	{

		if ( IsOkayLogFile( s ) )
		{
			if ( SetInfoObjDebugVal )
			{
				PrintText( "Process file:" + s );
			}
			auto curfiletime = std::experimental::filesystem::last_write_time( s );
			if ( curfiletime > LatestError )
			{
				if ( SetInfoObjDebugVal )
				{
					PrintText( "Found new file!" );
				}
				LatestError = curfiletime;
				sendfilename = s;
				foundlatesterror = true;
			}
		}
	}

	if ( foundlatesterror )
	{
		if ( SetInfoObjDebugVal )
		{
			PrintText( "Found error: " + sendfilename );
		}
		string filedata = GetFileContent( sendfilename );
		string encodedfiledata;
		if ( !Base64::Encode( filedata, &encodedfiledata ) )
		{
			SendPostRequest( url, encodedfiledata.c_str( ) );
			if ( SetInfoObjDebugVal )
			{
				PrintText( "Error success sent!" );
			}
		}
		else
		{
			if ( SetInfoObjDebugVal )
			{
				PrintText( "Error send file! Encode Base64 problem." );
			}
		}
	}
	else
	{
		if ( SetInfoObjDebugVal )
		{
			PrintText( "Error not found!" );
		}
	}

	ofstream newFile( "Errors\\lastcheck.txt" );
	newFile << " Just empty file ";
	newFile.flush( );
	newFile.close( );

	return 1;
}
