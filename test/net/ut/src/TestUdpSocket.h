/*
 * TestUdpSocket.h
 *
 *  Created on: Nov 1, 2016
 *      Author: j.zhou
 */

#ifndef TEST_UDPSOCKET_H
#define TEST_UDPSOCKET_H

#include "TestSuite.h"
#include "UdpSocket.h"
#include "DataBuffer.h"

class TestUdpSocket : public TestSuite {
protected:
    virtual void SetUp() {
        std::cout << "TestUdpSocket::SetUp()" << std::endl;
        m_buffer = new cm::DataBuffer();
    }
    
    virtual void TearDown() {
        std::cout << "TestUdpSocket::TearDown()" << std::endl;
        delete m_buffer;
    }

    cm::DataBuffer* m_buffer;
    net::UdpSocket* m_udpServerSocket;
    net::UdpSocket* m_udpClientSocket;
};

#endif
