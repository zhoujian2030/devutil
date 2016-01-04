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
#include <sys/wait.h> 
#include <iostream>
#include "common.h"
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

void PipeDemo::ignoreSignal(int signum) {
    if (SIG_ERR == signal(signum, SIG_IGN)) {
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "ignore signal error.");
    }
}

void PipeDemo::installSIGCHLDHandler(sighandler_t handler) {
    if (signal(SIGCHLD, handler) == SIG_ERR) {
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "Install SIGCHLD handler error.");
    }
}

void PipeDemo::handleSIGCHLD(int signo) {
    LOG4CPLUS_INFO(_IPC_LOGGER_, "In Process " << getpid() <<": Catch a SIGCHLD signal and signo = " << signo);
    int status;
    pid_t pid;
    while((pid=waitpid(-1, &status, WNOHANG)) > 0) {
        LOG4CPLUS_INFO(_IPC_LOGGER_, "Child Process " << pid << " is exited, status = " << status);
    }

    LOG4CPLUS_INFO(_IPC_LOGGER_, "No more SIGCHLD signal.");
}

void PipeDemo::installSIGPIPEHandler(sighandler_t handler) {
    if (handler != NULL) {
        LOG4CPLUS_INFO(_IPC_LOGGER_, "Install SIGPIPE handler.");
    } else {
        LOG4CPLUS_INFO(_IPC_LOGGER_, "Uninstall SIGPIPE handler.");
    }
    if (signal(SIGPIPE, handler) == SIG_ERR) {
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "Install SIGPIPE handler error.");
    }
}

void PipeDemo::handleSIGPIPE(int signo) {
    LOG4CPLUS_INFO(_IPC_LOGGER_, "In Process " << getpid() <<": Catch a SIGPIPE signal and signo = " << signo);
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
        mP2CPipe->setReadOnly();
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": " << n++ 
            << " Block on reading from pipe");
        
        int result = mP2CPipe->recv(recvBuff, sizeof(recvBuff));
        if (JERROR == result) {
            LOG4CPLUS_ERROR(_IPC_LOGGER_, "In CHILD " << childPid << ": " << n++ 
                << " Fail to read data from pipe.");
        } else {
            LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": " << n++ 
                << " Read data: " << recvBuff);
        }
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

        // Install SIGCHLD handler to handle child process exit
        // PipeDemo::installSIGCHLDHandler(PipeDemo::handleSIGCHLD);

        LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": " << n++ 
            << " pid return from fork() should be child pid.");
        mP2CPipe->setWriteOnly();

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

        sleep(100);
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT "<< parentPid << ": sleep() could be interrupted by SIGCHLD signal from child process");
        sleep(3);
    }

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Demo End: " << getpid());
}

void PipeDemo::demoNWCR() {
    LOG4CPLUS_INFO(_IPC_LOGGER_, "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Demo Start: Parent and child close write fd, Child read, Block mode");

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

        char recvBuff[256];
        mP2CPipe->setReadOnly();
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": " << n++ 
            << " Block on reading from pipe");
        
        int result = mP2CPipe->recv(recvBuff, sizeof(recvBuff));
        if (JERROR == result) {
            LOG4CPLUS_ERROR(_IPC_LOGGER_, "In CHILD " << childPid << ": " << n++ 
                << " Fail to read data from pipe.");
        } else {
            LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": " << n++ 
                << " Read data: " << recvBuff);
        }

        LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": " << n++ 
            << " Read data complete, close all fds.");
        mP2CPipe->closeAll();
        delete mP2CPipe;
        mP2CPipe = NULL;

        // TODO 子进程应该怎么正常退出？
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": " << n++ << " _exit(0)");
        _exit(0);
    } else {
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": " << n++ 
            << " Sleep 2s and then close write fd.");
        sleep(2);
        mP2CPipe->setReadOnly();

        LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": " << n++ 
            << " Sleep 20s and then close all fds.");
        sleep(20);
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT "<< parentPid << ": sleep() could be interrupted by SIGCHLD signal from child process");

        mP2CPipe->closeAll();
        delete mP2CPipe;
        mP2CPipe = NULL;

        sleep(3);
    }  

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Demo End: " << getpid());  
}

void PipeDemo::demoNRCW() {
    LOG4CPLUS_INFO(_IPC_LOGGER_, "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Demo Start: Parent and child close read fd, Child write, Block mode");

    int n = 1;
    pid_t parentPid = getpid();
    LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": " << n++ 
        << " Create a pipe with default block mode");
    mC2PPipe = new Pipe();
    mC2PPipe->init();

    LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": " << n++ << " Fork a child process");
    pid_t pid = fork();

    // Return 0 for child process, return actual pid of child process for parent process
    if (-1 == pid) {
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "Fail to fork().");
    } else if (0 == pid) {
        pid_t childPid = getpid();

        LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": " << n++ 
            << " Close read fd, sleep 2s and then write to pipe.");

        mC2PPipe->setWriteOnly();
        char sendBuff[] = "**2b2b2b2b2b2b2b2b2b2b2b2b**";
        sleep(2);
        int result = mC2PPipe->send(sendBuff);

        if(JERROR == result) {
            LOG4CPLUS_ERROR(_IPC_LOGGER_, "In CHILD " << childPid << ": " << n++ 
                << " Fail to send data to pipe, result = " << result);
        } else {      
            LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": " << n++ 
                << " Send data success.");
        }

        LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": " << n++ 
            << " Send data complete, close all fds.");

        mC2PPipe->closeAll();
        delete mC2PPipe;
        mC2PPipe = NULL;

        // TODO 子进程应该怎么正常退出？
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": " << n++ << " _exit(0)");
        _exit(0);
    } else {
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": " << n++ 
            << " Close read fd.");
        mC2PPipe->setWriteOnly();

        LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": " << n++ 
            << " Sleep 20s and then close all fds.");
        sleep(20);

        LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT "<< parentPid << ": sleep() could be interrupted by SIGCHLD signal from child process");
        mC2PPipe->closeAll();
        delete mC2PPipe;
        mC2PPipe = NULL;

        sleep(3);
    }  

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Demo End: " << getpid());      
}

void PipeDemo::demoNRPW() {
    LOG4CPLUS_INFO(_IPC_LOGGER_, "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Demo Start: Parent and child close read fd, Parent write, Block mode");

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
            << " Close read fd.");

        mP2CPipe->setWriteOnly();

        LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": " << n++ 
            << " Sleep 3s and then close all fds.");
        sleep(3);

        mP2CPipe->closeAll();
        delete mP2CPipe;
        mP2CPipe = NULL;

        LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": " << n++ << " _exit(0)");
        _exit(0);
    } else {
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": " << n++ 
            << " Close read fd, sleep 2s then send data to pipe.");
        mP2CPipe->setWriteOnly();

        char sendBuff[] = "**2b2b2b2b2b2b2b2b2b2b2b2b**";
        sleep(2);
        int result = mP2CPipe->send(sendBuff);

        if(JERROR == result) {
            LOG4CPLUS_ERROR(_IPC_LOGGER_, "In PARENT " << parentPid << ": " << n++ 
                << " Fail to send data to pipe, result = " << result);
        } else {      
            LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": " << n++ 
                << " Send data success.");
        }

        LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": " << n++ 
            << " Send data complete, close all fds.");

        mP2CPipe->closeAll();
        delete mP2CPipe;
        mP2CPipe = NULL;

        // LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT "<< parentPid << ": sleep() could be interrupted by SIGCHLD signal from child process");

        sleep(3);
    }  

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Demo End: " << getpid());      
}
