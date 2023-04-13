/* ************************************************************************
> File Name:     safequeue.h
> Author:        Qian JiLi
> mail:          193937157@qq.com
> Created Time:  Sat Apr  8 21:42:34 2023
> Description:   
 ************************************************************************/

#ifndef _SAFEQUEUE_H_
#define _SAFEQUEUE_H_

#include "../mooc_project/public/_cmpublic.h"
#include <mutex>

template<typename T>
class SafeQueue{
private:
    std::queue<T> m_queue;
    std::mutex m_mutex;
public:
    SafeQueue(){

    }

    SafeQueue(SafeQueue &other){
        // TODO
    }

    ~SafeQueue(){

    }

    bool empty(){
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_queue.empty();
    }

    bool size(){
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_queue.size();
    }

    bool enqueue(T &t){ 
        std::unique_lock<std::mutex> lock(m_mutex);
        m_queue.push(t);
    }

    bool dequeue(T &t){
        std::unique_lock<std::mutex> lock(m_mutex);
        if(m_queue.empty()){
            return false;
        }
        t = std::move(m_queue.front());
        m_queue.pop();
        return true;
    }
};

#endif  // _SAFEQUEUE_H_
