#include "thread_pool.h"

ThreadPool::ThreadPool(int size) {
    for (int i = 0; i < size; ++i) {
        threads_.emplace_back([this] {
            while (true) {
                Task task;
                {
                    std::unique_lock<std::mutex> lk(mtx_);
                    cv_.wait(lk, [this]() { return stop_ || !tasks_.empty(); });
                    if (stop_ && tasks_.empty()) {
                        return;
                    }
                    task = tasks_.front();
                    tasks_.pop();
                }
                task();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lk(mtx_);
        stop_ = true;
    }
    cv_.notify_all();
    for (auto& thread : threads_) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}
void ThreadPool::add(ThreadPool::Task task) {
    {
        std::unique_lock<std::mutex> lk(mtx_);
        if (stop_) {
            throw std::runtime_error("ThreadPool already stop, can't add task any more");
        }
        tasks_.emplace(task);
    }
    cv_.notify_one();
}
