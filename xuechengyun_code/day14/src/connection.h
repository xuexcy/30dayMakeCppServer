#pragma once

#include "buffer.h"
#include "channel.h"
#include "event_loop.h"
#include "Socket.h"

class Connection {
public:
    enum State {
        Invalid = 1,
        Handshaking,
        Connected,
        Closed,
        Failed,
    };
    using Callback = std::function<void(Connection*)>;
    Connection(EventLoop* loop, Socket* client_sock);
    ~Connection();

    void SetOnConnectCallback(Callback);
    void SetDeleteConnectionCallback(Socket::Callback);

    void Read();
    void Write();
    State GetState();
    void Close();
    void SetSendBuffer(const char* str);
    Buffer* GetReadBuffer();
    const char* ReadBuffer();
    Buffer* GetSendBuffer();
    const char* SendBuffer();
    void GetLineSendBuffer();
    Socket* GetSocket();

    void OnConnect(Channel::Callback);
private:
    auto Fd() { return sock_->GetFd(); }
    void ReadNonBlocking();
    void ReadBlocking();
    void WriteNonBlocking();
    void WriteBlocking();

private:
    EventLoop* loop_;
    Socket* sock_;
    Channel* channel_;
    Callback on_connection_callback_;
    Socket::Callback delete_connection_callback_;
    Buffer* read_buffer_;
    Buffer* send_buffer_;
    State state_{State::Invalid};
};
