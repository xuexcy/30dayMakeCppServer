#pragma once

#include "acceptor.h"
#include "connection.h"
#include "thread_pool.h"

#include <map>

class EventLoop;
class Socket;

class Server {
public:
    explicit Server(EventLoop*);
    ~Server();
    // for user
    void OnConnect(std::function<void(Connection*)>);

public: // for inner
    void NewConnection(Socket* client_sock);  // 新加TCP连接
    void DeleteConnection(Socket* client_sock);  // 断开TCP连接
private:
    EventLoop* main_reactor_{nullptr}; // not own
    Acceptor* acceptor_{nullptr};
    ThreadPool* thread_pool_{nullptr};
    std::vector<EventLoop*> sub_reactors_;

    std::map<int/*client_fd*/, Connection*> connections_;  // 所有和客户端的连接
    std::function<void(Connection*)> on_connect_callback_;
};
