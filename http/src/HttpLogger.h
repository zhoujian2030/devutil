/*
 * HttpLogger.h
 *
 *  Created on: July 11, 2016
 *      Author: z.j
 */
 
#ifndef _HTTP_LOGGER_H_
#define _HTTP_LOGGER_H_

#include "CPPLogger.h"

#define _HTTP_LOOGER_NAME_ "HTTP"
#define _HTTP_LOGGER_ log4cplus::Logger::getInstance(_HTTP_LOOGER_NAME_)

namespace http {

class HttpLogger {
public:
    static void initConsoleLog();
    static void setLogLevel(cm::Level level);

private:
    static bool s_isInited;
};

} // end of namespace net

#endif
