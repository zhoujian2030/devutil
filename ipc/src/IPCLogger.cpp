/*
 * IPCLogger.cpp
 *
 *  Created on: Dec 07, 2015
 *      Author: z.j
 */

#include "IPCLogger.h"

using namespace ipc;
using namespace cm;

bool IPCLogger::s_isInited = false;

void IPCLogger::initConsoleLog() {
    log4cplus::Logger logger = log4cplus::Logger::getInstance(_IPC_LOOGER_NAME_);
    
    if (s_isInited) {
        LOG4CPLUS_DEBUG(logger, "Console logger is already initialized.");
        return;
    }

    CPPLogger::initConsoleLog(logger);
    s_isInited = true;
}

void IPCLogger::setLogLevel(Level level) {
    log4cplus::Logger logger = log4cplus::Logger::getInstance(_IPC_LOOGER_NAME_);

    CPPLogger::setLogLevel(logger, level);
}