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

void EventLoop::Loop() {
    while (!quit) {
        for (auto channel : ep->Poll()) {
            channel->HandleEvent();
        }
    }
}

void EventLoop::UpdateChannel(Channel* channel) {
    ep->UpdateChannel(channel);
}

