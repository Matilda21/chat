#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "client.h"
#include "regwindow.h"
#include "setwindow.h"
#include <QMainWindow>

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

public slots:
    void postOutMessage(QByteArray msg, QByteArray result);
    void postInMessage(QByteArray msg);
    void startChat(Client *client);
    void changeSettings(quint16 max_size, quint16 byte_per_sec);

private:
    Ui::MainWindow *ui;
    SetWindow *sWin;
public:
    Client *_client;
};
#endif // MAINWINDOW_H
