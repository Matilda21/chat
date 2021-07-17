#include "regwindow.h"
#include "ui_regwindow.h"

RegWindow::RegWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegWindow)
{
    ui->setupUi(this);
}

RegWindow::~RegWindow()
{
    delete ui;
}

void RegWindow::on_regButton_clicked()
{
    std::string name = ui->nameEdit->text().toStdString();
    quint16 port = ui->portEdit->text().toUShort();
    quint16 len = ui->lenEdit->text().toUShort();
    Client *client = new Client(port, name.c_str(), len);
    this->close();
    emit regFinished(client);
}
