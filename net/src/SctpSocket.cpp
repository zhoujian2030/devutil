/*
 * SctpSocket.cpp
 *
 *  Created on: Apr 19, 2016
 *      Author: z.j
 */

#include "SctpSocket.h"
#include "NetLogger.h"

#include <errno.h>
#include <string.h>

using namespace net;

#ifdef SCTP_SUPPORT
// ----------------------------------------------
SctpSocket::SctpSocket(std::string localIp, short localPort, int saFamily)
: Socket(localIp, localPort, SOCK_STREAM, IPPROTO_SCTP, AF_INET)
{
    // set socket option
}

// ---------------------------------------------
SctpSocket::SctpSocket(int socket) 
: Socket(socket, SOCK_STREAM)
{

}

// ----------------------------------------------
SctpSocket::~SctpSocket() {

}

// ----------------------------------------------
int SctpSocket::recv(char* theBuffer, int buffSize, int& numOfBytesReceived, int flags) {
    if (theBuffer == 0) {
        LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "null pointer buffer!");
        return SKT_ERR;
    }

    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "SctpSocket::recv(), fd = " << m_socket);

    struct sctp_sndrcvinfo sndrcvinfo;
    int msgFlags;
    int result = sctp_recvmsg(m_socket, theBuffer, buffSize, 0, 0, &sndrcvinfo, &msgFlags);

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

// ----------------------------------------------
int SctpSocket::send(const char* theBuffer, int numOfBytesToSend, int& numberOfBytesSent) {
    return SKT_SUCC;
}

#endif
