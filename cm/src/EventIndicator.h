/*
 * EventIndicator.h
 *
 *  Created on: Apr 19, 2016
 *      Author: z.j
 */

#ifndef EVENT_INDICATOR_H
#define EVENT_INDICATOR_H

#include <pthread.h>

namespace cm {
    class EventIndicator {
    public:
        EventIndicator(bool eventIsSet = false);
        virtual ~EventIndicator();

        void wait();

        void set();

    private: 
        pthread_cond_t m_condition;
        pthread_mutex_t m_mutex;
        bool m_eventIsSet;
    };
}

#endif