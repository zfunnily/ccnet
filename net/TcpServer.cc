#include "TcpServer.h"
#include "EventLoop.h"
#include "Acceptor.h"
#include "EventLoopThreadPool.h"

void defaultConnectionCallback(const TcpConnectionPtr& conn)
{
  // do not call conn->forceClose(), because some users want to register message callback only.
  printf("a conn !!\n");
}

void defaultMessageCallback(const TcpConnectionPtr&,
                                        Buffer* buf)
{
    printf("message !!\n");
}

TcpServer::TcpServer(EventLoop *loop, const TcpAddr& listenAddr, std::string name)
: acceptor_(new Acceptor(loop, listenAddr)),
loop_(loop),
nextConnId_(0),
name_(name),
connect_callback_(defaultConnectionCallback),
message_callback_(defaultMessageCallback),
thread_pool_(new EventLoopThreadPool(loop, name))
{
    acceptor_->setNewConnectionCallback(std::bind(&TcpServer::newConnection, this, std::placeholders::_1, std::placeholders::_2));
}

TcpServer::~TcpServer()
{
}

 void TcpServer::setNumThreads(int num_threads)
 {
     thread_pool_->setThreadNum(num_threads);
 }

void TcpServer::start()
{
    thread_pool_->start();
    loop_->runInLoop(std::bind(&Acceptor::listen, acceptor_.get()));
}

std::shared_ptr<EventLoopThreadPool> TcpServer::threadPool()
{
    return thread_pool_;
}
struct sockaddr_in TcpServer::getSockAddr(int sockfd)
{
    struct sockaddr_in localaddr;
    memset(&localaddr,0 ,sizeof localaddr);
    socklen_t addrlen = static_cast<socklen_t>(sizeof localaddr);
    if (::getsockname(sockfd, (struct sockaddr*)(&localaddr), &addrlen) < 0)
    {
        perror("getsockname error\n");
    }
    return localaddr;
}


void TcpServer::newConnection(int sockfd, const TcpAddr& peerAddr)
{
    TcpAddr localaddr(getSockAddr(sockfd));
    char buf[64];
    sprintf(buf,":%d",nextConnId_++);
    std::string connName = name_ + buf;
    TcpConnectionPtr conn(new TcpConnection(loop_,
                                          connName,
                                          sockfd,
                                          localaddr,
                                          peerAddr));
    connections_[connName] = conn;
    conn->setConnectionCallback(connect_callback_);
    conn->setMessageCallback(message_callback_);
    conn->setWriteCompleteCallback(writecomplete_callback_);
    conn->setCloseCallback(
        std::bind(&TcpServer::removeConnection, this, std::placeholders::_1)); // FIXME: unsafe
    loop_->runInLoop(std::bind(&TcpConnection::connectEstablished, conn));
}

void TcpServer::removeConnection(const TcpConnectionPtr& conn)
{
    loop_->runInLoop(std::bind(&TcpServer::removeConnectionInLoop, this, conn));
}

void TcpServer::removeConnectionInLoop(const TcpConnectionPtr& conn)
{
    size_t n = connections_.erase(conn->name());
    EventLoop* ioLoop = conn->getLoop();
    ioLoop->runInLoop(
        std::bind(&TcpConnection::connectDestroyed, conn));
}

