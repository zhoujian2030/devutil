/*
 * PosixThreadDemo.cpp
 *
 *  Created on: Mar 02, 2016
 *      Author: z.j
 */
#include <string.h>
#include <errno.h>
#include "PosixThreadDemo.h"
#include "IPCLogger.h"

using namespace ipc;
using namespace base;

// --------------------------------

DemoThreadA::DemoThreadA()
: m_status(0)
{
    m_mutex = new MutexLock();
    IPCLogger::initConsoleLog();
}

DemoThreadA::~DemoThreadA() 
{
    delete m_mutex;
}

// --------------------------------
// For test to wait a detached thread return
void DemoThreadA::waitMutex() {
    LOG4CPLUS_INFO(_IPC_LOGGER_, "wait for thread unlock");
    m_mutex->lock();
    LOG4CPLUS_INFO(_IPC_LOGGER_, "thread is unlocked");
    m_mutex->unlock();
    usleep(500);
}

// ---------------------------------
unsigned long DemoThreadA::run() {
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Thread A is started and running");
    m_mutex->lock();
    sleep(2);
    m_mutex->unlock();
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Thread A completes job and returns " << m_status);
    return m_status;
}

// -------------------------------------------------------------------------------
unsigned long DemoThreadB::run() {
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Thread B is started and pthread_exit " << m_status);
    pthread_exit((void*)m_status);
    
    // DO something

    return m_status;
}

// ---------------------------------
unsigned long DemoThreadC::run() {
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Thread C is started and keep running");

    while (true) {
        sleep(10);
    }

    return m_status;
}

// -------------------------------------------------------------------------------
unsigned long DemoThreadD::run() {
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Thread D is started and running");
    
    sleep(2);

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Thread D completes job and returns " << m_status);
    return m_status;
}

// -------------------------------------------------------------------------------
PosixThreadDemo::PosixThreadDemo() {
    IPCLogger::initConsoleLog();
}

// -------------------------------------
PosixThreadDemo::~PosixThreadDemo() {
    if (pThread != NULL) {
        delete pThread;
        pThread = NULL;
    }
}

// -------------------------
void PosixThreadDemo::demoAll() {
    this->demoModifyThreadDetachState();
    this->demoJoinableThreadReturnNormal();
    this->demoJoinableThreadExitNormal();
    // this->demoNotJoinTheJoinableThread();
}

// ------------------------
void PosixThreadDemo::demoModifyThreadDetachState() {
    LOG4CPLUS_INFO(_IPC_LOGGER_, "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Demo Start: Test the detach and joinable proterty of thread" <<
        "\n\tA - Create joinable thread by default and pthread_detach the running thread success" <<
        "\n\tB - Create joinable thread by default and pthread_detach the thread success after it returns" <<
        "\n\tC - Create detached thread with pthread_attr_setdetachstate setting, pthread_detach the running thread fail" <<
        "\n\tD - Create detached thread and pthread_detach thread fail after it returns");
    LOG4CPLUS_INFO(_IPC_LOGGER_, "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");

    // -------------------------------------------
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Test A - Call pthread_create to create a joinable thread by default");
    pThread = new DemoThreadA();
    pThread->start();
    usleep(500);
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Check if thread is running: " << pThread->isRunning());
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Check if thread is joinable: " << pThread->isJoinable());
    if (!pThread->isRunning() || !pThread->isJoinable()) {
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "Test Failure!!!");
        exit(0);
    }

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Call pthread_detach to detach a running joinable thread");
    pThread->detach();
    int result = pthread_detach(pThread->getId());
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Call pthread_detach to detach the thread again, and return value should be unspecified: " << result);
    if (result == 0){
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "Test Failure!!!");
        exit(0);
    }

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Check again if thread is joinable: " << pThread->isJoinable());
    if (pThread->isJoinable()) {
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "Test Failure!!!");
        exit(0);
    }

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Delete the thread object, call pthread_cancel and pthread_detach to make sure thread resouce is recycled");    
    delete pThread;
    pThread = NULL;

    // -------------------------------------
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Test B - Call pthread_create to create a joinable thread");
    pThread = new DemoThreadA();
    pThread->start();
    usleep(500);

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Check if thread is running: " << pThread->isRunning());
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Check if thread is joinable: " << pThread->isJoinable());
    if (!pThread->isRunning() || !pThread->isJoinable()) {
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "Test Failure!!!");
        exit(0);
    }

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Wait until thread completes job and returns");
    pThread->waitMutex();

    // TODO test pthread_cancel a finished thread?
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Call pthread_detach to try detaching the finished thread and it should be also succussfull");
    result = pthread_detach(pThread->getId());
    if (result != 0){
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "Test Failure!!!");
        exit(0);
    }

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Delete the thread object, call pthread_cancel and pthread_detach to make sure thread resouce is recycled");    
    delete pThread;
    pThread = NULL;

    // -----------------------------------------
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Test C - Call pthread_create to create a detached thread (pthread_attr_setdetachstate PTHREAD_CREATE_DETACHED)");
    pThread = new DemoThreadA();
    pThread->start(false);
    usleep(500);

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Check if thread is running: " << pThread->isRunning());
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Check if thread is joinable: " << pThread->isJoinable());
    if (!pThread->isRunning() || pThread->isJoinable()) {
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "Test Failure!!!");
        exit(0);
    }

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Call pthread_detach to try detaching the thread and return value should be unspecified: " << result);
    result = pthread_detach(pThread->getId());
    if (result == 0){
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "Test Failure!!!");
        exit(0);
    }    
    
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Delete the thread object, call pthread_cancel and pthread_detach to make sure thread resouce is recycled");    
    delete pThread;
    pThread = NULL;

    // -------------------------------------
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Test D - Call pthread_create to create a detached thread");
    pThread = new DemoThreadA();
    pThread->start(false);
    usleep(500);

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Check if thread is running: " << pThread->isRunning());
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Check if thread is joinable: " << pThread->isJoinable());
    if (!pThread->isRunning() || pThread->isJoinable()) {
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "Test Failure!!!");
        exit(0);
    }

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Wait until thread completes job and returns");
    pThread->waitMutex();

    // TODO test pthread_cancel a finished thread?
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Call pthread_detach to try detaching the finished thread and return value should be unspecified: " << result);
    result = pthread_detach(pThread->getId());
    if (result == 0){
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "Test Failure!!!");
        exit(0);
    }

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Delete the thread object, call pthread_cancel and pthread_detach to make sure thread resouce is recycled");    
    delete pThread;
    pThread = NULL;

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Demo End");
}

