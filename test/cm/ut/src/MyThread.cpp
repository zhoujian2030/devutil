/*
 * MyThread.cpp
 *
 *  Created on: Oct 17, 2017
 *      Author: J.ZH
 */

#include "MyThread.h"
#include <iostream>

using namespace std;
using namespace cm;

MyThread::MyThread() 
: Thread("Test Thread")
{

}

MyThread::~MyThread() {

}

unsigned long MyThread::run() {
    cout << "thread running" << endl;

    return 0;
}