/*
 * NamedPipeDemo.cpp
 *
 *  Created on: Jan 12, 2016
 *      Author: z.j
 */

#include "NamedPipeDemo.h"
#include "NamedPipe.h"
#include "IPCLogger.h"
#include "common.h"

using namespace ipc;
using namespace std;

NamedPipeDemo::NamedPipeDemo() {
    IPCLogger::initConsoleLog();
    IPCLogger::setLogLevel(logcpp::DEBUG);    
}

void NamedPipeDemo::demo(string wPathName, string rPathName) {
    LOG4CPLUS_INFO(_IPC_LOGGER_, "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Demo Start: Block mode, one process(P) write, one process(C) read");

    pid_t parentPid = getpid();
    LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": Fork a child process");

    pid_t pid = fork();
    if (-1 == pid) {
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "fail to fork.");
    } else if (0 == pid) {
        pid_t childPid = getpid();

        LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": block on open fifo with O_RDONLY mode"
            << ", fifo name = " << rPathName);
        m_pNamedPipe = new NamedPipe(rPathName);
        m_pNamedPipe->initRead();

        LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": block on read fifo");
        int result = m_pNamedPipe->read();

        if (JSUCCESS == result) {
            string recvData = m_pNamedPipe->getData();
            LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": read fifo success, data = " << recvData);
        }

        LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": sleep 3s then exit");
        sleep(3);
        delete m_pNamedPipe;
        m_pNamedPipe = NULL;
        _exit(0);
    } else {
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": sleep 3s then create and "
            << "open fifo with O_WRONLY mode, fifo name = " << wPathName);

        sleep(3);
        m_pNamedPipe = new NamedPipe(wPathName);
        m_pNamedPipe->initWrite();

        string sendData("1234567890987654321");
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": sleep 2s then write data to fifo: " << sendData);
        sleep(2);
        m_pNamedPipe->setData(sendData);
        int result = m_pNamedPipe->write();

        if (JERROR != result) {
            LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": write fifo success");
        }
    }

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Demo End: " << getpid());
}