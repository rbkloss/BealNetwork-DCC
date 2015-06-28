//_DCC_NET_TP1_CLIENT_H_
#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <cstdio>
#include <stdlib.h>
#include <string>

#include "ClientSocket.h"

class Client{
protected:
	ClientSocket socket_;
	bool connected_;
public:	
	Client();
	virtual ~Client();
	
	bool connectTo(const std::string &addr, const std::string &port);
	void sendMessage(std::string message);
	virtual void run() = 0;
	virtual bool parseReceived(std::string message) = 0;
	
	bool release();
	
};


#endif