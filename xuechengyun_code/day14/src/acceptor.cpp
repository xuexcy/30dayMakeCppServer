#include "acceptor.h"

#include "channel.h"
#include "inet_address.h"
#include "Socket.h"
#include "util.h"

Acceptor::Acceptor(EventLoop* loop): loop_(loop) {
   sock_ = new Socket();
   InetAddress addr("127.0.0.1", 8888);
   sock_->Bind(&addr);
   sock_->Listen();
   sock_->SetNonBlocking();

   accept_channel_ = new Channel(loop, sock_->GetFd());
   Channel::Callback cb = std::bind(&Acceptor::AcceptConnection, this);
   accept_channel_->SetReadCallback(cb);
   accept_channel_->EnableReading();
}

Acceptor::~Acceptor() {
   delete sock_;
   delete accept_channel_;
}

void Acceptor::AcceptConnection() {
   // 1. get client_sock
   InetAddress client_addr;
   auto client_sock = new Socket(sock_->Accept(&client_addr));
   CoutFormat(
       "new client fd {}! IP: {} Port: {}\n",
       client_sock->GetFd(),
       inet_ntoa(client_addr.addr.sin_addr),
      ntohs(client_addr.addr.sin_port));
   client_sock->SetNonBlocking();
   // 2. run [server new connection] callback
   new_connection_callback_(client_sock);
}

void Acceptor::SetNewConnectionCallback(Socket::Callback cb) {
   new_connection_callback_ = cb;
}
