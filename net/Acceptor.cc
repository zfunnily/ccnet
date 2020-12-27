#include "Acceptor.h"

Acceptor::Acceptor(EventLoop *loop, const TcpAddr& addr)
:loop_(loop),
accept_socket_(new Socket(addr))
{
    accept_socket_->createSocket();
    accept_socket_->bind();
    accept_channel_ = std::make_shared<Channel>(loop, accept_socket_->fd());
    accept_channel_->setReadCallback(std::bind(&Acceptor::handleRead, this));
}

Acceptor::~Acceptor()
{
    accept_channel_->disableAll();
    accept_channel_->remove();
}

void Acceptor::listen()
{
    accept_socket_->listen();
    accept_channel_->enableConnecting();
}

void Acceptor::handleRead()
{
    TcpAddr peerAddr;
    int connfd = accept_socket_->accept(&peerAddr);
    if (connfd >= 0)
    {
        if (newconnection_callback_)
        {
            newconnection_callback_(connfd, peerAddr);
        }
        else
        {
            accept_socket_->close();
        }
    }
}