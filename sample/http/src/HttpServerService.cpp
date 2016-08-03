/*
 * HttpServerService.cpp
 *
 *  Created on: Aug 03, 2016
 *      Author: z.j
 */

#include "HttpServerService.h"
#include "HttpLogger.h"
#include "HttpServer.h"
#include "HttpUserLayer.h"

using namespace http;
using namespace service;

// ------------------------------------------------
HttpServerService::HttpServerService(std::string serviceName) 
: Service(serviceName)
{
    HttpLogger::initConsoleLog();
    init();
}

// ------------------------------------------------
HttpServerService::~HttpServerService() {

}

// ------------------------------------------------
unsigned long HttpServerService::run() {
    LOG4CPLUS_DEBUG(_HTTP_LOOGER_NAME_, "Start initializing the HTTP server");

    // TODO read custome configuration from config file for below initialization

    HttpUserLayer userLayer;
    HttpServer httpServer(&userLayer, 12345);
    userLayer.init(&httpServer);
    httpServer.listen();

    m_stopEvent.wait();

    return 0;
}