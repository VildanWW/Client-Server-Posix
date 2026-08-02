#include "UserSession.h"
#include <vector>
#include <thread>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include "Settings.h"

void UserSession::runWorking() {
	while (running) {
		PacketData packet;

		int bytesReadPacket = recv(socketFd, &packet, sizeof(PacketData), MSG_WAITALL);

		if (bytesReadPacket == 0) {
			std::cout << "Client cleanly disconnected, socket: " << socketFd << '\n';
			running = false; 
			break;           
		}

		else if (bytesReadPacket < 0) {
			std::cerr << "Bytes read <= 0\n";
			running = false;
			break;
		}

		std::vector<char> dataBuffer(packet.dataSize);

		int bytesData = recv(socketFd, dataBuffer.data(), dataBuffer.size(), MSG_WAITALL);

		if (bytesData == 0) {
			std::cout << "Client cleanly disconnected, socket: " << socketFd << '\n';
			running = false;
			break;
		}

		else if (bytesData < 0) {
			std::cerr << "Bytes read <= 0\n";
			running = false;
			break;
		}

		if (status == UserStatus::WAITING_NAME) {
			name = std::string(dataBuffer.begin(), dataBuffer.end());
			status = UserStatus::CONNECTED_TO_CHAT;
			std::cout << "Client sets name: " << name << '\n';
			continue;
		}

		if (onMessageReceived) {
			InAppMessage messageToServer;

			messageToServer.senderName = name;
			messageToServer.type = packet.packetType;
			messageToServer.dataBuffer = std::move(dataBuffer);

			onMessageReceived(socketFd, messageToServer);
		}
	}
}

bool UserSession::getRunning() {
	return running;
}

bool UserSession::startWorking() {
	running = true;
	clientThread = std::thread(&UserSession::runWorking, this);
	return true;
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

