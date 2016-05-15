#include "server.h"


//***********************************************************************************************
//***********************************************************************************************
//The part which creates and starts the server

server::server(QObject *parent) :
    QTcpServer(parent)
{
    connectedTo = NULL;
}


void server::startServer()
{
    if(!this->listen(QHostAddress::Any, 5589))   //tells the server to start listening
    {
        qDebug() << "\nUnable to start server";
    }

    else
    {
        qDebug() << "\nServer started.";
    }
}




//***********************************************************************************************
//***********************************************************************************************
//The server core - the part which manages tasks related to connections

void server::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << "\nIncoming connection recieved.";

    clientPointers.append(new myobj(socketDescriptor, this));
    clientNames.append("Default");

    connect(clientPointers.last(), SIGNAL(gotName(QString, myobj *)), this, SLOT(gotName(QString, myobj *)));
    connect(clientPointers.last(), SIGNAL(finished(myobj *)), this, SLOT(flushClientData(myobj*)));

    qDebug() << "\nClient object created and pointer is " << clientPointers.last() << ".";
}



void server::gotName(QString name, myobj* correspondingPointer)
{
    emit changeStatus(name + " connected!", 1);

    for(int i = 0; i < clientNames.size(); i++)
    {
        if(name == clientNames[i])
        {
            correspondingPointer->disconnectClient();
            return;
        }
    }

    myobj** data = clientPointers.data();

    for(int i = 0; i < clientPointers.size(); i++)
        if (data[i] == correspondingPointer)
            clientNames.replace(i, name);

    emit addListMember(name);
}


void server::flushClientData(myobj *toBeDeleted)
{
    myobj** data1 = clientPointers.data();
    QString *data2 = clientNames.data();

    for(int i = 0; i < clientPointers.size(); i++)
    {
        if (data1[i] == toBeDeleted)
        {
            qDebug() << "\ni is " << i;

            qDebug() << "\nRemoved: " << clientPointers[i];
            clientPointers.remove(i);

            qDebug() << "\nRemoved: " << clientNames[i];
            emit changeStatus(data2[i] + " disconnected!", 0);
            emit removeListMember(data2[i]);
            clientNames.remove(i);
        }
    }

    if (connectedTo == toBeDeleted)
    {
        closeConnection();
        emit changeButtonText();
    }
}

//***********************************************************************************************
//***********************************************************************************************
//Server commands - accesible from outside the server class

void server::connectMe(QObject *ptr)
{
    if(connectedTo != NULL)
    {
        disconnect(connectedTo, SIGNAL(gotData(QByteArray)), 0, 0);
        disconnect(connectedTo, SIGNAL(clientIsGone()), 0, 0);
        connect(connectedTo,SIGNAL(clientIsGone()), ptr, SLOT(close()));
        connect(connectedTo, SIGNAL(gotData(QByteArray)), ptr, SLOT(dataAvailable(QByteArray)));
    }

    else
        qDebug() << "\nError: Not connected to any client.";
}

QVector<QString> &server::getList()
{
    return clientNames;
}

bool server::isConnected()
{
    if(connectedTo)
        return true;
    else
        return false;
}

void server::connectTo(QString client)
{
    myobj** data2 = clientPointers.data();

    for(int i = 0; i < clientNames.size(); i++)
        if(client == clientNames[i])
        {
            connectedTo = data2[i];
            qDebug() << "\nConnected to: " << clientNames[i];
        }

    if (connectedTo == NULL)
        qDebug() << "\nError establishing connection.";
}

void server::closeConnection()
{
    disconnect(connectedTo, SIGNAL(gotData(QByteArray)), 0, 0);
    disconnect(connectedTo, SIGNAL(clientIsGone()), 0, 0);
    connectedTo = NULL;
    qDebug() << "\nConnection closed.";
}

void server::disconnectMine()
{
    connectedTo->disconnectClient();
}

void server::sendCommand(QByteArray data)
{
    connectedTo->send(data);
}



//***********************************************************************************************
//***********************************************************************************************
//Server destructor

server::~server()
{
    clientPointers.clear();
    clientNames.clear();
}
