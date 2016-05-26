/*
 * TcpAcceptTask.cpp
 *
 *  Created on: May 21, 2016
 *      Author: z.j
 */

#include "TcpAcceptTask.h"
#include "NetLogger.h"
#include "TcpServerWorker.h"

using namespace net;
using namespace cm;

// --------------------------------
TcpAcceptTask::TcpAcceptTask(TcpSocket* theSocket, TcpServerWorker* theWorker) 
: m_tcpSocket(theSocket), m_tcpServerWorker(theWorker)
{
    NetLogger::initConsoleLog();
}

// --------------------------------
TcpAcceptTask::~TcpAcceptTask() {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "The task is executed complete and destroyed");
}

// --------------------------------
int TcpAcceptTask::execute() {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "TcpAcceptTask::execute()");
    
    m_tcpServerWorker->onConnectionCreated(m_tcpSocket);
        
    return TRC_CONTINUE;
}

