#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);  
    RegWindow regWindow;

    if (regWindow.exec() == QDialog::Accepted){
        _client.setName(regWindow.getName());
        _client.setPort(regWindow.getPort());
        _client.setMaxSize(regWindow.getLen());
    }
    this->connect(&_client, SIGNAL(msgObtained(QByteArray, quint16)), this, SLOT(postInMessage(QByteArray, quint16)));
    this->connect(&_client, SIGNAL(msgSent(QByteArray, QByteArray)), this, SLOT(postOutMessage(QByteArray, QByteArray)));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::postOutMessage(QByteArray msg, QByteArray result)
{
    QByteArray text = msg;
    text.insert(0, "You: ");
    ui->textBrowser->append(text.data());
    ui->textBrowser->append(result.data());
}

void MainWindow::postInMessage(QByteArray msg, quint16 key)
{
    if (key == FILE_MSG)
    {
        RecFileDialog dialog;

        if (dialog.exec() == QDialog::Accepted);
        {
            QFile *file =new QFile(dialog.getFilePath());
            file->open(QIODevice::WriteOnly);
            QDataStream in(file);
            in.writeRawData(msg.data(), msg.size());
            file->close();
            ui->textBrowser->append("File was downloaded");
        }
        dialog.done(0);
    }
    else
        ui->textBrowser->append(msg.data());
}

void MainWindow::on_sendButton_clicked()
{

    QByteArray msg(ui->sendEdit->text().toStdString().c_str());
    qDebug() << "message = " << msg.data();
    ui->sendEdit->clear();

    QString data = ui->contactEdit->toPlainText();
    QStringList strList = data.split(QRegExp("[\n]"),QString::SkipEmptyParts);
    std::vector<Contact> contacts;

    for (QString str : strList)
    {
        QStringList contact = str.split(QRegExp("[ ]"));

        contacts.push_back(Contact(contact.at(0), contact.at(1).toUShort()));
    }

    _client.send_text(msg, contacts);
}

void MainWindow::on_setButton_clicked()
{
    SetWindow sWin;

    if (sWin.exec() == QDialog::Accepted)
    {
        _client.setMaxSize(sWin.getMaxSize());
        _client.setSpeed(sWin.getSpeed());
    }
    sWin.done(0);
}


void MainWindow::on_fileButton_clicked()
{
    SendFileDialog dialog;

    if (dialog.exec() == QDialog::Accepted)
    {
        QFile *file = new QFile(dialog.getFilePath());
        file->open(QIODevice::ReadOnly);

        QString data = ui->contactEdit->toPlainText();
        QStringList strList = data.split(QRegExp("[\n]"),QString::SkipEmptyParts);
        std::vector<Contact> contacts;

        for (QString str : strList)
        {
            QStringList contact = str.split(QRegExp("[ ]"));

            contacts.push_back(Contact(contact.at(0), contact.at(1).toUShort()));
        }

        _client.send_file(file, contacts);
    }
    dialog.done(0);


}
