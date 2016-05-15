#ifndef CONNECTC_H
#define CONNECTC_H

#include <QWidget>
#include "QListWidgetItem"
#include "server.h"


namespace Ui {
class connectc;
}

class connectc : public QWidget
{
    Q_OBJECT
    server *mainServer;
    
public:
    explicit connectc(server *recieved, bool disconnected, QWidget *parent = 0);
    ~connectc();
    
private:
    QString connectedTo;
    Ui::connectc *ui;
    QList<QListWidgetItem *> clientList;

signals:
    void changeButtonText();

public slots:
    void newClientConnected(QString);
    void clientDisconnected(QString);
private slots:
    void on_pushButton_clicked();
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
};

#endif // CONNECTC_H
