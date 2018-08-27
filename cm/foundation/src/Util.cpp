/*
 * Util.cpp
 *
 *  Created on: Jan 11, 2016
 *      Author: z.j
 */

#include "Util.h"
#include "common.h"
#include <sstream>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

// ------------------------------
int Util::s2i(string theString) {
    int result;
    stringstream ss;
    ss << theString;
    ss >> result;

    // TODO check if success, handle the exception

    return result;
}

// ------------------------------
string Util::i2s(int number, int base) {

    if (base == 2) {
        char tmpStr[32];
        int tmp = number;
        int length = 0;

        while (tmp != 0) {
            tmpStr[length++] = (tmp & 0x01) + 0x30;
            tmp = tmp >> 1;
        }
        tmpStr[length] = '\0';

        string result(tmpStr);

        reverse(result.begin(), result.end());

        return result;
    } else {
        ostringstream ss;

        if (base == 8) {
            ss << std::oct << number;
        } else if (base == 16) {
            ss << std::hex << number;
        } else {
            ss << number;
        }

        return ss.str();
    }
}

// ------------------------------
int Util::installSignalHandler(int signo, sighandler_t handler) {
    if (signal(signo, handler) == SIG_ERR) {
        return JERROR;
    }

    return JSUCCESS;
}
