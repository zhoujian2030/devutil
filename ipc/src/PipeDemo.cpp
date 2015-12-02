/*
 * PipeDemo.cpp
 *
 *  Created on: Dec 01, 2015
 *      Author: z.j
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <iostream>
#include "PipeDemo.h"

using namespace ipc;
using namespace std;

PipeDemo::PipeDemo() {
    //mC2PPipe = new Pipe();
    //c2pPipe->init();
}

PipeDemo::~PipeDemo() {

}

void PipeDemo::demoPWCR() {
    cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "Demo Start: \n\tParent write to pipe \n\tChild read from pipe\n\tBlock mode" << endl;
    cout << endl;

    int n = 1;
    pid_t parentPid = getpid();
    cout << "[Parent: " << parentPid << "]" << n++ << " Create a pipe with default block mode"<< endl;
    mP2CPipe = new Pipe();
    mP2CPipe->init();

    cout << "[Parent: " << parentPid << "]" << n++ << " Fork a child process"<< endl;
    pid_t pid = fork();

    // Return 0 for child process, return actual pid of child process for parent process
    if (-1 == pid) {
        cout << "Fail to fork." << endl;
    } else if (0 == pid) {
        pid_t childPid = getpid();
        cout << "[Child: " << childPid << "]" << n++ << " pid return from fork() should be 0"<< endl;

        char recvBuff[256];
        cout << "[Child: " << childPid << "]" << n++ << " Close write fd"<< endl;
        mP2CPipe->setRead();
        cout << "[Child: " << childPid << "]" << n++ << " Block on reading from pipe"<< endl;
        int result = mP2CPipe->recv(recvBuff, sizeof(recvBuff));
        cout << "[Child: " << childPid << "]" << n++ << " Read data: " << recvBuff << endl;

        cout << "[Child: " << childPid << "]" << n++ << " Close all fds"<< endl;
        mP2CPipe->closeAll();
        delete mP2CPipe;
        mP2CPipe = NULL;

        // TODO 子进程应该怎么正常退出？
        cout << "[Child: " << childPid << "]" << n++ << " _exit(0). All fds wil be closed even they are not close mannually."<< endl;
        _exit(0);
    } else {
        cout << "[Parent: " << parentPid << "]" << n++ << " pid return from fork() should be child pid "<< pid << endl;

        cout << "[Parent: " << parentPid << "]" << n++ << " Close read fd"<< endl;
        mP2CPipe->setWrite();

        char sendBuff[] = "**2b2b2b2b2b2b2b2b2b2b2b2b**";
        cout << "[Parent: " << parentPid << "]" << n++ << " Delay 2s then write data: " << sendBuff << endl;
        sleep(2);
        mP2CPipe->send(sendBuff);

        cout << "[Parent: " << parentPid << "]" << n++ << " Close all fds" << endl;
        mP2CPipe->closeAll();
        delete mP2CPipe;
        mP2CPipe = NULL;

        sleep(1);
    }

    cout << "\nDemo End: " << getpid() << endl;
}