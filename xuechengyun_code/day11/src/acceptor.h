#include <functional>

#include "event_loop.h"
#include "Socket.h"

class Acceptor {
public:
    Acceptor(EventLoop* loop);
    ~Acceptor();

    void accept_connection();
    void set_new_connection_callback(Socket::Callback);
private:
    Socket::Callback new_connection_callback_;
    EventLoop* loop_;
    Socket* sock_;
    InetAddress* addr_;
    Channel* accept_channel_;
};
