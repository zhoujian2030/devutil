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

// ----------------------------------------------
void Sort::insertionSort(int array[], int length) {
    int n = 0;
    for (int i=1; i<length; i++) {
        int j = i;
        int target2inserted = array[i]; // suppose array[i] is the target to insert into left sorted array
        
        while (j > 0) {
            n++;
            if (target2inserted < array[j-1]) {
                array[j] = array[j-1];      // shift right
                j--;
            } else {
                break;
            }
        }

        if (j < i) {
            array[j] = target2inserted;
        }
    } 

    LOG4CPLUS_DEBUG(_ADS_LOOGER_NAME_, "n = " << n);  
}

// ----------------------------------------------
void Sort::selectionSort(int array[], int length) { 
    int n = 0;  
    int minIndex, temp;
    for (int i=0; i<length-1; i++) {
        minIndex = i;
        for (int j=i+1; j<length; j++) {
            if (array[j] < array[minIndex]) {
                minIndex = j;
            }

            n++;
        }
        if (minIndex != i) {
            temp = array[minIndex];
            array[minIndex] = array[i];
            array[i] = temp;
        }
    } 

    LOG4CPLUS_DEBUG(_ADS_LOOGER_NAME_, "n = " << n);  
}

// -----------------------------------------------
void Sort::bubbleSort(int array[], int length) {
    int n = 0;
    for (int i=0; i<length; i++) {
        bool swap = false;
        for (int j=length-1; j>i; j--) {
            if (array[j] < array[j-1]) {
                int temp = array[j];
                array[j] = array[j-1];
                array[j-1] = temp;

                swap = true;
            }
            n++;
        }

        if (!swap) {
            break;
        }
    }

    LOG4CPLUS_DEBUG(_ADS_LOOGER_NAME_, "n = " << n);
}

int gQuickN = 0;
// -------------------------------------------------
void Sort::quickSort(int array[], int length) {  
    gQuickN = 0;
    doQuickSort(array, length);
    LOG4CPLUS_DEBUG(_ADS_LOOGER_NAME_, "n = " << gQuickN);
}

// -------------------------------------------------
void Sort::doQuickSort(int* array, int length) {
    int left = 0, right = length -1;
    int temp = array[0];
    
    gQuickN++;
    
    while (left != right) {
        while (array[right] >= temp && right > left) {
            right--;
            
            gQuickN++;
        }
        
        while (array[left] <= temp && left < right) {
            left++;
            
            gQuickN++;
        }
        
        if (left < right) {
            int t = array[right];
            array[right] = array[left];
            array[left] = t;
            
            gQuickN++;
        }
    }
    
    // left == right now
    array[0] = array[left];
    array[left] = temp;
    
    if (left > 1) {
        doQuickSort(array, left);
    }
    if (length-left-1 > 1) {
        doQuickSort(&array[left+1], length-left-1);
    }
}

// ----------------------------------------------
void Sort::formatPrint(int array[], int length, int numOfColumn) {
    cout << "+------------------------------------------------+" << endl;
    for (int i=0; i<length; i++) {
        if (i != 0 && (i % numOfColumn == 0)) {
            cout << endl;
        }
        cout << array[i] << " ";
    }

    cout << endl;
    cout << "+------------------------------------------------+" << endl;
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
            array[i] = length - i;
        }
    }
}

// -----------------------------------------------
void Sort::breakOrder(int array[], int length) {
    srand(time(NULL));
    for (int i=0; i<length; i++) {
        int j = rand() % length;
        int k = rand() % length;
        int temp = array[j];
        array[j] = array[k];
        array[k] = temp;
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
    s_time = (tv.tv_sec + (double)tv.tv_usec / 1000000 - s_time) * 1000;
}
