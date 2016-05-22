/*
 * CMLogger.cpp
 *
 *  Created on: May 22, 2016
 *      Author: z.j
 */

#include "CMLogger.h"

using namespace cm;

bool CMLogger::s_isInited = false;

// ---------------------------------------
void CMLogger::initConsoleLog() {
    log4cplus::Logger logger = log4cplus::Logger::getInstance(_CM_LOOGER_NAME_);
    
    if (s_isInited) {
        LOG4CPLUS_DEBUG(logger, "Console logger is already initialized.");
        return;
    }

    CPPLogger::initConsoleLog(logger);
    CPPLogger::setLogLevel(logger, DEBUG);
    s_isInited = true;
}

// --------------------------------------
void CMLogger::setLogLevel(Level level) {
    log4cplus::Logger logger = log4cplus::Logger::getInstance(_CM_LOOGER_NAME_);

    CPPLogger::setLogLevel(logger, level);
}
