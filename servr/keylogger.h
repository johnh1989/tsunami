#ifndef KEYLOGGER_H
#define KEYLOGGER_H

#include <QWidget>
#include <QFile>
#include <QFileDialog>
#include "server.h"

namespace Ui {
class keylogger;
}

class keylogger : public QWidget
{
    Q_OBJECT
    server *mainServer;
    int beingWritten;
    int fileSize;
    int bytesWritten;
    QFile myFile;
    void completeFiling();
    QString fileName;
    
public:
    explicit keylogger(server *recieved, QWidget *parent = 0);
    ~keylogger();
    
private:
    Ui::keylogger *ui;

public slots:
    void dataAvailable(QByteArray);
private slots:
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
};

#endif // KEYLOGGER_H
