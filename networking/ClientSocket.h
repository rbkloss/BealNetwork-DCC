#ifndef _CLIENT_SOCKET_H_
#define _CLIENT_SOCKET_H_



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

#include "Networking.h"
#define DEFAULT_PORT "27015"

class ClientSocket {
	SOCKET socket_;
#ifndef __linux__
	WSADATA wsaData_;
	struct addrinfo *addrinfo_, hints;
#else
	struct addrinfo *addrinfo_, hints;
	struct hostent *host_;

#endif
	std::string hostName_, serv_;
	bool isConnected_;
	bool isClosed_;
	
public:
	ClientSocket();
	ClientSocket(SOCKET &socket, const std::string &hostName, const std::string& servName);
	virtual ~ClientSocket();
	bool connectTo(const std::string &address, const std::string& port = DEFAULT_PORT);

	std::string read();
	bool write(std::string msg);

	void release();

	std::string getAddr();
  std::string ClientSocket::getService();


};

#endif