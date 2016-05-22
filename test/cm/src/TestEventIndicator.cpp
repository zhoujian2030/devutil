/*
 * TestEventIndicator.cpp
 *
 *  Created on: Apr 23, 2016
 *      Author: z.j
 */

#include "DemoThread.h"
#include "CPPLogger.h"
#include <iostream>

using namespace std;
using namespace cm;

int main(int argc, char* argv[]) {
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
}