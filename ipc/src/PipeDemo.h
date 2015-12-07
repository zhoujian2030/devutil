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
    void demoPWCR();

private:
    Pipe* mP2CPipe;
    Pipe* mC2PPipe;
};

#endif
