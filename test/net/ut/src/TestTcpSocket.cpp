/*
 * TestTcpSocket.cpp
 *
 *  Created on: June 13, 2016
 *      Author: z.j
 */

#include "TestTcpSocket.h"
#include "MockTcpClient.h"
#include "SysctlUtil.h"
#include <iostream>

using namespace net;
using namespace cm;
using namespace std;

TEST_F(TestTcpSocket, BlockingIO_CloseSocketInServerSide) {
    ASSERT_TRUE(m_tcpServer != 0);
    ASSERT_GT(m_tcpServer->m_tcpServerSocket->getSocket(), 0);

    // client connect to server
    MockTcpClient* tcpClient = new MockTcpClient("127.0.0.1", 12345);
    EXPECT_TRUE(tcpClient->connect());

    // server accept the connection
    m_tcpSocket = m_tcpServer->accept();
    ASSERT_TRUE(m_tcpSocket != 0);

    // client send data to server
    string testData("Test Data");
    int expectLength = testData.length();
    tcpClient->send(testData);
    Thread::sleep(2);

    // server receive data from socket
    ASSERT_EQ(m_tcpSocket->receive(m_buffer->getEndOfDataPointer(), m_buffer->getRemainBufferSize()), expectLength);
    m_buffer->reset();

    // client close connection
    delete tcpClient;
    Thread::sleep(1);
    ASSERT_TRUE(m_tcpSocket->receive(m_buffer->getEndOfDataPointer(), m_buffer->getRemainBufferSize()) == 0);
    m_tcpSocket->close();
    ASSERT_TRUE(m_tcpSocketListener->m_closeCount == 0);
    ASSERT_TRUE(m_tcpSocket->getSocket() == -1);

    // server close again the TcpSocket after it is closed
    m_tcpSocket->close();
    ASSERT_TRUE(m_tcpSocketListener->m_closeCount == 0);
    ASSERT_TRUE(m_tcpSocket->getSocket() == -1);
    delete m_tcpSocket;
}

TEST_F(TestTcpSocket, NonblockingIO_CloseSocketInServerSide) {
    ASSERT_TRUE(m_tcpServer != 0);
    ASSERT_GT(m_tcpServer->m_tcpServerSocket->getSocket(), 0);

    // client connect to server
    MockTcpClient* tcpClient = new MockTcpClient("127.0.0.1", 12345);
    EXPECT_TRUE(tcpClient->connect());

    // server accept the connection
    m_tcpSocket = m_tcpServer->accept();
    ASSERT_TRUE(m_tcpSocket != 0);

    // server register EPOLLIN event to epoll
    m_tcpSocket->addSocketListener(m_tcpSocketListener);
    ASSERT_EQ(m_tcpSocket->receive(m_buffer->getEndOfDataPointer(), m_buffer->getRemainBufferSize()), 0);

    // client send data to server
    string testData("Test Data");
    int expectLength = testData.length();
    tcpClient->send(testData);
    Thread::sleep(3);
    ASSERT_TRUE(m_tcpSocketListener->m_recvDataLength == expectLength);
    m_buffer->reset();
    ASSERT_EQ(m_tcpSocket->receive(m_buffer->getEndOfDataPointer(), m_buffer->getRemainBufferSize()), 0);

    // client close connection
    delete tcpClient;
    Thread::sleep(3);
    ASSERT_TRUE(m_tcpSocketListener->m_recvDataLength == 0);
    m_tcpSocket->close();
    ASSERT_TRUE(m_tcpSocketListener->m_closeCount == 1);
    ASSERT_TRUE(m_tcpSocket->getSocket() == -1);
    m_tcpSocketListener->m_closeCount = 0;

    // server close again the TcpSocket after it is closed
    m_tcpSocket->close();
    ASSERT_TRUE(m_tcpSocketListener->m_closeCount == 0);
    ASSERT_TRUE(m_tcpSocket->getSocket() == -1);
    delete m_tcpSocket;
}

TEST_F(TestTcpSocket, NonblockingIO_SendBufferFull) {
    ASSERT_TRUE(m_tcpServer != 0);
    ASSERT_GT(m_tcpServer->m_tcpServerSocket->getSocket(), 0);

    // client connect to server
    MockTcpClient* tcpClient = new MockTcpClient("127.0.0.1", 12345);
    cout << "The recv buffer size of client: " << tcpClient->m_tcpSocket->getRecvBufferSize() << endl;
    EXPECT_TRUE(tcpClient->connect());

    // server accept the connection
    m_tcpSocket = m_tcpServer->accept();
    ASSERT_TRUE(m_tcpSocket != 0);
    int currentSendBufferSize = m_tcpSocket->getSendBufferSize();
    cout << "The send buffer size of server: " << currentSendBufferSize << endl;

    // server set the new connection socket nonblocking and test
    m_tcpSocket->makeNonBlocking();
    ASSERT_EQ(m_tcpSocket->receive(m_buffer->getEndOfDataPointer(), m_buffer->getRemainBufferSize()), -1);
       
    string testData;
    testData.clear();
    for (int i=0; i<currentSendBufferSize; ++i) {
        testData.append("T");
    }
    testData.append("END");
    const char* sendBuff = testData.c_str();
    int dataLength = testData.length();
    m_tcpSocket->addSocketListener(m_tcpSocketListener);
    Thread::sleep(5);
    EXPECT_LT(m_tcpSocket->send(sendBuff, dataLength), dataLength);

    Thread::sleep(100);

    // // client recv data
    // string recvData = tcpClient->receive(); 
    // EXPECT_TRUE(recvData == testData);
}
