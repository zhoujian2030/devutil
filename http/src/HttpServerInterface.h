/*
 * HttpServerInterface.h
 *
 *  Created on: July 7, 2016
 *      Author: z.j
 */
 
#ifndef HTTP_SERVER_INTERFACE_H
#define HTTP_SERVER_INTERFACE_H

namespace http {

    class HttpResponse;

    class HttpServerInterface {
    public:
        virtual void sendResponse(HttpResponse* theResponse) = 0;
    };
}

#endif