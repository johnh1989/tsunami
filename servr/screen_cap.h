#ifndef SCREEN_CAP_H
#define SCREEN_CAP_H

#include <QWidget>
#include <QBuffer>
#include <QPixmap>
#include <QScreen>
#include <QFile>
#include <QFileDialog>
#include <QGuiApplication>
#include "server.h"

namespace Ui {
class screen_cap;
}

class screen_cap : public QWidget
{
    Q_OBJECT
    server *mainServer;
    int fileSize;
    int bytesWritten;
    int beingWritten;
    QBuffer myBuffer;
    QByteArray myArray;
    QPixmap myPixmap;
    
public:
    explicit screen_cap(server *recieved, QWidget *parent = 0);
    ~screen_cap();
    
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::screen_cap *ui;    
    void gotImage();
    void ownShot();

public slots:
    void dataAvailable(QByteArray);
};

#endif // SCREEN_CAP_H
