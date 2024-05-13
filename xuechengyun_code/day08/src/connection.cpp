#include "connection.h"

#include <cerrno>
#include <format>
#include <iostream>
#include <string.h>
#include <unistd.h>

static const uint32_t READ_BUFFER = 1024;

Connection::Connection(EventLoop* loop, int client_sockfd):
        loop_(loop) {
    sock_ = new Socket(client_sockfd);
    sock_->setnonblocking();

    channel_ = new Channel(loop, client_sockfd);
    Channel::Callback cb = std::bind(&Connection::echo, this);
    channel_->set_callback(cb);
    channel_->enable_reading();
}

Connection::~Connection() {
    delete channel_;
    delete sock_;
}
void Connection::echo() {
    auto sockfd = sock_->get_fd();
    char buf[READ_BUFFER];
    while (true) {
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if (bytes_read > 0) {
            std::cout << std::format("message from client fd {}: {}\n", sockfd, buf);
            write(sockfd, buf, sizeof(buf));
        } else if (-1 == bytes_read) {
            if (EINTR == errno) {
                std::cout << "continue reading\n";
            } else if (EAGAIN == errno || EWOULDBLOCK == errno) {
                std::cout << std::format("finish reading once, errno: {}\n", errno);
                break;
            }
        } else if (0 == bytes_read) {
            std::cout << std::format("EOF, client fd {} disconnected\n", sockfd);
            // close(sockfd);   //关闭socket会自动将文件描述符从epoll树上移除
            delete_connection_callback_(sock_);
            break;
        }
    }
}

void Connection::set_delete_connection_callback(Socket::Callback cb) {
    delete_connection_callback_ = cb;
}

