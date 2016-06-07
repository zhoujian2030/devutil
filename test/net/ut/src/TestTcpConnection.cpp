#include "TestTcpConnection.h"
#include "MockTcpClient.h"
#include <iostream>

using namespace std;
using namespace net;
using namespace cm;
    
TEST_F(TestTcpConnection, AcceptNewConnectionAsync) {
    ASSERT_TRUE(m_tcpServer != 0);
    ASSERT_TRUE(m_tcpConnection == 0);
    ASSERT_GT(m_tcpServer->m_tcpServerSocket->getSocket(), 0);
    
    // set server socket nonblocking
    m_tcpServer->addSocketListener(m_tcpServer);
    m_tcpServer->accept();
    
    // client connect to server
    MockTcpClient* tcpClient = new MockTcpClient("127.0.0.1", 12345);
    ASSERT_TRUE(tcpClient->connect());
    
    Thread::sleep(10);
    
    // server accept new tcp connection
    MockTcpServerWorker* tcpServerWorker =  m_tcpServer->m_tcpServerWorker;
    m_tcpConnection = tcpServerWorker->m_tcpConn;
    ASSERT_TRUE(m_tcpConnection != 0);
    EXPECT_EQ(m_tcpConnection->getConnectionId(), tcpServerWorker->m_connId) << "The connection is generated by TCP server worker";   
    EXPECT_EQ(m_tcpConnection->getGlobalConnectionId(), tcpServerWorker->m_connId) << "The global connection should contain the TCP server worker id";  
    
    // set the new tcp socket nonblocking
    tcpServerWorker->addSocketListener(tcpServerWorker);
    m_tcpConnection->recvDataFromSocket();    
    
    // close the connection
    delete tcpClient;
    Thread::sleep(10);

    ASSERT_TRUE(tcpServerWorker->m_numOfBytesRecved == 0);
    ASSERT_TRUE(tcpServerWorker->m_socketUserData == m_tcpConnection->getConnectionId());
    m_tcpConnection->onConnectionClosed();
    delete m_tcpConnection;
    
    // change server socket back to blocking 
    m_tcpServer->addSocketListener(0);
    Thread::sleep(10);
}

TEST_F(TestTcpConnection, ReceiveAndSendAsync) {
    ASSERT_TRUE(m_tcpServer != 0);
    ASSERT_TRUE(m_tcpConnection == 0);
    ASSERT_GT(m_tcpServer->m_tcpServerSocket->getSocket(), 0);
    
    // set server socket nonblocking
    m_tcpServer->addSocketListener(m_tcpServer);
    m_tcpServer->accept();
    
    // client connect to server
    MockTcpClient* tcpClient = new MockTcpClient("127.0.0.1", 12345);
    ASSERT_TRUE(tcpClient->connect());
    
    Thread::sleep(10);   
    
    // server accept new tcp connection
    MockTcpServerWorker* tcpServerWorker =  m_tcpServer->m_tcpServerWorker;
    m_tcpConnection = tcpServerWorker->m_tcpConn;
    ASSERT_TRUE(m_tcpConnection != 0);
    ASSERT_TRUE(tcpServerWorker != 0);
    EXPECT_EQ(m_tcpConnection->getConnectionId(), tcpServerWorker->m_connId);   
    EXPECT_EQ(m_tcpConnection->getGlobalConnectionId(), tcpServerWorker->m_connId);      

    // set the new tcp socket nonblocking
    tcpServerWorker->addSocketListener(tcpServerWorker);
    m_tcpConnection->recvDataFromSocket();   
    
    // client send data to server
    string testData = "Test123456";
    int dataLength = testData.length();
    EXPECT_EQ(tcpClient->send(testData), dataLength);

    Thread::sleep(10); 
        
    // server receive data
    ASSERT_EQ(tcpServerWorker->m_numOfBytesRecved, dataLength);
    ASSERT_EQ(tcpServerWorker->m_socketUserData, m_tcpConnection->getConnectionId()); 
    m_tcpConnection->onDataReceived(tcpServerWorker->m_numOfBytesRecved);  
    
}