
#pragma once
#include <memory>
#include <vector>
#include <functional>

class EventLoop;
class EventLoopThread;

class EventLoopThreadPool
{
    typedef std::unique_ptr<EventLoopThread> EventLoopThreadPtr;
    typedef std::function<void(EventLoop*)> ThreadInitCallback;
public:
    EventLoopThreadPool(EventLoop* loop, const std::string& name);
    ~EventLoopThreadPool();

    void setThreadNum(int num_threads) { num_threads_ = num_threads;};
    void start(const ThreadInitCallback& cb = ThreadInitCallback());

    EventLoop* getNextLoop();
    EventLoop* getLoopFroHash(size_t hash_code);
    std::vector<EventLoop*> getAllLoops();
    bool started() const   { return started_; }
    const std::string& name() const { return name_; }
private:
    EventLoop* base_loop_;
    std::string name_;
    bool started_;
    int num_threads_;
    int next_;

    std::vector<EventLoopThreadPtr> threads_;
    std::vector<EventLoop*> loops_;
};