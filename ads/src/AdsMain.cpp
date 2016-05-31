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
#include "NodeUtil.h"

using namespace std;
using namespace ads;

// #define DEBUG_ADS

void testSort(int argc, char* argv[]);
void testInsertionSort(const int array[], int length);
void testSelectionSort(const int array[], int length);
void testBubbleSort(const int array[], int length);
void testQuickSort(const int array[], int length);

void testNode();

int main(int argc, char* argv[]) {
    AdsLogger::initConsoleLog();

    // testSort(argc, argv);
    
    testNode();
    // Knapsack knapsack;
    // knapsack.generateInput();
    // knapsack.calculate();
}

void testNode() {
    cout << "========================================================" << endl;
    cout << "Testing node list: " << endl;
    cout << "1 --> 3 --> 5 --> 7 --> 9 --> 11 --> 13 --> 15" << endl;
    int nodeValArray[] = {1, 3, 5, 7, 9, 11, 13, 15};
    Node* head = NodeUtil::generateNonLoopNodeList(nodeValArray, 8);
    assert(!NodeUtil::findLoop(head));
    cout << "No loop found in node list" << endl;
    NodeUtil::printNodeList(head);
    
    cout << endl << "========================================================" << endl;
    cout << "Testing node list: " << endl;
    cout << "1 -->  3 -->  5 -->  7" << endl;   
    cout << "|                    |" << endl;   
    cout << "15 <-- 13 <-- 11 <-- 9" << endl;   
    Node* loopNode = head; // 1
    NodeUtil::addNodeToTailByNode(loopNode, loopNode);
    assert(NodeUtil::findLoop(head)); 
    cout << "Found loop" << endl;        
    Node* tail = NodeUtil::findTail(head);
    assert(tail->val == 15);
    assert(tail->next->val == 1);
    cout << "Tail node is : " << tail->val << endl;
    cout << "Tail node links to node : " << tail->next->val << endl;
    cout << "Remove the loop by setting tail's next to 0" << endl;
    tail->next = 0;
    NodeUtil::printNodeList(head);
    
    cout << endl << "========================================================" << endl;
    cout << "Testing node list: " << endl;
    cout << "1 -->  3 -->  5 -->  7 --> 9" << endl;   
    cout << "              |            |" << endl;   
    cout << "             15 <-- 13 <-- 11" << endl;   
    loopNode = head->next->next; // 5
    NodeUtil::addNodeToTailByNode(loopNode, loopNode);
    assert(NodeUtil::findLoop(head)); 
    cout << "Found loop" << endl;        
    tail = NodeUtil::findTail(head);
    assert(tail->val == 15);
    assert(tail->next->val == 5);
    cout << "Tail node is : " << tail->val << endl;
    cout << "Tail node links to node : " << tail->next->val << endl;
    cout << "Remove the loop by setting tail's next to 0" << endl;
    tail->next = 0;
    NodeUtil::printNodeList(head);   
}

// ----------------------------
void testSort(int argc, char* argv[]) {
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
    testQuickSort(randomArray, length);
    cout << "--------------------" << endl;
 
    cout << "Sort Inverse Sorted Array" << endl << "-------------------------" << endl;
    // Sort::generateSortedArray(randomArray, length, false);
    testBubbleSort(inverseSortedArray, length);
    testInsertionSort(inverseSortedArray, length);
    testSelectionSort(inverseSortedArray, length);
    testQuickSort(randomArray, length);
    cout << "-------------------------" << endl;

    cout << "Sort Sorted Array" << endl << "-------------------------" << endl;
    Sort::generateSortedArray(randomArray, length, true);
    testBubbleSort(randomArray, length);
    testInsertionSort(randomArray, length);
    testSelectionSort(randomArray, length);
    testQuickSort(randomArray, length);
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

// ---------------------------
void testQuickSort(const int array[], int length) {
    int* newArray = new int[length];
    memcpy((void*)newArray, (void*)array, length * sizeof(int));
#ifdef DEBUG_ADS
    //cout << "quickSort original array [" << length << "]: " << endl;
    Sort::formatPrint(newArray, length, 10);
#endif  

    Sort::startTimer();
    Sort::quickSort(newArray, length);
    Sort::stopTimer();
#ifdef DEBUG_ADS
    //cout << "quickSort sorted array [" << length << "]: " << endl;
    Sort::formatPrint(newArray, length, 10); 
#endif  
    cout << "quickSort Execution time: " << Sort::s_time << endl;

    delete [] newArray;  
}