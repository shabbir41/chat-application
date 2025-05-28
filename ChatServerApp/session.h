#ifndef SESSION_H
#define SESSION_H

#include <boost/asio.hpp>
#include <memory>
#include <string>

class Server;

class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(boost::asio::ip::tcp::socket socket, Server& server);
    void start();
    void send(const std::string& msg);

private:
    void doRead();
    void doWrite(const std::string& msg);

    boost::asio::ip::tcp::socket mSocket;
    Server& mServer;
    char mData[1024];
};

#endif // SESSION_H
