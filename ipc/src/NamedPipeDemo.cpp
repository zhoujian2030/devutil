/*
 * NamedPipeDemo.cpp
 *
 *  Created on: Jan 12, 2016
 *      Author: z.j
 */

#include <stdlib.h>
#include <vector>
#include <fcntl.h>
#include "NamedPipeDemo.h"
#include "NamedPipe.h"
#include "IPCLogger.h"
#include "common.h"
#include "Util.h"

using namespace ipc;
using namespace std;

NamedPipeDemo::NamedPipeDemo() {
    IPCLogger::initConsoleLog();
    IPCLogger::setLogLevel(cm::DEBUG);    
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
        int n=0;
        while(n++ < 2) {
            LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": sleeping ... " << n);
            sleep(1);
        }
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": open fifo with O_RDONLY mode"
            << ", fifo name = " << rPathName);
        m_pNamedPipe = new NamedPipe(rPathName);
        m_pNamedPipe->initRead();

        // 2 read
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": block on read fifo");
        int result = m_pNamedPipe->read();

        if (JSUCCESS == result) {
            string recvData = m_pNamedPipe->getData();
            LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": read fifo success, data = " << recvData);
        } else {
            LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": Demo Test Fail !");
        }

        // 3 exit
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": sleeping ... 1");
        sleep(1);
        delete m_pNamedPipe;
        m_pNamedPipe = NULL;
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": exit");
        _exit(0);
    } else {
        // 1 mkfifo and open
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": create and "
            << "and block on open fifo with O_WRONLY mode, fifo name = " << wPathName);
        m_pNamedPipe = new NamedPipe(wPathName);
        m_pNamedPipe->initWrite();

        // 2 write
        int n=0;
        while(n++ < 2) {
            LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": sleeping ... " << n);
            sleep(1);
        }
        string sendData("1234567890987654321");
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": write data to fifo: " << sendData);
        m_pNamedPipe->setData(sendData);
        int result = m_pNamedPipe->write();

        if (JERROR != result) {
            LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": write fifo success");
        }

        n=0;
        while(n++ < 2) {
            LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": sleeping ... " << n);
            sleep(1);
        }
        delete m_pNamedPipe;
        m_pNamedPipe = NULL;
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": clear the fifo after test then exit: " << wPathName);
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
        int n=0;
        while(n++ < 1) {
            LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": sleeping ... " << n);
            sleep(1);
        }
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": read fifo");
        int result = m_pNamedPipe->read();

        if (JSUCCESS == result) {
            string recvData = m_pNamedPipe->getData();
            LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": read fifo success, data = " << recvData);
        } else {
            LOG4CPLUS_ERROR(_IPC_LOGGER_, "Demo Test Fail !");
            delete m_pNamedPipe;
            m_pNamedPipe = NULL;
            return;
        }

        // 3 exit
        sleep(1);
        delete m_pNamedPipe;
        m_pNamedPipe = NULL;
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": exit");
        _exit(0);
    } else {
        // 1 open
        int n=0;
        while(n++ < 2) {
            LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": sleeping ... " << n);
            sleep(1);
        }
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": open fifo with O_WRONLY mode, fifo name = " 
            << wPathName);
        m_pNamedPipe = new NamedPipe(wPathName);
        m_pNamedPipe->initWrite();

        // 2 write
        string sendData("1234567890987654321");
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": write data to fifo: " << sendData);
        
        m_pNamedPipe->setData(sendData);
        int result = m_pNamedPipe->write();

        if (JERROR != result) {
            LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": write fifo success");
        }

        sleep(3);
        delete m_pNamedPipe;
        m_pNamedPipe = NULL;
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": clear the fifo after test then exit: " << wPathName);
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
        processChild(rPathName);
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
            LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": write 8 more bytes data to fifo success");
        }

        sleep(3);
        delete m_pNamedPipe;
        m_pNamedPipe = NULL;
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": clear the fifo after test then exit: " << wPathName);
        string cmd("rm -rf " + wPathName);
        system(cmd.c_str());
    }

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Demo End: " << getpid());
}

