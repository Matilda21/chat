#ifndef UDPCLIENT_H
#define UDPCLIENT_H
#include <QObject>
#include <QtNetwork/QUdpSocket>
#include "iostream"
#include <vector>
#include <stdlib.h>
class UDPClient : public QObject
{
private:
    Q_OBJECT
    QUdpSocket *socket;
    int max_size = 20;

public:
    UDPClient(QObject *parent = 0)
    {
        this->socket = new QUdpSocket(this);

        connect(this->socket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));

    }
    virtual ~UDPClient(){}

    void sendPendingDatagram(QByteArray buf)
    {
        while (buf.size() > max_size)
        {
            splitMessage(buf);
            buf.chop(max_size);
        }
        splitMessage(buf);
      //  this->socket->writeDatagram(buf.data(), buf.size(), QHostAddress::LocalHost, 1601);
    }

    void splitMessage(QByteArray buf)
    {
        buf.truncate(max_size);
        this->socket->writeDatagram(buf.data(), buf.size(), QHostAddress::LocalHost, 1601);

    }

signals:
    void dataObtained(QByteArray data);

public slots:
    void readPendingDatagrams(){
        QByteArray buf;
        quint16 dataSize;
        buf.resize(this->socket->pendingDatagramSize());
        while (this->socket->hasPendingDatagrams())
        {
            dataSize = this->socket->readDatagram(buf.data(), buf.size(), NULL, NULL);
            emit dataObtained(buf.data());

            std::cout << buf.data() << std::endl;
        }


    }

};
#endif // UDPCLIENT_H
