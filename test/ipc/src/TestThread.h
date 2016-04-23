#include "Thread.h"

class TestThread : public cm::Thread {
public:
    TestThread();

    virtual
    ~TestThread(){}

    virtual unsigned long run();
};

