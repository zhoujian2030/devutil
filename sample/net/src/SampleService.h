/*
 * SampleService.h
 *
 *  Created on: June 7, 2016
 *      Author: z.j
 */

#ifndef SAMPLE_SERVICE_H
#define SAMPLE_SERVICE_H

#include "Service.h"
#include "EventIndicator.h"

namespace sample {

    // Initialize a sample service to support hight concurence
    // of TCP connection (long connection)
    class SampleService : public cm::Service {
    public:
        SampleService(std::string serviceName);
        virtual ~SampleService();

    private:
        virtual unsigned long run();
        
        cm::EventIndicator m_stopEvent;
    };

}

#endif