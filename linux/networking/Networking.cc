#include "Networking.h"

#ifndef __linux__
WSADATA Networking::wsaData_;
Networking Networking::singleton_;
#endif

Networking::Networking() {}


Networking::~Networking() {
#ifndef __linux__
	WSACleanup();
#else
	/*char yes=1;
	if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
	perror("setsockopt");
	exit(1);
	}*/
#endif
}

bool Networking::init() {
#ifndef __linux__
	static int callCount = 0;
	if (callCount++) {
		return false;
	}
	auto errorCode = WSAStartup(MAKEWORD(2, 2), &wsaData_);
	if (errorCode != 0) {
		printf("Init Failed error [%d]\n", WSAGetLastError());
		exit(-1);
	}
#endif
	return true;
}

void* Networking::get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}