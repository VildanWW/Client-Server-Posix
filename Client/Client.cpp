#include "Client.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>

void Client::runReceiving() {
	const int bufferSize = 4096;
	std::vector<char> data(bufferSize);

	while (running) {
		int bytesRead = recv(socketFd, data.data(), bufferSize, 0);

		if (bytesRead <= 0) {
			std::cout << "Bytes read <= 0\n";
			running = false;
			break;
		}

		std::string dataReceived(data.begin(), data.begin() + bytesRead);
		std::cout << dataReceived << '\n';
	}
}

bool Client::connectToServer(const std::string& ip, int port) {
	socketFd = socket(AF_INET, SOCK_STREAM, 0);

	if (socketFd < 0) {
		std::cerr << "Don't create socketFd\n";
		return false;
	}

	sockaddr_in clientAddr;
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons(port);

	if (inet_pton(AF_INET, ip.c_str(), &clientAddr.sin_addr) <= 0) {
		std::cerr<<"Error with inetPton\n";
		close(socketFd);
		socketFd = -1;
		return false;
	}

	if (connect(socketFd, (sockaddr*)&clientAddr, sizeof(clientAddr)) < 0) {
		std::cerr << "Error with connect\n";
		close(socketFd);
		socketFd = -1;
		return false;
	}

	running = true;

	clientThread = std::thread(&Client::runReceiving, this);

	return true;
}

bool Client::sendToData(const std::vector<char>& data) {
	if (socketFd == -1 || !running) return false;

	int bytesSent = send(socketFd, data.data(), data.size(), 0);

	if (bytesSent <= 0) {
		return false;
	}

	return true;
}

bool Client::stop() {
	if (!running) return false;
	running = false;

	if (socketFd != -1) {
		shutdown(socketFd, SHUT_RDWR);
		close(socketFd);
		socketFd = -1;
	}

	return true;
}

Client::~Client() {
	stop();
	if (clientThread.joinable()) {
		clientThread.join();
	}
}
