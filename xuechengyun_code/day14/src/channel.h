#pragma once

#include <cstdint>
#include <functional>

class EventLoop;


class Channel {
public:
    using Callback = std::function<void()>;
    Channel(EventLoop* loop, int fd);
    ~Channel() = default;

    void HandleEvent();
    void EnableReading(); // 将fd加入ep中，开启事件监听

    int GetFd() { return fd_; }
    uint32_t GetEvents() { return listen_events_; }
    uint32_t GetRevents() { return ready_events_; }
    bool GetInEpoll() const { return in_epoll_; }

    bool GetInEpoll() { return in_epoll_; }
    void SetInEpoll(bool in = true);
    void SetReadyEvents(uint32_t);
    void SetReadCallback(Callback);
private:
    void SetListenEvents(uint32_t);
private:
    EventLoop* loop_;  // fd的来源
    int fd_;  // 当前连接的fd
    uint32_t listen_events_{0};  // 希望当前fd监听的时间
    // return events
    uint32_t ready_events_{0};  // 当前fd正在发生的事件, 相当于 struct epoll_event.data
    bool in_epoll_{false};  // 是否一个在ep的红黑树中
    Callback read_callback_;
};
