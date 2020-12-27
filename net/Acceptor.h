#pragma once
#include <memory>
#include "Channel.h"
#include "Socket.h"

class EventLoop;
class TcpAddr;

class Acceptor
{
public:
    typedef std::function<void (int sockfd, const TcpAddr&)> NewConnectionCallback;
    Acceptor(EventLoop *loop, const TcpAddr& addr);
    ~Acceptor();

    void listen();
    int fd() const { return accept_socket_->fd();}

    void setNewConnectionCallback(const NewConnectionCallback& cb)
    { newconnection_callback_ = std::move(cb); }

    void handleRead();
private:
    EventLoop *loop_;
    NewConnectionCallback newconnection_callback_;
    std::shared_ptr<Channel> accept_channel_;
    std::unique_ptr<Socket> accept_socket_;
};
    