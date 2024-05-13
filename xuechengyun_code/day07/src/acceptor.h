#include <functional>

#include "event_loop.h"
#include "Socket.h"

using AcceptorCallback = std::function<void(Socket*)>;

class Acceptor {
public:
    Acceptor(EventLoop* loop);
    ~Acceptor();

    void accept_connection();
    void set_new_connection_callback(AcceptorCallback);
private:
    AcceptorCallback new_connection_callback_;
    EventLoop* loop_;
    Socket* sock_;
    InetAddress* addr_;
    Channel* accept_channel_;
};
