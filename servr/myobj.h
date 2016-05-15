#ifndef MYOBJ_H
#define MYOBJ_H

#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QString>
#include <QTextCodec>

class myobj : public QObject
{
    Q_OBJECT

    QTcpSocket *socket;
    int socketDescriptor;


public:
    explicit myobj(int ID, QObject *parent = 0);
    void send(QByteArray);
    void disconnectClient();
    
signals:
    void error(QTcpSocket::SocketError socketerror);
    void finished(myobj *);
    void gotName(QString, myobj *);
    void gotData(QByteArray);
    void clientIsGone();

public slots:
    void disconnected();
    void getName();
    void readyRead();
    
};

#endif // MYOBJ_H
