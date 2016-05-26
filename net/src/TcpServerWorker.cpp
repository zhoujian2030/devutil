/*
 * TcpServerWorker.cpp
 *
 *  Created on: May 25, 2016
 *      Author: z.j
 */
 
#include "TcpServerWorker.h"
#include "NetLogger.h"
#include "TcpSocket.h"
#include "Worker.h"
#include "DataBuffer.h"

using namespace net;
using namespace cm;

// -------------------------------------------
TcpServerWorker::TcpServerWorker(Worker* theWorker) 
: m_workerInstance(theWorker)
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
    
    theNewSocket->addSocketListener(this);
    
    // TODO create the TCP connection
    
    recvBuf = new DataBuffer(); 
    theNewSocket->receive(recvBuf->getEndOfDataPointer(), recvBuf->getSize() - recvBuf->getLength());
}

// -------------------------------------------
void TcpServerWorker::handleRecvResult(TcpSocket* theSocket, int numOfBytesRecved) {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "TcpServerWorker::handleRecvResult, new fd: " << theSocket->getSocket());
    
    // TODO add a new task to handle received data in worker thread
    
    // for test
    if (numOfBytesRecved == 0) {
        theSocket->close();        
    } else {
        recvBuf->increaseDataLength(numOfBytesRecved);
        LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "receive " << numOfBytesRecved << " bytes data from socket: " << recvBuf->getData());
        LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "buffer length: " << recvBuf->getLength());
        recvBuf->reset();
    }
}