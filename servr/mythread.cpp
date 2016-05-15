#include "mythread.h"

mythread::mythread(int ID, QObject *parent) :
    QThread(parent)
{
    socketDescriptor = ID;
}


void mythread::run()
{
    socket = new QTcpSocket();
    if(!socket->setSocketDescriptor(this->socketDescriptor)) //setting the socket descriptor
    {
        emit error(socket->error());
        qDebug() << "\nCan't set socket descriptor.";
        return;
    }

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    //test send - socket->write("You've connected to Project Tsunami!");

    exec();
}



void mythread::readyRead()
{
    QString input = socket->readAll();
    qDebug() <<"\nRead: " << input;
    emit statusChanged(input);
}

void mythread::disconnected()
{
    socket->deleteLater();
    qDebug() << "\nClient disconnected.";
    exit(0);
}

