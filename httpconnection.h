#ifndef HTTP_CONNECTION_H
#define HTTP_CONNECTION_H

#include<arpa/inet.h> //sockaddr_in
#include<sys/uio.h> //readv/writev
#include<iostream>
#include<sys/types.h>
#include<assert.h>

#include "buffer.h"
#include "HTTPrequest.h"
// #include "HTTPresponse.h"

class HttpConnection{
public:
    HttpConnection();
    ~HttpConnection();

    void initHttpConn(int socketFd,const sockaddr_in& addr_);

    //每个连接中定义的对缓冲区的读写接口
    ssize_t read(int* saveErrno);
    ssize_t write(int* saveErrno);

    //关闭HTTP连接的接口
    void closeHttpConn();
    //定义处理该HTTP连接的接口，主要分为request的解析和response的生成
    bool handleHttpConn();

    //其他方法
    const char* getIP() const;
    int getPort() const;
    int getFd() const;
    sockaddr_in getAddr() const;

    int writeBytes(){
        return iov[0].iov_len+iov[1].iov_len;
    }

    bool isKeepAlive() const
    {
        return request.isKeepAlive();
    }

    static bool isET;
    static std::string srcDir;
    static std::atomic<int>userCount;

private:
    int fd;                  //HTTP连接对应的描述符
    struct sockaddr_in addr;
    bool isClose;            //标记是否关闭连接

    int iovCnt_;
    struct iovec iov[2];

    Buffer readBuffer;       //读缓冲区
    Buffer writeBuffer;      //写缓冲区

    HttpRequest request;    
    // HTTPresponse response_;

};

#endif //HTTP_CONNECTION_H
