#pragma once
#include "TcpConnection.h"
class EventLoop;
class TcpAddr;
class Acceptor;
class TcpConnection;
class EventLoopThreadPool;

class TcpServer
{
public:
    typedef std::map<std::string, TcpConnectionPtr> ConnectionMap;
    typedef std::function<void(EventLoop*)> ThreadInitCallback;
public:
    TcpServer(EventLoop *loop, const TcpAddr& listenAddr, std::string name);
    ~TcpServer();

    void setNumThreads(int num_threads);
    void setThreadInitCallback(const ThreadInitCallback& cb) { threadinit_callback = std::move(cb);}
    void start();
    
    void setConnectionCallback(const ConnectionCallback& cb) { connect_callback_ = cb; }
    void setMessageCallback(const MessageCallback& cb)  { message_callback_ = cb; }
    void setWriteCompleteCallback(const WriteCompleteCallback& cb) { writecomplete_callback_ = cb; }

    std::shared_ptr<EventLoopThreadPool> threadPool();
private:
    void newConnection(int sockfd, const TcpAddr& peerAddr);
    void removeConnection(const TcpConnectionPtr& conn);
    void removeConnectionInLoop(const TcpConnectionPtr& conn);
    struct sockaddr_in getSockAddr(int sockfd);

private:
    EventLoop *loop_;
    std::unique_ptr<Acceptor> acceptor_;
    ConnectionMap connections_;

    //callback
    ConnectionCallback connect_callback_;
    MessageCallback message_callback_;
    WriteCompleteCallback writecomplete_callback_;
    ThreadInitCallback threadinit_callback;
    //thread pool
    std::shared_ptr<EventLoopThreadPool> thread_pool_;
    //id
    int nextConnId_;
    std::string name_;
};
