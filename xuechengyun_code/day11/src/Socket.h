#pragma once

#include <functional>

#include "inet_address.h"

class Socket {
public:
    using Callback = std::function<void(Socket*)>;  // for connect/disconnect Socket
private:
    int fd;
public:
    Socket();
    Socket(int);
    ~Socket();

    void bind(InetAddress*);
    void listen();
    void setnonblocking();

    int accept(InetAddress*);
    int get_fd();
};
