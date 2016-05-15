#ifndef SCREENCAP_H
#define SCREENCAP_H

#include <QObject>
#include <QBuffer>
#include <QPixmap>
#include <QScreen>
#include <QGuiApplication>
#include "tcpsocket.h"


//**************************************//
//**************************************//
//*** Client - Screen Capture Module ***//
//***      Class Header File         ***//
//**************************************//
//**************************************//


//This class is responsible for capturing the client PCs screen. It grabs the screen and saves it in a
//Pixmap object. It then converts the object to a QBuffer object. Then it send the QBuffer to the server in
//the form of a QByteArray. The server converts it back to a QBuffer and then to a Pixmap object which is
//displayed and can be saved.



//Inherits QObject class
class screenCap : public QObject
{
    Q_OBJECT
    tcpsocket *communicationSocket;

public:
    explicit screenCap(QObject *parent = 0, tcpsocket *comSock = NULL); //constructor
    
signals:
    
public slots:
    void recievedCommand(QByteArray input);  //The main function which gets and executes commands recieved fron the server
    
};

#endif // SCREENCAP_H
