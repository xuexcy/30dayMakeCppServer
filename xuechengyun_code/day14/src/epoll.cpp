#include "epoll.h"

#include <unistd.h>
#include <string.h>

#include "channel.h"
#include "util.h"

static const uint MAX_EVENTS{1024};

Epoll::Epoll() : epfd_(-1), events(nullptr) {
    epfd_ = epoll_create1(0);
    exit_if(-1 == epfd_, "epoll create error");
    events = new epoll_event[MAX_EVENTS];
    bzero(events, sizeof(*events) * MAX_EVENTS);
}

Epoll::~Epoll() {
    if (epfd_ != -1) {
        close(epfd_);
        epfd_ = -1;
    }
    delete [] events;
}

void Epoll::UpdateChannel(Channel* channel) {
    int fd = channel->GetFd();
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.ptr = channel;  // union类型的data不再是fd，而是指向Channel的ptr
    ev.events = channel->GetEvents();
    if (!channel->GetInEpoll()) {
        // 将channel放到epool中, 其实就是需要channel的地址(ev.data.ptr=channel)
        exit_if(-1 == epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &ev), "epoll add error");
        channel->SetInEpoll();
    } else {
        exit_if(-1 == epoll_ctl(epfd_, EPOLL_CTL_MOD, fd, &ev), "epoll modify error");
    }
}

void Epoll::DeleteChannel(Channel* channel) {
    exit_if(-1 == epoll_ctl(epfd_, EPOLL_CTL_DEL, channel->GetFd(), nullptr), "epoll delete error");
    channel->SetInEpoll(false);
}

std::vector<Channel*> Epoll::Poll(int timeout) {
    std::vector<Channel*> active_channels;
    int nfds = epoll_wait(epfd_, events, MAX_EVENTS, timeout);
    exit_if(-1 == nfds, "epoll wait error");
    for (int i = 0; i < nfds; ++i) {
        auto ch = (Channel*)events[i].data.ptr;
        ch->SetReadyEvents(events[i].events);
        active_channels.push_back(ch);
    }
    // 原来存的 epoll_event, 现在返回 Channel，里面存了 fd + (r)events
    return active_channels;
}
