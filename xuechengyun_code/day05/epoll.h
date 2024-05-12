#pragma once

#include <sys/epoll.h>
#include <vector>

class Channel;

class Epoll {
private:
    int epfd;
    struct epoll_event* events;
public:
    Epoll();
    ~Epoll();

    // 由Channel中的fd和events代替下面参数
    // void add_fd(int fd, uint32_t op);
    // std::vector<epoll_event> poll(int timeout = -1);
    void update_channel(Channel* channel);
    std::vector<Channel*> poll(int timeout = -1);
};
