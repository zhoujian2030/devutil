#include <iostream>
#include "TestThread.h"

// --------------------------------

// ---------------------------------
TestThread::TestThread()
: Thread("Test Thread")
{

}

unsigned long TestThread::run() {
    std::cout << this->getName() << " is running" << std::endl;
    std::cout << "sleep 1s ..." << std::endl;
    sleep(1);
    pthread_exit((void*)10);
    std::cout << "exit!" << std::endl;
    return 3;
}