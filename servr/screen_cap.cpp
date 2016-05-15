#include "screen_cap.h"
#include "ui_screen_cap.h"

screen_cap::screen_cap(server *recieved, QWidget *parent) :
    QWidget(parent), mainServer(recieved),
    ui(new Ui::screen_cap)
{    
    this->setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    this->setWindowTitle("Screen Capture");
    this->setWindowFlags(Qt::WindowCloseButtonHint);
    this->setWindowModality(Qt::ApplicationModal);
    this->setFixedHeight(this->height());
    this->setFixedWidth(this->width());


    ui->pushButton_2->setDisabled(true);
    ui->pushButton_3->setDisabled(true);

    mainServer->connectMe(this);
    mainServer->sendCommand(QByteArray("screenCap"));

    beingWritten = 0;
    fileSize = 0;
    bytesWritten = 0;

    myBuffer.setBuffer(&myArray);
    myBuffer.open(QIODevice::WriteOnly);

    ui->label->setAlignment(Qt::AlignCenter);

    ownShot();
}

screen_cap::~screen_cap()
{
    delete ui;
}

void screen_cap::on_pushButton_clicked()
{    
    mainServer->sendCommand(QByteArray("take"));
}


void screen_cap::dataAvailable(QByteArray input)
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

            qDebug() << "\nHeader stripped: " << size;
            qDebug() << "\nInput stripped: " << input;

            beingWritten = 1;

            bytesWritten += input.size();
            myBuffer.write(input);
            ui->progressBar->setValue(bytesWritten);

            if(fileSize == bytesWritten)
            {
                gotImage();
            }
        }

        else
        {
            bytesWritten += input.size();
            myBuffer.write(input);
            ui->progressBar->setValue(bytesWritten);

            if(fileSize == bytesWritten)
            {
                gotImage();
            }
        }

        return;
    }
}



void screen_cap::gotImage()
{
    beingWritten = 0;
    fileSize = 0;
    bytesWritten = 0;
    myBuffer.close();
    myBuffer.open(QIODevice::ReadOnly);

    myPixmap.loadFromData(myBuffer.buffer());

    ui->label->setPixmap(myPixmap.scaled(ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(true);


    myBuffer.close();
    myBuffer.open(QIODevice::WriteOnly);
}



void screen_cap::ownShot()
{
    QScreen *screen = QGuiApplication::primaryScreen();

    myPixmap = screen->grabWindow(0);

    ui->label->setPixmap(myPixmap.scaled(ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void screen_cap::on_pushButton_2_clicked()
{
    ui->pushButton_2->setDisabled(true);
    ui->pushButton_3->setDisabled(true);
    ui->progressBar->setValue(0);

    ownShot();
}

void screen_cap::on_pushButton_3_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Screenshot"),"/home", tr("Image Files (*.JPG)"));

    if(myPixmap.save(fileName, "JPG"))
    {
        ui->pushButton_2->setDisabled(true);
        ui->pushButton_3->setDisabled(true);
        ui->progressBar->setValue(0);
    }
}
