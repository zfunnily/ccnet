
#include "EventLoopThreadPool.h"
#include "EventLoopThread.h"


EventLoopThreadPool::EventLoopThreadPool(EventLoop* loop, const std::string& name)
: base_loop_(loop),
name_(name),
num_threads_(0),
next_(0),
started_(false)
{

}

EventLoopThreadPool::~EventLoopThreadPool()
{
}

void EventLoopThreadPool::start(const ThreadInitCallback& cb)
{
    for (int i = 0; i < num_threads_; i++)
    {
        char buf[name_.size()+32] = {0};
        sprintf(buf, "%s_%d", name_, i);
        EventLoopThread *thread(new EventLoopThread(cb, buf));
        threads_.push_back(std::unique_ptr<EventLoopThread>(thread));
        loops_.push_back(thread->startLoop());
    }
    if (num_threads_ == 0 && cb)
    {
        cb(base_loop_);
    }
    started_ = true;
}

EventLoop* EventLoopThreadPool::getNextLoop()
{
    EventLoop *loop = base_loop_;
    if (!loops_.empty())
    {
        loop = loops_[next_++];
        if (next_  >= loops_.size()) next_ = 0;
    }
    return loop;
}

EventLoop* EventLoopThreadPool::getLoopFroHash(size_t hash_code)
{
    EventLoop *loop = base_loop_;
    if (loops_.empty())
    {
         loop = loops_[hash_code % loops_.size()];
    }
    return loop;
}

std::vector<EventLoop*> EventLoopThreadPool::getAllLoops()
{
    if (loops_.empty())
    {
        return std::vector<EventLoop*>(1, base_loop_);
    }
    else
    {
        return loops_;
    }
}