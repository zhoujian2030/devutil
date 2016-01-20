/*
 * DbPgManager.cpp
 *
 *  Created on: Jan 20, 2016
 *      Author: z.j
 */

#include "DbPgManager.h"
#include "CPPLogger.h"

#define _DB_LOOGER_NAME_ "DB"
#define _DB_LOGGER_ log4cplus::Logger::getInstance(_DB_LOOGER_NAME_)

using namespace db;
using namespace std;

DbPgManager::DbPgManager(
    std::string dbIp,
    int dbPort,
    std::string dbName,
    std::string userName,
    std::string password,
    int connectTimeout)
:
m_conn(NULL),
m_result(NULL),
m_dbIP(dbIp),
m_dbPort(dbPort),
m_dbName(dbName),
m_userName(userName),
m_password(password),
m_connectTimeout(connectTimeout)
{
    logcpp::CPPLogger::initConsoleLog(_DB_LOOGER_NAME_);
}

DbPgManager::~DbPgManager() {

}

bool DbPgManager::getConnection() {
    string conninfo("hostaddr=");
    conninfo.append(m_dbIP);

    conninfo.append(" port=");
    ostringstream port;
    port << m_dbPort;
    conninfo.append(port.str());

    conninfo.append(" dbname=");
    conninfo.append(m_dbName);

    conninfo.append(" user=");
    conninfo.append(m_password);

    conninfo.append(" connect_timeout=");
    ostringstream t;
    t << m_connectTimeout;
    conninfo.append(t.str());

    LOG4CPLUS_DEBUG(_DB_LOGGER_, "DbPgManager::getConnection, conninfo = " << conninfo);

    m_conn = PQconnectdb(conninfo.c_str());

    if (PQstatus(m_conn) != CONNECTION_OK) {
        LOG4CPLUS_ERROR(_DB_LOGGER_, "Connection to database failed: " << PQerrorMessage(m_conn));
        PQfinish(m_conn);
        return false;
    }

    LOG4CPLUS_DEBUG(_DB_LOGGER_, "Get connection successfully.");
    return true;
}

void DbPgManager::releaseConnection() {
    LOG4CPLUS_DEBUG(_DB_LOGGER_, "Release connection");

    // if(m_result != NULL) {
    //     PQclear(m_result);
    // }
    
    // PQfinish(m_conn);
}