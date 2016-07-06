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
#include "TcpDataReceivedTask.h"
#include "TcpSendResultTask.h"
#include "TcpCloseTask.h"

using namespace net;
using namespace cm;
using namespace std;

// -------------------------------------------
TcpServerWorker::TcpServerWorker(Worker* theWorker, TcpServerCallback* theServerCallback) 
: m_worker(theWorker), m_connectionIdCounter(0), m_tcpServerCallback(theServerCallback)
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
    // use asynchronous mode to recv socket data
    theNewSocket->addSocketListener(this);
    createConnection(theNewSocket);
}

// -------------------------------------------
void TcpServerWorker::onDataReceived(TcpSocket* theSocket, int numOfBytesRecved) {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "TcpServerWorker::onDataReceived, " << numOfBytesRecved <<
        " bytes received,  fd: " << theSocket->getSocket());

    map<unsigned int, TcpConnection*>::iterator it = m_connMap.find((size_t)theSocket->getUserData());
    if (it != m_connMap.end()) {
        TcpConnection* tcpConn = it->second;
        
        if (numOfBytesRecved != 0) {
            tcpConn->onDataReceived(numOfBytesRecved);
        } else {
            tcpConn->onConnectionClosed();
            
            // currently the TcpSocket will close synchronously by removing 
            // the socket from EpollSocketSet and closing socket directly,
            // so here needs to release the connection resource
            // TODO in furture, consider the async close, then may change
            // to release the connection resource in async callback
            delete tcpConn;
            m_connMap.erase(it);
        }        
    }
}

// -------------------------------------------
void TcpServerWorker::onSendResult(TcpSocket* theSocket, int numOfBytesSent) {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "TcpServerWorker::onSendResult, " << numOfBytesSent <<
        " bytes received,  fd: " << theSocket->getSocket());    

    map<unsigned int, TcpConnection*>::iterator it = m_connMap.find((size_t)theSocket->getUserData());
    if (it != m_connMap.end()) {
        TcpConnection* tcpConn = it->second;
        tcpConn->onSendResult(numOfBytesSent);
    }
}

// -------------------------------------------
void TcpServerWorker::onConnectionClosed(TcpSocket* theSocket) {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "TcpServerWorker::onConnectionClosed, fd = " << theSocket->getSocket());

    map<unsigned int, TcpConnection*>::iterator it = m_connMap.find((size_t)theSocket->getUserData());
    if (it != m_connMap.end()) {
        TcpConnection* tcpConn = it->second;
        tcpConn->onConnectionClosed();
        delete tcpConn;
        m_connMap.erase(it);
    }
}

// -------------------------------------------
void TcpServerWorker::handleRecvResult(TcpSocket* theSocket, int numOfBytesRecved) {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "TcpServerWorker::handleRecvResult, " << numOfBytesRecved <<
        " bytes received,  fd: " << theSocket->getSocket());
    
    TcpDataReceivedTask* task = new TcpDataReceivedTask(theSocket, this, numOfBytesRecved);
    m_worker->addTask(task);
}

// -------------------------------------------
void TcpServerWorker::handleSendResult(TcpSocket* theSocket, int numOfBytesSent) {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "TcpServerWorker::handleSendResult, " << numOfBytesSent <<
        " bytes sent,  fd: " << theSocket->getSocket());    
    
    TcpSendResultTask* task = new TcpSendResultTask(this, theSocket, numOfBytesSent);
    m_worker->addTask(task);
}

// --------------------------------------------
void TcpServerWorker::handleCloseResult(TcpSocket* theSocket) {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "TcpServerWorker::handleCloseResult, fd = " << theSocket->getSocket());

    TcpCloseTask* task = new TcpCloseTask(this, theSocket);
    m_worker->addTask(task);
}

// --------------------------------------------
void TcpServerWorker::handleErrorResult(TcpSocket* theSocket) {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "TcpServerWorker::handleErrorResult, fd = " << theSocket->getSocket());

    // TODO
    // Test
    TcpCloseTask* task = new TcpCloseTask(this, theSocket);
    m_worker->addTask(task);
}

// -------------------------------------------
void TcpServerWorker::createConnection(TcpSocket* theNewSocket) {
    // TODO limit the max connection
    m_connectionIdCounter++;
    // use 24 bits for the sub connection id, the high 8 bits is reserved for global
    if (m_connectionIdCounter > 0xffffff) {
        m_connectionIdCounter = 0;
    }
    TcpConnection* newTcpConn = new TcpConnection(theNewSocket, m_connectionIdCounter, this, m_tcpServerCallback);
    
    std::pair<map<unsigned int, TcpConnection*>::iterator, bool> result = 
        m_connMap.insert(map<unsigned int, TcpConnection*>::value_type(m_connectionIdCounter, newTcpConn));
    
    // if the connection id is used, try 1000 times more
    if (!result.second) {
        for (int i=0; i<1000; ++i) {
            m_connectionIdCounter++;
            result = m_connMap.insert(map<unsigned int, TcpConnection*>::value_type(m_connectionIdCounter, newTcpConn));
            // if success, update the connection id and exit the loop
            if (result.second) {
                newTcpConn->setConnectionId(m_connectionIdCounter);
                break;
            }
        }
    }
    
    // if still fail to find a connection id, consider it as unrecoverable error, crash
    if (!result.second) {
        LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "Fail to allocate a connection for new connection, exit process.");
        _exit(-1);
    }
    
    // TODO start timer, disconnect if no heart beat?
    
    // set connection id in socket for later use when receiving data from the socket
    theNewSocket->setUserData((void*)m_connectionIdCounter);
    
    // start receiving data from socket. remove the connection if fail to receive
    if (!newTcpConn->recvDataFromSocket()) {
        m_connMap.erase(result.first);
        delete newTcpConn;
    }
}

// -------------------------------------------
void TcpServerWorker::sendData(TcpData* theTcpData) {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "TcpServerWorker::sendData()");

    unsigned int connId = theTcpData->getGlobalConnId() & 0x00ffffff;

    map<unsigned int, TcpConnection*>::iterator it = m_connMap.find(connId);
    if (it != m_connMap.end()) {
        it->second->sendDataToSocket(theTcpData);
    } else {
        LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "Tcp connection not exists, connection id = 0x" << std::hex << connId);
        delete theTcpData;
    }
}