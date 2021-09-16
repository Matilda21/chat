#include "setwindow.h"
#include "ui_setwindow.h"

SetWindow::SetWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetWindow)
{
    ui->setupUi(this);
}

SetWindow::~SetWindow()
{
    delete ui;
}

quint16 SetWindow::getMaxSize()
{
    return ui->sizeEdit->text().toUShort();
}

quint16 SetWindow::getSpeed()
{
    return ui->speedEdit->text().toUShort();

}

void SetWindow::on_okButton_clicked()
{
    this->accept();
}