void NamedPipeDemo::demo_write_less_than_64k_block(string wPathName, string rPathName) {    
    if (wPathName.compare(rPathName) != 0) {
        LOG4CPLUS_INFO(_IPC_LOGGER_, "Run demo_write_block Test Error.");
        return;
    }

    LOG4CPLUS_INFO(_IPC_LOGGER_, "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Demo Start: Block mode, A process(P) write, B process(C) read" <<
        "\n\tProcess A create and open fifo with write mode" <<
        "\n\tProcess A write 8, 4K, 8, 4K, 8, 4K, 8, 4K, 8, 4K, 8, 4K, 8, 4K, 8, 4K bytes data, "<<
        "\n\t          then block on write 8 more bytes data" <<
        "\n\tProcess B read 4096 bytes success in one time" <<
        "\n\tProcess A write 8 bytes success");
    LOG4CPLUS_INFO(_IPC_LOGGER_, "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");

    pid_t parentPid = getpid();
    LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": Fork a child process");

    pid_t pid = fork();
    if (-1 == pid) {
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "fail to fork.");
    } else if (0 == pid) {
        processChild(rPathName);
    } else {
        // 1 open
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": create and open fifo with O_WRONLY mode, fifo name = " << wPathName);

        m_pNamedPipe = new NamedPipe(wPathName);
        m_pNamedPipe->initWrite();

        // 2 write 8, 4K, 8, 4K, 8, 4K, 8, 4K, 8, 4K, 8, 4K, 8, 4K, 8, 4K bytes
        int n=NamedPipe::BUFF_SIZE/8;
        string bytes8("abcdabcd");
        string sample8("12345678"), bytes4k;
        for (int i=0; i<n; i++) {
            bytes4k.append(sample8);
        }

        int num = 0;
        vector<string> wdataVect;
        for (int i=0; i<8; i++) {
            wdataVect.push_back(bytes8);
            wdataVect.push_back(bytes4k);
            num += 8;
            num += 4096;
        }

        LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": write "<< num << " bytes data to fifo, then the fifo is full");
        
        for (unsigned int i=0; i<wdataVect.size(); i++) {
            m_pNamedPipe->setData(wdataVect.at(i));
            m_pNamedPipe->write();
        }

        // TODO clear vector, free the memory

        // 3 write 8 byte
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": block on write 8 more bytes data to fifo: " << bytes8);
        m_pNamedPipe->setData(bytes8);
        int result = m_pNamedPipe->write();

        if (JERROR != result) {
            LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": write 8 more bytes data to fifo success");
        } 

        sleep(3);
        delete m_pNamedPipe;
        m_pNamedPipe = NULL;
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": clear the fifo after test then exit: " << wPathName);
        string cmd("rm -rf " + wPathName);
        system(cmd.c_str());
    }

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Demo End: " << getpid());
}

void NamedPipeDemo::demo_write_catch_sigpipe(string wPathName, string rPathName) {
    if (wPathName.compare(rPathName) != 0) {
        LOG4CPLUS_INFO(_IPC_LOGGER_, "Run demo_write_catch_sigpipe Test Error.");
        return;
    }

    // install SIGPIPE handler
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Install SIGPIPE handler");
    Util::installSignalHandler(SIGPIPE, NamedPipeDemo::handleSIGPIPE);

    LOG4CPLUS_INFO(_IPC_LOGGER_, "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Demo Start: Block mode, A process(P) read, B process(C) write" <<
        "\n\tProcess B install SIGPIPE handler, then create and open fifo with write mode" <<
        "\n\tProcess A open fifo with read mode then close it" <<
        "\n\tProcess B write fifo fail and catch SIGPIPE signal due to A closed the fifo" <<
        "\n\tProcess B uninstall SIGPIPE handler and write fifo again, then B exited due to SIGPIPE");
    LOG4CPLUS_INFO(_IPC_LOGGER_, "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");

    pid_t parentPid = getpid();
    LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": Fork a child process");

    pid_t pid = fork();
    if (-1 == pid) {
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "fail to fork.");
    } else if (0 == pid) {
        pid_t childPid = getpid();
        // 1 open
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": create and open fifo with O_WRONLY mode, fifo name = " << wPathName);
        m_pNamedPipe = new NamedPipe(wPathName);
        m_pNamedPipe->initWrite();

        // 2 write 
        string sendData("12345678");
        sleep(1);
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": write data to fifo: " << sendData);      
        m_pNamedPipe->setData(sendData);
        int result = m_pNamedPipe->write();

        if (JERROR == result) {
            LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": write fail due to read fifo process " << 
                "has closed the fifo before this process write fifo");
        } else {
            LOG4CPLUS_ERROR(_IPC_LOGGER_, "Demo Test Fail !!");
            return;
        }

        // 3 write again
        sleep(1);
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": Uninstall SIGPIPE handler");
        Util::installSignalHandler(SIGPIPE, NULL);
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": write data to fifo again " <<
            "then the process exits duo to SIGPIPE");
        result = m_pNamedPipe->write();

        LOG4CPLUS_ERROR(_IPC_LOGGER_, "Demo Test fail !! The process should be exit duo to SIGPIPE");
        delete m_pNamedPipe;
        m_pNamedPipe = NULL;
        _exit(0);
        
    } else {
         // 1 open
        usleep(500);
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": open fifo with O_RDONLY mode"
            << ", fifo name = " << rPathName);
        m_pNamedPipe = new NamedPipe(rPathName);
        m_pNamedPipe->initRead();    

        // 2 close 
        usleep(200);
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": not read fifo, close fifo");
        m_pNamedPipe->close();   

        sleep(2);
        delete m_pNamedPipe;
        m_pNamedPipe = NULL;
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In PARENT " << parentPid << ": clear the fifo after test: " << wPathName);
        string cmd("rm -rf " + wPathName);
        system(cmd.c_str());
    }

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Uninstall SIGPIPE handler");
    Util::installSignalHandler(SIGPIPE, NULL);

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Demo End: " << getpid());
}

