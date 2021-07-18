#ifndef CLIENT_H
#define CLIENT_H
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <QObject>
#include <QtNetwork/QUdpSocket>
#include <QByteArray>
#include "contact.h"
#include <QTimer>
#include <queue>
#include "message.h"

class Client : public QObject
{
    Q_OBJECT
private:
    QUdpSocket *socket;
    QByteArray name;
    qint16 port;
    quint16 max_size;
    quint16 byte_per_sec;
    std::queue<Message> *msgQueue;
    QTimer *timer;

public:
    Client(quint16 _port, QByteArray _name, quint16 _max_size);
    void spltMsg(QByteArray msg, std::vector<Contact> *contacts);
    void send(QByteArray msg, std::vector<Contact> *contacts);
    void setMaxSize(quint16 _max_size){max_size = _max_size;};
    void setSpeed(quint16 _byte_per_sec){byte_per_sec = _byte_per_sec;};

signals:
    void msgObtained(QByteArray msg);
    void msgSent(QByteArray msg, QByteArray result);
    void startSending();
public slots:
    void receive();
    void sendSingleMsg();

};

#endif // CLIENT_H
