#ifndef ERRORINFO_H
#define ERRORINFO_H

#include <QWidget>
#include <QMessageBox>

namespace Ui {
class ErrorInfo;
}

class ErrorInfo : public QMessageBox
{
    Q_OBJECT

public:
    explicit ErrorInfo(QWidget *parent = nullptr);
    ~ErrorInfo();

private:
    Ui::ErrorInfo *ui;
};

#endif // ERRORINFO_H
