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

    class TcpServerWorker;
    class TcpServerSocket;
    class TcpSocket;

    class TcpServer : public TcpServerSocketListener {
    public:
        TcpServer(unsigned short localPort, std::string localIp = "0.0.0.0", int backlog = 100);
        ~TcpServer();

        void start();
        bool isRunning() const;

        virtual void handleAcceptResult(TcpServerSocket* serverSocket, TcpSocket* newSocket);
        virtual void handleCloseResult(TcpServerSocket* serverSocket);

    private:
        TcpServerSocket* m_tcpServerSocket;       
        // array to store number of TcpServerWorker instances
        TcpServerWorker** m_tcpServerWorkerArray;
        int m_numberOfWorkers;
        bool m_isRunning;
    };

    // ----------------------------------------
    inline bool TcpServer::isRunning() const {
        return m_isRunning;
    }
}

#endif