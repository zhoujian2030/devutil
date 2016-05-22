/*
 * AdsLogger.h
 *
 *  Created on: Apr 17, 2016
 *      Author: z.j
 */
#ifndef _ADS_LOGGER_H_
#define _ADS_LOGGER_H_

#include "CPPLogger.h"

#define _ADS_LOOGER_NAME_ "ADS"
#define _ADS_LOGGER_ log4cplus::Logger::getInstance(_ADS_LOOGER_NAME_)

namespace ads {

class AdsLogger {
public:
    static void initConsoleLog();
    static void setLogLevel(cm::Level level);

private:
    static bool s_isInited;
};

} // end of namespace ads

#endif