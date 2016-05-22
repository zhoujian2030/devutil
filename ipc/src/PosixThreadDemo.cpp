/*
 * PosixThreadDemo.cpp
 *
 *  Created on: Mar 02, 2016
 *      Author: z.j
 */
#include <string.h>
#include <errno.h>
#include <iostream>
#include "PosixThreadDemo.h"
#include "IPCLogger.h"

using namespace ipc;
using namespace cm;

// --------------------------------

DemoThreadA::DemoThreadA()
: 
Thread("Demo Thread A"),
m_status(0)
{
    m_mutex = new MutexLock();
    IPCLogger::initConsoleLog();
}

DemoThreadA::DemoThreadA(std::string theThreadName)
: 
Thread(theThreadName),
m_status(0)
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
    Thread::sleep(10);
}

// ---------------------------------
unsigned long DemoThreadA::run() {
    LOG4CPLUS_INFO(_IPC_LOGGER_, this->getName() << " is started and running");
    m_mutex->lock();
    Thread::sleep(1000);
    m_mutex->unlock();
    LOG4CPLUS_INFO(_IPC_LOGGER_, this->getName() << " completes job and returns " << m_status);
    return m_status;
}

// -------------------------------------------------------------------------------
DemoThreadB::DemoThreadB()
: DemoThreadA("Demo Thread B")
{

}

unsigned long DemoThreadB::run() {
    LOG4CPLUS_INFO(_IPC_LOGGER_, this->getName() << " is started and pthread_exit " << m_status);
    pthread_exit((void*)m_status);
    
    // DO something

    return m_status;
}

// ---------------------------------
DemoThreadC::DemoThreadC()
: DemoThreadA("Demo Thread C")
{

}
unsigned long DemoThreadC::run() {
    LOG4CPLUS_INFO(_IPC_LOGGER_, this->getName() << " is started and keep running");

    while (true) {
        Thread::sleep(10000);
    }

    return m_status;
}

// -------------------------------------------------------------------------------
DemoThreadD::DemoThreadD()
: DemoThreadA("Demo Thread D")
{

}

