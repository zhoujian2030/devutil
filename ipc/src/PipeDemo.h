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

    static void registerSIGCHLDHandler(sighandler_t handler);
    static void waitChildExit(int signo);

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

private:
    Pipe* mP2CPipe;
    Pipe* mC2PPipe;
};

#endif
