/*
 * SubscriberThread.cpp
 *
 *  Created on: Apr 23, 2016
 *      Author: z.j
 */

#include "SubscriberThread.h"
#include "CPPLogger.h"

unsigned long SubscriberThread::run() {
    LOG4CPLUS_DEBUG("CMTEST", this->getName() << " is running.");

    while(1) {
        LOG4CPLUS_DEBUG("CMTEST", this->getName() << " waiting event set.");
        
        m_eventBroadcast->wait(m_eventId);

        LOG4CPLUS_DEBUG("CMTEST",  this->getName() << " event is set.");

        Thread::sleep(1);
    }

    pthread_exit((void*)10);
    return 0;
}
