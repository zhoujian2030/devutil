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
#include "TcpServerCallback.h"
#include "TcpData.h"

using namespace net;
using namespace cm;

// ------------------------------------------------
TcpConnection::TcpConnection(
    TcpSocket* theNewSocket,
    unsigned int connectionId,
    TcpServerWorker* theServerWorker,
    TcpServerCallback* theServerCallback)
: m_tcpSocket(theNewSocket),
  m_connectionId(connectionId),
  m_tcpServerWorker(theServerWorker),
  m_tcpServerCallback(theServerCallback),
  m_recvBuffer(new DataBuffer()),
  m_recvState(READY_TO_RECV),
  m_sendData(0)
{
    NetLogger::initConsoleLog();
}

// ------------------------------------------------
TcpConnection::~TcpConnection() {
    delete m_tcpSocket;
    delete m_recvBuffer;
    if (m_sendData != 0) {
        delete m_sendData;
        m_sendData = 0;
    }
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
void TcpConnection::onDataReceived(int numOfBytesRecved) {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "TcpConnection::onDataReceive, fd: " << 
        m_tcpSocket->getSocket() << ", connection id: 0x" << std::hex << m_connectionId);
    
    char* startPointer = m_recvBuffer->getEndOfDataPointer();
    m_recvBuffer->increaseDataLength(numOfBytesRecved);
        
    // call user interface to handle the data.
    // when user is handling the receive data in this connection,
    // the reactor will not receive any new data on this socket as 
    // the EPOLLIN event is re-registered after the user handle 
    // data complete
    if (m_tcpServerCallback != 0) {
        m_tcpServerCallback->dataIndication(this->getGlobalConnectionId(), startPointer, numOfBytesRecved);
    }
    // clear the receive buffer for new receiving data
    m_recvBuffer->reset();
    
    // continue to receive new data
    m_recvState = READY_TO_RECV;
    recvDataFromSocket();
}

// ------------------------------------------------
void TcpConnection::sendDataToSocket(TcpData* theTcpData) {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "TcpConnection::sendDataToSocket(), fd: " << 
        m_tcpSocket->getSocket() << ", connection id: 0x" << std::hex << m_connectionId);

    int numOfBytesToSend;
    const char* buffer = theTcpData->getData(numOfBytesToSend);
    m_tcpSocket->send(buffer, numOfBytesToSend);

    // free the memory first to avoid memory leak
    if (m_sendData != 0) {
        delete m_sendData;
    }
    m_sendData = theTcpData;
}

// ------------------------------------------------
void TcpConnection::onSendResult(int numOfByteSent) {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "TcpConnection::onSendResult, fd: " << 
        m_tcpSocket->getSocket() << ", connection id: 0x" << std::hex << m_connectionId);    

    if (numOfByteSent == m_sendData->getLength()) {
        LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "Send TCP data complete.");
        m_tcpServerCallback->deliveryResult(this->getGlobalConnectionId(), true);
    } else {
        // TODO
        // currently, the output handler notifies listener only if all data sent complete
        // so is should not come here
        assert(false);
    }
}

// ------------------------------------------------
void TcpConnection::onConnectionClosed() {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "TcpConnection::onConnectionClosed, fd: " << 
        m_tcpSocket->getSocket() << ", connection id: ox" << std::hex << m_connectionId);
    
    // close indication to user layer
    if (m_tcpServerCallback != 0) {
        m_tcpServerCallback->closeIndication(this->getGlobalConnectionId());
    }

    close();
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
    return (m_tcpServerWorker->getWorkerId() << 24) | (m_connectionId & 0x00ffffff);
}



