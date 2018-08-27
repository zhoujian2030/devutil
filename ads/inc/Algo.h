/*
 * Algo.h
 *
 *  Created on: Aug 15, 2018
 *      Author: z.j
 */

#ifndef ALGO_H
#define ALGO_H

namespace ads {

    class Algo {
    public:

        // Description:
        //    求用十进制、二进制和八进制表示都是回文数的所有数字中，
        //    大于10进制数edgeNum的最小数
        static int findPalindromeNumberWithC(int edgeNum);
        static int findPalindromeNumberWithCPP(int edgeNum);

        // Q 03:
        //    n张牌排成一列，背面朝上。第1个人从第2张牌开始翻牌，每隔1张牌翻牌；
        //    第2个人从第3张牌开始每隔2张牌翻牌；如此这边，第i-1个人从第i张牌
        //    开始隔i-1张牌翻牌，直到无牌可翻（i=n）；
        // 求：当所有牌不再变动时，所有背面朝上的牌的数字
        static bool* findFlopNumber(int n);

        // Q 04:
        //    将长度为n的棍棒切分为长度为1的小段，一根木棒只能由1人切分，
        //    当木棒切分为3段则可由3人同时切分，求m个人最少要切分多少次
        static int cutStick(int n, int m, int current = 1);
        static int joinStick(int n, int m);
        // Q 04-1
        //    将长度为n的棍棒切分为长度为1的小段，一根木棒只能有1人切分，
        // 求：在指定时间内完成切割最少需要多少人？
        static int findBestWorkerNum(int n, int count);

        // Q 05:
        //    零钱兑换机输入为现金总值，可以兑换出不超过特定数量的硬币，
        //    硬币面值可能为500、100、50和10元
        // 求：在兑换指定现金（比如1000元）和最多硬币数量（比如15个）的
        //    情况下，会出现多少种组合？
        static int exchangeCoins(int cashVal, int maxNumCoins);

        // Q 06:
        //    考拉兹猜想：
        //       对自然数i循环计算: 偶数时：i/2; 奇数时： i*3 + 1，最终都会得到1
        //    修改后的考拉兹猜想：
        //       初始值i为偶数时，第一次计算用 i*3 + 1，后面的循环操作不变，查找
        //       能回到初始值i的数（最终也是得到1）
        // 求：10000以内“能回到初始值的数”有多少个？
        static int collatzConjecture(int n);

        // Q 07:
        //    日期用YYYYMMDD的整数表示，求在指定时间段内，满足二进制表示的数字是回文
        //    数的日期
        static int findBinaryPalindromeDate(int beginDate, int endDate);

        

    private:
        static bool isDecimalPalindromeNumer(int number);
        static bool isHexPalindromeNumer(int number);
        static bool isOctalPalindromeNumer(int number);
        static bool isBinaryPalindromeNumer(int number);

        static bool isValidDate(int date);

    };

}

#endif
