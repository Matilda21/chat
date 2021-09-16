#include "resenddialog.h"
#include "ui_resenddialog.h"


ResendDialog::ResendDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResendDialog)
{
    ui->setupUi(this);
}

ResendDialog::~ResendDialog()
{
    delete ui;
}

void ResendDialog::on_yesButton_clicked()
{
    qDebug("yes");
    this->setResult(YES);
    this->accept();
}

void ResendDialog::on_noButton_clicked()
{
    qDebug("no");
    this->setResult(NO);
    this->reject();
}
