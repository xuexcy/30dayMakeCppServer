#pragma once

#include <cstdint>

class Epoll;

/*
typedef union epoll_data {
  void *ptr;
  int fd;
  uint32_t u32;
  uint64_t u64;
} epoll_data_t;
struct epoll_event {
  uint32_t events;	// Epoll events
  epoll_data_t data;	// User data variable
} __EPOLL_PACKED;
*/

// 我们期望 epoll_data 这个union类型的ptr指向 Channel
// 这样在事件发生时，就能通过Channel知道更多的信息，而不仅仅是 epoll_data.fd
// 也就是我们希望 Channel 对应一个fd，为fd负责，而且能处理更多的事
class Channel {
public:
    Channel(Epoll* _ep, int _fd);
    ~Channel() = default;

    void enable_reading(); // 将fd加入ep中，开启事件监听

    int get_fd() { return fd; }
    uint32_t get_events() { return events; }
    uint32_t get_revents() { return revents; }
    bool get_in_epoll() { return in_epoll; }

    void set_in_epoll();
    void set_revents(uint32_t);
private:
    void set_events(uint32_t);
private:
    Epoll* ep;  // fd的来源
    int fd;  // 当前连接的fd
    uint32_t events{0};  // 希望当前fd监听的时间
    // return events
    uint32_t revents{0};  // 当前fd正在发生的事件, 相当于 struct epoll_event.data
    bool in_epoll{false};  // 是否一个在ep的红黑树中
};

// Channel* serv_channel = new Channel(ep, serv_sock->get_fd());
// serv_channel->enable_reading();
