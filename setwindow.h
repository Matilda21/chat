#ifndef SETWINDOW_H
#define SETWINDOW_H

#include <QDialog>

namespace Ui {
class SetWindow;
}

class SetWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SetWindow(QWidget *parent = nullptr);
    ~SetWindow();
    quint16 getMaxSize();
    quint16 getSpeed();

private slots:
    void on_okButton_clicked();
private:
    Ui::SetWindow *ui;
};

#endif // SETWINDOW_H
