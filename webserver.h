/* ************************************************************************
> File Name:     webserver.h
> Author:        Qian JiLi
> mail:          193937157@qq.com
> Created Time:  Tue Apr 11 09:43:52 2023
> Description:   
 ************************************************************************/

#ifndef _WEBSERVER_H_
#define _WEBSERVER_H_

#include "_cmpublic.h"
#include "httpconnection.h"
#include "threadPoll.h"
#include "timer.h"
#include "kqueue.h"
#include <memory>
#include <unordered_map>

class WebServer{
public:
    WebServer(int port, int trigMode, int timeoutMs, bool optLinger, int threadNum);
    ~WebServer();

    void start();       // everything start from this function
private:
    bool initSocket();

    void initEventMode(int trigMode);

    void addHttpConnection(int fd, sockaddr_in addr);
    void closeHttpConnection(HttpConnection* client);

    // when epoll have a event
    void handleListen();    // from listenFd: new connection
    void handleWrite(HttpConnection* client);   // write event
    void handleRead(HttpConnection* client);    // read event

    void onRead(HttpConnection* client);
    void onWrite(HttpConnection* client);
    void onProcess(HttpConnection* client);

    void sendError(int fd, const char* info);
    void extentTime(HttpConnection* client);

    static const int MAX_FD = 65536;
    static int setFdNonBlock(int fd);

    int port;
    int timeoutMs;
    bool isClose;
    int listenFd;
    bool openLinger;
    std::string srcDir;

    uint32_t listenEvent;
    uint32_t connectionEvent;

    std::unique_ptr<TimeManager> timer;
    std::unique_ptr<Epoller> epoller;
    std::unique_ptr<ThreadPool> threadpool;
    std::unordered_map<int, HttpConnection> users;
};

#endif  // _WEBSERVER_H_
