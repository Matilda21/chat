#include "client.h"
#define MAX_SIZE 10
#define DEFAULT_SPEED 1
#define MSEC_PER_SEC 1000

Client::Client(quint16 _port, QByteArray _name, quint16 _max_size)
{
    this->socket = new QUdpSocket(this);
    this->port = _port;
    this->name = _name;
    this->max_size = _max_size;
    this->byte_per_sec = DEFAULT_SPEED;
    this->msgQueue = new std::queue<Message>();
    this->timer = new QTimer();
    timer->setSingleShot(true);

    socket->bind(QHostAddress::AnyIPv4, port);

    connect(socket, SIGNAL(readyRead()), this, SLOT(receive()));
    connect(this, SIGNAL(startSending()), this, SLOT(sendSingleMsg()));
    connect(timer, SIGNAL(timeout()), this, SLOT(sendSingleMsg()));

}


void Client::send(QByteArray msg, std::vector<Contact> *contacts)
{
    spltMsg(msg, contacts);

    if (timer->isActive())
        return;

    emit startSending();
}

void Client::sendSingleMsg()
{
    if (msgQueue->empty())
        return;

    Message msg = msgQueue->front();

    while (msg.is_sent())
    {
        emit msgSent(msg.get_text(), "success");
        msgQueue->pop();

        if (msgQueue->empty())
            return;
        msg = msgQueue->front();
    }

    Contact receiver = msg.get_contact();
    QByteArray send_text = msg.add_name(name);
    this->socket->writeDatagram(send_text.data(), send_text.size(),
                        QHostAddress(receiver.get_ip()), receiver.get_port());
    msgQueue->front().pop_contact();

    timer->setInterval(((msg.get_size()+name.size()) / byte_per_sec + 1) * MSEC_PER_SEC);
    timer->start();
    std::cout << timer->interval() << std::endl;

}

void Client::spltMsg(QByteArray msg, std::vector<Contact> *contacts)
{
    while (msg.size() > max_size)
    {
        QByteArray tmp = msg;
        tmp.truncate(max_size);

        msgQueue->push(*(new Message(tmp, *contacts)));
        msg.remove(0, max_size);
    }
    msgQueue->push(*(new Message(msg, *contacts)));
}

void Client::receive()
{
    QByteArray buf;
    QHostAddress senderAdress;
    quint16 senderPort;

    while (this->socket->hasPendingDatagrams())
    {
        buf.resize(this->socket->pendingDatagramSize());
            this->socket->readDatagram(buf.data(), buf.size(),
               &senderAdress, &senderPort);

        emit msgObtained(buf);
    }
}

