/*
 * TcpServer.h
 *
 *  Created on: May 12, 2016
 *      Author: z.j
 */

#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include "TcpServerSocketListener.h"
#include <string>

namespace net {

    class TcpServerSocket;
    class TcpSocket;

    class TcpServer : public TcpServerSocketListener {
    public:
        TcpServer(std::string localIp, unsigned short localPort, int backlog = 100);
        ~TcpServer();

        void start();
        bool isRunning() const;

        virtual void handleAcceptResult(TcpServerSocket* serverSocket, TcpSocket* newSocket);
        virtual void handleCloseResult(TcpServerSocket* serverSocket);

    private:
        TcpServerSocket* m_TcpServerSocket;
        bool m_isRunning;
    };

    // ----------------------------------------
    inline bool TcpServer::isRunning() const {
        return m_isRunning;
    }
}

#endif