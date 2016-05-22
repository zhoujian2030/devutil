/*
 * AdsLogger.cpp
 *
 *  Created on: Apr 17, 2016
 *      Author: z.j
 */

#include "AdsLogger.h"

using namespace ads;
using namespace cm;

bool AdsLogger::s_isInited = false;

void AdsLogger::initConsoleLog() {
    log4cplus::Logger logger = log4cplus::Logger::getInstance(_ADS_LOOGER_NAME_);
    
    if (s_isInited) {
        LOG4CPLUS_DEBUG(logger, "Console logger is already initialized.");
        return;
    }

    CPPLogger::initConsoleLog(logger);
    s_isInited = true;
}

void AdsLogger::setLogLevel(Level level) {
    log4cplus::Logger logger = log4cplus::Logger::getInstance(_ADS_LOOGER_NAME_);

    CPPLogger::setLogLevel(logger, level);
}