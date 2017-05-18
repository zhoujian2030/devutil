/*
 * TaskQueue.cpp
 *
 *  Created on: May 21, 2016
 *      Author: z.j
 */
 
#include "TaskQueue.h"
#include "Task.h"
#include "CMLogger.h"

using namespace cm;
 
// ------------------------------------
TaskQueue::TaskQueue() 
: m_firstTask(0), m_lastTask(0), m_length(0)
{
    CMLogger::initConsoleLog();

    int result = pthread_mutex_init(&m_mutex, 0);
    if (result == 0) {
        result = pthread_cond_init(&m_condition, 0);
        if (result != 0) {
            LOG4CPLUS_ERROR(_CM_LOOGER_NAME_, "Fail to init pthread_cond_t"); 
            pthread_mutex_destroy(&m_mutex);
        }
    } else {
        LOG4CPLUS_ERROR(_CM_LOOGER_NAME_, "Fail to init pthread_mutex_t"); 
    }
}
 
// ------------------------------------
TaskQueue::~TaskQueue() {
    while (m_firstTask != 0) {
        Task* temp = m_firstTask->next;
        delete m_firstTask;
        m_firstTask = temp;
    }
    
    m_lastTask = 0;

    pthread_mutex_destroy(&m_mutex);
    pthread_cond_destroy(&m_condition);  
}

// ------------------------------------
int TaskQueue::executeTask() {          
    int result = pthread_mutex_lock(&m_mutex);
    if (result != 0) {
        LOG4CPLUS_ERROR(_CM_LOOGER_NAME_, "Fail to lock on mutex.");
        return TRC_END;
    }
    
    // block until there is available task in queue
    while (m_length == 0) {
        LOG4CPLUS_DEBUG(_CM_LOOGER_NAME_, "Empty queue, wait!");       
        result = pthread_cond_wait(&m_condition, &m_mutex);
        if (result != 0) {
            LOG4CPLUS_ERROR(_CM_LOOGER_NAME_, "Fail to wait on condition.");
            pthread_mutex_unlock(&m_mutex);
            return TRC_END;
        }
    }
    
    if (m_firstTask != 0) {
        Task* task = m_firstTask;
        m_firstTask = m_firstTask->next;
        if (m_firstTask == 0) {
            m_lastTask = 0;
        }
        m_length--;
        
        pthread_mutex_unlock(&m_mutex);
        if (result != 0) {
            LOG4CPLUS_ERROR(_CM_LOOGER_NAME_, "Fail to unlock mutex.");
            return TRC_END;
        }

        // execute the first task
        int result = task->execute();
        delete task;
        return result;
    } else {
        // should not happen
        LOG4CPLUS_WARN(_CM_LOOGER_NAME_, "No available task!");
        pthread_mutex_unlock(&m_mutex);
        return TRC_EMPTY;
    }
}
 
// ------------------------------------
bool TaskQueue::addTask(Task* theTask) {
    if (theTask == 0) {
        return false;
    }
    
    LOG4CPLUS_DEBUG(_CM_LOOGER_NAME_, "add new task into queue.");
    int result = pthread_mutex_lock(&m_mutex);
    if (result != 0) {
        LOG4CPLUS_ERROR(_CM_LOOGER_NAME_, "Fail to lock on mutex.");
        return false;
    } 
    
    if (m_lastTask == 0) {
        // the queue is empty
        m_firstTask = theTask;
        m_lastTask = theTask;
    } else {
        // the queue is not empty
        m_lastTask->next = theTask;
        m_lastTask = theTask;
    }
    
    m_length++;
    
    result = pthread_cond_signal(&m_condition);
    if (result != 0) {
        pthread_mutex_unlock(&m_mutex);
        LOG4CPLUS_ERROR(_CM_LOOGER_NAME_, "Fail to signal.");
        return false;
    }

    pthread_mutex_unlock(&m_mutex);
    if (result != 0) {
        LOG4CPLUS_ERROR(_CM_LOOGER_NAME_, "Fail to unlock mutex.");
        return false;
    }
    
    return true;
}
