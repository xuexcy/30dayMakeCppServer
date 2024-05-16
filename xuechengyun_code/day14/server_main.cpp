#include "src/connection.h"
#include "src/event_loop.h"
#include "src/server.h"
#include "src/util.h"

int main() {
    EventLoop loop;
    Server server(&loop);
    server.OnConnect([](Connection* conn) {
        conn->Read();
        if (Connection::State::Closed == conn->GetState()) {
            conn->Close();
            return;
        }
        CoutFormat(
            "Message from client {} : {}\n",
            conn->GetSocket()->GetFd(), conn->ReadBuffer());
        conn->SetSendBuffer(conn->ReadBuffer());
        conn->Write();
    });
    loop.Loop();
    return 0;
}
