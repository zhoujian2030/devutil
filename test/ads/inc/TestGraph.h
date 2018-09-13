/*
 * TestGraph.h
 *
 *  Created on: Sep 7, 2018
 *      Author: J.ZH
 */
 
#ifndef TEST_BFS_H
#define TEST_BFS_H

#include "TestSuite.h"

class TestGraph : public TestSuite {
protected:
    virtual void SetUp() {
        std::cout << "TestGraph::SetUp()" << std::endl;
    }
    
    virtual void TearDown() {
        std::cout << "TestGraph::TearDown()" << std::endl;
    }    
    
};

#endif
