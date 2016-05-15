#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QString>


class mythread : public QObject
{
    Q_OBJECT

    QTcpSocket *socket;
    int socketDescriptor;

public:
    explicit mythread(int ID, QObject *parent = 0);
    void run();
    
signals:
    void error(QTcpSocket::SocketError socketerror);
    void statusChanged(QString);
    
public slots:
    void readyRead();
    void disconnected();
    
};

#endif // MYTHREAD_H