void NamedPipeDemo::open_fail_fifo_not_exist(string pathName) {

    LOG4CPLUS_INFO(_IPC_LOGGER_, "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Demo Start: Block mode, A process open fifo in read/write mode" <<
        "\n\tfifo is not created, fail to open the fifo in read mode" <<
        "\n\tfifo is not created, fail to open the fifo in write mode");
    LOG4CPLUS_INFO(_IPC_LOGGER_, "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");

    // Delete the existing fifo
    LOG4CPLUS_INFO(_IPC_LOGGER_, "clear the fifo before test: " << pathName);
    string cmd("rm -rf " + pathName);
    system(cmd.c_str());

    LOG4CPLUS_INFO(_IPC_LOGGER_, "open fifo which is not created in O_RDONLY mode: " << pathName);
    m_pNamedPipe = new NamedPipe(pathName);
    int result = m_pNamedPipe->open(O_RDONLY);   

    if(JERROR == result) {
        LOG4CPLUS_INFO(_IPC_LOGGER_, "open fifo fail due to it is not created");
    } else {
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "Demo Test Fail !");
        return;
    }

    LOG4CPLUS_INFO(_IPC_LOGGER_, "open fifo which is not created in O_WRONLY mode: " << pathName);
    result = m_pNamedPipe->open(O_WRONLY);   

    if(JERROR == result) {
        LOG4CPLUS_INFO(_IPC_LOGGER_, "open fifo fail due to it is not created");
    } else {
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "Demo Test Fail !");
        return;
    }

    delete m_pNamedPipe;
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Demo End");
}

void NamedPipeDemo::processChild(string pathName) {
    pid_t childPid = getpid();

    // 1 open
    usleep(1000);
    LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": open fifo with O_RDONLY mode"
        << ", fifo name = " << pathName);
    m_pNamedPipe = new NamedPipe(pathName);
    m_pNamedPipe->initRead();

    // 2 read
    int n=0;
    while(n++ < 3) {
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": sleeping ... " << n);
        sleep(1);
    }
    LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": read fifo after it is full");
    int result = m_pNamedPipe->read();

    if (JSUCCESS == result) {
        string recvData = m_pNamedPipe->getData();
        LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": read " << recvData.size() << " bytes from fifo");
    }

    // 3 exit
    sleep(1);
    delete m_pNamedPipe;
    m_pNamedPipe = NULL;
    LOG4CPLUS_INFO(_IPC_LOGGER_, "In CHILD " << childPid << ": exit");
    _exit(0);    
}

void NamedPipeDemo::handleSIGPIPE(int signo) {
    LOG4CPLUS_INFO(_IPC_LOGGER_, "In Process " << getpid() <<": Catch a SIGPIPE signal and signo = " << signo);
    // Do nothing
}