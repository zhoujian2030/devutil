/*
 * TcpSocket.cpp
 *
 *  Created on: May 09, 2016
 *      Author: z.j
 */

#include "TcpSocket.h"
#include "NetLogger.h"
#include "IoException.h"
#include <string.h>

using namespace net;
using namespace std;
using namespace cm;

// ----------------------------------------------
TcpSocket::TcpSocket(std::string remoteIp, unsigned short remotePort) 
: Socket("", 0),
  m_tcpState(TCP_IDLE),
  m_reactor(Reactor::getInstance()), 
  m_remoteIp(remoteIp),
  m_socketListener(0) 
{
    Socket::getSockaddrByIpAndPort(&m_remoteAddrAndPort.addr, remoteIp, remotePort);
    m_remoteAddrAndPort.port = remotePort;
    m_lock = new MutexLock(true);
    
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "Create TCP client socket to connect to " << m_remoteIp << ":" 
        << m_remoteAddrAndPort.port);
}

// ----------------------------------------------
TcpSocket::TcpSocket(int socket, Socket::InetAddressPort& theRemoteAddrPort)
: Socket(socket, SOCK_STREAM),
  m_tcpState(TCP_CONNECTED), 
  m_reactor(Reactor::getInstance()), 
  m_socketListener(0) 
{
    memcpy(&m_remoteAddrAndPort, &theRemoteAddrPort, sizeof(theRemoteAddrPort));
    m_remoteIp = Socket::getHostAddress((struct sockaddr*)&theRemoteAddrPort.addr);
    m_lock = new MutexLock(true);
    
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "Create TCP socket, local addr = " << m_localIp << ":" 
        << m_localPort << ", remote addr = " << m_remoteIp << ":" << m_remoteAddrAndPort.port);
}

// -----------------------------------------------
TcpSocket::~TcpSocket() {
    close();
}

// -----------------------------------------------
void TcpSocket::addSocketListener(TcpSocketListener* socketListener) {
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

// -----------------------------------------------
int TcpSocket::receive(char* theBuffer, int buffSize) {
    // async mode
    if (m_socketListener != 0) {
        m_lock->lock();

        if (m_tcpState == TCP_CONNECTED || m_tcpState == TCP_SENDING) {
            if (m_tcpState == TCP_CONNECTED) {
                m_tcpState = TCP_RECEIVING;
            } else {
                m_tcpState = TCP_SENDING_RECEIVING;
            }
            m_lock->unlock();
            
            m_recvBuffer = theBuffer;
            m_recvBufferSize = buffSize;
            m_reactor->registerInputHandler(this, this);
        } else {
            m_lock->unlock();

            LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "error TCP connect state: " << m_tcpState);
        }
        
        return 0;
    } 
    // sync mode
    else {
        LOG4CPLUS_WARN(_NET_LOOGER_NAME_, "Not supported synchronize mode now!");
        // TODO
        return -1;
    }
}

// ----------------------------------------------
int TcpSocket::send(const char* theBuffer, int numOfBytesToSend) {
    // async mode
    if (m_socketListener != 0) {
        m_lock->lock();

        if (m_tcpState == TCP_CONNECTED || TCP_RECEIVING) {
            int numberOfBytesSent;
            int result = Socket::send(theBuffer, numOfBytesToSend, numberOfBytesSent);
            if (result == SKT_WAIT) {
                if (m_tcpState == TCP_CONNECTED) {
                    m_tcpState = TCP_SENDING;
                } else {
                    m_tcpState = TCP_SENDING_RECEIVING;
                }
                m_lock->unlock();

                m_sendBuffer = theBuffer;
                m_sendBuferSize = numOfBytesToSend;
                m_numOfBytesSent = 0;
                LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "register EPOLLOUT event for sendind data");
                // TODO register the EPOLLOUT event?
                exit(0); // exit for test
            } else {
                // if success, numberOfBytesSent is set actual number of bytes sent, 
                // or it is set -1 in Socket::send
                m_lock->unlock();
                return numberOfBytesSent;
            }
        } else {
            m_lock->unlock();
            LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "error TCP connect state: " << m_tcpState);
        }
        
        // async send now or socket not ready for sending, return 0 immediately
        return 0;
    } 
    // sync mode
    else {
        int numberOfBytesSent;
        int result = Socket::send(theBuffer, numOfBytesToSend, numberOfBytesSent);
        if (result != SKT_SUCC) {
            LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "error occur on send() in sync mode, close the connection");
            close();
        }
        
        // if success, numberOfBytesSent is set actual number of bytes sent, 
        // or it is set -1 in Socket::send
        return numberOfBytesSent;
    }    
}

// ----------------------------------------------
void TcpSocket::connect() {
    // async mode
    if (m_socketListener != 0) {
        // TODO
        LOG4CPLUS_WARN(_NET_LOOGER_NAME_, "Not support async mode yet");
    } 
    // sync mode
    else {
        try {
            assert(Socket::connect(m_remoteAddrAndPort));
            m_tcpState = TCP_CONNECTED; 
        } catch (IoException& e) {
            LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "fail to connect to TCP server");
            close();
            throw e;
        }
    }
}

// ----------------------------------------------
void TcpSocket::close() {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "TcpSocket::close(), fd = " << this->getSocket());
    m_tcpState = TCP_CLOSED;
    m_reactor->removeHandlers(this);
    Socket::close();
}

// ----------------------------------------------
void TcpSocket::handleInput(Socket* theSocket) {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "TcpSocket::handleInput(), fd = " << theSocket->getSocket());
   
    m_lock->lock();

    assert(m_tcpState == TCP_RECEIVING || m_tcpState == TCP_SENDING_RECEIVING);
    int numOfByteRecved;
    int result = recv(m_recvBuffer, m_recvBufferSize, numOfByteRecved);
    if (result == SKT_SUCC) {
        if (m_tcpState == TCP_RECEIVING) {
            m_tcpState = TCP_CONNECTED;
        } else {
            m_tcpState = TCP_SENDING;
        }
        m_lock->unlock();

        m_socketListener->handleRecvResult(this, numOfByteRecved);
    } else if (result == SKT_WAIT) {
        m_lock->unlock();
        LOG4CPLUS_WARN(_NET_LOOGER_NAME_, "no new TCP data received");
        // TODO close or ignore it?
    } else {
        m_lock->unlock();
        LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "error occur on recv(), close the connection");
        close();
        // TODO use async close ?
    }
}

