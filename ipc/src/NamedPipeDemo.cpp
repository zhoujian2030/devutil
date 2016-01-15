/*
 * NamedPipeDemo.cpp
 *
 *  Created on: Jan 12, 2016
 *      Author: z.j
 */

#include <stdlib.h>
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

void NamedPipeDemo::demo_wopen_read_block_fifo_not_exist(string wPathName, string rPathName) {
    if (wPathName.compare(rPathName) != 0) {
        LOG4CPLUS_INFO(_IPC_LOGGER_, "Run demo_wopen_read_block_fifo_not_exist Test Error.");
        return;
    }

    LOG4CPLUS_INFO(_IPC_LOGGER_, "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Demo Start: Block mode, A process(P) write, B process(C) read" <<
        "\n\tfifo is not existed" <<
        "\n\tProcess A mkfifo then block on open fifo with write mode" <<
        "\n\tProcess B open fifo with read mode, then both A and B open the fifo success" <<
        "\n\tProcess B block on read fifo" <<
        "\n\tProcess A write fifo, then B read fifo success");
    LOG4CPLUS_INFO(_IPC_LOGGER_, "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");

    // Delete the existing fifo
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Clear the fifo before test: " << wPathName);
    string cmd("rm -rf " + wPathName);
    system(cmd.c_str());

    pid_t parentPid = getpid();
    LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": Fork a child process");

    pid_t pid = fork();
    if (-1 == pid) {
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "fail to fork.");
    } else if (0 == pid) {
        pid_t childPid = getpid();

        // 1 open
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": sleep 2s then open fifo with O_RDONLY mode"
            << ", fifo name = " << rPathName);

        int n=0;
        while(n++ < 2) {
            LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": sleeping ... " << n);
            sleep(1);
        }
        m_pNamedPipe = new NamedPipe(rPathName);
        m_pNamedPipe->initRead();

        // 2 read
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": block on read fifo");
        int result = m_pNamedPipe->read();

        if (JSUCCESS == result) {
            string recvData = m_pNamedPipe->getData();
            LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": read fifo success, data = " << recvData);
        }

        // 3 exit
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": sleep 1s then exit");
        sleep(1);
        delete m_pNamedPipe;
        m_pNamedPipe = NULL;
        _exit(0);
    } else {
        // 1 mkfifo and open
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": create and "
            << "and block on open fifo with O_WRONLY mode, fifo name = " << wPathName);

        m_pNamedPipe = new NamedPipe(wPathName);
        m_pNamedPipe->initWrite();

        // 2 write
        string sendData("1234567890987654321");
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": sleep 3s then write data to fifo: " << sendData);
        
        int n=0;
        while(n++ < 3) {
            LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": sleeping ... " << n);
            sleep(1);
        }
        m_pNamedPipe->setData(sendData);
        int result = m_pNamedPipe->write();

        if (JERROR != result) {
            LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": write fifo success, sleep 2s then clear fifo and exit");
        }

        sleep(2);
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": clear the fifo after test: " << wPathName);
        system(cmd.c_str());
    }

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Demo End: " << getpid());
}

