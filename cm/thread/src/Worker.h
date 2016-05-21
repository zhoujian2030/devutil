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
#include "Task.h"
#include "TaskQueue.h"
#include "EventIndicator.h"

#include <arpa/inet.h>
#include <iostream>

namespace cm {

    class WorkerPool;

    class Worker : public Thread {
    public:
        static Worker* getInstance(const sockaddr_in& remoteAddr);
        static void initialize();
        
        bool addTask(Task* theTask);

    private:
        friend class WorkerPool;

        Worker(int index);
        virtual ~Worker();
        
        int getIndex() const;
        virtual unsigned long run();
        
        // index for the worker thread, range 0 ... NUM_OF_WORKER_THREAD-1
        int m_index;
        TaskQueue m_taskQueue;
        EventIndicator m_taskChangeIndicator;

        static WorkerPool* m_workerPoolInstance;
        static MutexLock m_lock;
    };
    
    // ----------------------------------------
    // Add a new task into the worker's queue and send
    // out indication to notify the worker thread.
    inline bool Worker::addTask(Task* theTask) {
        bool result = m_taskQueue.addTask(theTask);
        m_taskChangeIndicator.set();
        return result;
    }
    
    // ----------------------------------------
    inline int Worker::getIndex() const {
        return m_index;
    }

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
    
    // ----------------------------------------
    // @param sockAddr - remote address
    // @return a worker according to remote ip and port
    inline Worker* WorkerPool::getWorker(const sockaddr_in& remoteAddr) {
        int hashValue = remoteAddr.sin_addr.s_addr + remoteAddr.sin_port;
        std::cout << "get worker " << (hashValue % m_numOfWorkers) << std::endl;
        return m_workerArray[hashValue % m_numOfWorkers];
    }   
}

#endif
