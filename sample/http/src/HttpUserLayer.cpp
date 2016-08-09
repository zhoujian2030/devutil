/*
 * HttpUserLayer.cpp
 *
 *  Created on: Aug 03, 2016
 *      Author: z.j
 */

#include "HttpUserLayer.h"
#include "HttpLogger.h"

#include <string>

using namespace service;
using namespace http;
using namespace std;

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

    const string body = httpRequest->getBodyStr();
    LOG4CPLUS_DEBUG(_HTTP_LOOGER_NAME_, "body content (" << body.size() << ") bytes: " << body);
    // TODO
}

// ------------------------------------------------
void HttpUserLayer::closeIndication(unsigned int connId) {
    LOG4CPLUS_DEBUG(_HTTP_LOOGER_NAME_, "HttpUserLayer::closeIndication(), CONNECTION (0x" << std::hex << connId << ")");
}