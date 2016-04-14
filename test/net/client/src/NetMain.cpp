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
    cout << "  1 : Test Socket" << endl;
}

void testSocket(string ip, unsigned short port);

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
        string ip("127.0.0.1");
        if (argc > 2) {
            ip = argv[2];
        }
        testSocket(ip, 8080);
    } else if (testNumber.compare("2") == 0) {
        showUsage();
    } else if (testNumber.compare("3") == 0) {
        showUsage();
    } else {
        showUsage();
    }

    cout << "exit main " << getpid() << endl;
    return 0;
}

// ---------------------------------------------
void testSocket(string ip, unsigned short port) {
    Socket* socket = new Socket("null", 0);

    Socket::InetAddressPort remoteAddrAndPort;
    remoteAddrAndPort.port = port;
    Socket::getSockaddrByIpAndPort(&remoteAddrAndPort.addr, ip, port);

    socket->makeNonBlocking();
    while (true) {
        int result  = socket->connect(remoteAddrAndPort);
        assert(result != SKT_ERR);
        if (SKT_SUCC == result) {
            cout << "connect success!" << endl;

            base::Thread::sleep(100);

            char sendData[] = "TCP client request";
            int numberOfBytesSent;
            if (SKT_SUCC == socket->send(sendData, strlen(sendData), numberOfBytesSent)) {
                assert(numberOfBytesSent == strlen(sendData));
                cout << "send " << numberOfBytesSent << " bytes data complete" << endl;

                DataBuffer* response = new DataBuffer();
                int numOfBytesRecved = 0;

                while (true) {
                    result = socket->recv(response->getEndOfDataPointer(), response->getSize() - response->getLength(), numOfBytesRecved);
                    assert(result != SKT_ERR);
                    if (result == SKT_SUCC) {
                        assert(numOfBytesRecved != 0);
                        response->increaseDataLength(numOfBytesRecved);
                        cout << "recv " << numOfBytesRecved << " bytes data: " << response->getData() << endl;
                        response->reset();
                        break;
                    } else {
                        base::Thread::sleep(1);
                    }
                }
            }

            socket->close();
            break;
        } else {
            base::Thread::sleep(1);
        } 
    }
}