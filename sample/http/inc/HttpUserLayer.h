/*
 * HttpUserLayer.h
 *
 *  Created on: Aug 03, 2016
 *      Author: z.j
 */

#ifndef HTTP_USER_LAYER_H
#define HTTP_USER_LAYER_H

#include "HttpServerUserInterface.h"

namespace service {

    class HttpUserLayer : public http::HttpServerUserInterface {
    public:
        HttpUserLayer();
        virtual ~HttpUserLayer();

        virtual void deliveryResult(unsigned int connId, bool status);
        
        virtual void requestIndication(http::HttpRequest* httpRequest);
        
        virtual void closeIndication(unsigned int connId);
        
    };

}


#endif
