#include "buffer.h"
#include "channel.h"
#include "event_loop.h"
#include "Socket.h"

class Connection {
public:
    Connection(EventLoop* loop, int client_sockfd);
    ~Connection();
    void set_delete_connection_callback(Socket::Callback);

private:
    void echo();

private:
    EventLoop* loop_;
    Socket* sock_;
    Channel* channel_;
    Socket::Callback delete_connection_callback_;
    Buffer read_buffer_;
};
