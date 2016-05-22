/*
 * DbPgManager.cpp
 *
 *  Created on: Jan 20, 2016
 *      Author: z.j
 */

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <errno.h>
#include <sys/time.h>
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
    cm::CPPLogger::initConsoleLog(_DB_LOOGER_NAME_);
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

    if (!m_userName.empty()) {
        conninfo.append(" user=");
        conninfo.append(m_userName);
    }

    if (!m_password.empty()) {
        conninfo.append(" password=");
        conninfo.append(m_password);
    }

    conninfo.append(" connect_timeout=");
    ostringstream t;
    t << m_connectTimeout;
    conninfo.append(t.str());

    LOG4CPLUS_DEBUG(_DB_LOGGER_, "DbPgManager::getConnection, conninfo = " << conninfo);

    m_conn = PQconnectdb(conninfo.c_str());

    if (PQstatus(m_conn) != CONNECTION_OK) {
        LOG4CPLUS_ERROR(_DB_LOGGER_, "Connection to database failed: " << PQerrorMessage(m_conn));
        PQfinish(m_conn);
        m_conn = NULL;
        return false;
    }

    LOG4CPLUS_DEBUG(_DB_LOGGER_, "Get connection successfully.");
    return true;
}

void DbPgManager::releaseConnection() {
    LOG4CPLUS_DEBUG(_DB_LOGGER_, "Release connection");

    if(m_result != NULL) {
        PQclear(m_result);
        m_result = NULL;
    }
    
    if (m_conn != NULL) {
        PQfinish(m_conn);
        m_conn = NULL;
    }
}

bool DbPgManager::sqlExecuteQuery() {
    if (PQstatus(m_conn) != CONNECTION_OK) {
        LOG4CPLUS_ERROR(_DB_LOGGER_, "Connection is lost.");
        return false;
    }

    string sql("select count(1) as TOTALNUMBER");
    // string sql("select * from simdeviceinfo");
    int nParams = 0;
    Oid* paramTypes = new Oid[nParams];
    memset( paramTypes, 0, nParams*sizeof(Oid) );
    char** paramValues = new char*[nParams];
    memset( paramValues, 0, nParams*sizeof(char*) );
    int* paramLengths = new int[nParams];
    memset( paramLengths, 0, nParams*sizeof(int) );
    int* paramFormats = new int[nParams];
    memset( paramFormats, 0, nParams*sizeof(int) );

    LOG4CPLUS_DEBUG(_DB_LOGGER_, "DbPgManager::sqlExecuteQuery(): " << sql);

    int retVal = PQsendQueryParams(
        m_conn, 
        sql.c_str(),
        nParams,
        paramTypes,
        paramValues,
        paramLengths,
        paramFormats,
        0
        );

    // return 1 if success, 0 if fail
    if (0 == retVal) {
        LOG4CPLUS_ERROR(_DB_LOGGER_, "PQsendQueryParams failed: " << PQerrorMessage(m_conn));
        // TODO free memory?
        return false;
    }

    #if 0
    bool isResultReady = true;
    int n = 0;
    while (PQisBusy(m_conn)) {
        LOG4CPLUS_DEBUG(_DB_LOGGER_, "PQconsumeInput: " << n);

        usleep(500000);
        retVal = PQconsumeInput(m_conn);
        if (0 == retVal) {
            LOG4CPLUS_WARN(_DB_LOGGER_, "PQconsumeInput error: " << PQerrorMessage(m_conn));
        }

        n++;
        if (10 == n) {
            LOG4CPLUS_ERROR(_DB_LOGGER_, "Query Timeout !");
            isResultReady = false;
            break;
        }
    }
    #else
    bool isResultReady = false;
    int fd = PQsocket(m_conn);
    fd_set rfds;
    struct timeval tv;
    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);
    tv.tv_sec = 5;
    tv.tv_usec = 0;

    LOG4CPLUS_DEBUG(_DB_LOGGER_, "conn fd: " << fd << ", time out: " << tv.tv_sec);

    while (true) {
        // TODO why fd + 1??
        // should select in another thread? 
        retVal = select(fd + 1, &rfds, NULL, NULL, &tv);

        if (retVal > 0) {
            // result data is ready on the fd
            retVal = PQconsumeInput(m_conn);
            if (0 == retVal) {
                LOG4CPLUS_WARN(_DB_LOGGER_, "PQconsumeInput error: " << PQerrorMessage(m_conn));
            }

            if (!PQisBusy(m_conn)) {
                isResultReady = true;
                LOG4CPLUS_DEBUG(_DB_LOGGER_, "No more read data from fd: " << fd);
                break;
            }
        } else if (retVal < 0) {
            LOG4CPLUS_ERROR(_DB_LOGGER_, "select() error: " << strerror(errno));
            break;
        } else {
            LOG4CPLUS_ERROR(_DB_LOGGER_, "select() timeout");
            // TODO cancel the query to free resource??

            // In case network issue, PQcancel will also block until timeout in 45s
            // if (cancelRequest()) {
            //     LOG4CPLUS_DEBUG(_DB_LOGGER_, "Success to cancel query");
            // }
            break;
        }
    }
    #endif

    if (!isResultReady) {
        LOG4CPLUS_ERROR(_DB_LOGGER_, "Fail to get query result");
        return false;
    }

    LOG4CPLUS_DEBUG(_DB_LOGGER_, "Start to get query result");

    PQclear(m_result);
    m_result = NULL;
    PGresult* tmpResult = NULL;
    // Take the last result as the query result, and clear the other results if exists
    while ((tmpResult = PQgetResult(m_conn)) != NULL) {
        if (m_result != NULL) {
            LOG4CPLUS_DEBUG(_DB_LOGGER_, "PQclear last result");
            PQclear(m_result);
            m_result = NULL;
        }

        int status = PQresultStatus(tmpResult);
        if (status == PGRES_SINGLE_TUPLE || status == PGRES_TUPLES_OK) {
            int rowCnt = PQntuples(tmpResult);
            int colCnt = PQnfields(tmpResult);
            LOG4CPLUS_DEBUG(_DB_LOGGER_, "Query result: rowCnt = " << rowCnt << ", colCnt = " << colCnt);
            m_result = tmpResult;
        } else {
            // even PQresultStatus indicates a fatal error, PQgetResult should be called and result to be cleared
            LOG4CPLUS_ERROR(_DB_LOGGER_, "PQresultStatus(), get result fail: " << status);
            PQclear(tmpResult);
        }
    }

    return (m_result != NULL) ? true : false;
}

