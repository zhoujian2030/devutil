/*
 * JPipe.cpp
 *
 *  Created on: Nov 19, 2015
 *      Author: z.j
 */
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <iostream>
#include "common.h"
#include "JPipe.h"

using namespace std;
using namespace ipc;

JPipe::JPipe()
: 
mReadFd(0),
mWriteFd(0),
mDirection(P_READ_WRITE)
{

}

JPipe::JPipe(int readFd, int writeFd)
:
mReadFd(readFd),
mWriteFd(writeFd)
{
    mDirection = 0;
    if (0 != readFd) {
        mDirection = mDirection | P_READ;
    }

    if (0 != writeFd) {
        mDirection = mDirection | P_WRITE;
    }
}

JPipe::~JPipe() {

}

int JPipe::init() {
    int fd[2];
    int result = pipe(fd);
    if (-1 == result) {
        cout << "Fail to create pipe." << endl;
        return JERROR;
    }

    mReadFd = fd[0];
    mWriteFd = fd[1];
    cout << "Create pipe success: " << fd[0] << ", " << fd[1] << endl;
    return JSUCCESS;
}

void JPipe::setRead() {
    if (isWrite()) {
        close(mWriteFd);
        mDirection = mDirection & (~P_WRITE);
    }
}

void JPipe::setWrite() {
    if(isRead()) {
        close(mReadFd);
        mDirection = mDirection & (~P_READ);
    }
}

bool JPipe::isRead() {
    return ((mDirection & P_READ) != 0);
}

bool JPipe::isWrite() {
    return ((mDirection & P_WRITE) != 0);
}

void JPipe::closeAll() {
    close(mReadFd);
    close(mWriteFd);
}

int JPipe::getReadFd() {
    return this->mReadFd;
}

int JPipe::getWriteFd() {
    return this->mWriteFd;
}

// return actual number of bytes sent if success
// return JERROR if error
int JPipe::send(char* buff) {
    cout << "JPipe write data: " << buff << endl;
    
    if (!isWrite()) {
        cout << "Not allow to write data." << endl;
        return JERROR;
    }

    if (NULL == buff) {
        return JERROR;
    }

    int result = write(mWriteFd, buff, strlen(buff));
    if (-1 == result) {
        cout << "Write data to pipe failed." << endl;
        cout << "errno = " << errno << ", " << strerror(errno) << endl;
        return JERROR;
    }

    cout << "Write data success" << endl;

    return result;
}

// return JPIPE_NOT_EMPTY if there are still data in pipe
// return JSUCCESS if read data success, number of byte read is strlen(buff)
// return JERROR if error happen
int JPipe::recv(char* buff, int buffLen) {
    if (!isRead()) {
        cout << "Not allow to read data." << endl;
        return JERROR;
    }

    if (NULL == buff) {
        return JERROR;
    }

    int result = JSUCCESS;
    int recvLen = read(mReadFd, buff, buffLen);
    if (-1 == recvLen) {
        cout << "Read from pipe failed." << endl;
        cout << "errno = " << errno << ", " << strerror(errno) << endl;
        result = JERROR;
    } else {   
        // if the write fd is close, return 0 without blocking
        if (0 == recvLen) {
            cout << "The write fd of pipe is closed." << endl;
            result = JERROR;
        } else {
            buff[recvLen] = '\0';
            // if data received in the buff is full, it indicats that there may be more data available in pipe
            if (recvLen == buffLen) {
                cout << "More data might available in pipe." << endl;
                result = JPIPE_NOT_EMPTY;
            }

            cout << "JPipe read message: " << buff << endl;
        }
    }

    return result;
}

