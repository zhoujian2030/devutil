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
    LOG4CPLUS_DEBUG(_CM_LOOGER_NAME_, this->getName() << " " << this->getIndex() << " is running.");
    
    int result = TRC_CONTINUE;
    while (result == TRC_CONTINUE) {
        // if the task queue is empty, block and wait until there
        // new task available
        if (m_taskQueue.getLength() == 0) {
            m_taskChangeIndicator.wait();
        } 
        
        result = m_taskQueue.executeTask();          
    }

    LOG4CPLUS_INFO(_CM_LOOGER_NAME_, this->getName() << " " << this->getIndex() << " is exited.");
    return 0; 
}

// -----------------------------------------
WorkerPool::WorkerPool() 
: m_numOfWorkers(NUM_OF_WORKER_THREAD)
{
    CMLogger::initConsoleLog();
    
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

// -----------------------------------------
// @param sockAddr - remote address
// @return a worker according to remote ip and port
Worker* WorkerPool::getWorker(const sockaddr_in& remoteAddr) {
    int hashValue = remoteAddr.sin_addr.s_addr + remoteAddr.sin_port;
    LOG4CPLUS_DEBUG(_CM_LOOGER_NAME_, "get worker " << (hashValue % m_numOfWorkers));
    return m_workerArray[hashValue % m_numOfWorkers];
} 