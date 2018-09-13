/*
 * TestAlgo.cpp
 *
 *  Created on: Aug 15, 2018
 *      Author: J.ZH
 */

#include "TestAlgo.h"
#include "Algo.h"
#include <iostream>
#include "logger.h"
#include <sys/time.h>

using namespace ads;
using namespace std;

TEST_F(TestAlgo, findPalindromeNumber) 
{
    LoggerSetlevel(INFO);

    int testNum = 10;
    int expectedNum = 585;

    double prevTime, currTime;
    struct timeval tv;
    gettimeofday(&tv, 0);
    prevTime = tv.tv_sec + (double)tv.tv_usec / 1000000;

    ASSERT_EQ(expectedNum, Algo::findPalindromeNumberWithC(testNum));

    gettimeofday(&tv, 0);
    currTime = tv.tv_sec + (double)tv.tv_usec / 1000000;

    cout << "Execute time : " << (currTime - prevTime)*1000 << endl;

    gettimeofday(&tv, 0);
    prevTime = tv.tv_sec + (double)tv.tv_usec / 1000000;

    ASSERT_EQ(expectedNum, Algo::findPalindromeNumberWithCPP(testNum));

    gettimeofday(&tv, 0);
    currTime = tv.tv_sec + (double)tv.tv_usec / 1000000;

    cout << "Execute time : " << (currTime - prevTime)*1000 << endl;
}

TEST_F(TestAlgo, findFlopNumber) 
{
    int testNum = 100;
    bool* pCards = Algo::findFlopNumber(testNum);
    ASSERT_TRUE(pCards != 0);

    bool* expectedCards = new bool[testNum];
    for (int i=0; i<testNum; i++) {
        expectedCards[i] = true;
    }
    expectedCards[0] = false;
    expectedCards[3] = false;
    expectedCards[8] = false;
    expectedCards[15] = false;
    expectedCards[24] = false;
    expectedCards[35] = false;
    expectedCards[48] = false;
    expectedCards[63] = false;
    expectedCards[80] = false;
    expectedCards[99] = false;

    EXPECT_TRUE(memcmp((void*)pCards, (void*)expectedCards, testNum*sizeof(bool)) == 0);
}

TEST_F(TestAlgo, cutStick) 
{
    int n = 8, m = 3, count = 4;
    ASSERT_EQ(count, Algo::cutStick(n, m));

    n = 20, m = 3, count = 8;
    ASSERT_EQ(count, Algo::cutStick(n, m));

    n = 100, m = 5, count = 22;
    ASSERT_EQ(count, Algo::cutStick(n, m));
}

TEST_F(TestAlgo, joinStick) 
{
    LoggerSetlevel(INFO);

    int n = 8, m = 3, count = 4;
    ASSERT_EQ(count, Algo::joinStick(n, m));

    n = 20, m = 3, count = 8;
    ASSERT_EQ(count, Algo::joinStick(n, m));

    n = 100, m = 5, count = 22;
    ASSERT_EQ(count, Algo::joinStick(n, m));

    for (n=2; n<=100; n++) {
        for (m=1; m<=(n/2); m++) {
            ASSERT_EQ(Algo::cutStick(n, m), Algo::joinStick(n, m));
        }
    }
}

TEST_F(TestAlgo, findBestWorkerNum) 
{
    LoggerSetlevel(INFO);

    int n = 100, m = 5, count = 22;
    ASSERT_EQ(m, Algo::findBestWorkerNum(n, count));

    n = 100, m = 23, count = 8;
    ASSERT_EQ(m, Algo::findBestWorkerNum(n, count));

    n = 8, m = 2, count = 4;
    ASSERT_EQ(m, Algo::findBestWorkerNum(n, count));

    n = 2, m = 1, count = 1;
    ASSERT_EQ(m, Algo::findBestWorkerNum(n, count));

    n = 6, m = 2, count = 4;
    ASSERT_EQ(m, Algo::findBestWorkerNum(n, count));

    LoggerSetlevel(WARNING);
    int minCount = 0;
    for (n=2; n<=100; n++) {
        minCount = Algo::cutStick(n, n/2);
        // printf("n = %d, minCount = %d\n", n, minCount);
        for (count=minCount; count<n; count++) {
            m = Algo::findBestWorkerNum(n, count);
            ASSERT_GT(Algo::cutStick(n, m-1), count);
        }
    }
}

TEST_F(TestAlgo, exchangeCoins) 
{
    LoggerSetlevel(DEBUG);

    int cashVal = 1000, maxNumCoins = 15, count = 20;
    ASSERT_EQ(count, Algo::exchangeCoins(cashVal, maxNumCoins));
}

TEST_F(TestAlgo, collatzConjecture) 
{
    LoggerSetlevel(DEBUG);

    int n = 10000, count = 34;
    ASSERT_EQ(count, Algo::collatzConjecture(n));
}

TEST_F(TestAlgo, findBinaryPalindromeDate) 
{
    LoggerSetlevel(DEBUG);

    int beginDate = 19641010, endDate = 20200724, count = 6;
    ASSERT_EQ(count, Algo::findBinaryPalindromeDate(beginDate, endDate));

    beginDate = 19641010, endDate = 20200732, count = 0;
    ASSERT_EQ(count, Algo::findBinaryPalindromeDate(beginDate, endDate));

    beginDate = 19630229, endDate = 20200724, count = 0;
    ASSERT_EQ(count, Algo::findBinaryPalindromeDate(beginDate, endDate));

    beginDate = 19641010, endDate = 21000229, count = 0;
    ASSERT_EQ(count, Algo::findBinaryPalindromeDate(beginDate, endDate));

    beginDate = 20200724, endDate = 19641010, count = 0;
    ASSERT_EQ(count, Algo::findBinaryPalindromeDate(beginDate, endDate));
}

unsigned char isValidSf(unsigned char sf) {
    unsigned int bitMask = 0x0110;

    unsigned char temp = (bitMask >> sf) & 0x01;
    printf ("temp = %d\n", temp);

    return temp;
}


TEST_F(TestAlgo, test) 
{
    ASSERT_EQ(0, isValidSf(0));
    ASSERT_EQ(0, isValidSf(1));
    ASSERT_EQ(0, isValidSf(2));
    ASSERT_EQ(0, isValidSf(3));
    ASSERT_EQ(1, isValidSf(4));
    ASSERT_EQ(0, isValidSf(5));
    ASSERT_EQ(0, isValidSf(6));
    ASSERT_EQ(0, isValidSf(7));
    ASSERT_EQ(1, isValidSf(8));
    ASSERT_EQ(0, isValidSf(9));
}