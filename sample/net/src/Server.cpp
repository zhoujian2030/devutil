/*
 * Server.cpp
 *
 *  Created on: June 7, 2016
 *      Author: z.j
 */

#include "SampleService.h"

int main(int argc, char* argv[]) {

    sample::SampleService* service = new sample::SampleService("Sample Tcp Server");

    service->wait();

    delete service;

    return 0;
}