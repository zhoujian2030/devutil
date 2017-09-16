/*
 * Logger.h
 *
 *  Created on: May 24, 2017
 *      Author: j.zhou
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdarg.h>
#include <string>

namespace cm {

    typedef enum {
        TRACE = 0,
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        FATAL
    } LogLevel;

    class Logger {
    public:
        Logger(std::string loggerName);
        virtual ~Logger();

        void trace(const char* fmt, ...);
        void debug(const char* fmt, ...);
        void info(const char* fmt, ...);
        void warning(const char* fmt, ...);
        void error(const char* fmt, ...);
        void fatal(const char* fmt, ...);

        void setLogLevel(int logLevel);

    private:     
        // static Logger* getInstance();
        // static Logger* m_theInstance;

        void log(std::string level, va_list ap);

        bool isTraceEnabled() const;
        bool isDebugEnabled() const;
        bool isInfoEnabled() const;
        bool isWarningEnabled() const;
        bool isErrorEnabled() const;
        bool isFatalEnabled() const;

        int m_level;
        std::string m_loggerName;
    };

    //------------------------------------
    void Logger::trace(const char* fmt, ...) {
        if (isTraceEnabled()) {
            va_list ap;
            va_start(ap, fmt);
            this->log("TRACE", ap);
            va_end(ap);
        }
    }

    //------------------------------------
    void Logger::debug(const char* fmt, ...) {
        if (isDebugEnabled()) {
            va_list ap;
            va_start(ap, fmt);
            this->log("DEBUG", ap);
            va_end(ap);
        }
    }

    //------------------------------------
    void Logger::info(const char* fmt, ...) {
        if (isInfoEnabled()) {
            va_list ap;
            va_start(ap, fmt);
            this->log("INFO", ap);
            va_end(ap);
        }
    }

    //------------------------------------
    void Logger::error(const char* fmt, ...) {
        if (isErrorEnabled()) {
            va_list ap;
            va_start(ap, fmt);
            this->log("ERROR", ap);
            va_end(ap);
        }
    }

    //------------------------------------
    void Logger::fatal(const char* fmt, ...) {
        if (isFatalEnabled()) {
            va_list ap;
            va_start(ap, fmt);
            this->log("FATAL", ap);
            va_end(ap);
        }
    }

    //------------------------------------
    void Logger::setLogLevel(int logLevel) {
        this->m_level = logLevel;
    }

    //------------------------------------
    bool Logger::isTraceEnabled() const {
        return (m_level <= TRACE);
    }

    //------------------------------------
    bool Logger::isDebugEnabled() const {
        return (m_level <= DEBUG);
    }

    //------------------------------------
    bool Logger::isInfoEnabled() const {
        return (m_level <= INFO);
    }

    //------------------------------------
    bool Logger::isWarningEnabled() const {
        return (m_level <= WARNING);
    }

    //------------------------------------
    bool Logger::isErrorEnabled() const {
        return (m_level <= ERROR);
    }

    //------------------------------------
    bool Logger::isFatalEnabled() const {
        return (m_level <= FATAL);
    }
}


#endif

