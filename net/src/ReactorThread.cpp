/*
 * ReactorThread.cpp
 *
 *  Created on: Apr 02, 2016
 *      Author: z.j
 */

#include "ReactorThread.h"
#include "NetLogger.h"

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

unsigned long ReactorThread::run() {
    bool isRunning = true;

    while (isRunning) {
        LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, this->getName() << " is running");
        Thread::sleep(1000);
    }

    return 1;
}