#ifndef RECFILEDIALOG_H
#define RECFILEDIALOG_H
#include <QDialog>
#include <QWidget>

namespace Ui {
class RecFileDialog;
}

class RecFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RecFileDialog(QWidget *parent = nullptr);
    ~RecFileDialog();
    QString getFilePath();

private slots:
    void on_saveButton_clicked();

private:
    Ui::RecFileDialog *ui;
};

#endif // RECFILEDIALOG_H
