/*
 * TcpSocket.h
 *
 *  Created on: May 09, 2016
 *      Author: z.j
 */

#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

#include "Socket.h"
#include "TcpSocketListener.h"
#include "Reactor.h"

namespace net {

    // This TcpSocket object can be created by TCP server as a new accepted TCP socket, 
    // or created as a TCP client socket in TCP client (need to call connect() after created)
    class TcpSocket : public Socket {
    public:
        virtual ~TcpSocket();

        // add a TcpSocketListener to the TCP socket in acync mode
        // need to change the socket to nonblocking if the listener 
        // is 0 before
        void addSocketListener(TcpSocketListener* socketListener);
        
        // @return a hash value generated according to remote ip and port
        unsigned int getHashValue() const;
        
        const sockaddr_in& getRemoteAddress() const;
        
        // @description called by worker to receive data from socket
        //  asynchronize mode - save the buffer pointer and register the socket
        //      to epoll (need to have the socket lister added before)
        //  synchronize mode - not supported yet TODO
        // @param theBuffer - the buffer pointer to store received data
        // @param buffSize - buffer size of the buffer
        // @return number of bytes received in sync mode, 0 in async mode
        //      return -1 if error occurrs
        int receive(char* theBuffer, int buffSize);
        
        // @description - close the connection
        void close();

    protected:
        // Only TcpServerSocket is allowed to create a TcpSocket with an
        // created(connected) socket fd.
        friend class TcpServerSocket;

        TcpSocket(int socket, Socket::InetAddressPort& theRemoteAddrPort);

        virtual void handleInput(Socket* theSocket);

    private: 
        typedef enum {
            // for new created socket in server side, initialize state 
            // is TCP_CONNECTED
            TCP_CONNECTED,
            
            // register the socket to epoll for EPOLLIN event, waiting
            // for data coming
            TCP_RECEIVING,
            
            TCP_CLOSING,
            TCP_CLOSED,
            TCP_ERROR
        } TcpConnectState;
        
        TcpConnectState m_tcpState;
        Reactor* m_reactorInstance;
        std::string m_remoteIp;
        unsigned short m_remotePort;
        struct sockaddr_in m_remoteSa;
        char* m_recvBuffer;
        int m_recvBufferSize;
        
        TcpSocketListener* m_socketListener;
    };
    
    // --------------------------------------------
    inline unsigned int TcpSocket::getHashValue() const {
        return m_remoteSa.sin_addr.s_addr + m_remotePort;
    }

    // --------------------------------------------
    inline const sockaddr_in& TcpSocket::getRemoteAddress() const {
        return m_remoteSa;
    }
}

#endif