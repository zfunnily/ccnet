#pragma once
#include <functional>
#include <vector>
#include <memory>
#include "Callbacks.h"

class EPollPoller;
class Channel;

typedef std::vector<Channel*> ChannelList;

class EventLoop
{
public:
    typedef std::function<void()> Functor;
    EventLoop(/* args */);
    ~EventLoop();

    void  loop();
    void  runInLoop(Functor cb);
    void  updateChannel(Channel* channel);
    void  removeChannel(Channel* channel);

public:
private:
    bool quit_;
    bool looping_;
    int epfd_;
    ChannelList active_channels_;
    std::unique_ptr<EPollPoller> poller_;
};
