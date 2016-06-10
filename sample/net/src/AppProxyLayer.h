/*
 * AppProxyLayer.h
 *
 *  Created on: June 7, 2016
 *      Author: z.j
 */

#ifndef APP_PROXY_LAYER_H
#define APP_PROXY_LAYER_H

#include "TcpServerCallback.h"
#include "TcpServerInterface.h"

namespace sample {

    class AppProxyLayer : public net::TcpServerCallback {
    public:
        AppProxyLayer();
        virtual ~AppProxyLayer();
        
        virtual void dataIndication(unsigned int globalConnId, char* buffer, int numOfBytes);
        
        virtual void closeIndication(unsigned int globalConnId);

        void init(net::TcpServerInterface* theTcpServer);

    private:
        net::TcpServerInterface* m_tcpServer;
    };
}

#endif