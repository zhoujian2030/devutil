/*
 * Algo.cpp
 *
 *  Created on: Aug 15, 2018
 *      Author: z.j
 */

#include "Algo.h"
#include "logger.h"
#include "Util.h"
#include <algorithm>
#include <iostream>

using namespace std;
using namespace ads;

// ----------------------------------------------
bool Algo::isDecimalPalindromeNumer(int number) {
    int sum = 0;
    int tmp = number;
    while (tmp) {
        sum = sum * 10 + tmp % 10;
        tmp = tmp / 10;
    }

    if (sum == number) {
        LOG_MSG(LOG_MODULE_ADS, DEBUG, "number = %d", number);
        return true;
    }

    return false;
}

// ----------------------------------------------
bool Algo::isHexPalindromeNumer(int number) {
    int sum = 0;
    int tmp = number;
    while (tmp) {
        sum = (sum << 4) | (tmp & 0x0f);
        tmp = tmp >> 4;
    }

    if (sum == number) {
        LOG_MSG(LOG_MODULE_ADS, DEBUG, "number = %d -> %x", number, number);
        return true;
    }

    return false;
}

// ----------------------------------------------
bool Algo::isOctalPalindromeNumer(int number) {
    int sum = 0;
    int tmp = number;
    while (tmp) {
        sum = (sum << 3) | (tmp & 0x07);
        tmp = tmp >> 3;
    }

    if (sum == number) {
        LOG_MSG(LOG_MODULE_ADS, DEBUG, "number = %d -> %o", number, number);
        return true;
    }

    return false;
}

// ----------------------------------------------
bool Algo::isBinaryPalindromeNumer(int number) {
    if (number % 2 == 0) {
        return false;
    }

    int tmp = number;
    int sum = 0;

    while (tmp) {
        sum = (sum << 1) | (tmp & 0x01);
        tmp = tmp >> 1;
    }

    if (sum == number) {
        LOG_MSG(LOG_MODULE_ADS, DEBUG, "number = %d", number);
        return true;
    }

    return false;
}

// ----------------------------------------------
bool Algo::isValidDate(int date) {
    int year = date / 10000;
    int month = (date / 100) % 100;
    int day = date % 100;

    if (year == 0 || month == 0 || day == 0 || month > 12) {
        // LOG_MSG(LOG_MODULE_ADS, ERROR, "invalid [%d] : year = %d, month = %d, day = %d", date, year, month, day);
        return false;
    }

    int daysOfMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool isLeapYear = (year % 4 == 0) ? true : false;
    if (year % 100 == 0 && year % 400 != 0) {
        isLeapYear = false;
    }
    if (isLeapYear) {
        daysOfMonth[1] = 29;
    }

    if (day > daysOfMonth[month - 1]) {
        // LOG_MSG(LOG_MODULE_ADS, ERROR, "invalid [%d] : day = %d", day);
        return false;
    }

    return true;
}

// ----------------------------------------------
int Algo::findPalindromeNumberWithC(int edgeNum) 
{
    // example
    // 9(D) = 1001(B) = 11(O)

    // Analysis:
    //   1. for binary number, the least and most significant bit must be 1
    //   2. accord to 1, it is odd number

    int num = edgeNum;
    if (num % 2 == 0) {
        num += 1;
    } else {
        num += 2;
    }

    while (1) {

        if (isDecimalPalindromeNumer(num) && 
            isOctalPalindromeNumer(num) && 
            isBinaryPalindromeNumer(num)) 
        {
            LOG_MSG(LOG_MODULE_ADS, INFO, "find match numer = %d", num);
            return num;
            break;
        }

        // only check odd number
        num += 2; 
    }
}

// ----------------------------------------------
int Algo::findPalindromeNumberWithCPP(int edgeNum) 
{
    int num = edgeNum;
    if (num % 2 == 0) {
        num += 1;
    } else {
        num += 2;
    }

    string str;
    string reverseStr;

    while (1) {
        str = Util::i2s(num); 
        reverseStr = str;
        reverse(reverseStr.begin(), reverseStr.end());
        if (str == reverseStr) {
            str = Util::i2s(num, 8); 
            reverseStr = str;
            reverse(reverseStr.begin(), reverseStr.end());            
            if (str == reverseStr) {
                str = Util::i2s(num, 2); 
                reverseStr = str;
                reverse(reverseStr.begin(), reverseStr.end());
                if (str == reverseStr) {
                    LOG_MSG(LOG_MODULE_ADS, INFO, "find match numer = %d", num);
                    return num;
                }
            }
        }

        // only check odd number
        num += 2; 
    }
}

