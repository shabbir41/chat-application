#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <thread>
#include <memory>

#include "server.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


signals:
    void serverReceivedMessage(QString msg);

public slots:
    void onSendClicked();


private:
    Ui::MainWindow *ui;

    std::unique_ptr<Server> mServer;
    std::thread mServerThread;
};
#endif // MAINWINDOW_H
