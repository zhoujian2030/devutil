/*
 * HttpRequest.cpp
 *
 *  Created on: July 7, 2016
 *      Author: z.j
 */

#include "HttpRequest.h"
#include "HttpLogger.h"

using namespace http;

// ---------------------------------------
HttpRequest::HttpRequest(unsigned long connId)
: m_connId(connId), m_isParseError(false)
{
    HttpLogger::initConsoleLog();
}

// ---------------------------------------
HttpRequest::HttpRequest(std::string method, std::string url) 
: m_connId(0), m_isParseError(false), m_method(method), m_url(url)
{
    HttpLogger::initConsoleLog();
}

// ----------------------------------------
void HttpRequest::setHeader(std::string name, std::string value) {
    LOG4CPLUS_DEBUG(_HTTP_LOOGER_NAME_, "HttpRequest::setHeader, name = " << name << ", value = " << value);
    m_headerMap[name] = value;
}

// ----------------------------------------
std::string HttpRequest::getHeader(std::string name) {
    HeaderMap::iterator it = m_headerMap.find(name);
    if (it != m_headerMap.end()) {
        return it->second;
    } else {
        return "";
    }
}

// ---------------------------------------
const HttpRequest::HeaderMap & HttpRequest::getHeaders() const {
    return m_headerMap;
}

// ----------------------------------------
void HttpRequest::show() {
    LOG4CPLUS_DEBUG(_HTTP_LOOGER_NAME_, "method: " << m_method);
    LOG4CPLUS_DEBUG(_HTTP_LOOGER_NAME_, "url: " << m_url);
    LOG4CPLUS_DEBUG(_HTTP_LOOGER_NAME_, "version: " << m_version);
    for (HeaderMap::iterator it = m_headerMap.begin(); it != m_headerMap.end(); ++it) {
        LOG4CPLUS_DEBUG(_HTTP_LOOGER_NAME_, it->first << ": " << it->second);
    }
}

// ----------------------------------------
void HttpRequest::setData(char* buff, int numOfBytes) {
    buff[numOfBytes] = '\0';
    m_data.clear();
    m_data.append(buff);
}
