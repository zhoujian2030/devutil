/*
 * DemoThread.h
 *
 *  Created on: Apr 23, 2016
 *      Author: z.j
 */

#include "Thread.h"
#include "EventIndicator.h"
#include "MutexLock.h"

class ThreadA : public base::Thread {
public:
    ThreadA();
    ~ThreadA();

    virtual unsigned long run();

    void setEvent();

private:
    base::Lock* m_lock;
    cm::EventIndicator m_eventIndicator;
};

inline ThreadA::ThreadA()
: Thread("ThreadA")
{
    m_lock = new base::MutexLock(true);
}

inline ThreadA::~ThreadA() {
    delete m_lock;
}
