/*
 * TestThread.cpp
 *
 *  Created on: Oct 17, 2017
 *      Author: J.ZH
 */

#include "TestThread.h"
#include <iostream>

using namespace cm;
using namespace std;

TEST_F(TestThread, SetThreadStackSize) {
    struct rlimit limit;
    getrlimit(RLIMIT_STACK, &limit);
    cout << "default stack size (ulimit -s): " << limit.rlim_cur << endl;

    m_thread = new MyThread();
    unsigned int curStackSize = m_thread->getStackSize();
    EXPECT_EQ(limit.rlim_cur, curStackSize);

    unsigned int stackSize = 20000;
    m_thread->setStackSize(stackSize);
    curStackSize = m_thread->getStackSize();
    EXPECT_EQ(curStackSize, stackSize);

    stackSize = 1000;
    m_thread->setStackSize(stackSize);
    curStackSize = m_thread->getStackSize();
    EXPECT_EQ(curStackSize, PTHREAD_STACK_MIN);

    m_thread->start();
    m_thread->wait();
}