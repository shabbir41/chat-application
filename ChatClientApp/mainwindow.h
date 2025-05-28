#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "client.h"

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

public slots:
    void connectToServer(QString host, quint16 port);
    void onMessageReceived(const QString &message);

private slots:
    void onSendClicked();

private:
    Ui::MainWindow *ui;
    Client* mClient;
};
#endif // MAINWINDOW_H
