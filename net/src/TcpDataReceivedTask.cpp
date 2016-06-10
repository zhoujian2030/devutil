/*
 * TcpDataReceivedTask.h
 *
 *  Created on: June 08, 2016
 *      Author: z.j
 */

#include "TcpDataReceivedTask.h"
#include "NetLogger.h"
#include "TcpServerWorker.h"

using namespace net;
using namespace cm;

// --------------------------------
TcpDataReceivedTask::TcpDataReceivedTask(
    TcpSocket* theSocket, 
    TcpServerWorker* theWorker,
    int numOfBytesReceived) 
: m_tcpSocket(theSocket), 
  m_tcpServerWorker(theWorker),
  m_numOfBytesReceived(numOfBytesReceived)
{
    NetLogger::initConsoleLog();
}

// --------------------------------
TcpDataReceivedTask::~TcpDataReceivedTask() {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "The task is executed complete and destroyed");
}

// --------------------------------
int TcpDataReceivedTask::execute() {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "TcpDataReceivedTask::execute()");
    
    m_tcpServerWorker->onDataReceived(m_tcpSocket, m_numOfBytesReceived);
        
    return TRC_CONTINUE;
}

