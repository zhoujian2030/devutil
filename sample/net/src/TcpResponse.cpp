/*
 * TcpResponse.cpp
 *
 *  Created on: June 10, 2016
 *      Author: z.j
 */

#include "TcpResponse.h"

using namespace sample;
using namespace net;

// ---------------------------------------------
TcpResponse::TcpResponse(unsigned int globalConnId)
: TcpData(globalConnId)
{

}

// ---------------------------------------------
TcpResponse::~TcpResponse() {

}

// ---------------------------------------------
void TcpResponse::setData(std::string respData) {
    m_data = respData;
}