/*
 * PosixMQ.h
 *
 *  Created on: Mar 23, 2016
 *      Author: z.j
 */

#include "PosixMQ.h"

using namespace mq;

// ----------------------------------
PosixMQ::PosixMQ() 
: m_mqID(0)
{

}

// ----------------------------------
PosixMQ::~PosixMQ() {

}

// ----------------------------------
bool PosixMQ::send(std::string msg) {
    // TODO
    return false;
}

// ----------------------------------
bool PosixMQ::recv() {
    // TODO
    return false;
}