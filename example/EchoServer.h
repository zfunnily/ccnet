#include "EventLoop.h"
#include "TcpAddr.h"
#include "TcpServer.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

class EchoServer
{
public:
    EchoServer(EventLoop *loop, const TcpAddr& addr);
    ~EchoServer();

    void onConnect(const TcpConnectionPtr& conn);
    void onMessage(const TcpConnectionPtr& conn,Buffer* buf);

    void start() { server_.start(); }
private:
    TcpServer server_;
};

