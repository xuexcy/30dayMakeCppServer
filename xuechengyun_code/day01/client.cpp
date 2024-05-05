//
// Created by 薛成韵 on 2024/4/29.
//

#include <cstring>
#include <iostream>

#include <arpa/inet.h>

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

    connect(sockfd, (sockaddr*)&serv_addr, serv_addr_len);
    return 0;
}