/*
 * Thread.h
 *
 *  Created on: Feb 16, 2016
 *      Author: z.j
 */

#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>

namespace base {

    class Thread {
    public:

        // Start the thread
        bool start(bool isJoinable = true);

        //
        // Wait for termination of the thread. The function returns when the
        // thread has terminated.
        //
        long wait();

        // Kill the thread
        void terminate();

        // Return true if thread is running
        bool isRunning() const;

        //
        // Return this POSIX thread id. pthread_self() returns the id of the
        // thread calling it, which may not be the same as getId()
        //
        unsigned long getId() const;

        // check if the thread is joinable or not
        bool isJoinable();

        // change a running thread to PTHREAD_CREATE_DETACHED state
        void detach();

    protected:
        Thread();

        virtual 
        ~Thread();

        virtual unsigned long run() = 0;

    private:
        // The start_routine of the thread
        static void* entry(void* theParameter);

        // Indicates if thread is running
        bool m_isRunning;
        // Thread properties
        pthread_attr_t  m_threadAttributes;
        // Thread ID
        pthread_t       m_threadHandle;

        bool m_isJoinable;
    };

    // --------------------------
    inline unsigned long Thread::getId() const {
        return m_threadHandle;
    }

    // --------------------------
    inline bool Thread::isRunning() const {
        return m_isRunning;
    }

    // --------------------------
    inline bool Thread::isJoinable() {
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
