/*
 * TestThread.h
 *
 *  Created on: Oct 17, 2017
 *      Author: J.ZH
 */
 
#ifndef TEST_THREAD_H
#define TEST_THREAD_H

#include "TestSuite.h"
#include "MyThread.h"

class TestThread : public TestSuite {
protected:
    virtual void SetUp() {
        std::cout << "TestThread::SetUp()" << std::endl;
    }
    
    virtual void TearDown() {
        std::cout << "TestThread::TearDown()" << std::endl;
    }    
    
    MyThread* m_thread;
};

#endif
