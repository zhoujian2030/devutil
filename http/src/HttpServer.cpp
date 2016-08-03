/*
 * HttpServer.cpp
 *
 *  Created on: July 7, 2016
 *      Author: z.j
 */

#include "HttpServer.h"
#include "HttpServerCallback.h"
#include "HttpResponse.h"

using namespace http;
using namespace net;

// ----------------------------------------------
HttpServer::HttpServer( 
    HttpServerUserInterface* httpUser,
    unsigned short localPort, 
    std::string localIp, 
    int backlog)
{
    HttpServerCallback* serverCallback = new HttpServerCallback(httpUser);
    m_tcpServer = new TcpServer(serverCallback, localPort, localIp, backlog);
}

// ----------------------------------------------
HttpServer::~HttpServer() {
    delete m_tcpServer;
}

// ----------------------------------------------
void HttpServer::listen() {
    m_tcpServer->listen();
}

// ----------------------------------------------
void HttpServer::sendResponse(HttpResponse* theResponse) {
    m_tcpServer->sendData(theResponse);
}