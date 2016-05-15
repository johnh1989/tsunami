#-------------------------------------------------
#
# Project created by QtCreator 2013-05-03T18:56:20
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ProjectServer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    file_man.cpp \
    keylogger.cpp \
    screen_cap.cpp \
    server.cpp \
    chat.cpp \
    connectc.cpp \
    myobj.cpp \
    url.cpp

HEADERS  += mainwindow.h \
    file_man.h \
    keylogger.h \
    screen_cap.h \
    server.h \
    chat.h \
    connectc.h \
    myobj.h \
    url.h

FORMS    += mainwindow.ui \
    file_man.ui \
    keylogger.ui \
    screen_cap.ui \
    chat.ui \
    connectc.ui \
    url.ui

RESOURCES += \
    Resources.qrc
