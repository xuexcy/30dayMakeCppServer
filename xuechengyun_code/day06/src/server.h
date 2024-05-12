#pragma once

class EventLoop;
class Socket;

class Server {
public:
    Server(EventLoop*);
    ~Server() = default;

    void handle_read_event(int fd);
    void new_connection(Socket* server_sock);
private:
    EventLoop* loop_;
};
