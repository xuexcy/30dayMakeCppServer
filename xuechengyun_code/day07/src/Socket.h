#pragma once

#include "inet_address.h"

class Socket {
private:
    int fd;
public:
    Socket();
    Socket(int);
    ~Socket() = default;

    void bind(InetAddress*);
    void listen();
    void setnonblocking();

    int accept(InetAddress*);
    int get_fd();
};
