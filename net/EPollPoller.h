#pragma once
#include <netinet/in.h>
#include <error.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <vector>
#include <memory>
#include <map>
#include "EventLoop.h"

typedef std::map<int,Channel*> ChannleMap;
class EPollPoller
{
public:
    EPollPoller(EventLoop *loop);
    ~EPollPoller();

    void  poll(ChannelList* active_channels);

    void  update(int operation, Channel *channel);
    void  updateChannel(Channel* channel);
    void  removeChannel(Channel* channel);
private:
    EventLoop *loop_;
    int sock_fd_;
    int epfd_;
    std::vector<struct epoll_event> events_;
    ChannleMap channels_;
};
