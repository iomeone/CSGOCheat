#include "SecurityUtils.h"

#define _WINSOCKAPI_    // stops windows.h including winsock.h
#include <windows.h>

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <ws2def.h>
#include <intrin.h>
#include <iphlpapi.h>
#include <sstream>
#include <fstream>
#include <direct.h>
#include <tchar.h>
#include <urlmon.h>
#include <TlHelp32.h>
#include <fstream>
#include <iostream>
#include <algorithm>

#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "IPHLPAPI.lib")

#include "XorString\XorStr.hpp"

std::string SecurityUtils::username = "";

int IsProcessRunning(const char *processName)
{
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry))
	{
		while (Process32Next(snapshot, &entry))
		{
			if (!strcmp((char*)entry.szExeFile, processName))
			{
				return entry.th32ProcessID;
			}
		}
	}

	CloseHandle(snapshot);
	return -1;
}

#include "LoggerUtils.h"

void SecurityUtils::SetData()
{
	/*----------------------------------------------------------------------------------------------------------------------------------*/
	std::string web_domain = /*dominanced.com*/XorStr(14, 0x40, 0x24D053D4, 0x52DA54DA, 0x5DD318D6, 0x5BDE0000);
	std::string php_request = "launcher/get_cheat_user_php.php?sess=";
	std::string start_php = php_request + SecurityUtils::get_session_id();
	/*----------------------------------------------------------------------------------------------------------------------------------*/
	SecurityUtils::username = SecurityUtils::get_web_response(web_domain, start_php);
	Logger::SaveToLog( ("Username found = " + SecurityUtils::username).c_str() );
}

void SecurityUtils::check_guard()
{
	// Old
	if (IsProcessRunning(/*guardian.exe*/XorStr(12, 0x0D, 0x6AF96AF8, 0x6DE166E8, 0x2BE17BE7).c_str()))
	{
		/*Logger::SaveToLog("Guardian process was not found!");
		exit(-1);*/
	}
	if (!(SecurityUtils::username.length() > 0))
	{
		//exit(-1);
	}
}

std::string SecurityUtils::get_session_id()
{
	std::ifstream input(/*C:\\session_id*/XorStr(13, 0xB4, 0xF709EE42, 0xD55CDD44, 0xC345F540, 0xCC000000).c_str());
	for (std::string line; getline(input, line); )
	{
		return line;
	}
}

std::string SecurityUtils::get_web_response(std::string domain, std::string web_get)
{
	// Initialize Dependencies to the Windows Socket.
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		return "";
	}

	// We first prepare some "hints" for the "getaddrinfo" function
	// to tell it, that we are looking for a IPv4 TCP Connection.
	struct addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;          // We are targeting IPv4
	hints.ai_protocol = IPPROTO_TCP;    // We are targeting TCP
	hints.ai_socktype = SOCK_STREAM;    // We are targeting TCP so its SOCK_STREAM

										// Aquiring of the IPv4 address of a host using the newer
										// "getaddrinfo" function which outdated "gethostbyname".
										// It will search for IPv4 addresses using the TCP-Protocol.
	struct addrinfo* targetAdressInfo = NULL;
	DWORD getAddrRes = getaddrinfo(domain.c_str(), NULL, &hints, &targetAdressInfo);
	if (getAddrRes != 0 || targetAdressInfo == NULL)
	{
		return "";
		WSACleanup();
	}

	// Create the Socket Address Informations, using IPv4
	// We dont have to take care of sin_zero, it is only used to extend the length of SOCKADDR_IN to the size of SOCKADDR
	SOCKADDR_IN sockAddr;
	sockAddr.sin_addr = ((struct sockaddr_in*) targetAdressInfo->ai_addr)->sin_addr;    // The IPv4 Address from the Address Resolution Result
	sockAddr.sin_family = AF_INET;  // IPv4
	sockAddr.sin_port = htons(80);  // HTTP Port: 80

									// We have to free the Address-Information from getaddrinfo again
	freeaddrinfo(targetAdressInfo);

	// Creation of a socket for the communication with the Web Server,
	// using IPv4 and the TCP-Protocol
	SOCKET webSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (webSocket == INVALID_SOCKET)
	{
		return "";
		WSACleanup();
	}

	// Establishing a connection to the web Socket
	if (connect(webSocket, (SOCKADDR*)&sockAddr, sizeof(sockAddr)) != 0)
	{
		return "";
		closesocket(webSocket);
		WSACleanup();
	}

	// Sending a HTTP-GET-Request to the Web Server
	std::string formed_login_url = /*GET /*/XorStr(5, 0x0F, 0x48CB59AC, 0x24000000);
	formed_login_url += web_get;
	formed_login_url += /* HTTP/1.1\r\nHost: */XorStr(17, 0xBF, 0x9F76E968, 0xEB158816, 0x863BBF7C, 0xDC41C50A, 0x8F000000);
	formed_login_url += domain.c_str();
	formed_login_url += /*\r\nConnection: close\r\n\r\n*/XorStr(23, 0xFF, 0xF274BE13, 0x95149C1B, 0x831F9A1A, 0xC952921C, 0x801D8861, 0xE167E300);

	const char* httpRequest = formed_login_url.c_str();
	int sentBytes = send(webSocket, httpRequest, strlen(httpRequest), 0);
	if (sentBytes < strlen(httpRequest) || sentBytes == SOCKET_ERROR)
	{
		return "";
		closesocket(webSocket);
		WSACleanup();
	}

	// Receiving and Displaying an answer from the Web Server
	std::string response_b;
	std::string response;

	char ch;
	bool skipped = false;
	while (recv(webSocket, &ch, 1, 0) > 0)
	{
		if (response_b.find("UTF-8") != std::string::npos)
		{
			skipped = true;
		}
		
		if (skipped)
		{
			response += ch;
		}
		response_b += ch;
	}

	// Cleaning up Windows Socket Dependencies
	closesocket(webSocket);
	WSACleanup();

	if (response.length() > 14)
	{
		response.erase(response.end() - 7, response.end());
		response.erase(response.begin(), response.begin() + 7);
	}
	response.erase(std::remove(response.begin(), response.end(), '\n'), response.end());
	response.erase(std::remove(response.begin(), response.end(), '\0'), response.end());
	return response;
}