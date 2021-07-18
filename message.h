#ifndef MESSAGE_H
#define MESSAGE_H
#include "contact.h"
class Message
{
private:
    std::vector<Contact> contacts;
    QByteArray text;

public:
    Message(QByteArray _text, std::vector<Contact> _contacts)
    {
        contacts = _contacts;
        text = _text;
    }

    QByteArray get_text(){return this->text;};
    QByteArray add_name(QByteArray name)
    {
       QByteArray tmp = text;
       tmp.insert(0, ": ");
       tmp.insert(0, name);
       return tmp;
    }
    Contact get_contact()
    {
        return contacts.back();
    }
    void pop_contact()
    {
        std::cout << contacts.back().get_port() << "pop_contact"<< contacts.size() << std::endl;
        contacts.pop_back();
    }
    bool is_sent()
    {
        return contacts.empty();
    }
    quint16 get_size(){return this->text.size();};

};

#endif // MESSAGE_H
