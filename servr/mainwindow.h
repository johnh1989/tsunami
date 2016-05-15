#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "file_man.h"
#include "keylogger.h"
#include "screen_cap.h"
#include "chat.h"
#include "connectc.h"
#include "server.h"
#include "url.h"

#include <QMainWindow>
#include <QString>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

public slots:
    void onStatusChanged(QString, int colour);
    void changeButtonText();

private:
    Ui::MainWindow *ui;
    screen_cap *scopen;
    file_man *fmopen;
    keylogger *klopen;
    chat *copen;
    connectc *cnopen;
    server *mainServer;
    URL *urlopen;
};

#endif // MAINWINDOW_H
