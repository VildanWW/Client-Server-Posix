#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include "Settings.h"

class Client : public QObject {
    Q_OBJECT
private:
    QTcpSocket* socket = nullptr;

    void onReadyRead();
public:
    explicit Client(QObject *parent = nullptr);

    bool connectToServer(const std::string& ip);
    bool sendToData(DataType type, const QByteArray& sendBuffer);
    bool stop();

    ~Client();
signals:
    void messageReceived(DataType type, const QByteArray& data);
    void disconnected();
};

#endif // CLIENT_H
