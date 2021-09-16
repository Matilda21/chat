#ifndef MESSAGE_H
#define MESSAGE_H
#include "contact.h"
#include <iostream>
#include <memory>
#include <QDebug>
#include <QDataStream>
#include <cstring>
#include <QUuid>
#define ID_SIZE 39
#define FILE_MSG 1
#define TXT_MSG 2
#define REC_MSG 3
#define SIGNAL_MSG 4
#define MAX_NAME_LEN 20
#define ASK 1
#define REGECT 0
#define IS_FULL 2

struct MessageData//данные части сообщения
{
    quint16 key; //ключ части текстового сообщения или файла
    QUuid id;//айди
    quint16 cur;//номер по порядку данной части
    quint16 data_size;//размер данных записаных в данной части
};

struct RecData //данные квитанции
{
    quint16 key;       //ключ квитанции
    QUuid id;         //айди сообщения к которому относится квитанция
    quint16 received; //кол-во успешно доставленных частей сообщения
    quint16 rec_key;//требуется ли доотправить недоедшие части сообщения
};

struct SignalData //данные сообщения-сигнала конца сообщения
{
    quint16 key;       //ключ сообщение-сигнал конца сообщения
    QString name;//имя отправителя
    QUuid id;         //айди сообщения к которому относится квитанция
    quint16 num;//кол-во частей сообщения
};

QDataStream &operator << (QDataStream &in, const struct MessageData &msg);

QDataStream &operator >> (QDataStream &out, struct MessageData &msg);

class MsgInfo //класс информации о целом сообщении(хранится у отправителя до полной отправки)
{
private:
    QUuid id; //айди сообщения (и частей)
    quint16 key; // тип сообщения (файл или текст)
    quint16 parts_left; //осталось неотправленных частей этого сообщения
    QByteArray data; //данные всего сообщения
public:
    MsgInfo() = default;
    MsgInfo(QUuid id, quint16 key, quint16 _parts_left, QByteArray data);
    QUuid get_id();
    quint16 get_key();
    quint16 get_parts_left();
    void delete_part();
    void set_parts_left(quint16 n);
    QByteArray get_data();
};

class ByteBlock //класс информации прочитанной из сокета
{
private:
    quint16 key; //тип сообщения (квитанция, часть сообщения, стоп-сигнал)
    QByteArray block;

public:
    quint16 get_key();
    bool is_msg();
    bool is_rec();
    bool is_signal();
    ByteBlock(const QByteArray &_block);
    ByteBlock() = default;
    ~ByteBlock() = default;
    char *data();
    const char *data() const;
    int size() const;
    void resize(int size);
    QByteArray toByteArray() const;
};


class SendData//базовый класс данных для отправки(квитанция, часть сообщения, стоп-сигнал)
{
public:
    virtual ~SendData() = default;
    virtual QByteArray toByteArray() = 0;
    virtual QUuid get_id() const = 0;
    virtual bool id_equal(const SendData &msg);
    virtual bool is_msg() const = 0;
    virtual bool is_rec() const = 0;
    virtual bool is_signal() const = 0;
    virtual bool index_equal(quint16 index) const = 0;

    virtual Contact get_contact() const = 0;

    virtual void set_contact(const Contact &_contact) = 0;
};

class Message : public SendData//класс части сообщения
{
private:
    Contact contact;
    struct MessageData info;
    QByteArray data;
public:
    Message(const ByteBlock &block, const Contact &cnt);
    Message(const struct MessageData &meta, const QByteArray &_data, const Contact &_contact);

    virtual ~Message()  = default;
    virtual QByteArray toByteArray() override;
    virtual QUuid get_id() const override;
    virtual bool index_equal(quint16 index) const override;
    virtual bool is_msg() const override;
    virtual bool is_rec() const override;
    virtual bool is_signal() const override;

    virtual Contact get_contact() const override;

    quint16 get_key() const;
    quint16 get_size() const;
    quint16 get_cur() const;
    QByteArray get_data() const;

    Message &add(Message &msg);
    virtual void set_contact (const Contact &_contact) override;
};

class Reciept : public SendData//класс квитанции о получении сообщения
{
private:
    Contact contact;
    struct RecData info;
    std::vector<quint16> parts;
public:

    Reciept(const ByteBlock &block, const Contact &cnt);
    Reciept(const struct RecData &_info, const std::vector<quint16> &_parts, const Contact &contact);
    virtual ~Reciept()  = default;

    virtual Contact get_contact() const override;
    virtual QByteArray toByteArray() override;
    virtual QUuid get_id() const override;
    virtual bool index_equal(quint16 index) const override;
    virtual bool is_msg() const override;
    virtual bool is_rec() const override;
    virtual bool is_signal() const override;

    quint16 get_key() const;
    quint16 get_rec_key() const;
    quint16 get_received() const;
    std::vector<quint16> get_parts() const;

    void set_info(struct RecData &_info);
    void set_rec_key(quint16 _rec_key);
    void set_parts(const std::vector<quint16> &_parts);
    virtual void set_contact (const Contact &_contact) override;
    void add_part(quint16 part);
};

class StopSignal : public SendData//класс сигнала о конце сообщения
{
private:
    Contact contact;
    struct SignalData info;
public:

    StopSignal(const ByteBlock &block, const Contact &cnt);
    StopSignal(const struct SignalData &_info, const Contact &_contact);
    virtual ~StopSignal()  = default;

    virtual Contact get_contact() const override;
    virtual QByteArray toByteArray() override;

    virtual bool index_equal(quint16 index) const override;
    virtual bool is_msg() const override;
    virtual bool is_rec() const override;
    virtual bool is_signal() const override;

    quint16 get_key() const;
    virtual QUuid get_id() const override;
    QString get_name() const;
    quint16 get_num() const;

    void set_info(struct SignalData &_info);
    virtual void set_contact (const Contact &_contact) override;
};

QDataStream &operator << (QDataStream &in, StopSignal &sig);

QDataStream &operator >> (QDataStream &out, StopSignal &sig);

QDataStream &operator << (QDataStream &in, Reciept &rec);

QDataStream &operator >> (QDataStream &out, Reciept &rec);


#endif // MESSAGE_H
