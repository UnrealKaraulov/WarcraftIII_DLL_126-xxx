#pragma once

class HTTPRequest
{
private:
	const std::string Host;
	u_short Port;
	int ErrorCode;
	SOCKET Sock;
	std::string Response;
	BOOL loop_recieve( );
	BOOL resolve_and_connect( );
public:
	HTTPRequest( const std::string& host, u_short  port );
	~HTTPRequest( );
	std::string get_response( );
	int GetErrorCode( );
	BOOL get_request( const std::string& path );
	BOOL post_request( const std::string& path, const std::string& dat );

};