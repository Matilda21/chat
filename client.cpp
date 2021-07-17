
#include "client.h"
#define MAX_SIZE 10


Client::Client(quint16 _port, QByteArray _name, quint16 _max_size)
{
    this->socket = new QUdpSocket(this);
    this->port = _port;
    this->name = _name;
    this->max_size = _max_size;

    socket->bind(QHostAddress::AnyIPv4, port);

    connect(socket, SIGNAL(readyRead()), this, SLOT(receive()));
}


void Client::send(QByteArray msg, std::vector<quint16> *contacts)
{
    std::vector<QByteArray> *msg_parts = new std::vector<QByteArray>();
    spltMsg(msg, *msg_parts);
    for (QByteArray message : *msg_parts)
    {

        message.insert(0, ": ");
        QByteArray my_msg = message;
        my_msg.insert(0, "You");
        message.insert(0, this->name);

        for (auto con_port : *contacts)
       {
            std::cout << port << "->" << con_port << std::endl;
            this->socket->writeDatagram(message.data(), message.size(),
                   QHostAddress::LocalHost, con_port);
       }

        emit msgObtained(my_msg);
    }
}

void Client::send(QByteArray msg, std::vector<Contact> *contacts)
{
    std::vector<QByteArray> *msg_parts = new std::vector<QByteArray>();
    spltMsg(msg, *msg_parts);
    for (QByteArray message : *msg_parts)
    {

        message.insert(0, ": ");
        QByteArray my_msg = message;
        my_msg.insert(0, "You");
        message.insert(0, this->name);

        QByteArray result = "sent\n";


        for (Contact contact : *contacts)
       {
            if (message.size() != this->socket->writeDatagram(message.data(), message.size(),
                   QHostAddress(contact.get_ip()), contact.get_port()))
                result = "error\n";
       }

        emit msgSent(my_msg, result);

    }
}

void Client::spltMsg(QByteArray msg, std::vector<QByteArray> &msg_parts)
{
    while (msg.size() > max_size)
    {
       // std::cout << msg.size() << std::endl;
        QByteArray tmp = msg;
        tmp.truncate(max_size);
        msg_parts.push_back(tmp);
        msg.remove(0, max_size);
    }
    msg_parts.push_back(msg);

}

void Client::receive()
{
    std::cout << "obtained" << std::endl;
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

