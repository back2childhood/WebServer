/* ************************************************************************
> File Name:     webserver.cpp
> Author:        Qian JiLi
> mail:          193937157@qq.com
> Created Time:  Sat Apr 15 11:51:53 2023
> Description:   
 ************************************************************************/

#include "webserver.h"
#include "httpconnection.h"
#include "kqueue.h"
#include <_types/_uint32_t.h>
#include <cassert>
#include <cstring>
#include <sys/event.h>
#include <unistd.h>

WebServer::WebServer(int port_, int trigMode, int timeoutMs_, bool openLinger_, int threadNum):
    port(port_), openLinger(openLinger_), timeoutMs(timeoutMs_), isClose(false),
    timer(new TimeManager()), threadpool(new ThreadPool(threadNum)), epoller(new Epoller()){
    srcDir = getcwd(nullptr, 256);
    assert(srcDir.size() > 0);
    srcDir += "/resources/";
    
    HttpConnection::userCount = 0;
    HttpConnection::srcDir = srcDir;

    initEventMode(trigMode);
    if(!initSocket()) isClose = true;
}

WebServer::~WebServer(){
    close(listenFd);
    isClose = true;
}

void WebServer::initEventMode(int trigMode){
    listenEvent = ;
    connectionEvent = EV_ONESHOT | EV_
    switch (trigMode) {
        case 0:
            break; 
        case 1:
            connectionEvent |= ET;
        default:
            
            break;
    }
}

void WebServer::start(){
    int timeMS = -1;
    if(!isClose){
        std::cout<<"============================";
        std::cout<<"Server Start!";
        std::cout<<"============================";
        std::cout<<std::endl;
    }
    while(!isClose){
        if(timeoutMs > 0){
            timeMS = timer -> getNetHandle();
        }
        int eventCnt = epoller->wait(timeMS);
        for(int i = 0; i < eventCnt; ++ i){
            int fd = epoller->getEventFd(i);
            uint32_t events = epoller->getEvents(i);

            if(fd == listenFd){
                handleListen();
            }else if()
        }
    }
}

