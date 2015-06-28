#ifndef __linux__
#include "stdafx.h"
#endif

#include "tp3Client.h"

bool experiments;

int main(int argc, char* argv[]) {
	
	if (argc < 4) {
		printf("Usage: bin address port path_to_directory_for_listing\n");
    argv[1] = const_cast<char*>("localhost");
    argv[2] = const_cast<char*>("27015");
    argv[3] = const_cast<char*>("id1");
		//return 0;
  } 
	
	TP3Client client(argv[3]);
	printf("Client Started\n");
	if (client.connectTo(argv[1], argv[2])) {
		printf("Client: Connection established\n");
		client.run();
	}


	return 0;
}

