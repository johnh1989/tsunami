#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include "myobj.h"
#include <QDebug>
#include <QString>
#include <QVector>

class server : public QTcpServer
{
    Q_OBJECT
    QVector<QString> clientNames;
    QVector<myobj *> clientPointers;
    myobj *connectedTo;
    QString dataRecieved;


public:
    explicit server(QObject *parent = 0);
    void startServer();
    void connectTo(QString);
    void closeConnection();
    void sendCommand(QByteArray);
    QVector<QString> &getList();
    bool isConnected();
    void connectMe(QObject *);
    void disconnectMine();
    ~server();
    
signals:
    void addListMember(QString);
    void removeListMember(QString);
    void changeStatus(QString, int);
    void changeButtonText();
    
public slots:
    void flushClientData(myobj *);
    void gotName(QString, myobj *);

protected:
    void incomingConnection(qintptr socketDescriptor);
    
};

#endif // SERVER_H
