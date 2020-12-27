#pragma once
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>

#include "TcpAddr.h"
#include "nocopyable.h"

class TcpAddr;
class Socket : noncopyable
{
public:
    Socket(const TcpAddr& addr);
    ~Socket();

    int   createSocket();
    int   bind();
    int   listen();
    int   accept(TcpAddr* per_addr);
    void  close() { ::close(fd_);}
    int   fd() const {return fd_;};

    void  shutdownWrite() { ::shutdown(fd_, SHUT_WR); }

    struct sockaddr_in getSockAddr(int sockfd);
private:
    TcpAddr addr_;
    int fd_;
};
