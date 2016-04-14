/*
 * Socket.h
 *
 *  Created on: Apr 05, 2016
 *      Author: z.j
 */

#ifndef SOCKET_H
#define SOCKET_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string>

namespace net {

    typedef enum {
        SKT_SUCC,
        SKT_ERR,
        SKT_WAIT
    } SocketErrCode;

    // currently only support IPV4 TCP socket
    class Socket {
    public:

        struct InetAddressPort {
            struct sockaddr_in addr;
            unsigned short port;
        };

        Socket(std::string localIp, short localPort, int socketType = SOCK_STREAM, int saFamily = AF_INET);
        Socket(int socket, int socketType = SOCK_STREAM);
        virtual ~Socket();

        // Only applicable for TCP server socket
        bool bind();
        bool listen(int backlog=10000);
        int accept(int& theSocket, InetAddressPort& theRemoteAddrPort);

        // Only applicable for TCP client socket
        int connect(const InetAddressPort& theRemoteAddrPort);

        // Close socket
        void close();

        // Receive data from socket
        int recv(char* theBuffer, int buffSize, int& numOfBytesReceived, int flags = 0);
        // Identical to recv() with flags set to 0
        int read(char* theBuffer, int buffSize, int& numOfBytesReceived);

        // Send data to socket
        int send(const char* theBuffer, int numOfBytesToSend, int& numberOfBytesSent);
        int write(const char* theBuffer, int numOfBytesToSend, int& numberOfBytesSent);

        void makeNonBlocking();
        void makeBlocking();

        int getSocket() const;

        static std::string getHostAddress(struct sockaddr* sockaddr);
        static void getSockaddrByIpAndPort(struct sockaddr_in* sockaddr, std::string ip, unsigned short port);

    private: 
        typedef enum {
            // The socket is successfull created by socket()
            CREATED,

            // The socket is successfull binded with local ip
            // and port by bind()
            // Only valid for server socket
            BINDED,

            // The socket is listenning by listen()
            // Only valid for server socket
            LISTENING,

            // For server, accept a new connection from client, 
            // new socket is created
            // For client, the socket is successfull connected
            // to a server
            CONNECTED,

            // Initial state for a new socket, or the socket is
            // already closed
            CLOSED
        } State;

        // socket fd of the server of client
        int m_socket;

        int m_socketType;

        // local ip and port
        std::string m_localIp;
        unsigned short m_localPort;
        struct sockaddr_in m_localSa;

        State m_state;
    };


    // ----------------------------------------------------------
    inline int Socket::getSocket() const {
        return m_socket;
    }
}

#endif