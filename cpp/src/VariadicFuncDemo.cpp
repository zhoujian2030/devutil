/*
 * VariadicFuncDemo.cpp
 *
 *  Created on: Dec 03, 2015
 *      Author: z.j
 */
#include <cstdarg>
#include <stdlib.h>
#include <iostream>

using namespace std;

int add(int First, int Second, ...);
int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "usage: add-demo <integer para1> <integer para2> ..." << endl;
        return 0;
    }

    string str = "aa << bb << cc";
    cout << str;

    int numInt = argc - 1;
    cout << "argv size: " << numInt << endl;
    int* pArgvList = new int(numInt);
    for (int i=0; i<numInt; i++) {
        *(pArgvList+i) = atoi(argv[i+1]);
    }

    int sum = 0;
    switch (numInt) {
        case 2:
            sum = add(*pArgvList, *(pArgvList+1));
            break;

        case 3:
            sum = add(*pArgvList, *(pArgvList+1), *(pArgvList+2));
            break;

        case 4:
            sum = add(*pArgvList, *(pArgvList+1), *(pArgvList+2), *(pArgvList+3));
            break;

        default:
            cout << "No more than 4 arguments." << endl;
            return 0;
    }

    cout << sum << endl;

    return 0;
}

int add(int First, int Second, ...) {
    int R = First + Second; 
    va_list va;
    va_start(va, First);
    while (int V = va_arg(va, int)) {
        cout << "v = " << V << endl;
        R += V;  
    }
    va_end(va);
    return R;
}