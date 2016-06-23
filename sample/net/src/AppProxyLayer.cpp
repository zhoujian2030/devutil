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
void AppProxyLayer::deliveryResult(unsigned int globalConnId, bool status) {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "CONNECTION (0x" << std::hex << globalConnId << ") deliver status: " << status);    
} 

// --------------------------------------------------
void AppProxyLayer::dataIndication(unsigned int globalConnId, char* buffer, int numOfBytes) {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "CONNECTION (0x" << std::hex << globalConnId << ") receive " << numOfBytes << " bytes data: " 
        << buffer);

    // TODO parse the data and add a new task to handle it async

    // Test
    std::string respStr;
    // respStr.append(buffer, numOfBytes);
    for (int i=0; i<169160  ; ++i) {
        respStr.append("!");
    }
    respStr.append("end");

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