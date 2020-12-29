#pragma once
#include <thread>
#include <mutex>
#include <memory>
#include <condition_variable>
#include <string>

class EventLoop;
class EventLoopThread
{
    typedef std::function<void(EventLoop*)> ThreadInitCallback;
    typedef std::shared_ptr<std::thread> ThreadPtr;
public:
    EventLoopThread(const ThreadInitCallback& cb = ThreadInitCallback(), const std::string& name = "");
    ~EventLoopThread();

    EventLoop* startLoop();

private:
    void threadFunc();
private:
    EventLoop* loop_;
    bool ready;

    ThreadPtr thread_;
    std::mutex mutex_;
    std::condition_variable cond_;
    ThreadInitCallback callback_;

};