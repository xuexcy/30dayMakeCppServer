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
            thread_pool_.add([channel]() {
                channel->handle_event();
            });
        }
    }
}

void EventLoop::update_channel(Channel* channel) {
    ep->update_channel(channel);
}

