#include "Server.h"

#include <thread>

Server::Server(const std::string& port) : socket_(ServerSocket::create(port)) {}

Server::~Server() {

}

void Server::run() {
	static std::vector<std::thread> threads;
	while (true) {
		printf("Server: Listenning and Waiting Connection\n");
		auto client = socket_.acceptClient();
		threads.push_back(std::thread([this,client]() {
			this->onAccept(client);
		}));
	}
}