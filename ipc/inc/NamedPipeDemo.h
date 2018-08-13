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

        // fifo is not existed
        // Process A mkfifo then block on open fifo with write mode
        // Process B open fifo with read mode, then both A and B open the fifo success
        // Process B block on read fifo
        // Process A write fifo, then B read fifo success
        void demo_wopen_read_block_fifo_not_exist(std::string wPathName, std::string rPathName);

        // fifo is existed
        // Process B block on open fifo with read mode
        // Process A open fifo with write mode, then both A and B open the fifo success
        // Process A write fifo
        // Process B read fifo success
        void demo_ropen_block_fifo_exist(std::string wPathName, std::string rPathName);

        // fifo is not existed
        // Process A create and open fifo with write mode
        // Process A write 8191*8 = 65536 bytes data to fifo, then block on write 8 more bytes data
        // Process B read 4096 bytes success in one time
        // Process A write 8 bytes success
        void demo_write_block(std::string wPathName, std::string rPathName);

        // Process A create and open fifo with write mode
        // Process A write 8, 4K, 8, 4K, 8, 4K, 8, 4K, 8, 4K, 8, 4K, 8, 4K, 8, 4K bytes data
        //           then block on write 8 more bytes data
        // Process B read 4096 bytes success in one time
        // Process A write 8 bytes success
        void demo_write_less_than_64k_block(std::string wPathName, std::string rPathName);

        // Process B install SIGPIPE handler, then create and open fifo with write mode
        // Process A open fifo with read mode then close it
        // Process B write fifo fail and catch SIGPIPE signal due to A closed the fifo
        // Process B uninstall SIGPIPE handler and write fifo again, then B exited due to SIGPIPE
        void demo_write_catch_sigpipe(std::string wPathName, std::string rPathName);

        // fifo is not created, fail to open the fifo in read mode
        // fifo is not created, fail to open the fifo in write mode
        void open_fail_fifo_not_exist(std::string pathName);

        // test umask unlink
        // test non block
        // test 2 process open read, 1 process open write
        // test 2 process open write, 1 process open read

    private:
        NamedPipe* m_pNamedPipe;

        void processChild(std::string pathName);

        static void handleSIGPIPE(int signo);
    };
}

#endif
