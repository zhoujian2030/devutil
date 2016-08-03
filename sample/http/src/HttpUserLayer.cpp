/*
 * HttpUserLayer.cpp
 *
 *  Created on: Aug 03, 2016
 *      Author: z.j
 */

#include "HttpUserLayer.h"
#include "HttpLogger.h"

using namespace service;
using namespace http;

// ------------------------------------------------
HttpUserLayer::HttpUserLayer() {
    HttpLogger::initConsoleLog();
}

// ------------------------------------------------
HttpUserLayer::~HttpUserLayer() {

}

// ------------------------------------------------
void HttpUserLayer::deliveryResult(unsigned int connId, bool status) {
    LOG4CPLUS_DEBUG(_HTTP_LOOGER_NAME_, "HttpUserLayer::deliveryResult(), CONNECTION (0x" << std::hex << connId << ")");
    // TODO
}

// ------------------------------------------------
void HttpUserLayer::requestIndication(http::HttpRequest* httpRequest) {
    LOG4CPLUS_DEBUG(_HTTP_LOOGER_NAME_, "HttpUserLayer::requestIndication(), CONNECTION (0x" << std::hex << httpRequest->getConnId() << ")");
    LOG4CPLUS_DEBUG(_HTTP_LOOGER_NAME_, "Received HTTP request: ");
    httpRequest->show();
    // TODO
}

// ------------------------------------------------
void HttpUserLayer::closeIndication(unsigned int connId) {
    LOG4CPLUS_DEBUG(_HTTP_LOOGER_NAME_, "HttpUserLayer::closeIndication(), CONNECTION (0x" << std::hex << connId << ")");
}