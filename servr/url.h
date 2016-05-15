#ifndef URL_H
#define URL_H

#include <QDialog>
#include "server.h"

namespace Ui {
class URL;
}

class URL : public QDialog
{
    Q_OBJECT

    server *mainServer;
    QString prepend;
    
public:
    explicit URL(server *recieved = NULL, int mode = 1, QWidget *parent = 0);
    ~URL();
    
private slots:
    void on_buttonBox_accepted();

private:
    Ui::URL *ui;
};

#endif // URL_H
