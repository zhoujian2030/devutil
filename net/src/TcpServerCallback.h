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

        // @description - this method is called when data is received on a socket
        //      all data in the buffer must be copied out by the receiver as the buffer
        //      will be cleared in lower layer after return
        // @param globalConnId - the global connection id used to identify a unique TCP connection
        // @param buffer - buffer pointer of the received data
        // @param numOfBytes - data length in the buffer
        virtual void dataIndication(unsigned int globalConnId, char* buffer, int numOfBytes) = 0;
        
        virtual void closeIndication(unsigned int globalConnId) = 0;
    };
    
}

#endif