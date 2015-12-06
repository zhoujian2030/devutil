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
#include "IPCLogger.h"
#include "PipeDemo.h"

using namespace ipc;
using namespace logcpp;


PipeDemo::PipeDemo() {
    IPCLogger::initConsoleLog();
    IPCLogger::setLogLevel(DEBUG);
    //mC2PPipe = new Pipe();
    //c2pPipe->init();
}

PipeDemo::~PipeDemo() {

}

void PipeDemo::demoPWCR() {
    LOG4CPLUS_INFO(_IPC_LOGGER_, "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Demo Start: Parent write pipe, Child read pipe, Block mode");

    int n = 1;
    pid_t parentPid = getpid();
    LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": " << n++ 
        << " Create a pipe with default block mode");
    mP2CPipe = new Pipe();
    mP2CPipe->init();

    LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": " << n++ << " Fork a child process");
    pid_t pid = fork();

    // Return 0 for child process, return actual pid of child process for parent process
    if (-1 == pid) {
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "Fail to fork().");
    } else if (0 == pid) {
        pid_t childPid = getpid();
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": " << n++ 
            << " pid return from fork() should be 0");

        char recvBuff[256];
        mP2CPipe->setRead();
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": " << n++ 
            << " Block on reading from pipe");
        
        int result = mP2CPipe->recv(recvBuff, sizeof(recvBuff));
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": " << n++ << " Read data: " << recvBuff);

        LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": " << n++ 
            << " Read data complete, close all fds.");
        mP2CPipe->closeAll();
        delete mP2CPipe;
        mP2CPipe = NULL;

        // TODO 子进程应该怎么正常退出？
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": " << n++ 
            << " _exit(0). All fds in this process be closed even not closed mannually.");
        _exit(0);
    } else {
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": " << n++ 
            << " pid return from fork() should be child pid.");
        mP2CPipe->setWrite();

        char sendBuff[] = "**2b2b2b2b2b2b2b2b2b2b2b2b**";
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": " << n++ 
            << " Delay 2s and then write data: " << sendBuff);
        sleep(2);
        mP2CPipe->send(sendBuff);

        LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": " << n++ 
            << " Write complete, close all fds.");
        mP2CPipe->closeAll();
        delete mP2CPipe;
        mP2CPipe = NULL;

        sleep(1);
    }

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Demo End: " << getpid());
}