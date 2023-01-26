#include "Main.h"
#pragma warning(push, 3)
#include <ws2tcpip.h>
#pragma warning(pop)
#include "HttpClass.h"

HTTPRequest::HTTPRequest(const std::string& host, u_short  port)
	: Host(host), Port(port)
{
	ErrorCode = 0;

	WSADATA WsaDat;

	if (WSAStartup(MAKEWORD(2, 2), &WsaDat) != 0)
	{
		ErrorCode = 1;
	}
	if (!ErrorCode)
	{
		if ((Sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
		{
			ErrorCode = 2;
		}
	}
}

HTTPRequest::~HTTPRequest()
{
	closesocket(Sock);
	WSACleanup();
}


std::string HTTPRequest::get_response()
{
	return Response;
}


int HTTPRequest::GetErrorCode()
{
	return ErrorCode;
}

int HTTPRequest::loop_recieve()
{
	if (!Response.empty())
		Response.clear();
	while (true)
	{
		char recvBuf[256];

		int nret = recv(Sock, recvBuf, sizeof(recvBuf), 0);
		if (nret == -1)
		{
			return false;
		}
		else if (nret == 0)
		{
			break;
		}

		Response.append(recvBuf, (unsigned int)nret);
	}

	return true;
}

int HTTPRequest::resolve_and_connect()
{
	int ret = false;

	ADDRINFO hints = { 0 };
	hints.ai_flags = AI_ALL;
	hints.ai_family = PF_INET;
	hints.ai_protocol = IPPROTO_IPV4;

	ADDRINFO* pResult = nullptr;
	if (getaddrinfo(Host.c_str(), nullptr, &hints, &pResult) || !pResult)
	{
		return false;
	}

	sockaddr_in servAddr = { 0 };
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(Port);

	for (size_t i = 0; i < pResult->ai_addrlen; i++)
	{
		if (&pResult->ai_addr[i])
		{
			servAddr.sin_addr.S_un.S_addr = (ULONG)((sockaddr_in*)&pResult->ai_addr[i])->sin_addr.S_un.S_addr;

			if (connect(Sock, (sockaddr*)&servAddr, sizeof(servAddr)) != SOCKET_ERROR)
			{
				ret = true;
				break;
			}
		}
		else
			return false;
	}

	freeaddrinfo(pResult);

	return ret;
}

int HTTPRequest::get_request(const std::string& path)
{
	if (!resolve_and_connect())
		return false;

	std::string request = "GET " + path + " HTTP/1.1" + "\r\n";
	request += "Host: " + Host + "\r\n";
	request += "Accept: */*\r\n";
	request += "Accept-Language: en-us\r\n";
	request += "User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:40.0) Gecko/20100101 Firefox/40.0\r\n";
	request += "Connection: close\r\n";
	request += "\r\n";

	if (send(Sock, request.c_str(), (int)request.length(), 0) == SOCKET_ERROR)
	{
		return false;
	}

	return loop_recieve();
}



int HTTPRequest::post_request(const std::string& path, const std::string& dat)
{
	if (!resolve_and_connect())
		return false;

	std::string request = "POST " + path + " HTTP/1.1" + "\r\n";
	request += "Host: " + Host + "\r\n";
	request += "Accept: */*\r\n";
	request += "Accept-Language: en-us\r\n";
	request += "User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:40.0) Gecko/20100101 Firefox/40.0\r\n";
	request += "Connection: close\r\n";
	request += "Content-Type: application/x-www-form-urlencoded\r\n";
	request += "Content-Length: ";
	request += std::to_string(dat.length());
	request += "\r\n\r\n";
	request += dat;
	request += "\r\n";

	if (send(Sock, request.c_str(), (int)request.length(), 0) == SOCKET_ERROR)
	{
		return false;
	}

	return loop_recieve();
}

