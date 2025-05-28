#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>
#include <set>
#include <memory>
#include <functional>

class Session;

class Server
{
public:
    Server(boost::asio::io_context& context, unsigned short port);

    void broadcast(const std::string& msg);
    void setMessageHandler(std::function<void(const std::string&)> handler);

    void join(std::shared_ptr<Session> session);
    void leave(std::shared_ptr<Session> session);

    std::function<void(const std::string&)> mMessageHandler;

private:
    void doAccept();

    boost::asio::ip::tcp::acceptor mAcceptor;
    std::set<std::shared_ptr<Session>> mSessions;


};


#endif // SERVER_H
