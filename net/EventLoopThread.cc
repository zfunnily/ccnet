
#include "EventLoopThread.h"
#include "EventLoop.h"

EventLoopThread::EventLoopThread(const ThreadInitCallback& cb, const std::string& name)
: loop_(nullptr),
mutex_(),
cond_(),
ready(false),
thread_(new std::thread(std::bind(&EventLoopThread::threadFunc,this), name)),
callback_(std::move(cb))
{

}

EventLoopThread::~EventLoopThread()
{
    thread_->join();
}

EventLoop* EventLoopThread::startLoop()
{
    EventLoop *loop = nullptr;
    std::unique_lock<std::mutex> lock(mutex_);
    while (loop_ == nullptr)
    {
        cond_.wait(lock, [&] { return ready;});
    }
    loop = loop_;
    return loop;
    
}

void EventLoopThread::threadFunc()
{
    EventLoop loop;
    if (callback_)
    {
        callback_(&loop);
    }
    {
        std::lock_guard<std::mutex> lock(mutex_);
        loop_ = &loop;
        ready = true;
        cond_.notify_one();
    }

    loop_->loop();

    std::lock_guard<std::mutex> lock(mutex_);
    loop_ = nullptr;
}
