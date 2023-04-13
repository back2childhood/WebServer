/* ************************************************************************
> File Name:     httpconnection.cpp
> Author:        Qian JiLi
> mail:          193937157@qq.com
> Created Time:  Mon Apr 10 18:19:13 2023
> Description:   
 ************************************************************************/

#include "httpconnection.h"
#include <_types/_uint8_t.h>
#include <arpa/inet.h>
#include <cassert>
#include <ctime>
#include <sys/_types/_size_t.h>
#include <sys/_types/_ssize_t.h>
#include <sys/uio.h>

const char* HttpConnection::srcDir;
std::atomic<int> HttpConnection::userCount;
bool HttpConnection::isET;

HttpConnection::HttpConnection(){
    fd_ = -1;
    addr_ = {0};
    isClose_ = true;
}

HttpConnection::~HttpConnection(){
    closeHttpConn();
}

void HttpConnection::initHttpConn(int socketFd, const sockaddr_in &addr){
    assert(socketFd > 0);
    userCount ++;
    addr_ = addr;
    fd_ = socketFd;
    writeBuffer_.initPtr();
    readBuffer_.initPtr();
    isClose_ = false;
}

void HttpConnection::closeHttpConn(){
    if(isClose_ == false){
        isClose_ = true;
        userCount --;
        close(fd_);
    }
}

int HttpConnection::getFd() const{
    return fd_;
}

struct sockaddr_in HttpConnection::getAddr() const{
    return addr_;
}

const char* HttpConnection::getIP() const{
    return inet_ntoa(addr_.sin_addr);
}

int HttpConnection::getPort() const{
    return addr_.sin_port;
}

ssize_t HttpConnection::readBuffer(int *saveErrno){
    ssize_t len = -1;
    do{
        len = readBuffer_.readFd(fd_, saveErrno);
        if(len <= 0) break;
    }while(isET);
    return len;
}

ssize_t HttpConnection::writeBuffer(int *saveErrno){
    ssize_t len = -1;
    do{
        len = writev(fd_, iov_, iovCnt_);
        if(len <= 0){
            *saveErrno = errno;
            break;
        }
        if(iov_[0].iov_len + iov_[1].iov_len == 0) break;
        else if(static_cast<size_t> (len) > iov_[0].iov_len){
            iov_[1].iov_base = (uint8_t*)iov_[1].iov_base + (len - iov_[0].iov_len);
            iov_[1].iov_len -= (len - iov_[0].iov_len);
            if(iov_[0].iov_len){
                writeBuffer_.initPtr();
                iov_[0].iov_len = 0;
            }
        }else{
            iov_[0].iov_base = (uint8_t*)iov_[0].iov_base + len;
            iov_[0].iov_len -= len;
            writeBuffer_.updateReadPtr(len);
        }
    }while(isET || writeBytes() > 10240);
    return len;
}



































