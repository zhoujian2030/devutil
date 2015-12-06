#include "CPPLogger.h"

using namespace std;
using namespace logcpp;

int main() {
    log4cplus::Logger logger_1 = CPPLogger::getLogger("TestLogger_1");
    CPPLogger::initConsoleLog(logger_1);

    log4cplus::Logger logger_2 = CPPLogger::getLogger("TestLogger_2");
    CPPLogger::initConsoleLog(logger_2);
    CPPLogger::setLogLevel(logger_2, INFO);

    int n = 1;
    LOG4CPLUS_TRACE(logger_1, "Init and start to test log4cplus: " << n);
    n++;
    LOG4CPLUS_DEBUG(logger_1, "Init and start to test log4cplus: " << n);
    n++;
    LOG4CPLUS_INFO(logger_1, "Init and start to test log4cplus: " << n);
    n++;
    LOG4CPLUS_DEBUG(logger_2, "Init and start to test log4cplus: " << n); // will not print out
    n++;
    LOG4CPLUS_INFO(logger_2, "Init and start to test log4cplus: " << n);

    CPPLogger::setLogLevel(logger_1, DEBUG);
    CPPLogger::setLogLevel(logger_2, DEBUG);
    n++;
    LOG4CPLUS_TRACE(logger_1, "Init and start to test log4cplus: " << n);
    n++;
    LOG4CPLUS_DEBUG(logger_1, "Init and start to test log4cplus: " << n);
    n++;    
    LOG4CPLUS_DEBUG(logger_2, "Init and start to test log4cplus: " << n); 
}