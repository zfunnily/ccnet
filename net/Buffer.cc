#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>

#include "Buffer.h"

Buffer::Buffer(ssize_t initialsize)
:buffer_(kInitialHeadSize + initialsize),
write_index_(kInitialHeadSize),
read_index_(kInitialHeadSize)
{
}

Buffer::~Buffer()
{
}

ssize_t Buffer::readableBytes()
{
    return write_index_ - read_index_;
}

ssize_t Buffer::writableBytes()
{
    return buffer_.size() - write_index_;
}

const char* Buffer::peek()
{
    return begin() + read_index_;
}

void Buffer::retrieve(ssize_t len)
{
    if (len < readableBytes())
    {
        read_index_ += len;
    }else
    {
        retrieveAll();
    }
    
}

void Buffer::retrieveAll()
{
    read_index_ = kInitialHeadSize;
    write_index_ = kInitialHeadSize;
}

char* Buffer::begin()
{
    return &*buffer_.begin();
}

char* Buffer::beginWrite()
{
    return begin() + write_index_;
}
ssize_t Buffer::readFd(int fd , int* savedErr)
{
    ssize_t n = 0;
    while (true)
    {
        ssize_t writeable =  writableBytes();
        ssize_t l = ::recv(fd, begin()+write_index_,writeable, MSG_DONTWAIT);
        if (l <= 0)
        {
            *savedErr = errno;
            break;
        }
        if (l <= writeable)
        {
            write_index_ += l;
        }
        else
        {
            write_index_ = buffer_.size();
        }
        n += l;
    }
    return n;
}

ssize_t Buffer::append(const char* data, ssize_t len)
{
    //todo makespace
    makeSpace(len);
    std::copy(data, data+len, beginWrite());
    write_index_ += len;
}


void Buffer::makeSpace(ssize_t len)
{
    if (len > writableBytes())
    {
        buffer_.resize(write_index_ + len);
    }
    else
    {
        //todo
    }
    
}
