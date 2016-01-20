/*
 * DbPgManager.h
 *
 *  Created on: Jan 20, 2016
 *      Author: z.j
 */

#ifndef DB_PG_MANAGER_H
#define DB_PG_MANAGER_H

#include <libpq-fe.h>
#include <string>

namespace db {

class DbPgManager {
public:
    DbPgManager(
        std::string dbIp,
        int dbPort,
        std::string dbName,
        std::string userName,
        std::string password,
        int connectTimeout=5
        );
    ~DbPgManager();

    bool getConnection();
    void releaseConnection();

private:

    PGconn* m_conn;
    PGresult* m_result;

    std::string m_dbIP;
    int m_dbPort;
    std::string m_dbName;
    std::string m_userName;
    std::string m_password;
    int m_connectTimeout;
};

}

#endif

