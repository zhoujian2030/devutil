/*
 * DemoThread.h
 *
 *  Created on: Apr 23, 2016
 *      Author: z.j
 */

#include "Thread.h"
#include "EventIndicator.h"
#include "MutexLock.h"

class ThreadA : public cm::Thread {
public:
    ThreadA();
    ~ThreadA();

    virtual unsigned long run();

    void setEvent();

private:
    cm::Lock* m_lock;
    cm::EventIndicator m_eventIndicator;
};

inline ThreadA::ThreadA()
: Thread("ThreadA")
{
    m_lock = new cm::MutexLock(true);
}

inline ThreadA::~ThreadA() {
    delete m_lock;
}
