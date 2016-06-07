/*
 * TestTcpConnection.h
 *
 *  Created on: June 03, 2016
 *      Author: z.j
 */
 
#ifndef TEST_TCPCONNECTION_H
#define TEST_TCPCONNECTION_H

#include "TestSuite.h"
#include "TcpConnection.h"
#include "MockTcpServer.h"
#include "MockTcpServerWorker.h"
#include "MockTcpServerUser.h"

class TcpServerWorkerMock;
class TestTcpConnection : public TestSuite {
protected:
    virtual void SetUp() {
        std::cout << "TestTcpConnection::SetUp()" << std::endl;
        cm::Worker::initialize(1);
        net::Reactor::initialize(1);
        m_tcpServerUser = new MockTcpServerUser();
        m_tcpServer = new MockTcpServer(m_tcpServerUser, 12345, "127.0.0.1");
        m_tcpConnection = 0;
    }
    
    virtual void TearDown() {
        std::cout << "TestTcpConnection::TearDown()" << std::endl;
        delete m_tcpServer;
        delete m_tcpServerUser;
    }    
    
    MockTcpServer* m_tcpServer;
    net::TcpConnection* m_tcpConnection;
    MockTcpServerUser* m_tcpServerUser;
};

#endif