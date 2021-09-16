#ifndef CONTACT_H
#define CONTACT_H
#include <QString>
#include <QHostAddress>

class Contact
{
public:
    Contact(QHostAddress _ip, quint16 _port);
    Contact() = default;
    ~Contact() = default;
    QHostAddress get_ip() const;
    quint16 get_port() const;
    QHostAddress *get_ip_ptr();
    quint16 *get_port_ptr();
    bool operator==(const Contact &cnt);
private:
    QHostAddress ip;
    quint16 port;
};
#endif // CONTACT_H
