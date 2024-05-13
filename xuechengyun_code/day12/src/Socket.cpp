#include "Socket.h"

#include <fcntl.h>
#include <new>
#include <unistd.h>

#include <sys/socket.h>

#include "util.h"

Socket::Socket(): Socket(socket(AF_INET, SOCK_STREAM, 0)) {
}

// Socket::Socket(): fd(-1) {
//     // placement new
//     new (this) Socket(socket(AF_INET, SOCK_STREAM, 0));
// }

// Socket::Socket(): fd(-1) {
//     fd = socket(AF_INET, SOCK_STREAM, 0);
//     exit_if(-1 == fd, "socket creat error");
// }

Socket::Socket(int _fd) : fd(_fd) {
    exit_if(-1 == fd, "socket create error");
}

Socket::~Socket() {
    if (fd != -1) {
        close(fd);
        fd = -1;
    }
}

void Socket::bind(InetAddress* addr) {
    exit_if(-1 == ::bind(fd, (sockaddr*)&addr->addr, addr->addr_len), "socket bind error");
}

void Socket::listen() {
    exit_if(-1 == ::listen(fd, SOMAXCONN), "socket listen error");
}

void Socket::setnonblocking() {
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int Socket::accept(InetAddress* addr) {
    int client_sockfd = ::accept(fd, (sockaddr*)&addr->addr, &addr->addr_len);
    exit_if(-1 == client_sockfd, "socket accept error");
    return client_sockfd;
}

int Socket::get_fd() {
    return fd;
}
