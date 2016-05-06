
#ifndef TCP_SERVER_SOCKET_EVENT_HANDLER_TEST_H
#define TCP_SERVER_SOCKET_EVENT_HANDLER_TEST_H

#include "SocketEventHandler.h"

namespace net {

    class Socket;
    class ReactorThread;

    class TcpServerSocketEventHandlerTest : public SocketEventHandler {
    public:
        TcpServerSocketEventHandlerTest(ReactorThread* reactorThread);

        virtual void handleInput(Socket* theSocket);

    private:
        ReactorThread* m_reactorThread;
    };

    class TcpSocketEventHandlerTest : public SocketEventHandler {
    public:
        TcpSocketEventHandlerTest(ReactorThread* reactorThread);
        virtual void handleInput(Socket* theSocket);

    private:
        ReactorThread* m_reactorThread;
    };

}

#endif

