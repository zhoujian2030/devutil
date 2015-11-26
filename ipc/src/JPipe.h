/*
 * JPipe.h
 *
 *  Created on: Nov 19, 2015
 *      Author: z.j
 */
#ifndef _JPIPE_H_
#define _JPIPE_H_

namespace ipc 
{
    class JPipe {
        public:
            JPipe();
            JPipe(int readFd, int writeFd);
            ~JPipe();

            int init();
            int send(char* buff);
            int recv(char* buff, int buffLen);

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