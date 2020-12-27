#include "Socket.h"

Socket::Socket(const TcpAddr& addr):addr_(addr)
{
    
}

Socket::~Socket()
{
    ::close(fd_);
}

int Socket::createSocket()
{
    fd_ = ::socket(addr_.family(), SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    if (fd_ == -1)
    {
        perror("sock error\n"); 
        return -1;
    }
    printf("create fd: %d\n", fd_);
    return 0;
}

int Socket::bind()
{
    const struct sockaddr* ad = addr_.getSockAddr();
    if ( ::bind(fd_, ad, sizeof(struct sockaddr)) == -1)
    {
        perror("bind error");
        return -1;
    }
    return 0;
}

int Socket::listen()
{
    if (::listen(fd_, 5) == -1)
    {
        perror("listen error");
        return -1;
    }
    return 0;
}

int Socket::accept(TcpAddr* per_addr)
{
    struct sockaddr_in addr;
    memset(&addr,0, sizeof addr);

    socklen_t len = 0;
    int connfd = ::accept(fd_,(struct sockaddr*)&addr,&len);
    if  (connfd >= 0)
    {
        per_addr->setSockAddr(addr);
    }
    return connfd;
}

struct sockaddr_in Socket::getSockAddr(int sockfd)
{
    struct sockaddr_in localaddr;
    memset(&localaddr,0 ,sizeof localaddr);
    socklen_t addrlen = static_cast<socklen_t>(sizeof localaddr);
    if (::getsockname(sockfd, (struct sockaddr*)(&localaddr), &addrlen) < 0)
    {
        perror("getsockname error\n");
    }
    return localaddr;
}