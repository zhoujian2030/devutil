/*
 * TestUdpSocket.cpp
 *
 *  Created on: Nov 1, 2016
 *      Author: j.zhou
 */

#include "TestUdpSocket.h"
#include "SysctlUtil.h"
#include <iostream>

using namespace net;
using namespace cm;
using namespace std;

TEST_F(TestUdpSocket, BlockingIO_NormalTest) {
    // create udp server and client socket
    //net::Reactor::initialize(1);
    m_udpServerSocket = new UdpSocket("127.0.0.1", 12345);
    m_udpClientSocket = new UdpSocket();
    Socket::InetAddressPort serverAddress;
    serverAddress.addr = m_udpServerSocket->getLocalAddress();
    serverAddress.port = 12345;

    // send data from client to server
    string testData("client hello");
    int expectLength = testData.length();
    m_buffer = new DataBuffer(testData);
    cout << ">>>>> client send data: " << testData << endl;
    ASSERT_EQ(m_udpClientSocket->send(m_buffer->getStartOfDataPointer(), m_buffer->getLength(), serverAddress), expectLength);
    Thread::sleep(2);

    // server receive data from socket
    m_buffer->reset();
    Socket::InetAddressPort remoteAddress;
    ASSERT_EQ(m_udpServerSocket->receive(m_buffer->getEndOfDataPointer(), m_buffer->getRemainBufferSize(), remoteAddress), expectLength);
    m_buffer->increaseDataLength(expectLength);
    cout << "<<<<< server recv data: " << m_buffer->getData() << endl;

    // close
    delete m_udpServerSocket;
    delete m_udpClientSocket;
    ASSERT_EQ(m_udpClientSocket->send(m_buffer->getStartOfDataPointer(), m_buffer->getLength(), serverAddress), -1);
    ASSERT_EQ(m_udpServerSocket->receive(m_buffer->getEndOfDataPointer(), m_buffer->getRemainBufferSize(), remoteAddress), -1);
    cout << "<<<<< close socket and return error from send or receive" << endl;

    delete m_buffer;
}

TEST_F(TestUdpSocket, NonBlockingIO_NormalTest) {
    // create udp server and client socket
    //net::Reactor::initialize(1);
    m_udpServerSocket = new UdpSocket("127.0.0.1", 9999);
    m_udpClientSocket = new UdpSocket();
    Socket::InetAddressPort serverAddress;
    serverAddress.addr = m_udpServerSocket->getLocalAddress();
    serverAddress.port = 9999;

    // send data from client to server
    string testData("client hello");
    int expectLength = testData.length();
    m_buffer = new DataBuffer(testData);
    cout << ">>>>> client send data: " << testData << endl;
    ASSERT_EQ(m_udpClientSocket->send(m_buffer->getStartOfDataPointer(), m_buffer->getLength(), serverAddress), expectLength);
    Thread::sleep(2);

    // server receive data from socket
    m_buffer->reset();
    Socket::InetAddressPort remoteAddress;
    ASSERT_EQ(m_udpServerSocket->receive(m_buffer->getEndOfDataPointer(), m_buffer->getRemainBufferSize(), remoteAddress), 
        expectLength);
    m_buffer->increaseDataLength(expectLength);
    cout << "<<<<< server recv data: " << m_buffer->getData() << endl;

    // change socket to non-blocking
    m_udpServerSocket->makeNonBlocking();    
    ASSERT_EQ(m_udpServerSocket->receive(m_buffer->getEndOfDataPointer(), m_buffer->getRemainBufferSize(), remoteAddress), 0);
    cout << "<<<<< server socket changed to non-blocking, and recv no data from socket without blocking" << endl;

    // client send data again
    cout << ">>>>> client send data again: " << m_buffer->getData() << endl;
    ASSERT_EQ(m_udpClientSocket->send(m_buffer->getStartOfDataPointer(), m_buffer->getLength(), serverAddress), expectLength);
    Thread::sleep(2);

    // server receive data again
    m_buffer->reset();
    ASSERT_EQ(m_udpServerSocket->receive(m_buffer->getEndOfDataPointer(), m_buffer->getRemainBufferSize(), remoteAddress), expectLength);
    m_buffer->increaseDataLength(expectLength);
    cout << "<<<<< server recv data: " << m_buffer->getData() << endl;

    delete m_udpServerSocket;
    delete m_udpClientSocket;
    ASSERT_EQ(m_udpClientSocket->send(m_buffer->getStartOfDataPointer(), m_buffer->getLength(), serverAddress), -1);
    ASSERT_EQ(m_udpServerSocket->receive(m_buffer->getEndOfDataPointer(), m_buffer->getRemainBufferSize(), remoteAddress), -1);
    cout << "<<<<< close socket and return error from send or receive" << endl;
}
