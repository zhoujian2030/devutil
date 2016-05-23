/*
 * TcpAcceptTask.cpp
 *
 *  Created on: May 21, 2016
 *      Author: z.j
 */

#include "TcpAcceptTask.h"
#include "NetLogger.h"
#include "DataBuffer.h"

using namespace net;
using namespace cm;

// --------------------------------
TcpAcceptTask::TcpAcceptTask(TcpSocket* theSocket) 
: m_tcpSocket(theSocket)
{
    NetLogger::initConsoleLog();
}

// --------------------------------
TcpAcceptTask::~TcpAcceptTask() {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "The task is executed complete and destroyed");
}

// --------------------------------
int TcpAcceptTask::execute() {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "Handling new TCP socket: " << m_tcpSocket->getSocket());
    
    // for test, use sync mode
    DataBuffer* recvBuf = new DataBuffer(); 
    while (true) {
        int numOfBytesRecved;
        int result = m_tcpSocket->recv(recvBuf->getEndOfDataPointer(), 
            recvBuf->getSize() - recvBuf->getLength(), numOfBytesRecved);
        if (numOfBytesRecved == 0) {
            LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "disconnected by peer");
            delete m_tcpSocket;
            delete recvBuf;
            break;
        }
        
        recvBuf->increaseDataLength(numOfBytesRecved);
        LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "receive " << numOfBytesRecved << " bytes data from socket: " << recvBuf->getData());
        LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "buffer length: " << recvBuf->getLength());
        recvBuf->reset();
    }

    // TODO handle the new TcpSocket, receives data from the socket (need to register to epoll)
    
    return TRC_CONTINUE;
}

