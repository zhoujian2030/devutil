/*
 * Sort.cpp
 *
 *  Created on: Apr 17, 2016
 *      Author: z.j
 */

#include "Sort.h"
#include "AdsLogger.h"

#include <iostream>

#include <stdlib.h>
#include <sys/time.h>

using namespace std;
using namespace ads;

// #define DEBUG_ADS

// ----------------------------------------------
void Sort::insertionSort(int array[], int length) {
    int temp;
    for (int i=1; i<length; i++) {
        for (int j=i; j>0; j--) {
            if (array[j] < array[j-1]) {
                temp = array[j];
                array[j] = array[j-1];
                array[j-1] = temp;
            } else {
                // no need to compare with the rest elements of 
                // the sorted sub array
                break;
            }
        }
    }
}

// ----------------------------------------------
void Sort::selectionSort(int array[], int length) {   
    int minIndex, temp;
    for (int i=0; i<length-1; i++) {
        minIndex = i;
        for (int j=i+1; j<length; j++) {
            if (array[j] < array[minIndex]) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            temp = array[minIndex];
            array[minIndex] = array[i];
            array[i] = temp;
        }
    } 
}

// ----------------------------------------------
void Sort::formatPrint(int array[], int length, int numOfColumn) {
#ifdef DEBUG_ADS
    cout << "+------------------------------------------------+" << endl;
    for (int i=0; i<length; i++) {
        if (i != 0 && (i % numOfColumn == 0)) {
            cout << endl;
        }
        cout << array[i] << " ";
    }

    cout << endl;
    cout << "+------------------------------------------------+" << endl;
#endif
}

// ------------------------------------------------
void Sort::generateRandomArray(int array[], int length) {
    srand(time(NULL));
    for (int i=0; i<length; i++) {
        array[i] = rand() % length + 1;
    }
}

// ------------------------------------------------
void Sort::generateSortedArray(int array[], int length, bool ascending) {
    if (ascending) {
        for (int i=0; i<length; ++i) {
            array[i] = i + 1;
        }
    } else {
        for (int i=0; i<length; ++i) {
            array[i] = length - i + 1;
        }
    }
}

// ------------------------------------------------
void Sort::getCurrentTime(long& sec, long& usec) {
    struct timeval tv;
    gettimeofday(&tv, 0);

    sec = tv.tv_sec;
    usec = tv.tv_usec;
}

double Sort::s_time = 0;
// -------------------------------------------------
void Sort::startTimer() {
    struct timeval tv;
    gettimeofday(&tv, 0);
    s_time = tv.tv_sec + (double)tv.tv_usec / 1000000;
}

// -------------------------------------------------
void Sort::stopTimer() {
    struct timeval tv;
    gettimeofday(&tv, 0);
    s_time = tv.tv_sec + (double)tv.tv_usec / 1000000 - s_time;
}