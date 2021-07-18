#ifndef SETWINDOW_H
#define SETWINDOW_H

#include <QWidget>

namespace Ui {
class SetWindow;
}

class SetWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SetWindow(QWidget *parent = nullptr);
    ~SetWindow();

private slots:
    void on_okButton_clicked();
signals:
    void setChanged(quint16 max_size, quint16 byte_per_sec);
private:
    Ui::SetWindow *ui;
};

#endif // SETWINDOW_H
