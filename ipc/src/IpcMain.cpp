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
#include <string>

#include "PipeDemo.h"
#include "Pipe.h"
#include "NamedPipe.h"
#include "IPCLogger.h"
#include "common.h"
#include "Util.h"
#include "NamedPipeDemo.h"
#include "PosixThreadDemo.h"

using namespace std;
using namespace ipc;

void testPipe();
void testNamedPipe();
void testNamedPipe2();
void testPosixThread();

void showUsage() {
    cout << "Usage: " << endl;
    cout << "ipc [Test Number]" << endl;
    cout << "  1 : Test Pipe" << endl;
    cout << "  2 : Test Named Pipe (FIFO)" << endl;
    cout << "  3 : Test POSIX Thread" << endl;
}

int main(int argc, char* argv[]) {

    if (argc != 2) {
        showUsage();
        return 0;
    }

    string testNumber(argv[1]);
    cout << "testNumber = " << testNumber << endl;

    // IPCLogger::initConsoleLog();
    // IPCLogger::setLogLevel(logcpp::INFO);

    if (testNumber.compare("1") == 0) {
        testPipe();
    } else if (testNumber.compare("2") == 0) {
        testNamedPipe();
    } else if (testNumber.compare("3") == 0) {
        testPosixThread();
    } else {
        showUsage();
    }

    cout << "exit main " << getpid() << endl;
    return 0;
}

void testPosixThread() {
    // DemoThreadA* tha1 = new DemoThreadA();
    // DemoThreadA* tha2 = new DemoThreadA();
    // cout << "starting the threads ..." << endl;
    // tha1->start();
    // tha2->start();

    // cout << "this is main process " << tha1->getId() << ", " << tha2->getId() << endl;
    // cout << "this is main process " << tha1->getThreadId() << ", " << tha2->getThreadId() << endl;
    // tha1->wait();
    // tha2->wait();
    // // sleep(5);

    PosixThreadDemo ptd;
    ptd.demoAll();
}

// ------------------------------------

void testNamedPipe() {
    string pipeName = "/tmp/fifo";

    NamedPipeDemo npd;
    npd.demo_wopen_read_block_fifo_not_exist(pipeName, pipeName);

    npd.demo_ropen_block_fifo_exist(pipeName, pipeName);

    npd.demo_write_block(pipeName, pipeName);

    npd.demo_write_less_than_64k_block(pipeName, pipeName);

    npd.demo_write_catch_sigpipe(pipeName, pipeName);

    npd.open_fail_fifo_not_exist(pipeName);
}

void testNamedPipe2() {
    string pipeName = "/tmp/fifo";

    pid_t pid = fork();
    if (-1 == pid) {
        cout << "fail to fork" << endl;
    } else if (0 == pid) {
        NamedPipe * pipe = new NamedPipe(pipeName);
        pipe->initRead();

        int result = 0;
        while (true) {
            result = pipe->read();
            string data = pipe->getData();
            cout << "recv data(" << data.size() << "): " << data << endl;

            if (JPIPE_WRITE_CLOSE == result) {
                // break;
                sleep(1);
            }
        }
        cout << pipe->toString() << endl;
        // sleep(100);
        _exit(0);
    } else {
        NamedPipe * pipe = new NamedPipe(pipeName);
        pipe->initWrite();
        pipe->setData("12345678");

        for (int i=0; i<512*17; i++) {
            cout << i << endl;
            usleep(100);
            pipe->write();
        }
        sleep(10);
        pipe->close();
        sleep(5);
        pipe->initWrite();
        sleep(5);
        pipe->close();
        sleep(100);
        cout << pipe->toString() << endl;
    }
    

    // pipe->setData("aldfkjslkfjsdlfjlkdfjlsfjsdlfskldfkskfsfxxx");
    // string data = pipe->getData();
    // cout << "get data: " << data << endl;
    // cout << "pipe: " << pipe->toString() << endl;

    // delete pipe;
    // pipe = NULL;
    // cout << "get data: " << data << endl;
}

void testPipe() {
    PipeDemo* demo = new PipeDemo();
    PipeDemo::installSIGCHLDHandler(PipeDemo::handleSIGCHLD);
    PipeDemo::installSIGPIPEHandler(PipeDemo::handleSIGPIPE);
    //PipeDemo::ignoreSignal(SIGPIPE);

    demo->demoPWCR();
    cout << endl;

    demo->demoNWCR();
    cout << endl;

    demo->demoNRCW();
    cout << endl;

    PipeDemo::installSIGPIPEHandler(NULL);
    demo->demoNRCW();
    cout << endl;
    PipeDemo::installSIGPIPEHandler(PipeDemo::handleSIGPIPE);

    demo->demoNRPW();
    cout << endl;

    PipeDemo::installSIGPIPEHandler(NULL);
    demo->demoNRPW();
    cout << endl;
    PipeDemo::installSIGPIPEHandler(PipeDemo::handleSIGPIPE);


    // Test End
    PipeDemo::installSIGCHLDHandler(NULL);
    PipeDemo::installSIGPIPEHandler(NULL);

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
    //     c2pPipe->setWriteOnly();
    //     c2pPipe->send(sendBuff);
    //     //c2pPipe->closeAll();

    //     char recvBuff[256];
    //     p2cPipe->setReadOnly();
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
    //     c2pPipe->setReadOnly();
    //     int result = c2pPipe->recv(recvBuff, sizeof(recvBuff));
    //     //sleep(10);
    //     // p2cPipe->setWriteOnly();
    //     // p2cPipe->send(recvBuff);


    //     // // c2pPipe->recv(recvBuff, sizeof(recvBuff));
    // }

    // // while (1) {
    // //     sleep(1);
    // //     cout << pid << endl;
    // // }    
}