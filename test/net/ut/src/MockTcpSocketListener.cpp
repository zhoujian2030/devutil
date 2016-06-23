/*
 * MockTcpSocketListener.h
 *
 *  Created on: June 13, 2016
 *      Author: z.j
 */

#include "MockTcpSocketListener.h"
#include <iostream>

using namespace net;
using namespace std;

// -----------------------------------------------
void MockTcpSocketListener::handleRecvResult(net::TcpSocket* theSocket, int numOfBytesRecved) {

}

// -----------------------------------------------
void MockTcpSocketListener::handleSendResult(net::TcpSocket* theSocket, int numOfBytesSent) {
    cout << "Send " << numOfBytesSent << " data" << endl;
    //TODO
}