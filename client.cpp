#include "client.h"

Client::Client(QObject *parent):
    QUdpSocket(parent),
    byte_per_sec(DEFAULT_SPEED),
    n_msg(0),
    timer(new QTimer(this))
{
    timer->setSingleShot(true);

    connect(this, &Client::readyRead, this, &Client::receive);
    connect(this, &Client::startSending, this, &Client::sendSingleMsg);
    connect(timer, &QTimer::timeout, this, &Client::sendSingleMsg);
}

Client::~Client()
{
    while (!outMsgVec.empty())
    {
        delete (outMsgVec)[0];
        outMsgVec.erase(outMsgVec.begin());
        qDebug() << "deleted from outMsgVec" << outMsgVec.size();
    }
    while (!outMsgInfo.empty())
    {
        delete (outMsgInfo)[0];
        outMsgInfo.erase(outMsgInfo.begin());
        qDebug() << "deleted from outMsgInfo";
    }
}

void Client::send_text(QByteArray &msg, const std::vector<Contact> &contacts)
{
    //подготовка текстового сообщения для отправки
    QDataStream in(&msg, QIODevice::ReadOnly);
    QUuid id = QUuid::createUuid();
    quint16 num = splitMsg(in, msg.size(), id, contacts, TXT_MSG);
    emit msgSent(msg);
    outMsgInfo.push_back(new MsgInfo(id, TXT_MSG, num, msg));
    emit startSending();
}

void Client::send_file(QIODevice &file, const std::vector<Contact> &contacts, QString name)
//подготовка файла для отправки
{
    QDataStream in(&file);
    QUuid id = QUuid::createUuid();
    quint16 num = splitMsg(in, file.bytesAvailable(), id, contacts, FILE_MSG);
    outMsgInfo.push_back(new MsgInfo(id, FILE_MSG, num, name.toStdString().c_str()));
    emit msgSent(QByteArray(name.toStdString().c_str()));
    file.close();
    emit startSending();
}

void Client::send(SendData *msg)
{
    //запись особщения в сокет
    qDebug()<<"send";
    Contact receiver = msg->get_contact();
    QByteArray send_text = msg->toByteArray();
    writeDatagram(send_text.data(), send_text.size(),
                        QHostAddress(receiver.get_ip()), receiver.get_port());
    timer->setInterval(round(((double)(send_text.size())/byte_per_sec)*MSEC_PER_SEC));
    timer->start();
}

void Client::sendSingleMsg()
{
    //отправка сообщения или квитанции по таймеру
    if (!timer->isActive() and outMsgVec.size()  > n_msg)
    {
        qDebug()<<"send single msg" << n_msg << outMsgVec.size();
        SendData *msg = outMsgVec.at(n_msg);
        send(msg);

        if (msg->is_rec())
        {
            qDebug() << "send rec" << outMsgVec.size();
            delete_el(outMsgVec, n_msg);
            qDebug() << "send rec" << outMsgVec.size();
         }
         else
            n_msg ++;
    }
}

std::vector<Message> Client::get_parts(QUuid id)
{
//возвращает части сообщения с данным айди
    std::vector<Message> parts;

    for (size_t i = 0; i < inMsgVec.size();)
    {
        if (inMsgVec[i].get_id() == id)
        {
            parts.push_back(inMsgVec[i]);
            inMsgVec.erase(inMsgVec.begin()+i);
        }
        else
            i++;
    }
    return parts;
}

std::vector<quint16> Client::get_parts_nums(QUuid id)
//возвращает неповторяющиеся номера частей сообщения с данным айди
{
    std::vector<quint16> num_parts;

    for (size_t i = 0; i < inMsgVec.size(); i++)
    {
        if (inMsgVec[i].get_id() == id)
            num_parts.push_back(inMsgVec[i].get_cur());
    }
    size_t n = num_parts.size();

    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n - i - 1; j++)
        {
            if (num_parts[j] > num_parts[j+1])
                std::swap(num_parts[j], num_parts[j+1]);
        }
   }
    for (size_t i = 0; i < num_parts.size() - 1; i++)
    {
        if (num_parts[i+1] == num_parts[i])
            num_parts.erase(num_parts.begin() + i);
        else
            i++;
    }
    return num_parts;
}

void Client::sort_parts(std::vector<Message> &parts)
//сортируем части сообщения по порядку
{
    size_t n = parts.size();

    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n - i - 1; j++)
        {
            if (parts[j].get_cur() > parts[j+1].get_cur())
                std::swap(parts[j], parts[j+1]);
        }
   }

}

void Client::merge_parts(const ByteBlock &block, const Contact &contact)
//обьединяем все части сообщения с тем же id что и сигнал конца сообщения
{
     qDebug() << "merge parts called";
     StopSignal sig(block, contact);
     QUuid id = sig.get_id();
     quint16 num = sig.get_num();
     std::vector<quint16> num_parts = get_parts_nums(id);

     struct RecData info = {REC_MSG, id, num_parts.size(), IS_FULL};
     Reciept *rec = new Reciept(info, num_parts, sig.get_contact());

     if (num_parts.size() == num)
    {
        qDebug() << "all parts of message obtained";
        std::vector<Message> parts = get_parts(id);
        sort_parts(parts);

        for (size_t i = 1; i < parts.size(); i++)//склеиваем части сообщения
        {
            if (parts[i].get_cur() != parts[0].get_cur())
                parts[0].add(parts[i]);
        }
        add_msg(rec);
        emit msgObtained(parts[0].get_data(), parts[0].get_key(), sig.get_name());
    }
    else
        emit msgNotFull(rec);
}

