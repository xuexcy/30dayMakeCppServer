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
private:
    Epoll* ep{nullptr};
    ThreadPool thread_pool_;
    bool quit{false};
};
