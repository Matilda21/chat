#include "message.h"

quint16 MsgInfo::get_parts_left()
{
    qDebug() << "get parts left called";
    return parts_left;
}

void MsgInfo::delete_part(){parts_left--;}

void  MsgInfo::set_parts_left(quint16 n){parts_left = n;}

MsgInfo::MsgInfo(QUuid _id, quint16 _key, quint16 _parts_left, QByteArray _data):
    id(_id), key(_key), parts_left(_parts_left), data(_data){};

QUuid MsgInfo::get_id(){return id;}

quint16 MsgInfo::get_key(){return key;}

QByteArray MsgInfo::get_data(){return data;}
