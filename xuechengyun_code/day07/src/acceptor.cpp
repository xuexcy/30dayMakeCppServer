#include "acceptor.h"

#include "channel.h"
#include "inet_address.h"
#include "Socket.h"

Acceptor::Acceptor(EventLoop* loop): loop_(loop) {
   sock_ = new Socket();
   addr_ = new InetAddress("127.0.0.1", 8888);
   sock_->bind(addr_);
   sock_->listen();
   sock_->setnonblocking();

   accept_channel_ = new Channel(loop, sock_->get_fd());
   Callback cb = std::bind(&Acceptor::accept_connection, this);
   accept_channel_->set_callback(cb);
   accept_channel_->enable_reading();
}

Acceptor::~Acceptor() {
   delete sock_;
   delete addr_;
   delete accept_channel_;
}

void Acceptor::accept_connection() {
   new_connection_callback_(sock_);
}

void Acceptor::set_new_connection_callback(AcceptorCallback cb) {
   new_connection_callback_ = cb;
}
