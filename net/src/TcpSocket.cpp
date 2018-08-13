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
  m_reactor(0),
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
            m_reactor = Reactor::getInstance(); 
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
        int numOfByteRecved;
        int result = Socket::recv(theBuffer, buffSize, numOfByteRecved);
        if (result == SKT_SUCC) {
            return numOfByteRecved;
        }
        LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "error occurred on recv()");
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

            if (result == SKT_SUCC) {
                // complete sending all data (normal case)
                if (numberOfBytesSent >= numOfBytesToSend) {
                    m_lock->unlock();
                    m_socketListener->handleSendResult(this, numberOfBytesSent);
                    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "numberOfBytesSent = " << numberOfBytesSent);
                    return numberOfBytesSent;
                }

                // not send data complete due to send buffer full
                if (m_tcpState == TCP_CONNECTED) {
                    m_tcpState = TCP_SENDING;
                } else {
                    m_tcpState = TCP_SENDING_RECEIVING;
                }
                m_lock->unlock();

                m_sendBuffer = theBuffer + numberOfBytesSent;
                m_sendBuferSize = numOfBytesToSend - numberOfBytesSent;
                m_numOfBytesSent = numberOfBytesSent;
                LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "send " << numberOfBytesSent << " bytes, register EPOLLOUT event for sendind data");
                m_reactor->registerOutputHandler(this, this);
                return numberOfBytesSent;
            }

            // no data sent due to send buffer full, register to reactor epoll
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
                m_reactor->registerOutputHandler(this, this);
                return 0;
            }

            // error happen
            m_lock->unlock();
            LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "error on send()");
            return -1;
        }

        m_lock->unlock();
        LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "error TCP connect state: " << m_tcpState);
        return -1;
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
            Socket::connect(m_remoteAddrAndPort);
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
    if (this->getSocket() < 0) {
        return;
    }

    if (m_socketListener != 0) {
        // async mode
        m_lock->lock();
        if (m_tcpState != TCP_CLOSED) {
            int preState = m_tcpState;
            m_tcpState = TCP_CLOSED;
            m_lock->unlock();

            m_reactor->removeHandlers(this);
            Socket::close();

            // notify listener to release 
            if (preState == TCP_ERROR_CLOSING) {
                m_socketListener->handleErrorResult(this);
            } else {
                m_socketListener->handleCloseResult(this);
            }
        } else {
            m_lock->unlock();
        }
    } else {
        // sync mode
        // TODO
        if (m_tcpState != TCP_CLOSED) {
            if (m_reactor) {
                m_reactor->removeHandlers(this);
            }
            Socket::close();
            m_tcpState = TCP_CLOSED;   
        }   
    }
}

// ----------------------------------------------
void TcpSocket::handleInput(Socket* theSocket) {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "TcpSocket::handleInput(), fd = " << theSocket->getSocket());
   
    m_lock->lock();

    assert(m_tcpState == TCP_RECEIVING || m_tcpState == TCP_SENDING_RECEIVING);
    int numOfByteRecved;
    int result = Socket::recv(m_recvBuffer, m_recvBufferSize, numOfByteRecved);
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
        m_tcpState = TCP_ERROR_CLOSING;
        m_lock->unlock();
        LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "error occur on recv(), close the connection");
        close();
    }
}

// ----------------------------------------------
void TcpSocket::handleOutput(Socket* theSocket) {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "TcpSocket::handleOutput(), fd = " << theSocket->getSocket());
    m_lock->lock();

    assert(m_tcpState == TCP_SENDING || m_tcpState == TCP_SENDING_RECEIVING || m_tcpState == TCP_CONNECTING);
    
    if (m_tcpState == TCP_SENDING || m_tcpState == TCP_SENDING_RECEIVING) {
        int numOfBytesSent;
        int result = Socket::send(m_sendBuffer, m_sendBuferSize, numOfBytesSent);
        
        // SKT_WAIT should not happen as epoll notify the socket is ready to send
        if (result != SKT_SUCC) {
            m_tcpState = TCP_ERROR_CLOSING;
            m_lock->unlock();
            close();
        } else {
            m_sendBuffer += numOfBytesSent;
            m_sendBuferSize -= numOfBytesSent;
            m_numOfBytesSent += numOfBytesSent;
            if (m_sendBuferSize == 0) {
                if (m_tcpState == TCP_SENDING) {
                    m_tcpState = TCP_CONNECTED;
                } else {
                    m_tcpState = TCP_RECEIVING;
                }
                m_lock->unlock();
                
                // notify the listener send complete
                m_socketListener->handleSendResult(this, m_numOfBytesSent);
            } else {
                m_lock->unlock();

                // register the EPOLLOUT again to continue to send remaining data
                m_reactor->registerOutputHandler(this, this);
            }
        }
    } else {
        // TODO connecting
        m_lock->unlock();
    }

    
}
