/*
 * Util.cpp
 *
 *  Created on: Jan 11, 2016
 *      Author: z.j
 */

#include "Util.h"
#include <sstream>
#include <string>
#include <iostream>

using namespace std;

int Util::s2i(string theString) {
    int result;
    stringstream ss;
    ss << theString;
    ss >> result;

    // TODO check if success, handle the exception

    return result;
}