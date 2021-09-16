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
    this->connect(&_client, &Client::msgObtained, this, &MainWindow::postInMessage);
    this->connect(&_client, &Client::msgSent, this, &MainWindow::postOutMessage);
    this->connect(&_client, &Client::msgNotFull, this, &MainWindow::askForResend);
    this->connect(&_client, &Client::gotRec, this, &MainWindow::showRec);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showErr(const QString &description)
{
    ErrorInfo box;
    box.setText(description);
    box.exec();
}


void MainWindow::postOutMessage(QByteArray msg)
{
    QByteArray text = msg;
    text.insert(0, "You: ");
    ui->textBrowser->append(text.data());
}

void MainWindow::postInMessage(QByteArray msg, quint16 key, QString name)
{
    if (key == FILE_MSG)
    {
        RecFileDialog dialog;

        if (dialog.exec() == QDialog::Accepted);
        {
            QFile file(dialog.getFilePath());

            if (file.open(QIODevice::WriteOnly) == false)
            {
                showErr(FILE_PATH_ERR);
                postInMessage(msg, key, name);
                return;
            }
            QDataStream in(&file);
            in.writeRawData(msg.data(), msg.size());
            file.close();
            ui->textBrowser->append(name + ": " + "File was downloaded");
        }
        dialog.done(0);
    }
    else
        ui->textBrowser->append(name + ": " + msg.data());
}


void MainWindow::showRec(const QByteArray &text, const Contact &cnt, quint16 rec_key)
{
    RecieptDialog rd;

    rd.set_text(text, cnt, rec_key);
    rd.exec();

}

void MainWindow::askForResend(Reciept *rec)
{
    ResendDialog rd;
    int result = rd.exec();

    if (result == QDialog::Accepted or result == QDialog::Rejected)
    {
        if (rd.result() == YES)
            rec->set_rec_key(ASK);
        if (rd.result() == NO)
            rec->set_rec_key(REGECT);
        _client.add_msg(rec);
    }
    rd.done(0);
}

void MainWindow::on_sendButton_clicked()
{
    QByteArray msg(ui->sendEdit->text().toStdString().c_str());
    qDebug() << "message = " << msg.data();  
    std::vector<Contact> contacts = get_contacts();

    if (contacts.size() > 0)
    {
        ui->sendEdit->clear();
       _client.send_text(msg, contacts);
    }

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

std::vector<Contact> MainWindow::get_contacts()
{
    std::vector<Contact> contacts;
    QString data = ui->contactEdit->toPlainText();
    QStringList strList = data.split(QRegExp("[\n]"),QString::SkipEmptyParts);
    qDebug() << strList.size();

    if (strList.size() == 0)
        showErr(ADRESS_ERR);
    else
    {
        for (QString str : strList)
        {
            QStringList contact = str.split(QRegExp("[ ]"));
            if (contact.size() != 2)
            {
                showErr(ADRESS_ERR);
                return contacts;
            }
            contacts.push_back(Contact(QHostAddress(contact.at(0)), contact.at(1).toUShort()));
        }
    }
    return contacts;
}


void MainWindow::on_fileButton_clicked()
{
    SendFileDialog dialog;

    if (dialog.exec() == QDialog::Accepted)
    {
        QFile file(dialog.getFilePath());

        if (file.exists() == false)
        {
            showErr(FILE_PATH_ERR);
            return;
        }

        file.open(QIODevice::ReadOnly);
        std::vector<Contact> contacts = get_contacts();

        if (contacts.size() > 0)
            _client.send_file(file, contacts, dialog.getFilePath());

    }
    dialog.done(0);
}
