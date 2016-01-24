/*
 * DbMain.cpp
 *
 *  Created on: Jan 20, 2016
 *      Author: z.j
 */

#include "DbPgManager.h"
#include "Util.h"
#include <iostream>

using namespace std;
using namespace db;

void showUsage() {
    cout << "Usage: " << endl;
    cout << "dbtest <host addr> <port> <db name> [user name] [password] [connect timeout]" << endl;
    cout << "  Example : ./dbtest 10.10.10.11 5432 mydb admin admin 5" << endl;
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        showUsage();
        return 0;
    }

    string ip(argv[1]);
    string port(argv[2]);
    string dbName(argv[3]);

    string userName;
    if (argc >= 5) {
        userName.append(argv[4]);
    }

    string password;
    if (argc >= 6) {
        password.append(argv[5]);
    }

    string timeout;
    if (argc >= 7) {
        timeout.append(argv[6]);
    }

    DbPgManager * pDbManager;
    if (timeout.empty()) {
        pDbManager = new DbPgManager(ip, Util::s2i(port), dbName, userName, password);
    } else {
        pDbManager = new DbPgManager(ip, Util::s2i(port), dbName, userName, password, Util::s2i(timeout));
    }

    pDbManager->getConnection();
    pDbManager->isDbAlive();

    while (true) {
        if (!pDbManager->sqlExecuteQuery()) {
            pDbManager->releaseConnection();
            pDbManager->getConnection();
        }
        sleep(5);
    }

    return 0;
}