#ifndef MOCK_TCP_SERVER_WORKER_H
#define MOCK_TCP_SERVER_WORKER_H

#include "TcpServerWorker.h"
#include "Worker.h" 
#include "TcpConnection.h"
#include "TcpSocket.h"
#include "TcpServerCallback.h"

// using namespace net;
// using namespace cm;

class MockTcpServerWorker : public net::TcpServerWorker {
public:
    MockTcpServerWorker(cm::Worker* theWorker, net::TcpServerCallback* theServerCallback);
    
    virtual ~MockTcpServerWorker();
    
    virtual void onConnectionCreated(net::TcpSocket* theNewSocket);
    
    virtual void handleRecvResult(net::TcpSocket* theSocket, int numOfBytesRecved);
    
    void addSocketListener(net::TcpSocketListener* theSocketListener);
    
    net::TcpConnection* m_tcpConn;
    unsigned int m_connId;
    
    net::TcpSocket* m_newTcpSocket;
    
    unsigned int m_socketUserData;
    int m_numOfBytesRecved;
};


#endif