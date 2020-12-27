#pragma once

#include <sys/types.h>
#include <vector>

class Buffer
{
public:
    static const size_t kInitialHeadSize = 8;
    static const size_t kInitialSize = 1024;

    explicit Buffer(ssize_t initialsize = kInitialSize);
    ~Buffer();
    
    ssize_t      writableBytes();
    ssize_t      readableBytes();
    const char* peek();
    void        retrieve(ssize_t len);
    void        retrieveAll();

    char*       begin();
    char*       beginWrite();
    //recv
    ssize_t     readFd(int fd , int* savedErr); 

    //send
    ssize_t     append(const char* data, ssize_t len);

    //space
    void        makeSpace(ssize_t len);

private:
    std::vector<char> buffer_;
    ssize_t write_index_;
    ssize_t read_index_;
};