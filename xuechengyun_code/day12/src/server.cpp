#include "server.h"

#include <format>
#include <functional>
#include <iostream>

#include <string.h>

#include "Socket.h"
#include "inet_address.h"
#include "channel.h"

Server::Server(EventLoop* loop): main_reactor_(loop) {
    acceptor_ = new Acceptor(main_reactor_);
    Socket::Callback cb = std::bind(
        &Server::new_connection, this,
        std::placeholders::_1);
    acceptor_->set_new_connection_callback(cb);

    thread_pool_ = new ThreadPool();
    for (int i = 0; i < thread_pool_->size(); ++i) {
        sub_reactors_.emplace_back(new EventLoop());
        thread_pool_->add([l=sub_reactors_[i]]() {
            l->loop();
        });
    }
}

Server::~Server() {
    delete acceptor_;
    for (auto sub_reactor : sub_reactors_) {
        delete sub_reactor;
    }
    delete thread_pool_;
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

    Connection* connection = new Connection(
        sub_reactors_[client_sockfd % sub_reactors_.size()], client_sockfd);
    Socket::Callback cb = std::bind(&Server::delete_connection, this, std::placeholders::_1);
    connection->set_delete_connection_callback(cb);
    connections_[client_sockfd] = connection;
}

void Server::delete_connection(Socket* sock) {
    if (auto it = connections_.find(sock->get_fd()); it != connections_.end()) {
        connections_.erase(it);
        delete it->second;
    }
}
