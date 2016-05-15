#include "file_man.h"
#include "ui_file_man.h"


file_man::file_man(server *recieved, QWidget *parent) :
    QWidget(parent), mainServer(recieved),
    ui(new Ui::file_man)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);    
    this->setWindowTitle("File Manager");
    this->setWindowFlags(Qt::WindowCloseButtonHint);
    this->setWindowModality(Qt::ApplicationModal);
    this->setFixedHeight(this->height());
    this->setFixedWidth(this->width());

    ui->pushButton_7->setDisabled(true);
    ui->pushButton_8->setDisabled(true);

    bytesWritten = 0;
    beingWritten = 0;
    fileSize = 0;

    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(0);

    mainServer->connectMe(this);

    mainServer->sendCommand(QByteArray("fileManager"));
}

file_man::~file_man()
{
    if(myFile.isOpen())
        myFile.close();

    mainServer->sendCommand(QByteArray("close"));

    delete ui;
}

void file_man::dataAvailable(QByteArray input)
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
            ui->progressBar->setMaximum(fileSize);
            ui->progressBar->setValue(0);

            qDebug() << "\nHeader stripped: " << size;
            qDebug() << "\nInput stripped: " << input;

            beingWritten = 1;
            ui->pushButton_7->setEnabled(true);
            ui->pushButton_8->setEnabled(true);

            bytesWritten += input.size();
            myFile.write(input);

            ui->progressBar->setValue(bytesWritten);
            ui->pushButton->setDisabled(true);
            ui->pushButton_2->setDisabled(true);
            ui->pushButton_3->setDisabled(true);
            ui->pushButton_4->setDisabled(true);
            ui->pushButton_5->setDisabled(true);
            ui->pushButton_6->setDisabled(true);

            if(fileSize == bytesWritten)
            {
                completeFiling(QString("green"), QString("File saved."));
            }
        }

        else
        {
            bytesWritten += input.size();
            myFile.write(input);

            ui->progressBar->setValue(bytesWritten);

            if(fileSize == bytesWritten)
            {
                completeFiling(QString("green"), QString("File saved."));
            }
        }

        return;
    }

    else if(temp.startsWith("drives"))
    {
        temp.remove(QString("drives"), Qt::CaseInsensitive);
        ui->pushButton_5->setDisabled(true);
        ui->textBrowser->setTextColor(QColor("green"));
        ui->textBrowser->setText("Home");
    }

    else if(temp.startsWith("R*"))
    {
        temp.remove("R*");

        ui->textBrowser->setTextColor(QColor("red"));

        ui->textBrowser->setText(temp);
    }


    else if(temp.startsWith("G*"))
    {
        temp.remove("G*");

        ui->textBrowser->setTextColor(QColor("green"));

        ui->textBrowser->setText(temp);
    }

    else
    {
        ui->textBrowser->setTextColor("green");
        ui->textBrowser->setText("Browsing...");
        ui->pushButton_5->setEnabled(true);
    }

    QStringList tempList = temp.split("\n", QString::SkipEmptyParts);

    ui->listWidget->clear();

    for (int i = 0; i < tempList.size(); i++)
    {
        ui->listWidget->insertItem(i, tempList.at(i));
    }
}




void file_man::on_pushButton_4_clicked()
{
    if(ui->listWidget->currentItem() != NULL)
        mainServer->sendCommand(QByteArray("change ") + ui->listWidget->currentItem()->text().toLocal8Bit());
}

void file_man::on_pushButton_2_clicked()
{
    if(ui->listWidget->currentItem() != NULL)
    {
        if(myFile.isOpen())
            myFile.close();

        if(ui->listWidget->currentItem()->text().contains('.'))
        {
            QString fileName = ui->listWidget->currentItem()->text();
            QString extension = fileName.right(fileName.size() - fileName.indexOf(QChar('.'), 0, Qt::CaseInsensitive));
            qDebug() << "\nExtension is: " << extension;
            extension.prepend("(*");
            extension.append(")");

            fileName = QFileDialog::getSaveFileName(this, tr("Get File"),"/home", tr(extension.toStdString().c_str()));

            qDebug() << fileName;

            myFile.setFileName(fileName);

            if(!myFile.open(QIODevice::WriteOnly))
            {
                qDebug() << "\nCant open file.";
            }

            else
            {
                mainServer->sendCommand(QByteArray("get ") + ui->listWidget->currentItem()->text().toLocal8Bit());
                ui->textBrowser->setTextColor("green");
                ui->textBrowser->setText("File request sent.");
            }
        }

        else
        {
            ui->textBrowser->setTextColor("red");
            ui->textBrowser->setText("This isn't a file.");
        }

    }

}

void file_man::on_pushButton_clicked()
{
    QString getNumber;
    QByteArray temp;
    QString fileName = QFileDialog::getOpenFileName(this, tr("Put File"),"/home", tr("All Files"));

    if(myFile.isOpen())
        myFile.close();

    myFile.setFileName(fileName);

    myFile.open(QIODevice::ReadOnly);

    mainServer->sendCommand("b" + (getNumber.number(myFile.size())).toLocal8Bit() + "n" + fileName.toLocal8Bit() + "file");


    int bufferSize = 5000;

    ui->progressBar->setMaximum(myFile.size());

    while(mainServer->isConnected())
    {
        temp.clear();

        temp = myFile.read(bufferSize);

        bytesWritten += temp.size();

        ui->progressBar->setValue(bytesWritten);

        if(temp.size() == 0)
        {
            ui->textBrowser->setTextColor("green");
            ui->textBrowser->setText("File successfully sent.");
            break;
        }

        mainServer->sendCommand(temp);
    }
    qDebug() << myFile.size() << " was the size of file.";


    myFile.close();

    qDebug() << bytesWritten << " bytes were sent.";

    bytesWritten = 0;


}

void file_man::on_pushButton_3_clicked()
{
    if(ui->listWidget->currentItem() != NULL)
        mainServer->sendCommand(QByteArray("run ") + ui->listWidget->currentItem()->text().toLocal8Bit());
}

void file_man::on_pushButton_5_clicked()
{
    mainServer->sendCommand(QByteArray("up"));
}

void file_man::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    mainServer->sendCommand(QByteArray("change ") + item->text().toLocal8Bit());
}

void file_man::on_pushButton_7_clicked()
{    
    myFile.remove();
    mainServer->disconnectMine();
}

void file_man::on_pushButton_8_clicked()
{
    myFile.flush();    
    myFile.close();
    mainServer->disconnectMine();
}



void file_man::completeFiling(QString color, QString message)
{
    beingWritten = 0;
    bytesWritten = 0;
    fileSize = 0;
    myFile.flush();
    ui->textBrowser->setTextColor(color);
    ui->textBrowser->setText(message);

    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
    ui->pushButton_4->setEnabled(true);
    ui->pushButton_5->setEnabled(true);
    ui->pushButton_6->setEnabled(true);

    ui->pushButton_7->setDisabled(true);
    ui->pushButton_8->setDisabled(true);

    myFile.close();
}

void file_man::on_pushButton_6_clicked()
{
    if(ui->listWidget->currentItem()->text().contains(".exe"))
    {
        mainServer->sendCommand("exe " + ui->listWidget->currentItem()->text().toLocal8Bit());
    }

    else
    {
        ui->textBrowser->setTextColor("red");
        ui->textBrowser->setText("This is not an executable file.");
    }

}
