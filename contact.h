#ifndef CONTACT_H
#define CONTACT_H
class Contact
{
public:
    Contact(QString _ip, quint16 _port)
    {
        ip = _ip;
        port = _port;
    }
    QString get_ip()
    {
        return this->ip;
    }
    quint16 get_port()
    {
        return this->port;
    }
private:
    QString ip;
    quint16 port;
};
#endif // CONTACT_H
