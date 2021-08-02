#include "message.h"

QDataStream &operator << (QDataStream &in, const struct MessageData &msg)
{
    in << msg.key;
    in.writeRawData(msg.id, ID_SIZE);
    in << msg.num;
    in << msg.cur;
    in << msg.data_size;
    return in;
}

QDataStream &operator >> (QDataStream &out, struct MessageData &msg)
{
    out >> msg.key;
    qDebug() << "md key = " << msg.key;
    out.readRawData(msg.id, ID_SIZE);
    qDebug() << "md id = " << msg.id;
    out >> msg.num;
    qDebug() << "md num = " << msg.num;
    out >> msg.cur;
    qDebug() << "md cur = " << msg.cur;
    out >> msg.data_size;
    qDebug() << "md data size = " << msg.data_size;
    return out;
}

Message::Message(const QByteArray &msg)
{
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.writeRawData(msg.data(), msg.size());
}

Message::Message(const struct MessageData &meta, const QByteArray &_data, const std::vector<Contact> &_contacts)
{
    contacts = _contacts;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << meta;
    stream.writeRawData(_data.data(), _data.size());
    qDebug() << "size of message = " << data.size();

}


Message &Message::add(Message &msg)
{
    //изменяет data_size и дозаписывает данные сообщения
    struct MessageData md = this->get_info();
    struct MessageData new_md = msg.get_info();

    char msg_data[md.data_size+new_md.data_size];
    memcpy(msg_data, this->get_data(), md.data_size);
    memcpy(msg_data+md.data_size, msg.get_data(), new_md.data_size);
    md.data_size += new_md.data_size;

    QDataStream stream_write(&data, QIODevice::WriteOnly);
    qDebug() << "len of data" << md.data_size;
    stream_write << md;
    stream_write.writeRawData(msg_data, md.data_size);
    return *this;
}

bool Message::id_equal(Message &msg)
{
    struct MessageData md = this->get_info();
    struct MessageData new_md = msg.get_info();

    if (memcmp(md.id, new_md.id, ID_SIZE) == 0)
        return true;
    return false;
}

bool Message::is_last()
{
    struct MessageData md = this->get_info();

    if (md.cur == md.num)
        return true;
    return false;
}


Contact Message::get_contact() const
{
    return contacts.back();
}

void Message::pop_contact()
{
    contacts.pop_back();
}

bool Message::is_sent() const
{
    return contacts.empty();
}

QByteArray Message::get_data()
{
    qDebug() << "get_data called";
    QDataStream stream(&data, QIODevice::ReadOnly);
    struct MessageData md;


    stream >> md;
    QByteArray msg(md.data_size, 0);
    qDebug() << "readed" << stream.readRawData(msg.data(), md.data_size);
    return msg;
}

QByteArray Message::toByteArray()
{
    return this->data;
}

quint16 Message::get_key()
{
    return this->get_info().key;

}

struct MessageData Message::get_info()
{
    qDebug() << "get_info called";
    QDataStream stream(&data, QIODevice::ReadOnly);
    struct MessageData md;
    stream >> md;
    return md;
}

quint16 Message::get_size()
{
    return this->data.size();
}

