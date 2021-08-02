#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "client.h"
#include "regwindow.h"
#include "setwindow.h"
#include "sendfiledialog.h"
#include "recfiledialog.h"
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
    void on_fileButton_clicked();

public slots:
    void postOutMessage(QByteArray msg, QByteArray result);
    void postInMessage(QByteArray msg, quint16 key);

private:
    Ui::MainWindow *ui;

    Client _client;
};
#endif // MAINWINDOW_H
