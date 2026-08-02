#pragma once
#include <string>
#include <thread>
#include <functional>
#include <vector>
#include "Settings.h"

class UserSession {
private:
	std::function<void(int, const InAppMessage&)> onMessageReceived;
	std::thread clientThread;
	std::string name;

	UserStatus status = UserStatus::WAITING_NAME;

	int socketFd;
	bool running = false;

	void runWorking();
	bool stop();
public:
	UserSession(int socketFd, 
		std::function<void(int, const InAppMessage&)> onMessageReceived)
		: name(""), socketFd(socketFd), onMessageReceived(onMessageReceived) {}

	bool getRunning();
	bool startWorking();

	~UserSession();
};