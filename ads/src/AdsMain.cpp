/*
 * AdsMain.cpp
 *
 *  Created on: Apr 16, 2016
 *      Author: z.j
 */

#include "Sort.h"
#include "AdsLogger.h"
#include "Util.h"
#include <iostream>
#include <string.h>

using namespace std;
using namespace ads;

void testInsertionSort(const int array[], int length);
void testSelectionSort(const int array[], int length);

int main(int argc, char* argv[]) {
    AdsLogger::initConsoleLog();

    int length = 100000/2;
    if (argc == 2) {
        string temp(argv[1]);
        length = Util::s2i(temp);
    }
    
    int* randomArray = new int[length];
    Sort::generateRandomArray(randomArray, length);

    testInsertionSort(randomArray, length);
    testSelectionSort(randomArray, length);

    Sort::generateSortedArray(randomArray, length, false);

    testInsertionSort(randomArray, length);
    testSelectionSort(randomArray, length);

    delete [] randomArray;
}

// ----------------------------
void testInsertionSort(const int array[], int length) {
    int* newArray = new int[length];
    memcpy((void*)newArray, (void*)array, length * sizeof(int));

    cout << "insertionSort original array [" << length << "]: " << endl;
    Sort::formatPrint(newArray, length, 10);
    Sort::startTimer();
    Sort::insertionSort(newArray, length);
    Sort::stopTimer();
    cout << "insertionSort sorted array [" << length << "]: " << endl;
    Sort::formatPrint(newArray, length, 10);
    cout << "Execution time: " << Sort::s_time << endl;
}

// ----------------------------
void testSelectionSort(const int array[], int length) {
    int* newArray = new int[length];
    memcpy((void*)newArray, (void*)array, length * sizeof(int));

    cout << "selectionSort original array [" << length << "]: " << endl;
    Sort::formatPrint(newArray, length, 10);
    Sort::startTimer();
    Sort::selectionSort(newArray, length);
    Sort::stopTimer();
    cout << "selectionSort sorted array [" << length << "]: " << endl;
    Sort::formatPrint(newArray, length, 10);   
    cout << "Execution time: " << Sort::s_time << endl;

    delete [] newArray;
}