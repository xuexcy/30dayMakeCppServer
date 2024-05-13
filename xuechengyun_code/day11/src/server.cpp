#include "server.h"

#include <format>
#include <functional>
#include <iostream>

#include <string.h>

#include "Socket.h"
#include "inet_address.h"
#include "channel.h"

Server::Server(EventLoop* loop): loop_(loop) {
    acceptor_ = new Acceptor(loop);
    Socket::Callback cb = std::bind(
        &Server::new_connection, this,
        std::placeholders::_1);
    acceptor_->set_new_connection_callback(cb);
}

Server::~Server() {
    delete acceptor_;
    for (auto [_, connection] : connections_) {
        delete connection;
    }
}

void Server::new_connection(Socket* server_sock) {
    InetAddress client_addr;
    auto client_sockfd = server_sock->accept(&client_addr);
    std::cout << std::format(
        "new client fd {}! IP: {} Port: {}\n",
        client_sockfd,
        inet_ntoa(client_addr.addr.sin_addr),
        ntohs(client_addr.addr.sin_port));

    Connection* connection = new Connection(loop_, client_sockfd);
    Socket::Callback cb = std::bind(&Server::delete_connection, this, std::placeholders::_1);
    connection->set_delete_connection_callback(cb);
    connections_[client_sockfd] = connection;
}

void Server::delete_connection(Socket* sock) {
    auto it = connections_.find(sock->get_fd());
    connections_.erase(it);
    delete it->second;
}
