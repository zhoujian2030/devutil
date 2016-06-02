/*
 * TcpConnection.h
 *
 *  Created on: May 31, 2016
 *      Author: z.j
 */
 
#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include "DataBuffer.h"
 
namespace net {
    
    class TcpSocket;
    class TcpServerWorker;
    
    // m_recvBuffer is used to stored the data received on the socket,
    // it is written by reactor thread, but processed by worker thread,
    // so we must make sure reactor thread won't written data until worker
    // thread calls onDataReceive() to process the data
    class TcpConnection {
    public:
        TcpConnection(
            TcpSocket* theNewSocket,
            unsigned int connectionId,
            TcpServerWorker* theServerWorker);
        virtual ~TcpConnection();
        
        // @description - start to recv data from socket
        //      async : register the socket to epoll and return true
        //      sync : block on recving data, return false if error
        bool recvDataFromSocket();
        
        void onDataReceive(int numOfBytesRecved);
        
        // @description - close connection
        void close();
                
        // @description - get connection id
        // @return the unique connection id created within a TcpServerWorker
        unsigned int getConnectionId() const;
        
        // @description - get global connection id
        // @return the global unique connection id created within a TcpServer
        unsigned int getGlobalConnectionId() const;
        
    private:
        friend class TcpServerWorker;
        // @description - set connection id
        // @param connectionId - the new generated connection id
        void setConnectionId(unsigned int connectionId);
        
        typedef enum {
            READY_TO_RECV,
            RECVING
        }TcpRecvState;
        
        TcpSocket* m_tcpSocket;
        unsigned int m_connectionId;
        TcpServerWorker* m_tcpServerWorker;
        cm::DataBuffer* m_recvBuffer;
        
        TcpRecvState m_recvState;
    };
    
    // -------------------------------------------------
    inline void TcpConnection::setConnectionId(unsigned int connectionId) {
        this->m_connectionId = connectionId;
    }
     
    // -------------------------------------------------
    inline unsigned int TcpConnection::getConnectionId() const {
        return m_connectionId;
    }
    

}
 
#endif