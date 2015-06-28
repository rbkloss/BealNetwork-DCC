
#include "Client.h"



Client::Client() {}

Client::~Client() {}

bool Client::connectTo(const std::string &addr, const std::string &port) {
	auto ans = socket_.connectTo(addr, port);
	connected_ = true;
	return ans;
}


void Client::sendMessage(std::string message) {
	socket_.write(message);
}



bool Client::release() {
	socket_.~ClientSocket();
	return true;
}
