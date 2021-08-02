#include "client.h"

Client::Client(QObject *parent):
    QUdpSocket(parent),
    byte_per_sec(DEFAULT_SPEED),
    timer(new QTimer(this))
{
    timer->setSingleShot(true);

    connect(this, SIGNAL(readyRead()), this, SLOT(receive()));
    connect(this, SIGNAL(startSending()), this, SLOT(sendSingleMsg()));
    connect(timer, SIGNAL(timeout()), this, SLOT(sendSingleMsg()));
}

void Client::send_text(QByteArray &msg, const std::vector<Contact> &contacts)
{
    QDataStream in(&msg, QIODevice::ReadOnly);
    splitMsg(in, msg.size(), contacts, TXT_MSG);

    if (timer->isActive())
        return;

    emit startSending();
}

void Client::send_file(QIODevice &file, const std::vector<Contact> &contacts)
{
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    splitMsg(in, file.bytesAvailable(), contacts, FILE_MSG);
    file.close();

    if (timer->isActive())
        return;

    emit startSending();
}

void Client::sendSingleMsg()
{
    if (msgQueue.empty())
        return;

    Message msg = msgQueue.front();

    while (msg.is_sent())
    {
       // emit msgSent(msg.get_data(), "success");
        msgQueue.pop();

        if (msgQueue.empty())
            return;
        msg = msgQueue.front();
    }

    Contact receiver = msg.get_contact();
    QByteArray send_text = msg.toByteArray();
    writeDatagram(send_text.data(), send_text.size(),
                        QHostAddress(receiver.get_ip()), receiver.get_port());
    msgQueue.front().pop_contact();

    timer->setInterval(round(  ((double) (send_text.size()) / byte_per_sec) * MSEC_PER_SEC));
    timer->start();

}

void Client::receive()
{
    QByteArray buf;
    while (hasPendingDatagrams())
    {
        buf.resize(pendingDatagramSize());

        readDatagram(buf.data(), buf.size(),
               NULL, NULL);
        Message msg(buf);
        bool is_new = true;

        for (size_t i = 0; i < inMsgVec.size(); i++)
        {

            if (msg.id_equal(inMsgVec[i])) //части одного сообщения
            {
                qDebug() << "same part found";
                is_new = false;
                inMsgVec[i].add(msg);

                if (msg.is_last()) //получена последняя часть сообщения
                {
                    qDebug() << "full message obtained";
                    emit msgObtained(inMsgVec[i].get_data(), msg.get_key());
                    inMsgVec.erase(inMsgVec.begin() + i);
                }
            }
        }
        if (is_new == true)
        {
            qDebug() << "new message part obtained";

            if (msg.is_last()) //получена последняя часть сообщения
            {
                qDebug() << "full message obtained";
                emit msgObtained(msg.get_data(), msg.get_key());
            }
            else
                inMsgVec.push_back(msg);
        }
    }
}

void Client::splitMsg(QDataStream &in, quint16 full_size, const std::vector<Contact> &contacts, quint16 type)
{
    QByteArray block(max_size, 0);
    QUuid id;
    QByteArray ba_id = id.createUuid().toByteArray();

    quint16 num = full_size / max_size + 1;

    for (quint16 num_part = 1; num_part <= num; num_part++)
    {
        if (full_size < block.size())
            block.resize(full_size);

        quint16 size = in.readRawData(block.data(), block.size());
        full_size -= size;

        struct MessageData md = {type, {}, num, num_part, size};
        memcpy(md.id, ba_id.data(), ba_id.size());
        Message msg(md, block, contacts);

        msgQueue.push(msg);
    }
    emit startSending();
}

void Client::setMaxSize(quint16 _max_size)
{
    max_size = _max_size;
}

void Client::setSpeed(quint16 _byte_per_sec)
{
    byte_per_sec = _byte_per_sec;
}

void Client::setPort(quint16 _port)
{
    port = _port;
    bind(QHostAddress::AnyIPv4, port);
}

void Client::setName(const QByteArray &_name)
{
    name =_name;
}

