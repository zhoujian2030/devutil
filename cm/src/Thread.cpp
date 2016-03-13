/*
 * Thread.cpp
 *
 *  Created on: Feb 16, 2016
 *      Author: z.j
 */
#include <iostream>
#include "Thread.h"

using namespace base;

Thread::Thread()
: 
m_isRunning(false), 
m_isJoinable(true)
{
    m_threadHandle = 0;
    // TODO
}

Thread::~Thread()
{
    terminate();
    // TODO
}

// ---------------------------------------------------
// Start the thread, return true if thread is started
// or it is already running
bool Thread::start(bool isJoinable) {
    if (!m_isRunning) {
        m_isJoinable = isJoinable;
        int result = pthread_attr_init(&m_threadAttributes);
        if (0 != result) {
            std::cout << "pthread_attr_init error: " << result << std::endl;
            return false;
        }

        // Other value is PTHREAD_SCOPE_PROCESS
        // Only need to set this attribute for real-time thread??
        pthread_attr_setscope(&m_threadAttributes, PTHREAD_SCOPE_SYSTEM);

        // If create a joinable thread, no need to this api explicitly
        if (m_isJoinable) {
            pthread_attr_setdetachstate(&m_threadAttributes, PTHREAD_CREATE_JOINABLE);
        } else {
            pthread_attr_setdetachstate(&m_threadAttributes, PTHREAD_CREATE_DETACHED);
        }

        result = pthread_create(&m_threadHandle, &m_threadAttributes, Thread::entry, this);
        if (result != 0) {
            std::cout << "pthread_create error: " << result << std::endl;
            return false;
        }

        m_isRunning = true;
    }

    return true;
}

// -----------------------------------------------
void Thread::terminate() {
    if (m_isRunning) {
        std::cout << "Teminating the thread : " << m_threadHandle << std::endl;
        
        int result = 0;
        if ((result = pthread_cancel(m_threadHandle)) != 0) {
            std::cout << "Error. pthread_cancel result code is " << result << std::endl;
        }
        if ((result = pthread_detach(m_threadHandle)) != 0) {
            std::cout << "Error. pthread_detach result code is " << result << std::endl;
        }
        
        m_isRunning = false;
    }
}

// -----------------------------------------------
long Thread::wait() {
    if (!m_isRunning) {
        return 0;
    }

    void *status;
    int result = pthread_join(m_threadHandle, &status);
    if (0 != result) {
        std::cout << "Error. return code from pthread_join is " << result << std::endl;
        return result;
    } else {
        return (long)status;
    }
}

// ----------------------------------------------------
// The start routine of the thread

void* Thread::entry(void* theParameter) {
    Thread* theThread =  (Thread*)theParameter;
    unsigned long result = theThread->run();
    std::cout << "Thread::entry return " << result << std::endl;
    return (void*)result;
}