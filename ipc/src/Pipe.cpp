/*
 * Pipe.cpp
 *
 *  Created on: Nov 19, 2015
 *      Author: z.j
 */
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h> 
#include <iostream>
#include "common.h"
#include "IPCLogger.h"
#include "Pipe.h"

using namespace ipc;
using namespace logcpp;

Pipe::Pipe()
: 
mReadFd(0),
mWriteFd(0),
mDirection(P_READ_WRITE)
{
    IPCLogger::initConsoleLog();
}

Pipe::Pipe(int readFd, int writeFd)
:
mReadFd(readFd),
mWriteFd(writeFd)
{
    IPCLogger::initConsoleLog();

    mDirection = 0;
    if (0 != readFd) {
        mDirection = mDirection | P_READ;
    }

    if (0 != writeFd) {
        mDirection = mDirection | P_WRITE;
    }
}

Pipe::~Pipe() {

}

int Pipe::init() {
    LOG4CPLUS_TRACE(_IPC_LOGGER_, "Pipe::init()");

    int fd[2];
    int result = pipe(fd);
    if (-1 == result) {
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "Fail to create pipe.");
        return JERROR;
    }

    mReadFd = fd[0];
    mWriteFd = fd[1];
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Create pipe success: fd[0] = " << fd[0] << ", fd[1] = " << fd[1]);
    return JSUCCESS;
}

void Pipe::setReadOnly() {
    LOG4CPLUS_TRACE(_IPC_LOGGER_, "Pipe::setReadOnly()");

    if (isWrite()) {
        LOG4CPLUS_DEBUG(_IPC_LOGGER_, "Close write fd: " << mWriteFd);
        close(mWriteFd);
        mDirection = mDirection & (~P_WRITE);
    }
}

void Pipe::setWriteOnly() {
    LOG4CPLUS_TRACE(_IPC_LOGGER_, "Pipe::setWriteOnly()");

    if(isRead()) {
        LOG4CPLUS_DEBUG(_IPC_LOGGER_, "Close read fd: " << mReadFd);
        close(mReadFd);
        mDirection = mDirection & (~P_READ);
    }
}

bool Pipe::isRead() {
    return ((mDirection & P_READ) != 0);
}

bool Pipe::isWrite() {
    return ((mDirection & P_WRITE) != 0);
}

void Pipe::closeAll() {
    LOG4CPLUS_DEBUG(_IPC_LOGGER_, "Close all fd.");
    close(mReadFd);
    close(mWriteFd);
}

int Pipe::getReadFd() {
    return this->mReadFd;
}

int Pipe::getWriteFd() {
    return this->mWriteFd;
}

// return actual number of bytes sent if success
// return JERROR if error
int Pipe::send(char* buff) {
    if (NULL == buff) {
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "null buff.");
        return JERROR;
    }

    LOG4CPLUS_DEBUG(_IPC_LOGGER_, "Write data to Pipe: " << buff);
    
    if (!isWrite()) {
        LOG4CPLUS_WARN(_IPC_LOGGER_, "Not allow to write data as write fd is closed.");
        return JERROR;
    }

    int result = write(mWriteFd, buff, strlen(buff));
    if (-1 == result) {
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "Write data to pipe failed. " << errno << ": " << strerror(errno));
        return JERROR;
    }

    LOG4CPLUS_DEBUG(_IPC_LOGGER_, "Write data success");

    return result;
}

// return JPIPE_NOT_EMPTY if there are still data in pipe
// return JSUCCESS if read data success, number of byte read is strlen(buff)
// return JERROR if error happen
int Pipe::recv(char* buff, int buffLen) {
    if (NULL == buff) {
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "null buff.");
        return JERROR;
    }

    if (!isRead()) {
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "Not allow to read data as read fd is closed.");
        return JERROR;
    }

    int result = JSUCCESS;
    int recvLen = read(mReadFd, buff, buffLen);
    if (-1 == recvLen) {
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "Read data from pipe failed. " << errno << ": " << strerror(errno));
        result = JERROR;
    } else {   
        // if the write fd is close, return 0 without blocking
        if (0 == recvLen) {
            LOG4CPLUS_ERROR(_IPC_LOGGER_, "The write fd of pipe is closed in all processes, return 0 from read.");
            buff[0] = '\0';
            result = JERROR;
        } else {
            buff[recvLen] = '\0';
            // if data received in the buff is full, it indicats that there may be more data available in pipe
            if (recvLen == buffLen) {
                LOG4CPLUS_INFO(_IPC_LOGGER_, "More data might available in pipe.");
                result = JPIPE_NOT_EMPTY;
            }

            LOG4CPLUS_DEBUG(_IPC_LOGGER_, "Read data from Pipe success: " << buff);
        }
    }

    return result;
}


void Pipe::setReadNonBlock() {
    LOG4CPLUS_TRACE(_IPC_LOGGER_, "Pipe::setReadNonBlock()");

    int flags = fcntl(this->mReadFd, F_GETFL);
    fcntl(this->mReadFd,F_SETFL,flags | O_NONBLOCK);

    LOG4CPLUS_DEBUG(_IPC_LOGGER_, "Original flags: " << flags);
    LOG4CPLUS_DEBUG(_IPC_LOGGER_, "Current flags: " << fcntl(this->mReadFd, F_GETFL));
}

void Pipe::setReadBlock() {
    LOG4CPLUS_TRACE(_IPC_LOGGER_, "Pipe::setReadBlock()");

    int flags = fcntl(this->mReadFd, F_GETFL);
    fcntl(this->mReadFd,F_SETFL,flags & (~O_NONBLOCK));

    LOG4CPLUS_DEBUG(_IPC_LOGGER_, "Original flags: " << flags);
    LOG4CPLUS_DEBUG(_IPC_LOGGER_, "Current flags: " << fcntl(this->mReadFd, F_GETFL));
}

void Pipe::setWriteNonBlock() {
    LOG4CPLUS_TRACE(_IPC_LOGGER_, "Pipe::setWriteNonBlock()");

    int flags = fcntl(this->mWriteFd, F_GETFL);
    fcntl(this->mWriteFd,F_SETFL,flags | O_NONBLOCK);

    LOG4CPLUS_DEBUG(_IPC_LOGGER_, "Original flags: " << flags);
    LOG4CPLUS_DEBUG(_IPC_LOGGER_, "Current flags: " << fcntl(this->mWriteFd, F_GETFL));
}

void Pipe::setWriteBlock() {
    LOG4CPLUS_TRACE(_IPC_LOGGER_, "Pipe::setWriteBlock()");

    int flags = fcntl(this->mWriteFd, F_GETFL);
    fcntl(this->mWriteFd,F_SETFL,flags & (~O_NONBLOCK));

    LOG4CPLUS_DEBUG(_IPC_LOGGER_, "Original flags: " << flags);
    LOG4CPLUS_DEBUG(_IPC_LOGGER_, "Current flags: " << fcntl(this->mWriteFd, F_GETFL));
}