#include "myobj.h"


myobj::myobj(int ID, QObject *parent) :
    QObject(parent), socketDescriptor(ID)
{
    socket = new QTcpSocket();
    if(!socket->setSocketDescriptor(this->socketDescriptor)) //setting the socket descriptor
    {
        emit error(socket->error());
        qDebug() << "\nCan't set socket descriptor.";
        return;
    }

    connect(socket, SIGNAL(readyRead()), this, SLOT(getName()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
}



void myobj::getName()
{
    QString data = socket->read(socket->bytesAvailable());
    qDebug() << "\n" << data;

    emit gotName(data, this);

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    disconnect(socket, SIGNAL(readyRead()), this, SLOT(getName()));
}


void myobj::send(QByteArray data)
{
    socket->write(data);
}


void myobj::readyRead()
{
    QByteArray input = socket->read(socket->bytesAvailable());
    qDebug() <<"\nRead: " << input;
    emit gotData(input);
}



void myobj::disconnectClient()
{
    socket->disconnectFromHost();
}

void myobj::disconnected()
{
    socket->deleteLater();
    this->deleteLater();
    qDebug() << "\nClient disconnected.";
    emit clientIsGone();
    emit finished(this);
}
