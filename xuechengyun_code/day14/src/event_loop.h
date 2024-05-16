#pragma once

#include "thread_pool.h"

class Epoll;
class Channel;

class EventLoop {
public:
    EventLoop();
    ~EventLoop();

    void Loop();
    void UpdateChannel(Channel*);
private:
    Epoll* ep{nullptr};
    bool quit{false};
};
