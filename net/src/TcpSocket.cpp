/*
 * TcpSocket.cpp
 *
 *  Created on: May 09, 2016
 *      Author: z.j
 */

#include "TcpSocket.h"
#include "NetLogger.h"
#include <string.h>

using namespace net;
using namespace std;

// ----------------------------------------------
TcpSocket::TcpSocket(int socket, Socket::InetAddressPort& theRemoteAddrPort)
: Socket(socket, SOCK_STREAM),
  m_tcpState(TCP_CONNECTED), 
  m_reactorInstance(Reactor::getInstance()), 
  m_socketListener(0) 
{
    memcpy(&m_remoteSa, &theRemoteAddrPort.addr, sizeof(theRemoteAddrPort.addr));
    m_remoteIp = Socket::getHostAddress((struct sockaddr*)&theRemoteAddrPort.addr);
    m_remotePort = theRemoteAddrPort.port;

    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "Create TCP socket, local addr = " << m_localIp << ":" 
        << m_localPort << ", remote addr = " << m_remoteIp << ":" << m_remotePort);
}

// -----------------------------------------------
TcpSocket::~TcpSocket() {
    
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
        if (m_tcpState == TCP_CONNECTED) {
            m_tcpState = TCP_RECEIVING;
            m_recvBuffer = theBuffer;
            m_recvBufferSize = buffSize;
            m_reactorInstance->registerInputHandler(this, this);
        } else {
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
void TcpSocket::close() {
    if (m_tcpState == TCP_CONNECTED || m_tcpState == TCP_RECEIVING) {
        LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "TcpSocket::close(), fd = " << this->getSocket());
        m_tcpState = TCP_CLOSED;
        m_reactorInstance->removeHandlers(this);
        Socket::close();
    }
}

// ----------------------------------------------
void TcpSocket::handleInput(Socket* theSocket) {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "TcpSocket::handleInput(), fd = " << theSocket->getSocket());

    int numOfByteRecved;
    int result = recv(m_recvBuffer, m_recvBufferSize, numOfByteRecved);
    
    assert(m_tcpState == TCP_RECEIVING);
    
    if (result == SKT_SUCC) {
        m_socketListener->handleRecvResult(this, numOfByteRecved);
    } else if (result == SKT_WAIT) {
        LOG4CPLUS_WARN(_NET_LOOGER_NAME_, "no new TCP data received");
        // TODO
    } else {
        LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "error occur on recv(), close the connection");
        close();
    }
}

