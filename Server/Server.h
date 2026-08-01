#pragma once
#include <unordered_map>
#include <memory>
#include <thread>
#include "UserSession.h"
#include <mutex>

struct ServerConfig {
	static constexpr int port = 9090;
	static constexpr int sizeLog = 64;
	static constexpr int bufferSize = 4096;
};

class Server {
private:
	std::unordered_map<int, std::unique_ptr<UserSession>> clientSessions;
	std::thread threadForListenning;
	int socketFd = -1;
	bool running = false;
	std::mutex serverMutexForSession;

	void cleanDeadSessions();
	bool initializeSocketFd();
	void sendData(int socketFd, const std::vector<char>& data);
	bool stop();
	bool startListenning();
public:
	bool startServer(int port);

	~Server();
};

