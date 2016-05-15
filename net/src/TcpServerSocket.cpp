/*
 * TcpServerSocket.cpp
 *
 *  Created on: May 08, 2016
 *      Author: z.j
 */

#include "TcpServerSocket.h"
#include "NetLogger.h"

using namespace net;
using namespace cm;

// ------------------------------------
TcpServerSocket::TcpServerSocket(
    std::string localIp, 
    unsigned short localPort,
    int backlog)
: Socket(localIp, localPort, SOCK_STREAM, 0, AF_INET),
  m_reactorInstance(Reactor::getInstance()),
  m_backlog(backlog),
  m_tcpState(TCP_LISTENING),
  m_socketListener(0)
{
    m_lock = new MutexLock(true);

    if (bind()) {
        if (!listen(m_backlog)) {
            LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "fail to listen on socket " << this->getSocket());
            m_tcpState = TCP_ERROR;
        }
    } else {
        LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "fail to bind socket " << this->getSocket());
        m_tcpState = TCP_ERROR;
    }

    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "success to bind and listen on socket " << this->getSocket());
}

// -------------------------------------------------------
TcpServerSocket::TcpServerSocket(
    std::string localIp, 
    unsigned short localPort,
    TcpServerSocketListener* socketListener,
    int backlog)
: Socket(localIp, localPort, SOCK_STREAM, 0, AF_INET),
  m_reactorInstance(Reactor::getInstance()),
  m_backlog(backlog),
  m_tcpState(TCP_LISTENING),
  m_socketListener(0)
{
    m_lock = new MutexLock(true);

    addSocketListener(socketListener);

    if (bind()) {
        if (!listen(m_backlog)) {
            LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "fail to listen on socket " << this->getSocket());
            m_tcpState = TCP_ERROR;
        }
    } else {
        LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "fail to bind socket " << this->getSocket());
        m_tcpState = TCP_ERROR;
    }

    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "success to bind and listen on socket " << this->getSocket());
}

// -------------------------------------------------------
TcpServerSocket::~TcpServerSocket() {
    delete m_lock;
}

// -------------------------------------------------------
void TcpServerSocket::addSocketListener(TcpServerSocketListener* socketListener) {
    if (socketListener != 0) {
        if (m_socketListener == 0) {
            // if previous listener is null, it means the socket is blocking mode currently
            makeNonBlocking();
        }
    } else {
        if (m_socketListener != 0) {
            makeBlocking();
        }
    }

    m_socketListener = socketListener;
}

// -------------------------------------------------------
void TcpServerSocket::close() {
    if (m_tcpState == TCP_LISTENING) {
        LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "TcpServerSocket::close(), fd = " << this->getSocket());
        m_tcpState = TCP_CLOSED;
        Socket::close();
    }
}

// -------------------------------------------------------
TcpSocket* TcpServerSocket::accept() {
    //m_lock->lock();

    if (m_tcpState == TCP_LISTENING) {
        // asynchronous mode
        if (m_socketListener != 0) {
            //m_tcpState = TCP_ACCEPTING;
            //m_lock->unlock();
            m_reactorInstance->registerInputHandler(this, this);
            return 0;
        } 
        // synchronous mode
        else {
            //m_lock->unlock();
            int newSocket;
            Socket::InetAddressPort remoteAddrPort;
            int result = Socket::accept(newSocket, remoteAddrPort);
            if (result == SKT_SUCC) {
                LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "accept a new TCP connection, new fd = " << newSocket);
                return new TcpSocket(newSocket, remoteAddrPort);
            } else {
                LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "fail to accept(), result = " << result);
                close();
                return 0;
            }
        }

    } else {
        //m_lock->unlock();
        LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "Not allow to accept new connection in state: " << m_tcpState);
        return 0;
    }

}

// -------------------------------------------------------
void TcpServerSocket::handleInput(Socket* theSocket) {
    int newSocket;
    Socket::InetAddressPort remoteAddrPort;
    int result = theSocket->accept(newSocket, remoteAddrPort);

    assert(m_tcpState == TCP_LISTENING);
    TcpSocket* newTcpSocket;

    if (result == SKT_SUCC) {
        newTcpSocket = new TcpSocket(newSocket, remoteAddrPort);
        m_socketListener->handleAcceptResult(this, newTcpSocket);
    } else if (result == SKT_WAIT) {
        LOG4CPLUS_WARN(_NET_LOOGER_NAME_, "no new TCP connection accepted.");
        // TODO only need to acdept again if the server socket is deleted from epoll
        // after receiving epoll event on the socket
        //this->accept();
    } else {
        LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "error occur on accept(), close the server socket");
        m_reactorInstance->removeHandlers(this);
        close();
    }
}