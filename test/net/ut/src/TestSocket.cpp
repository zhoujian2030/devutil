/*
 * TestSocket.cpp
 *
 *  Created on: June 12, 2016
 *      Author: z.j
 */

#include "TestSocket.h"
#include "SysctlUtil.h"
#include <iostream>

using namespace net;
using namespace std;

// 
TEST_F(TestSocket, ChangeSendRecvBuffer) {
    int sendBufferSize = m_socket->getSendBufferSize();
    EXPECT_GT(sendBufferSize, 0) << "Current send buffer size: " << sendBufferSize;
    cout << "Current send buffer size: " << sendBufferSize << endl;

    // int changeSendBufferSize = 249800;
    // m_socket->setSendBufferSize(changeSendBufferSize);
    // sendBufferSize = m_socket->getSendBufferSize();
    // EXPECT_EQ(sendBufferSize, changeSendBufferSize) << "New send buffer size: " << sendBufferSize;
    // cout << "New send buffer size: " << sendBufferSize << endl;

    // int wmem_max = cm::SysctlUtil::getMaxWmem();
    // cout << "wmem_max = " << wmem_max << endl;

    // int rmem_max = cm::SysctlUtil::getMaxRmem();
    // cout << "rmem_max = " << rmem_max << endl;
}
