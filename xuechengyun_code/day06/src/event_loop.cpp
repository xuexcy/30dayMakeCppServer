#include "event_loop.h"

#include <vector>

#include "channel.h"
#include "epoll.h"

EventLoop::EventLoop() {
    ep = new Epoll();
}

EventLoop::~EventLoop() {
    delete ep;
}

void EventLoop::loop() {
    while (!quit) {
        std::vector<Channel*> chs = ep->poll();
        for (auto channel : chs) {
            channel->handle_event();
        }
    }
}

#include <iostream>

void EventLoop::update_channel(Channel* channel) {
    std::cout << channel->get_fd() << std::endl;
    ep->update_channel(channel);
}

