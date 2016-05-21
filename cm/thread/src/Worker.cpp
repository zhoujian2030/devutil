/*
 * Worker.cpp
 *
 *  Created on: May 15, 2016
 *      Author: z.j
 */

#include "Worker.h"

using namespace cm;
using namespace std;

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
    if (m_workerPoolInstance == 0) {
        m_lock.lock();
        if (m_workerPoolInstance == 0) {
            WorkerPool* temp = new WorkerPool();
            m_workerPoolInstance = temp;
        }
        m_lock.unlock();
    }

    return m_workerPoolInstance->getWorker(remoteAddr);
}

// ----------------------------------------
void Worker::initialize() {
    if (m_workerPoolInstance == 0) {
        m_lock.lock();
        if (m_workerPoolInstance == 0) {
            WorkerPool* temp = new WorkerPool();
            m_workerPoolInstance = temp;
        }
        m_lock.unlock();
    }    
}

// ----------------------------------------
unsigned long Worker::run() {
    std::cout << this->getName() << " " << this->getIndex() << " is running." << endl;
    
    int result = TRC_CONTINUE;
    while (result == TRC_CONTINUE) {
        // if the task queue is empty, block and wait until there
        // new task available
        if (m_taskQueue.getLength() == 0) {
            m_taskChangeIndicator.wait();
        } 
        
        result = m_taskQueue.executeTask();          
    }

    return 0; 
}

// -----------------------------------------
WorkerPool::WorkerPool() 
: m_numOfWorkers(NUM_OF_WORKER_THREAD)
{
    m_workerArray = new Worker*[m_numOfWorkers];
    for (int i=0; i<m_numOfWorkers; i++) {
        Worker* worker = new Worker(i);
        worker->start();
        m_workerArray[i] = worker;
    }
}

// -----------------------------------------
WorkerPool::~WorkerPool() {
    for (int i=0; i<m_numOfWorkers; i++) {
        delete m_workerArray[i];
    }
    delete m_workerArray;
}