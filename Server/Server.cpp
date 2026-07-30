#include "Server.h"
#include <thread>
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include "UserSession.h"

bool Server::startListenning() {
	while (running) {
		sockaddr_in clientAddress;
		socklen_t sizeAddr = sizeof(clientAddress);

		int clientSocketFd = accept(socketFd, (sockaddr*)&clientAddress, &sizeAddr);

		if (clientSocketFd < 0) {
			std::cerr << "ClientSocketFd is wrong\n";
			continue;
		}

		std::cout << "New good connect!\n";

		clientSessions[clientSocketFd] = std::make_unique<UserSession>();
		clientSessions[clientSocketFd]->startWorking();
	}

	return true;
}

bool Server::startServer(int port) {
	if (!initializeSocketFd()) {
		std::cerr << "Server can't to initialize the socketFd\n";
		return false;
	}

	running = true;

	threadForListenning = std::thread(&Server::startListenning, this);

	std::cout << "Server is running!\n";
	return true;
}

void Server::handleClient() {

}

bool Server::initializeSocketFd() {
	socketFd = socket(AF_INET, SOCK_STREAM, 0);

	if (socketFd < 0) {
		std::cerr << "Failed to create the socketFd\n";
		return false;
	}

	sockaddr_in address{};
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(ServerConfig::port);

	socklen_t sizeAddr = sizeof(address);
	if (bind(socketFd, (sockaddr*)&address, sizeAddr) < 0){
		std::cerr << "Error with bind\n";
		close(socketFd);
		return false;
	}

	if (listen(socketFd, ServerConfig::sizeLog) < 0) {
		std::cerr << "Can't to tune listen with sizeLog\n";
		close(socketFd);
		return false;
	}

	std::cout << "Server is running on port: " << ServerConfig::port << '\n';
	
	return true;
}

bool Server::stop() {
	if (!running) return false;
	running = false;

	if (socketFd != -1) {
		close(socketFd);
		socketFd = -1;
	}

	return true;
}

Server::~Server() {
	stop();
	if (threadForListenning.joinable()) {
		threadForListenning.join();
	}
}

