/*
 * AppProxyLayer.cpp
 *
 *  Created on: June 7, 2016
 *      Author: z.j
 */

#include "AppProxyLayer.h"
#include "NetLogger.h"
#include "TcpResponse.h"

using namespace sample;
using namespace net;


// --------------------------------------------------
AppProxyLayer::AppProxyLayer() {
    NetLogger::initConsoleLog();
}

// --------------------------------------------------
AppProxyLayer::~AppProxyLayer() {

}

// --------------------------------------------------
void AppProxyLayer::dataIndication(unsigned int globalConnId, char* buffer, int numOfBytes) {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "CONNECTION (0x" << std::hex << globalConnId << ") receive " << numOfBytes << " bytes data: " 
        << buffer);

    // TODO parse the data and add a new task to handle it async

    // Test
    std::string respStr;
    respStr.append(buffer, numOfBytes);
    TcpResponse* tcpResponse = new TcpResponse(globalConnId);
    tcpResponse->setData(respStr);
    m_tcpServer->sendData(tcpResponse);
}

// --------------------------------------------------
void AppProxyLayer::closeIndication(unsigned int globalConnId) {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "CONNECTION (0x" << std::hex << globalConnId << ") closed");
}

// --------------------------------------------------
void AppProxyLayer::init(TcpServerInterface* theTcpServer) {
    m_tcpServer = theTcpServer;
}