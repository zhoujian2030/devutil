/*
 * main.cpp
 *
 *  Created on: Nov 19, 2015
 *      Author: z.j
 */
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <iostream>
#include "Pipe.h"

using namespace std;
using namespace ipc;

int main(int argc, char* argv[]) {

    cout << "Start PIPE Simulator."<< endl;
  
    // TODO print usage
    int readFd = 0, writeFd = 0;

    if (argc < 2) {
        cout << "At least one argument is needed." << endl;
        return -1;
    }

    if (2 == argc) {
        readFd = atoi(argv[1]);
        cout << "Read fd: " << readFd << endl;
    }

    if (3 == argc) {
        readFd = atoi(argv[1]);
        writeFd = atoi(argv[2]);
        cout << "Read fd: " << readFd << ", Write fd: " << writeFd << endl;
    }

    Pipe* pPipe = new Pipe(readFd, writeFd);

    char sendBuff[] = "123456789012345";
    pPipe->send(sendBuff);

    char recvBuff[256];
    int result = pPipe->recv(recvBuff, sizeof(recvBuff));

    return 0;

}

