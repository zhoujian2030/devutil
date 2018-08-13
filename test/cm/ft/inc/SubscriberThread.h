/*
 * SubscriberThread.h
 *
 *  Created on: Apr 23, 2016
 *      Author: z.j
 */

#include "Thread.h"
#include "EventBroadcasting.h"
#include "MutexLock.h"
#include <string>

class SubscriberThread : public cm::Thread {
public:
    SubscriberThread(std::string threadName, cm::EventBroadcasting* pEventBroadcast);
    ~SubscriberThread();

    virtual unsigned long run();

private:
    int m_eventId;
    cm::EventBroadcasting* m_eventBroadcast;
};

inline SubscriberThread::SubscriberThread(std::string threadName, cm::EventBroadcasting* pEventBroadcast)
: Thread(threadName)
{
    m_eventBroadcast = pEventBroadcast;
    m_eventId = m_eventBroadcast->subscribe();
}

inline SubscriberThread::~SubscriberThread() {

}
