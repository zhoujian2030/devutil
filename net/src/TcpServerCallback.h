/*
 * TcpServerCallback.h
 *
 *  Created on: June 6, 2016
 *      Author: z.j
 */
 
#ifndef TCP_SERVER_CALLBACK_H
#define TCP_SERVER_CALLBACK_H

namespace net {
    
    class TcpServerCallback {
    public:
    
        virtual void dataIndication(unsigned int connId, char* buffer, int numOfBytes) = 0;
        
        virtual void closeNotification(unsigned int connId) = 0;
    };
    
}

#endif