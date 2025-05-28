#include "client.h"

Client::Client(QObject *parent)
    : QObject{parent}, mSocket(nullptr)
{}

Client::~Client()
{
    mIOContext.stop();
    if(ioThread.joinable()){
        ioThread.join();
    }
}

void Client::connectToServer(const std::string &host, int port)
{
    mSocket = std::make_unique<boost::asio::ip::tcp::socket>(mIOContext);

    boost::asio::ip::tcp::resolver resolver(mIOContext);
    auto endpoints = resolver.resolve(host, std::to_string(port));

    boost::asio::async_connect(*mSocket, endpoints,
                               [this] (boost::system::error_code ec, boost::asio::ip::tcp::endpoint) {
        if(!ec) {
            QMetaObject::invokeMethod(this, "connected", Qt::QueuedConnection);
            doRead();
        } else {
            QMetaObject::invokeMethod(this, "disconnected", Qt::QueuedConnection);
        }
    });

    ioThread = std::thread([this] () {
        try {
            mIOContext.run();
        }
        catch(...){
            QMetaObject::invokeMethod(this, "disconnected", Qt::QueuedConnection);
        }
    });
}

void Client::send(const std::string &msg)
{
    if(mSocket && mSocket->is_open()){
        boost::asio::async_write(*mSocket, boost::asio::buffer(msg),
                                 [this] (boost::system::error_code ec, std::size_t length) {});
    }
}

void Client::doRead()
{
    mSocket->async_read_some(boost::asio::buffer(mData, 1024),
                             [this] (boost::system::error_code ec, std::size_t length) {
                                 if(!ec) {
                                     QString msg = QString::fromUtf8(mData, static_cast<int>(length));
                                     emit messageReceived(msg);
                                     doRead();
                                 } else {
                                     emit disconnected();
                                 }
                             } );
}
