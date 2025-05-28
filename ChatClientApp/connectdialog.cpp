#include "connectdialog.h"
#include "ui_connectdialog.h"

ConnectDialog::ConnectDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ConnectDialog)
{
    ui->setupUi(this);

    connect(ui->pushButtonConnect, &QPushButton::clicked, this, [=] () {
        QString host = ui->lineEditHost->text();
        quint16 port = ui->lineEditPort->text().toUShort();
        emit connectToServerRequested(host, port);
        this->accept();
    });
}

ConnectDialog::~ConnectDialog()
{
    delete ui;
}
