#include "tcpsocket.h"

tcpsocket::tcpsocket(QObject *parent, QString name) :
    QObject(parent), myName(name)
{
    sock = new QTcpSocket(this);

    connect(sock, SIGNAL(connected()), this, SLOT(connected()));
    connect(sock, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(sock, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
    connect(sock, SIGNAL(readyRead()), this, SLOT(readyRead()));

    connectToServer();
}


void tcpsocket::connectToServer()
{
    sock->connectToHost("localhost", 5589);

    sock->flush();

    if(!sock->waitForConnected(15000))
    {
        qDebug() << "\nError: " << sock->errorString();

        qDebug() << "\nTrying again in 5s.";

        Sleep(5000);

        connectToServer();
    }
}


void tcpsocket::connected()
{
    varConnected = true;

    qDebug() << "\nConnected to server.";

    qDebug() << "\nSending name.";

    QByteArray temp = myName.toLocal8Bit();

    sock->write(temp);
}



void tcpsocket::disconnected()
{
    varConnected = false;

    qDebug() << "\nDisconnected from host. Connecting again in 5s.";

    Sleep(5000);

    emit disconnectedFromHost();

    connectToServer();
}


void tcpsocket::bytesWritten(qint64 bytes)
{
    qDebug() << bytes << " bytes sent.";
}


void tcpsocket::readyRead()
{
    qDebug() << "\nRecieving data...";

    command = sock->read(sock->bytesAvailable());

    qDebug() << command;

    emit recievedCommand(command);
}




void tcpsocket::sendReply(QByteArray reply)
{
    sock->write(reply);
    sock->flush();
}


bool tcpsocket::isConnected()
{
    return varConnected;
}
