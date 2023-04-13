/* ************************************************************************
> File Name:     buffer.h
> Author:        Qian JiLi
> mail:          193937157@qq.com
> Created Time:  Thu Apr  6 15:59:04 2023
> Description:   
 ************************************************************************/

#ifndef _BUFFER_H_
#define _BUFFER_H_

#include "../mooc_project/public/_cmpublic.h"
#include <sys/_types/_size_t.h>
#include <sys/_types/_ssize_t.h>

class Buffer{
public:
    Buffer(int initBufferSize = 1024);
    ~Buffer()=default;

    size_t writeableBytes() const;
    size_t readableBytes() const;
    size_t readBytes() const;

    const char* curReadPtr() const;
    const char* curWritePtrConst() const;
    char* curWritePtr();

    void updateReadPtr(size_t len);
    void updateReadPtrUntilEnd(const char* end);

    void updateWritePtr(size_t len);

    void initPtr();

    void ensureWriteable(size_t len);

    void append(const char* str, size_t len);
    void append(const std::string& str);
    void append(const void* data, size_t len);
    void append(const Buffer& buffer);

    ssize_t readFd(int fd, int* Errno);
    ssize_t writeFd(int fd, int* Errno);

    std::string Alltostr();
private:
    char* BeginPtr_();
    const char* BeginPtr_() const;

    void allocateSpace(size_t len);

    std::vector<char> buffer_;
    std::atomic<std::size_t> readPos_;
    std::atomic<std::size_t> writePos_;
};

#endif  // _BUFFER_H_
