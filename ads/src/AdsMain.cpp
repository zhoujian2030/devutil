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

// #define DEBUG_ADS

void testInsertionSort(const int array[], int length);
void testSelectionSort(const int array[], int length);
void testBubbleSort(const int array[], int length);

int main(int argc, char* argv[]) {
    AdsLogger::initConsoleLog();

    int length = 100000/2;
    if (argc == 2) {
        string temp(argv[1]);
        length = Util::s2i(temp);
    }
    
    int* randomArray = new int[length];
    int* inverseSortedArray = new int[length];
    // Sort::generateRandomArray(randomArray, length);
    Sort::generateSortedArray(inverseSortedArray, length, false);

    memcpy((void*)randomArray, (void*)inverseSortedArray, length * sizeof(int));
    Sort::breakOrder(randomArray, length);

    cout << "Sort Random Array" << endl << "--------------------" << endl;
    testBubbleSort(randomArray, length);
    testInsertionSort(randomArray, length);
    testSelectionSort(randomArray, length);
    cout << "--------------------" << endl;
 
    cout << "Sort Inverse Sorted Array" << endl << "-------------------------" << endl;
    // Sort::generateSortedArray(randomArray, length, false);
    testBubbleSort(inverseSortedArray, length);
    testInsertionSort(inverseSortedArray, length);
    testSelectionSort(inverseSortedArray, length);
    cout << "-------------------------" << endl;

    cout << "Sort Sorted Array" << endl << "-------------------------" << endl;
    Sort::generateSortedArray(randomArray, length, true);
    testBubbleSort(randomArray, length);
    testInsertionSort(randomArray, length);
    testSelectionSort(randomArray, length);
    cout << "-------------------------" << endl;

    delete [] randomArray;
    delete [] inverseSortedArray;
}

// ----------------------------
void testBubbleSort(const int array[], int length) {
    int* newArray = new int[length];
    memcpy((void*)newArray, (void*)array, length * sizeof(int));

#ifdef DEBUG_ADS
    //cout << "bubbleSort original array [" << length << "]: " << endl;
    Sort::formatPrint(newArray, length, 10);
#endif
    Sort::startTimer();
    Sort::bubbleSort(newArray, length);
    Sort::stopTimer();
#ifdef DEBUG_ADS
    //cout << "bubbleSort sorted array [" << length << "]: " << endl;
    Sort::formatPrint(newArray, length, 10);
#endif
    cout << "BubbleSort Execution time: " << Sort::s_time << endl;    
}

// ----------------------------
void testInsertionSort(const int array[], int length) {
    int* newArray = new int[length];
    memcpy((void*)newArray, (void*)array, length * sizeof(int));

#ifdef DEBUG_ADS
    //cout << "insertionSort original array [" << length << "]: " << endl;
    Sort::formatPrint(newArray, length, 10);
#endif
    Sort::startTimer();
    Sort::insertionSort(newArray, length);
    Sort::stopTimer();
#ifdef DEBUG_ADS
    //cout << "insertionSort sorted array [" << length << "]: " << endl;
    Sort::formatPrint(newArray, length, 10);
#endif
    cout << "InsertionSort Execution time: " << Sort::s_time << endl;
}

// ----------------------------
void testSelectionSort(const int array[], int length) {
    int* newArray = new int[length];
    memcpy((void*)newArray, (void*)array, length * sizeof(int));
#ifdef DEBUG_ADS
    //cout << "selectionSort original array [" << length << "]: " << endl;
    Sort::formatPrint(newArray, length, 10);
#endif
    Sort::startTimer();
    Sort::selectionSort(newArray, length);
    Sort::stopTimer();
#ifdef DEBUG_ADS
    //cout << "selectionSort sorted array [" << length << "]: " << endl;
    Sort::formatPrint(newArray, length, 10); 
#endif  
    cout << "SelectionSort Execution time: " << Sort::s_time << endl;

    delete [] newArray;
}