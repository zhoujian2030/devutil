/*
 * TcpServerWorker.h
 *
 *  Created on: May 25, 2016
 *      Author: z.j
 */
 
#ifndef TCP_SERVER_WORKER_H
#define TCP_SERVER_WORKER_H

#include "TcpSocketListener.h"
#include "Worker.h" 

namespace net {
    
    class TcpSocket;
    class DataBuffer;
     
    // A TcpServerWorker is only responsible and used for one Worker thread  
    // to handle new TCP connection. So the application should create the 
    // same number of TcpServerWorker instance as Worker instance. And it
    // is also the TcpSocket listener which implements the listener interfaces
    // that called by reactor thread when there is data received on the socket
    class TcpServerWorker : public TcpSocketListener {
    public:
        TcpServerWorker(cm::Worker* theWorker);
        virtual ~TcpServerWorker();
           
        // @Description - called by TcpAcceptTask to handle new accepted connection
        //      will always called by the same worker thread 
        // @return theNewSocket created by reactor for the new 
        //      accepted connection 
        virtual void onConnectionCreated(TcpSocket* theNewSocket);  
        
        // @description - called by TcpSocket to handle new data received on the socket
        //      could be called by different reactor thread for different socket, need
        //      to be carefull for thread conflict
        // @param theSocket - the tcp socket that has data received
        // @param numOfBytesRecved - number bytes of data received
        virtual void handleRecvResult(TcpSocket* theSocket, int numOfBytesRecved);
        
    private:
        cm::Worker* m_workerInstance;
        // for test
        DataBuffer* recvBuf;
    };
     
}
 
 #endif