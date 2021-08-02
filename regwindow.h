#ifndef REGWINDOW_H
#define REGWINDOW_H
#include "client.h"
#include <QWidget>
#include <QDialog>

namespace Ui {
class RegWindow;
}

class RegWindow : public QDialog
{
    Q_OBJECT

public:
    explicit RegWindow(QWidget *parent = 0);
    ~RegWindow();
    QByteArray getName();
    quint16 getPort();
    quint16 getLen();
private slots:
    void on_regButton_clicked();

private:
    Ui::RegWindow *ui;
};

#endif // REGWINDOW_H
