#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <chrono>
#include <iomanip>
#include <sstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->textEditLog->setReadOnly(true);

    connect(ui->pushButtonSend, &QPushButton::clicked, this, &MainWindow::onSendClicked);
    connect(ui->lineEditInput, &QLineEdit::returnPressed, this, &MainWindow::onSendClicked);
    connect(this, &MainWindow::serverReceivedMessage, ui->textEditLog, &QTextEdit::append);

    mServerThread = std::thread([this] () {
        try {
            boost::asio::io_context io_cont;
            mServer = std::make_unique<Server>(io_cont, 12345);

            mServer->setMessageHandler([this] (const std::string& msg) {
                QString qmsg = QString::fromStdString(msg).trimmed();
                QMetaObject::invokeMethod(this, "serverReceivedMessage", Qt::QueuedConnection, Q_ARG(QString, qmsg));
            });
            io_cont.run();
        } catch (const std::exception& e) {
            qWarning("Server Error: %s", e.what());
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onSendClicked()
{
    QString message = ui->lineEditInput->text();

    auto now = std::chrono::system_clock::now();
    auto now_time = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << "[" << std::put_time(std::localtime(&now_time), "%H:%M:%S") << "] ";
    std::string formatted = ss.str() + "[Server]: " + message.toStdString();

    if(!message.isEmpty())
    {
        QString fullmsg = QString::fromStdString(formatted);
        ui->textEditLog->append(fullmsg);
        ui->lineEditInput->clear();
        if(mServer){
            mServer->broadcast(fullmsg.toStdString());
        }
    }
}

