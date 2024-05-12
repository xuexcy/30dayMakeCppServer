#include "channel.h"

#include "epoll.h"

Channel::Channel(Epoll* _ep, int _fd): ep(_ep), fd(_fd) {}

void Channel::enable_reading() {
    events = EPOLLIN | EPOLLET;
    ep->update_channel(this);
}

void Channel::set_events(uint32_t _ev) {
    events = _ev;
}

void Channel::set_revents(uint32_t _ev) {
    revents = _ev;
}

void Channel::set_in_epoll() {
    in_epoll = true;
}
