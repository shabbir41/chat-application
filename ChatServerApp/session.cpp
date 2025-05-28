#include "session.h"
#include "server.h"

#include <chrono>
#include <iomanip>
#include <sstream>

Session::Session(boost::asio::ip::tcp::socket socket, Server &server)
    : mSocket(std::move(socket)), mServer(server)
{

}

void Session::start()
{
    doRead();
}

void Session::send(const std::string &msg)
{
    doWrite(msg);
}

void Session::doRead()
{
    auto self(shared_from_this());
    mSocket.async_read_some(boost::asio::buffer(mData, 1024),
                            [this, self] (boost::system::error_code ec, std::size_t length){
                                if(!ec){
                                    auto now = std::chrono::system_clock::now();
                                    auto now_time = std::chrono::system_clock::to_time_t(now);
                                    std::stringstream ss;
                                    ss << "[" << std::put_time(std::localtime(&now_time), "%H:%M:%S") << "] ";

                                    std::string msg(mData, length);

                                    std::string formatted = ss.str() + "[Client]: " + msg;

                                    if (!msg.empty())
                                    {
                                        mServer.broadcast(formatted);
                                        if(mServer.mMessageHandler)
                                            mServer.mMessageHandler(formatted);
                                    }
                                    doRead();
                                } else {
                                    mServer.leave(shared_from_this());
                                }
                            });
}

void Session::doWrite(const std::string &msg)
{
    auto self(shared_from_this());
    boost::asio::async_write(mSocket, boost::asio::buffer(msg),
                             [this, self] (boost::system::error_code ec, std::size_t length){
                                if (ec){
                                    mServer.leave(shared_from_this());
                                }
                            });
}


