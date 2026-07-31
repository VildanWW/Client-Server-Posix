#pragma once
#include <string>
#include <thread>
#include <functional>
#include <vector>

enum UserStatus {
	WAITING_NAME,
	CONNECTED_TO_CHAT
};

class UserSession {
private:
	std::string name;
	int socketFd;
	std::function<void(int, const std::vector<char>&)> onMessageReceived;
	std::thread clientThread;
	bool running = false;
	UserStatus status = UserStatus::WAITING_NAME;

	void runWorking();
	bool stop();
public:
	UserSession(int socketFd, std::function<void(int, const std::vector<char>&)> onMessageReceived) : name(""), socketFd(socketFd), onMessageReceived(onMessageReceived) {}

	bool startWorking();

	~UserSession();
};