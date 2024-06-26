#include "epoll.h"

#include <unistd.h>
#include <string.h>

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

void Epoll::add_fd(int fd, uint32_t op) {
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.fd = fd;
    ev.events = op;
    exit_if(-1 == epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev), "epoll add event error");
}

std::vector<epoll_event> Epoll::poll(int timeout) {
    std::vector<epoll_event> active_events;
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, timeout);
    exit_if(-1 == nfds, "epoll wait error");
    for (int i = 0; i < nfds; ++i) {
        active_events.push_back(events[i]);
    }
    return active_events;
}
