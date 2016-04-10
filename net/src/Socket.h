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

    // currently only support IPV4 TCP socket
    class Socket {
    public:

        struct InetAddressPort {
            struct sockaddr_in addr;
            unsigned short port;
        };

        Socket(std::string ip, short port, int socketType = SOCK_STREAM, int saFamily = AF_INET);
        Socket(int socket, int socketType = SOCK_STREAM);
        virtual ~Socket();

        // Only applicable for TCP server socket
        bool bind();
        bool listen(int backlog=10000);
        bool accept(InetAddressPort& theRemoteAddrPort, int& theSocket);

        // Only applicable for TCP client socket
        bool connect();

        // Close socket
        void close();

        // Receive data from socket
        bool recv(char* theBuffer, int buffSize, int& numOfBytesReceived);
        // Send data to socket
        bool send(char* theBuffer, int numberOfBytesSent);

        int getSocket() const;

        static std::string getHostAddress(struct sockaddr* sockaddr);

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

        // for server socket, they stand for local ip and port
        // for client socket, they stand for remote ip and port
        std::string m_hostIp;
        unsigned short m_port;
        struct sockaddr_in m_sa;

        State m_state;
    };


    // ----------------------------------------------------------
    inline int Socket::getSocket() const {
        return m_socket;
    }
}

#endif