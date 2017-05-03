/*
 * TestEventIndicator.cpp
 *
 *  Created on: Apr 23, 2016
 *      Author: z.j
 */

#include "DemoThread.h"
#include "SubscriberThread.h"
#include "CPPLogger.h"
#include <iostream>

using namespace std;
using namespace cm;

int main(int argc, char* argv[]) {
#if 0
    log4cplus::Logger logger = log4cplus::Logger::getInstance("CMTEST");
    CPPLogger::initConsoleLog(logger);

    LOG4CPLUS_DEBUG("CMTEST", "start Thread.");

    ThreadA th;
    th.start();

    LOG4CPLUS_DEBUG("CMTEST", "sleep 2s then set event.");
    Thread::sleep(2000);
    th.setEvent();

    LOG4CPLUS_DEBUG("CMTEST", "waiting thread termination.");

    th.wait();

    th.terminate();
    LOG4CPLUS_DEBUG("CMTEST", "sleep 1s then set event and start thread again.");
    Thread::sleep(1000);
    th.setEvent();
    th.start();

    LOG4CPLUS_DEBUG("CMTEST", "waiting thread termination.");
    th.wait();

    LOG4CPLUS_DEBUG("CMTEST", th.getName() << " exited " << th.getExitStatus());
    return 0;

#else
    log4cplus::Logger logger = log4cplus::Logger::getInstance("CMTEST");
    CPPLogger::initConsoleLog(logger);

    EventBroadcasting* eventBroadcast = new EventBroadcasting();
    SubscriberThread* t1 = new SubscriberThread("Subscriber 1", eventBroadcast);
    SubscriberThread* t2 = new SubscriberThread("Subscriber 2", eventBroadcast);
    SubscriberThread* t3 = new SubscriberThread("Subscriber 3", eventBroadcast);
    t1->start();
    t2->start();
    t3->start();

    Thread::sleep(100);
    
    while(1) {
        LOG4CPLUS_DEBUG("CMTEST", "-------------------------------------------------------------");
        
        eventBroadcast->notifyAll();
        Thread::sleep(5);
    }

    t1->wait();
    LOG4CPLUS_DEBUG("CMTEST", "t1 waiting thread termination.");

#endif
}