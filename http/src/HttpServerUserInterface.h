/*
 * HttpServerUserInterface.h
 *
 *  Created on: July 7, 2016
 *      Author: z.j
 */

#ifndef HTTP_SERVER_USER_INTERFACE_H
#define HTTP_SERVER_USER_INTERFACE_H

#include "HttpRequest.h"

namespace http {

    class HttpServerInterface;

    class HttpServerUserInterface {
    public:
        void init(HttpServerInterface* httpServer);

        virtual void deliveryResult(unsigned int connId, bool status) = 0;
        
        virtual void requestIndication(HttpRequest* httpRequest) = 0;
        
        virtual void closeIndication(unsigned int connId) = 0;

    protected:
        HttpServerInterface* m_httpServer;

    };

    // ---------------------------------------
    inline void HttpServerUserInterface::init(HttpServerInterface* httpServer) {
        m_httpServer = httpServer;
    }
}

#endif
