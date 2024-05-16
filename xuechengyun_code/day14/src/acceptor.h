#include <functional>

#include "event_loop.h"
#include "Socket.h"

class Acceptor {
public:
    Acceptor(EventLoop* loop);
    ~Acceptor();

    void AcceptConnection();
    void SetNewConnectionCallback(Socket::Callback);
private:
    Socket::Callback new_connection_callback_;
    EventLoop* loop_;
    Socket* sock_;
    Channel* accept_channel_;
};
