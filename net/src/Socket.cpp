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
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;
using namespace net;

// ------------------------------------------------
Socket::Socket(
    std::string localIp, 
    unsigned short localPort, 
    int socketType, 
    int protocol,
    int saFamily)
: m_socket(-1), m_socketType(socketType), m_localIp(localIp), 
  m_localPort(localPort), m_state(CLOSED)
{
    NetLogger::initConsoleLog();

    // create socket
    m_socket = ::socket(saFamily, socketType, protocol);
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

    // for client socket, m_localPort is set to 0, m_localIp is unspecified ?
    if (m_localPort != 0) {
        // check if the ip is valid
        if (INADDR_NONE == inet_addr(m_localIp.c_str())) {
            LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "IP is not valid: " << m_localIp);
            close();
            return;
        }
        memset(&m_localSa, 0, sizeof(struct sockaddr_in));
        m_localSa.sin_family = saFamily;
        m_localSa.sin_addr.s_addr = inet_addr(m_localIp.c_str());
        m_localSa.sin_port = htons(m_localPort);
        
        m_role = SERVER_SOCKET;
    } else {
        m_role = CLIENT_SOCKET;
    }

    m_state = CREATED;

    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "Success to create socket: " << m_socket << ", address = " << m_localIp << ":" << m_localPort);
}

// ------------------------------------------------
Socket::Socket(int socket, int socketType)
: m_socket(socket), m_socketType(socketType), m_localIp("null"), 
  m_localPort(0), m_state(CONNECTED)
{
    socklen_t length = sizeof(struct sockaddr);

    if (::getsockname(m_socket, (struct sockaddr*)&m_localSa, &length) == 0) {
        m_localPort = ntohs(m_localSa.sin_port);
        m_localIp = Socket::getHostAddress((struct sockaddr*)&m_localSa);
    }
    
    m_role = CONNECT_SOCKET;

    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "initialize the connected socket, fd = " << m_socket 
        << ", address = " << m_localIp << ":" << m_localPort);
}

// ------------------------------------------------
Socket::~Socket() {
    close();
}

// ------------------------------------------------
bool Socket::bind() {
    if (m_state == CREATED && m_localPort != 0) {
        int result = ::bind(m_socket, (struct sockaddr *)&m_localSa, sizeof(struct sockaddr_in));
        if (result == -1) {
            LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "fail to bind socket, fd = " << m_socket << ", address = " << m_localIp << 
                ":" << m_localPort << ". errno = " << errno << " - " << strerror(errno));
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
        LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "success to listen socket: " << m_socket);
    } else {
        LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "socket is not ready to listen: " << m_socket);
        return false;
    }

    return true;
}

// ------------------------------------------------
int Socket::accept(int& theSocket, InetAddressPort& theRemoteAddrPort) {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "Socket::accept(), fd = " << m_socket);

    if (m_state != LISTENING) {
        LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "socket is not ready to accept connection: " << m_socket);
        return SKT_ERR;
    }

    struct sockaddr_in remoteAddr;
    socklen_t length = sizeof(remoteAddr);
    int newFd = ::accept(m_socket, (struct sockaddr*)&remoteAddr, &length);
    if (newFd == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            // For non-blocking socket, it would return EAGAIN or EWOULDBLOCK 
            // when no data read from socket
            LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "no new connection coming now, fd = " << m_socket);
            return SKT_WAIT;
        } else {
            LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "fail to accept for socket: " << m_socket <<
                ". errno = " << errno << " - " << strerror(errno));
            // TODO throw io exception ??
            return SKT_ERR;
        }
    }

    memcpy(&theRemoteAddrPort.addr, &remoteAddr, sizeof(theRemoteAddrPort.addr));
    theRemoteAddrPort.port = ntohs(remoteAddr.sin_port);
    theSocket = newFd;

    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "accept new connection, fd = " << theSocket << ", remote address = " 
        << Socket::getHostAddress((struct sockaddr*)&theRemoteAddrPort.addr) << ":" << theRemoteAddrPort.port);

    return SKT_SUCC;
}

