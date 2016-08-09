/*
 * TcpServerCallbackImpl.h
 *
 *  Created on: July 7, 2016
 *      Author: z.j
 */

#ifndef HTTP_SERVER_CALLBACK_H
#define HTTP_SERVER_CALLBACK_H

#include "TcpServerCallback.h"
#include "HttpHeaderParser.h"
#include "HttpRequestLineParser.h"

namespace http {

    class HttpServerUserInterface;
    class HttpHeaderParser;
    class HttpRequestLineParser;

    class TcpServerCallbackImpl : public net::TcpServerCallback {
    public:
        TcpServerCallbackImpl(HttpServerUserInterface* httpUser);
        virtual ~TcpServerCallbackImpl();

        virtual void deliveryResult(unsigned int globalConnId, bool status);
        
        virtual void dataIndication(unsigned int globalConnId, char* buffer, int numOfBytes);
        
        virtual void closeIndication(unsigned int globalConnId);

    private:

        HttpServerUserInterface* m_httpUser;
        HttpRequestLineParser m_httpRequestLineParser;
        HttpHeaderParser m_httpHeaderParser;
    };
}

#endif