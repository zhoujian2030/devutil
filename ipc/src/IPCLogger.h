/*
 * IPCLogger.h
 *
 *  Created on: Dec 07, 2015
 *      Author: z.j
 */
#ifndef _IPC_LOGGER_H_
#define _IPC_LOGGER_H_

#include "CPPLogger.h"

#define _IPC_LOOGER_NAME_ "IPC"
#define _IPC_LOGGER_ log4cplus::Logger::getInstance(_IPC_LOOGER_NAME_)

namespace ipc {

class IPCLogger {
public:
    static void initConsoleLog();
    static void setLogLevel(cm::Level level);

private:
    static bool s_isInited;
};

} // end of namespace ipc

#endif
