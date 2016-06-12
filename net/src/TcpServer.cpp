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
#include "TcpServerWorker.h"
#include "TcpServerCallback.h"
#include "TcpDataSendTask.h"
#include <stdexcept>

using namespace net;
using namespace std;
using namespace cm;

// -------------------------------------------
TcpServer::TcpServer(
    TcpServerCallback* theServerCallback,
    unsigned short localPort,
    string localIp,
    int backlog)
: m_numberOfWorkers(Worker::getNumberOfWorkers()),
  m_tcpServerCallback(theServerCallback)
{
    NetLogger::initConsoleLog();
    // create the TCP server socket and register itself as the socket listener
    m_tcpServerSocket = new TcpServerSocket(localIp, localPort, this, backlog);
    
    // create the same number of TCP server workers as the Worker thread number
    m_tcpServerWorkerArray = new TcpServerWorker*[m_numberOfWorkers];
    for (int i=0; i<m_numberOfWorkers; ++i) {
        m_tcpServerWorkerArray[i] = new TcpServerWorker(Worker::getInstance(i), m_tcpServerCallback);
    }
}

// -------------------------------------------
TcpServer::~TcpServer() {
    delete m_tcpServerSocket;
    for (int i=0; i<m_numberOfWorkers; ++i) {
        delete m_tcpServerWorkerArray[i];
    }
    delete m_tcpServerWorkerArray;
}

// -------------------------------------------
void TcpServer::start() {
    m_isRunning = true;
    m_tcpServerSocket->accept();
}

// -------------------------------------------
void TcpServer::handleAcceptResult(TcpServerSocket* serverSocket, TcpSocket* newSocket) {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "TcpServer::handleAcceptResult, server socket: " << 
        serverSocket->getSocket() << ", accepted socket: " << newSocket->getSocket());

    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "index = " << newSocket->getHashValue());
    Worker* worker = Worker::getInstance(newSocket->getHashValue());
    TcpAcceptTask* task = new TcpAcceptTask(newSocket, m_tcpServerWorkerArray[worker->getIndex()]);
    worker->addTask(task);
}

void TcpServer::handleCloseResult(TcpServerSocket* serverSocket) {
    // TODO
}

// -------------------------------------------
void TcpServer::sendData(TcpData* theTcpData) {
    if (theTcpData == 0) {
        throw std::invalid_argument("null pointer");
    }
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "TcpServer::sendData(), global connection id = " <<
        std::hex << theTcpData->getGlobalConnId());
    
    int workerIndex = theTcpData->getGlobalConnId() >> 24;
    if (workerIndex >= m_numberOfWorkers) {
        delete theTcpData;
        throw std::invalid_argument("Inavlid global connection id");
    }

    Worker* worker = Worker::getInstance(workerIndex);
    TcpDataSendTask* task = new TcpDataSendTask(m_tcpServerWorkerArray[workerIndex], theTcpData);
    worker->addTask(task);
}