/* ************************************************************************
> File Name:     kqueue.h
> Author:        Qian JiLi
> mail:          193937157@qq.com
> Created Time:  Tue Apr 11 09:45:23 2023
> Description:   
 ************************************************************************/

#ifndef _KQUEUE_H_
#define _KQUEUE_H_

#include "_cmpublic.h"
#include <sys/event.h>
#include <errno.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <_types/_uint32_t.h>
#include <sys/_types/_size_t.h>

const int kReadEvent = 1;
const int kWriteEvent = 2;

class Epoller{
public:
    explicit Epoller(int maxEvent = 1024);
    ~Epoller();

    bool addFd(int fd, uint32_t events);
    bool modFd(int fd, uint32_t events);
    bool delFd(int fd);

    int wait(int timewait = -1);

    int getEventFd(size_t i) const;

    uint32_t getEvents(size_t i) const;
private:
    int epollerFd_;
    struct kevent* events_;
    int maxlen;
};

#endif  // _KQUEUE_H_
