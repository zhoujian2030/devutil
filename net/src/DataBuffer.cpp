/*
 * DataBuffer.cpp
 *
 *  Created on: Apr 08, 2016
 *      Author: z.j
 */

#include "DataBuffer.h"

#include <string.h>

using namespace net;

const int DataBuffer::InitialBufferSize = 2000;

// -----------------------------------------------
DataBuffer::DataBuffer() 
: m_bufferSize(InitialBufferSize)
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

    memcpy(m_buffer, theDataBuffer.getStartOfDataPointer(), theDataBuffer.getLength());
    this->increaseDataLength(theDataBuffer.getLength());
}

// -----------------------------------------------
DataBuffer::~DataBuffer() {
    delete [] m_buffer;
}