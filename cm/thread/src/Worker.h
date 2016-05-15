/*
 * Worker.h
 *
 *  Created on: May 15, 2016
 *      Author: z.j
 */

#ifndef WORKER_H
#define WORKER_H

#include "Thread.h"

namespace cm {

    class Worker : public Thread {
    public:
        Worker();
        virtual ~Worker();

    private:
        virtual unsigned long run();

        
    };

    // ----------------------------------------------
    class WorkerPool {
    public:
        WorkerPool();
        ~WorkerPool();

    private:
        enum {
            NUM_OF_WORKER_THREAD = 15
        };

        Worker** m_workerArray;
    };

    // -----------------------------


}

#endif
