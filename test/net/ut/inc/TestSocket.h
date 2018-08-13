/*
 * TestSocket.h
 *
 *  Created on: June 12, 2016
 *      Author: z.j
 */
 
#ifndef TEST_SOCKET_H
#define TEST_SOCKET_H

#include "TestSuite.h"
#include "Socket.h"

class TestSocket : public TestSuite {
protected:
    virtual void SetUp() {
        std::cout << "TestSocket::SetUp()" << std::endl;
        m_socket = new net::Socket("127.0.0.1", 12345);
    }
    
    virtual void TearDown() {
        std::cout << "TestSocket::TearDown()" << std::endl;
        delete m_socket;
    }    
    
    net::Socket* m_socket;
};

#endif
