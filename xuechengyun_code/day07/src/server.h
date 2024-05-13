#pragma once

#include "acceptor.h"

class EventLoop;
class Socket;

class Server {
public:
    Server(EventLoop*);
    ~Server();

    void handle_read_event(int fd);
    void new_connection(Socket* server_sock);
private:
    EventLoop* loop_{nullptr};
    Acceptor* acceptor_{nullptr};
};
