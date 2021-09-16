#include "recieptdialog.h"
#include "ui_recieptdialog.h"
#include <string>

RecieptDialog::RecieptDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RecieptDialog)
{
    ui->setupUi(this);
}

void RecieptDialog::set_text(const QByteArray &data, const Contact &cnt, quint16 rec_key)
{
    QString res;
    if (rec_key == IS_FULL)
            res = "было получено адресатом ";
    if (rec_key == REGECT)
            res = "не было получено адресатом ";

    ui->label->setText("Сообщение " + QString(data.data()) + "\n" + res + cnt.get_ip().toString() + " " + std::to_string(cnt.get_port()).c_str());
}

RecieptDialog::~RecieptDialog()
{
    delete ui;
}

void RecieptDialog::on_pushButton_clicked()
{
    this->accept();
    this->done(1);
}
