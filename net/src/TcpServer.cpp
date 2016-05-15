/*
 * TcpServer.cpp
 *
 *  Created on: May 12, 2016
 *      Author: z.j
 */

#include "TcpServer.h"
#include "TcpServerSocket.h"
#include "TcpSocket.h"
#include "NetLogger.h"

using namespace net;
using namespace std;

// -------------------------------------------
TcpServer::TcpServer(
    string localIp,
    unsigned short localPort,
    int backlog) 
{
    NetLogger::initConsoleLog();
    m_TcpServerSocket = new TcpServerSocket(localIp, localPort, this, backlog);
}

// -------------------------------------------
TcpServer::~TcpServer() {
    delete m_TcpServerSocket;
}

// -------------------------------------------
void TcpServer::start() {
    m_isRunning = true;
    m_TcpServerSocket->accept();
}

// -------------------------------------------
void TcpServer::handleAcceptResult(TcpServerSocket* serverSocket, TcpSocket* newSocket) {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "TcpServer::handleAcceptResult, server socket: " << 
        serverSocket->getSocket() << ", accepted socket: " << newSocket->getSocket());

    // TODO handle the new TcpSocket, receives data from the socket (need to register to epoll)
}

void TcpServer::handleCloseResult(TcpServerSocket* serverSocket) {
    // TODO
}