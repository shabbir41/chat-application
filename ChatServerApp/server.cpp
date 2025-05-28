#include "server.h"
#include "session.h"

Server::Server(boost::asio::io_context &context, unsigned short port)
    :mAcceptor(context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
{
    doAccept();
}

void Server::broadcast(const std::string &msg)
{
    for (auto& session: mSessions){
        session->send(msg);
    }
}

void Server::setMessageHandler(std::function<void (const std::string &)> handler)
{
    mMessageHandler = std::move(handler);
}

void Server::join(std::shared_ptr<Session> session)
{
    mSessions.insert(session);
}

void Server::leave(std::shared_ptr<Session> session)
{
    mSessions.erase(session);
}


void Server::doAccept()
{
    mAcceptor.async_accept([this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket){
        if(!ec){
            auto session = std::make_shared<Session>(std::move(socket), *this);
            join(session);
            session->start();
        }
        doAccept();
    });
}



