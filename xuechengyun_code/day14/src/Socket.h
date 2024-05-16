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

    void Bind(InetAddress*);
    void Listen();
    void SetNonBlocking();
    bool IsNonBlocking();

    int Accept(InetAddress*);
    int GetFd();
};
