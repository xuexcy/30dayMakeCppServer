//
// Created by 薛成韵 on 2024/5/9.
//

#include <cstring>
#include <format>
#include <iostream>
#include <unistd.h>

#include <arpa/inet.h>

#include "util.h"
#include "inet_address.h"
#include "Socket.h"

void write_data_to_server(int client_fd) {
    while (true) {
        char buf[1024];
        bzero(&buf, sizeof(buf));
        std::cin >> buf;
        ssize_t bytes = write(client_fd, buf, sizeof(buf));
        if (bytes == -1) {
            std::cout << std::format("socket already disconnected, can't write any more!\n");
            break;
        }
        bzero(&buf, sizeof(buf));
        bytes = read(client_fd, buf, sizeof(buf));
        if (bytes > 0) {
            std::cout << std::format("message from server: {}\n", buf);
        } else if (bytes == 0) {
            std::cout << "server socket disconnected!\n";
            break;
        } else if (bytes == -1) {
            close(client_fd);
            exit_if(true, "socket read error");
        }
    }

}

int main() {
    // 创建一个客户端socket
    Socket sock;

    // 服务器地址
    InetAddress serv_addr("127.0.0.1", 8888);


    exit_if(-1 == connect(sock.get_fd(), (sockaddr*)&serv_addr.addr, serv_addr.addr_len), "socket connect error");

    write_data_to_server(sock.get_fd());

    return 0;
}
