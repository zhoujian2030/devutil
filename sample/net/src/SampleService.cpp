/*
 * SampleService.cpp
 *
 *  Created on: June 7, 2016
 *      Author: z.j
 */

#include "SampleService.h"
#include "AppProxyLayer.h"
#include "NetLogger.h"
#include "TcpServer.h"

using namespace sample;
using namespace net;

// ------------------------------------------------
SampleService::SampleService(std::string serviceName) 
: Service(serviceName)
{
    NetLogger::initConsoleLog();
    init();
}

// ------------------------------------------------
SampleService::~SampleService() {

}

// ------------------------------------------------
unsigned long SampleService::run() {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "Start initializing the TCP server");

    // TODO read custome configuration from config file for below initialization

    AppProxyLayer proxyLayer;
    TcpServer tcpServer(&proxyLayer, 12345);
    proxyLayer.init(&tcpServer);
    tcpServer.listen();

    m_stopEvent.wait();
}