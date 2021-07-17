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

class Client : public QObject
{
    Q_OBJECT
private:
    QUdpSocket *socket;
    QByteArray name;
    qint16 port;
    quint16 max_size;

public:
    Client(quint16 _port, QByteArray _name, quint16 _max_size);
    void spltMsg(QByteArray msg, std::vector<QByteArray> &msg_parts);
    void send(QByteArray msg, std::vector<quint16> *contacts);
    void send(QByteArray msg, std::vector<Contact> *contacts);

signals:
    void msgObtained(QByteArray msg);
    void msgSent(QByteArray msg, QByteArray result);
public slots:
    void receive();

};

#endif // CLIENT_H
