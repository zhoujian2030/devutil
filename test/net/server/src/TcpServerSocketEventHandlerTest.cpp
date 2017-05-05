#include "Socket.h"
#include "ReactorThread.h"
#include "TcpServerSocketEventHandlerTest.h"
#include "DataBuffer.h"

#include <assert.h>
#include <iostream>
#include <string.h>

using namespace std;
using namespace net;
using namespace cm;


TcpServerSocketEventHandlerTest::TcpServerSocketEventHandlerTest(ReactorThread* reactorThread)
: m_reactorThread(reactorThread) 
{

}

void TcpServerSocketEventHandlerTest::handleInput(Socket* theSocket) {
    cout << "TcpServerSocketEventHandlerTest::handleInput()" << endl;

    Socket::InetAddressPort remoteAddrAndPort;
    int newSocketFd = -1;

    int result = theSocket->accept(newSocketFd, remoteAddrAndPort);
    assert(result != SKT_ERR);
    // as the reactor will not call event handler until it receives EPOLLIN or EPOLLOUT event
    // so we expect the accept will return success
    assert(result != SKT_WAIT);

    if (result == SKT_SUCC) {
        Socket* newSocket = new Socket(newSocketFd);
        newSocket->makeNonBlocking();
        TcpSocketEventHandlerTest* tcpSocketHandler = new TcpSocketEventHandlerTest(m_reactorThread);
        // TODO when to delete these new object to avoid memory leak?
        m_reactorThread->registerInputHandler(newSocket, tcpSocketHandler);
    }
}

TcpSocketEventHandlerTest::TcpSocketEventHandlerTest(ReactorThread* reactorThread)
: m_reactorThread(reactorThread) 
{

}

void TcpSocketEventHandlerTest::handleInput(Socket* theSocket) {
    DataBuffer* recvBuf = new DataBuffer();
    int numOfBytesRecved = 0;

    int result = theSocket->recv(recvBuf->getEndOfDataPointer(), recvBuf->getSize() - recvBuf->getLength(), numOfBytesRecved);
    assert(result != SKT_ERR);
    assert(result != SKT_WAIT);

    if (result == SKT_SUCC) {
        if (numOfBytesRecved == 0) {
            cout << "socket is disconnected by peer: " << theSocket->getSocket() << endl;
            // close socket in server side
            m_reactorThread->removeInputHandler(theSocket);
            theSocket->close();
            delete theSocket;
        } else {
            recvBuf->increaseDataLength(numOfBytesRecved);
            cout << "receive " << numOfBytesRecved << " bytes data from socket: " << recvBuf->getData() << endl;
            cout << "buffer length: " << recvBuf->getLength() << endl;

            char respData[] = "TCP server response";
            int numOfBytesSent;
            assert(SKT_SUCC == theSocket->send(respData, strlen(respData), numOfBytesSent));
            assert(numOfBytesSent == strlen(respData));
            cout << "send " << numOfBytesSent << " bytes data to socket: " << respData << endl;
        }
        recvBuf->reset();
    }
}
