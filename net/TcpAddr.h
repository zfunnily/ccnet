#pragma once
#include <string>
#include <string.h>
#include <netinet/in.h>
#include "copyable.h"

class TcpAddr : public copyable
{
public:
    TcpAddr(unsigned short port);
    TcpAddr() {}
    TcpAddr(struct sockaddr_in addr):addr_(addr){}
    ~TcpAddr();
    
    sa_family_t         family() const {return addr_.sin_family;};
    void                setSockAddr(const struct sockaddr_in& addr) {addr_ = addr;}
    void                getSockAddr(int fd);
    struct sockaddr*    getSockAddr() const { return (struct sockaddr*)(&addr_);}
private:
    unsigned short port_;
    struct sockaddr_in addr_;
};
