/*
 * TcpServerWorker.h
 *
 *  Created on: May 25, 2016
 *      Author: z.j
 */
 
#ifndef TCP_SERVER_WORKER_H
#define TCP_SERVER_WORKER_H
 
namespace net {
    
    class TcpSocket;
     
    // A TcpServerWorker is only responsible and used for one Worker thread  
    // to handle new TCP connection. So the application should create the 
    // same number of TcpServerWorker instance as Worker instance
    class TcpServerWorker {
    public:
        TcpServerWorker();
        virtual ~TcpServerWorker();
           
        // @Description called by TcpAcceptTask to handle new 
        //      accepted connection
        // @return theNewSocket created by reactor for the new 
        //      accepted connection 
        virtual void onConnectionCreated(TcpSocket* theNewSocket);  
    };
     
}
 
 #endif