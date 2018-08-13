/*
 * TestFile.h
 *
 *  Created on: May 18, 2016
 *      Author: j.zhou
 */
 
#ifndef TEST_FILE_H
#define TEST_FILE_H

#include "TestSuite.h"
#include "File.h"

class TestFile : public TestSuite {
protected:
    virtual void SetUp() {
        std::cout << "TestFile::SetUp()" << std::endl;
    }
    
    virtual void TearDown() {
        std::cout << "TestFile::TearDown()" << std::endl;
        //delete m_file;
    }    
    
    cm::File* m_file;
};

#endif
