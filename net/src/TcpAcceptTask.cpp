/*
 * TcpAcceptTask.cpp
 *
 *  Created on: May 21, 2016
 *      Author: z.j
 */

#include "TcpAcceptTask.h"
#include "NetLogger.h"

using namespace net;
using namespace cm;

// --------------------------------
TcpAcceptTask::TcpAcceptTask(TcpSocket* theSocket) 
: m_tcpSocket(theSocket)
{
    NetLogger::initConsoleLog();
}

// --------------------------------
TcpAcceptTask::~TcpAcceptTask() {
    //
}

// --------------------------------
int TcpAcceptTask::execute() {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "Handling new TCP socket: " << m_tcpSocket->getSocket());
    
    // TODO handle the new TcpSocket, receives data from the socket (need to register to epoll)
    
    return TRC_CONTINUE;
}

