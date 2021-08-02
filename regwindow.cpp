#include "regwindow.h"
#include "ui_regwindow.h"

RegWindow::RegWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegWindow)
{
    ui->setupUi(this);
}

RegWindow::~RegWindow()
{
    delete ui;
}

QByteArray RegWindow::getName()
{
    QByteArray name(ui->nameEdit->text().toStdString().c_str(), -1);
    return name;
}

quint16 RegWindow::getPort()
{
    return ui->portEdit->text().toUShort();
}

quint16 RegWindow::getLen()
{
    return ui->lenEdit->text().toUShort();
}

void RegWindow::on_regButton_clicked()
{
    this->accept();
}
