#include "url.h"
#include "ui_url.h"

URL::URL(server *recieved, int mode, QWidget *parent) :
    QDialog(parent), mainServer(recieved),
    ui(new Ui::URL)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowCloseButtonHint);
    this->setWindowModality(Qt::ApplicationModal);
    this->setFixedHeight(this->height());
    this->setFixedWidth(this->width());

    if(mode == 1)
    {
        this->setWindowTitle("Enter URL");
        ui->label->setText("URL Name: ");
        prepend = "url ";
    }


    else if(mode == 2)
    {
        this->setWindowTitle("Enter Process Name");
        ui->label->setText("Process Name: ");
        prepend = "kill ";
    }
}

URL::~URL()
{
    delete ui;
}

void URL::on_buttonBox_accepted()
{
    mainServer->sendCommand(prepend.toLocal8Bit() + ui->lineEdit->text().toLocal8Bit());
    this->close();
}
