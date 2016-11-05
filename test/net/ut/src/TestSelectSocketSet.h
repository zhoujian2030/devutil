/*
 * TestSelectSocketSet.h
 *
 *  Created on: Nov 4, 2016
 *      Author: j.zhou
 */

#ifndef TEST_SELECT_SOCKET_SET_H
#define TEST_SELECT_SOCKET_SET_H

#include "TestSuite.h"
#include "SelectSocketSet.h"

class TestSelectSocketSet : public TestSuite {
protected:
    virtual void SetUp() {
        std::cout << "TestSelectSocketSet::SetUp()" << std::endl;
    }
    
    virtual void TearDown() {
        std::cout << "TestSelectSocketSet::TearDown()" << std::endl;
    }

    net::SelectSocketSet* m_selectSocketSet;
};

#endif
