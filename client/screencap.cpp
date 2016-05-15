#include "screencap.h"

screenCap::screenCap(QObject *parent, tcpsocket *comSock):
    QObject(parent), communicationSocket(comSock)
{
}


void screenCap::recievedCommand(QByteArray input)
{
    if(input == "take")
    {
        QString temp;

        QScreen *screen = QGuiApplication::primaryScreen();

        QPixmap myPixmap = screen->grabWindow(0);

        QByteArray myArray;

        QBuffer myBuffer(&myArray);

        myBuffer.open(QIODevice::WriteOnly);

        myPixmap.save(&myBuffer, "JPG");

        communicationSocket->sendReply("b" + (temp.number(myArray.size())).toLocal8Bit() + "file" + myArray);
    }
}

