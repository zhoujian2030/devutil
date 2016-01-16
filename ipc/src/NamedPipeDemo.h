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

    private:
        NamedPipe* m_pNamedPipe;

        void processChild(std::string pathName);
    };
}

#endif
