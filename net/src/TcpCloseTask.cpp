/*
 * TcpCloseTask.cp
 *
 *  Created on: July 4, 2016
 *      Author: z.j
 */

#include "TcpCloseTask.h"
#include "NetLogger.h"
#include "TcpServerWorker.h" 

using namespace net;
using namespace cm;

// ----------------------------------------
TcpCloseTask::TcpCloseTask(TcpServerWorker* theWorker, TcpSocket* theSocket) 
: m_tcpSocket(theSocket), m_tcpServerWorker(theWorker)
{
    NetLogger::initConsoleLog();
}

// ----------------------------------------
TcpCloseTask::~TcpCloseTask() {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "The task is executed complete and destroyed");
}

// -----------------------------------------
// --------------------------------
int TcpCloseTask::execute() {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "TcpCloseTask::execute()");
    
    m_tcpServerWorker->onConnectionClosed(m_tcpSocket);
        
    return TRC_CONTINUE;
}
