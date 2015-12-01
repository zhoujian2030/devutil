/*
 * Pipe.h
 *
 *  Created on: Nov 19, 2015
 *      Author: z.j
 */
#ifndef _PIPE_H_
#define _PIPE_H_

namespace ipc 
{
    class Pipe {
        public:
            Pipe();
            Pipe(int readFd, int writeFd);
            ~Pipe();

            // Create pipe
            int init();

            // Write data to pipe
            int send(char* buff);

            // Read data from pipe
            int recv(char* buff, int buffLen);

            void setReadNonBlock();
            void setReadBlock();

            void setWriteNonBlock();
            void setWriteBlock();

            void setRead();
            void setWrite();
            bool isRead();
            bool isWrite();

            int getReadFd();
            int getWriteFd();

            void closeAll(); // for test

            enum{P_READ = 1, P_WRITE = 2, P_READ_WRITE = 3};

        protected:
            int mReadFd;
            int mWriteFd;

            int mDirection;
    };

}

#endif
// end of file