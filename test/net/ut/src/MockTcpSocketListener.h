/*
 * MockTcpSocketListener.h
 *
 *  Created on: June 13, 2016
 *      Author: z.j
 */

#ifndef MOCK_TCP_SOCKET_LISTENER_H
#define MOCK_TCP_SOCKET_LISTENER_H

#include "TcpSocketListener.h"
#include "TcpSocket.h"

class MockTcpSocketListener : public net::TcpSocketListener {
public:
    MockTcpSocketListener();
    virtual void handleRecvResult(net::TcpSocket* theSocket, int numOfBytesRecved);
    virtual void handleSendResult(net::TcpSocket* theSocket, int numOfBytesSent);
    virtual void handleCloseResult(net::TcpSocket* theSocket);
    virtual void handleErrorResult(net::TcpSocket* theSocket);

    int m_recvDataLength;
    int m_closeCount;
};

#endif
