/*
 * SelectSocketSet.h
 *
 *  Created on: Nov 04, 2016
 *      Author: j.zhou
 */

#include "SelectSocketSet.h"

#include <errno.h>
#include <string.h>

using namespace net;
using namespace cm;

// -------------------------------------------------
SelectSocketSet::SelectSocketSet()
: m_lock(new MutexLock(true)),m_numFds(0), m_selectFd(0)
{
    NetLogger::initConsoleLog();

    // store all available SelectSocket objects
    m_readySocketArray = new SelectSocket[MAX_FD_SIZE_FOR_SELECT + 1];

    FD_ZERO(&m_rFdSet);
    FD_ZERO(&m_wFdSet);
}

// -------------------------------------------------
SelectSocketSet::~SelectSocketSet() {
    if (m_readySocketArray != 0) {
        delete [] m_readySocketArray;
        m_readySocketArray = 0;
    }

    FD_ZERO(&m_rFdSet);
    FD_ZERO(&m_wFdSet);
}

// -------------------------------------------------
void SelectSocketSet::updateEvents() {
    m_lock->lock();

    // update the sockets in epoll if any
    int eventNumber = m_updateSocketList.size();
    if (eventNumber > 0) {
        for (UpdateSocketList::iterator it = m_updateSocketList.begin(); it != m_updateSocketList.end(); ++it) {
            if (it->op == SELECT_ADD_EV) {
                // set fd
                if (it->events & SELECT_R_EV) {
                    if (!FD_ISSET(it->fd, &m_rFdSet)) {
                        FD_SET(it->fd, &m_rFdSet);
                    }
                }
                if (it->events & SELECT_W_EV) {
                    if (!FD_ISSET(it->fd, &m_wFdSet)) {
                        FD_SET(it->fd, &m_wFdSet);
                    }
                }
            } else {
                // clear fd
                if (it->events & SELECT_R_EV) {
                    FD_CLR(it->fd, &m_rFdSet);
                }
                if (it->events & SELECT_W_EV) {
                    FD_CLR(it->fd, &m_wFdSet);
                }
            }

            if (m_selectFd < it->fd) {
                m_selectFd = it->fd;
            }
        }

        m_updateSocketList.clear();
    }

    m_lock->unlock();
}

// -------------------------------------------------
void* SelectSocketSet::poll(int theTimeout) {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "poll, theTimeout = " << theTimeout);

    updateEvents();

    // check if any event available,
    // if theTimeout = -1, block on waiting
    // else if theTimeout = 0, return immediately
    // else return after timeout in millisecond
    fd_set readFds = m_rFdSet;
    fd_set writeFds = m_wFdSet;

    if (theTimeout >= 0) {
        struct timeval  tv;
        tv.tv_sec = theTimeout / 1000;
        tv.tv_usec = (theTimeout % 1000) * 1000;
        m_numFds = select(m_selectFd + 1, &readFds, &writeFds, 0, &tv);
    } else {
        m_numFds = select(m_selectFd + 1, &readFds, &writeFds, 0, 0);
    }    

    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "m_numFds = " << m_numFds);

    if (m_numFds <= 0) {
        // if (m_numFds == -1 && errno != EINTR) {
        //     LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "fail to wait epoll events. errno = "
        //         << errno << " - " << strerror(errno));
        //     return 0;
        // } else {
            // timeout or no socket event received
            m_readySocketArray[0].events = 0;
            return m_readySocketArray;
        // }
    }

    m_lock->lock();

    int readySocketIndex(0);
    SelectSocketMap::iterator it;
    for (it = m_selectSocketMap.begin(); it != m_selectSocketMap.end(); ++it) {
        bool isSocketReady;

        if (FD_ISSET(it->first, &readFds)) {
            m_readySocketArray[readySocketIndex].socket = it->second.socket;
            m_readySocketArray[readySocketIndex].eventHandler = it->second.eventHandler;
            m_readySocketArray[readySocketIndex].events = SELECT_R_EV;
            isSocketReady = true;
        } 

        if (FD_ISSET(it->first, &writeFds)) {
            if (isSocketReady) {
                 m_readySocketArray[readySocketIndex].events |= SELECT_W_EV;
            } else {
                m_readySocketArray[readySocketIndex].socket = it->second.socket;
                m_readySocketArray[readySocketIndex].eventHandler = it->second.eventHandler;
                m_readySocketArray[readySocketIndex].events = SELECT_R_EV;
                isSocketReady = true;
            }           
        }   

        if (isSocketReady) {
            readySocketIndex++;
        }     
    }

    // Set last socket event 0 to indicate ending
    m_readySocketArray[readySocketIndex].events = 0;
    m_lock->unlock();

    return (void*)m_readySocketArray;
}
