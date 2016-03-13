/*
 * TestPThread.cpp
 *
 *  Created on: Mar 01, 2016
 *      Author: z.j
 */

#include <iostream>
#include "MutexLock.h"
#include "TestThread.h"

using namespace std;
using namespace base;

int main(int argc, char* argv[]) {

    MutexLock * mutex = new MutexLock();
    mutex->lock();
    cout << "mutex is locking" << endl;
    mutex->unlock();
    cout << "mutex is unlocked" << endl;

    TestThread *t = new TestThread();
    t->start();

    long status = t->wait();
    cout << "return from thread status is : " << status << endl;

    while(true) {
        sleep(10);
    }
    return 0;
}