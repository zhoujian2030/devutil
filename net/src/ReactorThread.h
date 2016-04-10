/*
 * ReactorThread.h
 *
 *  Created on: Apr 02, 2016
 *      Author: z.j
 */

#ifndef REACTOR_THREAD_H
#define REACTOR_THREAD_H

#include "Thread.h"
#include "MutexLock.h"

namespace net {
    class ReactorThread : public base::Thread {
    public:
        ReactorThread();

        virtual ~ReactorThread();

        virtual unsigned long run();

    private:
        base::Lock* m_lock;
    };
}

#endif