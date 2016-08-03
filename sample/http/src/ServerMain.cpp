/*
 * ServerMain.cpp
 *
 *  Created on: Aug 03, 2016
 *      Author: z.j
 */

#include "HttpServerService.h"

int main(int argc, char* argv[]) {

    service::HttpServerService* service = new service::HttpServerService("Http Server");

    service->wait();

    delete service;

    return 0;
}