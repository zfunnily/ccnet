#include <iostream>
#include "EchoServer.h"

int main()
{
    EventLoop loop;
    TcpAddr addr(8883);
    EchoServer server(&loop,addr);
    server.start();

    loop.loop();
}