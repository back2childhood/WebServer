/* ************************************************************************
> File Name:     kqueue.cpp
> Author:        Qian JiLi
> mail:          193937157@qq.com
> Created Time:  Tue Apr 11 10:38:10 2023
> Description:   
 ************************************************************************/

#include "kqueue.h"
#include <_types/_uint32_t.h>
#include <cassert>
#include <cstddef>
#include <malloc/_malloc.h>
#include <sys/_types/_intptr_t.h>
#include <sys/event.h>

Epoller::Epoller(int maxEvent):epollerFd_(kqueue()), maxlen(maxEvent){
    events_ = (struct kevent*)malloc(maxEvent);
    assert(epollerFd_ >= 0 && maxlen > 0);
}

Epoller::~Epoller(){
    close(epollerFd_);
}

bool Epoller::addFd(int fd, uint32_t events){
    if(fd < 0) return false;
    struct kevent ev[2];
    int n = 0;
    if(events & kReadEvent)
        EV_SET(&ev[n++], fd, EVFILT_READ, EV_ADD|EV_ENABLE, 0, 0, (void*)(intptr_t)fd);
    if(events & kWriteEvent)
        EV_SET(&ev[n++], fd, EVFILT_WRITE, EV_ADD|EV_ENABLE, 0, 0, (void*)(intptr_t)fd);
    return -1 != kevent(epollerFd_, ev, n, NULL, 0, NULL);
}

bool Epoller::modFd(int fd, uint32_t events){
    if(fd < 0) return false;
    struct kevent ev[2];
    int n = 0;
    if(events & kReadEvent)
        EV_SET(&ev[n++], fd, EVFILT_READ, EV_ADD|EV_ENABLE, 0, 0, (void*)(intptr_t)fd);
    else EV_SET(&ev[n++], fd, EVFILT_READ, EV_DELETE, 0, 0, (void*)(intptr_t)fd);
    if(events & kWriteEvent)
        EV_SET(&ev[n++], fd, EVFILT_WRITE, EV_ADD|EV_ENABLE, 0, 0, (void*)(intptr_t)fd);
    else EV_SET(&ev[n++], fd, EVFILT_WRITE, EV_DELETE, 0, 0, (void*)(intptr_t)fd);
    return -1 != kevent(epollerFd_, ev, n, NULL, 0, NULL);
}

bool Epoller::delFd(int fd){
    if(fd < 0) return false;
    struct kevent ev[2];
    int n = 0;
    EV_SET(&ev[n++], fd, EVFILT_WRITE, EV_DELETE, 0, 0, (void*)(intptr_t)fd);
    EV_SET(&ev[n++], fd, EVFILT_READ, EV_DELETE, 0, 0, (void*)(intptr_t)fd);
    return -1 != kevent(epollerFd_, ev, n, NULL, 0, NULL);
}

int Epoller::wait(int timeoutMs){
    struct timespec timeout;
    timeout.tv_sec = timeoutMs / 1000;
    timeout.tv_nsec = (timeoutMs % 1000) * 1000 * 1000;
    return kevent(epollerFd_, NULL, 0, events_, maxlen, &timeout);
}

int Epoller::getEventFd(size_t i) const{
    assert(i < maxlen && i >= 0);
    return events_[i].ident;
}

uint32_t Epoller:: getEvents(size_t i) const{
    assert(i < maxlen && i >= 0);
    return events_[i].filter;
}
