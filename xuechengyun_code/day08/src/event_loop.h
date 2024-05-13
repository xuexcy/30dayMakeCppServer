#pragma once

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
    bool quit{false};
};
