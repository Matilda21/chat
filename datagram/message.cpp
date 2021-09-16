#include "message.h"

QDataStream &operator << (QDataStream &in, const struct MessageData &msg)
{
    in << msg.key;  
    in.writeRawData(msg.id.toByteArray().data(), ID_SIZE);
    in << msg.cur;
    in << msg.data_size;
    return in;
}

QDataStream &operator >> (QDataStream &out, struct MessageData &msg)
{
    out >> msg.key;    
    qDebug() << "md key = " << msg.key;
    char id[ID_SIZE];
    out.readRawData(id, ID_SIZE);
    msg.id = QUuid(id);
    qDebug() << "md id = " << msg.id;
    out >> msg.cur;
    qDebug() << "md cur = " << msg.cur;
    out >> msg.data_size;
    qDebug() << "md data size = " << msg.data_size;
    return out;
}

bool SendData::id_equal(const SendData &msg)
{
    qDebug() << "SendData::id_equal" << get_id() << msg.get_id();
    return (get_id() == msg.get_id());
}


quint16 Message::get_cur() const
{
    return info.cur;
}

Contact Message::get_contact() const
{
    return contact;
}

QByteArray Message::toByteArray()
{
    QByteArray byte_msg;
    QDataStream in(&byte_msg, QIODevice::WriteOnly);
    in << info;
    in.writeRawData(data.data(), data.size());
    return byte_msg;
}

Message::Message(const ByteBlock &block, const Contact &cnt)
{
    qDebug() << "constructor 1";
    QByteArray ba = block.toByteArray();
    QDataStream in(&data, QIODevice::WriteOnly);
    QDataStream out(&ba, QIODevice::ReadOnly);
    out >> info;
    QByteArray msg_msg;
    msg_msg.resize(info.data_size);

    out.readRawData(msg_msg.data(), info.data_size);
    in.writeRawData(msg_msg.data(), info.data_size);
    contact = cnt;
}

Message::Message(const struct MessageData &meta, const QByteArray &_data, const Contact &_contact)
{
    qDebug() << "constructor 2";
    contact = _contact;
    info = meta;
    data = _data;
    qDebug() << "size of message = " << data.size();
}

bool Message::is_msg() const
{
    return true;
}


bool Message::is_rec() const
{
    return false;
}

bool Message::is_signal() const
{
    return false;
}

QUuid Message::get_id() const
{
    return info.id;
}

Message &Message::add(Message &msg)
{
    //изменяет data_size и дозаписывает данные сообщения
    qDebug() << "added" << msg.get_data().data();
    char msg_data[info.data_size+msg.get_size()];
    memcpy(msg_data, this->get_data().data(), info.data_size);
    memcpy(msg_data+info.data_size, msg.get_data().data(), msg.get_size());
    info.data_size += msg.get_size();
    info.cur = msg.get_cur();

    QDataStream stream_write(&data, QIODevice::WriteOnly);
    qDebug() << "len of data" << info.data_size << msg_data;
    stream_write.writeRawData(msg_data, info.data_size);
    return *this;
}

QByteArray Message::get_data() const
{
    qDebug() << "get_data called";
    return data;
}

bool Message::index_equal(quint16 index) const
{
    return info.cur == index;
}

quint16 Message::get_key() const
{
    return info.key;
}

quint16 Message::get_size() const
{
    qDebug() << "GET_SIZE called";
    return info.data_size;
}

void Message::set_contact(const Contact &_contact)
{
    contact = _contact;
}
