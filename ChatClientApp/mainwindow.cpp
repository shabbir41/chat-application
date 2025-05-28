#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mClient(nullptr)
{
    ui->setupUi(this);

    ui->textEditChat->setReadOnly(true);

    connect(ui->pushButtonSend, &QPushButton::clicked, this, &MainWindow::onSendClicked);
    connect(ui->lineEditInput, &QLineEdit::returnPressed, this, &MainWindow::onSendClicked);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectToServer(QString host, quint16 port)
{
    mClient = new Client(this);

    connect(mClient, &Client::messageReceived, this, &MainWindow::onMessageReceived);
    connect(mClient, &Client::connected, this, [this] () {
        ui->textEditChat->append("[Client] Connected to [Server]...");
    });
    connect(mClient, &Client::disconnected, this, [this] () {
        ui->textEditChat->append("[Client] Disconnected from [Server]...");
    });

    mClient->connectToServer(host.toStdString(), port);


}

void MainWindow::onMessageReceived(const QString &message)
{
    ui->textEditChat->append(message);
}




void MainWindow::onSendClicked()
{
    QString message = ui->lineEditInput->text();
    if(!message.isEmpty() && mClient){
        mClient->send(message.toStdString());
        // ui->textEditChat->append("[You]: " + message);
        ui->lineEditInput->clear();
    }
}

