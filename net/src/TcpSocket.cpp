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
: Socket(socket, SOCK_STREAM)  
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

// ----------------------------------------------
void TcpSocket::handleInput(Socket* theSocket) {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "TcpSocket::handleInput(), fd = " << theSocket->getSocket());

    // TODO
}
