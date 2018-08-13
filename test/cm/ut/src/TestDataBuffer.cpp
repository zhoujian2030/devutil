/*
 * TestDataBuffer.cpp
 *
 *  Created on: Aug 13, 2018
 *      Author: J.ZH
 */

#include "TestDataBuffer.h"
#include "DataBuffer.h"
#include <iostream>

using namespace cm;
using namespace std;

TEST_F(TestDataBuffer, Normal_Test) {
    DataBuffer *pBuffer = new DataBuffer(10);
    delete pBuffer;
}