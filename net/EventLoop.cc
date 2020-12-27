#include <csignal>
#include "EventLoop.h"
#include "EPollPoller.h"
#include "Channel.h"

EventLoop::EventLoop(/* args */)
:poller_(new EPollPoller(this))
{
}

EventLoop::~EventLoop()
{

}

void EventLoop::loop()
{
    quit_ = false;
    looping_ = true;

    while(!quit_)
    {
        active_channels_.clear();
        poller_->poll(&active_channels_);
        for (auto &channel: active_channels_)
        {
            channel->handleEvent();
        }
    }
}

void  EventLoop::runInLoop(Functor cb)
{
    cb();
}

void EventLoop::updateChannel(Channel* channel)
{
    poller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel* channel)
{
    poller_->removeChannel(channel);
}