/*
 * TcpDataSendTask.cpp
 *
 *  Created on: June 10, 2016
 *      Author: z.j
 */

#include "TcpDataSendTask.h"
#include "TcpServerWorker.h"
#include "TcpData.h"
#include "NetLogger.h"

using namespace net;
using namespace cm;

// -----------------------------------------
TcpDataSendTask::TcpDataSendTask(TcpServerWorker* theWorker, TcpData* theTcpData) 
: m_tcpServerWorker(theWorker), m_tcpData(theTcpData)
{
    NetLogger::initConsoleLog();
}

// -----------------------------------------
TcpDataSendTask::~TcpDataSendTask() {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "The task is executed complete and destroyed");
}

// -----------------------------------------
int TcpDataSendTask::execute() {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "TcpDataSendTask::execute()");
    
    m_tcpServerWorker->sendData(m_tcpData);
        
    return TRC_CONTINUE;
}
