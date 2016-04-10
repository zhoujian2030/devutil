/*
 * EpollSocketSet.h
 *
 *  Created on: Apr 04, 2016
 *      Author: z.j
 */

#ifndef EPOLL_SOCKET_SET_H
#define EPOLL_SOCKET_SET_H

#include <sys/epoll.h>
#include <map>
#include <list>

#include "Socket.h"
#include "MutexLock.h"

namespace net {

    class SocketEventHandler;
    
    // provide API to register listenning fd and associated event handler callback.
    // max number of listening fd supported depends on system limit for current process,
    // but we limit the value to 1024 ~ 65536.
    // use LT (Level Trigger) mode by default, not support ET (Edge Trigger) yet !!
    class EpollSocketSet {
    public:

        // 
        struct EpollSocket {
            Socket*  socket;
            SocketEventHandler* eventHandler;
            int events;
        };

        // 
        struct UpdateSocket {
            int fd;
            int op;
            int events;
        };

        EpollSocketSet();
        virtual ~EpollSocketSet();

        void registerReadSocket(Socket* theSocket, SocketEventHandler* theEventHandler);
        // void registerWriteSocket(int fd);

    private:

        void addEvents();

        // use non-recursive mutex
        base::MutexLock m_lock;

        struct epoll_event* m_epollEvents;
        int m_epollFdSize;

        // epoll fd
        int m_epollFd;

        // number of events become available 
        int m_numFds;

        // save all the sockets registered to epoll
        typedef std::map<int, EpollSocket> EpollSocketMap;
        EpollSocketMap m_epollSocketMap;

        // save all the new sockets that need to be added
        // into epoll, or sockets that need to be changed
        // in epoll
        // TODO use vector or list?
        typedef std::list<UpdateSocket> UpdateSocketList;
        UpdateSocketList m_updateSocketList;


    };

    // --------------------------------------------
    inline void EpollSocketSet::registerReadSocket(Socket* theSocket, SocketEventHandler* theEventHandler) {
        if (theSocket == 0) {
            return;
        }

        int fd = theSocket->getSocket();
        int operation = EPOLL_CTL_ADD;

        EpollSocket epollSocket;
        epollSocket.socket = theSocket;
        epollSocket.eventHandler = theEventHandler;
        epollSocket.events = EPOLLIN;

        m_lock.lock();

        std::pair<EpollSocketMap::iterator, bool> result = 
            m_epollSocketMap.insert(EpollSocketMap::value_type(fd, epollSocket));
        if (!result.second) {
            // The socket is already registered in epoll
            operation = EPOLL_CTL_MOD;
            (result.first)->second.events |= epollSocket.events;
            (result.first)->second.socket = epollSocket.socket;
            (result.first)->second.eventHandler = epollSocket.eventHandler;
            epollSocket.events = (result.first)->second.events;
        }

        UpdateSocket updateSocket;
        updateSocket.fd = fd;
        updateSocket.op = operation;
        updateSocket.events = epollSocket.events;
        m_updateSocketList.push_back(updateSocket);

        m_lock.unlock();
    }

    // // --------------------------------------------
    // void EpollSocketSet::registerWriteSocket(int fd) {
    //     // TODO
    // }
}

#endif