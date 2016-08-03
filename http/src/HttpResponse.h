/*
 * HttpResponse.h
 *
 *  Created on: July 7, 2016
 *      Author: z.j
 */

#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include "TcpData.h"

namespace http {

    class HttpResponse : public net::TcpData {
    public:
        HttpResponse(unsigned long connId);
        virtual ~HttpResponse();
    };

}

#endif
