/*
 * TcpSocketListener.h
 *
 *  Created on: May 23, 2016
 *      Author: z.j
 */

#ifndef TCP_SOCKET_LISTENER_H
#define TCP_SOCKET_LISTENER_H

namespace net {
    
    class TcpSocket;
    
    class TcpSocketListener {
    public:
        virtual void handleRecvResult(TcpSocket* theSocket, int numOfBytesRecved) = 0;
    };
}

#endif