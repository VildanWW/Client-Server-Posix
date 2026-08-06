#ifndef SETTINGS_H
#define SETTINGS_H

#include <vector>
#include <string>

enum UserStatus : int32_t {
    WAITING_NAME,
    CONNECTED_TO_CHAT
};

enum class DataType : int32_t {
    TEXT,
    IMAGE,
    AUDIO,
    VIDEO
};

struct PacketData {
    DataType packetType;
    int dataSize;
    char senderName[32];
};

struct InAppMessage {
    DataType type;
    std::string senderName;
    std::vector<char> dataBuffer;
};

struct ServerConfig {
    static constexpr int port = 9090;
    static constexpr int sizeLog = 64;
    static constexpr int bufferSize = 4096;
    static constexpr int timeCleanSession = 5;
    static const inline std::string serverIp = "127.0.0.1";
};

struct ClientConfig{
    static constexpr int timeWaitingConnected = 3000;
    static constexpr int timeWaitingDisconnected = 1000;
};

#endif // SETTINGS_H
