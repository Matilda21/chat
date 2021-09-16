#include "errorinfo.h"
#include "ui_errorinfo.h"

ErrorInfo::ErrorInfo(QWidget *parent) :
    QMessageBox(parent),
    ui(new Ui::ErrorInfo)
{
    ui->setupUi(this);
}

ErrorInfo::~ErrorInfo()
{
    delete ui;
}
