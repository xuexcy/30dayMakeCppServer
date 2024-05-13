//
// Created by 薛成韵 on 2024/5/9.
//

#include <cstring>
#include <format>
#include <iostream>
#include <unistd.h>

#include <arpa/inet.h>

#include "src/buffer.h"
#include "src/inet_address.h"
#include "src/Socket.h"
#include "src/util.h"

void write_data_to_server(int client_fd) {
    Buffer send_buffer;
    Buffer read_buffer;
    while (true) {
        send_buffer.getline(); // cin
        ssize_t bytes = write(client_fd, send_buffer.c_str(), send_buffer.size());
        if (bytes == -1) {
            std::cout << std::format("socket already disconnected, can't write any more!\n");
            break;
        }

        int already_read = 0;
        char buf[1024];
        while (true) {
            bzero(&buf, sizeof(buf));
            auto bytes = read(client_fd, buf, sizeof(buf));
            if (bytes > 0) {
                read_buffer.append(buf, bytes);
                already_read += bytes;
            } else if (bytes == 0) {
                std::cout << "server socket disconnected!\n";
                return;
            }
            if (already_read >= send_buffer.size()) {
                std::cout << std::format("message from server: {}\n", read_buffer.c_str());
                break;
            }
        }
        read_buffer.clear();
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
