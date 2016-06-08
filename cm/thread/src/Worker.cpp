/*
 * Worker.cpp
 *
 *  Created on: May 15, 2016
 *      Author: z.j
 */

#include "Worker.h"
#include "CMLogger.h"

using namespace cm;

WorkerPool* Worker::m_workerPoolInstance = 0;
MutexLock Worker::m_lock;

// ----------------------------------------
Worker::Worker(int index)
: Thread("WorkerThread"),
  m_index(index)
{
    
}

// ----------------------------------------
Worker::~Worker() {

}

// ----------------------------------------
Worker* Worker::getInstance(const sockaddr_in& remoteAddr) {
    initialize();
    return m_workerPoolInstance->getWorker(remoteAddr);
}

// ----------------------------------------
Worker* Worker::getInstance(unsigned int index) {
    initialize();
    return m_workerPoolInstance->getWorker(index);
}

// ----------------------------------------
void Worker::initialize(int numOfWorkers) {
    if (m_workerPoolInstance == 0) {
        m_lock.lock();
        if (m_workerPoolInstance == 0) {
            WorkerPool* temp = new WorkerPool(numOfWorkers);
            m_workerPoolInstance = temp;
        }
        m_lock.unlock();
    }    
}

// ----------------------------------------
unsigned long Worker::run() {
    LOG4CPLUS_DEBUG(_CM_LOOGER_NAME_, this->getName() << " " << this->getIndex() << " is running.");

    // fix the race condition when reactor thread receives
    // new connection before worker thread come up to wait
    // the event if queue is empty
    // the issue is that, the reactor adds task to worker's
    // queue and set the event indicator, then worker thread
    // starts to run, and handle the new task without waiting
    // event as the queue is not empty. but it will wait the
    // event in next loop as the queue becomes empty, then it
    // receives empty queue event and exits running    
    m_taskChangeIndicator.reset();

    int result = TRC_CONTINUE;
    while (result == TRC_CONTINUE) {
        // if the task queue is empty, block and wait until there 
        // is new task available
        if (m_taskQueue.getLength() == 0) {
            m_taskChangeIndicator.wait();
        } 
        
        result = m_taskQueue.executeTask();        
    }

    LOG4CPLUS_INFO(_CM_LOOGER_NAME_, this->getName() << " " << this->getIndex() << " is exited.");
    return 0; 
}
