//
// Created by 薛成韵 on 2024/5/5.
//

#include <cstring>
#include <format>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>

void exit_if(bool condition, const char* errmsg) {
    if (condition) {
        perror(errmsg);
        exit(EXIT_FAILURE);
    }
}

int/*socket_fd*/ get_server_fd() {
    int sockfd = socket(AF_INET, SOCK_STREAM/*流格式*/, 0/*协议，0是自动推导*/);
    exit_if(sockfd == -1, "socket create error");

    // 服务器地址
    sockaddr_in serv_addr;
    socklen_t serv_addr_len = sizeof(serv_addr);
    bzero(&serv_addr, serv_addr_len);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    // 将地址配置和socket绑定
    exit_if( -1 == bind(sockfd, (sockaddr*)&serv_addr, serv_addr_len), "socket bind error");
    // 监听socket端口
    exit_if( -1 == listen(sockfd, SOMAXCONN/*最大监听队列长度，128*/), "socket listen error");
    return sockfd;
}

int/*socket_fd*/ get_client_fd(int fd) {
    // 初始化客户端地址
    sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    bzero(&client_addr, client_addr_len);

    // 接受客户端连接，并保存客户端地址
    // 阻塞程序直到有客户端socket被接受
    // 需要写入client_addr的长度，所以是 &client_addr_len
    int client_sockfd = accept(fd, (sockaddr*)&client_addr, &client_addr_len);
    exit_if(-1 == client_sockfd, "socket accept error");
    std::cout << std::format(
        "new client fd {}! ip:port: {}:{}",
        client_sockfd,
        inet_ntoa(client_addr.sin_addr),
        ntohs(client_addr.sin_port)) << std::endl;
    return client_sockfd;
}

void read_data_from_client(int client_fd) {
    while (true) {
        char buf[1024];
        bzero(&buf, sizeof(buf));
        // read data to buf
        ssize_t bytes = read(client_fd, buf, sizeof(buf));
        if (bytes > 0) {
            std::cout << std::format("message from client fd {}: {}\n", client_fd, buf);
            // return same data to client
            write(client_fd, buf, sizeof(buf));
        } else if (bytes == 0) {
            std::cout << std::format("client fd {} disconnected\n", client_fd);
            close(client_fd);
            break;
        } else if (bytes == -1) {
            close(client_fd);
            exit_if(true, "socket read error");
        }
    }
}

int main() {
    auto server_fd = get_server_fd();
    auto client_fd = get_client_fd(server_fd);
    read_data_from_client(client_fd);
    return 0;
}
