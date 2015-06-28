#ifndef _NETWORKING_H_
#define _NETWORKING_H_

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
#include <arpa/inet.h>

typedef int SOCKET;
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
#define SD_SEND 1
#endif

#include <string>
#include <cstdio>
#include <iostream>

class Networking {
#ifndef __linux__
	static WSADATA wsaData_;
	static Networking singleton_;
#endif
protected:
	Networking();
public:
	
	~Networking();

	static bool init();
	
	// get sockaddr, IPv4 or IPv6:
	static void *get_in_addr(struct sockaddr *sa);
};

#endif