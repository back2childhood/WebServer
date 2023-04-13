/* ************************************************************************
> File Name:     timer.cpp
> Author:        Qian JiLi
> mail:          193937157@qq.com
> Created Time:  Mon Apr 10 11:22:20 2023
> Description:   
 ************************************************************************/

#include "timer.h"
#include <cassert>
#include <chrono>
#include <sys/_types/_size_t.h>
#include <type_traits>

void TimeManager::siftup_(size_t i){
    assert(i >= 0 && i < heap_.size());
    size_t j = (i - 1) / 2;
    while(j >= 0){
        if(heap_[j] < heap_[i]) break;
        swapNode(i, j);
        i = j;
        j = (i - 1) / 2;
    }
}

void TimeManager::swapNode(size_t i, size_t j){
    assert(i >= 0 && i < heap_.size());
    assert(j >= 0 && j < heap_.size());
    std::swap(heap_[i], heap_[j]);
    ref_[heap_[i].id] = i;
    ref_[heap_[i].id] = j;
}

bool TimeManager::siftdown_(size_t index, size_t n){
    assert(index >= 0 && index < heap_.size());
    assert(n >= 0 && n < heap_.size());
    size_t i = index;
    size_t j = i * 2 + 1;
    while(j < n){
        if(j + 1 < n && heap_[j + 1] < heap_[j]) j ++;
        if(heap_[i] < heap_[j]) break;
        swapNode(i, j);
        i = j;
        j = i * 2 + 1;
    }
    return i > index;
}

void TimeManager::addTimer(int id, int timeout, const timeOutCallBack &cb){
    assert(id >= 0);
    size_t i;
    if(ref_.count(id) == 0){
        i = heap_.size();
        ref_[id] = i;
        heap_.push_back({id, Clock::now() + Ms(timeout), cb});
        siftup_(i);
    }else{
        i = ref_[id];
        heap_[i].expire = Clock::now() + Ms(timeout);
        heap_[i].cb = cb;
        if(!siftdown_(i, heap_.size())){
            siftup_(i);
        }
    }
}

void TimeManager::del_(size_t index){
    assert(!heap_.empty() && index < heap_.size() && index >= 0);
    size_t i = index;
    size_t n = heap_.size() - 1;
    if(i < n){
        swapNode(i, n);
        if(!siftdown_(i, n)){
            siftup_(i);
        }
    }
    ref_.erase(heap_.back().id);
    heap_.pop_back();
}

void TimeManager::pop(){
    assert(!heap_.empty());
    del_(0);
}

void TimeManager::clear(){
    ref_.clear();
    heap_.clear();
}

int TimeManager::getNetHandle(){
    handle_expired_event();
    size_t res = -1;
    if(!heap_.empty()){
        res = std::chrono::duration_cast<Ms>(heap_.front().expire - Clock::now()).count();
        if(res < 0) res = 0;
    }
    return res;
}
