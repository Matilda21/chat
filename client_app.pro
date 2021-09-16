QT       += core gui
QT       += network
QT += core
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    datagram/byteblock.cpp \
    client.cpp \
    contact.cpp \
    interface/errorinfo.cpp \
    main.cpp \
    interface/mainwindow.cpp \
    datagram/message.cpp \
   # outmsgqueue.cpp \
    datagram/msginfo.cpp \
    interface/recfiledialog.cpp \
    datagram/reciept.cpp \
    interface/recieptdialog.cpp \
    interface/regwindow.cpp \
    interface/resenddialog.cpp \
    interface/sendfiledialog.cpp \
    interface/setwindow.cpp \
    datagram/stopsignal.cpp

HEADERS += \
    client.h \
    interface/errorinfo.h \
    interface/mainwindow.h \
    datagram/message.h \
   # outmsgqueue.h \
    interface/recfiledialog.h \
    interface/recieptdialog.h \
    interface/regwindow.h \
    contact.h \
    interface/resenddialog.h \
    interface/sendfiledialog.h \
    interface/setwindow.h

FORMS += \
    interface/errorinfo.ui \
    interface/mainwindow.ui \
    interface/recfiledialog.ui \
    interface/recieptdialog.ui \
    interface/regwindow.ui \
    interface/resenddialog.ui \
    interface/sendfiledialog.ui \
    interface/setwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
