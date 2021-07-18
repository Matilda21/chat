#include "setwindow.h"
#include "ui_setwindow.h"

SetWindow::SetWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetWindow)
{
    ui->setupUi(this);
}

SetWindow::~SetWindow()
{
    delete ui;
}

void SetWindow::on_okButton_clicked()
{
    quint16 max_size, byte_per_sec;
    this->close();
    max_size = ui->sizeEdit->text().toUShort();
    byte_per_sec = ui->speedEdit->text().toUShort();
    emit setChanged(max_size, byte_per_sec);
}
