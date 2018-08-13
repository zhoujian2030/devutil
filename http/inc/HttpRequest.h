/*
 * HttpRequest.h
 *
 *  Created on: July 7, 2016
 *      Author: z.j
 */

#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <string>
#include <map>

namespace http {

    class HttpRequest {
    public:

        typedef std::map<std::string, std::string> HeaderMap;

        HttpRequest(unsigned long connId);
        HttpRequest(std::string method, std::string url);
        ~HttpRequest();

        unsigned long getConnId() const;

        void setHeader(std::string name, std::string value);
        std::string getHeader(std::string name);

        void setMethod(std::string method);
        std::string getMethod() const;

        void setUrl(std::string url);
        std::string getUrl() const;

        void setVersion(std::string version);
        std::string getVersion() const;

        const HeaderMap & getHeaders() const;

        void setBodyStr(char* body, int bodyLength);
        const std::string & getBodyStr() const;

        void show();

        void setParseError();

        // Only for test
        void setData(char* buff, int numOfBytes);
        std::string m_data;

    private:
        unsigned long m_connId;

        std::string m_method;
        std::string m_url;
        std::string m_version;

        HeaderMap m_headerMap;

        std::string m_bodyStr;

        bool m_isParseError;
    };

    // ------------------------------------------
    inline unsigned long HttpRequest::getConnId() const {
        return m_connId;
    }

    // ------------------------------------------
    inline void HttpRequest::setMethod(std::string method) {
        this->m_method = method;
    }

    // ------------------------------------------
    inline std::string HttpRequest::getMethod() const {
        return this->m_method;
    }

    // ------------------------------------------
    inline void HttpRequest::setUrl(std::string url) {
        this->m_url = url;
    }    

    // ------------------------------------------
    inline std::string HttpRequest::getUrl() const {
        return this->m_url;
    }

    // ------------------------------------------
    inline void HttpRequest::setVersion(std::string version) {
        this->m_version = version;
    }

    // ------------------------------------------
    inline std::string HttpRequest::getVersion() const {
        return this->m_version;
    }

    // ------------------------------------------
    inline void HttpRequest::setBodyStr(char* body, int bodyLength) {
        m_bodyStr.clear();
        m_bodyStr.append(body, bodyLength);
    }

    // ------------------------------------------
    inline const std::string & HttpRequest::getBodyStr() const {
        return m_bodyStr;
    }

    // ------------------------------------------
    inline void HttpRequest::setParseError() {
        this->m_isParseError = true;
    }
    
}

#endif
