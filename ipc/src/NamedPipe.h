/*
 * NamedPipe.h
 *
 *  Created on: Dec 21, 2015
 *      Author: z.j
 */
#ifndef _NAMED_PIPE_H_
#define _NAMED_PIPE_H_

#include <limits.h>
#include <string>

namespace ipc 
{
    class NamedPipe {

    public:
        NamedPipe(std::string& pathName);
        ~NamedPipe();

        int initRead();
        int initWrite();

        // read data from fifo and store in m_data
        // return actual number of byte read if success
        // 
        int recv();
        std::string& getData();

        // write data to fifo
        void setData(const std::string& theString);
        int send();

        std::string toString() const;

        enum {
            BUFF_SIZE = PIPE_BUF
        };

    private:
        int create();
        int openFifo(int flags);

        bool m_isRead;

        std::string m_pathName;
        int m_fd;

        std::string m_messageBuffer;
        unsigned char m_rxBuffer[BUFF_SIZE];
    };


}

#endif // end of ifndef
