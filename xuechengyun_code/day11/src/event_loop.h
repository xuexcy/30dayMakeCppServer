#pragma once

#include "thread_pool.h"

class Epoll;
class Channel;

class EventLoop {
public:
    EventLoop();
    ~EventLoop();

    void loop();
    void update_channel(Channel*);
    void add_task(ThreadPool::Task task);  // 如果 callback 都在 thread_pool 中执行，就可以不需要这个函数
private:
    Epoll* ep{nullptr};
    ThreadPool thread_pool_;
    bool quit{false};
};
