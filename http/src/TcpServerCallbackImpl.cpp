/*
 * TcpServerCallbackImpl.cpp
 *
 *  Created on: July 8, 2016
 *      Author: z.j
 */

#include "TcpServerCallbackImpl.h"
#include "HttpServerUserInterface.h"
#include "HttpRequest.h"
#include "HttpLogger.h"
#include "HttpParserException.h"

#include <stdlib.h>

using namespace http;
using namespace net;
using namespace std;

// -------------------------------------------
TcpServerCallbackImpl::TcpServerCallbackImpl(HttpServerUserInterface* httpUser)
: m_httpUser(httpUser) 
{
    HttpLogger::initConsoleLog();
}

// -------------------------------------------
TcpServerCallbackImpl::~TcpServerCallbackImpl() {
    
}

// -------------------------------------------
void TcpServerCallbackImpl::deliveryResult(unsigned int globalConnId, bool status) {

}
        
// -------------------------------------------
void TcpServerCallbackImpl::dataIndication(unsigned int globalConnId, char* buffer, int numOfBytes) {
    LOG4CPLUS_DEBUG(_HTTP_LOOGER_NAME_, "TcpServerCallbackImpl::dataIndication(), connection id: ox" << std::hex << globalConnId);
    
    HttpRequest* httpRequest = new HttpRequest(globalConnId);
    char* parsedBuff = buffer;
    char* endBuff = buffer + numOfBytes;

    try {
        // parse request line
        while (parsedBuff < endBuff) {
            if(m_httpRequestLineParser.parse(*parsedBuff++)) {
                httpRequest->setMethod(m_httpRequestLineParser.getMethod());
                httpRequest->setUrl(m_httpRequestLineParser.getUrl());
                httpRequest->setVersion(m_httpRequestLineParser.getVersion());
                break;
            }
        }

        // parse http header
        m_httpHeaderParser.init(httpRequest);
        while (parsedBuff < endBuff) {
            if (m_httpHeaderParser.parse(*parsedBuff++)) {
                // parse http body 
                string contentLength = httpRequest->getHeader("Content-Length");
                if (!contentLength.empty()) {
                    int length = atoi(contentLength.c_str());

                    // TODO check if the body is received complete or there is more data of next request
                    httpRequest->setBodyStr(parsedBuff, length);
                }
                break;
            }
        }
    } catch (HttpParserException& exception) {
        LOG4CPLUS_ERROR(_HTTP_LOOGER_NAME_, "Parse error.");
        httpRequest->setParseError();
    }
    
    m_httpUser->requestIndication(httpRequest);
}
        
// -------------------------------------------
void TcpServerCallbackImpl::closeIndication(unsigned int globalConnId) {
    LOG4CPLUS_DEBUG(_HTTP_LOOGER_NAME_, "TcpServerCallbackImpl::closeIndication(), connection id: ox" << std::hex << globalConnId);

    //TODO
}
