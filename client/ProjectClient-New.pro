#-------------------------------------------------
#
# Project created by QtCreator 2013-05-19T20:17:51
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ProjectClient-New
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    filemanager.cpp \
    client.cpp \
    tcpsocket.cpp \
    tkey.cpp \
    chat.cpp \
    screencap.cpp

HEADERS  += mainwindow.h \
    filemanager.h \
    client.h \
    tcpsocket.h \
    tkey.h \
    chat.h \
    screencap.h

FORMS    += mainwindow.ui \
    chat.ui
