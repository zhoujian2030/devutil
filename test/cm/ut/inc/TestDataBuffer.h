/*
 * TestDataBuffer.h
 *
 *  Created on: Oct 17, 2018
 *      Author: J.ZH
 */
 
#ifndef TEST_DATA_BUFFER_H
#define TEST_DATA_BUFFER_H

#include "TestSuite.h"

class TestDataBuffer : public TestSuite {
protected:
    virtual void SetUp() {
        std::cout << "TestThread::SetUp()" << std::endl;
    }
    
    virtual void TearDown() {
        std::cout << "TestThread::TearDown()" << std::endl;
    }    

};

#endif
