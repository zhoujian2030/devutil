/*
 * TcpServerWorker.cpp
 *
 *  Created on: May 25, 2016
 *      Author: z.j
 */
 
#include "TcpServerWorker.h"
#include "NetLogger.h"
#include "TcpSocket.h"
#include "DataBuffer.h"

using namespace net;

// -------------------------------------------
TcpServerWorker::TcpServerWorker() 
{
    NetLogger::initConsoleLog();
}

// -------------------------------------------
TcpServerWorker::~TcpServerWorker() {
    // Empty
}

// -------------------------------------------
void TcpServerWorker::onConnectionCreated(TcpSocket* theNewSocket) {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "onConnectionCreated(), new fd: " << theNewSocket->getSocket());
    // for test, use sync mode
    DataBuffer* recvBuf = new DataBuffer(); 
    while (true) {
        int numOfBytesRecved;
        int result = theNewSocket->recv(recvBuf->getEndOfDataPointer(), 
            recvBuf->getSize() - recvBuf->getLength(), numOfBytesRecved);
        if (numOfBytesRecved == 0) {
            LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "disconnected by peer");
            delete theNewSocket;
            delete recvBuf;
            break;
        }
        
        recvBuf->increaseDataLength(numOfBytesRecved);
        LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "receive " << numOfBytesRecved << " bytes data from socket: " << recvBuf->getData());
        LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "buffer length: " << recvBuf->getLength());
        recvBuf->reset();
    }

    // TODO handle the new TcpSocket, receives data from the socket (need to register to epoll)
}