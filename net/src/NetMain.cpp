/*
 * NetMain.cpp
 *
 *  Created on: Apr 03, 2016
 *      Author: z.j
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <iostream>
#include <string>

#include "NetLogger.h"
#include "ReactorThread.h"
#include "EpollSocketSet.h"
#include "Socket.h"
#include "DataBuffer.h"


using namespace std;
using namespace net;


void showUsage() {
    cout << "Usage: " << endl;
    cout << "net [Test Number]" << endl;
    cout << "  1 : Test ReactorThread" << endl;
    cout << "  2 : Test EpollSocketSet" << endl;
    cout << "  3 : Test Socket" << endl;
}

void testReactorThread();
void testEpollSocketSet();
void testSocket(string ip);

int main(int argc, char* argv[]) {

    if (argc < 2) {
        showUsage();
        return 0;
    }

    string testNumber(argv[1]);
    cout << "testNumber = " << testNumber << endl;

    // NETLogger::initConsoleLog();
    // NETLogger::setLogLevel(logcpp::INFO);

    if (testNumber.compare("1") == 0) {
        testReactorThread();
    } else if (testNumber.compare("2") == 0) {
        testEpollSocketSet();
    } else if (testNumber.compare("3") == 0) {
        string ip("127.0.0.1");
        if (argc > 2) {
            ip = argv[2];
        }
        testSocket(ip);
    } else {
        showUsage();
    }

    cout << "exit main " << getpid() << endl;
    return 0;
}

// ---------------------------------------------
void testReactorThread() {
    ReactorThread *pReactorThread = new ReactorThread();
    pReactorThread->start();
    pReactorThread->wait();
}

// ---------------------------------------------
void testEpollSocketSet() {
    EpollSocketSet epollSocketSet;
}

// ---------------------------------------------
void testSocket(string ip) {
    Socket* socket = new Socket(ip, 8080);
    socket->bind();
    socket->listen();

    Socket::InetAddressPort remoteAddrAndPort;
    int newSocketFd = -1;

    if (socket->accept(remoteAddrAndPort, newSocketFd)) {
        Socket* newSocket = new Socket(newSocketFd);
        DataBuffer* recvBuf = new DataBuffer();

        int numOfBytesRecved = 0;

        while (true) {
            if (newSocket->recv(recvBuf->getEndOfDataPointer(), recvBuf->getSize() - recvBuf->getLength(), numOfBytesRecved)) {
                if (numOfBytesRecved == 0) {
                    cout << "socket is disconnected by peer: " << endl;
                    break;
                }

                recvBuf->increaseDataLength(numOfBytesRecved);
                cout << "receive " << numOfBytesRecved << " bytes data from socket: " << recvBuf->getData() << endl;
                cout << "buffer length: " << recvBuf->getLength() << endl;

                recvBuf->reset();
            } else {
                break;
            }
        }
    }
}