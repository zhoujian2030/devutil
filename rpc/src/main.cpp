/*
 * main.cpp
 *
 *  Created on: Nov 19, 2015
 *      Author: z.j
 */
#include <iostream>
#include "JPipe.h"

 using namespace std;
 using namespace jrpc;

 int main(int argc, char* argv[]) {

    cout<<"Start Main Thread..."<< endl;

    JPipe* pipe = new JPipe();
    pipe->send();
    pipe->recv();
}

