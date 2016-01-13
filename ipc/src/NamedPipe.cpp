/*
 * NamedPipe.cpp
 *
 *  Created on: Dec 21, 2015
 *      Author: z.j
 */
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sstream>
#include "NamedPipe.h"
#include "IPCLogger.h"
#include "common.h"


using namespace std;
using namespace ipc;

NamedPipe::NamedPipe(string& pathName)
: m_isRead(true),
  m_pathName(pathName),
  m_fd(-1)
  
{
    IPCLogger::initConsoleLog();
    //init();
}

NamedPipe::~NamedPipe() {
    if (-1 != m_fd) {
        ::close(m_fd);
    }
}

int NamedPipe::initRead() {
    m_isRead = true;
    m_messageBuffer.clear();
    memset(m_rxBuffer, '/0', sizeof(m_rxBuffer));  

    return open(O_RDONLY);
}

int NamedPipe::initWrite() {
    m_isRead = false;
    m_messageBuffer.clear();
    memset(m_rxBuffer, '/0', sizeof(m_rxBuffer));  
    if (JSUCCESS == create()) {
        return open(O_WRONLY);
    }

    return JERROR;
}

int NamedPipe::read() {
    int result = ::read(m_fd, m_rxBuffer, sizeof(m_rxBuffer));
    if (-1 == result) {
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "fail to read data from fifo " << m_pathName <<
            ", errno = " << errno << " - " << strerror(errno));
        return JERROR;
    } 

    // if all processes opened the fifo in write mode before close the fd
    // the process read from fifo will return 0 immediately
    if (0 == result) {
        LOG4CPLUS_INFO(_IPC_LOGGER_, "No process is ready to write fifo.");
        m_messageBuffer.clear();
        return JPIPE_WRITE_CLOSE;
    }

    m_rxBuffer[result] = '\0';
    m_messageBuffer.clear();
    m_messageBuffer.append(m_rxBuffer);

    LOG4CPLUS_DEBUG(_IPC_LOGGER_, "read data from fifo success(" << result << ") :" << m_messageBuffer);
    return JSUCCESS;
}

// return actual number of bytes sent if success
// return JERROR if error
int NamedPipe::write() {
    LOG4CPLUS_DEBUG(_IPC_LOGGER_, "write data to fifo " << m_pathName << " : " << m_messageBuffer);

    if (m_messageBuffer.size() > BUFF_SIZE) {
        LOG4CPLUS_WARN(_IPC_LOGGER_, "writing too many data may be non-atomic");
    }

    int result = ::write(m_fd, m_messageBuffer.c_str(), m_messageBuffer.size());
    if (-1 == result) {
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "fail to write data to fifo " << m_pathName << 
            ", errno = " << errno << " - " << strerror(errno));
        result = JERROR;
    }

    return result;
}

string& NamedPipe::getData() {
    return m_messageBuffer;
}

void NamedPipe::setData(const std::string& theString) {
    if (m_isRead) {
        LOG4CPLUS_WARN(_IPC_LOGGER_, "FIFO is open for read, not allowed to set data");
        return;
    }

    m_messageBuffer = theString;
}

void NamedPipe::close() {
    if (-1 != m_fd) {
        LOG4CPLUS_INFO(_IPC_LOGGER_, "close fifo: " << m_pathName);
        ::close(m_fd);
    }    
}

int NamedPipe::create() {    
    LOG4CPLUS_DEBUG(_IPC_LOGGER_, "NamedPipe::create()");

    if (-1 == access(m_pathName.c_str(), F_OK)) {
        LOG4CPLUS_INFO(_IPC_LOGGER_, "fifo " << m_pathName << " not exists, create it.");
        // the permission of the fifo file created is 0777 & ~umask
        // e.g. if umask = 022, 0777 & ~022 = 0755 (rwxr-xr-x)
        int res = mkfifo(m_pathName.c_str(), 0777);  
        if (res != 0) {
            LOG4CPLUS_ERROR(_IPC_LOGGER_, "fail to create fifo with path name " << m_pathName <<
                ". errno = " << errno << " - " << strerror(errno));
            return JERROR;
        } else {
            LOG4CPLUS_INFO(_IPC_LOGGER_, "success to create fifo.");
            return JSUCCESS;
        }
    }

    LOG4CPLUS_INFO(_IPC_LOGGER_, "fifo " << m_pathName << " has already been created.");
    return JSUCCESS;
}

int NamedPipe::open(int flags) {
    LOG4CPLUS_DEBUG(_IPC_LOGGER_, "NamedPipe::openFifo() - flags = " << flags);

    // the access mode in flags can be O_RDONLY, O_WRONLY, or O_RDWR
    // but in fact we should not open a fifo with O_RDWR (read/write)
    m_fd = ::open(m_pathName.c_str(), flags);
    if (-1 == m_fd) {
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "fail to open fifo with path name " << m_pathName <<
            ". errno = " << errno << " - " << strerror(errno));
        return JERROR;
    }

    LOG4CPLUS_INFO(_IPC_LOGGER_, "open fifo success, name = " << m_pathName << ", flags = " << flags);
    return JSUCCESS;
}

string NamedPipe::toString() const {
    string result("pathName = ");
    result.append(m_pathName);

    ostringstream s;
    s << ", BUFF_SIZE = " << BUFF_SIZE;  
    result.append(s.str());

    result.append(", messageBuffer = ");
    result.append(m_messageBuffer);

    return result;
}

void NamedPipe::clear() {
    m_messageBuffer.clear();
}