#ifndef REGWINDOW_H
#define REGWINDOW_H
#include "client.h"
#include <QWidget>

namespace Ui {
class RegWindow;
}

class RegWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RegWindow(QWidget *parent = 0);
    ~RegWindow();
signals:
    void regFinished(Client *client);
private slots:
    void on_regButton_clicked();

private:
    Ui::RegWindow *ui;
};

#endif // REGWINDOW_H
