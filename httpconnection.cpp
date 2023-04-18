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

const std::string HttpConnection::srcDir;
std::atomic<int> HttpConnection::userCount;
bool HttpConnection::isET;

HttpConnection::HttpConnection(){
    fd = -1;
    addr = {0};
    isClose = true;
}

HttpConnection::~HttpConnection(){
    closeHttpConn();
}

void HttpConnection::initHttpConn(int socketFd, const sockaddr_in &addr_){
    assert(socketFd > 0);
    userCount ++;
    addr = addr_;
    fd = socketFd;
    writeBuffer.initPtr();
    readBuffer.initPtr();
    isClose = false;
}

void HttpConnection::closeHttpConn(){
    if(isClose == false){
        isClose = true;
        userCount --;
        close(fd);
    }
}

int HttpConnection::getFd() const{
    return fd;
}

struct sockaddr_in HttpConnection::getAddr() const{
    return addr;
}

const char* HttpConnection::getIP() const{
    return inet_ntoa(addr.sin_addr);
}

int HttpConnection::getPort() const{
    return addr.sin_port;
}

ssize_t HttpConnection::read(int *saveErrno){
    ssize_t len = -1;
    do{
        len = readBuffer.readFd(fd, saveErrno);
        if(len <= 0) break;
    }while(isET);
    return len;
}

ssize_t HttpConnection::write(int *saveErrno){
    ssize_t len = -1;
    do{
        len = writev(fd, iov, iovCnt_);
        if(len <= 0){
            *saveErrno = errno;
            break;
        }
        if(iov[0].iov_len + iov[1].iov_len == 0) break;
        else if(static_cast<size_t> (len) > iov[0].iov_len){
            iov[1].iov_base = (uint8_t*)iov[1].iov_base + (len - iov[0].iov_len);
            iov[1].iov_len -= (len - iov[0].iov_len);
            if(iov[0].iov_len){
                writeBuffer.initPtr();
                iov[0].iov_len = 0;
            }
        }else{
            iov[0].iov_base = (uint8_t*)iov[0].iov_base + len;
            iov[0].iov_len -= len;
            writeBuffer.updateReadPtr(len);
        }
    }while(isET || writeBytes() > 10240);
    return len;
}

bool HttpConnection::handleHttpConn(){
    request.init();
    if(readBuffer.readableBytes() <= 0) return false;
    // else if(request.parse(readBuffer))  res
    else{
        std::cout << "400!" << std::endl;
        // response
    }


}



































