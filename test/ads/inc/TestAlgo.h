/*
 * TestAlgo.h
 *
 *  Created on: Aug 15, 2018
 *      Author: J.ZH
 */
 
#ifndef TEST_ALGO_H
#define TEST_ALGO_H

#include "TestSuite.h"

class TestAlgo : public TestSuite {
protected:
    virtual void SetUp() {
        std::cout << "TestAlgo::SetUp()" << std::endl;
    }
    
    virtual void TearDown() {
        std::cout << "TestAlgo::TearDown()" << std::endl;
    }    
    
};

#endif
