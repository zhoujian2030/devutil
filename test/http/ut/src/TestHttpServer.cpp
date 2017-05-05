/*
 * TestHttpServer.cpp
 *
 *  Created on: July 8, 2016
 *      Author: z.j
 */

#include "TestHttpServer.h"
#include "MockHttpClient.h"
#include "HttpServer.h"
#include "MockHttpUser.h"
#include "Thread.h"
#include <iostream>

using namespace std;
using namespace http;
using namespace cm;

TEST_F(TestHttpServer, Test) {
    MockHttpUser* m_httpUser = new MockHttpUser();
    m_httpServer = new HttpServer(m_httpUser, 1234, "127.0.0.1");
    m_httpServer->listen();

    MockHttpClient httpClient;
    // httpClient.post();

    HttpRequest* request = new HttpRequest("post", "http://127.0.0.1:1234/test");
    request->setHeader("Content-type", "application/json");
    request->setHeader("Accept", "application/json");
    request->setHeader("Connection", "close");
    request->setHeader("User-Agent", "Boost-HttpClient/1.0");
    httpClient.sendRequest(request);

    Thread::sleep(10);
}
