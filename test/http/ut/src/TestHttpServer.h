/*
 * TestHttpServer.h
 *
 *  Created on: July 8, 2016
 *      Author: z.j
 */

#ifndef TEST_HTTP_SERVER_H
#define TEST_HTTP_SERVER_H

#include "TestSuite.h"
#include "HttpServer.h"

class TestHttpServer : public TestSuite {
protected:
    virtual void SetUp() {
        std::cout << "TestHttpServer::SetUp()" << std::endl;
    }
    
    virtual void TearDown() {
        std::cout << "TestHttpServer::TearDown()" << std::endl;
    }

    http::HttpServer* m_httpServer;
};

#endif