/*
 * PipeDemo.h
 *
 *  Created on: Dec 01, 2015
 *      Author: z.j
 */
#ifndef _PIPE_DEMO_H_
#define _PIPE_DEMO_H_
#include "Pipe.h"

using namespace ipc;
class PipeDemo {
public:
    PipeDemo();
    ~PipeDemo();

    static void ignoreSignal(int signum);

    static void installSIGCHLDHandler(sighandler_t handler);
    static void installSIGPIPEHandler(sighandler_t handler);
    static void handleSIGCHLD(int signo);
    static void handleSIGPIPE(int signo);

    //P: parent process
    //C: child process
    //W: write
    //R: read
    //N: no

    // Mode: block
    // Test: normal case, parent write to pipe, child read from pipe
    // Expect: parent write success, child block and read success after parent write
    void demoPWCR();

    // Mode: block
    // Test: close write fd in both parent and child, then child read from pipe
    // Expect: return 0 immediately when read
    void demoNWCR();

    // Mode: block
    // Test: close read fd in both parent and child, then child write pipe
    // Expect: write fail, trigger SIGPIPE signal. If child not register SIGPIPE handler, process exits 13
    void demoNRCW();

    // Mode: block
    // Test: close read fd in both parent and child, then parent write pipe
    // Expect: write fail, trigger SIGPIPE signal. If parent not register SIGPIPE handler, proces exits
    //         immediately, leaving child process to be managed by init process
    void demoNRPW();

private:
    Pipe* mP2CPipe;
    Pipe* mC2PPipe;
};

#endif
