/*
 * HttpServerService.h
 *
 *  Created on: July 29, 2016
 *      Author: z.j
 */

#ifndef HTTP_SERVER_SERVICE_H
#define HTTP_SERVER_SERVICE_H

#include "Service.h"
#include "EventIndicator.h"

namespace service {

    // Initialize a http server service to support hight concurence
    // of HTTP request
    class HttpServerService : public cm::Service {
    public:
        HttpServerService(std::string serviceName);
        virtual ~HttpServerService();

    private:
        virtual unsigned long run();
        
        cm::EventIndicator m_stopEvent;
    };

}

#endif