/*
 * IpcMain.cpp
 *
 *  Created on: Nov 19, 2015
 *      Author: z.j
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <iostream>
#include "PipeDemo.h"
#include "Pipe.h"

using namespace std;
using namespace ipc;

void sighandler(int signo);
int main(int argc, char* argv[]) {

    cout<<"Start Main Thread..."<< endl;

    //TODO ???
    if(signal(SIGPIPE,sighandler) == SIG_ERR)
    {
        cout << "signal error" << endl;
        exit(EXIT_FAILURE);
    }
    //signal(SIGCHLD, SIG_IGN);
    PipeDemo::registerSIGCHLDHandler(PipeDemo::waitChildExit);

    PipeDemo* demo = new PipeDemo();
    demo->demoPWCR();
    sleep(100);


    // Pipe* c2pPipe = new Pipe();
    // c2pPipe->init();
    // Pipe* p2cPipe = new Pipe();
    // p2cPipe->init();

    // pid_t pid;

    // pid = fork();

    // // Return 0 for child process, return actual pid of child process for parent process
    // if (-1 == pid) {
    //     cout << "Fail to fork." << endl;
    // } else if (0 == pid) {
    //     cout << "Running child process, return pid: " << pid << endl;
        
    //     char sendBuff[] = "1234567890";
    //     sleep(1);
    //     c2pPipe->setWrite();
    //     c2pPipe->send(sendBuff);
    //     //c2pPipe->closeAll();

    //     char recvBuff[256];
    //     p2cPipe->setRead();
    //     p2cPipe->setReadNonBlock();
    //     p2cPipe->setReadBlock();
    //     int result = p2cPipe->recv(recvBuff, sizeof(recvBuff));
        
    //     c2pPipe->send(sendBuff);
    //     exit(0);

    //     // char argv1[10];
    //     // sprintf(argv1, "%d", p2cPipe->getReadFd());
    //     // char argv2[10];
    //     // sprintf(argv2, "%d", c2pPipe->getWriteFd());
    //     // //cout << "read fd = " << argv1 << ", write fd = " << argv2 << endl;
    //     // execlp("/noname/demo/test/ipc/obj/pipesimulator", "pipesimulator", argv1, argv2);
    // } else {
    //     cout << "Running parent process, child process pid: " << pid << endl;
    //     // c2pPipe->closeAll();
    //     char recvBuff[256];
    //     c2pPipe->setRead();
    //     int result = c2pPipe->recv(recvBuff, sizeof(recvBuff));
    //     //sleep(10);
    //     // p2cPipe->setWrite();
    //     // p2cPipe->send(recvBuff);


    //     // // c2pPipe->recv(recvBuff, sizeof(recvBuff));
    // }

    // // while (1) {
    // //     sleep(1);
    // //     cout << pid << endl;
    // // }    

    return 0;

}

void sighandler(int signo)
{
    cout <<"catch a SIGPIPE signal and signum = " << signo << endl;
}

