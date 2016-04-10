/*
 * Socket.cpp
 *
 *  Created on: Apr 05, 2016
 *      Author: z.j
 */

#include "Socket.h"
#include "NetLogger.h"

#include <errno.h>
#include <string.h>
#include <netdb.h>

using namespace std;
using namespace net;

// ------------------------------------------------
Socket::Socket(
    std::string ip, 
    short port, 
    int socketType, 
    int saFamily)
: m_socket(-1), m_socketType(socketType), m_hostIp(ip), 
  m_port(port), m_state(CLOSED)
{
    NetLogger::initConsoleLog();

    // create socket
    m_socket = ::socket(saFamily, socketType, 0);
    if (m_socket == -1) {
        LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "fail to create socket. errno = " << errno 
            << " - " << strerror(errno));

        // _exit() ??
        return;
    }

    // set the socket reuse address by default to avoid the port is locked after a system crash
    int option = 1;
    if (setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&option, sizeof(option)) != 0) {
        LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "fail to set SO_REUSEADDR. errno = " << errno
            << " - " << strerror(errno));
    }


    // check if the ip is valid
    if (INADDR_NONE == inet_addr(m_hostIp.c_str())) {
        LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "IP is not valid: " << m_hostIp);
        close();
        return;
    }
    memset(&m_sa, 0, sizeof(struct sockaddr_in));
    m_sa.sin_family = saFamily;
    m_sa.sin_addr.s_addr = inet_addr(m_hostIp.c_str());
    m_sa.sin_port = htons(m_port);

    m_state = CREATED;

    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "Success to create socket: " << m_socket << ", address = " << m_hostIp << ":" << m_port);
}

// ------------------------------------------------
Socket::Socket(int socket, int socketType)
: m_socket(socket), m_socketType(socketType), m_hostIp("null"), 
  m_port(0), m_state(CONNECTED)
{
    socklen_t length = sizeof(struct sockaddr);

    if (getsockname(m_socket, (struct sockaddr*)&m_sa, &length) == 0) {
        m_port = ntohs(m_sa.sin_port);
        m_hostIp = Socket::getHostAddress((struct sockaddr*)&m_sa);
    }

    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "initialize the connected socket, fd = " << m_socket 
        << ", address = " << m_hostIp << ":" << m_port);
}

// ------------------------------------------------
Socket::~Socket() {
    close();
}

// ------------------------------------------------
bool Socket::bind() {
    if (m_state == CREATED && m_port != 0) {
        int result = ::bind(m_socket, (struct sockaddr *)&m_sa, sizeof(struct sockaddr_in));
        if (result == -1) {
            LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "fail to bind socket, fd = " << m_socket << ", address = " << m_hostIp << 
                ":" << m_port << ". errno = " << errno << " - " << strerror(errno));
            close();
            return false;
        }
        m_state = BINDED;
        LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "success to bind socket: " << m_socket);
    } else if (m_state == CLOSED) {
        LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "socket is not created yet or already closed: " << m_socket);
        return false;
    } else {
        LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "no need to bind socket: " << m_socket);
    }
    
    return true;
}

// ------------------------------------------------
bool Socket::listen(int backlog) {
    if (m_state == LISTENING) {
        LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "socket is already listening: " << m_socket);
        return true;
    }

    if (m_state == BINDED) {
        int result = ::listen(m_socket, backlog);
        if (result == -1) {
            LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "fail to listen socket, fd = " << m_socket 
                << ". errno = " << errno << " - " << strerror(errno));
            close();
            return false;
        }
        m_state = LISTENING;
        LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "success to lisgen socket: " << m_socket);
    } else {
        LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "socket is not ready to listen: " << m_socket);
        return false;
    }

    return true;
}

// ------------------------------------------------
bool Socket::accept(InetAddressPort& theRemoteAddrPort, int& theSocket) {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "Socket::accept(), fd = " << m_socket);

    if (m_state != LISTENING) {
        LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "socket is not ready to accept connection: " << m_socket);
        return false;
    }

    struct sockaddr_in remoteAddr;
    socklen_t length = sizeof(remoteAddr);
    int newFd = ::accept(m_socket, (struct sockaddr*)&remoteAddr, &length);
    if (newFd == -1) {
        LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "fail to accept for socket: " << m_socket <<
            ". errno = " << errno << " - " << strerror(errno));
        return false;
    }

    memcpy(&theRemoteAddrPort.addr, &remoteAddr, sizeof(theRemoteAddrPort.addr));
    theRemoteAddrPort.port = ntohs(remoteAddr.sin_port);
    theSocket = newFd;

    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "accept new connection, fd = " << theSocket << ", remote address = " 
        << Socket::getHostAddress((struct sockaddr*)&theRemoteAddrPort.addr) << ":" << theRemoteAddrPort.port);

    return true;
}

// ------------------------------------------------
bool Socket::connect() {
    // TODO
    return false;
}

// ------------------------------------------------
void Socket::close() {
    m_state = CLOSED;
    if (m_socket != -1) {
        ::close(m_socket);
        m_socket = -1;
    }
}

// -------------------------------------------------
bool Socket::recv(char* theBuffer, int buffSize, int& numOfBytesReceived) {
    if (theBuffer == 0) {
        LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "null pointer buffer!");
        return false;
    }

    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "Socket::recv(), fd = " << m_socket);

    int result = ::recv(m_socket, theBuffer, buffSize, 0);

    if (result == -1) {
        LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "fail to recv data from socket: " << m_socket
            << ", errno = " << errno << " - " << strerror(errno));
        return false;
    }

    numOfBytesReceived = result;
    return true;
}

// -------------------------------------------------
bool Socket::send(char* theBuffer, int numOfBytesSent) {
    //TODO
    return false;
}

// ------------------------------------------------
std::string Socket::getHostAddress(struct sockaddr* sockaddr) {
    if (sockaddr == 0) {
        return "null";
    }

    char tempAddr[NI_MAXHOST];
    socklen_t length = sizeof(struct sockaddr_in);

    int result = getnameinfo(sockaddr, length, tempAddr, sizeof(tempAddr), 0, 0, NI_NUMERICHOST);

    if (result != 0) {
        return "null"; 
    }

    std::string hostAddr(tempAddr);

    return hostAddr;
}