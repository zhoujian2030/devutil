/*
 * DbMain.cpp
 *
 *  Created on: Jan 20, 2016
 *      Author: z.j
 */

#include "DbPgManager.h"
#include <iostream>

using namespace std;
using namespace db;

int main(int argc, char* argv[]) {
    DbPgManager * pDbManager = new DbPgManager(
        "10.175.192.239",
        8896,
        "ses_att",
        "ses_att",
        "ses_att",
        5);

    pDbManager->getConnection();

    return 0;
}