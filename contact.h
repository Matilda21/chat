#ifndef CONTACT_H
#define CONTACT_H
#include <QString>

class Contact
{
public:
    Contact(QString _ip, quint16 _port);
    QString get_ip();
    quint16 get_port();
private:
    QString ip;
    quint16 port;
};
#endif // CONTACT_H
