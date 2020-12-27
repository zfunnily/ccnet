#include <string.h>
#include "EPollPoller.h"
#include "Channel.h"
#include <unistd.h>

EPollPoller::EPollPoller(EventLoop* loop)
: epfd_(::epoll_create1(EPOLL_CLOEXEC)),
loop_(loop),
events_(16)
{
}

EPollPoller::~EPollPoller()
{
    ::close(epfd_);
}


void EPollPoller::poll(ChannelList* active_channels)
{
    int epoll_events_counts = ::epoll_wait(epfd_, &*events_.begin(), static_cast<int>(events_.size()), -1);
    if (epoll_events_counts <= 0)
    {
        perror("epoll_events_count <=0");
        return;
    }
    for (int i=0; i < epoll_events_counts; i ++)
    {
        Channel* channel = static_cast<Channel*>(events_[i].data.ptr);
        
        channel->setEvents(events_[i].events);
        active_channels->push_back(channel);
    }
    if  (epoll_events_counts == static_cast<int>(events_.size()))
    {
        events_.resize(events_.size()*2);
    }
}


void  EPollPoller::update(int operation, Channel *channel)
{
    struct epoll_event ev;
    memset(&ev, 0, sizeof ev);
    ev.data.ptr = channel;
    ev.events = channel->events();
    int fd = channel->fd();
    if (::epoll_ctl(epfd_, operation, fd, &ev) < 0)
    {
        //todo logs
        if (operation == EPOLL_CTL_DEL)
        {
        }
        else
        {
        }
    }
    // fcntl(sock_fd_, F_SETFL, fcntl(sock_fd_, F_GETFD, 0)| O_NONBLOCK);
}

void EPollPoller::updateChannel(Channel *channel)
{
    int fd = channel->fd();
    if (channels_.find(fd) != channels_.end())
    {
        if (channel->isNoEvent())
        {
            // printf("remove channel,fd: %d\n", fd);
            update(EPOLL_CTL_DEL, channel);
            removeChannel(channel);
        }else
        {
            update(EPOLL_CTL_MOD, channel);
        }
    }else {
        channels_[fd] = channel;
        update(EPOLL_CTL_ADD, channel);
        // printf("add channel,fd: %d\n", fd);
    }
}

void EPollPoller::removeChannel(Channel *channel)
{
    int fd = channel->fd();
    size_t n = channels_.erase(fd);
    update(EPOLL_CTL_DEL, channel);
}
