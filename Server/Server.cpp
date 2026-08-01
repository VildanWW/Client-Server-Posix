#include "Server.h"
#include <thread>
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include "UserSession.h"

bool Server::startListenning() {
	while (running) {
		cleanDeadSessions();

		sockaddr_in clientAddress;
		socklen_t sizeAddr = sizeof(clientAddress);

		int clientSocketFd = accept(socketFd, (sockaddr*)&clientAddress, &sizeAddr);

		if (clientSocketFd < 0) {
			std::cerr << "ClientSocketFd is wrong\n";
			continue;
		}

		std::cout << "New good connect!\n";
		std::lock_guard<std::mutex> lockGuard(serverMutexForSession);
		{
			clientSessions[clientSocketFd] = std::make_unique<UserSession>(clientSocketFd, [this](int fd, const std::vector<char>& data) {
				this->sendData(fd, data);
			});
		}

		clientSessions[clientSocketFd]->startWorking();
	}

	return true;
}

void Server::cleanDeadSessions() {
	std::lock_guard<std::mutex> lockGuard(serverMutexForSession);

	for (auto it = clientSessions.begin(); it != clientSessions.end();) {
		if (!it->second->getRunning()) {
			it = clientSessions.erase(it);
			std::cout << "Client session deleted from server memory\n";
		}
		else {
			it++;
		}
	}
}

void Server::sendData(int socketFd, const std::vector<char>& data) {
	if (data.empty()) {
		std::cerr << "Vector is empty in sendData method\n";
		return;
	}

	{
		std::lock_guard<std::mutex> lockGuard(serverMutexForSession);
		for (const auto& client : clientSessions) {
			int clientFd = client.first;

			if (socketFd == clientFd || !client.second->getRunning()) continue;


			int bytesSent = send(clientFd, data.data(), data.size(), 0);

			if (bytesSent <= 0) {
				std::cerr << "Bytes send <= 0, client clientFd: " << clientFd << '\n';
				continue;
			}
		}
	}
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
		shutdown(socketFd, SHUT_RDWR);
		close(socketFd);
		socketFd = -1;
	}
	std::cout << "Method stop of Server worked successfully\n";
	return true;
}

Server::~Server() {
	stop();
	if (threadForListenning.joinable()) {
		threadForListenning.join();
	}
	std::cout << "Server destructor worked successfully\n";
}

