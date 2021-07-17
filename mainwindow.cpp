#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    RegWindow *regWindow = new RegWindow();
    regWindow->show();
    this->connect(regWindow, SIGNAL(regFinished(Client*)), this, SLOT(startChat(Client*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startChat(Client *client)
{
    this->_client = client;
    this->show();
    this->connect(_client, SIGNAL(msgObtained(QByteArray)), this, SLOT(postInMessage(QByteArray)));
    this->connect(_client, SIGNAL(msgSent(QByteArray, QByteArray)), this, SLOT(postOutMessage(QByteArray, QByteArray)));
}

void MainWindow::postOutMessage(QByteArray msg, QByteArray result)
{
    ui->textBrowser->append(msg.data());
    ui->textBrowser->append(result.data());
}

void MainWindow::postInMessage(QByteArray msg)
{
    ui->textBrowser->append(msg.data());
}

void MainWindow::on_sendButton_clicked()
{
    char msg[30] = "Hello from client";
    std::string message = ui->sendEdit->text().toStdString();
    int i = 0;

    for (; i < 29, i < message.length(); i++)
        msg[i] = message[i];
    msg[i] = '\0';
    ui->sendEdit->clear();

    QString data = ui->contactEdit->toPlainText();
    QStringList strList = data.split(QRegExp("[\n]"),QString::SkipEmptyParts);
    //std::vector<quint16> *contacts = new std::vector<quint16>();
    std::vector<Contact> *contacts = new std::vector<Contact>();

    for (QString str : strList)
    {
        QStringList contact = str.split(QRegExp("[ ]"));

        contacts->push_back(*(new Contact(contact.at(0), contact.at(1).toUShort())));
       // contacts->push_back(str.toUShort());
       // std::cout << str.toUShort() << std::endl;
    }

    _client->send(msg, contacts);
}