bool DbPgManager::isDbAlive() {
    string conninfo("hostaddr=");
    conninfo.append(m_dbIP);

    conninfo.append(" port=");
    ostringstream port;
    port << m_dbPort;
    conninfo.append(port.str());   

    conninfo.append(" connect_timeout=");
    ostringstream t;
    t << m_connectTimeout;
    conninfo.append(t.str()); 

    LOG4CPLUS_DEBUG(_DB_LOGGER_, "DbPgManager::isDbAlive, conninfo = " << conninfo);

    if (PQping(conninfo.c_str()) != PQPING_OK) {
        LOG4CPLUS_ERROR(_DB_LOGGER_, "PQping DB failed");
        return false;
    }

    LOG4CPLUS_DEBUG(_DB_LOGGER_, "DB is alive");
    return true;
}

bool DbPgManager::cancelRequest() {
    LOG4CPLUS_DEBUG(_DB_LOGGER_, "DbPgManager::cancelRequest()");

    PGcancel * cancelObj = PQgetCancel(m_conn);
    if (cancelObj == NULL) {
        LOG4CPLUS_WARN(_DB_LOGGER_, "PQgetCancel() is NULL");
        return false;
    }

    char errbuf[256];
    int retVal = PQcancel(cancelObj, errbuf, sizeof(errbuf));

    if (1 == retVal) {
        LOG4CPLUS_DEBUG(_DB_LOGGER_, "PQcancel() success");
    } else {
        LOG4CPLUS_ERROR(_DB_LOGGER_, "PQcancel error: " << errbuf);
    }

    PQfreeCancel(cancelObj);

    return (retVal == 1) ? true : false;
}