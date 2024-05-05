//
// Created by 薛成韵 on 2024/4/29.
//

#include <cstring>
#include <format>
#include <iostream>

#include <arpa/inet.h>
#include <sys/socket.h>

int/*socket_fd*/ get_server_fd() {
    int sockfd = socket(AF_INET, SOCK_STREAM/*流格式*/, 0/*协议，0是自动推导*/);

    // 服务器地址
    sockaddr_in serv_addr;
    socklen_t serv_addr_len = sizeof(serv_addr);
    bzero(&serv_addr, serv_addr_len);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    // 将地址配置和socket绑定
    bind(sockfd, (sockaddr*)&serv_addr, serv_addr_len);
    // 监听socket端口
    listen(sockfd, SOMAXCONN/*最大监听队列长度，128*/);
    return sockfd;
}

void accept_from_client(int fd) {
    // 初始化客户端地址
    sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    bzero(&client_addr, client_addr_len);

    // 接受客户端连接，并保存客户端地址
    // 阻塞程序直到有客户端socket被接受
    // 需要写入client_addr的长度，所以是 &client_addr_len
    int client_sockfd = accept(fd, (sockaddr*)&client_addr, &client_addr_len);
    std::cout << std::format(
        "new client fd {}! ip:port: {}:{}",
        client_sockfd,
        inet_ntoa(client_addr.sin_addr),
        ntohs(client_addr.sin_port)) << std::endl;
}

int main() {
    auto fd = get_server_fd();
    accept_from_client(fd);
    return 0;
}
