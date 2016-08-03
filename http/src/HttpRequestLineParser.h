/*
 * HttpRequestLineParser.h
 *
 *  Created on: July 11, 2016
 *      Author: z.j
 */

#ifndef HTTP_REQUEST_LINE_PARSER_H
#define HTTP_REQUEST_LINE_PARSER_H

#include <string>

namespace http {

    class HttpRequest;

    class HttpRequestLineParser {
    public:
        HttpRequestLineParser();

        bool parse(unsigned char theChar);

        const std::string& getMethod() const;

        const std::string& getUrl() const;

        const std::string& getVersion() const;

    private:

        bool isTextChar(int theChar);

        typedef enum
        {
            IdleE,
            ParseMethodE,
            ParseUrlE,
            ParseVersionE,
            CrParsedE
        } State;

        State m_state;
        std::string m_method;
        std::string m_url;
        std::string m_version;
        bool m_verifyIfHttp;
        bool m_checkHttpProtocol;
    };

    // --------------------------------------------
    inline const std::string& HttpRequestLineParser::getMethod() const {
        return m_method;
    }

  
    // --------------------------------------------
    inline const std::string& HttpRequestLineParser::getUrl() const {
        return m_url;
    }  

    // --------------------------------------------
    inline const std::string& HttpRequestLineParser::getVersion() const {
        return m_version;
    }  
}

#endif
