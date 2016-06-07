#ifndef MOCK_TCP_SERVER_USER_H
#define MOCK_TCP_SERVER_USER_H

#include "TcpServerCallback.h"

class MockTcpServerUser : public net::TcpServerCallback {
public:
    
    virtual void dataIndication(unsigned int connId, char* buffer, int numOfBytes);
    
    virtual void closeNotification(unsigned int connId);
};

#endif