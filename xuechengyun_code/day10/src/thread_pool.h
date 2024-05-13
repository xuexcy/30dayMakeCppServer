#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

class ThreadPool {
public:
    using Task = std::function<void()>;
    ThreadPool(int size=std::thread::hardware_concurrency());
    ~ThreadPool();
    void add(ThreadPool::Task task);
private:
    std::vector<std::thread> threads_;
    std::queue<Task> tasks_;
    std::mutex mtx_;
    std::condition_variable cv_;
    bool stop_{false};
};
