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
using namespace base;

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

    while (true) {
        // TODO if no socket event registered, should wait ???
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
                }

                // handle input event
                if (epollSocket->events & EPOLLIN) {
                    eventHandler->handleInput(socket);
                    sleep = false;
                }
            }

            epollSocket++;
        }

        if (sleep) {
            LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "no socket event, sleep 1ms");
            // sleep 1ms
            struct timespec sleepTime;
            sleepTime.tv_sec = 0;
            sleepTime.tv_nsec = 1000000;
            nanosleep(&sleepTime, NULL);
        }
    }

    return 0;
}