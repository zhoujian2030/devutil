/*
 * TestTcpSocket.h
 *
 *  Created on: June 13, 2016
 *      Author: z.j
 */

#ifndef TEST_TCPSOCKET_H
#define TEST_TCPSOCKET_H

#include "TestSuite.h"
#include "TcpSocket.h"
#include "MockTcpServer.h"
#include "MockTcpSocketListener.h"
#include "DataBuffer.h"

class TestTcpSocket : public TestSuite {
protected:
    virtual void SetUp() {
        std::cout << "TestTcpSocket::SetUp()" << std::endl;
        m_tcpServer =  new MockTcpServer(0, 12345, "127.0.0.1");
        m_tcpSocketListener = new MockTcpSocketListener();
        m_buffer = new cm::DataBuffer();
    }
    
    virtual void TearDown() {
        std::cout << "TestTcpSocket::TearDown()" << std::endl;
        delete m_tcpServer;
        delete m_tcpSocketListener;
        delete m_buffer;
    }

    MockTcpServer* m_tcpServer;
    MockTcpSocketListener* m_tcpSocketListener;
    cm::DataBuffer* m_buffer;
    net::TcpSocket* m_tcpSocket;
};

#endif