void Client::resend_part(quint16 i)
//перемещение части сообщения для поворной отправки
{
    qDebug() << "resend part called";
    SendData *tmp = outMsgVec[i];
    outMsgVec.erase(outMsgVec.begin() + i);
    n_msg--;
    outMsgVec.push_back(tmp);
}

void Client::resend_message(const Reciept &rec)
//доотправка недошедших частей сообщения по квитанции и удаление дошедших частей
{
    QUuid id = rec.get_id();
    Contact cnt = rec.get_contact();
    MsgInfo *info = get_full_msg(id);
    quint16 sig_pos;

    for (int i = 0; i < n_msg;)
    {
        if (id == outMsgVec[i]->get_id() and outMsgVec[i]->get_contact() == cnt and outMsgVec[i]->is_msg())
        {
                Message *msg_part = dynamic_cast<Message *>(outMsgVec[i]) ;

                if (rec.index_equal(msg_part->get_cur()))
                {
                        qDebug() << "MESSAGE WAS EXCLUDED";
                        delete_el(outMsgVec, i);
                        info->delete_part();
                        n_msg--;

                }
                else //перемещаем вперед недоставленную часть
                        resend_part(i);
        }
        else
         {
              if (id == outMsgVec[i]->get_id() and outMsgVec[i]->is_signal() and outMsgVec[i]->get_contact() == cnt)
                  sig_pos = i;
               i++;
         }
    }
    resend_part(sig_pos);
    emit startSending();
}

void Client::delete_message(const Reciept &rec)
//удаление сообщения по квитанции
{
    QUuid id = rec.get_id();
    Contact cnt = rec.get_contact();
    MsgInfo *info = get_full_msg(id);

    for (int i = 0; i < n_msg;)
    {
        if (id == outMsgVec[i]->get_id() and outMsgVec[i]->get_contact() == cnt)
        {
                qDebug() << "MESSAGE WAS EXCLUDED";
                delete_el(outMsgVec, i);
                info->delete_part();
                n_msg--;
        }
        else
            i++;
    }
    if (info->get_parts_left() == 0)
        clean_info_vec(id);
}

void Client::process_rec(const ByteBlock &block, const Contact &contact)//обработка квитанции
{
//обработка квитанции
    qDebug() << "process_rec called";
    Reciept rec(block, contact);
    quint16 rec_key = rec.get_rec_key();
    show_rec(rec);

    if (rec_key == ASK)
        resend_message(rec);
    else
        delete_message(rec);
}


void Client::clean_info_vec(QUuid id)
//если все части сообщения с данным айди удалены то удаляем сообщение из outMsgInfo
{
    for (int i = 0; i < outMsgInfo.size(); i++)
    {
        if (outMsgInfo.at(i)->get_id() == id)
        {
            delete_el(outMsgInfo, i);
            qDebug() << "deleted message from outMsgInfo";
         }
    }
}

MsgInfo *Client::get_full_msg(QUuid id)
{
    qDebug() << "get full msg called";
    for (MsgInfo *mi : outMsgInfo)
        if (mi->get_id() == id)
        {
            qDebug() << "found full msg";
            return mi;
        }
}

void Client::show_rec(const Reciept &rec)
{
    qDebug() << "show rec called";
    QByteArray data = get_full_msg(rec.get_id())->get_data();

    if (rec.get_rec_key() == IS_FULL)
        emit gotRec(data, rec.get_contact(), IS_FULL);
    if (rec.get_rec_key() == REGECT)
        emit gotRec(data, rec.get_contact(), REGECT);
}

void Client::receive()
//получение сообщения или квитанции из сокета
{
    qDebug() << "receive called";
    ByteBlock block;
    Contact cnt;

    while (hasPendingDatagrams())
    {
        block.resize(pendingDatagramSize());

        readDatagram(block.data(), block.size(),
               cnt.get_ip_ptr(), cnt.get_port_ptr());

        if (block.is_rec()) //если сообщение - квитанция          
             process_rec(block, cnt);
        else if (block.is_signal()) //если сообщение - квитанция
             merge_parts(block, cnt);
        else
        {
            Message msg(block, cnt);
            inMsgVec.push_back(msg);
        }
    }
}

quint16 Client::splitMsg(QDataStream &in, quint16 full_size, QUuid id, const std::vector<Contact> &contacts, quint16 type)
{
    //разбиение сообщения на части
    QByteArray block(max_size, 0);
    quint16 num = full_size / max_size + 1;
    Message *mg;
    struct SignalData sd = {SIGNAL_MSG, name, id, num};

    for (quint16 num_part = 1; num_part <= num; num_part++)
    {
        if (full_size < block.size())
            block.resize(full_size);

        quint16 size = in.readRawData(block.data(), block.size());
        full_size -= size;
        struct MessageData md = {type, id, num_part, size};

        for (Contact cnt : contacts)
            outMsgVec.push_back(new Message(md, block, cnt));
    }
    for (Contact cnt : contacts)
        outMsgVec.push_back(new StopSignal(sd, cnt));
    emit startSending();
    return num * contacts.size();
}

void Client::add_msg(SendData *msg)
{
    outMsgVec.push_back(msg);
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

void Client::setName(const QString &_name)
{
    name =_name;
}


