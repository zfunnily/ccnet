#include "Channel.h"
#include "EPollPoller.h"

Channel::Channel(EventLoop *loop, int sockfd)
: loop_(loop),
events_(0),
revents_(0),
fd_(sockfd)
{
}

Channel::~Channel()
{
}

void Channel::update()
{
    loop_->updateChannel(this);
}

void Channel::remove()
{
    loop_->removeChannel(this);
}

bool Channel::isWriting() const
{
    return events_ &  EPOLLOUT;
}

bool Channel::isReading() const
{
    return events_ & (EPOLLIN | EPOLLPRI);
}

void Channel::enableConnecting()
{
    events_ |= EPOLLIN;
    update();
}

void Channel::enableReading()
{
    events_ |= (EPOLLIN | EPOLLPRI | EPOLLET);
    update();
}

void Channel::disableReading()
{
    //todo why
    events_ &= ~(EPOLLIN | EPOLLPRI | EPOLLET);
    update();
}

void Channel::enableWriting()
{
    events_ |= EPOLLOUT;
    update();
}

void Channel::disableWriting()
{
    //todo why
    events_ &= ~EPOLLOUT;
    update();
}


void Channel::handleEvent()
{
    std::shared_ptr<void> guard;
    guard = tie_.lock(); //这样做是为了防止TcpConnectionPtr的引用计数变为零，从而被析构。
    if ((revents_ & EPOLLHUP) && !(revents_ & EPOLLIN))
    {
        printf("handleEvent close: %d\n",fd_);
        if (close_callback_) close_callback_();
    }
    if (revents_ & ( EPOLLIN | EPOLLPRI | EPOLLRDHUP)) 
    {
        if (read_callback_) read_callback_();
    }

    if (revents_ & EPOLLOUT)
    {
        if (write_callback_) write_callback_();
    }
    if (revents_ & EPOLLERR)
    {
        printf("handleEvent error: error_callback_,  revents_: %d\n", revents_);
        if (error_callback_) error_callback_();
    }

}

void Channel::tie(const std::shared_ptr<void>& obj)
{
    tie_ = obj;
}