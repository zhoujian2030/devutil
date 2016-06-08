/*
 * AppProxyLayer.cpp
 *
 *  Created on: June 7, 2016
 *      Author: z.j
 */

#include "AppProxyLayer.h"
#include "NetLogger.h"

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
void AppProxyLayer::dataIndication(unsigned int connId, char* buffer, int numOfBytes) {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "CONNECTION (0x" << std::hex << connId << ") receive " << numOfBytes << " bytes data: " 
        << buffer);
}

// --------------------------------------------------
void AppProxyLayer::closeIndication(unsigned int connId) {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "CONNECTION (0x" << std::hex << connId << ") closed");
}