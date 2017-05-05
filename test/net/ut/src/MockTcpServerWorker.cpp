#include "MockTcpServerWorker.h"
#include <iostream>

MockTcpServerWorker::MockTcpServerWorker(cm::Worker* theWorker, net::TcpServerCallback* theServerCallback) 
: TcpServerWorker(theWorker, theServerCallback),m_tcpConn(0),m_connId(0)
{
    
}

MockTcpServerWorker::~MockTcpServerWorker() {
    
}

void MockTcpServerWorker::onConnectionCreated(net::TcpSocket* theNewSocket) {
    m_connId++;
    m_tcpConn = new net::TcpConnection(theNewSocket, m_connId, this, m_tcpServerCallback);
    theNewSocket->setUserData((void*)m_connId);
    
    m_newTcpSocket = theNewSocket;
}

void MockTcpServerWorker::handleRecvResult(net::TcpSocket* theSocket, int numOfBytesRecved) {
    std::cout << "Receive " << numOfBytesRecved << " bytes data." << std::endl;
    m_socketUserData = (size_t)theSocket->getUserData();
    m_numOfBytesRecved = numOfBytesRecved;
}

void MockTcpServerWorker::addSocketListener(net::TcpSocketListener* theSocketListener) {
    std::cout << "Add listener to the TCP socket" << std::endl;
    m_newTcpSocket->addSocketListener(theSocketListener);
}
