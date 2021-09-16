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
#include "datagram/message.h"
#include <QFile>
#include <cmath>
#include <QIODevice>
#include <QUuid>
#define DEFAULT_SPEED 500
#define MSEC_PER_SEC 1000

class Client : public QUdpSocket
{
    Q_OBJECT
private:
    QString name;
    qint16 port;
    quint16 max_size;
    quint16 byte_per_sec;
    quint16 n_msg;//индекс следующего сообщения для отправки;
    std::vector<SendData *> outMsgVec;
    std::vector<MsgInfo *> outMsgInfo;
    std::vector<Message> inMsgVec;
    QTimer *timer;

public:
    Client(QObject *parent = nullptr);
    ~Client();
    void send_text(QByteArray &msg, const std::vector<Contact> &contacts);
    void send_file(QIODevice &file, const std::vector<Contact> &contacts, QString name);
    void setMaxSize(quint16 _max_size);
    void setSpeed(quint16 _byte_per_sec);
    quint16 splitMsg(QDataStream &in, quint16 full_size, QUuid id, const std::vector<Contact> &contacts, quint16 type);
    void setPort(quint16 _port);
    void setName(const QString &_name);
    void merge_parts(const ByteBlock &block, const Contact &contact);
    void process_rec(const ByteBlock &block, const Contact &contact);
    void send(SendData *msg);
    void add_msg(SendData *msg);
    void show_rec(const Reciept &rec);
signals:
    void msgObtained(QByteArray msg, quint16 key, QString name);
    void msgSent(QByteArray msg);
    void startSending();
    void msgNotFull(Reciept *rec);
    void gotRec(const QByteArray &text, const Contact &cnt, quint16 rec_key);
public slots:
    void receive();
    void sendSingleMsg();
private:
    template <typename T>
    void delete_el(std::vector<T> &vec, quint16 i)
    {
        qDebug() << "delete called" << i;
        delete vec.at(i);
        vec.erase(vec.begin() + i);
        qDebug() << "deleted" << vec.size();
    };


     MsgInfo *get_full_msg(QUuid id);
     void sort_parts(std::vector<Message> &parts);
     void resend_part(quint16 i);
     std::vector<Message> get_parts(QUuid id);
     std::vector<quint16> get_parts_nums(QUuid id);
     void clean_info_vec(QUuid id);
     void resend_message(const Reciept &rec);
     void delete_message(const Reciept &rec);
};

#endif // CLIENT_H
