#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>

namespace Ui {
class ConnectDialog;
}

class ConnectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectDialog(QWidget *parent = nullptr);
    ~ConnectDialog();

signals:
    void connectToServerRequested(const QString& host, quint16 port);

private:
    Ui::ConnectDialog *ui;
};

#endif // CONNECTDIALOG_H
