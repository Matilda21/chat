#ifndef RESENDDIALOG_H
#define RESENDDIALOG_H
#define YES 1
#define NO 0
#include <QWidget>
#include <QDialog>

namespace Ui {
class ResendDialog;
}

class ResendDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResendDialog(QWidget *parent = nullptr);
    ~ResendDialog();

private slots:
    void on_yesButton_clicked();

    void on_noButton_clicked();

private:
    Ui::ResendDialog *ui;
};

#endif // RESENDDIALOG_H