// ----------------------------------------------
bool* Algo::findFlopNumber(int n)
{
    // 初始所有牌的背面朝上
    bool* pCards = new bool[n];
    for (int i=0; i<n; i++) {
        pCards[i] = false;
    }

    int i = 2;
    int j = i - 1;

    do {
        while (j < n) {
            pCards[j] = !(pCards[j]);
            j += i;
        }

        i++;
        j = i - 1;
    } while (i <= n);

    // 输出所有背面朝上的牌
    for (int i=0; i<n; i++) {
        if (!pCards[i]) {
            LOG_MSG(LOG_MODULE_ADS, INFO, "%d", i+1);
        }
    }

    return pCards;
}

// ----------------------------------------------
// n: 棍棒的长度
// m: 人数
// current: 目前棍棒的根数
int Algo::cutStick(int n, int m, int current)
{
    // 25 

    // 13 12

    // 7 6 6 6

    // 6 4 3 3 3 3 3

    // 3 3 3 3 3 3 2 2 2 1

    // 2 2 2 3 3 3 2 2 2 1 1 1 1

    // 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1

    // 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 1 1 1

    // 2 2 2 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1

    // 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1

    if (m == 0) {
        return n;
    }

    if (current >= n) {
        return 0;
    }

    if (current < m) {
        return (1 + cutStick(n, m, current*2));
    } else {
        return (1 + cutStick(n, m, current + m));
    }
}

// ----------------------------------------------
int Algo::joinStick(int n, int m)
{
    // 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 

    // 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 

    // 4 4 3 2 2 1 1 1 1 1 1 1 1 1 1 

    // 8 5 3 2 2 1 1 1 1 1 

    // 13 5 3 2 2

    // 18 5 2

    // 23 2

    // 28 > 25

    int count = 0;
    int current = 1;

    while (n > current) {
        count++;
        current += (current < m) ? current : m;
    }

    LOG_MSG(LOG_MODULE_ADS, DEBUG, "n = %d, m = %d, count = %d", n, m, count);
    return count;
}

// ----------------------------------------------
int Algo::findBestWorkerNum(int n, int count)
{
    int m = n / 2;
    int tmp = cutStick(n, m);

    LOG_MSG(LOG_MODULE_ADS, DEBUG, "n = %d, m = %d, tmp = %d", n, m, tmp);

    if (tmp > count) {
        LOG_MSG(LOG_MODULE_ADS, ERROR, "the best count is %d, while count = %d", tmp, count);
        return 0;
    }

    while (tmp < count) {
        m = (m > 1) ? (m / 2) : m;
        tmp = cutStick(n, m);
        LOG_MSG(LOG_MODULE_ADS, DEBUG, "n = %d, m = %d, tmp = %d", n, m, tmp);
    }

    while (tmp > count) {
        m += (m > 1) ? (m / 2) : m;    
        tmp = cutStick(n, m);
        LOG_MSG(LOG_MODULE_ADS, DEBUG, "n = %d, m = %d, tmp = %d", n, m, tmp);
    }

    if (m == 1) {
        if (tmp == count) {
            LOG_MSG(LOG_MODULE_ADS, INFO, "n = %d, m = %d, count = %d", n, m, count);
            return m;
        } else {
            LOG_MSG(LOG_MODULE_ADS, ERROR, "fail to get best count");
            return 0;
        }
    }

    while (1) {
        m--;
        tmp = cutStick(n, m);
        LOG_MSG(LOG_MODULE_ADS, DEBUG, "n = %d, m = %d, tmp = %d", n, m, tmp);

        if (tmp > count) {
            m++;
            break;
        }
    }

    LOG_MSG(LOG_MODULE_ADS, INFO, "n = %d, m = %d, count = %d", n, m, count);
    
    return m;
}

