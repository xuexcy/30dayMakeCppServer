#pragma once

#include "acceptor.h"
#include "connection.h"

#include <map>

class EventLoop;
class Socket;

class Server {
public:
    Server(EventLoop*);
    ~Server();

    void new_connection(Socket* server_sock);  // 新加TCP连接
    void delete_connection(Socket* client_sock);  // 断开TCP连接
private:
    EventLoop* loop_{nullptr};
    Acceptor* acceptor_{nullptr};
    std::map<int, Connection*> connections_;  // 所有和客户端的连接
};