// ------------------------------------------------
int Socket::connect(const InetAddressPort& theRemoteAddrPort) {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "Socket::connect(), fd = " << m_socket);

    if (m_state != CREATED || m_state == CONNECTED) {
        LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "socket is not ready to connect server: " << m_socket);
        return SKT_ERR;
    }

    int result = ::connect(m_socket, (struct sockaddr*)&theRemoteAddrPort.addr, sizeof(theRemoteAddrPort.addr));
    if (result == -1) {
        if (errno == EINPROGRESS) {
            LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "connecting, fd = " << m_socket << ", " << strerror(errno));
            return SKT_WAIT;
        } else {
            LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "fail to connect on socket: " << m_socket <<
                ". errno = " << errno << " - " << strerror(errno));
            // TODO throw io exception ??
            return SKT_ERR;
        }
    }

    socklen_t length = sizeof(struct sockaddr);
    if (::getsockname(m_socket, (struct sockaddr*)&m_localSa, &length) == 0) {
        m_localPort = ntohs(m_localSa.sin_port);
        m_localIp = Socket::getHostAddress((struct sockaddr*)&m_localSa);

        LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "connect success, local address = " << m_localIp << ", local port = "
            << m_localPort << ", fd = " << m_socket);

        m_state = CONNECTED;
        return SKT_SUCC;
    } else {
        LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "fail to getsockname on socket: " << m_socket <<
            ". errno = " << errno << " - " << strerror(errno));
        return SKT_ERR;
    }
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
//  only used in connected socket (TCP socket)
// -------------------------------------------------
int Socket::recv(char* theBuffer, int buffSize, int& numOfBytesReceived, int flags) {
    if (theBuffer == 0) {
        LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "null pointer buffer!");
        return SKT_ERR;
    }

    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "Socket::recv(), fd = " << m_socket);

    int result = ::recv(m_socket, theBuffer, buffSize, flags);

    if (result == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            // For non-blocking socket, it would return EAGAIN or EWOULDBLOCK 
            // when no data read from socket
            LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "no data received from the socket now, fd = " << m_socket
                << ", " << strerror(errno));
            return SKT_WAIT;
        } else {
            LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "fail to recv data from socket: " << m_socket
                << ", errno = " << errno << " - " << strerror(errno));
            // TODO throw io exception ??
            return SKT_ERR;
        }
    }

    numOfBytesReceived = result;
    return SKT_SUCC;
}

// -------------------------------------------------
int Socket::read(char* theBuffer, int buffSize, int& numOfBytesReceived) {
    if (theBuffer == 0) {
        LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "null pointer buffer!");
        return SKT_ERR;
    }

    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "Socket::read(), fd = " << m_socket);    

    int result = ::read(m_socket, theBuffer, buffSize);

    if (result == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            // For non-blocking socket, it would return EAGAIN or EWOULDBLOCK 
            // when no data read from socket
            LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "no data read from the socket now, fd = " << m_socket
                << ", " << strerror(errno));
            return SKT_WAIT;
        } else {
            LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "fail to read data from socket: " << m_socket
                << ", errno = " << errno << " - " << strerror(errno));
            // TODO throw io exception ??
            return SKT_ERR;
        }
    }

    numOfBytesReceived = result;
    return SKT_SUCC;    
}

// -------------------------------------------------
int Socket::send(const char* theBuffer, int numOfBytesToSend, int& numberOfBytesSent) {
    if (theBuffer == 0) {
        LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "null pointer buffer!");
        return SKT_ERR;
    }

    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "Socket::send(), fd = " << m_socket);

    int result = ::send(m_socket, theBuffer, numOfBytesToSend, 0);
    if (result == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            // For non-blocking socket, it would return EAGAIN or EWOULDBLOCK 
            // when send buffer is full
            LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "no data read from the socket now, fd = " << m_socket);
            return SKT_WAIT;
        } else {
            LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "fail to send data to socket: " << m_socket
                << ", errno = " << errno << " - " << strerror(errno));    
            return SKT_ERR;        
        }
    }

    numberOfBytesSent = result;
    return SKT_SUCC;
}

// -------------------------------------------------
int Socket::write(const char* theBuffer, int numOfBytesToSend, int& numberOfBytesSent) {
    // TODO
    return false;
}

// -------------------------------------------------
void Socket::makeNonBlocking() {
    int option = 1;
    if (ioctl(m_socket, FIONBIO, &option) == -1) {
        LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "fail to set non-blocking by ioctl. errno = " 
            << errno << " - " << strerror(errno));
    }
}

// -------------------------------------------------
void Socket::makeBlocking() {
    int flags = fcntl(m_socket, F_GETFL, 0);
    if (fcntl(m_socket, F_SETFL, flags & (~O_NONBLOCK)) == -1) {
        LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "fail to set blocking by fcntl. errno = " 
            << errno << " - " << strerror(errno));
    }

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

// ------------------------------------------------
void Socket::getSockaddrByIpAndPort(struct sockaddr_in* sockaddr, std::string ip, unsigned short port) {
    if (sockaddr == 0) {
        return;
    }

    memset(sockaddr, 0, sizeof(struct sockaddr_in));
    sockaddr->sin_family = AF_INET;
    sockaddr->sin_addr.s_addr = inet_addr(ip.c_str());
    sockaddr->sin_port = htons(port);

}