#include "contact.h"

Contact::Contact(QString _ip, quint16 _port)
{
    ip = _ip;
    port = _port;
}
QString Contact::get_ip()
{
    return this->ip;
}
quint16 Contact::get_port()
{
    return this->port;
}
