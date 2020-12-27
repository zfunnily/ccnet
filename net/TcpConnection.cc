#include "TcpConnection.h"
#include "EventLoop.h"
#include "TcpAddr.h"
#include "Channel.h"
#include "Socket.h"


TcpConnection::TcpConnection(EventLoop* loop, std::string name, int sockfd, const TcpAddr& localaddr, const TcpAddr&  peeraddr)
:loop_(loop),
name_(name),
local_addr_(localaddr),
peer_addr_(peeraddr),
fd_(sockfd),
channel_(new Channel(loop,sockfd))
{
    channel_->setReadCallback(std::bind(&TcpConnection::handleRead, this));
    channel_->setWriteCallback(std::bind(&TcpConnection::handleWrite, this));
    channel_->setErrorCallback(std::bind(&TcpConnection::handleError, this));
    channel_->setCloseCallback(std::bind(&TcpConnection::handleClose, this));
}

TcpConnection::~TcpConnection()
{
 
}

void TcpConnection::connectEstablished()
{
  setConnState(kConnected);
  channel_->enableReading();
  channel_->tie(shared_from_this());
  connect_callback_(shared_from_this());
}

void TcpConnection::connectDestroyed()
{
  if (conn_state_ == kConnected)
  {
    setConnState(kDisconnected);
    channel_->disableAll();
    connect_callback_(shared_from_this());
  }
  //channel_->remove();
}

void TcpConnection::send(Buffer* msg)
{
  if (conn_state_ == kConnected)
  {
    loop_->runInLoop(std::bind(&TcpConnection::sendInLoop,this, msg->peek(), msg->readableBytes()));
  }
}

void TcpConnection::sendInLoop(const char* data, int len)
{
  if (conn_state_ == kConnected)
  {
    output_buffer_.append(static_cast<const char*>(data), len);
    if (!channel_->isWriting())
    {
      channel_->enableWriting();
    }
  }
}

void TcpConnection::shutdown()
{
  if (conn_state_ == kConnected)
  {
    setConnState(kDisconnecting);
    loop_->runInLoop(std::bind(&TcpConnection::shutdownInLoop, this));
  }
}

void TcpConnection::shutdownInLoop()
{
  if (!channel_->isWriting())
  {
    socket_->shutdownWrite();
  }
}

void TcpConnection::handleRead()
{
  int save_err;
  ssize_t n = input_buffer_.readFd(channel_->fd(),  &save_err);
  // printf("handleRead::  recv len: %d\n", n);
  if (n > 0)
  {
    message_callback_(shared_from_this(), &input_buffer_);
  }
  else if (n == 0)
  {
    handleClose();
  }
  else
  {
    errno = save_err;
    handleError();
  }
}

void TcpConnection::handleWrite()
{
  if (channel_ && channel_->isWriting())
  {
    ssize_t n = ::send(channel_->fd(), output_buffer_.peek(), output_buffer_.readableBytes(), 0);
    if (n > 0)
    {
      output_buffer_.retrieve(n);
      if (output_buffer_.readableBytes() == 0)
      {
        //todo  缓冲区没有数据可以写入了。
        channel_->disableWriting();
      }

    }
  }
}

void TcpConnection::handleClose()
{
  setConnState(kDisconnected);
  channel_->disableAll();
  close_callback_(shared_from_this());
}

void TcpConnection::handleError()
{
  printf("TcpConnection::handleError, fd: %d\n", fd_);
}