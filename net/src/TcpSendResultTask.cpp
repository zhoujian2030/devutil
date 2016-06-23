/*
 * TcpSendResultTask.cpp
 *
 *  Created on: June 17, 2016
 *      Author: z.j
 */

#include "TcpSendResultTask.h"
#include "TcpServerWorker.h"
#include "TcpSocket.h"
#include "NetLogger.h"

using namespace net;
using namespace cm;

// -----------------------------------------------
TcpSendResultTask::TcpSendResultTask(TcpServerWorker* theWorker, TcpSocket* theTcpSocket, int numOfBytesSent)
: m_tcpServerWorker(theWorker), m_tcpSocket(theTcpSocket), m_numOfBytesSent(numOfBytesSent)
{
    NetLogger::initConsoleLog();
}

// -----------------------------------------------
TcpSendResultTask::~TcpSendResultTask() {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "The task is executed complete and destroyed");
}

// -----------------------------------------------
int TcpSendResultTask::execute() {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "TcpSendResultTask::execute()");
    
    m_tcpServerWorker->onSendResult(m_tcpSocket, m_numOfBytesSent);
        
    return TRC_CONTINUE;
}
