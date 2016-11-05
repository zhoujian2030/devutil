/*
 * NetLogger.cpp
 *
 *  Created on: Apr 02, 2016
 *      Author: z.j
 */

#include "NetLogger.h"

using namespace net;
using namespace cm;

bool NetLogger::s_isInited = false;
cm::MutexLock NetLogger::m_lock;

void NetLogger::initConsoleLog() {
    log4cplus::Logger logger = log4cplus::Logger::getInstance(_NET_LOOGER_NAME_);
    
    m_lock.lock();
    if (s_isInited) {
        LOG4CPLUS_DEBUG(logger, "Console logger is already initialized.");
        m_lock.unlock();
        return;
    }

    CPPLogger::initConsoleLog(logger);
    CPPLogger::setLogLevel(logger, DEBUG);
    s_isInited = true;
    m_lock.unlock();
}

void NetLogger::setLogLevel(Level level) {
    log4cplus::Logger logger = log4cplus::Logger::getInstance(_NET_LOOGER_NAME_);

    CPPLogger::setLogLevel(logger, level);
}