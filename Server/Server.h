#pragma once
#include <unordered_map>
#include <memory>
#include <thread>
#include "UserSession.h"

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
public:
	bool startServer(int port);
	bool startListenning();
	void handleClient();
	bool initializeSocketFd();
	bool stop();

	~Server();
};

