//
// Created by 薛成韵 on 2024/5/5.
//

#include <cstring>
#include <format>
#include <iostream>
#include <unistd.h>

#include <arpa/inet.h>

void exit_if(bool condition, const char* errmsg) {
    if (condition) {
        perror(errmsg);
        exit(EXIT_FAILURE);
    }
}

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
    int sockfd = socket(AF_INET, SOCK_STREAM/*流格式*/, 0/*协议，0是自动推导*/);

    // 服务器地址
    sockaddr_in serv_addr;
    socklen_t serv_addr_len = sizeof(serv_addr);
    bzero(&serv_addr, serv_addr_len);  // 初始化
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    exit_if(-1 == connect(sockfd, (sockaddr*)&serv_addr, serv_addr_len), "socket connect error");

    write_data_to_server(sockfd);

    return 0;
}
