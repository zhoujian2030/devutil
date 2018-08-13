/*
 * TestAsn1.h
 *
 *  Created on: Sep 18, 2017
 *      Author: j.zhou
 */
 
#ifndef TEST_ASN1
#define TEST_ASN1

#include "TestSuite.h"


class TestAsn1 : public TestSuite {
protected:
    virtual void SetUp() {
        std::cout << "TestAsn1::SetUp()" << std::endl;
    }
    
    virtual void TearDown() {
        std::cout << "TestAsn1::TearDown()" << std::endl;
    }    
};


 #endif