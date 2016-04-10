/*
 * NetLogger.cpp
 *
 *  Created on: Apr 02, 2016
 *      Author: z.j
 */

#include "NetLogger.h"

using namespace net;
using namespace logcpp;

bool NetLogger::s_isInited = false;

void NetLogger::initConsoleLog() {
    log4cplus::Logger logger = log4cplus::Logger::getInstance(_NET_LOOGER_NAME_);
    
    if (s_isInited) {
        LOG4CPLUS_DEBUG(logger, "Console logger is already initialized.");
        return;
    }

    CPPLogger::initConsoleLog(logger);
    s_isInited = true;
}

void NetLogger::setLogLevel(Level level) {
    log4cplus::Logger logger = log4cplus::Logger::getInstance(_NET_LOOGER_NAME_);

    CPPLogger::setLogLevel(logger, level);
}