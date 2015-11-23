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
#include "JPipe.h"

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

    JPipe* c2pPipe = new JPipe();
    c2pPipe->init();
    JPipe* p2cPipe = new JPipe();
    p2cPipe->init();

    pid_t pid;

    pid = fork();

    // Return 0 for child process, return actual pid of child process for parent process
    if (-1 == pid) {
        cout << "Fail to fork." << endl;
    } else if (0 == pid) {
        cout << "Child pid: " << pid << endl;
        char sendBuff[] = "1234567890";
        sleep(1);
        c2pPipe->setWrite();
        c2pPipe->send(sendBuff);
        //c2pPipe->closeAll();

        char recvBuff[256];
        p2cPipe->setRead();
        int result = p2cPipe->recv(recvBuff, sizeof(recvBuff));
        
        //c2pPipe->send(sendBuff);
        exit(0);
    } else {
        cout << "Parent pid: " << pid << endl;
        // c2pPipe->closeAll();
        char recvBuff[256];
        c2pPipe->setRead();
        int result = c2pPipe->recv(recvBuff, sizeof(recvBuff));

        p2cPipe->setWrite();
        p2cPipe->send(recvBuff);


        c2pPipe->recv(recvBuff, sizeof(recvBuff));
    }

    // while (1) {
    //     sleep(1);
    //     cout << pid << endl;
    // }    

    return 0;

}

void sighandler(int signo)
{
    cout <<"catch a SIGPIPE signal and signum = " << signo << endl;
}

