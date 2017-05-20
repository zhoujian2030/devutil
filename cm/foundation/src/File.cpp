/*
 * File.cpp
 *
 *  Created on: May 18, 2017
 *      Author: j.zhou
 */

#include "File.h"
#include "CMLogger.h"
#include "IoException.h"
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

using namespace std;
using namespace cm;

// -----------------------------------------
File::File(const std::string filename, OpenMode mode, AccessType accessType) 
: m_filename(filename), m_mode(mode), m_accessType(accessType)
{
    CMLogger::initConsoleLog();

    open();
}

// -----------------------------------------
File::File(const char* filename, OpenMode mode, AccessType accessType) {
    CMLogger::initConsoleLog();

    if (filename == 0) {
        throw std::invalid_argument("filename is a null pointer!");
    }

    m_filename.append(filename);
    m_mode = mode;
    m_accessType = accessType;

    open();
}

// -----------------------------------------
File::~File() {
    close();
}

// -----------------------------------------
void File::open() {
    int oflag;

    switch (m_accessType)
    {
        case FILE_READ_ONLY:
        {
            oflag = O_RDONLY;
            break;
        }
        case FILE_WRITE_ONLY:
        {
            oflag = O_WRONLY;
            break;
        }
        case FILE_READ_WRITE:
        {
            oflag = O_RDWR;
            break;
        }
        case FILE_READ_WRITE_APPEND:
        {
            oflag = O_RDWR | O_APPEND;
            break;
        }
        default:
        {
            throw std::invalid_argument("Invalid access Type " + m_filename);
        }   
    }

    switch (m_mode)
    {
        case FILE_OPEN:
        {
            // No additional OFLAGS here
            m_fd = ::open(m_filename.c_str(), oflag, 0);
            if (m_fd == -1)
            {
                LOG4CPLUS_ERROR(_CM_LOOGER_NAME_, "fail to open file " << m_filename << ". errno = " << errno 
                    << " - " << strerror(errno));
                throw IoException();
            }
            break;
        }
        case FILE_CREATE:
        {
            // Create file if it does not exist.
            // Truncate it if it exists.
            if(m_accessType == FILE_READ_WRITE_APPEND) {
                LOG4CPLUS_WARN(_CM_LOOGER_NAME_, "Not support O_APPEND when create new file");
                oflag = O_RDWR;
            }
            m_fd = ::open(m_filename.c_str(), oflag | O_CREAT | O_TRUNC, ALL_READ_WRITE);
            if (m_fd == -1)
            {
                LOG4CPLUS_ERROR(_CM_LOOGER_NAME_, "fail to create file " << m_filename << ". errno = " << errno 
                    << " - " << strerror(errno));
                throw IoException();
            }
            break;
        }
        case FILE_OPEN_CREATE:
        {
            // Create file if it does not exist.
            m_fd = ::open(m_filename.c_str(), oflag | O_CREAT, ALL_READ_WRITE);
            if (m_fd == -1)
            {
                LOG4CPLUS_ERROR(_CM_LOOGER_NAME_, "fail to open/create file " << m_filename << ". errno = " << errno 
                    << " - " << strerror(errno));
                throw IoException();
            }
            break;
        }
        default:
        {
            throw std::invalid_argument("Invalid access Type " + m_filename);
        }   
    }
}

// -----------------------------------------
bool File::checkState() {
    return !(m_fd == -1);
}

// -----------------------------------------
int File::write(const char* theBuffer, int numOfBytesToWrite, int& numberOfBytesWritten) {
    if (m_fd == -1) {
        LOG4CPLUS_ERROR(_CM_LOOGER_NAME_, "File not open");
        return FILE_ERR;
    }

    if (theBuffer == 0) {
        throw std::invalid_argument("theBuffer is a null pointer!");
    }

    numberOfBytesWritten = ::write(m_fd, theBuffer, numOfBytesToWrite);
    if (numberOfBytesWritten == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            // For non-blocking socket, it would return EAGAIN or EWOULDBLOCK 
            // when buffer is full
            LOG4CPLUS_INFO(_CM_LOOGER_NAME_, "no data write to file now, fd = " << m_fd);
            return FILE_WAIT;
        } else {
            LOG4CPLUS_ERROR(_CM_LOOGER_NAME_, "fail to write data to file: " << m_filename
                << ", errno = " << errno << " - " << strerror(errno));    
            return FILE_ERR;        
        }
    }

    return FILE_SUCC;
}

// -----------------------------------------
int File::read(char* theBuffer, int buffSize, int& numOfBytesRead) {
    if (m_fd == -1) {
        LOG4CPLUS_ERROR(_CM_LOOGER_NAME_, "File not open");
        return FILE_ERR;
    }

    if (theBuffer == 0) {
        throw std::invalid_argument("theBuffer is a null pointer!");
    }

    numOfBytesRead = ::read(m_fd, theBuffer, buffSize);

    if (numOfBytesRead == -1) {
        LOG4CPLUS_ERROR(_CM_LOOGER_NAME_, "fail to read data from file: " << m_filename
                << ", errno = " << errno << " - " << strerror(errno));    
        return FILE_ERR;  
    }

    return FILE_SUCC;
}

// -----------------------------------------
long File::seek(long thePos) {
    if (!checkState()) {
        LOG4CPLUS_ERROR(_CM_LOOGER_NAME_, "File not open");
        return -1L;
    }

    switch (thePos)
    {
        case F_SEEK_CURRENT:
        {
            return ::lseek(m_fd, 0L, SEEK_CUR);
        }
        case F_SEEK_END:
        {
            return ::lseek(m_fd, 0L, SEEK_END);
        }
        case F_SEEK_BEGIN:
        {
            return ::lseek(m_fd, 0L, SEEK_SET);
        }
    }

    if (thePos > 0)
    {
        return ::lseek(m_fd, thePos, SEEK_SET);
    }

    return -1L;
}

// -----------------------------------------
void File::close() {
    if (checkState()) {
        ::close(m_fd);
        m_fd = -1;
    }
}
