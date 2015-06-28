#include <cstdio>

#ifndef __linux__
#include "stdafx.h"
#endif

#include "TP3Server.h"
int main(int argc, char*argv[]) {

	if (argc < 2) {
		argv[1] = (char*) "27015";
		/*printf("Usage:%s port\n", argv[0]);
		return 0;*/
	}
	TP3Server server(argv[1]);
	printf("Server: Server Started\n");
	server.run();
	return 0;
}