#pragma once
#include <thread>
#include <vector>

class Client {
private:
	int socketFd = -1;
	bool running = false;
	std::thread clientThread;

	void runReceiving();
public:
	bool connectToServer(const std::string& ip, int port);
	bool sendToData(const std::vector<char>& data);
	bool stop();

	~Client();
};