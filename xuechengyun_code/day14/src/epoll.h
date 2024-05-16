#pragma once

#include <sys/epoll.h>
#include <vector>

class Channel;

class Epoll {
private:
    int epfd_;
    struct epoll_event* events;
public:
    Epoll();
    ~Epoll();

    void UpdateChannel(Channel* channel);
    void DeleteChannel(Channel* channel);
    std::vector<Channel*> Poll(int timeout = -1);
};
