#pragma once
#include <map>
#include "Callbacks.h"
#include "TcpAddr.h"
#include "Buffer.h"

class EventLoop;
class Channel;
class TcpAddr;
class Socket;
class Buffer;

class TcpConnection : public std::enable_shared_from_this<TcpConnection>
{
public:
enum ConnStateE{
    kDisconnected,
    kConnecting,
    kConnected,
    kDisconnecting
};
    TcpConnection(EventLoop* loop, std::string name, int sockfd, const TcpAddr& localaddr, const TcpAddr&  peeraddr);
    ~TcpConnection();

    void setConnectionCallback(ConnectionCallback cb) { connect_callback_ = std::move(cb);}
    void setMessageCallback(MessageCallback cb){ message_callback_ = std::move(cb);}
    void setWriteCompleteCallback(WriteCompleteCallback cb){ writecomplete_callback_ = std::move(cb);}
    void setCloseCallback(CloseCallback cb) { close_callback_ = std::move(cb);}

    void connectEstablished();
    void connectDestroyed();

    //send
    void send(Buffer* msg);
    void sendInLoop(const char* data, int len);

    //socket state
    void setConnState(ConnStateE state) { conn_state_ = state;}
    void shutdown();
    void shutdownInLoop();

    //io
    void handleRead();
    void handleWrite();
    void handleClose();
    void handleError();

    std::string name() { return name_; }
    EventLoop* getLoop() { return loop_; }
private:
    EventLoop *loop_;
    TcpAddr local_addr_;
    TcpAddr peer_addr_;
    std::string name_;
    int fd_;
    ConnStateE conn_state_;
    //socket
    std::unique_ptr<Socket> socket_;
    //buffer
    Buffer input_buffer_;
    Buffer output_buffer_; // FIXME: use list<Buffer> as output buffer.
    //channel
    std::unique_ptr<Channel> channel_;
    //callback
    ConnectionCallback connect_callback_;
    MessageCallback message_callback_;
    WriteCompleteCallback writecomplete_callback_;
    CloseCallback close_callback_;
};