unsigned long DemoThreadD::run() {
    LOG4CPLUS_INFO(_IPC_LOGGER_, this->getName() << " is started and running");
    
    Thread::sleep(2000);

    LOG4CPLUS_INFO(_IPC_LOGGER_, this->getName() << " completes job and returns " << m_status);
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
    IPCLogger::setLogLevel(cm::INFO);
    this->demoModifyThreadDetachState();
    std::cout << "------------------------------------------------------------------" << std::endl << std::endl;
    demoCancelThread();
    std::cout << "------------------------------------------------------------------" << std::endl << std::endl;
    this->demoJoinableThreadReturnNormal();
    std::cout << "------------------------------------------------------------------" << std::endl << std::endl;
    this->demoJoinableThreadExitNormal();
    std::cout << "------------------------------------------------------------------" << std::endl << std::endl;
    this->demoNotJoinTheJoinableThread();
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
    Thread::sleep(100);
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

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Check again and the thread should not be joinable: " << pThread->isJoinable());
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
    Thread::sleep(100);

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Check if thread is running: " << pThread->isRunning());
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Check if thread is joinable: " << pThread->isJoinable());
    if (!pThread->isRunning() || !pThread->isJoinable()) {
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "Test Failure!!!");
        exit(0);
    }

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Wait until thread completes job and returns");
    pThread->waitMutex();

    // TODO test pthread_cancel a finished thread?
    result = pthread_detach(pThread->getId());
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Call pthread_detach to try detaching the finished thread and it should be also succussfull: " << result);
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
    Thread::sleep(100);

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Check if thread is running: " << pThread->isRunning());
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Check if thread is joinable: " << pThread->isJoinable());
    if (!pThread->isRunning() || pThread->isJoinable()) {
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "Test Failure!!!");
        exit(0);
    }

    result = pthread_detach(pThread->getId());
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Call pthread_detach to try detaching the detached running thread and return value should be unspecified: " 
        << result);
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
    Thread::sleep(100);

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Check if thread is running: " << pThread->isRunning());
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Check if thread is joinable: " << pThread->isJoinable());
    if (!pThread->isRunning() || pThread->isJoinable()) {
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "Test Failure!!!");
        exit(0);
    }

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Wait until thread completes job and returns");
    pThread->waitMutex();

    // TODO test pthread_cancel a finished thread?
    result = pthread_detach(pThread->getId());
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Call pthread_detach to try detaching the finished detached thread and return value should be unspecified: " 
        << result);
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
void PosixThreadDemo::demoCancelThread() {
    LOG4CPLUS_INFO(_IPC_LOGGER_, "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Demo Start: Test cancel a thread" <<
        "\n\tA - pthread_cancel a joinable running thread success, then pthread_join the canceled thread success" <<
        "\n\tB - pthread_cancel a joinable running thread success, then pthread_detach the canceled thread success" <<
        "\n\tC - A joinable thread exited normally, then pthread_cancel the thread success, pthread_join also sucess");
    LOG4CPLUS_INFO(_IPC_LOGGER_, "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Test A - Call pthread_create to create a joinable thread by default and let it keep running");
    pThread = new DemoThreadC();
    pThread->start();
    Thread::sleep(100);

    LOG4CPLUS_INFO(_IPC_LOGGER_, "The thread should be running: " << pThread->isRunning() 
        << ", and joinable: " << pThread->isJoinable());
    if (!pThread->isRunning() || !pThread->isJoinable()) {
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "Test Failure!!!");
        exit(0);
    }

    int result  = pthread_cancel(pThread->getId());
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Call pthread_cancel to cancel the running thread success: " << result);
    if (result != 0) {
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "Test Failure!!!");
        exit(0);
    }

    bool joinResult = pThread->wait();
    long exitStatus = pThread->getExitStatus();
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Call pthread_join to join the canceled thread and get the thread exit status as PTHREAD_CANCELED: " << exitStatus);
    if(!joinResult || exitStatus != (long)PTHREAD_CANCELED) {
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "Test Failure!!!");
        exit(0);
    }

    delete pThread;
    pThread = NULL;

    // --------------------------------------------
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Test B - Call pthread_create to create a joinable thread and let it keep running");
    pThread = new DemoThreadC();
    pThread->start();
    Thread::sleep(100);

    LOG4CPLUS_INFO(_IPC_LOGGER_, "The thread should be running: " << pThread->isRunning() 
        << ", and joinable: " << pThread->isJoinable());
    if (!pThread->isRunning() || !pThread->isJoinable()) {
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "Test Failure!!!");
        exit(0);
    }

    result  = pthread_cancel(pThread->getId());
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Call pthread_cancel to cancel the running thread success: " << result);
    if (result != 0) {
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "Test Failure!!!");
        exit(0);
    }

    result = pthread_detach(pThread->getId());
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Call pthread_detach to detach the canceled thread and it should success");
    if (result != 0) {
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "Test Failure!!!");
        exit(0);
    }   

    delete pThread;
    pThread = NULL;

    // -----------------------------------
    long expectedExitStatus = 102;
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Test C - Call pthread_create to create a joinable thread and the pthread returns " 
        << expectedExitStatus);
    pThread = new DemoThreadA();
    pThread->setStatus(expectedExitStatus);
    pThread->start();
    Thread::sleep(100);

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Wait until the thread exit normally and then try to cancel the thread");
    pThread->waitMutex();

    result = pthread_cancel(pThread->getId());
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Call pthread_cancel to cancel the exited thread fail: " << result);
    if (result == 0) {
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "Test Failure!!!");
        exit(0);
    }

    joinResult = pThread->wait();
    exitStatus = pThread->getExitStatus();
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Call pthread_join to join the exited thread and get the thread exit status as : " 
        << exitStatus);
    if(!joinResult || exitStatus != expectedExitStatus) {
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "Test Failure!!!");
        exit(0);
    }    

    delete pThread;
    pThread = NULL;

    // TODO cancel a thread that not allow cansellation ?

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

    long exitStatus = 100;
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Call pthread_create to create a thread and let it start running and return " << exitStatus);
    pThread = new DemoThreadA();
    pThread->setStatus(exitStatus);
    pThread->start();

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Call pthread_join to wait the thread exit");
    if (!pThread->wait()) {
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "Test Failure!!!");
        exit(0);
    }

    LOG4CPLUS_INFO(_IPC_LOGGER_, "The thread exit status from pthread_join should be " << exitStatus);
     if (pThread->getExitStatus() != exitStatus) {
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "Test Failure!!!");
        exit(0);
    }   

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

    long exitStatus = 101;
    LOG4CPLUS_INFO(_IPC_LOGGER_, "Call pthread_create to create a thread and let it start running");
    pThread = new DemoThreadB();
    pThread->setStatus(exitStatus);
    pThread->start();

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Call pthread_join to wait the thread exit");
    if (!pThread->wait()) {
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "Test Failure!!!");
        exit(0);
    }

    LOG4CPLUS_INFO(_IPC_LOGGER_, "The thread exit status from pthread_join should be " << exitStatus);
     if (pThread->getExitStatus() != exitStatus) {
        LOG4CPLUS_ERROR(_IPC_LOGGER_, "Test Failure!!!");
        exit(0);
    }

    delete pThread;
    pThread = NULL;

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Demo End");
}



// --------------------------
void PosixThreadDemo::demoNotJoinTheJoinableThread() {

    LOG4CPLUS_INFO(_IPC_LOGGER_, "Process " << getpid() << " calls pthread_create to create a thread and let it start running");
    
    IPCLogger::setLogLevel(cm::INFO);

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
        Thread::sleep(1000);
        LOG4CPLUS_INFO(_IPC_LOGGER_, "sleep 10ms then wait for thread exit");
        Thread::sleep(100000);
        pThread->waitMutex();
        // pThread->wait();
        LOG4CPLUS_INFO(_IPC_LOGGER_, "Thread completes job and returns");
        delete pThread;
    }

    while(true) {
        Thread::sleep(1000);
    }
}
