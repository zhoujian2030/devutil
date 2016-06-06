/*
 * DataBuffer.cpp
 *
 *  Created on: Apr 08, 2016
 *      Author: z.j
 */

#include "DataBuffer.h"

#include <string.h>

using namespace cm;

// -----------------------------------------------
DataBuffer::DataBuffer() 
: m_bufferSize(INITIAL_BUFFER_SIZE)
{
    m_buffer = new char[m_bufferSize];
    m_endOfDataPointer = m_buffer;
}

// -----------------------------------------------
DataBuffer::DataBuffer(int bufferLength) 
: m_bufferSize(bufferLength)
{
    m_buffer = new char[m_bufferSize];
    m_endOfDataPointer = m_buffer;
}

// -----------------------------------------------
DataBuffer::DataBuffer(const DataBuffer& theDataBuffer) {
    m_bufferSize = theDataBuffer.m_bufferSize;
    m_buffer = new char[m_bufferSize];
    m_endOfDataPointer = m_buffer;
    
    memcpy(m_buffer, theDataBuffer.getStartOfDataPointer(), theDataBuffer.getLength());
    this->increaseDataLength(theDataBuffer.getLength());
}

// -----------------------------------------------
DataBuffer::DataBuffer(std::string bufferStr) {
    m_bufferSize = bufferStr.length();
    m_buffer = new char[m_bufferSize];
    m_endOfDataPointer = m_buffer;
    
    memcpy(m_buffer, (void*)bufferStr.c_str(), bufferStr.length());
    this->increaseDataLength(m_bufferSize);
}

// -----------------------------------------------
DataBuffer::~DataBuffer() {
    delete [] m_buffer;
}