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
    InetAddress* client_addr = new InetAddress;
    Socket* client_sock = new Socket(server_sock->accept(client_addr));
    std::cout << std::format(
        "new client fd {}! IP: {} Port: {}\n",
        client_sock->get_fd(),
        inet_ntoa(client_addr->addr.sin_addr),
        ntohs(client_addr->addr.sin_port));
    client_sock->setnonblocking();

    Connection* connection = new Connection(loop_, client_sock);

    Socket::Callback cb = std::bind(&Server::delete_connection, this, std::placeholders::_1);
    connection->set_delete_connection_callback(cb);
    connections_[client_sock->get_fd()] = connection;
}

void Server::delete_connection(Socket* sock) {
    auto it = connections_.find(sock->get_fd());
    connections_.erase(it);
    delete it->second;
}
