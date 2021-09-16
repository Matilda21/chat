#include "message.h"

QDataStream &operator << (QDataStream &in, Reciept &rec)
{
    in << rec.get_key();
    in.writeRawData(rec.get_id().toByteArray().data(), ID_SIZE);
    qDebug() << "reciept id = " <<rec.get_id().toByteArray().data();
    in << rec.get_received();

    for (quint16 part : rec.get_parts())
    {
        qDebug() << "rd part puted = " << part;
        in << part;
    }
    in << rec.get_rec_key();
    return in;
}

QDataStream &operator >> (QDataStream &out, Reciept &rec)
{
    struct RecData rd;
    std::vector<quint16> parts;

    out >> rd.key;
    qDebug() << "rd key = " << rd.key;
    char id[37];
    out.readRawData(id, ID_SIZE);
    rd.id = QUuid(id);
    qDebug() << "rd id = " << rd.id;
    out >> rd.received;
    qDebug() << "rd received = " << rd.received;

    quint16 part;
    for (quint16 i = 0; i < rd.received; i++)
    {
        out >> part;
        qDebug() << "rd part received = " << part;
        parts.push_back(part);

    }
    out >> rd.rec_key;
    qDebug() << "rd rec_key = " << rd.rec_key;
    rec.set_info(rd);
    rec.set_parts(parts);
    return out;
}

Contact Reciept::get_contact() const
{
    return contact;
}

void Reciept::set_contact(const Contact &cnt)
{
    contact = cnt;
}

QByteArray Reciept::toByteArray()
{
        qDebug() << "Reciept to byte array called" << this->info.id;
        QByteArray in;
        QDataStream stream(&in, QIODevice::WriteOnly);
        stream << *(this);
        return in;
}

QUuid Reciept::get_id() const
{
    return info.id;
}

bool Reciept::index_equal(quint16 index) const
{
    qDebug() << "Reciept::index_equal called " << parts;
    for (quint16 part : parts)
        if (index == part)
        {
            qDebug() << "equal index = " << part;
            return true;

        }
    return false;
}

Reciept::Reciept(const ByteBlock &block, const Contact &cnt)
{
    QByteArray ba = block.toByteArray();
    QDataStream stream(&ba, QIODevice::ReadOnly);
    stream >> *(this);
    contact = cnt;
}

Reciept::Reciept(const struct RecData &_info, const std::vector<quint16> &_parts, const Contact &_contact)
{
    info = _info;
    parts = _parts;
    contact = _contact;
    qDebug() << "info_id = "<< this->info.id;
    qDebug() << "parts = "<< this->parts;

}

quint16 Reciept::get_key() const
{
    return info.key;
}

bool Reciept::is_msg() const
{
    return false;
}

bool Reciept::is_rec() const
{
    return true;
}

bool Reciept::is_signal() const
{
    return false;
}

quint16 Reciept::get_received() const
{
    return info.received;
}

std::vector<quint16> Reciept::get_parts() const
{
    return parts;
}

quint16 Reciept::get_rec_key() const
{
    return info.rec_key;
}

void Reciept::set_info(struct RecData &_info)
{
    info = _info;
}

void Reciept::set_parts(const std::vector<quint16> &_parts)
{
    parts = _parts;
}

void Reciept::set_rec_key(quint16 _rec_key)
{
    info.rec_key = _rec_key;

}

void Reciept::add_part(quint16 part)
{
    parts.push_back(part);
}
