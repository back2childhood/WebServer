/* ************************************************************************
> File Name:     timer.h
> Author:        Qian JiLi
> mail:          193937157@qq.com
> Created Time:  Mon Apr 10 10:27:10 2023
> Description:   
 ************************************************************************/

#ifndef _TIMER_H_
#define _TIMER_H_

#include "../mooc_project/public/_cmpublic.h"
#include <chrono>
#include <memory>
#include <sys/_types/_size_t.h>
#include <vector>

typedef std::function<void()> timeOutCallBack;
typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::milliseconds Ms;
typedef Clock::time_point timeStamp;

class TimeNode{
public:
    int id;
    timeStamp expire;
    timeOutCallBack cb;

    bool operator<(const TimeNode& t){
        return expire < t.expire;
    }
};

class TimeManager{
private:
    typedef std::shared_ptr<TimeNode> SP_timeNode;
public:
    TimeManager(){
        heap_.reserve(64);
    };

    ~TimeManager(){
        clear();
    }

    void addTimer(int id, int timeout, const timeOutCallBack& cb);

    void handle_expired_event();

    int getNetHandle();

    void update(int id, int timeout);

    void work(int id);

    void pop();
    void clear();
private:
    void del_(size_t i);
    void siftup_(size_t i);
    bool siftdown_(size_t index, size_t n);
    void swapNode(size_t i, size_t j);

    std::vector<TimeNode> heap_;
    std::unordered_map<int, size_t> ref_;
};

#endif  // _TIMER_H_
