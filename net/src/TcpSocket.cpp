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
: Socket(socket, SOCK_STREAM), m_socketListener(0) 
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

// ----------------------------------------------
void TcpSocket::handleInput(Socket* theSocket) {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "TcpSocket::handleInput(), fd = " << theSocket->getSocket());

    // TODO
}
