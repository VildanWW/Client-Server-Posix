#include "UserSession.h"
#include <vector>
#include <thread>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>

bool UserSession::startWorking() {
	running = true;
	clientThread = std::thread(&UserSession::runWorking, this);
	return true;
}

void UserSession::runWorking() {
	const int bufferSize = 4096;
	std::vector<char> buffer(bufferSize);

	while (running) {
		int bytesRead = recv(socketFd, buffer.data(), bufferSize, 0);

		if (bytesRead <= 0) {
			std::cerr << "Bytes read <= 0\n";
			running = false;
			break;
		}

		std::vector<char> dataToServer(buffer.begin(), buffer.begin() + bytesRead);

		if (status == UserStatus::WAITING_NAME) {
			name = std::string(dataToServer.begin(), dataToServer.end());
			status = UserStatus::CONNECTED_TO_CHAT;
			std::cout << "Client sets name: " << name << '\n';
			continue;
		}

		if (onMessageReceived) {
			onMessageReceived(socketFd, dataToServer);
		}
	}
}

bool UserSession::stop() {
	if (!running) return false;
	running = false;

	if (socketFd != -1) {
		shutdown(socketFd, SHUT_RDWR);
		close(socketFd);
	}
	std::cout << "Method stop of UserSession worked successfully\n";
	return true;
}

UserSession::~UserSession() {
	stop();
	if (clientThread.joinable()) {
		clientThread.join();
	}
	std::cout << "UserSession destructor worked successfully\n";
}

