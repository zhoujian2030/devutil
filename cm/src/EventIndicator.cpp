/*
 * EventIndicator.cpp
 *
 *  Created on: Apr 22, 2016
 *      Author: z.j
 */

#include "EventIndicator.h"
#include <iostream>

using namespace std;
using namespace cm;

// --------------------------------------------
EventIndicator::EventIndicator(bool eventIsSet) 
: m_eventIsSet(eventIsSet)
{
    int result = pthread_mutex_init(&m_mutex, 0);
    if (result == 0) {
        result = pthread_cond_init(&m_condition, 0);
        if (result != 0) {
            cout << "ERROR. Fail to init pthread_cond_t" << endl; 
            pthread_mutex_destroy(&m_mutex);
        }
    } else {
        cout << "ERROR. Fail to init pthread_mutex_t" << endl; 
    }
}

// --------------------------------------------
EventIndicator::~EventIndicator() {
    pthread_mutex_destroy(&m_mutex);
    pthread_cond_destroy(&m_condition);    
}

// --------------------------------------------
void EventIndicator::wait() {
#ifdef DEBUG_CM
    cout << "EventIndicator::wait()" << endl;
#endif

    int result = pthread_mutex_lock(&m_mutex);
    if (result != 0) {
        cout << "ERROR. Fail to lock on mutex." << endl;
        return;
    }

    while (!m_eventIsSet) {       
        result = pthread_cond_wait(&m_condition, &m_mutex);
        if (result != 0) {
            cout << "ERROR. Fail to wait on condition." << endl;
            pthread_mutex_unlock(&m_mutex);
            return;
        }
    }

    m_eventIsSet = false;
    pthread_mutex_unlock(&m_mutex);
}

// --------------------------------------------
void EventIndicator::set() {
#ifdef DEBUG_CM
    cout << "EventIndicator::set()" << endl;
#endif

    int result = pthread_mutex_lock(&m_mutex);
    if (result != 0) {
        cout << "ERROR. Fail to lock on mutex." << endl;
        return;
    }        

    m_eventIsSet = true; 
    result = pthread_cond_signal(&m_condition);
    if (result != 0) {
        cout << "ERROR. Fail to signal." << endl;
    }

    pthread_mutex_unlock(&m_mutex);
}