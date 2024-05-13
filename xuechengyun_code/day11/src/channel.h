#pragma once

#include <cstdint>
#include <functional>

class EventLoop;


class Channel {
public:
    using Callback = std::function<void()>;
    Channel(EventLoop* loop, int fd);
    ~Channel() = default;

    void handle_event();
    void enable_reading(); // 将fd加入ep中，开启事件监听

    int get_fd() { return fd_; }
    uint32_t get_events() { return events_; }
    uint32_t get_revents() { return revents_; }
    bool get_in_epoll() { return in_epoll_; }

    void set_in_epoll();
    void set_revents(uint32_t);
    void set_callback(Callback);
private:
    void set_events(uint32_t);
private:
    EventLoop* loop_;  // fd的来源
    int fd_;  // 当前连接的fd
    uint32_t events_{0};  // 希望当前fd监听的时间
    // return events
    uint32_t revents_{0};  // 当前fd正在发生的事件, 相当于 struct epoll_event.data
    bool in_epoll_{false};  // 是否一个在ep的红黑树中
    Callback callback_;
};

// Channel* serv_channel = new Channel(ep, serv_sock->get_fd());
// serv_channel->enable_reading();
