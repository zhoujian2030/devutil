/*
 * WorkerPool.cpp
 *
 *  Created on: May 26, 2016
 *      Author: z.j
 */

#include "WorkerPool.h"
#include "Worker.h"
#include "CMLogger.h"

using namespace cm;

// -----------------------------------------
WorkerPool::WorkerPool(int numOfWorkers) 
: m_numOfWorkers(numOfWorkers)
{
    CMLogger::initConsoleLog();
    
    m_workerArray = new Worker*[m_numOfWorkers];
    for (int i=0; i<m_numOfWorkers; i++) {
        Worker* worker = new Worker(i);
        worker->start();
        m_workerArray[i] = worker;
    }
    
    LOG4CPLUS_INFO(_CM_LOOGER_NAME_, "+------------------------------------+");
    LOG4CPLUS_INFO(_CM_LOOGER_NAME_, "+ Create " << m_numOfWorkers << " workers!!");
    LOG4CPLUS_INFO(_CM_LOOGER_NAME_, "+------------------------------------+");
}

// -----------------------------------------
WorkerPool::~WorkerPool() {
    for (int i=0; i<m_numOfWorkers; i++) {
        delete m_workerArray[i];
    }
    delete m_workerArray;
}

// -----------------------------------------
// @param index - index to find the worker
// @return a worker according to index
Worker* WorkerPool::getWorker(unsigned int index) {
    int id = index % m_numOfWorkers;
    LOG4CPLUS_DEBUG(_CM_LOOGER_NAME_, "get worker " << id);
    return m_workerArray[id];
} 

// -----------------------------------------
// @param sockAddr - remote address
// @return a worker according to remote ip and port
Worker* WorkerPool::getWorker(const sockaddr_in& remoteAddr) {
    int id = (remoteAddr.sin_addr.s_addr + remoteAddr.sin_port) % m_numOfWorkers;
    LOG4CPLUS_DEBUG(_CM_LOOGER_NAME_, "get worker " << id);
    return m_workerArray[id];
} 

