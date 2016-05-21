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
#include "Worker.h"
#include "TcpAcceptTask.h"

using namespace net;
using namespace std;
using namespace cm;

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

    Worker* worker = Worker::getInstance(newSocket->getRemoteAddress());
    TcpAcceptTask* task = new TcpAcceptTask(newSocket);
    worker->addTask(task);
}

void TcpServer::handleCloseResult(TcpServerSocket* serverSocket) {
    // TODO
}