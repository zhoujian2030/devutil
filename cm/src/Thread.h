/*
 * Thread.h
 *
 *  Created on: Feb 16, 2016
 *      Author: z.j
 */

#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>
#include <string>

namespace base {

    class Thread {
    public:

        // Start the thread, default is joinable
        bool start(bool isJoinable = true);

        //
        // Wait for termination of the thread. The function returns when the
        // thread has terminated.
        //
        bool wait();

        // return a joinable thread exit status code, only valid after returning true from wait() 
        long getExitStatus() const;

        // terminate (kill) the thread 
        void terminate();

        // return true if thread is running
        bool isRunning() const;

        // get thread name
        std::string getName() const;

        //
        // Return this POSIX thread id. pthread_self() returns the id of the
        // thread calling it, which may not be the same as getId()
        //
        unsigned long getId() const;

        // check if the thread is joinable or not
        bool isJoinable() const;

        // call pthread_detach to change a running thread to PTHREAD_CREATE_DETACHED state
        void detach();

        // sleep milliseconds
        static void sleep(int milli);

    protected:
        Thread(std::string theThreadName);

        virtual 
        ~Thread();

        virtual unsigned long run() = 0;

    private:
        // The start_routine of the thread
        static void* entry(void* theParameter);

        // Thread name
        std::string m_threadName;

        // Indicates if thread is running
        bool m_isRunning;
        // Thread properties
        pthread_attr_t  m_threadAttributes;
        // Thread ID
        pthread_t       m_threadHandle;

        bool m_isJoinable;

        long m_exitStatus;
    };

    // --------------------------
    inline long Thread::getExitStatus() const {
        return m_exitStatus;
    }

    // --------------------------
    inline std::string Thread::getName() const {
        return m_threadName;
    }

    // --------------------------
    inline unsigned long Thread::getId() const {
        if (m_isRunning) {
            return m_threadHandle;
        }

        return 0L;
    }

    // --------------------------
    inline bool Thread::isRunning() const {
        return m_isRunning;
    }

    // --------------------------
    inline bool Thread::isJoinable() const{
        return m_isJoinable;
    }

    // --------------------------
    inline void Thread::detach() {
        if (m_isRunning && m_isJoinable) {
            pthread_detach(m_threadHandle);
            m_isJoinable = false;
        }
    }
}


#endif
