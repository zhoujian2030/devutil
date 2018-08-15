/*
 * socket.c
 *
 *  Created on: June 2, 2018
 *      Author: j.zh
 */

#include "socket.h"
#include <errno.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdio.h>

// ---------------------------------
int SocketClose(int fd)
{
    if (fd >= 0) {
        return close(fd);
    }

    return -1;
}

// ------------------------------------------------
void SocketGetSockaddrByIpAndPort(struct sockaddr_in* sockaddr, const char* ip, unsigned short port) {
    if (sockaddr == 0 || ip == 0) {
        return;
    }

    memset(sockaddr, 0, sizeof(struct sockaddr_in));
    sockaddr->sin_family = AF_INET;
    sockaddr->sin_addr.s_addr = inet_addr(ip);
    sockaddr->sin_port = htons(port);
}

// ---------------------------------
int SocketUdpInitAndBind(unsigned short port, char* localIp)
{
    // create socket
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1) {
        printf("[ERROR] [%s], fail to create socket. errno = %d - %s\n", __func__, errno, strerror(errno));
        return fd;
    }

    printf("[DEBUG] [%s], create socket fd = %d\n", __func__, fd);

    // set the socket reuse address by default to avoid the port is locked after a system crash
    int option = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&option, sizeof(option)) != 0) {
        printf("[WARN ] [%s], fail to set SO_REUSEADDR. errno = %d - %s\n", __func__, errno, strerror(errno));
    }

    struct sockaddr_in localSa;
    memset(&localSa, 0, sizeof(struct sockaddr_in));
    localSa.sin_family = AF_INET;
    localSa.sin_port = htons(port);
    if (localIp != 0) {
        // check if the ip is valid
        assert(INADDR_NONE != inet_addr(localIp));
        localSa.sin_addr.s_addr = inet_addr(localIp);
    } else {
        localSa.sin_addr.s_addr = inet_addr("0.0.0.0");
    }

    int result = bind(fd, (struct sockaddr *)&localSa, sizeof(struct sockaddr_in));
    if (result == -1) {
        close(fd);
        return result;
    }

    // printf("[DEBUG] [%s], bind socket fd success, ip = %s, port = %d\n", __func__, localIp, port);

    return fd;
}

// ---------------------------------
int SocketUdpInit()
{
    // create socket
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1) {
        printf("[ERROR] [%s], fail to create socket. errno = %d - %s\n", __func__, errno, strerror(errno));
        return fd;
    }

    printf("[DEBUG] [%s], create socket fd = %d\n", __func__, fd);

    // set the socket reuse address by default to avoid the port is locked after a system crash
    int option = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&option, sizeof(option)) != 0) {
        printf("[WARN ] [%s], fail to set SO_REUSEADDR. errno = %d - %s\n", __func__, errno, strerror(errno));
    }   

    return fd;
}

// ---------------------------------
void SocketMakeNonBlocking(int fd)
{
    if (fd <= 0) {
        printf("[ERROR] [%s], invalid fd = %d\n", __func__, fd);
        return;
    }

    int option = 1;
    if (ioctl(fd, FIONBIO, &option) == -1) {
        printf("[ERROR] [%s], fail to set non-blocking by ioctl. errno = %d - %s\n", __func__, errno, strerror(errno));
    }
}

// ---------------------------------
void SocketMakeBlocking(int fd)
{
    if (fd <= 0) {
        printf("[ERROR] [%s], invalid fd = %d\n", __func__, fd);
        return;
    }

    int flags = fcntl(fd, F_GETFL, 0);
    if (fcntl(fd, F_SETFL, flags & (~O_NONBLOCK)) == -1) {
        printf("[ERROR] [%s], fail to set blocking by fcntl.  errno = %d - %s", __func__, errno, strerror(errno));
    }
}

// ---------------------------------
int SocketUdpRecv(int fd, char* pBuffer, int bufferSize, struct sockaddr_in* pRemoteAddr)
{
    if (pBuffer == 0 || pRemoteAddr == 0 || bufferSize == 0 || fd <= 0) {
        printf("[ERROR] [%s], invalid parameters, fd = %d\n", __func__, fd);
        return -1;
    }

    socklen_t length = sizeof(*pRemoteAddr);
    int bytesRecvd = recvfrom(fd, pBuffer, bufferSize, 0, (struct sockaddr*)pRemoteAddr, &length);
    if (bytesRecvd == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            // For non-blocking socket, it would return EAGAIN or EWOULDBLOCK 
            // when no data read from socket
            //  printf("[TRACE] [%s], no data received from the socket now, fd = %d, %s\n", __func__, fd, strerror(errno));           
            return 0;
        } else {
            printf("[ERROR] [%s], fail to recv data from socket: %d, errno = %d - %s\n", __func__, fd, errno, strerror(errno));
            return -1;
        }
    }

    return bytesRecvd;
}

// ---------------------------------
int SocketUdpSend(int fd, char* pBuffer, int numBytesToSend, struct sockaddr_in* pRemoteAddr)
{
    if (pBuffer == 0 || pRemoteAddr == 0 || numBytesToSend == 0 || fd <= 0) {
        printf("[ERROR] [%s], invalid parameters\n", __func__);
        return -1;
    }

    socklen_t length = sizeof(*pRemoteAddr);
    int numberBytesSent = sendto(fd, pBuffer, numBytesToSend, 0, (struct sockaddr*)pRemoteAddr, length);
    if (numberBytesSent == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            // For non-blocking socket, it would return EAGAIN or EWOULDBLOCK 
            // when send buffer is full
            printf("[WARN ] [%s], no data send to the socket now, fd = %d\n", __func__, fd);
            return 0;
        } else {
            printf("[ERROR] [%s], fail to send data to socket: %d. errno = %d - %s\n", __func__, fd, errno, strerror(errno));    
            return -1;        
        }
    }

    return numberBytesSent;
}
