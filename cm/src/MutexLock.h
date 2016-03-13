/*
 * MutexLock.h
 *
 *  Created on: Feb 25, 2016
 *      Author: z.j
 */

#ifndef MUTEX_LOCK_H
#define MUTEX_LOCK_H

#include <pthread.h>

#include "Lock.h"

namespace base {

    class MutexLock: public Lock {
    public:
        MutexLock();

        virtual 
        ~MutexLock();

        virtual void lock();
        virtual void unlock();

    private:
        pthread_mutex_t m_mutex;

    };

    // -------------------------------------
    inline MutexLock::MutexLock() {
        pthread_mutex_init(&m_mutex, NULL);
    }

    inline MutexLock::~MutexLock() {
        pthread_mutex_destroy(&m_mutex);
    }

    // -------------------------------------
    inline void MutexLock::lock() {
        pthread_mutex_lock(&m_mutex);
    }

    // -------------------------------------
    inline void MutexLock::unlock() {
        pthread_mutex_unlock(&m_mutex);
    }
}


#endif