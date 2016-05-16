/*
 * Worker.h
 *
 *  Created on: May 15, 2016
 *      Author: z.j
 */

#ifndef WORKER_H
#define WORKER_H

#include "Thread.h"
#include "MutexLock.h"

#include <arpa/inet.h>
#include <iostream>

namespace cm {

    class WorkerPool;

    class Worker : public Thread {
    public:
        static Worker* getInstance(const sockaddr_in& remoteAddr);
        static void initialize();

    private:
        friend class WorkerPool;

        Worker(int index);
        virtual ~Worker();

        virtual unsigned long run();

        int m_index;

        static WorkerPool* m_workerPoolInstance;
        static MutexLock m_lock;
    };

    // ----------------------------------------------
    class WorkerPool {

    private:
        friend class Worker;

        WorkerPool();
        ~WorkerPool();

        Worker* getWorker(const sockaddr_in& remoteAddr);

        enum {
            NUM_OF_WORKER_THREAD = 15
        };

        Worker** m_workerArray;
        int m_numOfWorkers;
    };

    // -----------------------------
    // @param sockAddr - remote address
    // @return a worker according to remote ip and port
    inline Worker* WorkerPool::getWorker(const sockaddr_in& remoteAddr) {
        int hashValue = remoteAddr.sin_addr.s_addr + remoteAddr.sin_port;
        std::cout << "get worker " << (hashValue % m_numOfWorkers) << std::endl;
        return m_workerArray[hashValue % m_numOfWorkers];
    }

}

#endif
