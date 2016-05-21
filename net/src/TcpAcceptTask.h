/*
 * TcpAcceptTask.h
 *
 *  Created on: May 21, 2016
 *      Author: z.j
 */

#ifndef TCP_ACCEPT_TASK_H
#define TCP_ACCEPT_TASK_H

#include "Task.h"
#include "TcpSocket.h"

namespace net {
    
    class TcpAcceptTask : public cm::Task {
    public:
        TcpAcceptTask(TcpSocket* theSocket);
        virtual ~TcpAcceptTask();
        virtual int execute();  
        
    private:
        TcpSocket* m_tcpSocket;
    };
}

#endif