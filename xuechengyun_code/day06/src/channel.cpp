#include "channel.h"


#include <sys/epoll.h>

#include "event_loop.h"

Channel::Channel(EventLoop* loop, int fd): loop_(loop), fd_(fd) {}

void Channel::enable_reading() {
    events_ = EPOLLIN | EPOLLET;
    loop_->update_channel(this);
}

void Channel::handle_event() {
    callback_();
}

void Channel::set_events(uint32_t ev) {
    events_ = ev;
}

void Channel::set_revents(uint32_t ev) {
    revents_ = ev;
}

void Channel::set_in_epoll() {
    in_epoll_ = true;
}

void Channel::set_callback(Callback callback) {
    callback_ = callback;
}
