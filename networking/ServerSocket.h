#ifndef _SERVER_SOCKET_H_
#define _SERVER_SOCKET_H_

#ifndef __linux__

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#pragma comment(lib, "Ws2_32.lib")
#else
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#endif

#include <string>
#include <cstdio>
#include <iostream>
#include <memory>

#include "Networking.h"
#include "ClientSocket.h"

#define DEFAULT_PORT "27015"

class ServerSocket {
	struct addrinfo *addrinfo_, hints_;
#ifndef __linux
	WSADATA wsaData_;
	SOCKET socket_;
#else
	int socket_;
#endif
	bool isConnected_;
protected:
	ServerSocket();
	bool init(const std::string& port = DEFAULT_PORT);
public:
	static ServerSocket& create(const std::string& port = DEFAULT_PORT);
	virtual ~ServerSocket();

	std::shared_ptr<ClientSocket> acceptClient();
	//bool write(std::string msg);
	//std::string read();
};

#endif