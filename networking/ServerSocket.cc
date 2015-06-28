#include "ServerSocket.h"
#include "Networking.h"

#include <vector>

ServerSocket::ServerSocket() {
	socket_ = INVALID_SOCKET;
	addrinfo_ = NULL;

}


ServerSocket::~ServerSocket() {
	if (socket_ != INVALID_SOCKET) {
#ifndef __linux
		closesocket(socket_);
#else
		close(socket_);
#endif
	}
}


ServerSocket& ServerSocket::create(const std::string& port) {
	static std::vector<ServerSocket> sockets;
	ServerSocket socket;
	sockets.push_back(socket);
	if (!sockets.back().init(port)) {
		exit(-1);
	}
	return sockets.back();
}

bool ServerSocket::init(const std::string& port) {
	Networking::init();
	struct addrinfo *ptr = NULL;
	addrinfo_ = NULL;
#ifndef __linux__
	ZeroMemory(&hints_, sizeof(hints_));
#else
	bzero(&hints_, sizeof(hints_));
	
#endif
	hints_.ai_family = AF_INET6;
	hints_.ai_socktype = SOCK_STREAM;
	hints_.ai_flags = AI_PASSIVE | AI_ADDRCONFIG | AI_V4MAPPED;

	auto errorCode = getaddrinfo(NULL, port.c_str(), &hints_, &addrinfo_);
	if (errorCode != 0) {
		printf("getAddrinfo Failed[%s]\n", gai_strerror(errorCode));
		return false;
	}

	bool binded = false;
	int yes = 1;
	for (ptr = addrinfo_; ptr != NULL; ptr = ptr->ai_next) {
		binded = true;
		socket_ = INVALID_SOCKET;
		socket_ = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

		if (socket_ == INVALID_SOCKET) {
#ifndef __linux__
			printf("Socket could not connect, error Code %ld\n", WSAGetLastError());
#else
			perror("Socket could not connect\n");
			if (setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &yes,
				sizeof(int)) == -1) {
				perror("setsockopt");
				exit(1);
		}
#endif
			binded = false;
	}

		if (bind(socket_, ptr->ai_addr, (int)ptr->ai_addrlen) == SOCKET_ERROR) {
#ifndef __linux__
			char errorMsg[512] = "";
			sprintf(errorMsg, "bind failed with error: %d\n", WSAGetLastError());
#else
			perror("bind failed with error\n");
#endif
			binded = false;
		}
		if (binded) break;
}
	if (!binded)return false;

	if (listen(socket_, SOMAXCONN) == SOCKET_ERROR) {
		perror("listenning error\n");
		exit(-1);
	}
	char host[1024], serv[1024];
	if (getnameinfo(addrinfo_->ai_addr, (int)addrinfo_->ai_addrlen, host, sizeof(host), serv, sizeof(serv), NI_NUMERICHOST) != 0) {
		perror("getnameinfo failed:");
		exit(-1);
	}
	printf("Server: binded to host [%s] @[%s]\n", host, serv);
	freeaddrinfo(addrinfo_);
	return true;
}



std::shared_ptr<ClientSocket> ServerSocket::acceptClient() {

	//acception per se:

	SOCKET client = INVALID_SOCKET;

	struct sockaddr_storage clientAddr;
	socklen_t sin_size = sizeof(clientAddr);
	printf("Server:Accepting . . .\n");
	client = accept(socket_, (struct sockaddr*) &clientAddr, &sin_size);
	if (client == INVALID_SOCKET) {
#ifndef __linux
		printf("accept failed: %d\n", WSAGetLastError());
#else
		perror("accept failed:");
#endif
		exit(-1);
	}
	char host[1024], serv[1024];
	if (getnameinfo((struct sockaddr*)&clientAddr, (int)sin_size, host, sizeof(host), serv, sizeof(serv), NI_NUMERICHOST) != 0) {
		perror("getnameinfo failed:");
		exit(-1);
	}
	printf("Server: Accepted client [%s] @ [%s]\n", host, serv);

	return std::make_shared<ClientSocket>(client, host, serv);
}