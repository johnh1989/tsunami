#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <QString>
#include <QDesktopServices>
#include <QUrl>
#include <QFile>
#include <QProcess>
#include "tcpsocket.h"


//*************************************//
//*************************************//
//***  Client - File Manager Module ***//
//***      Class Header File        ***//
//*************************************//
//*************************************//


//This class has all the functions related to file managing. It can sending and recieve files. It can open
//files and execute exes too. It sends all the information required by the brother of this module which
//resides on the server side.


//Inherits QObject class
class fileManager : public QObject
{
    Q_OBJECT

    //Objects and variables which the class needs to work
    QDir myDir;
    QFile myFile;
    int fileSize;
    int beingWritten;
    int bytesWritten;
    QString outputBuffer;
    tcpsocket *communicationSocket;

    void addDrives();  //Adds a list of the drives on the client pc to the buffer
    void addEnteries();  //Adds a list of the files and folder in the current directory to the buffer
    void sendTextReply();  //Sends the completely formed buffer to the server

public:
    explicit fileManager(QObject *parent = 0, tcpsocket *mySock = 0); //Constructor

signals:
    //This class doesn't emit any signals

public slots:
    void recievedCommand(QByteArray input);  //The main function which gets and executes commands recieved fron the server
};

#endif // FILEMANAGER_H
