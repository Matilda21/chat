#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "client.h"
#include "regwindow.h"
#include "setwindow.h"
#include "sendfiledialog.h"
#include "recfiledialog.h"
#include "resenddialog.h"
#include "recieptdialog.h"
#include "errorinfo.h"
#include <QMainWindow>

#define FILE_PATH_ERR "Указан недействительный путь к файлу"
#define ADRESS_ERR "Указан недействительный адрес отправки"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_sendButton_clicked();
    void on_setButton_clicked();
    void on_fileButton_clicked();

public slots:
    void postOutMessage(QByteArray msg);
    void postInMessage(QByteArray msg, quint16 key, QString name);
    void askForResend(Reciept *rec);
    void showRec(const QByteArray &data, const Contact &cnt, quint16 rec_key);
    void showErr(const QString &description);
private:
    Ui::MainWindow *ui;

    Client _client;
    std::vector<Contact> get_contacts();
};
#endif // MAINWINDOW_H
