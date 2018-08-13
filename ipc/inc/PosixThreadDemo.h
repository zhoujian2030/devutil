/*
 * PosixThreadDemo.h
 *
 *  Created on: Mar 02, 2016
 *      Author: z.j
 */

#include <string>
#include "Thread.h"
#include "MutexLock.h"

namespace ipc {

    // ------------------------------------------
    class DemoThreadA : public cm::Thread {
    public:
        DemoThreadA();
        DemoThreadA(std::string theThreadName);

        virtual
        ~DemoThreadA();

        virtual unsigned long run();

        void setStatus(long status);

        void waitMutex();

    protected:
        long m_status;
        cm::MutexLock* m_mutex;
    };

    inline void DemoThreadA::setStatus(long status) {
        m_status = status;
    }

    // -------------------------------------------
    class DemoThreadB : public DemoThreadA {
    public:
        DemoThreadB();
        virtual unsigned long run();
    };

    // -------------------------------------------
    class DemoThreadC : public DemoThreadA {
    public:
        DemoThreadC();
        virtual unsigned long run();
    };

    // -------------------------------------------
    class DemoThreadD : public DemoThreadA {
    public:
        DemoThreadD();
        virtual unsigned long run();
    };    

    // --------------------------------------------
    class PosixThreadDemo {
    public:
        PosixThreadDemo();
        ~PosixThreadDemo();

        void demoAll();
        void demoModifyThreadDetachState();
        void demoCancelThread();
        void demoJoinableThreadReturnNormal();
        void demoJoinableThreadExitNormal();
        // TODO demo join thread that already exit normally
        void demoNotJoinTheJoinableThread();

    private:

        DemoThreadA* pThread;
    };

}
