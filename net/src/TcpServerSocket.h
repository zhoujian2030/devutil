/*
 * TcpServerSocket.h
 *
 *  Created on: May 06, 2016
 *      Author: z.j
 */

#ifndef TCP_SERVER_SOCKET_H
#define TCP_SERVER_SOCKET_H

#include "Socket.h"
#include "TcpServerSocketListener.h"
#include "TcpSocket.h"
#include "MutexLock.h"
#include "Reactor.h"

namespace net {

    // TODO only asynchronous mode is implemented currently

    // For asynchronous mode, there must be a socket listener registered to 
    // handle the new accepted socket connection. Normally the server initializer
    // call accept() only register this server socket to epoll and then returns.
    //
    // For synchronous mode, the thread who call accept() will be blocked and
    // must handle the new accepted socket by itself. No epoll is used in this case
    class TcpServerSocket : public Socket {
    public:

        // synchronous mode
        TcpServerSocket(
            std::string localIp, 
            unsigned short localPort,
            int backlog = 100);

        // asynchronous mode
        TcpServerSocket(
            std::string localIp, 
            unsigned short localPort,
            TcpServerSocketListener* socketListener,
            int backlog = 100);

        virtual ~TcpServerSocket();

        // register the socket listener and change socket property if needed
        // @param socketListener
        //    0 - if m_socketListener is not null, change socket to blocking mode
        //    not null - if m_socketListener is 0, change socket to nonblocking mode
        void addSocketListener(TcpServerSocketListener* socketListener);

        // accept a new TCP connection
        // synchronous mode:
        // the calling thread will be blocked until a new TCP socket connection is
        // accepted and return the new created TcpSocket.
        // asynchronous mode:
        // the calling thread registers the server socket and its input event handler
        // to epoll, when there is new connection request coming in, the reactor 
        // thread calls the event handler's handleInput() to accept the connection
        TcpSocket* accept();

        // close server socket
        void close();

    protected:
        virtual void handleInput(Socket* theSocket);

    private:
        typedef enum {
            TCP_LISTENING,
            TCP_ACCEPTING,
            TCP_CLOSING,
            TCP_CLOSED,
            TCP_ERROR
        } TcpServerState;

        Reactor* m_reactorInstance;
        int m_backlog;
        TcpServerState m_tcpState; 
        TcpServerSocketListener* m_socketListener;

        // currently m_tcpState would be changed when accept() or handleInput() is called,
        // if they could be called by different threads, an mutex is needed. But so far,
        // after the intializer calls accept() to start, only the reactor thread will call
        // handleInput() and accept(), so no mutex is needed yet.
        cm::Lock* m_lock;

    };
}

#endif