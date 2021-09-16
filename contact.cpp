#include "contact.h"

bool Contact::operator==(const Contact &cnt)
{
    if (ip == cnt.ip && port == cnt.port)
        return true;
    return false;
}


Contact::Contact(QHostAddress _ip, quint16 _port)
{
    ip = _ip;
    port = _port;
}
QHostAddress Contact::get_ip() const
{
    return this->ip;
}

quint16 Contact::get_port() const
{
    return this->port;
}

QHostAddress *Contact::get_ip_ptr()
{
    return &(this->ip);
}

quint16 *Contact::get_port_ptr()
{
    return &(this->port);
}
