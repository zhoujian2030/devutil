/*
 * PosixThreadDemo.h
 *
 *  Created on: Mar 02, 2016
 *      Author: z.j
 */

#include "Thread.h"
#include "MutexLock.h"

namespace ipc {

    // ------------------------------------------
    class DemoThreadA : public base::Thread {
    public:
        DemoThreadA();

        virtual
        ~DemoThreadA();

        virtual unsigned long run();

        void setStatus(int status);

        void waitMutex();

    protected:
        int m_status;
        base::MutexLock* m_mutex;
    };

    inline void DemoThreadA::setStatus(int status) {
        m_status = status;
    }

    // -------------------------------------------
    class DemoThreadB : public DemoThreadA {
    public:
        virtual unsigned long run();
    };

    // -------------------------------------------
    class DemoThreadC : public DemoThreadA {
    public:
        virtual unsigned long run();
    };

    // -------------------------------------------
    class DemoThreadD : public DemoThreadA {
    public:
        virtual unsigned long run();
    };    

    // --------------------------------------------
    class PosixThreadDemo {
    public:
        PosixThreadDemo();
        ~PosixThreadDemo();

        void demoAll();
        void demoModifyThreadDetachState();
        void demoJoinableThreadReturnNormal();
        void demoJoinableThreadExitNormal();
        void demoNotJoinTheJoinableThread();

    private:
        DemoThreadA* pThread;
    };

}
