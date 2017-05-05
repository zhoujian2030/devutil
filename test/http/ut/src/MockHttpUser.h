/*
 * MockHttpUser.h
 *
 *  Created on: July 10, 2016
 *      Author: z.j
 */

#ifndef MOCK_HTTP_USER_H
#define MOCK_HTTP_USER_H

#include "HttpServerUserInterface.h"

class MockHttpUser : public http::HttpServerUserInterface {
public:
    MockHttpUser();
    virtual ~MockHttpUser();

    virtual void deliveryResult(unsigned int connId, bool status);
    
    virtual void requestIndication(http::HttpRequest* httpRequest);
        
    virtual void closeIndication(unsigned int connId);
};

#endif
