/*
 * JPipe.cpp
 *
 *  Created on: Nov 19, 2015
 *      Author: z.j
 */
#include <iostream>
#include "JPipe.h"

using namespace std;
using namespace jrpc;

JPipe::JPipe() {

}

JPipe::~JPipe() {

}

void JPipe::send() {
    cout << "JPipe send message" << endl;
}

void JPipe::recv() {
    cout << "JPipe recv message" << endl;
}

