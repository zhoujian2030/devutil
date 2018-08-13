/*
 * MyThread.h
 *
 *  Created on: Oct 17, 2017
 *      Author: J.ZH
 */
 
#ifndef MY_THREAD_H
#define MY_THREAD_H

#include "Thread.h"

class MyThread : public cm::Thread {
public:
    MyThread();
    ~MyThread();

    virtual unsigned long run();
};


#endif
