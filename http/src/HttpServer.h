/*
 * HttpServer.h
 *
 *  Created on: July 7, 2016
 *      Author: z.j
 */
 
#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "TcpServer.h"
#include "HttpServerInterface.h"

namespace http {

    class HttpServerCallback;
    class HttpServerUserInterface;

    class HttpServer : public HttpServerInterface {
    public:
        HttpServer(
            HttpServerUserInterface* httpUser,
            unsigned short localPort, 
            std::string localIp = "0.0.0.0", 
            int backlog = 10000);

        ~HttpServer();

        void listen();

        virtual void sendResponse(HttpResponse* theResponse);

    private:
        net::TcpServer* m_tcpServer;

    };

}


#endif

