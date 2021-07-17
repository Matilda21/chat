#include "mainwindow.h"
#include "client.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
   // RegWindow r;
   // r.show();
    //w.show();
    return a.exec();
}
