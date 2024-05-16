#include "server.h"

#include <format>
#include <functional>
#include <iostream>

#include <string.h>

#include "channel.h"
#include "inet_address.h"
#include "Socket.h"
#include "util.h"

Server::Server(EventLoop* loop): main_reactor_(loop) {
    acceptor_ = new Acceptor(main_reactor_);
    Socket::Callback cb = std::bind(
        &Server::NewConnection, this,
        std::placeholders::_1);
    acceptor_->SetNewConnectionCallback(cb);

    thread_pool_ = new ThreadPool();
    for (int i = 0; i < thread_pool_->size(); ++i) {
        sub_reactors_.emplace_back(new EventLoop());
        thread_pool_->Add([l=sub_reactors_[i]]() {
            l->Loop();
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

void Server::NewConnection(Socket* client_sock) {
    exit_if(-1 == client_sock->GetFd(), "new connection error");
    auto idx = client_sock->GetFd() % sub_reactors_.size();
    Connection* conn = new Connection(sub_reactors_[idx], client_sock);
    Socket::Callback cb = std::bind(&Server::DeleteConnection, this, std::placeholders::_1);
    conn->SetDeleteConnectionCallback(cb);
    conn->SetOnConnectCallback(on_connect_callback_);
    connections_[client_sock->GetFd()] = conn;
}

void Server::DeleteConnection(Socket* client_sock) {
    if (auto it = connections_.find(client_sock->GetFd()); it != connections_.end()) {
        auto conn = it->second;
        connections_.erase(it);
        delete conn;
    }
}

void Server::OnConnect(Connection::Callback cb) {
    on_connect_callback_ = std::move(cb);
}
