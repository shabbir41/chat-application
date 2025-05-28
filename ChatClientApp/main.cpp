#include "mainwindow.h"
#include "connectdialog.h"

#include <QApplication>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ConnectDialog connectDialog;
    MainWindow mainWindow;

    QObject::connect(&connectDialog, &ConnectDialog::connectToServerRequested, &mainWindow, &MainWindow::connectToServer);

    connectDialog.exec();

    mainWindow.show();

    return a.exec();
}
