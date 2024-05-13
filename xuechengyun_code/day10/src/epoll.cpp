#include "epoll.h"

#include <unistd.h>
#include <string.h>

#include "channel.h"
#include "util.h"

static const uint MAX_EVENTS{1024};

Epoll::Epoll() : epfd(-1), events(nullptr) {
    epfd = epoll_create1(0);
    exit_if(-1 == epfd, "epoll create error");
    events = new epoll_event[MAX_EVENTS];
    bzero(events, sizeof(*events) * MAX_EVENTS);
}

Epoll::~Epoll() {
    if (epfd != -1) {
        close(epfd);
        epfd = -1;
    }
    delete [] events;
}

/*
void Epoll::add_fd(int fd, uint32_t op) {
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.fd = fd;
    ev.events = op;
    exit_if(-1 == epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev), "epoll add event error");
}
*/

void Epoll::update_channel(Channel* channel) {
    int fd = channel->get_fd();
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.ptr = channel;  // union类型的data不再是fd，而是指向Channel的ptr
    ev.events = channel->get_events();
    if (!channel->get_in_epoll()) {
        // 将channel放到epool中, 其实就是需要channel的地址(ev.data.ptr=channel)
        exit_if(-1 == epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev), "epoll add error");
        channel->set_in_epoll();
    } else {
        exit_if(-1 == epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev), "epoll modify error");
    }
}
/*
std::vector<epoll_event> Epoll::poll(int timeout) {
    std::vector<epoll_event> active_events;
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, timeout);
    exit_if(-1 == nfds, "epoll wait error");
    for (int i = 0; i < nfds; ++i) {
        active_events.push_back(events[i]);
    }
    return active_events;
}
*/

std::vector<Channel*> Epoll::poll(int timeout) {
    std::vector<Channel*> active_channels;
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, timeout);
    exit_if(-1 == nfds, "epoll wait error");
    for (int i = 0; i < nfds; ++i) {
        auto ch = (Channel*)events[i].data.ptr;
        ch->set_revents(events[i].events);
        active_channels.push_back(ch);
    }
    // 原来存的 epoll_event, 现在返回 Channel，里面存了 fd + (r)events
    return active_channels;
}
