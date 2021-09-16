#include "message.h"


QDataStream &operator << (QDataStream &in, StopSignal &sig)
{
    in << sig.get_key();
    in.writeRawData(sig.get_name().toStdString().c_str(), MAX_NAME_LEN);
    qDebug() << "signal name = " << sig.get_name().toStdString().c_str();
    in.writeRawData(sig.get_id().toByteArray().data(), ID_SIZE);
    qDebug() << "signal id = " <<sig.get_id().toByteArray().data();
    in << sig.get_num();
    return in;
}

QDataStream &operator >> (QDataStream &out, StopSignal &sig)
{
    struct SignalData sd;

    out >> sd.key;
    qDebug() << "sd key = " << sd.key;

    char ch_name[MAX_NAME_LEN];
    out.readRawData(ch_name, MAX_NAME_LEN);
    sd.name = QString(ch_name);

    qDebug() << "sd name = " << sd.name;

    char id[37];
    out.readRawData(id, ID_SIZE);
    sd.id = QUuid(id);
    qDebug() << "sd id = " << sd.id;
    out >> sd.num;
    qDebug() << "sd num = " << sd.num;

    sig.set_info(sd);
    return out;
}

Contact StopSignal::get_contact() const
{
    return contact;
}

//bool StopSignal::is_sent() const
//{
//    return contacts.empty();
//}

//quint16 StopSignal::num_contacts() const
//{
//    return contacts.size();
//}


QByteArray StopSignal::toByteArray()
{
        qDebug() << "Signal to byte array called" << this->info.id;
        QByteArray in;
        QDataStream stream(&in, QIODevice::WriteOnly);
        stream << *(this);
        return in;
}

QUuid StopSignal::get_id() const
{
    return info.id;
}

bool StopSignal::index_equal(quint16 index) const
{
    return false;
}


StopSignal::StopSignal(const ByteBlock &block, const Contact &cnt)
{
    qDebug() << "Signal 1 constructor";
    QByteArray ba = block.toByteArray();
    QDataStream stream(&ba, QIODevice::ReadOnly);
    stream >> *(this);
    contact = cnt;
}

StopSignal::StopSignal(const struct SignalData &_info, const Contact &_contact)
{
    info = _info;
    contact = _contact;

}

quint16 StopSignal::get_key() const
{
    return info.key;
}

QString StopSignal::get_name() const
{
    return info.name;
}

bool StopSignal::is_msg() const
{
    return false;
}

bool StopSignal::is_rec() const
{
    return false;
}

bool StopSignal::is_signal() const
{
    return true;
}

quint16 StopSignal::get_num() const
{
    return info.num;
}

void StopSignal::set_info(struct SignalData &_info)
{
    info = _info;
}

void StopSignal::set_contact(const Contact &_contact)
{
    contact = _contact;
  //  contacts.push_back(_contact);
}
