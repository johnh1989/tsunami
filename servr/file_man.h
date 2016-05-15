#ifndef FILE_MAN_H
#define FILE_MAN_H

#include <QCloseEvent>
#include <QWidget>
#include <QFile>
#include <QFileDialog>
#include <QListWidgetItem>
#include "server.h"


namespace Ui {
class file_man;
}

class file_man : public QWidget
{
    Q_OBJECT
    server *mainServer;
    QFile myFile;
    void completeFiling(QString, QString);

    
public:
    explicit file_man(server *recieved, QWidget *parent = 0);
    ~file_man();

public slots:
    void dataAvailable(QByteArray);
    
private slots:
    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_5_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::file_man *ui;
    int fileSize;
    int bytesWritten;
    int beingWritten;

};

#endif // FILE_MAN_H
