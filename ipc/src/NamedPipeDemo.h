/*
 * NamedPipeDemo.h
 *
 *  Created on: Jan 12, 2016
 *      Author: z.j
 */
#ifndef _NAMED_PIPE_DEMO_H_
#define _NAMED_PIPE_DEMO_H_

#include <string>

namespace ipc {

    class NamedPipe;
    class NamedPipeDemo {
    public:
        NamedPipeDemo();

        void demo1W1R_normal(std::string wPathName, std::string rPathName);

    private:
        NamedPipe* m_pNamedPipe;
    };
}

#endif