// -------------------------
void PosixThreadDemo::demoJoinableThreadReturnNormal() {
    LOG4CPLUS_INFO(_IPC_LOGGER_, "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Demo Start: Main process creates a joinable Thread and wait" <<
        "\n\tMain Process creates thread with default joinable property" <<
        "\n\tAfter thread is started, main process calls pthread_join to wait thread exit" <<
        "\n\tThread completes work and returns value 100 in the end normally" <<
        "\n\tMain process gets the thread status 100 from pthread_join");
    LOG4CPLUS_INFO(_IPC_LOGGER_, "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Call pthread_create to create a thread and let it start running");
    pThread = new DemoThreadA();
    pThread->setStatus(100);
    pThread->start();

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Call pthread_join to wait the thread exit");
    long status = pThread->wait();

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Get thread exit status from pthread_join: " << status);

    delete pThread;
    pThread = NULL;

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Demo End");
}

// ------------------------
void PosixThreadDemo::demoJoinableThreadExitNormal() {
    LOG4CPLUS_INFO(_IPC_LOGGER_, "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Demo Start: Main process creates a joinable Thread and wait" <<
        "\n\tMain Process creates thread with default joinable property" <<
        "\n\tAfter thread is started, main process calls pthread_join to wait thread exit" <<
        "\n\tThread calls pthread_exit to exit 101 in some time" <<
        "\n\tMain process gets the thread status 101 from pthread_join");
    LOG4CPLUS_INFO(_IPC_LOGGER_, "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Call pthread_create to create a thread and let it start running");
    pThread = new DemoThreadB();
    pThread->setStatus(101);
    pThread->start();

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Call pthread_join to wait the thread exit");
    long status = pThread->wait();

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Get thread exit status from pthread_join: " << status);

    delete pThread;
    pThread = NULL;

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Demo End");
}



// --------------------------
void PosixThreadDemo::demoNotJoinTheJoinableThread() {

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Process " << getpid() << " calls pthread_create to create a thread and let it start running");
    
    IPCLogger::setLogLevel(logcpp::INFO);

    long count = 0;
    while (true) {
        pThread = new DemoThreadA();
        if (!pThread->start()) {
            LOG4CPLUS_ERROR(_IPC_LOGGER_, "Fail to creat more thread, count = " << count << ", errno = " << errno 
                << " - " << strerror(errno));
            break;
        }
        count ++;

        LOG4CPLUS_INFO(_IPC_LOGGER_, "Count: " << count);
        LOG4CPLUS_INFO(_IPC_LOGGER_, "sleep 1ms to let new thread running");
        usleep(1);
        LOG4CPLUS_INFO(_IPC_LOGGER_, "sleep 10ms then wait for thread exit");
        usleep(10);
        pThread->waitMutex();
        // pThread->wait();
        LOG4CPLUS_INFO(_IPC_LOGGER_, "Thread completes job and returns");
        delete pThread;
    }

    while(true) {
        sleep(1);
    }
}
