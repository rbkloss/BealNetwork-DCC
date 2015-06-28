#ifndef _SERVER_H_
#define _SERVER_H_

#include <cstdio>
#include <stdlib.h>
#include <string>
#include <vector>
#include <memory>
#include <functional>

#include "ServerSocket.h"
#include "ClientSocket.h"

class Server {
protected:
	ServerSocket& socket_;
	//	std::vector<std::reference_wrapper<ClientSocket>> clients_;
public:
	Server(const std::string& port);
	virtual ~Server();


	//accept the client and implement your logic here
	void run();
	virtual void onAccept(std::shared_ptr<ClientSocket> client) = 0;

};


#endif