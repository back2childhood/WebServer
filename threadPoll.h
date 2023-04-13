/* ************************************************************************
> File Name:     threadPool.h
> Author:        Qian JiLi
> mail:          193937157@qq.com
> Created Time:  Mon Apr 10 00:13:28 2023
> Description:   
 ************************************************************************/

#ifndef _threadPool_H_
#define _threadPool_H_

#include "../mooc_project/public/_cmpublic.h"
#include "safequeue.h"
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <utility>
#include <vector>

class ThreadPool{
private:
    class ThreadWorker{
    private:
        int m_id;
        ThreadPool* m_pool;
    public:
        ThreadWorker(ThreadPool* pool, const int id)
        : m_pool(pool), m_id(id){

        }

        void operator()(){
            std::function<void()> func;
            bool dequeued;
            while(!m_pool -> m_shutdown){
                std::unique_lock<std::mutex> lock(m_pool->m_conditional_mutex);
                if(m_pool -> m_queue.empty()){
                    m_pool -> m_conditional_lock.wait(lock);
                }
                dequeued = m_pool -> m_queue.dequeue(func);
            }
            if(dequeued)
                func();
        }
    };
    bool m_shutdown;
    SafeQueue<std::function<void()>> m_queue;
    std::vector<std::thread> m_threads;
    std::mutex m_conditional_mutex;
    std::condition_variable m_conditional_lock;
public:
    ThreadPool(const int n_threads):
    m_threads(std::vector<std::thread>(n_threads)), m_shutdown(false){
    }

    ThreadPool(const ThreadPool& ) = delete;
    ThreadPool(ThreadPool&& ) = delete;

    void init(){
        for(int i = 0; i < m_threads.size(); ++ i){
            m_threads[i] = std::thread(ThreadWorker(this, i));
        }
    }

    void shutdown(){
        m_shutdown = true;
        m_conditional_lock.notify_all();

        for(int i = 0; i < m_threads.size(); ++ i){
            if(m_threads[i].joinable()){
                m_threads[i].join();
            }
        }
    }

    template<typename F, typename...Args>
    auto submit(F&& f, Args&&... args) -> std::future<decltype(f(args...))>{
        std::function<decltype(f(args...))()> func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        auto task_ptr = std::make_shared<std::packaged_task<decltype(f(args...))>> (func);

        std::function<void()> wrapper_func = [task_ptr](){
            (*task_ptr)();
        };

        m_queue.enqueue(wrapper_func);
        m_conditional_lock.notify_one();

        return task_ptr->getfure();
    }
};

#endif  // _threadPool_H_
