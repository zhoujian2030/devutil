/*
 * HttpLogger.cpp
 *
 *  Created on: July 11, 2016
 *      Author: z.j
 */

#include "HttpLogger.h"

using namespace http;
using namespace cm;

bool HttpLogger::s_isInited = false;

void HttpLogger::initConsoleLog() {
    log4cplus::Logger logger = log4cplus::Logger::getInstance(_HTTP_LOOGER_NAME_);
    
    if (s_isInited) {
        LOG4CPLUS_DEBUG(logger, "Console logger is already initialized.");
        return;
    }

    CPPLogger::initConsoleLog(logger);
    CPPLogger::setLogLevel(logger, DEBUG);
    s_isInited = true;
}

void HttpLogger::setLogLevel(Level level) {
    log4cplus::Logger logger = log4cplus::Logger::getInstance(_HTTP_LOOGER_NAME_);

    CPPLogger::setLogLevel(logger, level);
}