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
    initialize();
    return m_theInstance;
}

// -----------------------------------------
void Reactor::initialize(int numOfReactors) {
    if (m_theInstance == 0) {
        m_lock.lock();
        if (m_theInstance == 0) {
            Reactor* tmp = new Reactor(numOfReactors);
            m_theInstance = tmp;
        }
        m_lock.unlock();
    }    
}

// ---------------------------------------------------
Reactor::Reactor(int numOfReactors) 
: m_numOfReactors(numOfReactors)
{
    NetLogger::initConsoleLog();

    m_reactorThreadArray = new ReactorThread*[m_numOfReactors];
    
    for (int i=0; i<m_numOfReactors; i++) {
        m_reactorThreadArray[i] = new ReactorThread();
        m_reactorThreadArray[i]->start();
    }
    
    LOG4CPLUS_INFO(_NET_LOOGER_NAME_, "+------------------------------------+");
    LOG4CPLUS_INFO(_NET_LOOGER_NAME_, "+ Create " << m_numOfReactors << " reactors !!");
    LOG4CPLUS_INFO(_NET_LOOGER_NAME_, "+------------------------------------+");
}

// ---------------------------------------------------
Reactor::~Reactor() {
    for (int i=0; i<m_numOfReactors; ++i) {
        delete m_reactorThreadArray[i];
    }
    delete m_reactorThreadArray;
}

// ---------------------------------------------------
void Reactor::registerInputHandler(Socket* theSocket, SocketEventHandler* theEventHandler) {
    // make sure the same Socket object is registered to the same reactor thread if it is 
    // registered for multiple times
    size_t index = (((size_t)theSocket) >> NUM_OF_BIT_SHIFT) % m_numOfReactors;
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "register socket(" << theSocket->getSocket() << ") to " << 
        m_reactorThreadArray[index]->getName() << " " << index);

    m_reactorThreadArray[index]->registerInputHandler(theSocket, theEventHandler);
}

// ---------------------------------------------------
void Reactor::removeInputHandler(Socket* theSocket) {
    size_t index = (((size_t)theSocket) >> NUM_OF_BIT_SHIFT) % m_numOfReactors;
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "remove socket(" << theSocket->getSocket() << ") from " << 
        m_reactorThreadArray[index]->getName() << " " << index);

    //TODO
}

// ---------------------------------------------------
void Reactor::removeHandlers(Socket* theSocket) {
    size_t index = (((size_t)theSocket) >> NUM_OF_BIT_SHIFT) % m_numOfReactors;
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "remove socket(" << theSocket->getSocket() << ") from " << 
        m_reactorThreadArray[index]->getName() << " " << index);

    m_reactorThreadArray[index]->removeHandlers(theSocket);
}

// ---------------------------------------------------
void Reactor::registerOutputHandler(Socket* theSocket, SocketEventHandler* theEventHandler) {
    // make sure the same Socket object is registered to the same reactor thread if it is 
    // registered for multiple times
    size_t index = (((size_t)theSocket) >> NUM_OF_BIT_SHIFT) % m_numOfReactors;
    LOG4CPLUS_DEBUG(_NET_LOOGER_NAME_, "register socket(" << theSocket->getSocket() << ") to " << 
        m_reactorThreadArray[index]->getName() << " " << index);

    m_reactorThreadArray[index]->registerOutputHandler(theSocket, theEventHandler);
}
