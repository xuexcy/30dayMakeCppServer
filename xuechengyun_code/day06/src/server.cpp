#include "server.h"

#include <format>
#include <functional>
#include <iostream>

#include <string.h>
#include <unistd.h>

#include "Socket.h"
#include "inet_address.h"
#include "channel.h"

static const uint32_t READ_BUFFER = 1024;

Server::Server(EventLoop* loop): loop_(loop) {
    Socket* server_sock = new Socket();
    InetAddress* server_addr = new InetAddress("127.0.0.1", 8888);
    server_sock->bind(server_addr);
    server_sock->listen();
    server_sock->setnonblocking();

    Channel* server_channel = new Channel(loop, server_sock->get_fd());
    Callback callback = std::bind(&Server::new_connection, this, server_sock);
    server_channel->set_callback(callback);
    server_channel->enable_reading();
}

void Server::handle_read_event(int sockfd) {
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
            close(sockfd);
            break;
        }
    }
}

void Server::new_connection(Socket* server_sock) {
    InetAddress* client_addr = new InetAddress;
    Socket* client_sock = new Socket(server_sock->accept(client_addr));
    std::cout << std::format(
        "new client fd {}! IP: {} Port: {}\n",
        client_sock->get_fd(),
        inet_ntoa(client_addr->addr.sin_addr),
        ntohs(client_addr->addr.sin_port));
    client_sock->setnonblocking();

    Channel* client_channel = new Channel(loop_, client_sock->get_fd());
    Callback callback = std::bind(&Server::handle_read_event, this, client_sock->get_fd());
    client_channel->set_callback(callback);
    client_channel->enable_reading();
}
