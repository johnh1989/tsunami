#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>
#include <QDebug>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QString>
#include <QHostAddress>
#include <Windows.h>


//**************************************//
//**************************************//
//***   Client - Connection Module   ***//
//***      Class Header File         ***//
//**************************************//
//**************************************//


//This class is responsible for capturing the client PCs screen. It grabs the screen and saves it in a
//Pixmap object. It then converts the object to a QBuffer object. Then it send the QBuffer to the server in
//the form of a QByteArray. The server converts it back to a QBuffer and then to a Pixmap object which is
//displayed and can be saved.



//Inherits QObject class
class tcpsocket : public QObject
{
    Q_OBJECT

    QTcpSocket *sock;
    QString myName;
    QByteArray command;
    bool varConnected;
    void connectToServer();


public:
    explicit tcpsocket(QObject *parent = 0, QString name = "Client");    
    bool isConnected();  //returns true if client is connected to the server

signals:
    void recievedCommand(QByteArray);  //emits this signal whenever data is available
    void disconnectedFromHost();  //emits this signal when the client is disconnected from the host

public slots:
    void sendReply(QByteArray);  //function for sending data to the server
    void connected();  //function for sendig name when connected to the server
    void disconnected();  //function for connecting again to the server after disconnect
    void readyRead();  //function for reading data
    void bytesWritten(qint64 bytes); //function which tells about the number of bytes written



};

#endif // TCPSOCKET_H
