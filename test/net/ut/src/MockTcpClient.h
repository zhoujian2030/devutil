#ifndef MOCK_TCP_CLIENT_H
#define MOCK_TCP_CLIENT_H

#include "TcpSocket.h"
#include "DataBuffer.h"
#include <string>

class MockTcpClient {
public:
    MockTcpClient(std::string ip, unsigned short port);
    ~MockTcpClient();
    
    bool connect();   
    
    int send(std::string data);
    
    net::TcpSocket* m_tcpSocket;
    cm::DataBuffer* m_sendBuffer;
    // int m_sendBufferSize;
};

#endif