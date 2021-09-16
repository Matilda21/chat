#ifndef RECIEPTDIALOG_H
#define RECIEPTDIALOG_H

#include <QWidget>
#include <QDialog>
//#include "contact.h"
#include <QByteArray>
#include "datagram/message.h"
namespace Ui {
class RecieptDialog;
}

class RecieptDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RecieptDialog(QWidget *parent = nullptr);
    ~RecieptDialog();
    void set_text(const QByteArray &data, const Contact &cnt, quint16 rec_key);

private slots:
    void on_pushButton_clicked();

private:
    Ui::RecieptDialog *ui;
};

#endif // RECIEPTDIALOG_H
