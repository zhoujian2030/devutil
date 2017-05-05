/*
 * StrMain.cpp
 *
 *  Created on: Dec 15, 2015
 *      Author: z.j
 */

#include <string.h>
#include <string>
#include "CPPLogger.h"

using namespace std;
using namespace cm;

int main() {
    //char str[] = "This is. sample .string.";
    string str = "This is. sample .string.";
    char* token;

    log4cplus::Logger logger = CPPLogger::getLogger("StringTest");
    CPPLogger::initConsoleLog(logger);
    // CPPLogger::setLogLevel(logger, INFO);

    LOG4CPLUS_INFO(logger, "Spliting with strtok: \"" << str << "\", length = " << str.length());
    token = strtok((char*)str.c_str(), " .");
    while (token != NULL) {
        LOG4CPLUS_INFO(logger, token);
        LOG4CPLUS_DEBUG(logger, "str = " << str);
        token = strtok(NULL, " .");
    }
    LOG4CPLUS_INFO(logger, "+++++++++++++++++++++++++++++++++++++++++++++");

    char* saveptr;
    str = "This is. sample .string.";
    LOG4CPLUS_INFO(logger, "Spliting with strtok_r: \"" << str << "\", length = " << str.length());
    token = strtok_r((char*)str.c_str(), " .", &saveptr);
    while (token != NULL) {
        LOG4CPLUS_INFO(logger, token);
        LOG4CPLUS_DEBUG(logger, "saveptr: " << saveptr << "   str = " << str);
        token = strtok_r(NULL, " .", &saveptr);
    }

    return 0;
}
