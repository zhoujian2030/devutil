/*
 * ReactorThread.cpp
 *
 *  Created on: Apr 02, 2016
 *      Author: z.j
 */

#include "ReactorThread.h"
#include "NetLogger.h"

#include <time.h>

using namespace net;
using namespace cm;

// --------------------------------------------------
ReactorThread::ReactorThread()
: Thread("ReactorThread")
{
    NetLogger::initConsoleLog();
    // create a recursive mutex
    m_lock = new MutexLock(true);
}

// --------------------------------------------------
ReactorThread::~ReactorThread() {
    delete m_lock;
}

// --------------------------------------------------
unsigned long ReactorThread::run() {
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, this->getName() << " is running.");
    if (m_epollSocketSet.getNumberOfSocket() > 0) {
        m_socketSetChangeEvent.reset();
    }

    while (true) {
        // if no socket registered in epoll, wait
        if (m_epollSocketSet.getNumberOfSocket() < 1) {
            m_socketSetChangeEvent.wait();
            LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "New socket change event is set.");
        }

        EpollSocketSet::EpollSocket* epollSocket = m_epollSocketSet.poll(0);
        bool sleep = true;

        if (epollSocket == 0) {
            LOG4CPLUS_ERROR(_NET_LOOGER_NAME_, "Error occurred during epoll_wait");
            // TODO exit ??
            _exit(1);
        }

        // handle ready sockets, call the event handler
        while (epollSocket->events != 0) {
            SocketEventHandler* eventHandler = epollSocket->eventHandler;
            Socket* socket = epollSocket->socket;
            if (eventHandler != 0) {

                // handle output event
                if (epollSocket->events & EPOLLOUT) {
                    eventHandler->handleOutput(socket);
                    sleep = false;
                    // if socket has been closed by the output handler 
                    // due to socket error, skip possible input event
                    if (!socket->isReady()) {
                        continue;
                    }
                }

                // handle input event
                if (epollSocket->events & EPOLLIN) {
                    eventHandler->handleInput(socket);
                    sleep = false;
                }
            } else {
                LOG4CPLUS_WARN(_NET_LOOGER_NAME_, "eventHandler of socket " << socket->getSocket() 
                    << " is NULL.");

                // as the epoll event of this socket is not removed in m_epollSocketSet.poll()
                // here should remove this socket from the epoll to avoid keeping recving the event that
                // is not monitored any more
                if (socket != 0) {
                    removeHandlers(socket);
                    socket->close();
                    delete socket;
                    socket = 0;
                }

                // maybe epoll ET mode can be considered?
            }

            epollSocket++;
        }

        if (sleep) {
            LOG4CPLUS_TRACE(_NET_LOOGER_NAME_, "No socket event, sleep 1ms");
            // sleep 1ms
            struct timespec sleepTime;
            sleepTime.tv_sec = 0;
            sleepTime.tv_nsec = 1000000;
            nanosleep(&sleepTime, NULL);
        }
    }

    return 0;
}
