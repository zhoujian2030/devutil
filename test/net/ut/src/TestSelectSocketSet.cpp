/*
 * TestSelectSocketSet.cpp
 *
 *  Created on: Nov 4, 2016
 *      Author: j.zhou
 */

#include "TestSelectSocketSet.h"
#include "UdpSocket.h"
#include "DataBuffer.h"
#include <iostream>

using namespace net;
using namespace cm;
using namespace std;

//--gtest_filter=TestSelectSocketSet.*

TEST_F(TestSelectSocketSet, Normal_Select_UDP) {
    // create test targets
    m_selectSocketSet = new SelectSocketSet();
    UdpSocket* serverSocket = new UdpSocket("127.0.0.1", 12345);
    UdpSocket* clientSocket = new UdpSocket();
    Socket::InetAddressPort serverAddress;
    serverAddress.addr = serverSocket->getLocalAddress();
    serverAddress.port = 12345;
    m_selectSocketSet->registerInputHandler(serverSocket, 0);

    // send data from client to server
    string testData("client hello");
    int expectLength = testData.length();
    DataBuffer* buffer = new DataBuffer(testData);
    cout << ">>>>> client send data: " << testData << endl;
    ASSERT_EQ(clientSocket->send(buffer->getStartOfDataPointer(), buffer->getLength(), serverAddress), expectLength);
    Thread::sleep(2);

    // select socket
    SelectSocketSet::SelectSocket* readySockets = (SelectSocketSet::SelectSocket*)m_selectSocketSet->poll(1000);
    cout << "<<<<< select socket return" << endl;
    UdpSocket* availableSocket = (UdpSocket*)readySockets->socket;
    int events = readySockets->events;
    readySockets++;
    ASSERT_TRUE(availableSocket != 0);
    ASSERT_TRUE(readySockets != 0);
    ASSERT_EQ(availableSocket, serverSocket);
    ASSERT_EQ(events, SelectSocketSet::SELECT_R_EV);
    ASSERT_EQ(readySockets->events, 0);

    // server receive data from socket
    buffer->reset();
    Socket::InetAddressPort remoteAddress;
    ASSERT_EQ(availableSocket->receive(buffer->getEndOfDataPointer(), buffer->getRemainBufferSize(), remoteAddress), expectLength);
    buffer->increaseDataLength(expectLength);
    cout << "<<<<< server recv data: " << buffer->getData() << endl;

    readySockets = (SelectSocketSet::SelectSocket*)m_selectSocketSet->poll(300);
    cout << "<<<<< select socket timeout" << endl;
    ASSERT_TRUE(readySockets != 0);
    ASSERT_EQ(readySockets->events, 0);

    delete m_selectSocketSet;
    delete serverSocket;
    delete clientSocket;
    delete buffer;
}