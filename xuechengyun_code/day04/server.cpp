
#include <format>
#include <iostream>

#include <string.h>
#include <unistd.h>

#include "epoll.h"
#include "inet_address.h"
#include "Socket.h"

void handle_read_event(int client_fd);

int main() {
    Socket server_sock;
    InetAddress server_addr("127.0.0.1", 8888);
    server_sock.bind(&server_addr);
    server_sock.listen();
    server_sock.setnonblocking();

    Epoll ep;
    ep.add_fd(server_sock.get_fd(), EPOLLIN | EPOLLET);

    while (true) {
        for (auto& event : ep.poll()) {
            if (event.data.fd == server_sock.get_fd()) {
                InetAddress client_addr;
                Socket client_sock(server_sock.accept(&client_addr));
                std::cout << std::format(
                    "new client fd {}! IP: {} Port: {}\n",
                    client_sock.get_fd(),
                    inet_ntoa(client_addr.addr.sin_addr),
                    ntohs(client_addr.addr.sin_port));
                client_sock.setnonblocking();
                ep.add_fd(client_sock.get_fd(), EPOLLIN | EPOLLET);
            } else if (event.events & EPOLLIN) {
                handle_read_event(event.data.fd);
            } else {
                std::cout << "something else happened\n";
            }
        }
    }
    close(server_sock.get_fd());
    return 0;
}

void handle_read_event(int client_fd) {
    char buf[1024];
    while (true) {
        bzero(&buf, sizeof(buf));
        // read data to buf
        ssize_t bytes = read(client_fd, buf, sizeof(buf));
        if (bytes > 0) {
            std::cout << std::format("message from client fd {}: {}\n", client_fd, buf);
            // return same data to client
            write(client_fd, buf, sizeof(buf));
        } else if (bytes == 0) {
            std::cout << std::format("EOF client fd {} disconnected\n", client_fd);
            close(client_fd);
            break;
        } else if (bytes == -1) {
            if (errno == EINTR) {
                std::cout << "continue reading\n";
                continue;
            } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
                std::cout << std::format("finish reading once, errno: {}\n", errno);
                break;
            }
        }
    }
}
