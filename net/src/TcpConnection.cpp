/*
 * TcpConnection.cpp
 *
 *  Created on: June 02, 2016
 *      Author: z.j
 */
#include "TcpConnection.h"
#include "TcpSocket.h"
#include "TcpServerWorker.h"
#include "NetLogger.h"

using namespace net;
using namespace cm;

// ------------------------------------------------
TcpConnection::TcpConnection(
    TcpSocket* theNewSocket,
    unsigned int connectionId,
    TcpServerWorker* theServerWorker)
: m_tcpSocket(theNewSocket),
  m_connectionId(connectionId),
  m_tcpServerWorker(theServerWorker),
  m_recvBuffer(new DataBuffer()),
  m_recvState(READY_TO_RECV)
{
    NetLogger::initConsoleLog();
    
    // use asynchronous mode to recv socket data
    m_tcpSocket->addSocketListener(theServerWorker);
}

// ------------------------------------------------
TcpConnection::~TcpConnection() {
    delete m_tcpSocket;
    delete m_recvBuffer;
}

// ------------------------------------------------
bool TcpConnection::recvDataFromSocket() {
    if (m_recvState == READY_TO_RECV) {
        // async mode, only will register the socket to epoll
        // the socket data will be writen to m_recvBuffer by reactor thread when 
        // there is data coming
        int length = m_tcpSocket->receive(m_recvBuffer->getEndOfDataPointer(), m_recvBuffer->getRemainBufferSize());
        
        // for sync mode, if return -1, close the socket
        if (length < 0) {
            close();
            return false;
        }
        
        m_recvState = RECVING;
    }
    
    return true;
}

// ------------------------------------------------
void TcpConnection::onDataReceive(int numOfBytesRecved) {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "TcpConnection::onDataReceive, fd: " << 
        m_tcpSocket->getSocket() << ", connection id: " << m_connectionId);
    
    if (numOfBytesRecved == 0) {
        close();
        // TODO how to nodify TcpServerWorker remove the connection
    } else {
        m_recvBuffer->increaseDataLength(numOfBytesRecved);
        LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_,"receive " << numOfBytesRecved << " data: " <<
            m_recvBuffer->getData());
        // TODO process the data
        
        m_recvState = READY_TO_RECV;
        recvDataFromSocket();
    }
}


// ------------------------------------------------
void TcpConnection::close() {
    m_tcpSocket->close();
}

// ------------------------------------------------
unsigned int TcpConnection::getGlobalConnectionId() const {
    // aaaaaaaa bbbbbbbb bbbbbbbb bbbbbbbb
    // a - worker id
    // b - sub connection id
    return (m_tcpServerWorker->getWorkerId() << 24) & m_connectionId;
}