void NamedPipeDemo::demo_ropen_block_fifo_exist(string wPathName, string rPathName) {
    if (wPathName.compare(rPathName) != 0) {
        LOG4CPLUS_INFO(_IPC_LOGGER_, "Run demo_ropen_block_fifo_exist Test Error.");
        return;
    }

    LOG4CPLUS_INFO(_IPC_LOGGER_, "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Demo Start: Block mode, A process(P) write, B process(C) read" <<
        "\n\tfifo is existed" <<
        "\n\tProcess B block on open fifo with read mode" <<
        "\n\tProcess A open fifo with write mode, then both A and B open the fifo success" <<
        "\n\tProcess A write fifo" <<
        "\n\tProcess B read fifo success");
    LOG4CPLUS_INFO(_IPC_LOGGER_, "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");

    // create fifo
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Create the fifo before test: " << wPathName);
    string cmd("mkfifo -m 0755 " + wPathName);
    system(cmd.c_str());

    pid_t parentPid = getpid();
    LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": Fork a child process");

    pid_t pid = fork();
    if (-1 == pid) {
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "fail to fork.");
    } else if (0 == pid) {
        pid_t childPid = getpid();

        // 1 open
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": block on open fifo with O_RDONLY mode"
            << ", fifo name = " << rPathName);

        m_pNamedPipe = new NamedPipe(rPathName);
        m_pNamedPipe->initRead();

        // 2 read
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": sleep 1s then read fifo");
        int n=0;
        while(n++ < 1) {
            LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": sleeping ... " << n);
            sleep(1);
        }

        int result = m_pNamedPipe->read();

        if (JSUCCESS == result) {
            string recvData = m_pNamedPipe->getData();
            LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": read fifo success, data = " << recvData);
        }

        // 3 exit
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": sleep 1s then exit");
        sleep(1);
        delete m_pNamedPipe;
        m_pNamedPipe = NULL;
        _exit(0);
    } else {
        // 1 open
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": sleep 2s then open fifo with O_WRONLY mode, fifo name = " << wPathName);

        int n=0;
        while(n++ < 2) {
            LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": sleeping ... " << n);
            sleep(1);
        }
        m_pNamedPipe = new NamedPipe(wPathName);
        m_pNamedPipe->initWrite();

        // 2 write
        string sendData("1234567890987654321");
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": write data to fifo: " << sendData);
        
        m_pNamedPipe->setData(sendData);
        int result = m_pNamedPipe->write();

        if (JERROR != result) {
            LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": write fifo success, sleep 2s then clear fifo and exit");
        }

        sleep(3);
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": clear the fifo after test: " << wPathName);
        cmd.clear();
        cmd.append("rm -rf " + wPathName);
        system(cmd.c_str());
    }

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Demo End: " << getpid());
}


void NamedPipeDemo::demo_write_block(string wPathName, string rPathName) {
    if (wPathName.compare(rPathName) != 0) {
        LOG4CPLUS_INFO(_IPC_LOGGER_, "Run demo_write_block Test Error.");
        return;
    }

    LOG4CPLUS_INFO(_IPC_LOGGER_, "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Demo Start: Block mode, A process(P) write, B process(C) read" <<
        "\n\tProcess A create and open fifo with write mode" <<
        "\n\tProcess A write 8191*8 = 65536 bytes data to fifo, then block on write 8 more bytes data" <<
        "\n\tProcess B read 4096 bytes success in one time" <<
        "\n\tProcess A write 8 bytes success");
    LOG4CPLUS_INFO(_IPC_LOGGER_, "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");

    pid_t parentPid = getpid();
    LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": Fork a child process");

    pid_t pid = fork();
    if (-1 == pid) {
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "fail to fork.");
    } else if (0 == pid) {
        pid_t childPid = getpid();

        // 1 open
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": open fifo with O_RDONLY mode"
            << ", fifo name = " << rPathName);

        m_pNamedPipe = new NamedPipe(rPathName);
        m_pNamedPipe->initRead();

        // 2 read
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": sleep 3s to wait until fifo is full then read fifo");
        int n=0;
        while(n++ < 3) {
            LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": sleeping ... " << n);
            sleep(1);
        }

        int result = m_pNamedPipe->read();

        if (JSUCCESS == result) {
            string recvData = m_pNamedPipe->getData();
            LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": read " << recvData.size() << " bytes from fifo");
        }

        // 3 exit
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": sleep 1s then exit");
        sleep(1);
        delete m_pNamedPipe;
        m_pNamedPipe = NULL;
        _exit(0);
    } else {
        // 1 open
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": create and open fifo with O_WRONLY mode, fifo name = " << wPathName);

        m_pNamedPipe = new NamedPipe(wPathName);
        m_pNamedPipe->initWrite();

        // 2 write 65536 byte
        string sendData("12345678");
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": write 65556 bytes data to fifo: " << sendData);
        
        m_pNamedPipe->setData(sendData);
        for (int i=0; i<8192; i++) {
            m_pNamedPipe->write();
        }

        // 3 write 8 byte
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": block on write 8 more bytes data to fifo: " << sendData);
        int result = m_pNamedPipe->write();

        if (JERROR != result) {
            LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": write fifo success, sleep 2s then clear fifo and exit");
        }

        sleep(3);
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": clear the fifo after test: " << wPathName);
        string cmd("rm -rf " + wPathName);
        system(cmd.c_str());
    }

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Demo End: " << getpid());
}