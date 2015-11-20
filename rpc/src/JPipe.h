/*
 * JPipe.h
 *
 *  Created on: Nov 19, 2015
 *      Author: z.j
 */
#ifndef _JPIPE_H_
#define _JPIPE_H_

namespace jrpc 
{
    class JPipe {
        public:
            JPipe();
            ~JPipe();

            void send();
            void recv();

    };

}

#endif
// end of file