// ----------------------------------------------
int Algo::exchangeCoins(int cashVal, int maxNumCoins)
{
    int coinsVal[] = {500, 100, 50, 10};
    int nCoins = sizeof(coinsVal) / sizeof(int);

    int count = 0;

    if ((cashVal / coinsVal[0]) > maxNumCoins) {
        LOG_MSG(LOG_MODULE_ADS, ERROR, "cashVal = %d, maxNumCoins = %d, maxCoinVal = %d", cashVal, maxNumCoins, coinsVal[0]);
        return count;
    }

    int* maxNumSingleCoin = new int[nCoins];
    for (int i=0; i<nCoins; i++) {
        maxNumSingleCoin[i] = (cashVal / coinsVal[i]);
        maxNumSingleCoin[i] = (maxNumSingleCoin[i] > maxNumCoins) ? maxNumCoins : maxNumSingleCoin[i];        
        LOG_MSG(LOG_MODULE_ADS, DEBUG, "i = %d, maxNumSingleCoin = %d", i, maxNumSingleCoin[i]);
    }
    
    int j, k, m, n;
    for (j=0; j<=maxNumSingleCoin[0]; j++) {                // 500面值硬币最多2枚 （兑换1000元现金）
        for (k=0; k<=maxNumSingleCoin[1]; k++) {            // 100面值硬币最多10枚（兑换1000元现金）
            for (m=0; m<=maxNumSingleCoin[2]; m++) {        // 50面值硬币最多15枚 （兑换1000元现金，最多15枚硬币）
                for (n=0; n<=maxNumSingleCoin[3]; n++) {    // 10面值硬币最多15枚 （兑换1000元现金，最多15枚硬币）
                    if ((j+k+m+n) <= maxNumCoins) {
                        if ((j*coinsVal[0] + k*coinsVal[1] + m*coinsVal[2] + n*coinsVal[3]) == cashVal) {
                            count++;
                            LOG_MSG(LOG_MODULE_ADS, DEBUG, "j = %d, k = %d, m = %d, n = %d, count =%d", j, k, m, n, count);
                        }
                    } else {
                        break;
                    }
                }
            }
        }
    }

    LOG_MSG(LOG_MODULE_ADS, INFO, "cashVal =%d, maxNumCoins = %d, count = %d", cashVal, maxNumCoins, count);

    delete []maxNumSingleCoin;
    return count;
}

// ----------------------------------------------
int Algo::collatzConjecture(int n)
{
    int count = 0;
    unsigned long long i, j;
    for (i = 2; i < n; i += 2) {
        j = i*3 + 1;
        while (j != i && j != 1) {
            if (j % 2) {
                // odd
                j = j*3 + 1;
            } else {
                // even
                j = j / 2;
            }
            // LOG_MSG(LOG_MODULE_ADS, DEBUG, "i = %lu, j =%lu", i, j);
        }

        // LOG_MSG(LOG_MODULE_ADS, DEBUG, "i = %lu, j =%lu", i, j);

        if (j == i) {
            count++;
            LOG_MSG(LOG_MODULE_ADS, DEBUG, "j =%lu", j);
        }
    }

    LOG_MSG(LOG_MODULE_ADS, INFO, "count = %d, n = %d", count, n);

    return count;
}

// ----------------------------------------------
int Algo::findBinaryPalindromeDate(int beginDate, int endDate)
{
    if (beginDate > endDate) {
        LOG_MSG(LOG_MODULE_ADS, ERROR, "beginDate = %d > endDate = %d", beginDate, endDate);
        return 0;
    }

    if (!isValidDate(beginDate) || !isValidDate(endDate)) {
        LOG_MSG(LOG_MODULE_ADS, ERROR, "invalid date, beginDate = %d, endDate = %d", beginDate, endDate);
        return 0;
    }

    int count = 0;

    for (int i=beginDate; i<=endDate; i++) {
        if (isValidDate(i)) {
            if (isBinaryPalindromeNumer(i)) {
                LOG_MSG(LOG_MODULE_ADS, INFO, "find date = %d", i);
                count++;
            }
        } 
    }

    LOG_MSG(LOG_MODULE_ADS, INFO, "count = %d", count);

    return count;
}

