#include "MockTcpServer.h"
#include "Worker.h"
#include "TcpAcceptTask.h"
#include "Reactor.h"
#include "MockTcpServerWorker.h"
#include <iostream>

MockTcpServer::MockTcpServer(
    net::TcpServerCallback* theServerCallback,
    unsigned short localPort, 
    std::string localIp, 
    int backlog) {
    net::Reactor::initialize(1);
    m_tcpServerSocket = new net::TcpServerSocket(localIp, localPort, backlog);
    cm::Worker::initialize(1);
    m_tcpServerWorker = new MockTcpServerWorker(cm::Worker::getInstance(0), theServerCallback);
    //m_tcpServerSocket->accept();
}

MockTcpServer::~MockTcpServer() {
    delete m_tcpServerSocket;
    delete m_tcpServerWorker;
}

void MockTcpServer::addSocketListener(net::TcpServerSocketListener* theSocketListener) {
    m_tcpServerSocket->addSocketListener(theSocketListener);
}

net::TcpSocket* MockTcpServer::accept() {
    return m_tcpServerSocket->accept();
}

void MockTcpServer::handleAcceptResult(net::TcpServerSocket* serverSocket, net::TcpSocket* newSocket) {
    std::cout << "Accept a new TCP conection!" << std::endl;
    cm::Worker* worker = cm::Worker::getInstance(0); 
    net::TcpAcceptTask* task = new net::TcpAcceptTask(newSocket, m_tcpServerWorker);
    worker->addTask(task);  
    m_newTcpSocket = newSocket;
}

void MockTcpServer::handleCloseResult(net::TcpServerSocket* serverSocket) {
    
}
