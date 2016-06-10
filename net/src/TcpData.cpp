/*
 * TcpData.cpp
 *
 *  Created on: June 10, 2016
 *      Author: z.j
 */

#include "TcpData.h"

using namespace net;

// ---------------------------------------------
TcpData::TcpData(unsigned long globalConnId)
: m_globalConnId(globalConnId) {

}

// ---------------------------------------------
TcpData::~TcpData() {

}

// ---------------------------------------------
const char* TcpData::getData(int& dataLength) {
    dataLength = m_data.length();

    // use the string as data buffer for data to be sent
    // MUST not re-write the string by the C pointer as it 
    // is not safe due to "copy-on-write" on string
    return m_data.c_str();
}