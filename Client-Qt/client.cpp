#include "client.h"
#include "Settings.h"
#include <iostream>

Client::Client(QObject *parent) : QObject{parent} {
    socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::disconnected, this, &Client::disconnected);
    connect(socket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
}

void Client::onReadyRead() {
    if(socket->bytesAvailable() < sizeof(PacketData)){
        return;
    }

    PacketData packet;
    socket->peek(reinterpret_cast<char*>(&packet), sizeof(PacketData));

    int fullPacketSize = sizeof(PacketData) + packet.dataSize;

    if(socket->bytesAvailable() < fullPacketSize) {
        return;
    }

    socket->read(reinterpret_cast<char*>(&packet), sizeof(PacketData));

    QByteArray bufferRecived = socket->read(packet.dataSize);

    emit messageReceived(packet.packetType, bufferRecived);
}

bool Client::connectToServer(const std::string &ip) {
    if(!socket) {
        return false;
    }

    socket->connectToHost(QString::fromStdString(ip), ServerConfig::port);

    if(socket->waitForConnected(ClientConfig::timeWaitingConnected)) {
        std::cout << "Connected successfully\n";
        return true;
    }

    std::cerr << "Don't to connected to the server\n";
    return false;
}

bool Client::sendToData(DataType type, const QByteArray &sendBuffer) {
    if(!socket || socket->state()!=QAbstractSocket::ConnectedState) {
        return false;
    }

    PacketData packet;
    packet.packetType = type;
    packet.dataSize = sendBuffer.size();

    socket->write(reinterpret_cast<char*>(&packet), sizeof(PacketData));

    if(packet.dataSize>0) {
        socket->write(sendBuffer);
    }
    return socket->flush();
}

bool Client::stop() {
    if(socket && socket->state()!=QAbstractSocket::UnconnectedState) {
        socket->disconnectFromHost();
        socket->waitForDisconnected(ClientConfig::timeWaitingDisconnected);
    }
    return true;
}

Client::~Client() {
    stop();
    std::cout<<"Destructor worked!\n";
}

