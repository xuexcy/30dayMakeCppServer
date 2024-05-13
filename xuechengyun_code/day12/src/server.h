#pragma once

#include "acceptor.h"
#include "connection.h"
#include "thread_pool.h"

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
    EventLoop* main_reactor_{nullptr}; // not own
    Acceptor* acceptor_{nullptr};
    ThreadPool* thread_pool_{nullptr};
    std::map<int, Connection*> connections_;  // 所有和客户端的连接
    std::vector<EventLoop*> sub_reactors_;
};
