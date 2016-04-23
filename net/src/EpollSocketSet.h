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
#include "SocketEventHandler.h"
#include "MutexLock.h"

namespace net {
    
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

        EpollSocket* poll(int theTimeout);

        void registerInputHandler(Socket* theSocket, SocketEventHandler* theEventHandler);
        void removeInputHandler(Socket* theSocket);
        void registerOutputHandler(Socket* theSocket, SocketEventHandler* theEventHandler);

        int getNumberOfSocket() const;

    private:

        void updateEvents();

        // use non-recursive mutex
        cm::MutexLock m_lock;

        struct epoll_event* m_epollEvents;
        int m_epollFdSize;

        EpollSocket* m_readySocketArray;

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
    inline void EpollSocketSet::registerInputHandler(Socket* theSocket, SocketEventHandler* theEventHandler) {
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
            // The socket is already registered in epoll, update it
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

    // --------------------------------------------
    inline void EpollSocketSet::removeInputHandler(Socket* theSocket) {
        if (theSocket == 0) {
            return;
        }

        m_lock.lock();

        int fd = theSocket->getSocket();
        EpollSocketMap::iterator it = m_epollSocketMap.find(fd);

        if (it != m_epollSocketMap.end()) {
            it->second.events &= (~EPOLLIN);

            UpdateSocket updateSocket;
            updateSocket.fd = fd;
            
            // if no events, remove the socket from EpollSocketMap
            if (it->second.events == 0) {
                m_epollSocketMap.erase(it);
                updateSocket.op = EPOLL_CTL_DEL;
                updateSocket.events = 0;
            } else {
                updateSocket.op = EPOLL_CTL_MOD;
                updateSocket.events = it->second.events;
            }

            m_updateSocketList.push_back(updateSocket);
        }

        m_lock.unlock();
    }

    // --------------------------------------------
    inline void EpollSocketSet::registerOutputHandler(Socket* theSocket, SocketEventHandler* theEventHandler) {
        //TODO
    }

    // --------------------------------------------
    inline int EpollSocketSet::getNumberOfSocket() const {
        int num = 0;

        const_cast<EpollSocketSet*>(this)->m_lock.lock();
        num = m_epollSocketMap.size();
        const_cast<EpollSocketSet*>(this)->m_lock.unlock();

        return num;
    }

}

#endif