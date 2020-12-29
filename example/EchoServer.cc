#include "EchoServer.h"

EchoServer::EchoServer(EventLoop *loop, const TcpAddr& addr)
:server_(loop,addr, "EchoServer")
{
    server_.setConnectionCallback(std::bind(&EchoServer::onConnect, this, std::placeholders::_1));
    server_.setMessageCallback(std::bind(&EchoServer::onMessage,this, std::placeholders::_1, std::placeholders::_2));
    server_.setNumThreads(4);
}

EchoServer::~EchoServer()
{
}
void EchoServer::onConnect(const TcpConnectionPtr& conn)
{
    printf("a new Connect\n");
}

void EchoServer::onMessage(const TcpConnectionPtr& conn,Buffer* buf)
{
    printf("recv from client msg: %s, %d\n",  buf->peek(), buf->readableBytes());
    conn->send(buf);
    buf->retrieveAll();
}
