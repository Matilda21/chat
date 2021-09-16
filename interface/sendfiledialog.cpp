#include "sendfiledialog.h"
#include "ui_sendfiledialog.h"

SendFileDialog::SendFileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SendFileDialog)
{
    ui->setupUi(this);
}

SendFileDialog::~SendFileDialog()
{
    delete ui;
}

QString SendFileDialog::getFilePath()
{
    return ui->pathEdit->text();
}

void SendFileDialog::on_sendButton_clicked()
{
    this->accept();
}
