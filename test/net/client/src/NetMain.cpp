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
        string ip("127.0.0.1");
        if (argc > 2) {
            ip = argv[2];
        }
        testSocket(ip);
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
void testSocket(string ip) {


}