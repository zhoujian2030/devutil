/*
 * Logger.cpp
 *
 *  Created on: May 24, 2017
 *      Author: j.zhou
 */

#include "Logger.h" 

using namespace cm;

// ----------------------------------------
Logger::Logger(std::string loggerName) 
: m_level(DEBUG), m_loggerName(loggerName)
{

}

// ----------------------------------------
Logger::~Logger() {
    
}

// ----------------------------------------
void Logger::log(std::string level, va_list ap) {
    
}

