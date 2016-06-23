/*
 * TestTcpServerSocket.h
 *
 *  Created on: June 13, 2016
 *      Author: z.j
 */

#ifndef TEST_TCPSERVERSOCKET_H
#define TEST_TCPSERVERSOCKET_H

#include "TestSuite.h"
#include "TcpServerSocket.h"

class TestTcpServerSocket : public TestSuite {
protected:
    virtual void SetUp() {
        std::cout << "TestTcpServerSocket::SetUp()" << std::endl;

    }
    
    virtual void TearDown() {
        std::cout << "TestTcpServerSocket::TearDown()" << std::endl;

    }

    net::TcpServerSocket* m_tcpServerSocket;
};

#endif
