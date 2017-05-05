/*
 * MockTcpSocketListener.h
 *
 *  Created on: June 13, 2016
 *      Author: z.j
 */

#include "MockTcpSocketListener.h"
#include <iostream>

using namespace net;
using namespace std;

// -----------------------------------------------
MockTcpSocketListener::MockTcpSocketListener()
: m_recvDataLength(0), m_closeCount(0)
{

}

// -----------------------------------------------
void MockTcpSocketListener::handleRecvResult(net::TcpSocket* theSocket, int numOfBytesRecved) {
    cout << "Recv " << numOfBytesRecved << " bytes" << endl;
    m_recvDataLength = numOfBytesRecved;
}

// -----------------------------------------------
void MockTcpSocketListener::handleSendResult(net::TcpSocket* theSocket, int numOfBytesSent) {
    cout << "Send " << numOfBytesSent << " bytes" << endl;
    //TODO
}

// -----------------------------------------------
void MockTcpSocketListener::handleCloseResult(TcpSocket* theSocket) {
    cout << "MockTcpSocketListener::handleCloseResult" << endl;
    m_closeCount++;
}

// -----------------------------------------------
void MockTcpSocketListener::handleErrorResult(TcpSocket* theSocket) {
    cout << "MockTcpSocketListener::handleErrorResult" << endl;
}
