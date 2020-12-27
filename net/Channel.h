#pragma once
#include <map>
#include <memory>
#include <functional>

class EventLoop;

class Channel
{
public:
//channel event callback
typedef std::function<void ()> EventCallback;
typedef std::function<void ()> ReadCallBack;

    Channel(EventLoop *loop, int sockfd);
    ~Channel();

    void setReadCallback(ReadCallBack cb) { read_callback_ = std::move(cb);}
    void setWriteCallback(EventCallback cb) { write_callback_ = std::move(cb);}
    void setCloseCallback(EventCallback cb) { close_callback_ = std::move(cb);}
    void setErrorCallback(EventCallback cb) { error_callback_ = std::move(cb);}

    void update();
    void remove();

    int  fd()        const { return fd_;}
    int  isNoEvent() const { return events_ == 0;};
    int  events()    const {return events_;}
    void setEvents(int event) { revents_ = event;}

    void enableConnecting();
    void enableReading();
    void disableReading();
    void enableWriting();
    void disableWriting();
    void disableAll() { events_ = 0; update(); }
    bool isWriting() const;
    bool isReading() const;
    void handleEvent();

    void tie(const std::shared_ptr<void>& obj);
private:
    int fd_;
    int revents_;
    int events_;
    std::weak_ptr<void> tie_;

    ReadCallBack read_callback_;
    EventCallback write_callback_;
    EventCallback close_callback_;
    EventCallback error_callback_;

    EventLoop* loop_;
    
};