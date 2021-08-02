#include "recfiledialog.h"
#include "ui_recfiledialog.h"

RecFileDialog::RecFileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RecFileDialog)
{
    ui->setupUi(this);
}

RecFileDialog::~RecFileDialog()
{
    delete ui;
}

QString RecFileDialog::getFilePath()
{
    return ui->pathEdit->text();
}

void RecFileDialog::on_saveButton_clicked()
{
    this->accept();
}
