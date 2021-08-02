#ifndef SENDFILEDIALOG_H
#define SENDFILEDIALOG_H
#include <QDialog>
#include <QWidget>

namespace Ui {
class SendFileDialog;
}

class SendFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SendFileDialog(QWidget *parent = nullptr);
    ~SendFileDialog();
    QString getFilePath();

private slots:
    void on_sendButton_clicked();

private:
    Ui::SendFileDialog *ui;
};

#endif // SENDFILEDIALOG_H
