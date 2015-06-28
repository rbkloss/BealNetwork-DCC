#include "ClientSocket.h"

ClientSocket::ClientSocket() {
	Networking::init();
	isClosed_ = false;
	addrinfo_ = NULL;
#ifndef __linux__
	ZeroMemory(&hints, sizeof(hints));
#else
	bzero(&addrinfo_, sizeof(sockaddr_in));
#endif
	hints.ai_family = AF_INET6;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_V4MAPPED;
	isConnected_ = false;
}

ClientSocket::ClientSocket(SOCKET& socket, const std::string &hostName, const std::string& servName) {
	socket_ = socket;
	addrinfo_ = NULL;
#ifndef __linux__
	ZeroMemory(&hints, sizeof(hints));
#else
	bzero(&addrinfo_, sizeof(sockaddr_in));
#endif
	hints.ai_family = AF_INET6;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_V4MAPPED;
	isConnected_ = false;

	hostName_ = hostName;
	serv_ = servName;

}

ClientSocket::~ClientSocket() {
	if (!isClosed_) {
#ifndef __linux__
		closesocket(socket_);
#else
		close(socket_);
#endif		
	}
}

bool ClientSocket::connectTo(const std::string &address, const std::string &port) {
	struct addrinfo* ptr = NULL;
	if (isConnected_) {
		printf("Close the existing connection first!\n");
		return false;
	}

	auto errorCode = getaddrinfo(address.c_str(), port.c_str(), &hints, &addrinfo_);
	if (errorCode != 0) {
#ifndef __linux__
		printf("getAddrinfo Failed[%d]\n", WSAGetLastError());
#else
		//fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(socket));
#endif
		return false;
	}
	bool connected = false;
	for (ptr = addrinfo_; ptr != NULL; ptr = ptr->ai_next) {
		connected = true;
		socket_ = INVALID_SOCKET;
		socket_ = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);


		if (socket_ == INVALID_SOCKET) {
#ifndef __linux__
			printf("Socket could not connect, error Code %ld\n", WSAGetLastError());
#else
			perror("Error creating Socket\n");
#endif
			connected = false;
		}
		//connection per se:

		if (connect(socket_, ptr->ai_addr, (int)ptr->ai_addrlen) == SOCKET_ERROR) {
#ifndef __linux
			printf(" Error : Could not connect to server\n Error Code %ld\n", WSAGetLastError());
			socket_ = INVALID_SOCKET;
#else
			perror("Error : Could not connect to server\n");
#endif
			connected = false;
		}
		if (connected)
			break;
	}
	if (!connected) return false;
	char host[1024], serv[1024];
	if (getnameinfo(addrinfo_->ai_addr, (int)addrinfo_->ai_addrlen, host, sizeof(host), serv, sizeof(serv), NI_NUMERICHOST | NI_NUMERICSERV) != 0) {
		perror("getnameinfo failed:");
		exit(-1);
	}
	printf("Client: Connected to host [%s] @ [%s]\n", (char*)host, (char*)serv);
	hostName_ = host;
	serv_ = serv;
	return true;
}

bool ClientSocket::write(std::string msg) {
	auto buf = msg.c_str();
	auto len = strlen(buf);
	if (send(socket_, buf, len, 0) == SOCKET_ERROR) {
#ifndef __linux
		printf("Write failed%d\n", WSAGetLastError());
#else
		perror("Write failed\n");
#endif
		return false;
	}
	return true;
}

std::string ClientSocket::read() {
	std::string ans;
	char buffer[1024];
	strcpy(buffer, "");
	int bytesReceived = recv(socket_, buffer, 1024, 0);
	if (bytesReceived == 0) {
		printf("Client: connection closed\n");
	} else if (bytesReceived < 0) {
#ifndef __linux__
		auto error = WSAGetLastError();
		printf("Error code [%d]\n", error);
#else
		perror("Error on receiving attempt\n");
#endif
	} else {
		buffer[bytesReceived] = '\0';
	}
	ans = buffer;
	return ans;
}

std::string ClientSocket::getAddr() {
	return hostName_;
}
std::string ClientSocket::getService(){
  return serv_;
}

void ClientSocket::release() {
	shutdown(socket_, SD_SEND);
	isClosed_ = true;
}