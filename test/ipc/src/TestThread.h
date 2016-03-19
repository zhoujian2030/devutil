#include "Thread.h"

class TestThread : public base::Thread {
public:
    TestThread();

    virtual
    ~TestThread(){}

    virtual unsigned long run();
};

