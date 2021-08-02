#ifndef MESSAGE_H
#define MESSAGE_H
#include "contact.h"
#include <iostream>
#include <memory>
#include <QDebug>
#include <QDataStream>
#include <cstring>
#define ID_SIZE 39

struct MessageData
{
    quint16 key;
    char id[ID_SIZE];
    quint16 num;
    quint16 cur;
    quint16 data_size;

};

QDataStream &operator << (QDataStream &in, const struct MessageData &msg);

QDataStream &operator >> (QDataStream &out, struct MessageData &msg);

class Message
{
private:
    std::vector<Contact> contacts;

    QByteArray data;
public:
    Message(const QByteArray &msg);
    Message(const struct MessageData &meta, const QByteArray &_data, const std::vector<Contact> &_contacts);
     ~Message()  = default;
    Message &add(Message &msg);
    bool id_equal(Message &msg);
    bool is_last();
    Contact get_contact() const;
    void pop_contact();
    bool is_sent() const;
    QByteArray get_data();
    QByteArray toByteArray();
    quint16 get_key();
    struct MessageData get_info();
    quint16 get_size();
};

#endif // MESSAGE_H
