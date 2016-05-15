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
using namespace cm;

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

    // store all available EpollSocket objects
    m_readySocketArray = new EpollSocket[m_epollFdSize + 1];

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
        delete [] m_epollEvents;
        m_epollEvents = 0;
    }

    if (m_readySocketArray != 0) {
        delete [] m_readySocketArray;
        m_readySocketArray = 0;
    }

    // TODO close epoll fd ?
}

// -------------------------------------------------
void EpollSocketSet::updateEvents() {
    m_lock.lock();

    // update the sockets in epoll if any
    int eventNumber = m_updateSocketList.size();
    if (eventNumber > 0) {
        int result;
        for (UpdateSocketList::iterator it = m_updateSocketList.begin(); it != m_updateSocketList.end(); ++it) {
            struct epoll_event event;
            event.events = it->events;
            event.data.fd = it->fd;

            result = epoll_ctl(m_epollFd, it->op, it->fd, &event);
            if (result < 0) {
                if (errno == EBADF) {
                    // Do nothing.
                    // It may happen if EPOLL_CTL_DEL operation is called for a socket that has been closed
                    LOG4CPLUS_INFO(_NET_LOOGER_NAME_, "epoll_ctl fail, errno is EBADF, ignore it");

                } else if (errno == EEXIST) {
                    LOG4CPLUS_INFO(_NET_LOOGER_NAME_, "epoll_ctl fail, errno is EXIST, try EPOLL_CTL_MOD");
                    epoll_ctl(m_epollFd, EPOLL_CTL_MOD, it->fd, &event);
                    
                } else {
                    LOG4CPLUS_WARN(_NET_LOOGER_NAME_, "fail to change socket " << it->fd << " to epoll. errno = "
                        << errno << " - " << strerror(errno));
                }
            }
        }

        m_updateSocketList.clear();
    }

    m_lock.unlock();
}

// -------------------------------------------------
// @Description: 
EpollSocketSet::EpollSocket* EpollSocketSet::poll(int theTimeout) {

    updateEvents();

    // check if any event available,
    // if theTimeout = -1, block on waiting
    // else if theTimeout = 0, return immediately
    // else return after timeout in millisecond
    m_numFds = epoll_wait(m_epollFd, m_epollEvents, m_epollFdSize, theTimeout);


    if (m_numFds <= 0) {
        if (m_numFds == -1 && errno != EINTR) {
            LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "fail to wait epoll events. errno = "
                << errno << " - " << strerror(errno));
            return 0;
        } else {
            // timeout or no socket event received
            m_readySocketArray[0].events = 0;
            return m_readySocketArray;
        }
    }

    m_lock.lock();

    int readySocketIndex(0);

    for (int i=0; i<m_numFds; i++) {
        int fd = m_epollEvents[i].data.fd;
        int pollEvent = m_epollEvents[i].events;

        // we are only interested in read/write events
        if (!(pollEvent & (EPOLLIN | EPOLLOUT))) {
            LOG4CPLUS_WARN(_NET_LOOGER_NAME_, "drop the events and remove the socket. fd = " << fd 
                << ", events = " << pollEvent);
            m_epollSocketMap.erase(fd);
            struct epoll_event ev;
            epoll_ctl(m_epollFd, EPOLL_CTL_DEL, fd, &ev);
        } else {
            EpollSocketMap::iterator it = m_epollSocketMap.find(fd);
            if (it != m_epollSocketMap.end()) {
                m_readySocketArray[readySocketIndex].socket = it->second.socket;
                m_readySocketArray[readySocketIndex].eventHandler = it->second.eventHandler;
                // Make a "&" between the monitored events and the poll events to avoid notifying 
                // events that are not monitored any more
                m_readySocketArray[readySocketIndex].events = it->second.events & pollEvent;

                // TODO is it necessary to remove the socket event?
                // if do so, the event handler need to re-register the socket event later
                // after current event is handled

                // as the socket events are actually removed asynchronously when the reactor thread
                // calls poll() next time, while the reactor thread calls socket event handler in 
                // current time and normally the handler needs to re-register the socket event in 
                // epoll, that means in next time reactor thread will remove then add the same socket
                // event which must be a waste of time.

                // if (m_readySocketArray[readySocketIndex].events & EPOLLIN) {
                //     removeInputHandler(it);
                // }
                // if (m_readySocketArray[readySocketIndex].events & EPOLLOUT) {
                //     removeOutputHandler(it);
                // }

                readySocketIndex++;
            } else {
                // socket is no found, clean up
                LOG4CPLUS_WARN(_NET_LOOGER_NAME_, "the available socket is not found: " << fd);
                struct epoll_event ev;
                epoll_ctl(m_epollFd, EPOLL_CTL_DEL, fd, &ev);
            }
        }
    }

    // Set last socket event 0 to indicate ending
    m_readySocketArray[readySocketIndex].events = 0;

    m_lock.unlock();

    return m_readySocketArray;
}