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
void testEpollSocketSet(string ip);
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
        string ip("127.0.0.1");
        if (argc > 2) {
            ip = argv[2];
        }
        testEpollSocketSet(ip);
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
void testEpollSocketSet(string ip) {
    Socket* socket = new Socket(ip, 8080);
    socket->bind();
    socket->listen();  
    socket->makeNonBlocking();

    EpollSocketSet epollSocketSet;
    epollSocketSet.registerInputHandler(socket, 0);

    EpollSocketSet::EpollSocket* epollSocket;
    cout << "start to poll the socket" << endl;
    while (true) {
        epollSocket = epollSocketSet.poll(0);

        if (epollSocket != 0 && (epollSocket->events & EPOLLIN)) {
            if (epollSocket->socket->getSocket() == socket->getSocket()) {
                Socket::InetAddressPort remoteAddrAndPort;
                int newSocketFd = -1;

                int result = epollSocket->socket->accept(newSocketFd, remoteAddrAndPort);
                assert(result != SKT_ERR);
                if (result == SKT_SUCC) {
                    Socket* newSocket = new Socket(newSocketFd);
                    newSocket->makeNonBlocking();
                    epollSocketSet.registerInputHandler(newSocket, 0);
                }
            } else {
                DataBuffer* recvBuf = new DataBuffer();
                int numOfBytesRecved = 0;

                int result = epollSocket->socket->recv(recvBuf->getEndOfDataPointer(), recvBuf->getSize() - recvBuf->getLength(), numOfBytesRecved);
                assert(result != SKT_ERR);
                if (result == SKT_SUCC) {
                    if (numOfBytesRecved == 0) {
                        cout << "socket is disconnected by peer: " << epollSocket->socket->getSocket() << endl;
                        // close socket in server side
                        epollSocketSet.removeInputHandler(epollSocket->socket);
                        epollSocket->socket->close();
                        delete epollSocket->socket;
                    } else {
                        recvBuf->increaseDataLength(numOfBytesRecved);
                        cout << "receive " << numOfBytesRecved << " bytes data from socket: " << recvBuf->getData() << endl;
                        cout << "buffer length: " << recvBuf->getLength() << endl;

                        char respData[] = "TCP server response";
                        int numOfBytesSent;
                        assert(SKT_SUCC == epollSocket->socket->send(respData, strlen(respData), numOfBytesSent));
                        assert(numOfBytesSent == strlen(respData));
                        cout << "send " << numOfBytesSent << " bytes data to socket: " << respData << endl;
                    }
                    recvBuf->reset();
                } else {
                    cout << "no data, continue to read from socket: " << epollSocket->socket->getSocket() << endl;
                }
            }
        } else {
            //cout << "no available read events" << endl;
            base::Thread::sleep(1);
        }
    }
}

// ---------------------------------------------
void testSocket(string ip) {
    Socket* socket = new Socket(ip, 8080);
    socket->bind();
    socket->listen();

    Socket::InetAddressPort remoteAddrAndPort;
    int newSocketFd = -1;

    // Test non-blocking socket
    socket->makeNonBlocking();
    while (true) {
        int result = socket->accept(newSocketFd, remoteAddrAndPort);
        assert(result != SKT_ERR);
        if (result == SKT_SUCC) {
            Socket* newSocket = new Socket(newSocketFd);
            newSocket->makeNonBlocking();
            DataBuffer* recvBuf = new DataBuffer();

            int numOfBytesRecved = 0;

            while (true) {
                int result = newSocket->recv(recvBuf->getEndOfDataPointer(), recvBuf->getSize() - recvBuf->getLength(), numOfBytesRecved);
                assert(result != SKT_ERR);
                if (result == SKT_SUCC) {
                    if (numOfBytesRecved == 0) {
                        newSocket->close();
                        delete newSocket;
                        newSocket = 0;
                        cout << "socket is disconnected by peer: " << endl;
                        break;
                    }

                    recvBuf->increaseDataLength(numOfBytesRecved);
                    cout << "receive " << numOfBytesRecved << " bytes data from socket: " << recvBuf->getData() << endl;
                    cout << "buffer length: " << recvBuf->getLength() << endl;

                    char respData[] = "TCP server response";
                    int numOfBytesSent;
                    assert(SKT_SUCC == newSocket->send(respData, strlen(respData), numOfBytesSent));
                    assert(numOfBytesSent == strlen(respData));
                    cout << "send " << numOfBytesSent << " bytes data to socket: " << respData << endl;

                    recvBuf->reset();
                } else {
                    base::Thread::sleep(1000);
                }
            }

            // stop the non-blocking test after one client connected complete
            break;
        } else {
            base::Thread::sleep(1000);
        }
    }

    // Test blocking socket
    socket->makeBlocking();
    int result = socket->accept(newSocketFd, remoteAddrAndPort);
    assert(result != SKT_ERR);
    if (result == SKT_SUCC) {
        Socket* newSocket = new Socket(newSocketFd);
        DataBuffer* recvBuf = new DataBuffer();

        int numOfBytesRecved = 0;

        while (true) {
            int result = newSocket->read(recvBuf->getEndOfDataPointer(), recvBuf->getSize() - recvBuf->getLength(), numOfBytesRecved);
            assert(result != SKT_ERR);
            if (result == SKT_SUCC) {
                if (numOfBytesRecved == 0) {
                    newSocket->close();
                    delete newSocket;
                    newSocket = 0;
                    cout << "socket is disconnected by peer: " << endl;
                    break;
                }

                recvBuf->increaseDataLength(numOfBytesRecved);
                cout << "receive " << numOfBytesRecved << " bytes data from socket: " << recvBuf->getData() << endl;
                cout << "buffer length: " << recvBuf->getLength() << endl;

                char respData[] = "TCP server response";
                int numOfBytesSent;
                assert(SKT_SUCC == newSocket->send(respData, strlen(respData), numOfBytesSent));
                assert(numOfBytesSent == strlen(respData));
                cout << "send " << numOfBytesSent << " bytes data to socket: " << respData << endl;

                recvBuf->reset();
            } else {
                base::Thread::sleep(1);
            }
        }
    }

}