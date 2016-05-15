#ifndef CHAT_H
#define CHAT_H

#include <QWidget>
#include "server.h"

namespace Ui {
class chat;
}

class chat : public QWidget
{
    Q_OBJECT
    server *mainServer;
    
public:
    explicit chat(server *recieved, QWidget *parent = 0);
    ~chat();
    
private:
    Ui::chat *ui;


public slots:
    void dataAvailable(QByteArray);
private slots:
    void on_pushButton_clicked();
    void on_lineEdit_returnPressed();
};

#endif // CHAT_H
