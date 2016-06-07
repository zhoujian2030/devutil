#ifndef MOCK_TCP_SERVER_H
#define MOCK_TCP_SERVER_H

#include "TcpServerSocketListener.h"
#include "TcpServerSocket.h"
#include "TcpSocket.h"
#include "TcpServerCallback.h"
#include <string>

class MockTcpServerWorker;

class MockTcpServer : public net::TcpServerSocketListener {
public:
    MockTcpServer(
        net::TcpServerCallback* theServerCallback,
        unsigned short localPort = 12345, 
        std::string localIp = "127.0.0.1", 
        int backlog = 100);
            
    ~MockTcpServer();
    
    void addSocketListener(net::TcpServerSocketListener* theSocketListener);
    
    net::TcpSocket* accept();
    
    virtual void handleAcceptResult(net::TcpServerSocket* serverSocket, net::TcpSocket* newSocket);
    
    virtual void handleCloseResult(net::TcpServerSocket* serverSocket);
   
    net::TcpServerSocket* m_tcpServerSocket;
    MockTcpServerWorker* m_tcpServerWorker;
    
    net::TcpSocket* m_newTcpSocket;
};

#endif