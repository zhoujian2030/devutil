/*
 * EpollSocketSet.cpp
 *
 *  Created on: Apr 04, 2016
 *      Author: z.j
 */

#include "EpollSocketSet.h"
#include "NetLogger.h"

#include <sys/resource.h>
#include <errno.h>
#include <string.h>

using namespace net;
using namespace base;

// -------------------------------------------------
EpollSocketSet::EpollSocketSet()
: m_epollEvents(0), m_epollFdSize(0), m_epollFd(0), m_numFds(0)
{
    NetLogger::initConsoleLog();

    struct rlimit limit;
    if (getrlimit(RLIMIT_NOFILE, &limit) == 0) {
        m_epollFdSize = limit.rlim_cur;
    }
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "Get limit value: " << m_epollFdSize);
    if (m_epollFdSize < 1024) {
        m_epollFdSize = 1024;
    } else if (m_epollFdSize > 1024 * 64) {
        // limit the max fd size to 64KB (65536)
        m_epollFdSize = 1024 * 64;
    }

    // 
    m_epollEvents = new epoll_event[m_epollFdSize];

    // create epoll fd
    // Note: the size arg in epoll_create is unused since linux 2.6.8, but
    // the size passed to epoll_create must not be less than or equal to 0
    m_epollFd = epoll_create(m_epollFdSize);
    if (m_epollFd < 0) {
        LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "Fail to create epoll fd. errno = " << errno 
            << " - " << strerror(errno));

        // TODO exit value ?
        _exit(0);
    }
}

// -------------------------------------------------
EpollSocketSet::~EpollSocketSet() {
    if (m_epollEvents != 0) {
        delete m_epollEvents;
        m_epollEvents = 0;
    }

    // TODO close epoll fd ?
}

// -------------------------------------------------
void EpollSocketSet::addEvents() {
    
}