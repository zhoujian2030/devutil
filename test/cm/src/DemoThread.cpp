/*
 * DemoThread.cpp
 *
 *  Created on: Apr 23, 2016
 *      Author: z.j
 */

#include "DemoThread.h"
#include "CPPLogger.h"

unsigned long ThreadA::run() {
    LOG4CPLUS_DEBUG("CMTEST", this->getName() << " is running.");

    LOG4CPLUS_DEBUG("CMTEST", "Waiting event set.");
    m_eventIndicator.wait();

    LOG4CPLUS_DEBUG("CMTEST", "event is set.");

    pthread_exit((void*)10);
    return 0;
}

void ThreadA::setEvent() {
    // only allow one thread to set event in a time
    LOG4CPLUS_DEBUG("CMTEST", "set event now.");
    m_lock->lock();
    m_eventIndicator.set();
    m_lock->unlock();
}