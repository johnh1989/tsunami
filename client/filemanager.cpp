#include "filemanager.h"

fileManager::fileManager(QObject *parent, tcpsocket *mySock) :
    QObject(parent), communicationSocket(mySock)
{
    addDrives();
    sendTextReply();
    beingWritten = 0;
    bytesWritten = 0;
    fileSize = 0;
}

void fileManager::recievedCommand(QByteArray input)
{
    QString command(input);

    if(beingWritten == 1 || command.startsWith("b"))
    {
        if (beingWritten == 0)
        {
            input.remove(0, command.indexOf("file") + 4);

            qDebug() << "\nHeader recieved: " << command;
            command.remove(0, 1); //remove b
            QString size = command.left(command.indexOf("n"));
            command.remove(size);
            command.remove(0, 1); //remove n
            QString fileName = command.left(command.indexOf("file"));
            command.remove(fileName);
            command.remove(QString("file"));
            fileSize = size.toInt();
            while(fileName.indexOf('/', 0) != -1)
            {
                fileName.remove(0, fileName.indexOf('/', 0) + 1);
            }

            qDebug() << "\nHeader stripped: " << size;
            qDebug() << "\nInput stripped: " << input;
            qDebug() << "\nName of file: " << fileName;

            if(myFile.isOpen())
                myFile.close();

            myFile.setFileName(myDir.absolutePath() + "/" + fileName);

            myFile.open(QIODevice::WriteOnly);

            beingWritten = 1;

            bytesWritten += input.size();
            myFile.write(input);

            if(fileSize == bytesWritten)
            {
                beingWritten = 0;
                bytesWritten = 0;
                fileSize = 0;
                myFile.flush();
                myFile.close();
            }
        }

        else
        {
            bytesWritten += input.size();
            myFile.write(input);

            if(fileSize == bytesWritten)
            {
                beingWritten = 0;
                bytesWritten = 0;
                fileSize = 0;
                myFile.flush();
                myFile.close();
            }
        }


        qDebug() << "Bytes written set to: " << bytesWritten; //0 for whole file written

        return;
    }

    else if(command == "up")
    {
        outputBuffer.clear();

        if(myDir.absolutePath().size() == 3)
        {
            addDrives();

            sendTextReply();
        }

        else
        {
            myDir.cdUp();

            addEnteries();

            sendTextReply();
        }
    }


    else if(command.startsWith(QString("change "), Qt::CaseInsensitive))
    {
        command.remove(QString("change "), Qt::CaseInsensitive);

        if(!myDir.exists(command))
        {
            outputBuffer = "R*Directory doesn't exist.";
            outputBuffer.append("\n\n");
            addDrives();
            sendTextReply();
        }

        else
        {
            myDir.cd(command);

            outputBuffer.clear();

            addEnteries();

            sendTextReply();
        }
    }


    else if(command.startsWith(QString("run "), Qt::CaseInsensitive))
    {
        command.remove(QString("run "), Qt::CaseInsensitive);

        QFileInfo temp(myDir.absolutePath() + "/" + command);

        outputBuffer.clear();

        if(!temp.isFile())
        {

            outputBuffer.append("R*This isn't a file.");

            outputBuffer.append("\n\n");

            addEnteries();

            sendTextReply();
        }

       else
        {
            QDesktopServices::openUrl(QUrl::fromLocalFile(temp.filePath()));

            outputBuffer.append("G*File opened on client PC.");

            outputBuffer.append("\n\n");

            addEnteries();

            sendTextReply();
        }
    }


    else if(command.startsWith(QString("exe "), Qt::CaseInsensitive))
    {
        command.remove(QString("exe "), Qt::CaseInsensitive);

        QString temp(myDir.absolutePath() + "/" + command);

        outputBuffer.clear();

        QProcess::startDetached(temp);

        outputBuffer.append("G*Exe launched on client PC.");

        outputBuffer.append("\n\n");

        addEnteries();

        sendTextReply();
    }


    else if(command.startsWith(QString("get "), Qt::CaseInsensitive))
    {
        QByteArray outputBuffer;

        command.remove(QString("get "), Qt::CaseInsensitive);

        QString fileName = myDir.absolutePath() + "/" + command;

        QFileInfo temp(fileName);

        QFile myFile(fileName);

        if (!temp.isFile())
        {
             outputBuffer.append("R*This isn't a file.");
             outputBuffer.append("\n\n");
             sendTextReply();
             return;
        }

        else if(!myFile.open(QIODevice::ReadOnly))
        {
            outputBuffer.append("R*Cannot get this file.");
            outputBuffer.append("\n\n");
            sendTextReply();
            return;
        }

        else
        {
            QString temp;

            outputBuffer.clear();

            outputBuffer = "b" + (temp.number(myFile.size())).toLocal8Bit() + "file";

            communicationSocket->sendReply(outputBuffer);

            int bufferSize = 5000;

            while(communicationSocket->isConnected())
            {                
                outputBuffer.clear();                

                outputBuffer = myFile.read(bufferSize);

                if(outputBuffer.size() == 0)
                    break;

                communicationSocket->sendReply(outputBuffer);
            }
        }
    }


}


void fileManager::sendTextReply()
{
    communicationSocket->sendReply(outputBuffer.toLocal8Bit());
}


void fileManager::addDrives()
{
    if(!outputBuffer.startsWith(QChar('*'), Qt::CaseInsensitive))
        outputBuffer.append("drives");

    foreach(QFileInfo temp, myDir.drives())
    {
        outputBuffer.append(temp.absoluteFilePath() + "\n");

    }
}


void fileManager::addEnteries()
{
    foreach(QFileInfo temp, myDir.entryInfoList())
    {
        if(!(temp.fileName() == ".") && !(temp.fileName() == ".."))
            outputBuffer.append(temp.fileName() + "\n");
    }
}
