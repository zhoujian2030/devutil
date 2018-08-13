/*
 * TcpResponse.h
 *
 *  Created on: June 10, 2016
 *      Author: z.j
 */

#ifndef TCP_RESPONSE_H
#define TCP_RESPONSE_H

#include "TcpData.h"
#include <string>

namespace sample {

    class TcpResponse : public net::TcpData {
    public:
        TcpResponse(unsigned int globalConnId);
        virtual ~TcpResponse();

        void setData(std::string respData);
    };

}

#endif
