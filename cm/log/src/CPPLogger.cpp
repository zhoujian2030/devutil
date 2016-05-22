/*
 * CPPLogger.cpp
 *
 *  Created on: Dec 01, 2015
 *      Author: z.j
 */
#include "CPPLogger.h"

using namespace cm;
using namespace std;
using namespace log4cplus;
using namespace log4cplus::helpers;

Logger CPPLogger::getLogger(const tstring& loggerName) {
    return Logger::getInstance(loggerName);
}

void CPPLogger::initConsoleLog(const tstring& loggerName) {
    SharedObjectPtr<Appender> appender(new ConsoleAppender());
    string pattern = "[%D{%Y-%m-%d %H:%M:%S.%q}] [%-5p] [%c] [%t] - %m [%l] %n";
    auto_ptr<Layout> layout(new PatternLayout(pattern));
    appender->setLayout(layout);

    Logger logger = Logger::getInstance(loggerName);
    logger.addAppender(appender);
    logger.setLogLevel(ALL_LOG_LEVEL);

    LOG4CPLUS_INFO(logger, "Init console logger complete.");  
}

void CPPLogger::initConsoleLog(Logger& logger) {
    SharedObjectPtr<Appender> appender(new ConsoleAppender());
    string pattern = "[%D{%Y-%m-%d %H:%M:%S.%q}] [%-5p] [%c] [%t] - %m [%l] %n";
    auto_ptr<Layout> layout(new PatternLayout(pattern));
    appender->setLayout(layout);

    logger.addAppender(appender);
    logger.setLogLevel(ALL_LOG_LEVEL);

    LOG4CPLUS_INFO(logger, "Init console logger complete.");   
}

void CPPLogger::setLogLevel(const tstring& loggerName, LogLevel level) {
    Logger logger = Logger::getInstance(loggerName);
    logger.setLogLevel(level);
    LOG4CPLUS_INFO(logger, "Set LogLevel: " << level); 
}

void CPPLogger::setLogLevel(Logger& logger, LogLevel level) {
    logger.setLogLevel(level);
    LOG4CPLUS_INFO(logger, "Set LogLevel: " << level); 
}