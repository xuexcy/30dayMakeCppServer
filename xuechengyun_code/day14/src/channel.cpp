#include "channel.h"


#include <sys/epoll.h>

#include "event_loop.h"

Channel::Channel(EventLoop* loop, int fd): loop_(loop), fd_(fd) {}

void Channel::EnableReading() {
    listen_events_ = EPOLLIN | EPOLLET;
    loop_->UpdateChannel(this);
}

void Channel::HandleEvent() {
    read_callback_();
}

void Channel::SetListenEvents(uint32_t ev) {
    listen_events_ = ev;
}

void Channel::SetReadyEvents(uint32_t ev) {
    ready_events_ = ev;
}

void Channel::SetInEpoll(bool in) {
    in_epoll_ = in;
}

void Channel::SetReadCallback(Channel::Callback callback) {
    read_callback_ = callback;
}
