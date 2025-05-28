#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <boost/asio.hpp>
#include <thread>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    ~Client();

    void connectToServer(const std::string &host, int port);
    void send(const std::string &msg);


signals:
    void messageReceived(const QString& message);
    void connected();
    void disconnected();

private:
    void doRead();

    boost::asio::io_context mIOContext;
    std::unique_ptr<boost::asio::ip::tcp::socket> mSocket;
    std::thread ioThread;
    char mData[1024];
};

#endif // CLIENT_H
