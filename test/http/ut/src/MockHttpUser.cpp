/*
 * MockHttpUser.cpp
 *
 *  Created on: July 10, 2016
 *      Author: z.j
 */

#include "MockHttpUser.h"
#include <iostream>

using namespace http;
using namespace std;

MockHttpUser::MockHttpUser() {

}

MockHttpUser::~MockHttpUser() {

}

void MockHttpUser::deliveryResult(unsigned int connId, bool status) {

}

void MockHttpUser::requestIndication(HttpRequest* httpRequest) {
    cout << "MockHttpUser::requestIndication()" << endl;
    cout << "Connection ID = 0x" << std::hex << httpRequest->getConnId() << endl;
    cout << "Recv HTTP request: " << endl;
    httpRequest->show();
}

void MockHttpUser::closeIndication(unsigned int connId) {

}

