/*
 * CPPLogger.h
 *
 *  Created on: Dec 01, 2015
 *      Author: z.j
 */
#ifndef _CPP_LOGGER_H_
#define _CPP_LOGGER_H_
#include <log4cplus/logger.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/layout.h>

class CPPLogger {
public:
    CPPLogger();
    ~CPPLogger();

    void test();
};

#endif