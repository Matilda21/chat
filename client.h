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
#include <QFile>
#include <cmath>
#include <QIODevice>
#include <QUuid>
#define DEFAULT_SPEED 1
#define MSEC_PER_SEC 1000
#define FILE_MSG 1
#define TXT_MSG 2



class Client : public QUdpSocket
{
    Q_OBJECT
private:
    QByteArray name;
    qint16 port;
    quint16 max_size;
    quint16 byte_per_sec;
    std::queue<Message> msgQueue;
    std::vector<Message> inMsgVec;
    QTimer *timer;

public:
    Client(QObject *parent = nullptr);
    void send_text(QByteArray &msg, const std::vector<Contact> &contacts);
    void send_file(QIODevice &file, const std::vector<Contact> &contacts);
    void setMaxSize(quint16 _max_size);
    void setSpeed(quint16 _byte_per_sec);
    void splitMsg(QDataStream &in, quint16 full_size, const std::vector<Contact> &contacts, quint16 type);
    void setPort(quint16 _port);
    void setName(const QByteArray &_name);
signals:
    void msgObtained(QByteArray msg, quint16 key);
    void msgSent(QByteArray msg, QByteArray result);
    void startSending();
public slots:
    void receive();
    void sendSingleMsg();

};

#endif // CLIENT_H
