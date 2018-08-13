/*
 * MockHttpClient.h
 *
 *  Created on: July 8, 2016
 *      Author: z.j
 */

#ifndef MOCK_HTTP_CLIENT_H
#define MOCK_HTTP_CLIENT_H

#include "HttpRequest.h"

class MockHttpClient {
public:
    MockHttpClient();
    ~MockHttpClient();

    void sendRequest(http::HttpRequest* theRequest);

    void post();
};

#endif
