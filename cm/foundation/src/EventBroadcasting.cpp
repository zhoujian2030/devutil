/*
 * EventBroadcasting.cpp
 *
 *  Created on: Apr 22, 2016
 *      Author: z.j
 */

#include "EventBroadcasting.h"
#include "CMLogger.h"

using namespace cm;

// --------------------------------------------
EventBroadcasting::EventBroadcasting() 
: m_subscriberCnt(0), m_eventSet(0)
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

// --------------------------------------------
EventBroadcasting::~EventBroadcasting() {
    pthread_mutex_destroy(&m_mutex);
    pthread_cond_destroy(&m_condition);    
}

// --------------------------------------------
int EventBroadcasting::subscribe() {
    if (m_subscriberCnt < 32) {
        return m_subscriberCnt++;
    }

    return -1;
}

// --------------------------------------------
void EventBroadcasting::wait(int eventId) {
    LOG4CPLUS_DEBUG(_CM_LOOGER_NAME_, "EventBroadcasting::wait(), eventId = " << eventId);

    if (eventId < 0 || eventId > 31) {
        LOG4CPLUS_ERROR(_CM_LOOGER_NAME_, "Invalid eventId");
        return;
    }

    int result = pthread_mutex_lock(&m_mutex);
    if (result != 0) {
        LOG4CPLUS_ERROR(_CM_LOOGER_NAME_, "Fail to lock on mutex.");
        return;
    }

    int eventVal = (m_eventSet >> eventId) & 0x01;

    while (!eventVal) {       
        result = pthread_cond_wait(&m_condition, &m_mutex);
        if (result != 0) {
            LOG4CPLUS_ERROR(_CM_LOOGER_NAME_, "Fail to wait on condition.");
            pthread_mutex_unlock(&m_mutex);
            return;
        }
        eventVal = (m_eventSet >> eventId) & 0x01;
    }

    m_eventSet = ((0x01 << eventId) ^ 0xffffffff) & m_eventSet;
    pthread_mutex_unlock(&m_mutex);
}

// --------------------------------------------
void EventBroadcasting::notifyAll() {
    LOG4CPLUS_DEBUG(_CM_LOOGER_NAME_, "EventBroadcasting::notifyAll()");

    int result = pthread_mutex_lock(&m_mutex);
    if (result != 0) {
        LOG4CPLUS_ERROR(_CM_LOOGER_NAME_, "Fail to lock on mutex.");
        return;
    }        

    m_eventSet = 0xffffffff; 
    result = pthread_cond_broadcast(&m_condition);
    if (result != 0) {
        LOG4CPLUS_ERROR(_CM_LOOGER_NAME_, "Fail to broadcast.");
    }

    pthread_mutex_unlock(&m_mutex);
}

// --------------------------------------------
void EventBroadcasting::reset() {
    LOG4CPLUS_DEBUG(_CM_LOOGER_NAME_, "EventBroadcasting::reset()");

    int result = pthread_mutex_lock(&m_mutex);
    if (result != 0) {
        LOG4CPLUS_ERROR(_CM_LOOGER_NAME_, "Fail to lock on mutex.");
        return;
    }        

    m_eventSet = 0; 
    result = pthread_cond_broadcast(&m_condition);
    if (result != 0) {
        LOG4CPLUS_ERROR(_CM_LOOGER_NAME_, "Fail to broadcast.");
    }

    pthread_mutex_unlock(&m_mutex);
}