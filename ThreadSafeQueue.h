#pragma once

#ifndef SAFE_QUEUE
#define SAFE_QUEUE

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>

// A threadsafe-queue.
template <class T>
class SafeQueue
{
private:
    std::queue<T> q;
    mutable std::mutex m;
    std::condition_variable c;

public:
    SafeQueue(void)
        : q(), m(), c()
    {
    }

    ~SafeQueue(void)
    {
    }

    // Add an element to the queue.
    void enqueue(T t)
    {
        std::lock_guard<std::mutex> lock(m);
        q.push(t);
        c.notify_one();
    }

    // Get the "front"-element.
    // If the queue is empty, wait till a element is avaiable.
    T dequeue(void)
    {
        std::unique_lock<std::mutex> lock(m);
        while (q.empty())
        {
            // release lock as long as the wait and reaquire it afterwards.
            c.wait(lock);
            // Sleep(500);
            //std::cout << "aa";
        }
        //std::cout << "bb";

        T val = q.front();
        q.pop();
        return val;
    }
};
#endif

// #include <condition_variable>
// #include <memory>
// #include <mutex>
// #include <queue>

// template <typename T>
// class threadsafe_queue
// {
// private:
// 	std::queue<T> data_queue;
// 	std::mutex mut;
// 	std::condition_variable cond;

// public:
//     threadsafe_queue() {}
//     ~threadsafe_queue() {}

//     void push(T new_data)
//     {
//         std::lock_guard<std::mutex> lk(mut);  // 1.全局加锁
//         data_queue.push(std::move(new_data)); // 2.push时独占锁
//         cond.notify_one();
//     }
//     void wait_and_pop(T &val)
//     {
//         std::unique_lock<std::mutex> ulk(mut);                    // 3.全局加锁
//         cond.wait(ulk, [this]() { return !data_queue.empty(); }); // 4.front 和 pop_front时独占锁
//         val = std::move(data_queue.front());
//         data_queue.pop();
//     }
//     std::shared_ptr<T> wait_and_pop()
//     {
//         std::unique_lock<std::mutex> ulk(mut);
//         cond.wait(ulk, [this]() { return !data_queue.empty(); });
//         std::shared_ptr<T> val(std::make_shared<T>(std::move(data_queue.front())));
//         data_queue.pop();
//         return val;
//     }
//     bool try_pop(T &val)
//     {
//         std::lock_guard<std::mutex> lk(mut);
//         if (data_queue.empty())
//             return false;
//         val = std::move(data_queue.front());
//         data_queue.pop();
//         return true;
//     }
//     std::shared_ptr<T> try_pop()
//     {
//         std::shared_ptr<T> val;
//         std::lock_guard<std::mutex> lk(mut);
//         if (data_queue.empty())
//             return val;
//         val = std::make_shared<T>(std::move(data_queue.front()));
//         data_queue.pop();
//         return val;
//     }
//     bool empty()
//     {
//         std::lock_guard<std::mutex> lk(mut);
//         return data_queue.empty();
//     }
// };
