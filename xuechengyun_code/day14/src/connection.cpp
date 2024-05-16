#include "connection.h"

#include <cassert>
#include <cerrno>
#include <format>
#include <iostream>
#include <string.h>
#include <unistd.h>

#include "util.h"

static const uint32_t READ_BUFFER = 1024;

Connection::Connection(EventLoop* loop, Socket* client_sock):
        loop_(loop), sock_(client_sock) {
    channel_ = new Channel(loop_, sock_->GetFd());
    channel_->EnableReading();
    read_buffer_ = new Buffer();
    send_buffer_ = new Buffer();
    state_ = State::Connected;
}

Connection::~Connection() {
    delete channel_;
    delete sock_;
    delete read_buffer_;
    delete send_buffer_;
}

void Connection::Read() {
    ASSERT(State::Connected == state_, "connection state is disconnected!");
    read_buffer_->Clear();
    if (sock_->IsNonBlocking()) {
        ReadNonBlocking();
    } else {
        ReadBlocking();
    }
}

void Connection::Write() {
    ASSERT(State::Connected == state_, "connection state is disconnected!");
    if (sock_->IsNonBlocking()) {
        WriteNonBlocking();
    } else {
        WriteBlocking();
    }
    send_buffer_->Clear();
}

void Connection::ReadNonBlocking() {
    char buf[READ_BUFFER];
    while (true) {
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(Fd(), buf, sizeof(buf));
        if (bytes_read > 0) {
            read_buffer_->Append(buf, bytes_read);
        } else if (-1 == bytes_read) {
            if (EINTR == errno) {
                // 程序正常中断
                std::cout << "continue reading\n";
            } else if (EAGAIN == errno || EWOULDBLOCK == errno) {
                std::cout << read_buffer_->ToStr() << std::endl;
                // 全部读完
                break;
            }
        } else if (0 == bytes_read) {
            // 客户端连接中断
            CoutFormat("EOF, client fd {} disconnected\n", Fd());
            state_ = State::Closed;
            break;
        } else {
            CoutFormat("Other error on client fd {}\n", Fd());
            state_ = State::Closed;
            break;
        }
    }
}

void Connection::WriteNonBlocking() {
    int sockfd = Fd();
    int data_size = send_buffer_->Size();
    char buf[data_size];
    memcpy(buf, send_buffer_->ToStr(), data_size);
    int finished_size{0};
    while (finished_size < data_size) {
        ssize_t bytes_write = write(Fd(), buf + finished_size, data_size - finished_size);
        if (-1 != bytes_write) {
            finished_size += bytes_write;
            continue;
        }
        if (EINTR == errno) {
            std::cout << "continue writing\n";
            continue;
        } else if (EAGAIN == errno) {
            break;
        } else {
            CoutFormat("Other error on client fd {}\n", Fd());
            state_ = State::Closed;
            break;
        }
    }
}

// just for client, because server doesn't block
void Connection::ReadBlocking() {
    unsigned int rcv_size{0};
    socklen_t len{sizeof(rcv_size)};
    getsockopt(Fd(), SOL_SOCKET, SO_RCVBUF, &rcv_size, &len);
    char buf[rcv_size];
    ssize_t bytes_read = read(Fd(), buf, sizeof(buf));
    if (bytes_read > 0) {
        read_buffer_->Append(buf, bytes_read);
    } else if (0 == bytes_read) {
        CoutFormat("read EOF, blocking client fd {} disconnected\n", Fd());
        state_ = State::Closed;
    } else if (-1 == bytes_read) {
        CoutFormat("Other error on blocking client fd {}\n", Fd());
        state_ = State::Closed;
    }
}

// just for client, because server doesn't block
void Connection::WriteBlocking() {
    ssize_t bytes_write = write(Fd(), send_buffer_->ToStr(), send_buffer_->Size());
    if (-1 == bytes_write) {
        CoutFormat("Other error on blocking client fd {}", Fd());
    }
}

void Connection::Close() {
    delete_connection_callback_(sock_);
}

Connection::State Connection::GetState() { return state_; }
void Connection::SetSendBuffer(const char* str) { send_buffer_->SetBuf(str); }
Buffer* Connection::GetReadBuffer() { return read_buffer_; }
const char* Connection::ReadBuffer() { return read_buffer_->ToStr(); }
Buffer* Connection::GetSendBuffer() { return send_buffer_; }
const char* Connection::SendBuffer() { return read_buffer_->ToStr(); }

void Connection::SetDeleteConnectionCallback(Socket::Callback cb) {
    delete_connection_callback_ = cb;
}
void Connection::SetOnConnectCallback(Connection::Callback cb) {
    on_connection_callback_ = cb;
    channel_->SetReadCallback([this]() { on_connection_callback_(this); });
}

void Connection::GetLineSendBuffer() { send_buffer_->Getline(); }
Socket* Connection::GetSocket() { return sock_; }

