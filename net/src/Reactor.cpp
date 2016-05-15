/*
 * Reactor.cpp
 *
 *  Created on: May 10, 2016
 *      Author: z.j
 */

#include "Reactor.h"
#include "NetLogger.h"

using namespace net;
using namespace cm;

Reactor* Reactor::m_theInstance = 0;
MutexLock Reactor::m_lock;

// ---------------------------------------------------
Reactor* Reactor::getInstance() {
    if (m_theInstance == 0) {
        m_lock.lock();
        if (m_theInstance == 0) {
            Reactor* tmp = new Reactor();
            m_theInstance = tmp;
        }
        m_lock.unlock();
    }

    return m_theInstance;
}

// ---------------------------------------------------
Reactor::Reactor() {
    NetLogger::initConsoleLog();

    for (int i=0; i<NUM_OF_THREADS; i++) {
        m_reactorThreadArray[i].start();
    }
}

// ---------------------------------------------------
Reactor::~Reactor() {
    // empty
}

// ---------------------------------------------------
void Reactor::registerInputHandler(Socket* theSocket, SocketEventHandler* theEventHandler) {
    if (theSocket == 0 || (theSocket->getSocket() < 0)) {
        return;
    }

    // make sure the same Socket object is registered to the same reactor thread if it is 
    // registered for multiple times
    size_t index = (((size_t)theSocket) >> NUM_OF_BIT_SHIFT) % NUM_OF_THREADS;
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "register socket(" << theSocket->getSocket() << ") to " << 
        m_reactorThreadArray[index].getName() << " " << index);

    m_reactorThreadArray[index].registerInputHandler(theSocket, theEventHandler);
}

// ---------------------------------------------------
void Reactor::removeHandlers(Socket* theSocket) {
    if (theSocket == 0) {
        return;
    }

    size_t index = (((size_t)theSocket) >> NUM_OF_BIT_SHIFT) % NUM_OF_THREADS;
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "remove socket(" << theSocket->getSocket() << ") from " << 
        m_reactorThreadArray[index].getName() << " " << index);

    m_reactorThreadArray[index].removeHandlers(theSocket);

}